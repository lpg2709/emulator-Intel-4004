#include "disassembler.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>

#include "error.h"
#include "opcode.h"
#include "files.h"

typedef struct {
	char *buff;
	uint32_t size;
	uint32_t capacity;
} _output_buffer_t;

typedef struct {
	uint8_t code;
	char opcode[4];
	uint8_t size;
} _byte_op_t;

static bool _output_buff_append(_output_buffer_t *b, char *s, uint16_t s_size) {
	if(b->capacity == 0) {
		b->size = 0;
		b->capacity = 256;
		b->buff = (char*) calloc(b->capacity, sizeof(char));
		if(b->buff == NULL) {
			return false;
		}
	}
	if(b->size + s_size > b->capacity) {
		b->capacity *= 2;
		b->buff = (char*) realloc(b->buff, b->capacity);
		if(b->buff == NULL) {
			fprintf(stderr, "Fail to realloc memory for output buffer!\n");
			return false;
		}
	}

	memcpy(&b->buff[b->size], s, s_size);
	b->size += s_size;

	return true;
}

#define X(c, value, name, words) { .code = c, .opcode = name, .size = words },
const _byte_op_t lookup_table[] = {
	X_OPCODE
};
#undef X

_byte_op_t new_op_t(Opcode op) {
	const _byte_op_t ret = { 0 };

	if(op <= DCL) {
		for(int i = 0; i < OPCODE_COUNT; i++) {
			if(lookup_table[i].code == (uint8_t) op)
				return lookup_table[i];
		}
	}

	return ret;
}

bool _get_opcode(uint8_t byte, char op[4], uint8_t *word_size) {
	_byte_op_t op_object = { 0 };

	switch(byte & 0xF0){
		case 0x00: op_object = new_op_t(NOP); break;
		case 0x10: op_object = new_op_t(JCN); break;
		case 0x20:
			if((byte & 0x0F) % 2 == 0){
				op_object = new_op_t(FIM);
			}else{
				op_object = new_op_t(SRC);
			}
			break;
		case 0x30:
			switch(byte & 0x0F){
				case 0x00: op_object = new_op_t(FIN); break;
				case 0x01: op_object = new_op_t(JIN); break;
			}
			break;
		case 0x40: op_object = new_op_t(JUN); break;
		case 0x50: op_object = new_op_t(JMS); break;
		case 0x60: op_object = new_op_t(INC); break;
		case 0x70: op_object = new_op_t(ISZ); break;
		case 0x80: op_object = new_op_t(ADD); break;
		case 0x90: op_object = new_op_t(SUB); break;
		case 0xA0: op_object = new_op_t(LD); break;
		case 0xB0: op_object = new_op_t(XCH); break;
		case 0xC0: op_object = new_op_t(BBL); break;
		case 0xD0: op_object = new_op_t(LDM); break;
		case 0xF0:
			switch(byte & 0x0F){
				case 0x0: op_object = new_op_t(CLB); break;
				case 0x1: op_object = new_op_t(CLC); break;
				case 0x2: op_object = new_op_t(IAC); break;
				case 0x3: op_object = new_op_t(CMC); break;
				case 0x4: op_object = new_op_t(CMA); break;
				case 0x5: op_object = new_op_t(RAL); break;
				case 0x6: op_object = new_op_t(RAR); break;
				case 0x7: op_object = new_op_t(TCC); break;
				case 0x8: op_object = new_op_t(DAC); break;
				case 0x9: op_object = new_op_t(TCS); break;
				case 0xA: op_object = new_op_t(STC); break;
				case 0xB: op_object = new_op_t(DAA); break;
				case 0xC: op_object = new_op_t(KBP); break;
				case 0xD: op_object = new_op_t(DCL); break;
			}
			break;
		case 0xE0:
			switch(byte & 0x0F){
				case 0x0: op_object = new_op_t(WRM); break;
				case 0x1: op_object = new_op_t(WMP); break;
				case 0x2: op_object = new_op_t(WRR); break;
				case 0x4: op_object = new_op_t(WR0); break;
				case 0x5: op_object = new_op_t(WR1); break;
				case 0x6: op_object = new_op_t(WR2); break;
				case 0x7: op_object = new_op_t(WR3); break;
				case 0x8: op_object = new_op_t(SMB); break;
				case 0x9: op_object = new_op_t(RDM); break;
				case 0xA: op_object = new_op_t(RDR); break;
				case 0xB: op_object = new_op_t(ADM); break;
				case 0xC: op_object = new_op_t(RD0); break;
				case 0xD: op_object = new_op_t(RD1); break;
				case 0xE: op_object = new_op_t(RD2); break;
				case 0xF: op_object = new_op_t(RD3); break;
			}
		break;
	}

	if(op_object.opcode[0] != 0) {
		memcpy(op, op_object.opcode, sizeof(char) * 4);
		*word_size = op_object.size;
	}

	return false;
}

Error disassembler(Options *opt) {
	Error error = ERROR_NOT;
	uint32_t f_size;
	const char* rom = b_read_file(opt->in_file_path, &f_size);
	_output_buffer_t output_buff = { 0 };

	printf("Size of file: %d\n", f_size);

	uint8_t word_size = 0;
	for(uint32_t i = 0; i < f_size; i += word_size) {
		char op[4] = { 0 };
		_get_opcode(rom[i], op, &word_size);
		printf("%s - %d\n", op, word_size);
	}
	printf("\n");
	goto end;

	if(strcmp(opt->output_file_path, "") == 0){
		fprintf(stdout, "%s", output_buff.buff);
	} else {
		if(!b_write_file(opt->output_file_path, output_buff.buff, sizeof(char), sizeof(char) * output_buff.size)) {
			error =  ERROR_FAIL_WRITE_FILE;
			goto end;
		}
	}

end:
	if(output_buff.buff != NULL)
		free(output_buff.buff);
	free((void *)rom);
	return error;
}

