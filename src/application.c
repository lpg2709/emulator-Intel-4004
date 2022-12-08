#include "./application.h"

uint32_t cicles_limit;
uint32_t cicle;
uint8_t c_bank;
uint8_t c_chip;
bool shound_run;
bool animated_run;
bool run;

void application_init(Options opt, chip_4004 *c){
	cicles_limit = 0;
	cicle = 0;
	c_bank = 0;
	c_chip = 0;
	shound_run = true;
	animated_run = false;
	run = false;

	switch(opt.mode){
		case HELP:
			cli_screen_usage();
			exit(0);
			break;

		case VERSION:
			printf("4004-emulator %s\n", _VERSION);
			exit(0);
			break;

		case EMULATOR:
			if(strcmp(opt.rom_file_path, "") == 0){
				fprintf(stderr, "Error: rom-file parameter is required for"
								" emulation.\nFor help: 4004-emulator --help\n");
				exit(2);
			}
			long size;
			uint8_t *rom_source = (uint8_t*) b_read_file(opt.rom_file_path, &size);
			int i;

			for(i = 0; i < size; i++){
				printf("%.2X ", rom_source[i]);
			}

			loadProgramROM(c, rom_source, size);

			cli_screen_startup();
			break;

		case ASSEMBLER:
			if(strcmp(opt.source_file_path, "") == 0){
				fprintf(stderr, "Error: source-file parameter is required for"
								" assembler.\nFor help: 4004-emulator --help\n");
				exit(2);
			}

			if(strcmp(opt.output_file_path, "") == 0){
				fprintf(stderr, "Error: output-file parameter is required for"
								" assembler.\nFor help: 4004-emulator --help\n");
				exit(2);
			}

			parser(opt.source_file_path);

			exit(0);
			break;

		case DISASSEMBLER:
			exit(0);
			break;
	}


}

void application_main_loop(chip_4004 *c){
	char opt;
	while(shound_run){
		cli_main_view(c, c_bank, c_chip, cicle);
		if(!run)
			opt = application_input();

		if(run){
			cicle += chip_cycle(c, 99999999);
		}
		switch(opt){
			case 'r':
				run = true;
				break;
			case 'n':
				cicles_limit = 1;
				cicle += chip_cycle(c, cicles_limit);
				break;
			case 's':
				printf("How many steps forward (0 - 99999999): ");
				cicles_limit = keyboard_read_int();;
				cicle += chip_cycle(c, cicles_limit);
				break;
			case 'o':
				printf("(0) Set bank\n(1) Set chip\n(9) Cancel\n");
				switch(keyboard_read_char()){
					case '0':
						printf("Bank: (0-7)\n");
						c_bank = keyboard_read_int();
						break;
					case '1':
						printf("Chip: (0-3)\n");
						c_chip = keyboard_read_int();
						break;
					default:
						break;
				}
				break;
			case '1':
				cli_d_print_all_ram(c);
				printf("Press ENTER to continue...\n");
				keyboard_press_enter();
				break;
			case 'q':
				shound_run = false;
				break;
			default:
				printf("Invalid option, press ENTER to try again.");
				keyboard_press_enter();
				break;

		}
	}

}

char application_input(){
#if UI_OVERFLOW_80_CHARS
	printf("\nACTIONS\n"
		   "(n) next        (s) steps    (a) animated   (r) run   (o) operations   "
		   "(1) Print RAM   (q) quit\n> ");
#else
	printf("\nACTIONS\n"
		   "(n) next        (s) steps    (a) animated   (r) run   (o) operations\n"
		   "(1) Print RAM   (q) quit\n> ");
#endif
	return keyboard_read_char();
}

void keyboard_press_enter(){
	while(1){
		char c = getchar();
		if(c == '\n')
			break;
	}
}

char keyboard_read_char(){
	char t = getchar();
	getchar();
	return t;
}

int keyboard_read_int(){
	int t;
	scanf("%d", &t);
	getchar();
	return t;
}

