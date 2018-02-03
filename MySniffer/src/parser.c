#include <stdio.h> //NULL
//#include <arpa/inet.h> //ntohs()... /* included in parser.h */
#include <sys/socket.h> //inet_ntoa(),inet_aton(),...
#include <netinet/in.h> //inet_ntoa(),inet_aton(),...
#include "common.h"
#include "parser.h"

linklayer_proc linklayer_proc_table[] = 
{
    {IP_PACKET, lnklay_ipv4_packet_processer}, /* IPV4 */
    //{X75_PACKET, x75_packet_processer},
    //{X25L3_PACKET, x25l3_packet_processer},
    {ARP_PACKET, lnklay_arp_packet_processer}, /* ARP */
    //{FRM_RE_ARP_PACKET, frm_re_arp_packet_processer},
    //{ISIS_PACKET, isis_packet_processer},
    {RARP_PACKET, lnklay_rarp_packet_processer}, /* RARP */
    //{VLAN_PACKET, vlan_packet_processer},
    {IPV6_PACKET, lnklay_ipv6_packet_processer}, /* IPV6 */
    //{IEEE8023_APCKET, ieee8023_packet_processer},
    //{PPP_PACKET, ppp_packet_processer},
    //{PPPOE_DISCOVERY_PACKET, pppoe_disc_packet_processer},
    //{PPPOE_SESS_PACKET, pppoe_sess_packet_processer},
    //{IEEE8021X_PACKET, ieee8021x_packet_processer},
    //{IEEE80211I_PACKET, ieee80211i_packet_processer},
    //{IEEE80211_PREAUTH_PACKET, ieee80211_preauth_packet_processer}
};

ip_proc ip_proc_table[] =
{
    {ICMP_TYPE, ipv4_icmp_packet_processer},
    {IGMP_TYPE, ipv4_igmp_packet_processer},
    {TCP_TYPE, ipv4_tcp_packet_processer},
    {UDP_TYPE, ipv4_udp_packet_processer}
};

int get_ether_type(char *data, int len)
{
    ether_hdr *e_hdr = (ether_hdr *)data;

    if ((NULL == e_hdr) || (0 >= len))
    {
        return -1;
    }
    else
        return (int)(ntohs(e_hdr->type));
}

int get_ip_protocol(char *data, int len)
{
    ip_hdr *p_ip_hdr = (ip_hdr *)data;

    if ((NULL == p_ip_hdr) || (0 >= len))
        return -1;

#ifdef DEBUG
    printf("protocol: 0x%x\n", (p_ip_hdr->protocol));
#endif //DEBUG
    return (ip_protocol_type)(p_ip_hdr->protocol);
}

int lnklay_ipv4_packet_processer(char * data, int len)
{
    printf("Internet Protocol Version 4\n");
    return 0;
}

int lnklay_x75_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_x25l3_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_arp_packet_processer(char *data, int len)
{
    if ((NULL == data) || (0 >= len))
        return -1;

    data += sizeof(ether_hdr); /* jump ether head */

    show_lnklay_arp_data((arp_pack *)data);
    
    return (int)INTER_BREAK;
}
int lnklay_frm_re_arp_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_isis_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_rarp_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_vlan_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_ipv6_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_ieee8023_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_ppp_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_pppoe_disc_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_pppoe_sess_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_ieee8021x_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_ieee80211i_packet_processer(char *data, int len)
{
    return 0;
}
int lnklay_ieee80211_preauth_packet_processer(char *data, int len)
{
    return 0;
}

processer get_linklayer_processer(linklayer_type type)
{
    int index = 0;
    processer proc_func = NULL;

    GET_DATADRIV_RES(index, type, proc_func, linklayer_proc_table, type, proc_func);

    return proc_func;
}

int ipv4_icmp_packet_processer(char *data, int len)
{
    return 0;
}
int ipv4_igmp_packet_processer(char *data, int len)
{
    return 0;
}
int ipv4_tcp_packet_processer(char *data, int len)
{
    printf("---TCP_PACKET---\n");
    return 0;
}
int ipv4_udp_packet_processer(char *data, int len)
{
    printf("---UDP_PACKET---\n");
    return 0;
}

processer get_ip_processer(char *data, int len)
{
    int index = 0;
    int protocol = 0;
    processer proc_func = NULL;

    if ((NULL == data) || (0 >= len))
        return NULL;

    if (0 > (protocol = get_ip_protocol(data, len)))
        return NULL;
    
    GET_DATADRIV_RES(index, protocol, proc_func, ip_proc_table, protocol, proc_func);

    return proc_func;
}

