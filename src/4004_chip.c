#include "./4004_chip.h"

#include <stdio.h>
#include <string.h>

void init4004(chip_4004 *c){
	chip_reset(c);
}

bool chip_load_rom(chip_4004 *c, uint8_t *program, uint32_t program_size){
	if(program_size >= 4096){
		printf("Load program to ROM error: the maximum size is 4096 bits, your code have %d bit(s).\n", program_size);
		return false;
	}
	memcpy(c->ROM, program, program_size);

	return true;
}

uint32_t chip_cycle(chip_4004 *c, uint32_t cycles_limit){
	uint32_t cycle = 0;
	uint16_t mOpcode;

	while(cycle < cycles_limit){
		mOpcode = c->ROM[c->PC];

		switch(mOpcode & 0xF0){
			case 0x00:
				opcode_nop(c);
				break;
			case 0x10:
				opcode_jcn(c, (mOpcode << 8 | c->ROM[c->PC + 1]) & 0x0FFF);
				cycle++;
				break;
			case 0x20:
				if((mOpcode & 0x0F) % 2 == 0){
					opcode_fim(c, (mOpcode << 8 | c->ROM[c->PC + 1]) & 0x0FFF);
					cycle++;
				}else{
					opcode_src(c, mOpcode & 0x0F);
				}
				break;
			case 0x30:
				switch(mOpcode & 0x01){
					case 0x00:
						opcode_fin(c, mOpcode & 0x0F);
						break;
					case 0x01:
						opcode_jin(c, mOpcode & 0x0F);
						break;
				}
				break;
			case 0x40:
				opcode_jun(c, (mOpcode << 8 | c->ROM[c->PC + 1]) & 0x0FFF);
				cycle++;
				break;
			case 0x50:
				opcode_jms(c, (mOpcode << 8 | c->ROM[c->PC + 1]) & 0x0FFF);
				cycle++;
				break;
			case 0x60:
				opcode_inc(c, mOpcode & 0x0F);
				break;
			case 0x70:
				opcode_isz(c, (mOpcode << 8 | c->ROM[c->PC + 1]) & 0x0FFF);
				cycle++;
				break;
			case 0x80:
				opcode_add(c, mOpcode & 0x0F);
				break;
			case 0x90:
				opcode_sub(c, mOpcode & 0x0F);
				break;
			case 0xA0:
				opcode_ld(c, mOpcode & 0x0F);
				break;
			case 0xB0:
				opcode_xch(c, mOpcode & 0x0F);
				break;
			case 0xC0:
				opcode_bbl(c, mOpcode & 0x0F);
				break;
			case 0xD0:
				opcode_ldm(c, mOpcode & 0x0F);
				break;
			case 0xF0:
				switch(mOpcode & 0x0F){
					case 0x0:
						opcode_clb(c);
						break;
					case 0x1:
						opcode_clc(c);
						break;
					case 0x2:
						opcode_iac(c);
						break;
					case 0x3:
						opcode_cmc(c);
						break;
					case 0x4:
						opcode_cma(c);
						break;
					case 0x5:
						opcode_ral(c);
						break;
					case 0x6:
						opcode_rar(c);
						break;
					case 0x7:
						opcode_tcc(c);
						break;
					case 0x8:
						opcode_dac(c);
						break;
					case 0x9:
						opcode_tcs(c);
						break;
					case 0xA:
						opcode_stc(c);
						break;
					case 0xB:
						opcode_daa(c);
						break;
					case 0xC:
						opcode_kbp(c);
						break;
					case 0xD:
						opcode_dcl(c);
						break;
				}
				break;
			case 0xE0:
				switch(mOpcode & 0x0F){
					case 0x0:
						opcode_wrm(c);
						break;
					case 0x1:
						opcode_wmp(c);
						break;
					case 0x2:
						opcode_wrr(c);
						break;
					case 0x4:
						opcode_wrN(c, (uint8_t)0x00);
						break;
					case 0x5:
						opcode_wrN(c, (uint8_t)0x01);
						break;
					case 0x6:
						opcode_wrN(c, (uint8_t)0x02);
						break;
					case 0x7:
						opcode_wrN(c, (uint8_t)0x03);
						break;
					case 0x8:
						opcode_sbm(c);
						break;
					case 0x9:
						opcode_rdm(c);
						break;
					case 0xA:
						opcode_rdr(c);
						break;
					case 0xB:
						opcode_adm(c);
						break;
					case 0xC:
						opcode_rdN(c, (uint8_t) 0x0C);
						break;
					case 0xD:
						opcode_rdN(c, (uint8_t) 0x0D);
						break;
					case 0xE:
						opcode_rdN(c, (uint8_t) 0x0E);
						break;
					case 0xF:
						opcode_rdN(c, (uint8_t) 0x0F);
						break;
				}
				break;
		}

		cycle++;
	}
	return cycle;
}

void chip_reset(chip_4004 *c){
	memset((void*) c->IR, 0, sizeof(uint8_t)*16);
	memset((void*) c->ROM, 0, sizeof(uint8_t)*4096);
	memset((void*) c->RAM, 0, sizeof(uint8_t)*2048);
	memset((void*) c->RAM_status, 0, sizeof(uint8_t)*512);
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
	c->IR[IR_addrs] = (c->IR[IR_addrs] + 1) & 0xF;
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
	uint8_t aux = c->IR[(opa & 0xF)];
	c->IR[(opa & 0xF)] = c->ACC;
	c->ACC = aux;
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
	if(c->ACC == 0){
		c->ACC = 0xF;
		c->carry = false;
	} else {
		c->ACC-=1;
		if(c->ACC > 15){
			c->carry = true;
		}
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
	uint8_t IR_pair = ((opa & 0x0E) >> 1) * 2;
	c->RAM_addrs = ((c->IR[IR_pair] << 4) | c->IR[IR_pair+1]);
	c->PC++;
}

void opcode_wrm(chip_4004 *c){
	c->RAM[c->RAM_addrs] = c->ACC;
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
	uint8_t s_register = ((c->RAM_addrs & 0xF0) >> 4) & 0x03;
	uint8_t s_chip = (((c->RAM_addrs & 0xF0) >> 4) & 0x0C) >> 2;
	c->RAM_status[(((4 * s_register) + (16*s_chip)) + (64 * c->RAM_bank)) + n] = c->ACC &0x0F;
	c->PC++;
}

void opcode_sbm(chip_4004 *c){
	c->ACC += ~c->RAM[(c->RAM_addrs*c->RAM_bank)] + !c->carry;
	c->carry = false;
	if(c->ACC & 0xF0){
		c->ACC &= 0xF;
		c->carry = true;
	}
	c->PC++;
}

void opcode_rdm(chip_4004 *c){
	c->ACC = c->RAM[(c->RAM_addrs)];
	c->PC++;
}

void opcode_rdr(chip_4004 *c){
	c->ACC = c->ROM_io;
	c->PC++;
}

void opcode_adm(chip_4004 *c){
	c->ACC += c->RAM[(c->RAM_addrs)] + c->carry;
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

