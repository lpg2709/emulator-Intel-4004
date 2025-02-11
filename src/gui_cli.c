#include "gui_cli.h"

#ifdef WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include "config.h"

void cli_d_print_rom_memory(chip_4004 *c){
	printf("------------------------------------------ ROM MEMORY -----------------------------------------\n");
	int i;
	for(i = 0; i < 4096; i++){
		if(i%32 == 0 && i != 0)
			printf("\n");
		printf("%02X ", c->ROM[i]);
	}
	printf("\n-----------------------------------------------------------------------------------------------\n");
}

void cli_d_print_all_ram(chip_4004 *c){
	int i, j, k,bank, chip;

	printf("------------------------------------------ RAM MEMORY -----------------------------------------\n");
	bank=chip=j=0;
	for(i = 0; i < 2048; i++){
		if((i%16 == 0 && i != 0)) {
			printf("| ");
			for(k = 0; k < 4; k++){
				printf("%X ", c->RAM[k + (j*4)+((chip*15)+(bank*63))]);
			}
			printf("\n");
			j++;
			if(j > 3)
				j = 0;
		}

		if(i == 0 || i%64 == 0){
			printf("\n\n              BANK: %d CHIP: %d              \n", bank, chip);
			printf("            0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3\n");
		}
		if(i%64 == 0 && i != 0){
			chip++;
			if(chip%4 == 0){
				bank++;
				chip = 0;
			}
		}

		if(i == 0 || i%16 == 0) {
			printf("REGISTER %d: ", j);
		}

		printf("%X ", c->RAM[i]);
	}
	printf("\n-----------------------------------------------------------------------------------------------\n");

}

void cli_screen_usage(){
	printf(
		"Usage: 4004-emulator [OPTIONS] File\n"
		"Simple emulator of the Intel 4004 chip.\n"
		"Intel 4004 is a 4-bit CPU released by Intel in 1971, which was the \n"
		"  first commercial programmable microprocessor.\n"
		"\n"
		"Options:\n"
		"-e,  --emulator        Load a ROM/binary file and run. [Default]\n"
		"-a,  --assembler       Use the assembler of 4004-emulator to generate\n"
		"                         te binary file.\n"
		"-d,  --disassembler    Use the disassembler of 4004-emulator to\n"
		"                         generate the reverse opcodes of a binary file.\n"
		"-o,  --output-file     Path to the output file. (Used by assembler and\n"
        "                                                 disassembler)\n"
		"     --help            Display this help and exit.\n"
		"     --version         Output version information and exit\n"
		"\n"
		"File: \n"
		" File path to ROM or source file, depending on the mode chosen.\n"
		"  If in emulation mode or disassembler, the file need to be a \n"
		"    ROM/binary file.\n"
		"  If in assembler mode, the file need to be a source/text file.\n"
	);
}

void cli_screen_startup(){
	cli_clear_screen();
	printf(
		"\n"
		"           WELCOME TO\n"
		"      INTEL 4004 EMULATOR\n"
		"\n"
		"   --#--#--#--#--#--#--#--#--\n"
		"  |                          |\n"
		"  |      #   ##    ##      # |\n"
		"   --   ##  #  #  #  #    ## |\n"
		"     | # #  #  #  #  #   # # |\n"
		"   -- ####  #  #  #  #  #### |\n"
		"  |      #   ##    ##      # |\n"
		"  |                          |\n"
		"   --#--#--#--#--#--#--#--#--\n"
		"\n"
		"Press ENTER key to continue...\n");
	while(1){
		char c = getchar();
		if(c == '\n')
			break;
	}

}

