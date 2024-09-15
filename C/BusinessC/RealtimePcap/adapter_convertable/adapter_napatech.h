#ifndef ADAPTER_PCAP_H
#define ADAPTER_PCAP_H

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

#include "adapter.h"
#include "parser.h"
#include "daemon.h"
#include "signal.h"

#define ROTATE 10


int pcap_capture(Config* config);


#endif
