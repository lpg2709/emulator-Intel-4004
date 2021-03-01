#ifndef GUI_CLI_H
#define GUI_CLI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./4004_chip.h"

void cli_d_print_rom_memory(chip_4004 *c);
void cli_d_print_all_ram(chip_4004 *c);
void cli_screen_usage();
void cli_screen_startup();

void cli_main_view(chip_4004 *c,int bank, int chip, uint64_t cycle);
char cli_main_input();

#endif
