#include "./assembler.h"

#include <stdlib.h>
#include <string.h>

#include "a_token.h"
#include "a_scanner.h"
#include "files.h"
#include "error.h"
#include "4004_chip.h"
#include "a_hash_table.h"

#if DEBUG
static void d_print_scanned_tokens(scanner *s) {
	int i = 0;
	printf("==================================== TOKENS ===================================\n");
	do {
		printf("[%05d] ", i);
		print_token(&s->tokens[i++]);
	} while (s->tokens[i].type != TOKEN_TYPE_EOF && i < MAX_TOKENS);
	if(i == MAX_TOKENS)
		printf(">> MAX_TOKENS reached!\n");
}
#endif

static void parse_label_declaration(scanner *s, HashTable *labels) {
	uint16_t out_pos = 0;
	token *t = &s->tokens[0];
	while(t->type != TOKEN_TYPE_EOF) {
		switch(t->type) {
			case TOKEN_TYPE_OP_NOP:
			case TOKEN_TYPE_OP_CLB:
			case TOKEN_TYPE_OP_CLC:
			case TOKEN_TYPE_OP_IAC:
			case TOKEN_TYPE_OP_CMC:
			case TOKEN_TYPE_OP_RAL:
			case TOKEN_TYPE_OP_RAR:
			case TOKEN_TYPE_OP_TCC:
			case TOKEN_TYPE_OP_DAC:
			case TOKEN_TYPE_OP_TCS:
			case TOKEN_TYPE_OP_STC:
			case TOKEN_TYPE_OP_DAA:
			case TOKEN_TYPE_OP_KBP:
			case TOKEN_TYPE_OP_DCL:
			case TOKEN_TYPE_OP_WRM:
			case TOKEN_TYPE_OP_WMP:
			case TOKEN_TYPE_OP_WRR:
			case TOKEN_TYPE_OP_WPM:
			case TOKEN_TYPE_OP_WR0:
			case TOKEN_TYPE_OP_WR1:
			case TOKEN_TYPE_OP_WR2:
			case TOKEN_TYPE_OP_WR3:
			case TOKEN_TYPE_OP_SMB:
			case TOKEN_TYPE_OP_RDM:
			case TOKEN_TYPE_OP_RDR:
			case TOKEN_TYPE_OP_ADM:
			case TOKEN_TYPE_OP_RD0:
			case TOKEN_TYPE_OP_RD1:
			case TOKEN_TYPE_OP_RD2:
			case TOKEN_TYPE_OP_RD3:
			case TOKEN_TYPE_NUMBER:
			case TOKEN_TYPE_LABEL:
				out_pos++;
				t++;
				break;
			case TOKEN_TYPE_OP_JCN:
			case TOKEN_TYPE_OP_FIM:
			case TOKEN_TYPE_OP_FIN:
			case TOKEN_TYPE_OP_JIN:
			case TOKEN_TYPE_OP_INC:
			case TOKEN_TYPE_OP_ADD:
			case TOKEN_TYPE_OP_SUB:
			case TOKEN_TYPE_OP_LD:
			case TOKEN_TYPE_OP_XCH:
			case TOKEN_TYPE_OP_BBL:
			case TOKEN_TYPE_OP_LDM:
			case TOKEN_TYPE_OP_SRC:
				out_pos++;
				t += 2;
				break;
			case TOKEN_TYPE_OP_JUN:
			case TOKEN_TYPE_OP_JMS:
			case TOKEN_TYPE_OP_ISZ:
				out_pos += 2;
				t += 2;
				break;
			case TOKEN_TYPE_LABEL_DECLARATION: {
				char label[4] = { 0 };
				memcpy(label, t->lex, sizeof(char) * 3);
				if(t[1].type == TOKEN_TYPE_NUMBER) {
					char num[10] = { 0 };
					sprintf(num, "%.*s", t[1].lex_size, t[1].lex);
					hash_table_set(labels, label, (uint16_t) atoi(num));
					t+=2;
				} else {
					hash_table_set(labels, label, out_pos);
					t++;
				}
				break;
			}
			default:
				fprintf(stderr, "[Label Parser] Opcode (%.*s) invalid\n", t->lex_size, t->lex);
				t++;
				break;
		}
	}
}

static bool tok_is(token t, enum token_type ex) {
	return t.type == ex;
}

