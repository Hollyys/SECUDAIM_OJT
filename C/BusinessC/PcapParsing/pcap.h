#ifndef PCAP_H
#define PCAP_Hi

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <netdb.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

char errbuf[PCAP_ERRBUF_SIZE];

void local_net();
void parse_packet(const struct pcap_pkthdr *header, const u_char *packet, FILE *output_file);
void parse(char *filename, FILE *output_file);

#endif
