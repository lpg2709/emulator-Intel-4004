#include "./a_token.h"
#include <inttypes.h>

token new_token(enum token_type t, uint16_t l, uint8_t ib, char *le){
	token token;
	token.type = t;
	token.line = l;
	token.in_bytes = ib;
	strcpy(token.lexeme, le);
	return token;
}

void print_token(token t) {
	printf("Type: %d  Line: %" PRIu32 "  InByte: %" PRIu8 "  Lexame: %s\n",
			t.type, t.line, t.in_bytes, t.lexeme);
}
