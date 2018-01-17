#include <stdio.h>
#include <sys/select.h> //select()
// earlier std for using select()
//#include <sys/time.h>
//#include <sys/types.h>
//#include <unistd.h>
#include <sys/types.h> //recvfrom()
#include <sys/socket.h> //recvfrom()
#include "capture.h"

int do_cap(int sock_fd, char *buf, int size)
{
	int res = 0;
	fd_set fd_read;
	struct sockaddr saddr;
	int addr_size = sizeof(saddr);
	int data_len = 0;


	FD_ZERO(&fd_read);
	FD_SET(0, &fd_read);
	FD_SET(sock_fd, &fd_read);

	res = select(sock_fd+1, &fd_read, NULL, NULL, NULL);

	if (0 > res)
	{
		perror("select() ");
		return -1;
	}
	else
	{
		if (FD_ISSET(0, &fd_read))
		{
			return -1;
		}
		else if (FD_ISSET(sock_fd, &fd_read))
		{
            data_len = recv_data_from(sock_fd, buf, size, 0,\
								      &saddr, &addr_size);
			if (0 >= data_len)
				return -1;
            else
			    return data_len;
		}
		else
		{
			printf("Unknown error.\n");
			return -1;
		}
	}
}
