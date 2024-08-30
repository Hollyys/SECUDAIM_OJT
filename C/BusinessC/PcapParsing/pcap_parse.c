// pcap_parse.c

#include "pcap_parse.h"

void local_net()
{
	char *device;
	char *network;     
	char *mask;         
	bpf_u_int32 netp;
	bpf_u_int32 maskp;
	struct in_addr addr;

	printf("--------- Local Network Status -----------\n");
	device = pcap_lookupdev(errbuf);
	if(device == NULL)
	{
		printf("%s\n",errbuf);
		exit(1);
	}

	printf("Network interface name: %s\n",device);			

	if(pcap_lookupnet(device, &netp, &maskp, errbuf) == -1)
	{
		printf("%s\n",errbuf);
		exit(1);
	}
	addr.s_addr = netp;
	network = inet_ntoa(addr);
	if(network == NULL)
	{
		perror("inet_ntoa");
		exit(1);
	}
	printf("IP Adress: %s\n",network);

	addr.s_addr = maskp;
	mask = inet_ntoa(addr);
	if(mask == NULL)
	{
		perror("inet_ntoa");
		exit(1);
	}
  
	printf("Subnet Mask: %s\n",mask);
	printf("------------------------------------------\n");
}

void parse_packet(const struct pcap_pkthdr *header, const u_char *packet, FILE *output_file)
{
		struct ip *ip_header;
		struct tcphdr *tcp_header;
		struct protoent *l4proto;
		u_int16_t src_port, dst_port;

		ip_header = (struct ip *)(packet + 14);

		if(ip_header->ip_p == IPPROTO_TCP){
			tcp_header = (struct tcphdr *)(packet + 14 + ip_header->ip_hl * 4);
			src_port = ntohs(tcp_header->source);
			dst_port = ntohs(tcp_header->dest);

			
			// printf("%s:%u -> ", inet_ntoa(ip_header->ip_src), src_port);
			// printf("%s:%u ", inet_ntoa(ip_header->ip_dst), dst_port);
			fprintf(output_file, "%s:%u -> ", inet_ntoa(ip_header->ip_src), src_port);
			fprintf(output_file, "%s:%u ", inet_ntoa(ip_header->ip_dst), dst_port);
			
			l4proto = getprotobynumber(ip_header->ip_p);
			// printf("[%s]\n", l4proto->p_name);
			fprintf(output_file, "[%s]\n", l4proto->p_name);
		}
}
void parse(char *filename, FILE *output_file)
{
		struct pcap_pkthdr header;
		const u_char *packet;
		pcap_t *handle;

		handle = pcap_open_offline(filename, errbuf);
		if(handle == NULL){
			exit(EXIT_FAILURE);
		}

		while((packet = pcap_next(handle, &header)) != NULL){
			parse_packet(&header, packet, output_file);
		}

		pcap_close(handle);
}

