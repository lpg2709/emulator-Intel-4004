// Posix from: https://www.flipcode.com/archives/_kbhit_for_Linux.shtml
#include "term_io.h"

#include <stdio.h>

#ifdef WIN32
#include <windows.h>
#else
#include <sys/select.h>
#include <sys/ioctl.h>
#include <termios.h>
#define STDIN 0
#endif

#ifndef WIN32
static bool initialized = false;
static tcflag_t original_c_lflag = 0;

static bool posix_kbhit(){
	int bytes_waiting = 0;
	ioctl(STDIN, FIONREAD, &bytes_waiting);
	return bytes_waiting > 0;
}
#endif

void tio_init() {
#ifdef WIN32
	return;
#else
	if(!initialized) {
		struct termios term = { 0 };
		tcgetattr(STDIN, &term);
		original_c_lflag = term.c_lflag;
		term.c_lflag &= ~ICANON;
		tcsetattr(STDIN, TCSANOW, &term);
		setbuf(stdin, NULL);
		initialized = true;
	}
#endif
}

bool tio_kbhit() {
#ifdef WIN32
	return (bool) _kbhit();
#else
	return posix_kbhit();
#endif
}

void tio_deinit() {
#ifdef WIN32
	return;
#else
	if(initialized) {
		struct termios term = { 0 };
		tcgetattr(STDIN, &term);
		term.c_lflag = original_c_lflag;
		tcsetattr(STDIN, TCSANOW, &term);
		setbuf(stdin, NULL);
		initialized = false;
	}
#endif
}