int linklayer_data_processer(char *data, int len) 
{
    int err = 0;
    linklayer_type type = 0;
    processer linklayer_proc_func = NULL;
    
    if ((NULL == data) || (0 >= len))
        return -1;
    if (0 > (type = get_ether_type(data, len)))
        return -1;
    
    show_linklayer_data((ether_hdr *)data);

    /* linklayer data process */
    if (NULL == (linklayer_proc_func = get_linklayer_processer(type)))
    {
        printf("Parse ether packet fail\n");
        return -1;
    }
    if (0 != (err = linklayer_proc_func(data, len))) 
        return err;

    return 0;
}

int iplayer_data_processer(char *data, int len)
{
    processer ip_proc_func = NULL;

    if ((NULL == data) || (0 >= len))
        return -1;
    
    if (NULL == (ip_proc_func = get_ip_processer(data, len)))
    {
        printf("Parse iplayer data fail.");
        return -1;
    }
    if (0 > ip_proc_func(data+sizeof(ether_hdr), len-sizeof(ether_hdr)))
    {
        printf("Deal ip packet fail\n");
        return -1;
    }
    
    return 0;
}

void show_linklayer_data(ether_hdr *eth_data)
{
    printf("Ethernet II Data:\n");
    printf("--Destination: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                                             eth_data->dst_mac[0],
                                             eth_data->dst_mac[1],
                                             eth_data->dst_mac[2],
                                             eth_data->dst_mac[3],
                                             eth_data->dst_mac[4],
                                             eth_data->dst_mac[5]);
    
    printf("--Source: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                                        eth_data->src_mac[0],
                                        eth_data->src_mac[1],
                                        eth_data->src_mac[2],
                                        eth_data->src_mac[3],
                                        eth_data->src_mac[4],
                                        eth_data->src_mac[5]);
    printf("--Type: 0x%04x\n", ntohs(eth_data->type));
    
}

void show_lnklay_arp_data(arp_pack * arp_data)
{   
    printf("ARP Data:\n");
    printf("--Type of hardware: 0x%04x:\n", ntohs(arp_data->hw_type));
    printf("--Type of protocol: 0x%04x:\n", ntohs(arp_data->protocol_type));
    printf("--Length of hardware address: 0x%02x\n", arp_data->hw_addr_len);
    printf("--Length of protocol address: 0x%02x\n", arp_data->protocol_addr_len);
    printf("--Operation code: 0x%04x\n", ntohs(arp_data->op));
    printf("--Sender: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                                        arp_data->sender_eth_addr[0],
                                        arp_data->sender_eth_addr[1],
                                        arp_data->sender_eth_addr[2],
                                        arp_data->sender_eth_addr[3],
                                        arp_data->sender_eth_addr[4],
                                        arp_data->sender_eth_addr[5]);
    printf("--Sender ip address: %s\n", inet_ntoa(arp_data->sender_ip_addr));    
    printf("--Target: %02x:%02x:%02x:%02x:%02x:%02x\n", 
                                        arp_data->target_eth_addr[0],
                                        arp_data->target_eth_addr[1],
                                        arp_data->target_eth_addr[2],
                                        arp_data->target_eth_addr[3],
                                        arp_data->target_eth_addr[4],
                                        arp_data->target_eth_addr[5]);
    printf("--Target ip address: %s\n", inet_ntoa(arp_data->sender_ip_addr));
}

void show_iplayer_data(ip_hdr *eth_data)
{
}

int process_err(err_type err)
{
    int res = 0;
    
    switch (err)
    {
        case NO_ERR:
            break;
        case INTER_BREAK:
            res = -1;
            break;
        case LNKLAY_PARSE_ERR:
            printf("Parse linklayer data fail.\n");
            res = -1;
            break;
        case IPLAY_PARSE_ERR:
            printf("Parse iplayer data fail.\n");
            res = -1;
            break;
        default:
            break;
    }

    return res;
}

int data_processer(char *data, int len)
{
    int err = 0;
    
    if ((NULL == data) || (0 >= len))
        return -1;

    /* linklayer data process */
    if (0 != (err = linklayer_data_processer(data, len)))
    {
        return err;
    }

    /*jump ether head*/
    data += sizeof(ether_hdr);
    len -= sizeof(ether_hdr);

    /* iplayer data process */
    if (0 != (err = iplayer_data_processer(data, len)))
    {
        return err;
    }

    return err;
}
