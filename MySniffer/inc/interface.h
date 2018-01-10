#ifndef _INTERFACE_H_
#define _INTERFACE_H_

int do_promisc(char *ifname, int sock_fd);
int cancel_promisc(char *ifname, int sock_fd);

#endif //_INTERFACE_H_