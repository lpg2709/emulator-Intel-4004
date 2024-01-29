#ifndef A_TOKEN_H
#define A_TOKEN_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

enum token_type {
	OPCODE, OPERAND, NUMBER, HEX, COMMENT,
	T_EOF,
	TOKEN_TYPE_LEN
};

typedef struct sct_token token;

struct sct_token {
	enum token_type type;
	uint32_t line;
	uint8_t in_bytes;
	char lexeme[5];
};

token new_token(enum token_type t, uint16_t l, uint8_t ib, char le[5]);
void print_token(token t);

#endif
