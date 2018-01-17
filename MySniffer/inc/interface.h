#ifndef _INTERFACE_H_
#define _INTERFACE_H_

int do_promisc(char *ifname, int sock_fd);
int cancel_promisc(char *ifname, int sock_fd);
int get_netif_addr(char *ifname, int r_sock_fd, struct sockaddr_ll *sll);

#endif //_INTERFACE_H_