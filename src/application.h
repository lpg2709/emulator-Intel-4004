#ifndef APPLICATION_H
#define APPLICATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "./4004_chip.h"
#include "./gui_cli.h"

void application_init();
void application_main_loop(chip_4004 *c);
char application_input();

#endif
