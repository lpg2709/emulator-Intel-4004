#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./src/4004_chip.h"
#include "./src/files.h"
#include "./src/gui_cli.h"

int main(){
	
	cli_screen_startup();
	long size;
	uint8_t *rom_ram_test = (uint8_t*) b_read_file("./tests_src/WRM", &size);
	chip_4004 myChip;
	init4004(&myChip);

	loadProgramROM(&myChip, rom_ram_test, size);
	cli_d_print_rom_memory(&myChip);


	uint32_t cicles_limit = 0;
	uint32_t cicle = 0;
	int bank, chip;
	bank = chip = 0;
	bool shound_run = true;
	while(shound_run){
		cli_main_view(&myChip,bank, chip, cicle);
		char opt = cli_main_input();
		switch(opt){
			case 'n':
				cicles_limit = 1;
				cicle += chip_cycle(&myChip, cicles_limit);
				break;
			case 'o':
				printf("(0) Set bank\n(1) Set chip\n");
				getchar();
				switch(getchar()){
					case '0':
						printf("Bank: (0-7)\n");
						scanf("%d", &bank);
						break;
					case '1':
						printf("Chip: (0-3)\n");
						scanf("%d", &chip);
						break;
				}
				break;
			case 'q':
				shound_run = false;
				break;
		
		}
	}
	
	cli_d_print_rom_memory(&myChip);
	
	/*
	uint8_t hex[] = {0x20, 0xB4, 0x21, 0xDF, 0xE0};
	b_write_file("./tests_src/WRM", hex, sizeof(*hex), sizeof(hex));
	*/
	return 0;
}
