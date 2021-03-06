
#include "tunc_signal.h"

#ifdef __linux__

volatile sig_atomic_t running = 0;

signal_t signals[] = {

	{SIGUSR1, "SIGUSR1", signal_handler},
	{SIGWINCH, "SIGWINCH", signal_handler},
	{SIGTERM, "SIGTERM", signal_handler},
	{SIGQUIT, "SIGQUIT", signal_handler},
	{SIGUSR2, "SIGUSR2", signal_handler},
	{SIGINT, "SIGINT", signal_handler},
	{SIGIO, "SIGIO", signal_handler},
	{SIGCHLD, "SIGCHLD", signal_handler},
	{SIGSYS, "SIGSYS", SIG_IGN},
	{SIGPIPE, "SIGPIPE", SIG_IGN},
	{SIGHUP, "SIGHUP", signal_handler},

	{0, NULL, NULL}
};

void signal_handler(int sig_no)
{
	signal_t *sig;

	for (sig = signals; sig->sig_no != 0; sig++) {
		if (sig->sig_no == sig_no) {
			break;
		}
	}

	switch (sig_no) {
	case SIGQUIT:
		break;
	case SIGTERM:
	case SIGINT:
		std::cout << "ctrl + c received\n"; 
		sig_exit = 1;
		break;
	case SIGWINCH:
		break;
	case SIGHUP:
		break;
	case SIGUSR1:
	case SIGUSR2:
		break;
	case SIGIO:
		break;
	case SIGCHLD:
		break;
	}
}

int init_signals()
{
	struct sigaction   sa = {};
	signal_t  *sig;

	for (sig = signals; sig->sig_no != 0; sig++) {
		sa.sa_handler = sig->handler;
		sigemptyset(&sa.sa_mask);
		if (sigaction(sig->sig_no, &sa, NULL) == -1) {
			std::cout << "sigaction " << sig->sig_name << " fail";
			return -1;
		}
	}

	return 0;
}

#elif _WIN32

volatile bool running = true;

BOOL WINAPI signal_handler(DWORD ctrl_type)
{
    switch (ctrl_type)
    {
    case CTRL_C_EVENT:
        std::cout << "ctrl + c received\n"; 
        running = false;
        return TRUE;
    default:
        return FALSE;
    }
}

#else

// other os

#endif

