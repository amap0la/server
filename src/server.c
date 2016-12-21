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


int32_t create_server(struct sockaddr_in client_addr)
{
  struct sockaddr_in serv_addr; /* Address to bind server socket */
  int32_t sock_fd = 0; /* Server socket */
  char buf[BUF_LEN]; /* Buffer received from the client */
  int32_t b = 0; /* Return value for bind() */
  int32_t r = 0; /* Return value for recvfrom() */
  int32_t s = 0; /* Return value for sendto() */
  uint32_t size_client = sizeof(client_addr); /* Size of client_addr */
  
  /* Create socket */
  sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sock_fd < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot create socket for server",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  /* Declare struct serv_addr which will contain the sock_fd address */
  memset(&serv_addr, 0, sizeof(serv_addr)); /* FIXME: BSD: bzero */
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SERV_PORT);
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  /* Bind sock_fd with this address */
  b = bind(sock_fd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

  if (b < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot bind server socket",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  while(1)
  {
    /* Receive data from client
     - blocks until datagram received from the client */
    
    r = recvfrom(sock_fd, buf, BUF_LEN, 0,
                 (struct sockaddr *) &client_addr,
                 &size_client);
    
    /* Handle recvfrom() failure */
    if (r < 0)
    {
      syslog(LOG_ERR, "%s, %d: Cannot receive from client to server",
             basename(__FILE__), __LINE__);
      return DEVIDD_ERR;
    }
    buf[r] = '\0';

    /* FIXME: treatment */

    /* Send data to the client */
    s = sendto(sock_fd, buf, BUF_LEN, 0,
               (struct sockaddr *) &client_addr,
               size_client);
    
    /* Handle sendto() failure */
    if (s < 0)
    {
      syslog(LOG_ERR, "%s, %d: Cannot send from server to client",
             basename(__FILE__), __LINE__);
      return DEVIDD_ERR;
    }
  }

  if (close(sock_fd) < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot close server socket",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}

