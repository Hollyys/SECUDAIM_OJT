#include "adapter_dpdk.h"

void dpdk_init(int argc, char *argv[], u_int16_t port_id) {
    int ret;

    ret = rte_eal_init(argc, argv);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Error with EAL initialization\n");

    struct rte_eth_conf port_conf = {
        .rxmode = { .max_lro_pkt_size = RTE_ETHER_MAX_LEN }
    };

    ret = rte_eth_dev_configure(port_id, 1, 1, &port_conf);
    if (ret != 0)
        rte_exit(EXIT_FAILURE, "Error configuring the port\n");

    ret = rte_eth_rx_queue_setup(port_id, 0, 128, rte_eth_dev_socket_id(port_id), NULL,
                                 rte_pktmbuf_pool_create("MBUF_POOL", NUM_MBUFS, 32, 0,
                                                         RTE_MBUF_DEFAULT_BUF_SIZE, rte_socket_id()));
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Error setting up RX queue\n");

    ret = rte_eth_dev_start(port_id);
    if (ret < 0)
        rte_exit(EXIT_FAILURE, "Error starting the port\n");
}

int dpdk_capture(Config* config, uint16_t port_id) {
	pcap_t *handle;
    pcap_dumper_t *dumper;

	handle = pcap_open_dead(DLT_EN10MB, RTE_ETHER_MAX_LEN);

	if(signal(SIGTERM, signal_handler) == SIG_ERR)
	{
		printf("Fail to signal(SIGTERM): %m\n");
		return -1;
	}

	struct rte_mbuf *bufs[BURST_SIZE];
    struct pcap_pkthdr header;

	while(1)
    {
		char *FILE_SAVE_INFO = (char *)malloc(17 * sizeof(char));
		FILE_SAVE_INFO = init_file(config); 
		
        dumper = pcap_dump_open(handle, FILE_SAVE_INFO);
        if (dumper == NULL) {
            fprintf(stderr, "Couldn't open dump file: %s\n", pcap_geterr(handle));
            free(FILE_SAVE_INFO);
            return 1;
        }
        printf("Caturing Pcap...\n");
        time_t start_time = time(NULL);

        while (difftime(time(NULL), start_time) < 60) {
            uint16_t nb_rx = rte_eth_rx_burst(port_id, 0, bufs, BURST_SIZE);
            if (nb_rx > 0) {
				for (int j = 0; j < nb_rx; j++) {
                    struct rte_mbuf *mbuf = bufs[j];
                    header.ts.tv_sec = time(NULL);
                    header.ts.tv_usec = 0;
                    header.caplen = rte_pktmbuf_pkt_len(mbuf);
                    header.len = rte_pktmbuf_pkt_len(mbuf);
               		
					pcap_dump((u_char *)dumper, &header, rte_pktmbuf_mtod(mbuf, u_char *));
					rte_pktmbuf_free(mbuf);
				}
            }
        }

        pcap_dump_close(dumper);
        free(FILE_SAVE_INFO);
    }

    pcap_close(handle);
	rte_eth_dev_stop(port_id);
    rte_eth_dev_close(port_id);

    return 0;
}
