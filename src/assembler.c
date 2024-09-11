#include "./assembler.h"
#include "./a_token.h"
#include "./a_scanner.h"
#include "./files.h"
#include "error.h"
#include "4004_chip.h"

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

static uint8_t nop() {
	return NOP;
}

void parse() {

}

Error assembler(const char* source_path) {
	long f_size;
	const char* source = read_file(source_path, &f_size);
	char *output = (char*) calloc(sizeof(char), ROM_MAX_SIZE);
	size_t out_pos = 0;
	if(output == NULL) {
		fprintf(stderr, "Fail to alloc memory for assambler output\n");
		return ERROR_ALLOC_MEMORY;
	}
	scanner s;

	scan_tokens(&s, source, f_size);

#if DEBUG
	d_print_scanned_tokens(&s);
#endif
	token *t = &s.tokens[0];
	while(t->type != TOKEN_TYPE_EOF) {
		print_token(t);
		t++;
		switch(t->type) {
			case TOKEN_TYPE_OP_NOP:
				output[out_pos] = nop();
				out_pos++;
				break;
		}
	}

	free(s.tokens);

	free((void *)source);
	return ERROR_NOT;
}

