#include <stdint.h>

struct  packet_info
{
    /* data */
    char* payload;
    char[] recv_date;
    uint32_t size;
    int src_port;
    int dst_port;
    enum protocol_number;
};

void main
{
    struct packet_info packet;
    packet -> payload = "qjwekjqlkwjeljas;dkjasdjk;jaksldjoqwej";
    packet -> recv_date = "12/07/09";
    packet.size = 5;
    packet.src_port = 10;
    packet.dst_port
    packet.protocol_number
}
