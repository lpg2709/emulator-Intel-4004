#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <stdbool.h>

#define MAX_PATH 250

enum MODE { EMULATOR, ASSEMBLER, DISASSEMBLER, HELP, VERSION };

typedef struct {
	char in_file_path[MAX_PATH]; // for ROM and SRC
	char output_file_path[MAX_PATH];
	enum MODE mode;
} Options;

bool options_parser(int argc, char **argv, Options *opt);

#if DEBUG
void d_print_options(Options *opt);
#endif

#endif // OPTIONS_PARSER_H
