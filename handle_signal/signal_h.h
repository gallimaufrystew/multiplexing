
#ifndef TUNC_SIGNAL_INCLUDED_H
#define TUNC_SIGNAL_INCLUDED_H

#ifdef __linux__

#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

typedef struct {
	int sig_no;
	const char *sig_name;
	void (*handler)(int sig_no);
} signal_t;

int init_signals();
void signal_handler(int sig_no);

#elif _WIN32

#include <windows.h>

BOOL WINAPI signal_handler(DWORD ctrl_type);

#else

#endif //

// common headers
#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>

#endif // TUNC_SIGNAL_INCLUDED_H
