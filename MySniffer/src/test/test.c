#include <stdio.h>
#include <linux/if_ether.h> //ETH_P_ALL,
#include <netinet/in.h> //struct sockadd_in, 
#include <netpacket/packet.h> //struct sockadd_ll,
#include <net/if.h> //struct ifreq, 
#include <sys/ioctl.h> //SIOCGIFXXXX,
#include <time.h> //time_t
#include <string.h> //memset(),
#include <stdlib.h> //malloc(),
#include <unistd.h> //close(),
#include "comm.h"
#include "common.h"
#include "parser.h"
#include "capture.h"
#include "interface.h"
#include "log.h"
#include "sniffer.h"

int send_arp_packet(char *ifname, int sock_fd)
{
    char *buf = NULL;
    ether_hdr *p_eth = NULL;
    arp_pack *p_arp = NULL;
    struct sockaddr_ll sll;
    struct ifreq ifr;
    int i = 0;
    int len = 0;

    /* Get index of net interface */
    if (0 != get_spec_netif_info(ifname, sock_fd, SIOCGIFINDEX, &ifr))
    {
        return -1;
    }
    else
    {
        sll.sll_ifindex = ifr.ifr_ifindex;
    }

    buf = (char *)zelloc(sizeof(ether_hdr)+sizeof(arp_pack));
    if (NULL == buf)
        return -1;

    p_eth = (ether_hdr *)buf;

    p_eth->dst_mac[0] = 0xFF;
    p_eth->dst_mac[1] = 0xFF;
    p_eth->dst_mac[2] = 0xFF;
    p_eth->dst_mac[3] = 0xFF;
    p_eth->dst_mac[4] = 0xFF;
    p_eth->dst_mac[5] = 0xFF;
    
    p_eth->src_mac[0] = 0x00;
    p_eth->src_mac[1] = 0x0c;
    p_eth->src_mac[2] = 0x29;
    p_eth->src_mac[3] = 0x09;
    p_eth->src_mac[4] = 0x0f;
    p_eth->src_mac[5] = 0xa3;
    
    p_eth->type = htons(0x0806);

    p_arp = (arp_pack *)(buf + sizeof(ether_hdr));

    p_arp->hw_type = htons(0x0001);
    p_arp->protocol_type = htons(0x0800);
    p_arp->hw_addr_len = 0x06;
    p_arp->protocol_addr_len = 0x04;
    p_arp->op = htons(0x0001);

    p_arp->sender_eth_addr[0] = 0x00;
    p_arp->sender_eth_addr[1] = 0x0c;
    p_arp->sender_eth_addr[2] = 0x29;
    p_arp->sender_eth_addr[3] = 0x09;
    p_arp->sender_eth_addr[4] = 0x0f;
    p_arp->sender_eth_addr[5] = 0xa3;

    p_arp->sender_ip_addr.s_addr = inet_addr("192.168.27.255");

    p_arp->target_eth_addr[0] = 0x00;
    p_arp->target_eth_addr[1] = 0x00;
    p_arp->target_eth_addr[2] = 0x00;
    p_arp->target_eth_addr[3] = 0x00;
    p_arp->target_eth_addr[4] = 0x00;
    p_arp->target_eth_addr[5] = 0x00;

    p_arp->target_ip_addr.s_addr = inet_addr("192.168.1.1");

    len = sizeof(ether_hdr)+sizeof(arp_pack);

    hex_dump(buf, len);

    for (i = 0; i < 50; i++)
    {
        if (len > send_data_to(sock_fd, buf, len, 0, (struct sockaddr *)&sll, sizeof(sll)))
            return -1;    
    }

    free_mem(buf);

    return 0;
}

void t_set_color(void)
{
    SET_COLOR(RED);
    printf("RED\n");
    printf("RED\n");
    SET_COLOR(L_RED);
    printf("LIGHT_RED\n");
    SET_COLOR(GREEN);
    printf("GREEN\n");
    SET_COLOR(L_GREEN);
    printf("LIGHT_GREEN\n");
    SET_COLOR(DEFAULT_COLOR);
    printf("Default\n");
}

void t_log(void)
{
    FILE *f = NULL;
    char f_name[64] = {0};
    char s_time[64] = {0};
    log t_log;

    if (!dir_exist(LOG_DIR))
    {
        if (0 != exec_command_1(CMD_MK_LOG_DIR))
            return;
    }

    if (0 == get_str_time(s_time, sizeof(s_time), "%Y-%m-%d_%X", LOCAL_TIME))
    {
        printf("time to str fail.\n");
        return;
    }
    sprintf(f_name, "%s/%s.log", LOG_DIR, s_time);
    
    if (NULL == (f = open_file(f_name, "a")))
        return;
    printf("%s\n", f_name);

    t_log.f = f;
    t_log.s = stdout;
    t_log.limit_lv = LOG_LEVEL_DEBUG;

    log_write(&t_log, LOG_LV(DEBUG),  "%s\n", "DEBUG.");
    log_write(&t_log, LOG_LV(INFO),  "%s\n", "INFO.");
    log_write(&t_log, LOG_LV(WARN),  "%s\n", "WARN.");
    log_write(&t_log, LOG_LV(ERROR),  "%s\n", "ERROR.");
    log_write(&t_log, LOG_LV(FATAL),  "%s\n", "FATAL.");

    close_file(f);
}

