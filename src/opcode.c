#include "opcode.h"

int opcode_get_word_size(Opcode opcode) {
#define X(code, value, name, words) if(opcode == (code)) {\
	return words;}
	X_OPCODE
#undef X
	return 0;
}

