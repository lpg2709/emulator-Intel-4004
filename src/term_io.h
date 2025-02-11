#ifndef TERM_IO_H
#define TERM_IO_H

#include <stdbool.h>

void tio_init();
bool tio_kbhit();
void tio_deinit();

#endif // TERM_IO_H