void cli_main_view(chip_4004 *c, int bank, int chip, uint64_t cycle){
	cli_clear_screen();
#define RAM(index) c->RAM[index+(chip*64)+(bank*256)]
#define RAM_STATUS(index) c->RAM_status[index+(chip*16)+(bank*64)]

#if UI_OVERFLOW_80_CHARS
	printf(
			"\n"
			"    STACK             INDEX REGISTERS              RAM BANK: %d  RAM CHIP: %d              STATUS\n"
			"LEVEL 1: %03X     R0 R1: %X %X  R8 R9: %X %X               0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3\n"
			"LEVEL 2: %03X     R2 R3: %X %X  RA RB: %X %X\n"
			"LEVEL 3: %03X     R4 R5: %X %X  RC RD: %X %X   REGISTER 0: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"                 R6 R7: %X %X  RE RF: %X %X   REGISTER 1: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"                                          REGISTER 2: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"ACCUMULATOR: %X     CARRY: %X  TEST: %X      REGISTER 3: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"PC: %03X            CYCLES: %ld\n"
			"->: %.2X %.2X\n"
		 	,
			  bank, chip,
			  c->STACK.addrs[0], c->IR[0x0], c->IR[0x1], c->IR[0x8], c->IR[0x9],
			  c->STACK.addrs[1], c->IR[0x2], c->IR[0x3], c->IR[0xA], c->IR[0xB],
			  c->STACK.addrs[2], c->IR[0x4], c->IR[0x5], c->IR[0xC], c->IR[0xD],  RAM(0),  RAM(1),  RAM(2),  RAM(3),  RAM(4),  RAM(5),  RAM(6),  RAM(7),  RAM(8),  RAM(9), RAM(10), RAM(11), RAM(12), RAM(13), RAM(14), RAM(15), RAM_STATUS(0), RAM_STATUS(1), RAM_STATUS(2), RAM_STATUS(3),
			                     c->IR[0x6], c->IR[0x7], c->IR[0xE], c->IR[0xF], RAM(16), RAM(17), RAM(18), RAM(19), RAM(20), RAM(21), RAM(22), RAM(23), RAM(24), RAM(25), RAM(26), RAM(27), RAM(28), RAM(29), RAM(30), RAM(31), RAM_STATUS(4), RAM_STATUS(5), RAM_STATUS(6), RAM_STATUS(7),
			                                                                     RAM(32), RAM(33), RAM(34), RAM(35), RAM(36), RAM(37), RAM(38), RAM(39), RAM(40), RAM(41), RAM(42), RAM(43), RAM(44), RAM(45), RAM(46), RAM(47), RAM_STATUS(8), RAM_STATUS(9), RAM_STATUS(10), RAM_STATUS(11),
			  c->ACC, c->carry, c->test,                                         RAM(48), RAM(49), RAM(50), RAM(51), RAM(52), RAM(53), RAM(54), RAM(55), RAM(56), RAM(57), RAM(58), RAM(59), RAM(60), RAM(61), RAM(62), RAM(63), RAM_STATUS(12), RAM_STATUS(13), RAM_STATUS(14), RAM_STATUS(15),
			  c->PC, cycle, c->ROM[c->PC], c->ROM[c->PC + 1]);
#else
	printf(
			"\n"
			"    STACK             INDEX REGISTERS\n"
			"LEVEL 1: %03X     R0 R1: %X %X  R8 R9: %X %X\n"
			"LEVEL 2: %03X     R2 R3: %X %X  RA RB: %X %X\n"
			"LEVEL 3: %03X     R4 R5: %X %X  RC RD: %X %X\n"
			"                 R6 R7: %X %X  RE RF: %X %X\n"
			"\n"
			"         RAM BANK: %d  RAM CHIP: %d              STATUS\n"
			"            0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3\n"
			"REGISTER 0: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"REGISTER 1: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"REGISTER 2: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"REGISTER 3: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"\n"
			"ACCUMULATOR: %X     CARRY: %X  TEST: %X\n"
			"PC: %03X            CYCLES: %ld\n"
			"->: %.2X %.2X\n"
		 	,
			  c->STACK.addrs[0], c->IR[0x0], c->IR[0x1], c->IR[0x8], c->IR[0x9],
			  c->STACK.addrs[1], c->IR[0x2], c->IR[0x3], c->IR[0xA], c->IR[0xB],
			  c->STACK.addrs[2], c->IR[0x4], c->IR[0x5], c->IR[0xC], c->IR[0xD],
			                     c->IR[0x6], c->IR[0x7], c->IR[0xE], c->IR[0xF],
			  bank, chip,
			  RAM(0),  RAM(1),  RAM(2),  RAM(3),  RAM(4),  RAM(5),  RAM(6),  RAM(7),  RAM(8),  RAM(9), RAM(10), RAM(11), RAM(12), RAM(13), RAM(14), RAM(15), RAM_STATUS(0), RAM_STATUS(1), RAM_STATUS(2), RAM_STATUS(3),
			  RAM(16), RAM(17), RAM(18), RAM(19), RAM(20), RAM(21), RAM(22), RAM(23), RAM(24), RAM(25), RAM(26), RAM(27), RAM(28), RAM(29), RAM(30), RAM(31), RAM_STATUS(4), RAM_STATUS(5), RAM_STATUS(6), RAM_STATUS(7),
			  RAM(32), RAM(33), RAM(34), RAM(35), RAM(36), RAM(37), RAM(38), RAM(39), RAM(40), RAM(41), RAM(42), RAM(43), RAM(44), RAM(45), RAM(46), RAM(47), RAM_STATUS(8), RAM_STATUS(9), RAM_STATUS(10), RAM_STATUS(11),
			  RAM(48), RAM(49), RAM(50), RAM(51), RAM(52), RAM(53), RAM(54), RAM(55), RAM(56), RAM(57), RAM(58), RAM(59), RAM(60), RAM(61), RAM(62), RAM(63), RAM_STATUS(12), RAM_STATUS(13), RAM_STATUS(14), RAM_STATUS(15),
			  c->ACC, c->carry, c->test,
			  c->PC, cycle, c->ROM[c->PC], c->ROM[c->PC + 1]);
#endif

#undef RAM
#undef RAM_STATUS
}

#ifdef WIN32
static void _windows_cls() {
	HANDLE hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD count;
	DWORD cellCount;
	COORD homeCoords = { 0, 0 };

	hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	if (hStdOut == INVALID_HANDLE_VALUE)
		return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo( hStdOut, &csbi ))
		return;

	cellCount = csbi.dwSize.X * csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(hStdOut, (TCHAR) ' ', cellCount, homeCoords, &count))
		return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(hStdOut, csbi.wAttributes, cellCount, homeCoords, &count))
		return;

	/* Move the cursor home */
	SetConsoleCursorPosition( hStdOut, homeCoords );
}
#endif

void cli_clear_screen() {
#ifdef WIN32
	_windows_cls();
#else
	printf("\e[1;1H\e[2J");
#endif
}
