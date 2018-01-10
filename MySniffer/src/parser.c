#include <stdio.h> //NULL
#include <arpa/inet.h> //ntohs()...
#include "common.h"
#include "parser.h"

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

int ipv4_packet_processer(char *data, int len)
{
	printf("---IPV4 PACKET---\n");
	return 0;
}

int x75_packet_processer(char *data, int len)
{
	return 0;
}
int x25l3_packet_processer(char *data, int len)
{
	return 0;
}
int arp_packet_processer(char *data, int len)
{
	return 0;
}
int frm_re_arp_packet_processer(char *data, int len)
{
	return 0;
}
int isis_packet_processer(char *data, int len)
{
	return 0;
}
int rarp_packet_processer(char *data, int len)
{
	return 0;
}
int vlan_packet_processer(char *data, int len)
{
	return 0;
}
int ipv6_packet_processer(char *data, int len)
{
	return 0;
}
int ieee8023_packet_processer(char *data, int len)
{
	return 0;
}
int ppp_packet_processer(char *data, int len)
{
	return 0;
}
int pppoe_disc_packet_processer(char *data, int len)
{
	return 0;
}
int pppoe_sess_packet_processer(char *data, int len)
{
	return 0;
}
int ieee8021x_packet_processer(char *data, int len)
{
	return 0;
}
int ieee80211i_packet_processer(char *data, int len)
{
	return 0;
}
int ieee80211_preauth_packet_processer(char *data, int len)
{
	return 0;
}

processer dispatch_ether_packet(char *data, int len)
{
	int type = 0;
	processer proc_func = NULL;

	if ((NULL == data) || (0 >= len))
		return NULL;

	if (0 > (type = get_ether_type(data, len)))
		return NULL;

	switch ((ether_type)type)
	{
		case IP_PACKET:
			proc_func = ipv4_packet_processer;
			break;
		case X75_PACKET:
			proc_func = x75_packet_processer;
			break;
		case X25L3_PACKET:
			proc_func = x25l3_packet_processer;
			break;
		case ARP_PACKET:
			proc_func = arp_packet_processer;
			break;
		case FRM_RE_ARP_PACKET:
			proc_func = frm_re_arp_packet_processer;
			break;
		case ISIS_PACKET:
			proc_func = isis_packet_processer;
			break;
		case RARP_PACKET:
			proc_func = rarp_packet_processer;
			break;
		case VLAN_PACKET:
			proc_func = vlan_packet_processer;
			break;
		case IPV6_PACKET:
			proc_func = ipv6_packet_processer;
			break;
		case IEEE8023_APCKET:
			proc_func = ieee8023_packet_processer;
			break;
		case PPP_PACKET:
			proc_func = ppp_packet_processer;
			break;
		case PPPOE_DISCOVERY_PACKET:
			proc_func = pppoe_disc_packet_processer;
			break;
		case PPPOE_SESS_PACKET:
			proc_func = pppoe_sess_packet_processer;
			break;
		case IEEE8021X_PACKET:
			proc_func = ieee8021x_packet_processer;
			break;
		case IEEE80211I_PACKET:
			proc_func = ieee80211i_packet_processer;
			break;
		case IEEE80211_PREAUTH_PACKET:
			proc_func = ieee80211_preauth_packet_processer;
			break;
		default:
			printf("unknown type:%04x %s : %d", type, __FILE__, __LINE__);
			return NULL;
	}

	return proc_func;

}
