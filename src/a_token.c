#include "./a_token.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

token token_new(enum token_type t, uint16_t l, uint8_t ib, char *lex, uint32_t lex_size){
	token token;
	token.type = t;
	token.line = l;
	token.in_bytes = ib;
	token.lex = lex;
	token.lex_size = lex_size;
	return token;
}

void token_delete(token *t) {
	free(t);
}

void print_token(token *t) {
	printf("Type: ");
	switch(t->type) {
		case TOKEN_TYPE_UNKNOW: printf("UNKNOW"); break;
		case TOKEN_TYPE_TEXT: printf("TEXT"); break;
		case TOKEN_TYPE_LABEL: printf("LABEL"); break;
		case TOKEN_TYPE_OPCODE:printf("OPCODE"); break;
		case TOKEN_TYPE_NUMBER: printf("NUMBER"); break;
		case TOKEN_TYPE_REGISTER: printf("REGISTER"); break;
		case TOKEN_TYPE_REGISTER_PAIR: printf("REGISTER_PAIR"); break;
		case TOKEN_TYPE_EOF: printf("EOF"); break;
		case TOKEN_TYPE_LEN: printf("LEN"); break;
	}
	printf(" Line: %" PRIu32 "  InByte: %" PRIu8 "  Lexame: %.*s\n",
			t->line, t->in_bytes, t->lex_size, t->lex);
}

