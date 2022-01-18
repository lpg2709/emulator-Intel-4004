#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./4004_chip.h"
#include "./gui_cli.h"
#include "./options_parser.h"
#include "./files.h"
#include "./version.h"

void application_init(Options opt, chip_4004 *c);
void application_main_loop(chip_4004 *c);
char application_input();

void keyboard_press_enter();
int keyboard_read_int();
char keyboard_read_char();



#endif
