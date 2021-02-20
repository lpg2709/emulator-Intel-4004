#include "./gui_cli.h"

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
	int i, bank, chip;;
	
//	printf("            0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3\n");
//	printf("REGISTER 0: ");
	
	printf("------------------------------------------ RAM MEMORY -----------------------------------------\n");
	bank=chip=0;
	for(i = 0; i < 2048; i++){
		if(i == 0 || i%64 == 0){
			printf("\n\nBANK: %d  CHIP: %d\n", bank, chip);
		}
		if(i%64 == 0){
			chip++;
			if(chip%4 == 0){
				bank++;
				chip = 0;
			}
		}
		if(i%16 ==0)
			printf("\n");
		printf("%X ", c->RAM[i]);
	}
	printf("\n-----------------------------------------------------------------------------------------------\n");

}

void cli_screen_usage(){

}

void cli_screen_startup(){
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
		"Press ENTER  key to continue...\n");
	while(1){
		char c = getchar();
		if(c == '\n')
			break;
	}
			
}

void cli_main_view(chip_4004 *c, uint64_t cycle){
	printf(
			"\n"
			"    STACK             INDEX REGISTERS\n"
			"LEVEL 1: %03X     R0 R1: %X %X  R8 R9: %X %X\n"
			"LEVEL 2: %03X     R2 R3: %X %X  RA RB: %X %X\n"
			"LEVEL 3: %03X     R4 R5: %X %X  RC RD: %X %X\n"
			"                 R6 R7: %X %X  RE RF: %X %X\n"
			"\n"
			"ACCUMULATOR: %X     CARRY: %X  TEST: %X\n"
			"PC: %03X            CYCLES: %ld\n"
		 	, c->STACK.addrs[0], c->IR[0x0], c->IR[0x1], c->IR[0x8], c->IR[0x9],
			  c->STACK.addrs[1], c->IR[0x2], c->IR[0x3], c->IR[0xA], c->IR[0xB],
			  c->STACK.addrs[2], c->IR[0x4], c->IR[0x5], c->IR[0xC], c->IR[0xD],
			  c->IR[0x6], c->IR[0x7], c->IR[0xE], c->IR[0xF], 
			  c->ACC, c->carry, c->test,
			  c->PC, cycle);

	/*
	 * 
			"\n"
			"    STACK             INDEX REGISTERS              RAM BANK: 0  RAM CHIP: 0              STATUS\n"
			"LEVEL 1: %03X     R0 R1: %X %X  R8 R9: %X %X               0 1 2 3 4 5 6 7 8 9 A B C D E F | 0 1 2 3\n"
			"LEVEL 2: %03X     R2 R3: %X %X  RA RB: %X %X\n"
			"LEVEL 3: %03X     R4 R5: %X %X  RC RD: %X %X   REGISTER 0: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"                 R6 R7: %X %X  RE RF: %X %X   REGISTER 1: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"                                          REGISTER 2: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"ACCUMULATOR: %X     CARRY: %X  TEST: %X      REGISTER 3: %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X | %X %X %X %X\n"
			"PC: %03X            CYCLES: %ld\n"
	 * */
}
