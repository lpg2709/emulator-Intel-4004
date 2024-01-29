#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


#include "4004_chip.h"
#include "files.h"
#include "gui_cli.h"
#include "application.h"
#include "options_parser.h"

int main(int argc, char **argv){
	Options options;
	options_parser(argc, argv, &options);

	chip_4004 myChip;
	init4004(&myChip);

	application_init(options, &myChip);

	application_main_loop(&myChip);
	// cli_d_print_rom_memory(&myChip);

	/*
	// Write data in binary for test
	uint8_t hex[] = { 0x20, 0x00, 0x22, 0x00, 0xDC, 0xB2, 0x21, 0xE4, 0xF2, 0xE5, 0xF2, 0xE6, 0xF2, 0xE7, 0xF2, 0x60, 0x72, 0x06 };
	b_write_file("./roms/bin/ram_status_test", hex, sizeof(*hex), sizeof(hex));
	*/
	return 0;
}
