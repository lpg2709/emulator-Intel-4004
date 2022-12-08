#include "./assambler.h"
#include <stdint.h>

void parser(const char* source_path) {
	long f_size;
	const char* source = read_file(source_path, &f_size);
	uint32_t current_index = 0;
	char current = source[0];

	while(current != '\0'){
		switch(current) {
			case ';':
				while(source[++current_index] != '\n') {}
				current = '\r';
				break;
		}
		printf("%c", current);
		current_index++;
		current = source[current_index];
	}
}

uint8_t opcode_to_hex(const char* opcode) {
}
