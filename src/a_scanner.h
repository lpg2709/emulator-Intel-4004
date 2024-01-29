#ifndef A_SCANNER_H
#define A_SCANNER_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <assert.h>

#include "./a_token.h"

#define MAX_TOKENS 16384

typedef struct sct_scanner scanner;

struct sct_scanner {
	char* source;
	token tokens[MAX_TOKENS];
	uint16_t current_token;
	uint32_t start;
	uint32_t current;
	uint32_t line;
	long source_size;
};

token* scan_tokens(scanner *scan, const char* source, long source_size);
void scan_token(scanner *scan);

char next_char(scanner *scan);
char advance(scanner *scan);
bool get_lexame(char *lexam, scanner *scan);

#endif
