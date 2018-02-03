#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h> //uint8_t ...
#include <arpa/inet.h> // struct in_addr

typedef int (* processer)(char *data, int len); //

#pragma pack(1)
typedef struct _ether_hdr
{
    uint8_t dst_mac[6];
    uint8_t src_mac[6];
    uint16_t type;
} ether_hdr;

typedef struct _ip_hdr
{
#ifdef LITTLE_ENDIAN
    int hdr_len:4;
    int version:4;
#else
    int version:4;
    int hdr_len:4;
#endif //LITTLE_ENDIAN
    uint8_t tos;
    uint16_t tot_len;
    uint16_t id;
#ifdef LITTLE_ENDIAN
    int frag_off:13;
    int flags:3;
#else
    int flags:3;
    int frag_off:13;
#endif //LITTLE_ENDIAN
    uint8_t ttl;
    uint8_t protocol;
    struct in_addr srcaddr;
    struct in_addr dstaddr;
} ip_hdr;

typedef struct _icmp_hdr
{
    uint8_t type;
    uint8_t code;
    uint16_t csum;
} icmp_hdr;

typedef struct _arp_pack
{
    uint16_t hw_type;
    uint16_t protocol_type;
    uint8_t hw_addr_len;
    uint8_t protocol_addr_len;
    uint16_t op;
    uint8_t sender_eth_addr[6];
    struct in_addr sender_ip_addr;
    uint8_t target_eth_addr[6];
    struct in_addr target_ip_addr;
} arp_pack;

typedef struct _data
{
    union
    {
        ether_hdr lnk_hdr_eth;
    } lnk_hdr;
    union
    {
        arp_pack lnk_data_arp;
        //rarp_pack lnk_data_rarp;
        ip_hdr
        
    } lnk_data;
    
} eth_data;

#pragma pack()

typedef enum
{
    IP_PACKET                   = 0x0800,
    X75_PACKET                  = 0x0801,
    X25L3_PACKET                = 0x0805,
    ARP_PACKET                  = 0x0806,
    FRM_RE_ARP_PACKET           = 0x0808,
    ISIS_PACKET                 = 0x8000,
    RARP_PACKET                 = 0x8035,
    VLAN_PACKET                 = 0x8100,
    IPV6_PACKET                 = 0x86DD,
    IEEE8023_APCKET             = 0x8808,
    PPP_PACKET                  = 0x880B,
    PPPOE_DISCOVERY_PACKET      = 0x8863,
    PPPOE_SESS_PACKET           = 0x8864,
    IEEE8021X_PACKET            = 0x888E,
    IEEE80211I_PACKET           = 0x88A8,
    IEEE80211_PREAUTH_PACKET    = 0x88C7,
}linklayer_type;

typedef struct _linklayer_proc
{
    linklayer_type type;
    processer proc_func;
} linklayer_proc;

typedef enum
{
    //RESERVED  = 0x00, /* for IPV6 */
    ICMP_TYPE   = 0x01, 
    IGMP_TYPE   = 0x02,
    //STREAM_TYPE = 0x05,
    TCP_TYPE    = 0x06,
    //VOIP_TYPE = 0x0B,
    UDP_TYPE    = 0x11,
    //...
} ip_protocol_type;

typedef struct _ip_proc
{
    ip_protocol_type protocol;
    processer proc_func;
} ip_proc;

typedef enum
{
    NO_ERR = 0,
    INTER_BREAK,
    LNKLAY_PARSE_ERR,
    IPLAY_PARSE_ERR,
    ERR_TYPE_NUM,
} err_type;

int get_ether_type(char *data, int len);
int lnklay_ipv4_packet_processer(char *data, int len);
int lnklay_x75_packet_processer(char *data, int len);
int lnklay_x25l3_packet_processer(char *data, int len);
int lnklay_arp_packet_processer(char *data, int len);
int lnklay_frm_re_arp_packet_processer(char *data, int len);
int lnklay_isis_packet_processer(char *data, int len);
int lnklay_rarp_packet_processer(char *data, int len);
int lnklay_vlan_packet_processer(char *data, int len);
int lnklay_ipv6_packet_processer(char *data, int len);
int lnklay_ieee8023_packet_processer(char *data, int len);
int lnklay_ppp_packet_processer(char *data, int len);
int lnklay_pppoe_disc_packet_processer(char *data, int len);
int lnklay_pppoe_sess_packet_processer(char *data, int len);
int lnklay_ieee8021x_packet_processer(char *data, int len);
int lnklay_ieee80211i_packet_processer(char *data, int len);
int lnklay_ieee80211_preauth_packet_processer(char *data, int len);

int ipv4_icmp_packet_processer(char *data, int len);
int ipv4_igmp_packet_processer(char *data, int len);
int ipv4_tcp_packet_processer(char *data, int len);
int ipv4_udp_packet_processer(char *data, int len);

processer get_linklayer_processer(linklayer_type type);
processer get_ip_processer(char *data, int len);

void show_linklayer_data(ether_hdr *eth_data);
void show_iplayer_data(ip_hdr *ip_data);

void show_lnklay_arp_data(arp_pack *arp_data);

int linklayer_data_processer(char *data, int len);
int iplayer_data_processer(char *data, int len);

int data_processer(char *data, int len);

int process_err(err_type err);

#endif //_PARSER_H_