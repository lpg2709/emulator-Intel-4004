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
	// TODO: Compleat function
	return opcode[0];
}

void parser(const char* source_path) {
	long f_size;
	const char* source = read_file(source_path, &f_size);
	// token *tokens;
	scanner s;

	scan_tokens(&s, source, f_size);

	int i = 0;
	while (s.tokens[i].type != T_EOF) {
		print_token(s.tokens[i]);
		i++;
	}
}

