#include "./a_scanner.h"
#include "./a_token.h"
#include <string.h>

token* scan_tokens(scanner *scan, const char *source, long source_size){
	scan->source = (char *) malloc(sizeof(char) * source_size);
	memcpy(scan->source, source, source_size);
	memset((void*) scan->tokens, 0, sizeof(token) * 16384);
	scan->start = 0;
	scan->current = 0;
	scan->current_token = 0;
	scan->line = 1;

	while(source[scan->current] != EOF) {
		scan->start = scan->current;
		scan_token(scan);
	}

	scan->tokens[scan->current_token] = new_token(EOF, scan->line, 0, "\n");

	return scan->tokens;
}

void scan_token(scanner *scan){
	char c = advance(scan);
	switch(c) {
		case ';':
			while(advance(scan) != '\n');
			break;
		case ' ':
		case '\r':
		case '\t':
			break;
		case '\n':
			scan->line++;
			break;
		default:
			// check if is digit or string/instruction
			break;
	}
}

char advance(scanner *scan) {
	scan->current++;
	return scan->source[scan->current - 1];
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
