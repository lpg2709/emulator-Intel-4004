#include "./4004_chip.h"

void init4004(chip_4004 *c){
	memset((void*) c->IR, 0, 16);
	memset((void*) c->ROM, 0, 4096);
	memset((void*) c->RAM, 0, 4096*8);
	memset((void*) c->STACK.addrs, 0, 3);
	c->STACK.SP = 0;
	c->PC = 0;
	c->ACC = 0;
	c->carry = false;
	c->test = false;
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

void chip_cycle(chip_4004 *c, uint32_t cycles_limit){
	uint32_t cycle = 0;

	while(cycle < cycles_limit){


	}
}

void opcode_nop(){
	return;
}

void opcode_jcn(chip_4004 *c, uint16_t opa){
	uint8_t C1 = (opa & 0x800) >> 11;
	uint8_t C2 = (opa & 0x400) >> 10;
	uint8_t C3 = (opa & 0x200) >> 9;
	uint8_t C4 = (opa & 0x100) >> 8;
	uint8_t addr = (opa & 0xFF) | (0xF << 8);

	uint8_t acc_is_0 = (c->ACC == 0);
	
	if((!C1 && ((acc_is_0 && C2) || (c->carry && C3) || (!c->test && C4))) || (C1 && ((acc_is_0 || !C2) && (c->carry || C3) && (c->test || !C4))))
		c->PC &= addr;
	else
		c->PC += 2;
}

void opcode_fim(chip_4004 *c, uint16_t opa){
	uint8_t IR_pair = (opa & 0xE00) >> 8;	
	uint8_t rom_data = (opa & 0xFF);

	c->IR[IR_pair] = (rom_data & 0xF0)>>4;
	c->IR[IR_pair+1] = (rom_data & 0xF);

	c->PC += 2;
}

void opcode_fin(chip_4004 *c, uint8_t opa){
	uint8_t IR_pair = (opa & 0xE);	
	uint16_t rom_addrs = (((c->PC + 1) % ROM_PAGE_SIZE == 1) ? ((c->PC+1) & 0xF00): (c->PC & 0xF00)) | (c->IR[0] << 4) | (c->IR[1]);
	c->IR[IR_pair] = (c->ROM[rom_addrs] & 0xF);
	c->IR[IR_pair+1] = (c->IR[rom_addrs] & 0xF0) >> 4;
	c->PC++;
}

void opcode_jin(chip_4004 *c, uint8_t opa){
	uint8_t IR_pair = (opa & 0xE);	
	c->PC  = (((c->PC + 1) % ROM_PAGE_SIZE == 1) ? ((c->PC+1) & 0xF00): (c->PC & 0xF00)) | (c->IR[IR_pair] << 4) | (c->IR[IR_pair + 1]);
}

void opcode_jun(chip_4004 *c, uint16_t opa){
	c->PC = (opa & 0XFFF);
}

void opcode_jms(chip_4004 *c, uint16_t opa){
	c->STACK.addrs[c->STACK.SP] = c->PC;
	c->STACK.SP == 2 ? c->STACK.SP = 0: c->STACK.SP++;	
	c->PC = (opa & 0XFFF);
}

void opcode_inc(chip_4004 *c, uint8_t opa){
	c->IR[opa & 0xF]++;
	c->PC++;
}

void opcode_isz(chip_4004 *c, uint16_t opa){
	uint8_t IR_addrs = (opa & 0xF00) >> 8;
	uint8_t rom_addrs = (opa & 0xFF);
	c->IR[IR_addrs]++;
	if(c->IR[IR_addrs] != 0)
		c->PC  = (((c->PC + 1) % ROM_PAGE_SIZE == 1) ? ((c->PC+1) & 0xF00): (c->PC & 0xF00)) | (rom_addrs);
	else
		c->PC += 2;
}

void opcode_add(chip_4004 *c, uint8_t opa){
	uint8_t IR_addrs = (opa & 0xF);
	c->ACC += c->IR[IR_addrs] + c->carry;
	c->carry = false;
	if(c->ACC & 0xF0){
		c->ACC &= 0xF;
		c->carry = true;
	}
	c->PC++;
}

void opcode_sub(chip_4004 *c, uint8_t opa){
	uint8_t IR_addrs = (opa & 0xF);
	c->ACC += ~c->IR[IR_addrs] + !c->carry;
	c->carry = false;
	if(c->ACC & 0xF0){
		c->ACC &= 0xF;
		c->carry = true;
	}
	c->PC++;
}

void opcode_ld(chip_4004 *c, uint8_t opa){
	c->ACC = c->IR[(opa & 0xF)];
	c->PC++;
}

void opcode_xch(chip_4004 *c, uint8_t opa){
	c->IR[(opa & 0xF)] = c->ACC;
	c->PC++;
}
