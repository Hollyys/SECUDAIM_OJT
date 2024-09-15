#ifndef DAEMON_H
#define DAEMON_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define trace(...) log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define fatal(...) log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void daemonize(void);

#endif
