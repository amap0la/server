#ifndef SERVER_H
# define SERVER_H

# define SERV_PORT 4196
# define BUF_LEN 1024

int32_t serv_socket(int32_t sock_fd);

int32_t serv_bind(int32_t sock_fd);

int32_t serv_recv(int32_t sock_fd, char **buf);

int32_t serv_send(int32_t sock_fd, char **buf);

int32_t serv_core(struct sockaddr_in client_addr)


#endif // !SERVER_H