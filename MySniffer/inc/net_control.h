#ifndef _NET_CONTROL_H
#define _NET_CONTROL_H

int do_promisc(char *ifname, int sock_fd);
int cancel_promisc(char *ifname, int sock_fd);

#endif //_NET_CONTROL_H