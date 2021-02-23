#include <stdio.h>
#include <stdlib.h>

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


	chip_cycle(&myChip, 5);
	cli_main_view(&myChip, 0);
	cli_d_print_all_ram(&myChip);
	
	
	/*
	uint8_t hex[] = {0x20, 0xB4, 0x21, 0xDF, 0xE0};
	b_write_file("./tests_src/WRM", hex, sizeof(*hex), sizeof(hex));
	*/
	return 0;
}
