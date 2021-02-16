#ifndef CHIP4004_H
#define CHIP4004_H

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
	uint8_t RAM_status[8];
	uint8_t RAM_bank;
	uint8_t RAM_addrs;
	uint8_t RAM_output[4*8];        // on 4004 are 4 bits not 8, all RAM banks have 1 output port
	uint8_t ROM_io;                // on 4004 are 4 bits not 8
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

void opcode_nop(chip_4004 *c);					// 0x0000
void opcode_jcn(chip_4004 *c, uint16_t opa);	// 0x1---
void opcode_fim(chip_4004 *c, uint16_t opa);	// 0x2---
void opcode_fin(chip_4004 *c, uint8_t opa);		// 0x3-0
void opcode_jin(chip_4004 *c, uint8_t opa);		// 0x3-1
void opcode_jun(chip_4004 *c, uint16_t opa);	// 0x4---
void opcode_jms(chip_4004 *c, uint16_t opa);	// 0x5---
void opcode_inc(chip_4004 *c, uint8_t opa);		// 0x6-
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
void opcode_src(chip_4004 *c, uint8_t opa);     // 0x2-
void opcode_wrm(chip_4004 *c);					// 0xE0
void opcode_wmp(chip_4004 *c);                  // 0xE1
void opcode_wrr(chip_4004 *c);                  // 0xE2
void opcode_wrN(chip_4004 *c, uint8_t n);       // 0xE4--0xE7
/* 
 * wr0  -  wr1 -  wr2 -  wr3
 * 0xE4 - 0xE5 - 0xE6 - 0xE7
 */
void opcode_smb(chip_4004 *c);                  // 0xE8
void opcode_rdm(chip_4004 *c);                  // 0xE9
void opcode_rdr(chip_4004 *c);                  // 0xEA
void opcode_adm(chip_4004 *c);                  // 0xEB
void opcode_rdN(chip_4004 *c, uint8_t n);       // 0xEC--0xEF
/* 
 * rd0  -  rd1 -  rd2 -  rd3
 * 0xEC - 0xED - 0xEE - 0xEF
 */

#endif

