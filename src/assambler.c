#include "./assambler.h"
#include "./a_token.h"
#include "./a_scanner.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void append_mnemonic(char m[3], char c) {
	int i;
	for(i = 0; i < 3; i++){
		if(m[i] == 0){
			m[i] = c;
			return;
		}
	}
}

uint8_t opcode_to_hex(const char* opcode) {
}

void parser(const char* source_path) {
	long f_size;
	const char* source = read_file(source_path, &f_size);
	token *tokens;
	scanner s;

	tokens = scan_tokens(&s, source, f_size);

	print_token(tokens[0]);
}

