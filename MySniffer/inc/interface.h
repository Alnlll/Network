#ifndef _INTERFACE_H_
#define _INTERFACE_H_

#include <net/if.h> //IFF_RUNNING, IFF_PROMISC, struct ifreq, 

#define ERR_BUF_SIZE 64

//typedef int (*netif_handler)(int, struct ifreq *);

int do_promisc(char *ifname, int sock_fd);
int cancel_promisc(char *ifname, int sock_fd);
int get_spec_netif_info(char *ifname, int sock_fd, unsigned long cmd, struct ifreq *ifr);

#endif //_INTERFACE_H_