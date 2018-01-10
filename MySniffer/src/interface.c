#include <stdio.h>
#include <string.h> //strcpy(),
#include <sys/ioctl.h> //ioctl(),
#include <net/if.h> //IFF_RUNNING, IFF_PROMISC
#include "interface.h"

int do_promisc(char *ifname, int sock_fd)
{
	int s = 0;
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
	int s = 0;
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
