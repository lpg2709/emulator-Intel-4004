#ifndef CHIP4004_H
#define CHIP4004_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define ROM_PAGE_SIZE 255

typedef struct sct_chip_4004 chip_4004;

struct sct_chip_4004{
	uint8_t IR[16];               // On 4004 the index registers are 4 bits not 8
	uint8_t ROM[4096];	
	uint8_t RAM[4096*8];
	struct sct_stack{
		uint16_t addrs[3];        // Save the address, on 4004 are 12 bits not 16
		uint8_t SP:2;             // Stack Pointer
	}STACK;
	uint16_t PC;                  // on 4004 are 12 bits not 16
};

void init4004(chip_4004 *c);
bool loadProgramROM(chip_4004 *c, const char *program);


#endif
