#include "./assambler.h"
#include <stdint.h>
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
	uint32_t current_index = 0;
	char current = source[0];
	char mnemonic[3];
	memset(mnemonic, 0, 3);

	while(current != '\0'){
		switch(current) {
			case ';':
				while(source[++current_index] != '\n') {}
				current = '\r';
				break;
			case '\n':
				break;
			case ' ':
				memset(mnemonic, 0, 3);
				break;
			default:
				if(isalpha(current)){
					append_mnemonic(mnemonic, current);
				}

		}
		// printf("%c", current);
		current_index++;
		current = source[current_index];
	}
}

