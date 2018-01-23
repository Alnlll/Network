#ifndef _COMM_H_
#define _COMM_H_

int create_socket(int dommain, int type, int protocol);
void destroy_socket(int sock_fd);
int send_data_to(int sock_fd, char *buf, int len, int flags, struct sockaddr *dest_addr, int addr_len);
int recv_data_from(int sock_fd, char * buf, int len, int flags, struct sockaddr *src_addr, socklen_t *addr_len);
//int send_data();
//int recv_data();

#endif //_COMM_H_