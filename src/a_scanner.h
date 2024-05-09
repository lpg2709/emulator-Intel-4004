#ifndef A_SCANNER_H
#define A_SCANNER_H

#include <stdbool.h>

#include "./a_token.h"

#define MAX_TOKENS 16384

typedef struct sct_scanner scanner;

struct sct_scanner {
	const char* source;
	token *tokens;
	uint16_t current_token;
	char *start;
	char *current;
	uint32_t line;
	long source_size;
};

token* scan_tokens(scanner *scan, const char* source, long source_size);
void scan_token(scanner *scan);

#endif
