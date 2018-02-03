#include <stdio.h>
#include <unistd.h> //close(),getopt()
#include <arpa/inet.h> //htons(),
#include <linux/if_ether.h> //ETH_P_ALL,
#include "interface.h"
#include "capture.h"
#include "common.h"
#include "parser.h"
#include "comm.h"
#include "sniffer.h"

char *usage = "Usage:\n\tmain ifname\n";

int para_parser(sniff_info *info, int argc, char *argv[])
{
    int ch;

    while(EOF != (ch = getopt(argc, argv, SNIFF_PARA_FORMAT)))
    {
        switch (ch)
        {
            case 'd':
                info->ifname = optarg;
                break;
            case 's':
                break;
            case 'f':
                info->log_ops.filename = optarg;
                break;
            default:
                break;
        }
    }

    return 0;
}

void cap_file_hdr_init(cap_file_hdr *head)
{
    head->magic = CAP_FILE_HDR_MAGIC;
    head->version_major = CAP_FILE_HDR_MAJOR_VER;
    head->version_minor = CAP_FILE_HDR_MINOR_VER;
    head->thiszone = CAP_FILE_HDR_T_ZONE;
    head->sigfigs = CAP_FILE_HDR_SIGFIG;
    head->snaplen = CAP_FILE_HDR_SNAPLEN;
    head->linktype = 0x01; //
}

#if 0
int main(int argc, char *argv[])
{
    sniff_info info;
    para_parser(&info, argc, argv);

    printf("ifname: %s\n", info.ifname);
    printf("log file: %s\n", info.log_ops.filename);
}
#endif //0

#if 0
int main(int argc, char *argv[])
{
#define REV_DATA_SIZE 8192
    int err = 0;
    int r_sock_fd = -1;
    char data_buf[REV_DATA_SIZE] = {0};
    int rev_len = 0;

    if (2 > argc)
        printf("%s", usage);

    r_sock_fd = create_socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (-1 == r_sock_fd)
        return -1;

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
            hex_dump(data_buf, rev_len);
            err = data_processer(data_buf, rev_len);
            if (0 != process_err((err_type)err))
                continue;
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

    destroy_socket(r_sock_fd);

    return 0;   
}
#endif //0
