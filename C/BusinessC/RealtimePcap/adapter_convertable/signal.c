#include "signal.h"

void signal_handler(int sig)
{
	if(sig == SIGTERM){
		// do something
		const char *message = "SIGTERM RECEIVED.\n";
		write(STDOUT_FILENO, message, 20);
	}
	exit(0);
}
