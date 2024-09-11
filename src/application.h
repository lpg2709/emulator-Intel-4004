#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdint.h>
#include <stdbool.h>

#include "./4004_chip.h"
#include "./options_parser.h"

void application_init(Options opt, chip_4004 *c);
void application_main_loop(chip_4004 *c);
char application_input();

void keyboard_press_enter();
int keyboard_read_int();
char keyboard_read_char();



#endif
