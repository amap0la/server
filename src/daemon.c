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

int32_t daemonize(void)
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

