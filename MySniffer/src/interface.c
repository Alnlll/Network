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

/************************************************************************
*Function   : get_netif_info()
*Description: Get net interface information with ioctl.
*Calls      :
*Called By  :
*Input      : 
*             @ifname : interface name str.
*             @sock_fd: socket file discreptor.
*             @cmd    : Device-dependent request code
*                       SIOCGIFINDEX  : get interface index.
*                       SIOCGIFADDR   : get netif ip address.
*                       SIOCGIFBRDADDR: get netif broadcast address.
*                       SIOCGIFNETMASK: get netif net mask.
*                       SIOCGIFHWADDR : get netif hardware address.
*             @ifr    : interface name str.
*Output     :
*Return     :
*Others     :
************************************************************************/

int get_spec_netif_info(char *ifname, int sock_fd, unsigned long cmd, struct ifreq *ifr)
{
    int err = 0;
    char err_buf[ERR_BUF_SIZE] = {0};
    
    if ((NULL == ifname) || (0 >= sock_fd) || (NULL == ifr))
    {
        err = -1;
        goto end;
    }

    strcpy(ifr->ifr_name, ifname);

    if (-1 == (err = ioctl(sock_fd, cmd, ifr)))
    {
        sprintf(err_buf, "get_netif_info()-%lu", cmd);
        perror(err_buf);
        goto end;
    }
    
end:
    return err;
}

