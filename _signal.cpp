
#include "_signal.h"

sig_atomic_t sig_exit;

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
	signal_t          *sig;
	struct sigaction   sa;

	for (sig = signals; sig->sig_no != 0; sig++) {
		memset(&sa, 0, sizeof(struct sigaction));
		sa.sa_handler = sig->handler;
		sigemptyset(&sa.sa_mask);
		if (sigaction(sig->sig_no, &sa, NULL) == -1) {
			error_log( "sigaction (%s) fail", sig->sig_name);
			return -1;
		}
	}

	return 0;
}

