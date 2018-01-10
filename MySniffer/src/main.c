#include <stdio.h>
#include <unistd.h> //close()
#include <arpa/inet.h> //htons(),
#include </usr/include/linux/if_ether.h> //ETH_P_ALL,
#include "interface.h"
#include "capture.h"
#include "common.h"
#include "parser.h"

char *usage = "Usage:\n\tmain ifname\n";

int main(int argc, char *argv[])
{
#define REV_DATA_SIZE 8192
	
	int r_sock_fd = -1;
	int ret = 0;
	char data_buf[REV_DATA_SIZE] = {0};
	int rev_len = 0;
	processer proc_func = NULL;

	if (2 > argc)
		printf("%s", usage);

	r_sock_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
	if (-1 == r_sock_fd)
	{
		perror("Create socket fail");
		return -1;
	}

	if (0 != do_promisc(argv[1], r_sock_fd))
	{
		close(r_sock_fd);
		fprintf(stderr, "%s", "Promisc mode setting fail.\n");
		return -1;
	}

	while (1)
	{
		if (0 < (rev_len = do_cap(r_sock_fd, data_buf, sizeof(data_buf))))
		{
			if (NULL == (proc_func = dispatch_ether_packet(data_buf, rev_len)))
			{
				printf("Parse ether packet fail\n");
				break;
			}

			proc_func(data_buf, rev_len);
			hex_dump(data_buf, rev_len);
		}
		else
		{
			break;
		}
	}

	if (0 != cancel_promisc(argv[1], r_sock_fd))
	{
		close(r_sock_fd);
		fprintf(stderr, "%s", "Promisc mode canceling fail.\n");
		return -1;	
	}

	close(r_sock_fd);

	return 0;	
}