static uint8_t token_to_byte(token t, HashTable *labels) {
	uint8_t in_bytes = 0;
	switch(t.type) {
		case TOKEN_TYPE_REGISTER_PAIR:  {
			in_bytes = ((uint8_t) (t.lex[0] - '0')) * 2;
			if(in_bytes > 16) // TODO: Invalid register pair
				in_bytes = 0;
			break;
		}
		case TOKEN_TYPE_REGISTER:  {
			in_bytes = (uint8_t) (t.lex[1] - '0');
			if(in_bytes > 16) // TODO: Invalid register
				in_bytes = 0;
			break;
		}
		case TOKEN_TYPE_NUMBER:  {
			char num[10] = { 0 };
			sprintf(num, "%.*s", t.lex_size, t.lex);
			in_bytes = (uint8_t) atoi(num);
			break;
		}
		case TOKEN_TYPE_LABEL:  {
			uint16_t in_b = 0;
			hash_table_get(labels, t.lex, &in_b);
			in_bytes = (uint8_t) in_b;
			break;
		}
		default: goto end;
	}

end:
	return in_bytes;
}

static uint16_t token_to_2byte(token t, HashTable *labels) {
	uint16_t in_bytes = 0;
	switch(t.type) {
		case TOKEN_TYPE_NUMBER:  {
			char num[10] = { 0 };
			sprintf(num, "%.*s", t.lex_size, t.lex);
			in_bytes = (uint8_t) atoi(num);
			break;
		}
		case TOKEN_TYPE_LABEL: {
			hash_table_get(labels, t.lex, &in_bytes);
			break;
		}
		default: goto end;
	}

end:
	return in_bytes;
}

#define GEN_INSTRUCTION_FIXED(opcode) { \
		output[out_pos++] = (uint8_t) ((opcode) & 0xFF); \
		t++; \
		break; \
	}

#define GEN_INSTRUCTION_ONE_PARAM(opcode, param_type) { \
		uint8_t instruction = (uint8_t) ((opcode) & 0xFF) << 4; \
		if(! ( tok_is(t[1], (param_type)) || tok_is(t[1], TOKEN_TYPE_LABEL) )) { \
			fprintf(stderr, "Unexpected token on convertion of ("#opcode")\n"); \
			break; \
		}\
		instruction |= token_to_byte(t[1], labels);\
		output[out_pos++] = instruction;\
		t += 2;\
		break;\
	}