int t_get_netif_info(char *ifname)
{
    int err = 0;

    int sock_fd = 0;
    struct sockaddr_ll sll;
    struct sockaddr_in *sin;
    struct ifreq ifr;

    if (-1 == (sock_fd = create_socket(AF_INET, SOCK_DGRAM, 0)))
    {
        err = -1;
        goto end;
    }
    
    /* get index */
    if ( 0 != (err = get_spec_netif_info(ifname, sock_fd, SIOCGIFINDEX, &ifr)))
    {
        goto end;
    }
    sll.sll_ifindex = ifr.ifr_ifindex;

    printf("%s index: %d\n", ifname, sll.sll_ifindex);

    /* get ip addr */
    if ( 0 != (err = get_spec_netif_info(ifname, sock_fd, SIOCGIFADDR, &ifr)))
    {
        goto end;
    }
    sin = (struct sockaddr_in *)&(ifr.ifr_addr);

    printf("%s ip address: %s\n", ifname, inet_ntoa(sin->sin_addr));

    /* get broadcast addr */
    if ( 0 != (err = get_spec_netif_info(ifname, sock_fd, SIOCGIFBRDADDR, &ifr)))
    {
        goto end;
    }
    sin = (struct sockaddr_in *)&(ifr.ifr_addr);

    printf("%s broadcast address: %s\n", ifname, inet_ntoa(sin->sin_addr));

    /* get net mask */
    if ( 0 != (err = get_spec_netif_info(ifname, sock_fd, SIOCGIFNETMASK, &ifr)))
    {
        goto end;
    }
    sin = (struct sockaddr_in *)&(ifr.ifr_addr);

    printf("%s net mask: %s\n", ifname, inet_ntoa(sin->sin_addr));

    /* get hwaddr */
    if ( 0 != (err = get_spec_netif_info(ifname, sock_fd, SIOCGIFHWADDR, &ifr)))
    {
        goto end;
    }

    printf("%s mac addr: %02x:%02x:%02x:%02x:%02x:%02x\n", ifname, ifr.ifr_netmask.sa_data[0],
                                                                  ifr.ifr_netmask.sa_data[1],
                                                                  ifr.ifr_netmask.sa_data[2],
                                                                  ifr.ifr_netmask.sa_data[3],
                                                                  ifr.ifr_netmask.sa_data[4],
                                                                  ifr.ifr_netmask.sa_data[5]);

end:
    destroy_socket(sock_fd);
    return err;
}

void t_blist()
{
    typedef struct _int_node
    {
        int val;
        BLIST_ENTRY(struct _int_node) entry;
    } int_node;

    int i = 0;
    int_node *n_node = NULL;
    int_node *p_tail = NULL;
    int_node *index = NULL;

    BLIST_HEAD(num_list, struct _int_node) nl_head;
    BLIST_INIT(&nl_head);

    for (i = 0; i < 10; i++)
    {
        if (NULL == (n_node = (int_node *)malloc(sizeof(int_node))))
        {
            perror("malloc()");
            return;
        }
        n_node->val = i;
        BLIST_INSERT_AFTER(&nl_head, n_node, p_tail, entry);
        p_tail = n_node;
    }

    BLIST_FOREACH_HEAD(index, &nl_head, entry)
    {
        printf("%d\n", index->val);
        printf("index: %p\n", index);
        if (index->val == 4)
        {
            if (NULL == (n_node = (int_node *)malloc(sizeof(int_node))))
            {
                perror("malloc()");
                return;
            }
            n_node->val = 1;
            BLIST_INSERT_BEFORE(&nl_head, n_node, index, entry);

            if (NULL == (n_node = (int_node *)malloc(sizeof(int_node))))
            {
                perror("malloc()");
                return;
            }
            n_node->val = 1;
            BLIST_INSERT_AFTER(&nl_head, n_node, index, entry);
            break;
        }
    }

    BLIST_FOREACH_HEAD(index, &nl_head, entry)
    {
        if (9 == index->val)
            BLIST_REMOVE(&nl_head, index, entry);
    }

    BLIST_FOREACH_HEAD(index, &nl_head, entry)
    {
        printf("%d\n", index->val);
    }
}

