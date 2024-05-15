#include "./options_parser.h"

#include <stdio.h>
#include <string.h>
#include "error.h"

static Error setFilePath(char *dest, int *i, char **argv, bool consume_next) {
	if(consume_next)
		(*i)++;
	if(strlen(argv[*i]) < MAX_PATH) {
		strcpy(dest, (const char*) argv[*i]);
		return ERROR_NOT;
	}
	return ERROR_FILE_PATH_TO_LARGE;
}

bool options_parser(int argc, char **argv, Options *opt){
	int i = 1; // First is the program name
	opt->mode = EMULATOR;
	memset(opt->in_file_path, 0, MAX_PATH);

	while(i < argc){
		char *o = argv[i];
		if(strcmp(o, "--emulator") == 0 || strcmp(o, "-e") == 0){
			opt->mode = ASSEMBLER;
			// TODO: Meke something with the error
			setFilePath(opt->in_file_path, &i, argv, true);
			return true;
		} else if(strcmp(o, "--assembler") == 0 || strcmp(o, "-a") == 0){
			opt->mode = ASSEMBLER;
		} else if(strcmp(o, "--disassembler") == 0 || strcmp(o, "-d") == 0){
			opt->mode = DISASSEMBLER;
		} else if(strcmp(o, "--output-file") == 0 || strcmp(o, "-o") == 0){
			// TODO: Meke something with the error
			setFilePath(opt->output_file_path, &i, argv, true);
		} else if(strcmp(o, "--help") == 0){
			opt->mode = HELP;
			return true;
		} else if(strcmp(o, "--version") == 0){
			opt->mode = VERSION;
			return true;
		} else {
			// TODO: Meke something with the error
			setFilePath(opt->in_file_path, &i, argv, false);
		}

		i++;
	}

	return true;
}

#if DEBUG
void d_print_options(Options *opt){
	char mode[15];
	switch(opt->mode){
		case EMULATOR:
			strcpy(mode, "Emulator");
			break;
		case ASSEMBLER:
			strcpy(mode, "Assembler");
			break;
		case DISASSEMBLER:
			strcpy(mode, "Disassembler");
			break;
		case HELP:
			strcpy(mode, "Help");
			break;
		case VERSION:
			strcpy(mode, "Version");
			break;
	}
	printf("Options: \n"
		   "- File: %s\n"
		   "- Mode: %s\n", opt->in_file_path, mode);
}
#endif
