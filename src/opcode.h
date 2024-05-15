#ifndef A_OPCODE_H
#define A_OPCODE_H

// OPCODE, VALUE, String name, operands
#define X_OPCODE X(NOP, 0x00, "NOP", 0) \
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
	X(CLB, 0xF0, "CLB", 0) \
	X(CLC, 0xF1, "CLC", 0) \
	X(IAC, 0xF2, "IAC", 0) \
	X(CMC, 0xF3, "CMC", 0) \
	X(RAL, 0xF5, "RAL", 0) \
	X(RAR, 0xF6, "RAR", 0) \
	X(TCC, 0xF7, "TCC", 0) \
	X(DAC, 0xF8, "DAC", 0) \
	X(TCS, 0xF9, "TCS", 0) \
	X(STC, 0xFA, "STC", 0) \
	X(DAA, 0xFB, "DAA", 0) \
	X(KBP, 0xFC, "KBP", 0) \
	X(DCL, 0xFD, "DCL", 0) \
	X(SRC, 0x20, "SRC", 1) \
	X(WRM, 0xE0, "WRM", 0) \
	X(WMP, 0xE1, "WMP", 0) \
	X(WRR, 0xE2, "WRR", 0) \
	X(WPM, 0xE3, "WPM", 0) \
	X(WR0, 0xE4, "WR0", 0) \
	X(WR1, 0xE5, "WR1", 0) \
	X(WR2, 0xE6, "WR2", 0) \
	X(WR3, 0xE7, "WR3", 0) \
	X(SMB, 0xE8, "SMB", 0) \
	X(RDM, 0xE9, "RDM", 0) \
	X(RDR, 0xEA, "RDR", 0) \
	X(ADM, 0xEB, "ADM", 0) \
	X(RD0, 0xEC, "RD0", 0) \
	X(RD1, 0xED, "RD1", 0) \
	X(RD2, 0xEE, "RD2", 0) \
	X(RD3, 0xEF, "RD3", 0)

#define X(code, value, name, operands) code = value,
typedef enum {
	X_OPCODE
} Opcode;
#undef X


#endif
