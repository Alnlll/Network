#include <stdio.h>
#include <linux/if_ether.h> //ETH_P_ALL,
#include <netinet/in.h> //struct sockadd_in, 
#include <netpacket/packet.h> //struct sockadd_ll,
#include <net/if.h> //struct ifreq, 
#include <sys/ioctl.h> //SIOCGIFXXXX,
#include <time.h> //time_t
#include <string.h> //memset(),
#include <stdlib.h> //malloc(),
#include "comm.h"
#include "common.h"
#include "parser.h"
#include "interface.h"
#include "log.h"

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
    t_blist();
    
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

