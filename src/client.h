#ifndef SERVER_H
# define SERVER_H

# define SERV_PORT 4196
# define BUF_LEN 1024

int32_t client_socket(int32_t sock_fd);

int32_t client_bind(int32_t sock_fd);

int32_t client_recv(int32_t sock_fd, char **buf);

int32_t client_send(int32_t sock_fd, char **buf);

int32_t client_core(struct sockaddr_in serv_addr)


#endif // !SERVER_H
