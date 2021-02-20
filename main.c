#include <stdio.h>
#include <stdlib.h>

#include "./src/4004_chip.h"
#include "./src/files.h"
#include "./src/gui_cli.h"

int main(){
	
	cli_screen_startup();
	long size;
	uint8_t *rom_ram_test = (uint8_t*) b_read_file("./tests_src/ram_test", &size);
	chip_4004 myChip;
	init4004(&myChip);

	loadProgramROM(&myChip, rom_ram_test, size);
	cli_d_print_rom_memory(&myChip);


	chip_cycle(&myChip, 5);
	cli_main_view(&myChip, 0);
	cli_d_print_all_ram(&myChip);
	
	/*
	uint8_t hex[] = {0x20, 0x00, 0x22, 0x00, 0xDC, 0xB2, 0x21, 0xE0, 0xF2, 0x71, 0x06, 0x60, 0x72, 0x06, 0x20, 0x00, 0x22, 0x00, 0xDC, 0xB2, 0x21, 0xE4, 0xF2, 0xE5, 0xF2, 0xE6, 0xF2, 0xE7, 0xF2, 0x60, 0x72, 0x14, 0x40, 0x20};
	b_write_file("./tests_src/ram_test", hex, sizeof(*hex), sizeof(hex));
	*/
	return 0;
}
