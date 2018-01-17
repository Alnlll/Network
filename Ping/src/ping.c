//#include "ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <netinet/ip.h> //struct ip & struct icmp
#include <linux/types.h> //all types
#include "ping.h"

char send_buf[SEND_BUF_SIZE] = {0};

uint16_t cal_chsum(struct icmp *p_icmp)
{
	uint16_t *data = (uint16_t *)p_icmp;
	uint16_t tmp = 0;
	uint32_t csum = 0;
	int len = ICMP_LEN;


	while (1 < len)
	{
		sum += *data++;
		len -= 2;
	}

	if (1 == len)
	{
		tmp = *data;
		tmp &= 0xff00;
		csum += tmp;
	}

	csum = (csum >> 16) + (csum & 0x0000ffff);
	csum += (csum >> 16);
	//csum = ~csum;

	return ~csum;
}

void call(int argc, char *argv[])
{
	struct hostent *p_hostname = NULL;
	struct sockaddr_in tar_addr;
	in_addr_t inaddr;

	if (2 > argc)
	{
		printf("Usage: ping [hostname/IP]\n");
		exit(-1);
	}

	if (INADDR_NONE == (inaddr = inet_addr(argv[1])))
	{
		if (NULL == (p_hostname = gethostbyname(argv[1])))
		{
			herror("gethostbyname()");
			exit(-1);
		}

		memmove(&tar_addr.sin_addr, p_hostname->h_addr_list, p_hostname->h_length);
	}
	else
	{
		tar_addr.sin_addr.s_addr = inaddr;
	}

	printf("Ping %s(%s) %d bytes of data\n", argv[1], inet_ntoa(tar_addr.sin_addr), 64);
}

int main(int argc, char *argv[])
{

	call(argc, argv);

	return 0;
}