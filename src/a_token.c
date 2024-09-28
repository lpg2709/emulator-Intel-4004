#include "./a_token.h"

#include <inttypes.h>
#include <stdio.h>

token token_new(enum token_type t, uint16_t l, char *lex, uint32_t lex_size){
	token token;
	token.type = t;
	token.line = l;
	token.lex = lex;
	token.lex_size = lex_size;
	return token;
}

#if DEBUG
void print_token(token *t) {
	printf("Type: ");
#define P(s) printf("%-15s", s);
#define X(code, value, name, words) case TOKEN_TYPE_OP_##code: \
	P((name)); break;
	switch(t->type) {
		case TOKEN_TYPE_UNKNOW: P("UNKNOW"); break;
		case TOKEN_TYPE_LABEL: P("LABEL"); break;
		case TOKEN_TYPE_LABEL_DECLARATION: P("LABEL_DECLARATION"); break;
		case TOKEN_TYPE_NUMBER: P("NUMBER"); break;
		case TOKEN_TYPE_REGISTER: P("REGISTER"); break;
		case TOKEN_TYPE_REGISTER_PAIR: P("REGISTER_PAIR"); break;
		case TOKEN_TYPE_PC_MOD: P("PC_MOD"); break;
		case TOKEN_TYPE_ADD: P("ADD"); break;
		case TOKEN_TYPE_SUB: P("SUB"); break;
		case TOKEN_TYPE_EOF: P("EOF"); break;
		case TOKEN_TYPE_LEN: P("LEN"); break;
		X_OPCODE
		default: printf("%-15d", t->type); break;
	}
#undef X
#undef P
	printf(" | Line: %-5" PRIu32 " | Lexame: %.*s\n",
			t->line, t->lex_size, t->lex);
}
#endif

