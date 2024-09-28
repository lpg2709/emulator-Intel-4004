#ifndef A_TOKEN_H
#define A_TOKEN_H

#include <stdint.h>
#include "opcode.h"

#define X(code, value, name, words) TOKEN_TYPE_OP_##code,
enum token_type {
	TOKEN_TYPE_UNKNOW,
	TOKEN_TYPE_LABEL,
	TOKEN_TYPE_LABEL_DECLARATION, // End with comma and the first 3 chars need to be unique.
	TOKEN_TYPE_NUMBER,
	TOKEN_TYPE_REGISTER,          // R1 R2 ... Rnscanne
	TOKEN_TYPE_REGISTER_PAIR,     // P0 P1 ... Pn
	TOKEN_TYPE_PC_MOD,            // 3.1.5 OPERAND FIELD - Item 2
	TOKEN_TYPE_ADD,               // 3.1.5 OPERAND FIELD - Item 5
	TOKEN_TYPE_SUB,               // 3.1.5 OPERAND FIELD - Item 5
	TOKEN_TYPE_EOF,
	X_OPCODE
	TOKEN_TYPE_LEN
};
#undef X

typedef struct sct_token token;

struct sct_token {
	enum token_type type;
	uint32_t line;
	char *lex;
	uint32_t lex_size;
};

token token_new(enum token_type t, uint16_t l, char *lex, uint32_t lex_size);

#if DEBUG
void print_token(token *t);
#endif

#endif
