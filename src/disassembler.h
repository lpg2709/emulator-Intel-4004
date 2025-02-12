#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdbool.h>
#include <inttypes.h>

#include "error.h"
#include "options_parser.h"

bool dis_byte_to_op(uint16_t byte, char op[4], uint8_t *word_size);
Error disassembler(Options *opt);

#endif // !DISASSEMBLER_H
