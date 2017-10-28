//#include "ping.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>

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