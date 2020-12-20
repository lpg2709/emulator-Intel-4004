#include <stdio.h>
#include <stdlib.h>

#include "./src/4004_chip.h"
#include "./src/utils.h"

int main(){
	
	const char *ram_test = read_file("./src_asm/ram_test.o");
	chip_4004 myChip;
	init4004(&myChip);

	loadProgramROM(&myChip, ram_test);

	printf("\n%s\n", ram_test);

	return 0;
}