void t_pcap_file(void)
{
    FILE *f = NULL;
    //struct timezone tz;

    cap_file_hdr f_hdr;
    cap_pkt_hdr p_hdr;

    char data[] = {0x80,0x89,0x17,0xfc,0x35,0xc6,0x0a,0x9b,0x93,0xa9,0x30,0xc8,
                   0x08,0x00,0x45,0x00,0x00,0x2c,0x31,0x16,0x00,0x00,0x40,0x11,
                   0x95,0x19,0xc0,0xa8,0x00,0x67,0x74,0x1b,0x7f,0x67,0x11,0x72,
                   0x56,0x41,0x00,0x18,0x93,0xf5,0x4b,0xff,0x00,0x01,0x04,0x00,
                   0x00,0x08,0x00,0x00,0x00,0x16,0x00,0x00,0x00,0x0f};

    if (NULL == (f = open_file("./test.cap", "wb")))
    {
        return;
    }

    f_hdr.magic = 0xa1b2c3d4;
    f_hdr.version_major = 0x02;
    f_hdr.version_minor = 0x04;
    f_hdr.thiszone = 0x00;
    f_hdr.sigfigs = 0x00;
    f_hdr.linktype = 0x01;
    f_hdr.snaplen = 0x01;

    // if(get_day_time(&(p_hdr.ts), &tz))
    // {
    //     perror("get_day_time");
    // }
    p_hdr.caplen = 0x3a;
    p_hdr.len = 0x3a;


    if (0 >= fwrite(&f_hdr, sizeof(p_hdr), 1, f))
    {
        perror("fwrite1");
    }
    if (0 >= fwrite(&p_hdr, sizeof(p_hdr), 1, f))
    {
        perror("fwrite2");
    }
    
    if (0 >= fwrite(&data, sizeof(data), 1, f))
    {
        perror("fwrite3");
    }

    close_file(f);
}


// void t_get_day_time(void)
// {
//     struct timeval tv;
//     struct timezone tz;

//     if (gettimeofday(&tv, &tz))
//         perror("gettimeofday()");
//     else
//     {
//         printf("tv: %d, %d\n", tv.tv_sec, tv.tv_usec);
//         printf("tz: %d, %d\n", tz.tz_minuteswest, tz.tz_dsttime);
//     }
// }

int t_cap_and_write_cap_file(char *ifname)
{
#define REV_DATA_SIZE 8192
    char *usage = "Usage:\n\ttest ifname\n";
    int err = 0;
    int r_sock_fd = -1;
    char data_buf[REV_DATA_SIZE] = {0};
    int rev_len = 0;
    FILE *cap_f = NULL;
    cap_file_hdr cap_filehead;
    cap_pkt_hdr cap_pkthead;
    
    if (NULL == ifname)
    {
        printf("%s", usage);
        return -1;
    }

    if (NULL == (cap_f = open_file("./test.cap", "wb")))
    {
        return -1;
    }

    cap_file_hdr_init(&cap_filehead);
    
    r_sock_fd = create_socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (-1 == r_sock_fd)
    {
        return -1;
    }

    if (0 != do_promisc(ifname, r_sock_fd))
    {
        fprintf(stderr, "%s", "Promisc mode setting fail.\n");
        err = -1;
        goto end;
    }

    if (0 >= (err = file_write(&cap_filehead, sizeof(cap_file_hdr), 1, cap_f)))
    {
        printf("write cap file head fail: %d\n", err);
        goto end;
    }

    while (1)
    {
        if (0 < (rev_len = do_cap(r_sock_fd, data_buf, sizeof(data_buf))))
        {
            if (get_day_time(&(cap_pkthead.ts)))
            {
                continue;
            }
            else
            {
                printf("==time:%xu, %xu===\n", cap_pkthead.ts.tvm_sec, cap_pkthead.ts.tvm_usec);
                cap_pkthead.caplen = rev_len;
                cap_pkthead.len = rev_len;
                if (0 >= file_write(&cap_pkthead, sizeof(cap_pkthead), 1, cap_f))
                {
                    printf("write pkt head fail.\n");
                    break;
                }
                if (0 >= file_write(data_buf, rev_len, 1, cap_f))
                {
                    printf("write data fail.\n");
                    break;
                }
            }
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

    if (0 != cancel_promisc(ifname, r_sock_fd))
    {
        close(r_sock_fd);
        fprintf(stderr, "%s", "Promisc mode canceling fail.\n");
        err = -1;
        goto end;
    }

end:
    close_file(cap_f);
    destroy_socket(r_sock_fd);
    return err;
}

int main(int argc, char *argv[])
{
    int err = 0;
    int sock_fd = -1;

#if 0
    sock_fd = create_socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (0 > sock_fd)
    {
        err = -1;
        goto end;
    }
#endif //0

    //t_set_color();
    //t_get_sys_time();
    //t_log();
    //t_get_netif_info(argv[1]);
    //t_blist();
    //t_pcap_file();
    //t_get_day_time();
    t_cap_and_write_cap_file(argv[1]);
    
#if 0
    send_arp_packet(argv[1], sock_fd);

    if (-1 == (err = t_get_netif_addr(argv[1], sock_fd)))
    {
        err = 1;
        goto end;
    }
#endif //0
    goto end;

end:
    destroy_socket(sock_fd);
    return err;
}

