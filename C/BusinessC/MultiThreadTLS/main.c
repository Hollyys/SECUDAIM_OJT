// main.c
#include "threads.h"
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main() {
	setup_signal_handler();
	create_join_thread();

    return 0;
}
 
