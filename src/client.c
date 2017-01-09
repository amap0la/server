# define _DEFAULT_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/stat.h>

#include "client.h"
#include "error_handler.h"


static int32_t daemonize(void)
{
  int32_t pid = 0;
  int32_t err = 0;

  /* Child creation */
  pid = fork();
  
  /* Fork failure */
  if (pid < 0)
  { 
    /* Set err depending on errno value */
    err = (errno == EAGAIN)? DEVIDD_ERR_EAGAIN : DEVIDD_ERR_MEM;
    syslog(LOG_ERR, "%s, %d: Cannot create daemon, fork failure", 
           basename(__FILE__), __LINE__);
    return err;
  }

  /* Kill parent process */
  if (pid > 0)
  {
    return DEVIDD_SUCCESS;
  }
  
  /* Open a new session */
  if (setsid() < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot open session for daemon",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR_PERM;
  }

  /* New file permissions */
  umask(0);

  /* Change the current working directory to root */
  if (chdir("/") < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot change current working directory to root",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  /* Close stdin, stdout, stderr */
  if (close(STDIN_FILENO) || close(STDOUT_FILENO) || close(STDERR_FILENO))
  {
    syslog(LOG_ERR, "%s, %d: Cannot close standard fd",
           basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }
  openlog("devidd_ctl", LOG_PID, LOG_DAEMON); // FIXME: LOG_DAEMON??

  return DEVIDD_SUCCESS;
}


int32_t client_socket(int32_t *sock_fd)
{
  /* Create socket */
  *sock_fd = socket(AF_INET, SOCK_DGRAM, 0);

  if (*sock_fd < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot create socket for client", 
        basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;


}

int32_t client_bind(int32_t *sock_fd, struct sockaddr_in *serv_addr)
{
  /*int32_t b; Return value for bind() */
  int32_t i; /* Return value for inet_aton() */
  sock_fd = sock_fd; 

  /* Declare struct serv_addr which will contain the sock_fd adress */
  memset(serv_addr, 0, sizeof (*serv_addr));
  serv_addr->sin_family = AF_INET;
  serv_addr->sin_port = htons(SERV_PORT);
  i = inet_aton(SERV_ADDR, &serv_addr->sin_addr);
  if (i == 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot convert server address string \
        into binary address", basename(__FILE__), __LINE__);

    return DEVIDD_ERR;
  }

int32_t client_send(int32_t *sock_fd, char **buf, struct sockaddr_in *serv_addr)
{
  int32_t s; /* Return value for sendto() */

  *buf = strdup("why?\n");

  /* Send to server */
  s = sendto(*sock_fd, *buf, BUF_LEN, 0,
      (struct sockaddr *) serv_addr, sizeof(*serv_addr));

  if (s < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot send from client to server // %s",
        basename(__FILE__), __LINE__, strerror(errno));
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}

int32_t client_recv(int32_t *sock_fd, char **buf, struct sockaddr_in *serv_addr)
{
  int32_t r; /* Return value for recvfrom() */
  uint32_t len = sizeof (*serv_addr);

  /* Receive from server */ 
  r = recvfrom(*sock_fd, *buf, BUF_LEN, 0,
      (struct sockaddr *) serv_addr, &len);

  if (r < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot receive from server to client",
        basename(__FILE__), __LINE__);
    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}


int32_t client_core(void)
{
  int32_t sock_fd = 0; /* Client socket */ 
  struct sockaddr_in serv_addr;  
  char *buf = NULL;

  buf = malloc(BUF_LEN);
  if (!buf)
  {
    syslog(LOG_ERR, "%s, %d: Cannot close client socket",
        basename(__FILE__), __LINE__);

    return DEVIDD_ERR_MEM;
  }

  /* Create client socket and bind it */
  if ((client_socket(&sock_fd) != DEVIDD_SUCCESS)
      || (client_bind(&sock_fd, &serv_addr) != DEVIDD_SUCCESS))
  {
    return DEVIDD_ERR;   
  }

  if (client_send(&sock_fd, &buf, &serv_addr) != DEVIDD_SUCCESS)
  {
    free(buf);
    printf("send error\n");
    return DEVIDD_ERR;
  }

  if (client_recv(&sock_fd, &buf, &serv_addr) != DEVIDD_SUCCESS)
  {
    free(buf);
    printf("recv error");
    return DEVIDD_ERR;
  }

  printf("Server: %s\n", buf);

  /* Close */
  if (close(sock_fd) < 0)
  {
    syslog(LOG_ERR, "%s, %d: Cannot close client socket",
        basename(__FILE__), __LINE__);

    return DEVIDD_ERR;
  }

  return DEVIDD_SUCCESS;
}

int main(void) /* FIXME */
{
  if (client_core())
    return DEVIDD_ERR;

  return 0;
}

