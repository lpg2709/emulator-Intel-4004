#include "./4004_chip.h"

void init4004(chip_4004 *c){
	chip_reset(c);
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

void chip_reset(chip_4004 *c){
	memset((void*) c->IR, 0, sizeof(uint8_t)*16);
	memset((void*) c->ROM, 0, sizeof(uint8_t)*4096);
	memset((void*) c->RAM, 0, sizeof(uint8_t)*4096*8);
	memset((void*) c->RAM_status, 0, sizeof(uint8_t)*8);
	memset((void*) c->STACK.addrs, 0, sizeof(uint16_t)*3);
	memset((void*) c->RAM_output, 0, sizeof(uint8_t)*4*8);
	c->STACK.SP = 0;
	c->RAM_bank = 0;
	c->RAM_addrs = 0;
	c->ROM_io = 0;
	c->PC = 0;
	c->ACC = 0;
	c->carry = false;
	c->test = false;
}

void opcode_nop(chip_4004 *c){
	c->PC++;
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

void opcode_bbl(chip_4004 *c, uint8_t opa){
	c->ACC = (opa & 0xF);
	c->PC = c->STACK.addrs[c->STACK.SP];
	c->STACK.SP <= 0 ? c->STACK.SP = 0: c->STACK.SP--;	
}

void opcode_ldm(chip_4004 *c, uint8_t opa){
	c->ACC = (opa & 0xF);
	c->PC++;
}

void opcode_clb(chip_4004 *c){
	c->ACC = 0x0;
	c->carry = false;
	c->PC++;
}

void opcode_clc(chip_4004 *c){
	c->carry = false;
	c->PC++;
}

void opcode_iac(chip_4004 *c){
	c->ACC++;
	if(c->ACC > 15){
		c->ACC = 0;
		c->carry = true;
	}
	c->PC++;
}

void opcode_cmc(chip_4004 *c){
	c->carry = !c->carry;
	c->PC++;
}

void opcode_cma(chip_4004 *c){
	c->ACC ^= c->ACC;
	c->PC++;
}

void opcode_ral(chip_4004 *c){
	c->ACC = (c->ACC | c->carry << 4) << 1;
	if((c->ACC & 0xF0) == 0)
		c->carry = false;
	else
		c->carry = true;
	c->ACC = c->ACC & 0xF;
	c->PC++;
}

void opcode_rar(chip_4004 *c){
	uint8_t temp = c->ACC & 1;
	c->ACC = (c->ACC>>1 | c->carry <<3 );
	c->carry = temp;
	c->PC++;
}

void opcode_tcc(chip_4004 *c){
	c->ACC = (int) c->carry;
	c->carry = false;
	c->PC++;
}

void opcode_dac(chip_4004 *c){
	c->ACC-=1;
	if(c->ACC < 0){
		c->ACC = 0xF;
		c->carry = false;
	}
	if(c->ACC > 15){
		c->ACC = 0;
		c->carry = true;
	}
	c->PC++;
}

void opcode_tcs(chip_4004 *c){
	if(c->carry == false)
		c->ACC = 0x9;
	else
		c->ACC = 0xA;
	c->carry = false;
	c->PC++;
}

void opcode_stc(chip_4004 *c){
	c->carry = true;
	c->PC++;
}

void opcode_daa(chip_4004 *c){
	if(c->ACC > 9 || c->carry == true){
		c->ACC+=6;
		if(c->ACC > 15){
			c->ACC = 0;
			c->carry = true;
		}
	 }
	c->PC++;
}

void opcode_kbp(chip_4004 *c){
	if(c->ACC != 0){
		switch (c->ACC) {
			case 0:
				c->ACC = 0x0; // 0b0000
				break;
			case 1:
				c->ACC = 0x1; // 0b0001
				break;
			case 2:
				c->ACC = 0x2; // 0b0010
				break;
			case 4:
				c->ACC = 0x3; // 0b0011
				break;
			case 8:
				c->ACC = 0x4; // 0b0100
				break;
			default:
				c->ACC = 0xFF; // 0b1111
				break;
		}
	}
	c->PC++;
}

void opcode_dcl(chip_4004 *c){
	c->RAM_bank =  c->ACC & 0x7;
	c->PC++;
}

void opcode_src(chip_4004 *c, uint8_t opa){
	uint8_t IR_pair = (opa & 0x0F) >> 1;
	c->RAM_addrs = ((c->IR[IR_pair]) << 4) || c->IR[IR_pair+1];
	c->PC++;
}

void opcode_wrm(chip_4004 *c){
	c->RAM[(c->RAM_addrs*c->RAM_bank)] = c->ACC;
	c->PC++;
}

void opcode_wmp(chip_4004 *c){
	c->RAM_output[c->RAM_bank] = c->ACC & 0x0F;
	c->PC++;
}

void opcode_wrr(chip_4004 *c){
	c->ROM_io = c->ACC & 0x0F;
	c->PC++;
}

void opcode_wrN(chip_4004 *c, uint8_t n){
	c->RAM_status[(n*4 + c->RAM_bank)] = c->ACC &0x0F;
	c->PC++;
}

void opcode_smb(chip_4004 *c){
	c->ACC += ~c->RAM[(c->RAM_addrs*c->RAM_bank)] + !c->carry;
	c->carry = false;
	if(c->ACC & 0xF0){
		c->ACC &= 0xF;
		c->carry = true;
	}
	c->PC++;
}

void opcode_rdm(chip_4004 *c){
	c->ACC = c->RAM[(c->RAM_addrs*c->RAM_bank)];
	c->PC++;
}

void opcode_rdr(chip_4004 *c){
	c->ACC = c->ROM_io;
	c->PC++;
}

void opcode_adm(chip_4004 *c){
	c->ACC += c->RAM[(c->RAM_addrs*c->RAM_bank)] + c->carry;
	if(c->ACC & 0xF0){
		c->ACC &= 0xF;
		c->carry = true;
	}
	c->PC++;
}

void opcode_rdN(chip_4004 *c, uint8_t n){
	c->ACC = c->RAM_status[(n*4 + c->RAM_bank)];
	c->PC++;
}

