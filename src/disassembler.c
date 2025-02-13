#include "disassembler.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <strings.h>

#include "error.h"
#include "opcode.h"
#include "files.h"

#define X(code, value, name, words) _##code,
typedef enum {
	X_OPCODE
} _opcode_index;
#undef X

typedef struct {
	char *buff;
	uint32_t size;
	uint32_t capacity;
} _output_buffer_t;

#define X(code, value, name, words) words,
const uint8_t bytes_ocupped[] = {
	X_OPCODE
};
#undef X

static bool _output_buff_append(_output_buffer_t *b, char *s) {
	if(b->capacity == 0) {
		b->size = 0;
		b->capacity = 256;
		b->buff = (char*) calloc(b->capacity, sizeof(char));
		if(b->buff == NULL) {
			return false;
		}
	}
	uint16_t s_size = strlen(s);
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

static uint8_t _nop(char op[20]) {
	memcpy(op, "NOP\n", sizeof(char) * 4);
	return bytes_ocupped[NOP];
}

static uint8_t _jcn(char op[20], uint8_t *bytes) {
	uint8_t cn = (bytes[0] & 0x0F);
	uint8_t addr = bytes[1];
	sprintf(op, "%s %d, %d\n", "JCN", cn, addr);
	return bytes_ocupped[_JCN];
}

static uint8_t _fim(char op[20], uint8_t *bytes) {
	uint8_t IR_pair = (bytes[0] & 0x0E) >> 1;
	uint8_t rom_data = (bytes[1] & 0xFF);
	sprintf(op, "%s %dP, %d\n", "FIM", IR_pair, rom_data);
	return bytes_ocupped[_FIM];
}

static uint8_t _fin(char op[20], uint8_t *bytes) {
	uint8_t IR_pair = (bytes[0] & 0xE) >> 1;
	sprintf(op, "%s %dP\n", "FIN", IR_pair);
	return bytes_ocupped[_FIN];
}

static uint8_t _jin(char op[20], uint8_t *bytes) {
	uint8_t IR_pair = (bytes[0] & 0xE) >> 1;
	sprintf(op, "%s %dP\n", "JIN", IR_pair);
	return bytes_ocupped[_JIN];
}

static uint8_t _jun(char op[20], uint8_t *bytes) {
	uint16_t rom_data = (bytes[0] << 8 | bytes[1]) & 0x0FFF;
	sprintf(op, "%s %d\n", "JUN", rom_data);
	return bytes_ocupped[_JUN];
}

static uint8_t _jms(char op[20], uint8_t *bytes) {
	uint16_t rom_data = (bytes[0] << 8 | bytes[1]) & 0x0FFF;
	sprintf(op, "%s %d\n", "JMS", rom_data);
	return bytes_ocupped[_JMS];
}

static uint8_t _inc(char op[20], uint8_t *bytes) {
	uint8_t reg = bytes[0] & 0x0F;
	sprintf(op, "%s %d\n", "INC", reg);
	return bytes_ocupped[_INC];
}

static uint8_t _isz(char op[20], uint8_t *bytes) {
	uint8_t IR_addrs = (bytes[0] & 0x0F);
	uint8_t rom_addrs = (bytes[1] & 0xFF);
	sprintf(op, "%s %d, %d\n", "ISZ", IR_addrs, rom_addrs);
	return bytes_ocupped[_ISZ];
}

static uint8_t _add(char op[20], uint8_t *bytes) {
	uint8_t reg = (bytes[0] & 0x0F);
	sprintf(op, "%s %d\n", "ADD", reg);
	return bytes_ocupped[_ADD];
}

static uint8_t _sub(char op[20], uint8_t *bytes) {
	uint8_t reg = (bytes[0] & 0x0F);
	sprintf(op, "%s %d\n", "SUB", reg);
	return bytes_ocupped[_SUB];
}

static uint8_t _ld (char op[20], uint8_t *bytes) {
	uint8_t reg = (bytes[0] & 0x0F);
	sprintf(op, "%s %d\n", "LD", reg);
	return bytes_ocupped[_LD];
}

static uint8_t _xch(char op[20], uint8_t *bytes) {
	uint8_t reg = (bytes[0] & 0x0F);
	sprintf(op, "%s %d\n", "XCH", reg);
	return bytes_ocupped[_XCH];
}

static uint8_t _bbl(char op[20], uint8_t *bytes) {
	uint8_t data = (bytes[0] & 0x0F);
	sprintf(op, "%s %d\n", "BBL", data);
	return bytes_ocupped[_BBL];
}

static uint8_t _ldm(char op[20], uint8_t *bytes) {
	uint8_t data = (bytes[0] & 0x0F);
	sprintf(op, "%s %d\n", "LDM", data);
	return bytes_ocupped[_LDM];
}

static uint8_t _clb(char op[20]) {
	memcpy(op, "CLB\n", sizeof(char) * 4);
	return bytes_ocupped[_CLB];
}

static uint8_t _clc(char op[20]) {
	memcpy(op, "CLC\n", sizeof(char) * 4);
	return bytes_ocupped[_CLC];
}

static uint8_t _iac(char op[20]) {
	memcpy(op, "IAC\n", sizeof(char) * 4);
	return bytes_ocupped[_IAC];
}

static uint8_t _cmc(char op[20]) {
	memcpy(op, "CMC\n", sizeof(char) * 4);
	return bytes_ocupped[_CMC];
}

static uint8_t _cma(char op[20]) {
	memcpy(op, "CMA\n", sizeof(char) * 4);
	return bytes_ocupped[_CMA];
}

static uint8_t _ral(char op[20]) {
	memcpy(op, "RAL\n", sizeof(char) * 4);
	return bytes_ocupped[_RAL];
}

static uint8_t _rar(char op[20]) {
	memcpy(op, "_RAR\n", sizeof(char) * 4);
	return bytes_ocupped[_RAR];
}

static uint8_t _tcc(char op[20]) {
	memcpy(op, "TCC\n", sizeof(char) * 4);
	return bytes_ocupped[_TCC];
}

static uint8_t _dac(char op[20]) {
	memcpy(op, "DAC\n", sizeof(char) * 4);
	return bytes_ocupped[_DAC];
}

static uint8_t _tcs(char op[20]) {
	memcpy(op, "TCS\n", sizeof(char) * 4);
	return bytes_ocupped[_TCS];
}

static uint8_t _stc(char op[20]) {
	memcpy(op, "STC\n", sizeof(char) * 4);
	return bytes_ocupped[_STC];
}

static uint8_t _daa(char op[20]) {
	memcpy(op, "DAA\n", sizeof(char) * 4);
	return bytes_ocupped[_DAA];
}

static uint8_t _kbp(char op[20]) {
	memcpy(op, "KBP\n", sizeof(char) * 4);
	return bytes_ocupped[_KBP];
}

static uint8_t _dcl(char op[20]) {
	memcpy(op, "DCL\n", sizeof(char) * 4);
	return bytes_ocupped[_DCL];
}

static uint8_t _src(char op[20], uint8_t *bytes) {
	uint8_t IR_pair = (bytes[0] & 0xE) >> 1;
	sprintf(op, "%s %dP\n", "SRC", IR_pair);
	return bytes_ocupped[SRC];
}

static uint8_t _wrm(char op[20]) {
	memcpy(op, "WRM\n", sizeof(char) * 4);
	return bytes_ocupped[_WRM];
}

static uint8_t _wmp(char op[20]) {
	memcpy(op, "WMP\n", sizeof(char) * 4);
	return bytes_ocupped[_WMP];
}

static uint8_t _wrr(char op[20]) {
	memcpy(op, "WRR\n", sizeof(char) * 4);
	return bytes_ocupped[_WRR];
}

static uint8_t _wr0(char op[20]) {
	memcpy(op, "WR0\n", sizeof(char) * 4);
	return bytes_ocupped[_WR0];
}

static uint8_t _wr1(char op[20]) {
	memcpy(op, "WR1\n", sizeof(char) * 4);
	return bytes_ocupped[_WR1];
}

static uint8_t _wr2(char op[20]) {
	memcpy(op, "WR2\n", sizeof(char) * 4);
	return bytes_ocupped[_WR2];
}

static uint8_t _wr3(char op[20]) {
	memcpy(op, "WR3\n", sizeof(char) * 4);
	return bytes_ocupped[_WR3];
}

static uint8_t _sbm(char op[20]) {
	memcpy(op, "SBM\n", sizeof(char) * 4);
	return bytes_ocupped[_SBM];
}

static uint8_t _rdm(char op[20]) {
	memcpy(op, "RDM\n", sizeof(char) * 4);
	return bytes_ocupped[_RDM];
}

static uint8_t _rdr(char op[20]) {
	memcpy(op, "RDR\n", sizeof(char) * 4);
	return bytes_ocupped[_RDR];
}

static uint8_t _adm(char op[20]) {
	memcpy(op, "ADM\n", sizeof(char) * 4);
	return bytes_ocupped[_ADM];
}

static uint8_t _rd0(char op[20]) {
	memcpy(op, "RD0\n", sizeof(char) * 4);
	return bytes_ocupped[_RD0];
}

static uint8_t _rd1(char op[20]) {
	memcpy(op, "RD1\n", sizeof(char) * 4);
	return bytes_ocupped[_RD1];
}

static uint8_t _rd2(char op[20]) {
	memcpy(op, "RD2\n", sizeof(char) * 4);
	return bytes_ocupped[_RD2];
}

static uint8_t _rd3(char op[20]) {
	memcpy(op, "RD3\n", sizeof(char) * 4);
	return bytes_ocupped[_RD3];
}

bool disassembler_get_opcode(uint8_t *bytes, char op[20], uint8_t *word_size) {
//	_byte_op_t op_object = { 0 };
	switch(bytes[0] & 0xF0) {
		case 0x00: *word_size = _nop(op); break;
		case 0x10: *word_size = _jcn(op, bytes); break;
		case 0x20:
			if((bytes[0] & 0x0F) % 2 == 0){
				*word_size = _fim(op, bytes);
			}else{
				*word_size = _src(op, bytes);
			}
			break;
		case 0x30:
			switch(bytes[0] & 0x01){
				case 0x00: *word_size = _fin(op, bytes); break;
				case 0x01: *word_size = _jin(op, bytes); break;
			}
			break;
		case 0x40: *word_size = _jun(op, bytes); break;
		case 0x50: *word_size = _jms(op, bytes); break;
		case 0x60: *word_size = _inc(op, bytes); break;
		case 0x70: *word_size = _isz(op, bytes); break;
		case 0x80: *word_size = _add(op, bytes); break;
		case 0x90: *word_size = _sub(op, bytes); break;
		case 0xA0: *word_size = _ld(op, bytes); break;
		case 0xB0: *word_size = _xch(op, bytes); break;
		case 0xC0: *word_size = _bbl(op, bytes); break;
		case 0xD0: *word_size = _ldm(op, bytes); break;
		case 0xF0:
			switch(bytes[0] & 0x0F){
				case 0x0: *word_size = _clb(op); break;
				case 0x1: *word_size = _clc(op); break;
				case 0x2: *word_size = _iac(op); break;
				case 0x3: *word_size = _cmc(op); break;
				case 0x4: *word_size = _cma(op); break;
				case 0x5: *word_size = _ral(op); break;
				case 0x6: *word_size = _rar(op); break;
				case 0x7: *word_size = _tcc(op); break;
				case 0x8: *word_size = _dac(op); break;
				case 0x9: *word_size = _tcs(op); break;
				case 0xA: *word_size = _stc(op); break;
				case 0xB: *word_size = _daa(op); break;
				case 0xC: *word_size = _kbp(op); break;
				case 0xD: *word_size = _dcl(op); break;
			}
			break;
		case 0xE0:
			switch(bytes[0] & 0x0F){
				case 0x0: *word_size = _wrm(op); break;
				case 0x1: *word_size = _wmp(op); break;
				case 0x2: *word_size = _wrr(op); break;
				case 0x4: *word_size = _wr0(op); break;
				case 0x5: *word_size = _wr1(op); break;
				case 0x6: *word_size = _wr2(op); break;
				case 0x7: *word_size = _wr3(op); break;
				case 0x8: *word_size = _sbm(op); break;
				case 0x9: *word_size = _rdm(op); break;
				case 0xA: *word_size = _rdr(op); break;
				case 0xB: *word_size = _adm(op); break;
				case 0xC: *word_size = _rd0(op); break;
				case 0xD: *word_size = _rd1(op); break;
				case 0xE: *word_size = _rd2(op); break;
				case 0xF: *word_size = _rd3(op); break;
			}
		break;
	}

	return true;
}

Error disassembler(Options *opt) {
	Error error = ERROR_NOT;
	uint32_t f_size;
	const char* rom = (char *) b_read_file(opt->in_file_path, &f_size);
	_output_buffer_t output_buff = { 0 };

	uint8_t word_size = 0;
	for(uint32_t i = 0; i < f_size; i += word_size) {
		char op[20] = { 0 };
		if(disassembler_get_opcode((uint8_t*) &rom[i], op, &word_size)) {
			_output_buff_append(&output_buff, op);
		} else {
			fprintf(stderr, "Fail to disassembly byte %.2X!\n", rom[i]);
			error = ERROR_DIS_UNKNOW_BYTE;
			goto end;
		}
	}

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

