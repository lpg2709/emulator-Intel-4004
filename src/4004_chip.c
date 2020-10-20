#include "./4004_chip.h"
#include <stdio.h>
#include <string.h>
#include <strings.h>

void init4004(chip_4004 *c){
	memset((void*) c->IR, 0, 16);
	memset((void*) c->ROM, 0, 4096);
	memset((void*) c->RAM, 0, 4096*8);
	memset((void*) c->STACK.addrs, 0, 3);
	c->STACK.SP = 0;
	c->PC = 0;
}

bool loadProgramROM(chip_4004 *c, const char *program){
	int program_size = strlen(program);
	if(program_size >= 4096){
		printf("Load program to ROM error: the maximum size is 4096 bits, your code have %d bit(s).\n", program_size);
		return false;
	}
	memcpy(c->ROM, program, program_size);
	return true;
}
