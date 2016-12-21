#ifndef DEVIDD_CTL_H
# define DEVIDD_CTL_H

# define SERV_PORT 4196
# define BUF_LEN 1024

int32_t daemonize(void);

int32_t create_server(struct sockaddr_in client_addr);

int32_t create_client(struct sockaddr_in serv_addr);


#endif // !DEVIDD_CTL_H
