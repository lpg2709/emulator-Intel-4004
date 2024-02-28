#include "./a_scanner.h"

static bool isAtEnd(scanner *scan) {
	return *scan->current == '\0';
}

token* scan_tokens(scanner *scan, const char *source, long source_size){
	scan->source = source;
	scan->source_size = source_size;
	scan->current = (char*) source;
	scan->start = scan->current;
	scan->current_token = 0;
	scan->line = 1;
	memset((void*) scan->tokens, 0, sizeof(token) * MAX_TOKENS);

	while(isAtEnd(scan) && scan->current_token < MAX_TOKENS) {
		scan->start = scan->current;
		scan_token(scan);
	}

	scan->tokens[scan->current_token] = new_token(TOKEN_TYPE_EOF, scan->line, 0, NULL, 0);

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
			if(isdigit(c)){
				while(isAtEnd(scan) && isdigit(c)) {
					c = advance(scan);
				}

				enum token_type tt = TOKEN_TYPE_NUMBER;

				if(isalpha(c)) {
					assert(!(c != 'p' && c != 'P')); // TODO: how report this????
					tt = TOKEN_TYPE_REGISTER_PAIR;
				}

				scan->tokens[scan->current_token] = new_token(tt, scan->line, 0, scan->start, scan->current - scan->start);
				scan->current_token++;
				if(c == '\n')
					scan->line++;
			} else if(isalpha(c)) {
				enum token_type tt = TOKEN_TYPE_OPCODE;
				bool maybeRegister = false;
				if(c == 'R' || c == 'r')
					maybeRegister = true;

				while(isAtEnd(scan) && isalnum(c)) {
					c = advance(scan);
				}

				if((scan->current - scan->start < 2) && isdigit(c) && maybeRegister)
					tt = TOKEN_TYPE_REGISTER;

				if(c == ',')
					tt = TOKEN_TYPE_LABEL;
				scan->tokens[scan->current_token] = new_token(tt, scan->line, 0, scan->start, scan->current - scan->start);
				scan->current_token++;
				if(c == '\n')
					scan->line++;
			}
			break;
	}
}

char advance(scanner *scan) {
	return *scan->current++;
}

