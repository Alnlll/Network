#include <stdio.h>
#include <sys/types.h> //socket(),
#include <sys/socket.h> //socket(),
#include <unistd.h> //close(),
#include "comm.h"

int create_socket(int dommain, int type, int protocol)
{
    int sock_fd = -1;

    sock_fd = socket(dommain, type, protocol);
    if (-1 == sock_fd)
        perror("create_socket() ");

    return sock_fd;
}

void destroy_socket(int sock_fd)
{
    if (0 <= sock_fd)
        close(sock_fd);
}

int send_data_to(int sock_fd, char *buf, int len, int flags, struct sockaddr *dest_addr, int addr_len)
{
    int s_len = 0;
    int remain = len;
    
    if ((0 > sock_fd) || (NULL == buf) || (NULL == dest_addr) || (0 >= addr_len))
        return -1;

    while (remain > 0)
    {
        if (0 >= (s_len = sendto(sock_fd, buf+len-remain, remain, flags, dest_addr, addr_len)))
        {
            perror("send_data_to() ");
            return len-remain;
        }

        remain -= s_len;
    }

    return len;
    
}

int recv_data_from(int sock_fd, char * buf, int len, int flags, struct sockaddr *src_addr, socklen_t *addr_len)
{
    int recv_len = 0;
    
    if ((0 > sock_fd) || (NULL == buf) || (NULL == src_addr) || (NULL == addr_len))
        return -1;
    
    recv_len = recvfrom(sock_fd, buf, len, flags, src_addr, addr_len);
    if (0 > recv_len)
        perror("recv_data_from() ");
        
    return recv_len;
}

