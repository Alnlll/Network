#include <stdio.h>
#include <string.h> //strcpy(),
#include <sys/ioctl.h> //ioctl(),
#include <net/if.h> //IFF_RUNNING, IFF_PROMISC
#include <netpacket/packet.h> //struct sockadd_ll,
#include "interface.h"

int do_promisc(char *ifname, int sock_fd)
{
	struct ifreq ifr;

	strcpy(ifr.ifr_name, ifname);

	if (0 != ioctl(sock_fd, SIOCGIFFLAGS, &ifr))
	{
		perror("ioctl() ");
		return -1;
	}

	if (ifr.ifr_flags & IFF_RUNNING)
		printf("%s link up.\n", ifname);
	else
		printf("%s link down.\n", ifname);

	ifr.ifr_flags |= IFF_PROMISC;
	if (0 != ioctl(sock_fd, SIOCSIFFLAGS, &ifr))
	{
		perror("ioctl()");
		return -1;
	}

	printf("Set interface :::%s::: to promisc.\n", ifr.ifr_name);

	return 0;
}

int cancel_promisc(char *ifname, int sock_fd)
{
	struct ifreq ifr;

	strcpy(ifr.ifr_name, ifname);

	if (0 != ioctl(sock_fd, SIOCGIFFLAGS, &ifr))
	{
		perror("ioctl() ");
		return -1;
	}

	if (ifr.ifr_flags & IFF_RUNNING)
		printf("%s link up.\n", ifname);
	else
		printf("%s link down.\n", ifname);

	ifr.ifr_flags &= (~IFF_PROMISC);
	if (0 != ioctl(sock_fd, SIOCSIFFLAGS, &ifr))
	{
		perror("ioctl()");
		return -1;
	}

	printf("Cancel interface :::%s::: to promisc.\n", ifr.ifr_name);

	return 0;
}

int get_netif_addr(char *ifname, int r_sock_fd, struct sockaddr_ll *sll)
{
    int err = 0;
    struct ifreq ifr;
    
    if ((NULL == sll) || (NULL == ifname))
        err = -1;

    strcpy(ifr.ifr_name, ifname);

    if (-1 == (err = ioctl(r_sock_fd, SIOCGIFINDEX, &ifr)))
    {
        perror("get_net_interface_addr() ");
        return err;
    }
    sll->sll_ifindex = ifr.ifr_ifindex;

    return err;
}

