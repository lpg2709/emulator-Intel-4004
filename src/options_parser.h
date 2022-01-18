#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct stc_opt Options;

enum MODE { EMULATOR, ASSEMBLER, DISASSEMBLER, HELP, VERSION };

struct stc_opt {
	char rom_file_path[250];
	char source_file_path[250];
	char output_file_path[250];
	enum MODE mode;
};

bool options_parser(int argc, char **argv, Options *opt);
void d_print_options(Options *opt);

#endif // OPTIONS_PARSER_H
