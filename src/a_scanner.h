#ifndef A_SCANNER_H
#define A_SCANNER_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "./a_token.h"

struct sct_scanner {
	const char* source;
	token tokens[16384];
	uint16_t current_token;
	uint32_t start;
	uint32_t current;
	uint32_t line;
}scanner;

token* scan_tokens(const char* source);
void scan_token();

char advance();
bool is_digit(char c);
bool is_alpha(char c);
bool is_alpha_numeric(char c);

#endif
