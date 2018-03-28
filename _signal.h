
#ifndef _SIGNAL2_H_
#define _SIGNAL2_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "afchip/_logc.h"

typedef struct {
	int sig_no;
	const char *sig_name;
	void (*handler)(int sig_no);
} signal_t;

int init_signals();
void signal_handler(int sig_no);

#endif

