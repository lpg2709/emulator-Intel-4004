#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdbool.h>
#include <inttypes.h>

#include "error.h"
#include "options_parser.h"

bool disassembler_get_opcode(uint8_t *bytes, char op[20], uint8_t *word_size);
Error disassembler(Options *opt);

#endif // !DISASSEMBLER_H
