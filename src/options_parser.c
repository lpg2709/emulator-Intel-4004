#include "./options_parser.h"

bool  options_parser(int argc, char **argv, Options *opt){
	int i = 1;
	opt->mode = EMULATOR;
	strcpy(opt->rom_file_path, "\0");
	strcpy(opt->source_file_path, "\0");

	while(i < argc){
		char *o = argv[i];
		if(strcmp(o, "--rom-file") == 0 || strcmp(o, "-rf") == 0){
			i++;
			strcpy(opt->rom_file_path, (const char*) argv[i]);
		}

		if(strcmp(o, "--source-file") == 0 || strcmp(o, "-sf") == 0){
			i++;
			strcpy(opt->source_file_path, (const char*) argv[i]);
		}

		if(strcmp(o, "--output-file") == 0 || strcmp(o, "-of") == 0){
			i++;
			strcpy(opt->source_file_path, (const char*) argv[i]);
		}

		if(strcmp(o, "--assembler") == 0 || strcmp(o, "-a") == 0){
			opt->mode = ASSEMBLER;
		}

		if(strcmp(o, "--disassembler") == 0 || strcmp(o, "-d") == 0){
			opt->mode = DISASSEMBLER;
		}

		if(strcmp(o, "--help") == 0){
			opt->mode = HELP;
		}

		if(strcmp(o, "--version") == 0){
			opt->mode = VERSION;
		}

		i++;
	}

	return true;
}

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
		   "- ROM file: %s\n"
		   "- Source file: %s\n"
		   "- Mode: %s\n", opt->rom_file_path, opt->source_file_path, mode);
}
