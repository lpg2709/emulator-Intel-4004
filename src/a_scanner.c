#include "./a_scanner.h"
#include "a_token.h"

token* scan_tokens(const char *source){
	strcpy((char*) source, scanner.source);
	memset((void*) scanner.tokens, 0, sizeof(token));
	scanner.start = 0;
	scanner.current = 0;
	scanner.current_token = 0;
	scanner.line = 1;

	while(source[scanner.current] != EOF) {
		scanner.start = scanner.current;
		scan_token();
	}

	scanner.tokens[scanner.current_token] = new_token(EOF, scanner.line, 0, "\n");

	return scanner.tokens;
}

void scan_token(){
	char c = advance();
	switch(c) {
		case ';':
			while(advance() != '\n');
			break;
		case ' ':
		case '\r':
		case '\t':
			break;
		case '\n':
			scanner.line++;
			break;
		default:
			// check if is digit or string/instruction
			break;
	}
}

char advance() {
	scanner.current++;
	return scanner.source[scanner.current - 1];
}

bool is_digit(char c){
	return c >= '0' && c <= '9';
}

bool is_alpha(char c) {
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool is_alpha_numeric(char c){
	return is_alpha(c) || is_digit(c);
}
