#ifndef A_SCANNER_H
#define A_SCANNER_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "./a_token.h"

typedef struct sct_scanner scanner;

struct sct_scanner {
	char* source;
	token tokens[16384];
	uint16_t current_token;
	uint32_t start;
	uint32_t current;
	uint32_t line;
	long source_size;
};

token* scan_tokens(scanner *scan, const char* source, long source_size);
void scan_token(scanner *scan);

char advance(scanner *scan);
bool is_digit(char c);
bool is_alpha(char c);
bool is_alpha_numeric(char c);

#endif