#define GEN_INSTRUCTION_TWO_PARAM(opcode, param1_type, param2_type) { \
		uint8_t instruction = (uint8_t) ((opcode) & 0xFF) << 4; \
		if(! ( tok_is(t[1], (param1_type)) || tok_is(t[1], TOKEN_TYPE_LABEL) )) { \
			fprintf(stderr, "Unexpected token on convertion of ("#opcode") param 1. Token founded "#param1_type"\n"); \
			break; \
		} \
		if(! ( tok_is(t[2], (param2_type)) || tok_is(t[2], TOKEN_TYPE_LABEL) )) { \
			fprintf(stderr, "Unexpected token on convertion of ("#opcode") param 2\n"); \
			break; \
		} \
		instruction |= token_to_byte(t[1], labels); \
		output[out_pos++] = instruction; \
		output[out_pos++] = token_to_byte(t[2], labels); \
		t += 3; \
		break; \
	}

uint16_t parse(char *output, scanner *s, HashTable *labels) {
	uint16_t out_pos = 0;
	token *t = &s->tokens[0];
	while(t->type != TOKEN_TYPE_EOF) {
		// TODO: Handle Unexpected tokens syntax error
		switch(t->type) {
			case TOKEN_TYPE_OP_NOP:
				output[out_pos++] = (uint8_t) NOP;
				t++;
				break;
			case TOKEN_TYPE_OP_JCN: GEN_INSTRUCTION_TWO_PARAM(JCN, TOKEN_TYPE_NUMBER, TOKEN_TYPE_NUMBER);
			case TOKEN_TYPE_OP_FIM: GEN_INSTRUCTION_TWO_PARAM(FIM, TOKEN_TYPE_REGISTER_PAIR, TOKEN_TYPE_NUMBER);
			case TOKEN_TYPE_OP_FIN: {
					uint8_t instruction = (uint8_t) ((FIN) & 0xFF) << 4;
					if(!tok_is(t[1], (TOKEN_TYPE_REGISTER_PAIR))) {
						fprintf(stderr, "Expected a register pair on FIN\n");
						break;
					}
					instruction |= token_to_byte(t[1], labels);
					output[out_pos++] = instruction;
					t += 2;
					break;
				}
			case TOKEN_TYPE_OP_JIN: {
					uint8_t instruction = (uint8_t) ((JIN) & 0xFF) << 4;
					if(!tok_is(t[1], (TOKEN_TYPE_REGISTER_PAIR))) {
						fprintf(stderr, "Expected a register pair on JIN\n");
						break;
					}
					instruction |= (token_to_byte(t[1], labels) | 0b01);
					output[out_pos++] = instruction;
					t += 2;
					break;
				}
			case TOKEN_TYPE_OP_JUN: {
					uint8_t instruction = (uint8_t) JUN << 4;
					if(!( tok_is(t[1], (TOKEN_TYPE_NUMBER)) || tok_is(t[1], (TOKEN_TYPE_LABEL)) ))  {
						fprintf(stderr, "Unexpected token on convertion of (JUN)\n");
						t += 2;
						break;
					}
					uint16_t addres = token_to_2byte(t[1], labels);
					instruction |= (addres & 0xF00);
					output[out_pos++] = instruction;
					output[out_pos++] = (addres & 0xFF);
					t += 2;
					break;
				}
			case TOKEN_TYPE_OP_JMS: {
					uint8_t instruction = (uint8_t) JMS << 4;
					if(!tok_is(t[1], (TOKEN_TYPE_NUMBER))) {
						fprintf(stderr, "Unexpected token on convertion of (JMS)\n");
						break;
					}
					uint16_t addres = token_to_2byte(t[1], labels);
					instruction |= (addres & 0xF00);
					output[out_pos++] = instruction;
					output[out_pos++] = (addres & 0xFF);
					t += 2;
					break;
				}
			case TOKEN_TYPE_OP_INC: GEN_INSTRUCTION_ONE_PARAM(INC, TOKEN_TYPE_REGISTER);
			case TOKEN_TYPE_OP_ISZ: GEN_INSTRUCTION_TWO_PARAM(ISZ, TOKEN_TYPE_REGISTER, TOKEN_TYPE_NUMBER);
			case TOKEN_TYPE_OP_ADD: GEN_INSTRUCTION_ONE_PARAM(ADD, TOKEN_TYPE_REGISTER);
			case TOKEN_TYPE_OP_SUB: GEN_INSTRUCTION_ONE_PARAM(SUB, TOKEN_TYPE_REGISTER);
			case TOKEN_TYPE_OP_LD:  GEN_INSTRUCTION_ONE_PARAM(LD, TOKEN_TYPE_REGISTER);
			case TOKEN_TYPE_OP_XCH: GEN_INSTRUCTION_ONE_PARAM(XCH, TOKEN_TYPE_REGISTER);
			case TOKEN_TYPE_OP_BBL: GEN_INSTRUCTION_ONE_PARAM(BBL, TOKEN_TYPE_NUMBER);
			case TOKEN_TYPE_OP_LDM: GEN_INSTRUCTION_ONE_PARAM(LDM, TOKEN_TYPE_NUMBER);
			case TOKEN_TYPE_OP_CLB: GEN_INSTRUCTION_FIXED(CLB);
			case TOKEN_TYPE_OP_CLC: GEN_INSTRUCTION_FIXED(CLC);
			case TOKEN_TYPE_OP_IAC: GEN_INSTRUCTION_FIXED(IAC);
			case TOKEN_TYPE_OP_CMC: GEN_INSTRUCTION_FIXED(CMC);
			case TOKEN_TYPE_OP_RAL: GEN_INSTRUCTION_FIXED(RAL);
			case TOKEN_TYPE_OP_RAR: GEN_INSTRUCTION_FIXED(RAR);
			case TOKEN_TYPE_OP_TCC: GEN_INSTRUCTION_FIXED(TCC);
			case TOKEN_TYPE_OP_DAC: GEN_INSTRUCTION_FIXED(DAC);
			case TOKEN_TYPE_OP_TCS: GEN_INSTRUCTION_FIXED(TCS);
			case TOKEN_TYPE_OP_STC: GEN_INSTRUCTION_FIXED(STC);
			case TOKEN_TYPE_OP_DAA: GEN_INSTRUCTION_FIXED(DAA);
			case TOKEN_TYPE_OP_KBP: GEN_INSTRUCTION_FIXED(KBP);
			case TOKEN_TYPE_OP_DCL: GEN_INSTRUCTION_FIXED(DCL);
			case TOKEN_TYPE_OP_SRC: {
				uint8_t instruction = (uint8_t) SRC;
				if(!tok_is(t[1], TOKEN_TYPE_REGISTER_PAIR) && !tok_is(t[1], TOKEN_TYPE_LABEL) ) {
					fprintf(stderr, "Unexpected token\n");
				}
				instruction |= 0x1;
				instruction |= token_to_byte(t[1], labels);
				output[out_pos++] = instruction;
				t += 2;
				break;
			}
			case TOKEN_TYPE_OP_WRM: GEN_INSTRUCTION_FIXED(WRM);
			case TOKEN_TYPE_OP_WMP: GEN_INSTRUCTION_FIXED(WMP);
			case TOKEN_TYPE_OP_WRR: GEN_INSTRUCTION_FIXED(WRR);
			case TOKEN_TYPE_OP_WPM: GEN_INSTRUCTION_FIXED(WPM);
			case TOKEN_TYPE_OP_WR0: GEN_INSTRUCTION_FIXED(WR0);
			case TOKEN_TYPE_OP_WR1: GEN_INSTRUCTION_FIXED(WR1);
			case TOKEN_TYPE_OP_WR2: GEN_INSTRUCTION_FIXED(WR2);
			case TOKEN_TYPE_OP_WR3: GEN_INSTRUCTION_FIXED(WR3);
			case TOKEN_TYPE_OP_SMB: GEN_INSTRUCTION_FIXED(SMB);
			case TOKEN_TYPE_OP_RDM: GEN_INSTRUCTION_FIXED(RDM);
			case TOKEN_TYPE_OP_RDR: GEN_INSTRUCTION_FIXED(RDR);
			case TOKEN_TYPE_OP_ADM: GEN_INSTRUCTION_FIXED(ADM);
			case TOKEN_TYPE_OP_RD0: GEN_INSTRUCTION_FIXED(RD0);
			case TOKEN_TYPE_OP_RD1: GEN_INSTRUCTION_FIXED(RD1);
			case TOKEN_TYPE_OP_RD2: GEN_INSTRUCTION_FIXED(RD2);
			case TOKEN_TYPE_OP_RD3: GEN_INSTRUCTION_FIXED(RD3);
			case TOKEN_TYPE_LABEL_DECLARATION: t++; break;
			default:
				fprintf(stderr, "Opcode (%.*s) invalid\n", t->lex_size, t->lex);
				t++;
				break;
		}
	}
	return out_pos;
}
#undef GEN_INSTRUCTION_TWO_PARAM
#undef GEN_INSTRUCTION_ONE_PARAM
#undef GEN_INSTRUCTION_FIXED

Error assembler(Options *opt) {
	long f_size;
	const char* source = read_file(opt->in_file_path, &f_size);
	char output[ROM_MAX_SIZE] = { 0 };
	HashTable labels = { 0 };
	scanner s;

	hash_table_init(&labels);

	scan_tokens(&s, source, f_size);

#if DEBUG
	d_print_scanned_tokens(&s);
#endif

	// First pass getting the lable address
	parse_label_declaration(&s, &labels);
	// Parse tokens to byte code
	uint16_t output_size = parse(output, &s, &labels);

#if DEBUG
	printf("=============================== COMPILED OUTPUT ===============================\n");
	for(int i = 0; i < (int) output_size; i++) {
		printf("%.2X ", output[i] & 0xFF);
		if(i % 8 == 0 && i != 0)
			printf("\n");
	}
	printf("\n");
#endif

	free(s.tokens);
	free((void *)source);
	hash_table_deinit(&labels);

	bool file_status = b_write_file(opt->output_file_path, output, sizeof(char), sizeof(char) * output_size);

	if(!file_status)
		return ERROR_FAIL_WRITE_FILE;

	return ERROR_NOT;
}

