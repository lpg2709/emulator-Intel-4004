#include "./a_scanner.h"

#include <bits/stdint-uintn.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include "./a_token.h"

static bool isAtEnd(scanner *scan) {
	return *scan->current == '\0';
}

static char advance(scanner *scan) {
	return *scan->current++;
}

static char peek(scanner *scan, int offset) {
	return scan->current[1 - offset];
}

static enum token_type identifierType(char *start, uint32_t offset) {
	if(offset < 1 || offset > 3)
		goto unknow;

	// Not using strcmp for performace?
	switch(*start) {
		case 'A':
			switch(start[1]) {
				case 'D':
					if(start[2] == 'D')
						return TOKEN_TYPE_OP_ADD;
					if(start[2] == 'M')
						return TOKEN_TYPE_OP_ADM;
				default: goto unknow;
			}
			break;
		case 'B':
			switch(start[1]) {
				case 'B':
					if(start[2] == 'L')
						return TOKEN_TYPE_OP_BBL;
				default: goto unknow;
			}
			break;
		case 'C':
			switch(start[1]) {
				case 'L':
					switch(start[2]) {
						case 'B':
							return TOKEN_TYPE_OP_CLB;
						case 'C':
							return TOKEN_TYPE_OP_CLC;
						default: goto unknow;
					}
				case 'M':
					switch(start[2]) {
						case 'C':
							return TOKEN_TYPE_OP_CMC;
						case 'M':
							return TOKEN_TYPE_OP_CMM;
						default: goto unknow;
					}
				default: goto unknow;
			}
			break;
		case 'D':
			switch(start[1]) {
				case 'A':
					switch(start[2]) {
						case 'A':
							return TOKEN_TYPE_OP_DAA;
						case 'C':
							return TOKEN_TYPE_OP_DAC;
						default: goto unknow;
					}
				case 'C':
					if(start[2] == 'L')
						return TOKEN_TYPE_OP_DCL;
					goto unknow;
				default: goto unknow;
			}
			break;
		case 'F':
			switch(start[1]) {
				case 'I':
					if(start[2] == 'M')
						return TOKEN_TYPE_OP_FIM;
					if(start[2] == 'N')
						return TOKEN_TYPE_OP_FIN;
				default: goto unknow;
			}
			break;
		case 'I':
			switch(start[1]) {
				case 'A':
					if(start[2] == 'C')
						return TOKEN_TYPE_OP_IAC;
				case 'S':
					if(start[2] == 'Z')
						return TOKEN_TYPE_OP_ISZ;
				default: goto unknow;
			}
			break;

		case 'J':
			switch(start[1]) {
				case 'C':
					if(start[2] == 'N')
						return TOKEN_TYPE_OP_JCN;
				case 'I':
					if(start[2] == 'N')
						return TOKEN_TYPE_OP_JIN;
				case 'M':
					if(start[2] == 'S')
						return TOKEN_TYPE_OP_JMS;
				case 'U':
					if(start[2] == 'N')
						return TOKEN_TYPE_OP_JUN;
				default: goto unknow;
			}
			break;
		case 'K':
			switch(start[1]) {
				case 'B':
					if(start[2] == 'P')
						return TOKEN_TYPE_OP_KBP;
				default: goto unknow;
			}
			break;
		case 'L':
			switch(start[1]) {
				case 'D':
					if(offset == 2)
						return TOKEN_TYPE_OP_LD;
					if(start[2] == 'M')
						return TOKEN_TYPE_OP_LDM;
				default: goto unknow;
			}
			break;
		case 'N':
			switch(start[1]) {
				case 'O':
					if(start[2] == 'P')
						return TOKEN_TYPE_OP_NOP;
				default: goto unknow;
			}
			break;
		default: goto unknow;
	}

unknow:
	return TOKEN_TYPE_UNKNOW;
}

token* scan_tokens(scanner *scan, const char *source, long source_size){
	scan->source = source;
	scan->source_size = source_size;
	scan->current = (char*) source;
	scan->start = scan->current;
	scan->current_token = 0;
	scan->line = 1;
	scan->tokens = calloc(sizeof(token), MAX_TOKENS);
	if(scan->tokens == NULL) {
		fprintf(stderr, "Fail to allocate memory for tokens!");
		exit(1);
	}

	while(!isAtEnd(scan) && scan->current_token < MAX_TOKENS) {
		scan->start = scan->current;
		scan_token(scan);
	}

	scan->tokens[scan->current_token] = token_new(TOKEN_TYPE_EOF, scan->line, 0, NULL, 0);

	return scan->tokens;
}

void scan_token(scanner *scan){
	char c = advance(scan);
	if(isdigit(c)){
		while(isdigit(c) && !isAtEnd(scan) && peek(scan, 1) != '\n') {
			c = advance(scan);
		}

		enum token_type tt = TOKEN_TYPE_NUMBER;

		if(isalpha(c)) {
			if(c != 'p' && c != 'P')
				tt = TOKEN_TYPE_UNKNOW;
			else
				tt = TOKEN_TYPE_REGISTER_PAIR;
		}

		scan->tokens[scan->current_token++] = token_new(tt, scan->line, 0, scan->start, scan->current - scan->start);
		if(c == '\n')
			scan->line++;
		return;
	}

	if(isalpha(c)) {
		enum token_type tt = TOKEN_TYPE_UNKNOW;
		bool maybeRegister = false;
		if(c == 'R' || c == 'r')
			maybeRegister = true;

		while(isalpha(c) && !isAtEnd(scan) && peek(scan, 1) != '\n' && peek(scan, 1) != ',') {
			c = advance(scan);
		}

		if((scan->current - scan->start < 3) && isdigit(c) && maybeRegister)
			tt = TOKEN_TYPE_REGISTER;
		else if(peek(scan, 1) == ',')
			tt = TOKEN_TYPE_LABEL;
		else
			tt = identifierType(scan->start, scan->current - scan->start);

		scan->tokens[scan->current_token++] = token_new(tt, scan->line, 0, scan->start, scan->current - scan->start);
		if(c == '\n')
			scan->line++;
		return;
	}

	switch(c) {
		case ';':
			while(advance(scan) != '\n');
			scan->line++;
			break;
		case ' ':
		case ',':
		case '\r':
		case '\t':
			break;
		case '\n':
			scan->line++;
			break;
		default: // TODO: Better error handle
			fprintf(stderr, "Unknow char '%c' found on line %d\n", c, scan->line);
			break;
	}
}


