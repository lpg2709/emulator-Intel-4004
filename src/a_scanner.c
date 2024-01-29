#include "./a_scanner.h"

token* scan_tokens(scanner *scan, const char *source, long source_size){
	scan->source = (char *) malloc(sizeof(char) * source_size);
	memcpy(scan->source, source, source_size);
	memset((void*) scan->tokens, 0, sizeof(token) * MAX_TOKENS);
	scan->start = 0;
	scan->current = 0;
	scan->current_token = 0;
	scan->line = 1;
	scan->source_size = source_size;

	while(source[scan->current] != '\0' && scan->current_token < MAX_TOKENS) {
		scan->start = scan->current;
		scan_token(scan);
	}

	scan->tokens[scan->current_token] = new_token(TOKEN_TYPE_EOF, scan->line, 0, "\n");

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
				while((scan->current < scan->source_size) && isdigit(c)) {
					c = advance(scan);
				}

				enum token_type tt = TOKEN_TYPE_NUMBER;

				if(isalpha(c)) {
					if (c != 'p' && c != 'P') {
						printf("TODO: Invalid register pair identifier on line: %d", scan->line);
						break;
					} else {
						tt = TOKEN_TYPE_REGISTER_PAIR;
					}
				}

				char lexame[5];
				get_lexame(lexame, scan);

				scan->tokens[scan->current_token] = new_token(tt, scan->line, 0, lexame);
				scan->current_token++;
				if(c == '\n')
					scan->line++;
			} else if(isalpha(c)) {
				enum token_type tt = TOKEN_TYPE_OPCODE;
				bool maybeRegister = false;
				if(c == 'R' || c == 'r')
					maybeRegister = true;

				while((scan->current < scan->source_size) && isalnum(c)) {
					c = advance(scan);
				}

				if((scan->current - scan->start < 2) && isdigit(c) && maybeRegister)
					tt = TOKEN_TYPE_REGISTER;

				char lexame[5];
				get_lexame(lexame, scan);

				if(c == ',')
					tt = TOKEN_TYPE_LABEL;
				scan->tokens[scan->current_token] = new_token(tt, scan->line, 0, lexame);
				scan->current_token++;
				if(c == '\n')
					scan->line++;
			}
			// check if is digit or string/instruction
			break;
	}
}

char advance(scanner *scan) {
	char c = scan->source[scan->current];
	scan->current++;
	return c;
}

bool get_lexame(char *lexam, scanner *scan) {
	if( (scan->current - scan->start) > 5 )
		return false;

	uint32_t i = 0;
	uint8_t j = 0;
	for(i = scan->start; i < scan->current - 1; i++) {
		if(j < 4) {
			lexam[j] = scan->source[i];
			j++;
		}
	}
	lexam[j++] = '\0';

	return true;
}
