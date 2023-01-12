#include "./a_token.h"

token new_token(enum token_type t, uint16_t l, uint8_t ib, char *le){
	token token;
	token.type = t;
	token.line = l;
	token.in_bytes = ib;
	strcpy(le, token.lexeme);
	return token;
}

char* token_string(token t) {
	char* fs;
	sprintf(fs, "Type: %d  Line: %d  InByte: %X  Lexame: %s",
			t.type, t.line, t.in_bytes, t.lexeme);
	return fs;
}
