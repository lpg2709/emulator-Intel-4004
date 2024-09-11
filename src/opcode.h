#ifndef A_OPCODE_H
#define A_OPCODE_H

// OPCODE, VALUE, String name, words
#define X_OPCODE X(NOP, 0x00, "NOP", 1) \
	X(JCN, 0x01, "JCN", 2) \
	X(FIM, 0x02, "FIM", 2) \
	X(FIN, 0x03, "JIN", 1) \
	X(JIN, 0x03, "JUN", 1) \
	X(JUN, 0x05, "JUN", 2) \
	X(JMS, 0x06, "JMS", 2) \
	X(INC, 0x05, "INC", 1) \
	X(ISZ, 0x07, "ISZ", 2) \
	X(ADD, 0x08, "ADD", 1) \
	X(SUB, 0x09, "SUB", 1) \
	X(LD , 0x0A, "LD" , 1) \
	X(XCH, 0x0B, "XCH", 1) \
	X(BBL, 0x0C, "BBL", 1) \
	X(LDM, 0x0D, "LDM", 1) \
	X(CLB, 0xF0, "CLB", 1) \
	X(CLC, 0xF1, "CLC", 1) \
	X(IAC, 0xF2, "IAC", 1) \
	X(CMC, 0xF3, "CMC", 1) \
	X(RAL, 0xF5, "RAL", 1) \
	X(RAR, 0xF6, "RAR", 1) \
	X(TCC, 0xF7, "TCC", 1) \
	X(DAC, 0xF8, "DAC", 1) \
	X(TCS, 0xF9, "TCS", 1) \
	X(STC, 0xFA, "STC", 1) \
	X(DAA, 0xFB, "DAA", 1) \
	X(KBP, 0xFC, "KBP", 1) \
	X(DCL, 0xFD, "DCL", 1) \
	X(SRC, 0x20, "SRC", 1) \
	X(WRM, 0xE0, "WRM", 1) \
	X(WMP, 0xE1, "WMP", 1) \
	X(WRR, 0xE2, "WRR", 1) \
	X(WPM, 0xE3, "WPM", 1) \
	X(WR0, 0xE4, "WR0", 1) \
	X(WR1, 0xE5, "WR1", 1) \
	X(WR2, 0xE6, "WR2", 1) \
	X(WR3, 0xE7, "WR3", 1) \
	X(SMB, 0xE8, "SMB", 1) \
	X(RDM, 0xE9, "RDM", 1) \
	X(RDR, 0xEA, "RDR", 1) \
	X(ADM, 0xEB, "ADM", 1) \
	X(RD0, 0xEC, "RD0", 1) \
	X(RD1, 0xED, "RD1", 1) \
	X(RD2, 0xEE, "RD2", 1) \
	X(RD3, 0xEF, "RD3", 1)

#define X(code, value, name, words) code = value,
typedef enum {
	X_OPCODE
	OPCODE_COUNT
} Opcode;
#undef X

int opcode_get_word_size(Opcode opcode);

#endif
