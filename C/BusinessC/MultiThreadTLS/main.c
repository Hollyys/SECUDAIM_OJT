// main.c
#include "threads.h"
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

int main() {

	signal(SIGINT, signal_handler);
	create_join_thread();

    return 0;
}
 
