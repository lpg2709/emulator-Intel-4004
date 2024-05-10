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

// This function is pretty ugly, I know. But it work
static enum token_type identifierType(char *start, uint32_t offset) {
	if(offset < 1 || offset > 3)
		goto unknow;

// Not use strncmp for performacet ?
#define DIST_A ('a' - 'A')
#define CASE(x) case (x): \
    case ((x) + (DIST_A))
#define IF_CHAR(s, c, tok) if( (s) == (c) || (s) == ((c) + (DIST_A)) ) { \
	return (tok); } break;

	switch(*start) {
		CASE('A'):
			switch(start[1]) {
				CASE('D'):
					IF_CHAR(start[2], 'D', TOKEN_TYPE_OP_ADD)
					IF_CHAR(start[2], 'M', TOKEN_TYPE_OP_ADM)
					break;
				default: goto unknow;
			}
			break;
		CASE('B'):
			switch(start[1]) {
				CASE('B'): IF_CHAR(start[2], 'L', TOKEN_TYPE_OP_BBL)
				default: goto unknow;
			}
			break;
		CASE('C'):
			switch(start[1]) {
				CASE('L'):
					switch(start[2]) {
						CASE('B'): return TOKEN_TYPE_OP_CLB;
						CASE('C'): return TOKEN_TYPE_OP_CLC;
						default: goto unknow;
					}
				CASE('M'):
					switch(start[2]) {
						CASE('C'): return TOKEN_TYPE_OP_CMC;
						default: goto unknow;
					}
				default: goto unknow;
			}
			break;
		CASE('D'):
			switch(start[1]) {
				CASE('A'):
					switch(start[2]) {
						CASE('A'): return TOKEN_TYPE_OP_DAA;
						CASE('C'): return TOKEN_TYPE_OP_DAC;
						default: goto unknow;
					}
				CASE('C'): IF_CHAR(start[2], 'L', TOKEN_TYPE_OP_DCL)
				default: goto unknow;
			}
			break;
		CASE('F'):
			switch(start[1]) {
				CASE('I'):
					IF_CHAR(start[2], 'M', TOKEN_TYPE_OP_FIM)
					IF_CHAR(start[2], 'N', TOKEN_TYPE_OP_FIN)
				default: goto unknow;
			}
			break;
		CASE('I'):
			switch(start[1]) {
				CASE('A'): IF_CHAR(start[2], 'C', TOKEN_TYPE_OP_IAC)
				CASE('N'): IF_CHAR(start[2], 'C', TOKEN_TYPE_OP_INC)
				CASE('S'): IF_CHAR(start[2], 'Z', TOKEN_TYPE_OP_ISZ)
				default: goto unknow;
			}
			break;

		CASE('J'):
			switch(start[1]) {
				CASE('C'): IF_CHAR(start[2], 'N', TOKEN_TYPE_OP_JCN)
				CASE('I'): IF_CHAR(start[2], 'N', TOKEN_TYPE_OP_JIN)
				CASE('M'): IF_CHAR(start[2], 'M', TOKEN_TYPE_OP_JMS)
				CASE('U'): IF_CHAR(start[2], 'N', TOKEN_TYPE_OP_JUN)
				default: goto unknow;
			}
			break;
		CASE('K'):
			switch(start[1]) {
				CASE('B'): IF_CHAR(start[2], 'P', TOKEN_TYPE_OP_KBP)
				default: goto unknow;
			}
			break;
		CASE('L'):
			switch(start[1]) {
				CASE('D'):
					if(offset == 2) return TOKEN_TYPE_OP_LD;
					IF_CHAR(start[2], 'M', TOKEN_TYPE_OP_LDM)
				default: goto unknow;
			}
			break;
		CASE('N'):
			switch(start[1]) {
				CASE('O'): IF_CHAR(start[2], 'P', TOKEN_TYPE_OP_NOP)
				default: goto unknow;
			}
			break;
		CASE('R'):
			switch(start[1]) {
				CASE('A'):
					IF_CHAR(start[2], 'L', TOKEN_TYPE_OP_RAL)
					IF_CHAR(start[2], 'R', TOKEN_TYPE_OP_RAR)
				CASE('D'):
					if(start[2] == '0') return TOKEN_TYPE_OP_RD0;
					if(start[2] == '1') return TOKEN_TYPE_OP_RD1;
					if(start[2] == '2') return TOKEN_TYPE_OP_RD2;
					if(start[2] == '3') return TOKEN_TYPE_OP_RD3;
					IF_CHAR(start[2], 'M', TOKEN_TYPE_OP_RDM)
					IF_CHAR(start[2], 'R', TOKEN_TYPE_OP_RDR)
				default: goto unknow;
			}
			break;
		CASE('S'):
			switch(start[1]) {
				CASE('M'): IF_CHAR(start[2], 'B', TOKEN_TYPE_OP_SMB)
				CASE('R'): IF_CHAR(start[2], 'C', TOKEN_TYPE_OP_SRC)
				CASE('T'): IF_CHAR(start[2], 'C', TOKEN_TYPE_OP_STC)
				CASE('U'): IF_CHAR(start[2], 'B', TOKEN_TYPE_OP_SUB)
				default: goto unknow;
			}
			break;
		CASE('T'):
			switch(start[1]) {
				CASE('C'):
					IF_CHAR(start[2], 'C', TOKEN_TYPE_OP_TCC)
					IF_CHAR(start[2], 'S', TOKEN_TYPE_OP_TCS)
				default: goto unknow;
			}
			break;
		CASE('W'):
			switch(start[1]) {
				CASE('M'): IF_CHAR(start[2], 'P', TOKEN_TYPE_OP_WMP)
				CASE('P'): IF_CHAR(start[2], 'M', TOKEN_TYPE_OP_WPM)
				CASE('R'):
					switch(start[2]) {
						case '0': return TOKEN_TYPE_OP_WR0;
						case '1': return TOKEN_TYPE_OP_WR1;
						case '2': return TOKEN_TYPE_OP_WR2;
						case '3': return TOKEN_TYPE_OP_WR3;
						CASE('M'): return TOKEN_TYPE_OP_WRM;
						CASE('R'): return TOKEN_TYPE_OP_WRR;
						default: goto unknow;
					}
					break;
				default: goto unknow;
			}
			break;
		CASE('X'):
			switch(start[1]) {
				CASE('C'): IF_CHAR(start[2], 'H', TOKEN_TYPE_OP_XCH)
				default: goto unknow;
			}
			break;
		default: goto unknow;
	}
#undef IF_CHAR
#undef CASE
#undef DIST_A

unknow:
	return TOKEN_TYPE_TEXT;
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
		while(isdigit(c) && !isAtEnd(scan) && peek(scan, 1) != '\n' && peek(scan, 1) != ' ') {
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
		enum token_type tt = TOKEN_TYPE_TEXT;
		bool maybeRegister = false;
		if(c == 'R' || c == 'r')
			maybeRegister = true;

		while(isalpha(c) && !isAtEnd(scan) && peek(scan, 1) != '\n' && peek(scan, 1) != ',' && peek(scan, 1) != ' ') {
			c = advance(scan);
		}

		if((scan->current - scan->start < 3) && isdigit(c) && maybeRegister)
			tt = TOKEN_TYPE_REGISTER;
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


