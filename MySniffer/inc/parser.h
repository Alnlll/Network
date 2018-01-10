#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdint.h> //uint8_t ...

typedef int (* processer)(char *data, int len);

#pragma pack(1)
typedef struct _ether_hdr
{
	uint8_t dst_mac[6];
	uint8_t src_mac[6];
	uint16_t type;
} ether_hdr;
#pragma pack()

typedef enum
{
	IP_PACKET 					= 0x0800,
	X75_PACKET					= 0x0801,
	X25L3_PACKET	 			= 0x0805,
	ARP_PACKET 					= 0x0806,
	FRM_RE_ARP_PACKET			= 0x0808,
	ISIS_PACKET					= 0x8000,
	RARP_PACKET					= 0x8035,
	VLAN_PACKET					= 0x8100,
	IPV6_PACKET 				= 0x86DD,
	IEEE8023_APCKET 			= 0x8808,
	PPP_PACKET 					= 0x880B,
	PPPOE_DISCOVERY_PACKET 		= 0x8863,
	PPPOE_SESS_PACKET 			= 0x8864,
	IEEE8021X_PACKET 			= 0x888E,
	IEEE80211I_PACKET			= 0x88A8,
	IEEE80211_PREAUTH_PACKET	= 0x88C7,
}ether_type;

int get_ether_type(char *data, int len);
int ipv4_packet_processer(char *data, int len);
int x75_packet_processer(char *data, int len);
int x25l3_packet_processer(char *data, int len);
int arp_packet_processer(char *data, int len);
int frm_re_arp_packet_processer(char *data, int len);
int isis_packet_processer(char *data, int len);
int rarp_packet_processer(char *data, int len);
int vlan_packet_processer(char *data, int len);
int ipv6_packet_processer(char *data, int len);
int ieee8023_packet_processer(char *data, int len);
int ppp_packet_processer(char *data, int len);
int pppoe_disc_packet_processer(char *data, int len);
int pppoe_sess_packet_processer(char *data, int len);
int ieee8021x_packet_processer(char *data, int len);
int ieee80211i_packet_processer(char *data, int len);
int ieee80211_preauth_packet_processer(char *data, int len);

processer dispatch_ether_packet(char *data, int len);


#endif //_PARSER_H_