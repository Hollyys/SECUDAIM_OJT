#include <stdio.h>
#include "realtimePcap.h"
#include "daemon.h"

int main() {

	daemonize();

	capture();
    
	return 0;
}

