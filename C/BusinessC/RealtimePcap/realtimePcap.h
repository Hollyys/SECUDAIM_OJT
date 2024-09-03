#ifndef REALTIMEPCAP_H
#define REALTIMEPCAP_H

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>

#define ROTATE 2

char* filename();
int capture();

#endif
