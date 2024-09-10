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

#define snc_trace(...) snc_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define snc_debug(...) snc_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define snc_info(...)  snc_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define snc_warn(...)  snc_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define snc_error(...) snc_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define snc_fatal(...) snc_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

#define snc_trace_rl(...) snc_log_rl(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define snc_debug_rl(...) snc_log_rl(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define snc_info_rl(...)  snc_log_rl(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define snc_warn_rl(...)  snc_log_rl(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define snc_error_rl(...) snc_log_rl(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define snc_fatal_rl(...) snc_log_rl(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void daemonize(void);

#endif
