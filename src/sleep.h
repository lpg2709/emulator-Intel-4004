#ifndef SLEEP_H
#define SLEEP_H

#include <time.h>
#ifdef WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

void sleep_ms(int milliseconds);

#endif // SLEEP_H
