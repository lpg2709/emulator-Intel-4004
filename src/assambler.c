#include "./assambler.h"
#include "./a_token.h"
#include "./a_scanner.h"
#include "./files.h"
#include "error.h"

#if DEBUG
static void d_print_scanned_tokens(scanner *s) {
	int i = 0;
	printf("==================================== TOKENS ===================================\n");
	do {
		printf("[%05d] ", i);
		print_token(&s->tokens[i++]);
	} while (s->tokens[i].type != TOKEN_TYPE_EOF && i < MAX_TOKENS);
	if(i == MAX_TOKENS)
		printf(">> MAX_TOKENS reached!\n");
}
#endif

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

Error parser(const char* source_path) {
	long f_size;
	const char* source = read_file(source_path, &f_size);
	scanner s;

	scan_tokens(&s, source, f_size);

#if DEBUG
	d_print_scanned_tokens(&s);
#endif

	free(s.tokens);

	return ERROR_NOT;
}

