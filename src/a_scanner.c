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
	scan->source_size = source_size;

	while(source[scan->current] != '\n') {
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
			scan->line++;
			break;
		case ' ':
		case '\r':
		case '\t':
			break;
		case '\n':
			scan->line++;
			break;
		default:
			if(is_digit(c)){
				// TODO: FIX when is new line
				while((scan->current < scan->source_size) && is_digit(c)) {
					c = advance(scan);
				}

				// TODO: handle registers pairs
				if(is_alpha(c)) {
					if (c != 'p' && c != 'P') {
						printf("TODO: Invalid register pair identifier on line: %d", scan->line);
					}
				}

				char lexame[5];
				int i, j = 0;
				for(i = scan->start; i < scan->current; i++) {
					if(j < 5) {
						lexame[j] = scan->source[i];
						j++;
					}
				}
				lexame[j++] = '\0';

				scan->tokens[scan->current_token] = new_token(NUMBER, scan->line, 0, lexame);
				scan->current_token++;
			} else if(is_alpha(c)) {
				while((scan->current < scan->source_size) && is_alpha(c)) {
					c = advance(scan);
				}
				char lexame[5];
				int i, j = 0;
				for(i = scan->start; i < scan->current; i++) {
					if(j < 4) {
						lexame[j] = scan->source[i];
						j++;
					}
				}
				lexame[j++] = '\0';
				scan->tokens[scan->current_token] = new_token(OPCODE, scan->line, 0, lexame);
				scan->current_token++;
			}
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
