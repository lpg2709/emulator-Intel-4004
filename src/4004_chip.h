#ifndef CHIP4004_H
#define CHIP4004_H

#include <bits/stdint-uintn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define ROM_PAGE_SIZE 256
#define WORD_I 8

typedef struct sct_chip_4004 chip_4004;

struct sct_chip_4004{
	uint8_t IR[16];               // On 4004 the index registers are 4 bits not 8
	uint8_t ROM[4096];	
	uint8_t RAM[4096*8];
	uint8_t RAM_bank;
	struct sct_stack{
		uint16_t addrs[3];        // Save the address, on 4004 are 12 bits not 16
		uint8_t SP:2;             // Stack Pointer
	}STACK;
	uint16_t PC;                  // on 4004 are 12 bits not 16
	uint8_t ACC;
	bool carry;
	bool test;
};

void init4004(chip_4004 *c);
bool loadProgramROM(chip_4004 *c, const char *program);

void chip_cycle(chip_4004 *c, uint32_t cycles_limit);
void chip_reset(chip_4004 *c); 		// Reset all chip


/*
 * Instruction Set
 * */

void opcode_nop(chip_4004 *c); 					// 0x0000
void opcode_jcn(chip_4004 *c, uint16_t opa); 	// 0x1---
void opcode_fim(chip_4004 *c, uint16_t opa); 	// 0x2---
void opcode_fin(chip_4004 *c, uint8_t opa);  	// 0x3-0
void opcode_jin(chip_4004 *c, uint8_t opa); 	// 0x3-1
void opcode_jun(chip_4004 *c, uint16_t opa); 	// 0x4---
void opcode_jms(chip_4004 *c, uint16_t opa); 	// 0x5---
void opcode_inc(chip_4004 *c, uint8_t opa); 	// 0x6-
void opcode_isz(chip_4004 *c, uint16_t opa);    // 0x7---
void opcode_add(chip_4004 *c, uint8_t opa); 	// 0x8-
void opcode_sub(chip_4004 *c, uint8_t opa); 	// 0x9-
void opcode_ld(chip_4004 *c, uint8_t opa); 		// 0xA-
void opcode_xch(chip_4004 *c, uint8_t opa); 	// 0xB-
void opcode_bbl(chip_4004 *c, uint8_t opa); 	// 0xC-
void opcode_ldm(chip_4004 *c, uint8_t opa); 	// 0xD-
void opcode_clb(chip_4004 *c); 					// 0xF0
void opcode_clc(chip_4004 *c); 					// 0xF1
void opcode_iac(chip_4004 *c); 					// 0xF2
void opcode_cmc(chip_4004 *c); 					// 0xF3
void opcode_cma(chip_4004 *c); 					// 0xF4
void opcode_ral(chip_4004 *c); 					// 0xF5
void opcode_rar(chip_4004 *c); 					// 0xF6
void opcode_tcc(chip_4004 *c); 					// 0xF7
void opcode_dac(chip_4004 *c); 					// 0xF8
void opcode_tcs(chip_4004 *c); 					// 0xF9
void opcode_stc(chip_4004 *c); 					// 0xFA
void opcode_daa(chip_4004 *c); 					// 0xFB
void opcode_kbp(chip_4004 *c); 					// 0xFC
void opcode_dcl(chip_4004 *c); 					// 0xFD

#endif
