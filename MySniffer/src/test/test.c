#include <stdio.h>
#include <linux/if_ether.h> //ETH_P_ALL,
#include <netinet/in.h> //struct sockadd_in, 
#include <netpacket/packet.h> //struct sockadd_ll,
#include "comm.h"
#include "common.h"
#include "parser.h"
#include "interface.h"

int send_arp_packet(char *ifname, int sock_fd)
{
    char *buf = NULL;
    ether_hdr *p_eth = NULL;
    arp_pack *p_arp = NULL;
    struct sockaddr_ll sll;
    int i = 0;
    int len = 0;

    if (-1 == get_netif_addr(ifname, sock_fd, &sll))
        return -1;

    buf = (char *)zelloc(sizeof(ether_hdr)+sizeof(arp_pack));
    if (NULL == buf)
        return -1;

    p_eth = (ether_hdr *)buf;

    p_eth->dst_mac[0] = 0xFF;
    p_eth->dst_mac[1] = 0xFF;
    p_eth->dst_mac[2] = 0xFF;
    p_eth->dst_mac[3] = 0xFF;
    p_eth->dst_mac[4] = 0xFF;
    p_eth->dst_mac[5] = 0xFF;
    
    p_eth->src_mac[0] = 0x00;
    p_eth->src_mac[1] = 0x0c;
    p_eth->src_mac[2] = 0x29;
    p_eth->src_mac[3] = 0x09;
    p_eth->src_mac[4] = 0x0f;
    p_eth->src_mac[5] = 0xa3;
    
    p_eth->type = htons(0x0806);

    p_arp = (arp_pack *)(buf + sizeof(ether_hdr));

    p_arp->hw_type = htons(0x0001);
    p_arp->protocol_type = htons(0x0800);
    p_arp->hw_addr_len = 0x06;
    p_arp->protocol_addr_len = 0x04;
    p_arp->op = htons(0x0001);

    p_arp->sender_eth_addr[0] = 0x00;
    p_arp->sender_eth_addr[1] = 0x0c;
    p_arp->sender_eth_addr[2] = 0x29;
    p_arp->sender_eth_addr[3] = 0x09;
    p_arp->sender_eth_addr[4] = 0x0f;
    p_arp->sender_eth_addr[5] = 0xa3;

    p_arp->sender_ip_addr.s_addr = inet_addr("192.168.27.255");

    p_arp->target_eth_addr[0] = 0x00;
    p_arp->target_eth_addr[1] = 0x00;
    p_arp->target_eth_addr[2] = 0x00;
    p_arp->target_eth_addr[3] = 0x00;
    p_arp->target_eth_addr[4] = 0x00;
    p_arp->target_eth_addr[5] = 0x00;

    p_arp->target_ip_addr.s_addr = inet_addr("192.168.1.1");

    len = sizeof(ether_hdr)+sizeof(arp_pack);

    hex_dump(buf, len);

    for (i = 0; i < 50; i++)
    {
        if (len > send_data_to(sock_fd, buf, len, 0, (struct sockaddr *)&sll, sizeof(sll)))
            return -1;    
    }

    free_mem(buf);

    return 0;
}

int t_get_netif_addr(char *ifname, int r_sock_fd)
{
    struct sockaddr_ll sll;
    
    if ((NULL == ifname) || (0 > r_sock_fd))
        return -1;

    if (-1 == get_netif_addr(ifname, r_sock_fd, &sll))
        return -1;

    printf("ifindex: %x\n", sll.sll_ifindex);
    return 0;
    
}

int main(int argc, char *argv[])
{
    int err = 0;
    int sock_fd = -1;

    sock_fd = create_socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (0 > sock_fd)
    {
        err = -1;
        goto end;
    }

    send_arp_packet(argv[1], sock_fd);
#if 0
    if (-1 == (err = t_get_netif_addr(argv[1], sock_fd)))
    {
        err = 1;
        goto end;
    }
#endif //0

end:
    destroy_socket(sock_fd);
    return err;
}

