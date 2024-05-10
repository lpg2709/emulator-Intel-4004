#ifndef A_TOKEN_H
#define A_TOKEN_H

#include <stdint.h>
#include "opcode.h"

#define X(code, value, name, operands) TOKEN_TYPE_OP_##code,
enum token_type {
	TOKEN_TYPE_UNKNOW,
	TOKEN_TYPE_TEXT,  // Everythin, can change after scanner
	TOKEN_TYPE_LABEL, // End with comma and the first 3 chars need to be unique.
	TOKEN_TYPE_NUMBER,
	TOKEN_TYPE_REGISTER,      // R1 R2 ... Rnscanne
	TOKEN_TYPE_REGISTER_PAIR, // P0 P1 ... Pn
	TOKEN_TYPE_EOF,
	X_OPCODE
	TOKEN_TYPE_LEN
};
#undef X

typedef struct sct_token token;

struct sct_token {
	enum token_type type;
	uint32_t line;
	uint8_t in_bytes;
	char *lex;
	uint32_t lex_size;
};

token token_new(enum token_type t, uint16_t l, uint8_t ib, char *lex, uint32_t lex_size);

#ifndef DEBUG
void print_token(token *t);
#endif

#endif
