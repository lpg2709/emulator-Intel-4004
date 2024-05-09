#ifndef A_TOKEN_H
#define A_TOKEN_H

#include <stdint.h>

enum token_type {
	TOKEN_TYPE_UNKNOW,
	TOKEN_TYPE_TEXT,  // Everythin, can change after scanner
	TOKEN_TYPE_LABEL, // End with comma and the first 3 chars need to be unique.
	TOKEN_TYPE_NUMBER,
	TOKEN_TYPE_REGISTER,      // R1 R2 ... Rnscanne
	TOKEN_TYPE_REGISTER_PAIR, // P0 P1 ... Pn
	TOKEN_TYPE_OP_NOP,
	TOKEN_TYPE_OP_JCN,
	TOKEN_TYPE_OP_FIM,
	TOKEN_TYPE_OP_FIN,
	TOKEN_TYPE_OP_JIN,
	TOKEN_TYPE_OP_JUN,
	TOKEN_TYPE_OP_JMS,
	TOKEN_TYPE_OP_ISZ,
	TOKEN_TYPE_OP_ADD,
	TOKEN_TYPE_OP_SUB,
	TOKEN_TYPE_OP_LD,
	TOKEN_TYPE_OP_XCH,
	TOKEN_TYPE_OP_BBL,
	TOKEN_TYPE_OP_LDM,
	TOKEN_TYPE_OP_CLB,
	TOKEN_TYPE_OP_CLC,
	TOKEN_TYPE_OP_IAC,
	TOKEN_TYPE_OP_CMC,
	TOKEN_TYPE_OP_CMM,
	TOKEN_TYPE_OP_RAL,
	TOKEN_TYPE_OP_RAR,
	TOKEN_TYPE_OP_TCC,
	TOKEN_TYPE_OP_DAC,
	TOKEN_TYPE_OP_TCS,
	TOKEN_TYPE_OP_STC,
	TOKEN_TYPE_OP_DAA,
	TOKEN_TYPE_OP_KBP,
	TOKEN_TYPE_OP_DCL,
	TOKEN_TYPE_OP_SRC,
	TOKEN_TYPE_OP_WRM,
	TOKEN_TYPE_OP_WMP,
	TOKEN_TYPE_OP_WRR,
	TOKEN_TYPE_OP_WPM,
	TOKEN_TYPE_OP_WR0,
	TOKEN_TYPE_OP_WR1,
	TOKEN_TYPE_OP_WR2,
	TOKEN_TYPE_OP_WR3,
	TOKEN_TYPE_OP_SMB,
	TOKEN_TYPE_OP_RDM,
	TOKEN_TYPE_OP_RDR,
	TOKEN_TYPE_OP_ADM,
	TOKEN_TYPE_OP_RD0,
	TOKEN_TYPE_OP_RD1,
	TOKEN_TYPE_OP_RD2,
	TOKEN_TYPE_OP_RD3,
	TOKEN_TYPE_EOF,
	TOKEN_TYPE_LEN
};

typedef struct sct_token token;

struct sct_token {
	enum token_type type;
	uint32_t line;
	uint8_t in_bytes;
	char *lex;
	uint32_t lex_size;
};

token token_new(enum token_type t, uint16_t l, uint8_t ib, char *lex, uint32_t lex_size);
void token_delete(token *t);
void print_token(token *t);

#endif
