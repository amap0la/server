#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/stat.h>

#include "devidd_ctl.h"
#include "error_handler.h"

int32_t client_socket(int32_t sock_fd)
{
  /* Create socket */
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (sock_fd < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot create socket for client", 
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;

}

int32_t client_bind(int32_t sock_fd)
{
  int32_t b; /* Return value for bind() */
  struct sockaddr_in client_addr; /* Adress to bind client socket */

  /* Declare struct clien_addr which will contain the sock_fd adress */
  memset(&client_addr, 0, sizeof (client_addr));
  client_addr.sin_family = AF_INET;
  client_addr.sin_port = htons(SERV_PORT);
  client_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Bind sock_fd with this address */
  b = bind(sock_fd, (struct sockaddr *) &client_addr,
      sizeof(client_addr));

  if (b < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot bind socket for client", 
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}

int32_t client_send()
{
  int32_t s; /* Return value for sendto() */




  /* Send to server */
  s = sendto(sock_fd, buf, BUF_LEN, 0,
      (struct sockaddr *) &serv_addr, sizeof(serv_addr));

  if (s < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot send from client to server",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}

int32_t client_recv()
{
  int32_t r; /* Return value for recvfrom() */
  uint32_t size_serv = sizeof (serv_addr); /* Size of serv_addr */


  /* Receive from server */ 
  r = recvfrom(sock_fd, buf, BUF_LEN, 0,
      (struct sockaddr *) &serv_addr, &size_serv);

  if (r < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot receive from server to client",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}


int32_t core_client(struct sockaddr_in serv_addr)
{
  int32_t sock_fd; /* Client socket */ 
  char buf[BUF_LEN]; /* */

  /* Create client socket and bind it */
  if ((client_socket(sock_fd) != DEVIDD_SUCCESS)
      || client_bind(sock_fd) != DEVIDD_SUCCESS)
  {
    return DEVIDD_ERR;   
  }

  while(1)
  {
    if (client_send(sock_fd, &buf) != DEVIDD_SUCCESS)
    {
      return DEVIDD_ERR;
    }

    if (client_recv(sock_fd, &buf) != DEVIDD_SUCCESS)
    {
      return DEVIDD_ERR;
    }

  }

  /* Close */
  if (close(sock_fd) < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot close client socket",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}

int main(int argc, char **argv)
{
  
  if (core_client(serv_addr))
    return DEVIDD_ERR;

  return 0;
}







