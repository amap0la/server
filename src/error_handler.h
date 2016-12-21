#ifndef ERROR_HANDLER_H
# define ERROR_HANDLER_H

#include <stdint.h>
#include <syslog.h>
#include <libgen.h>

# define ERR_MAX_LEN 1024

enum devidd_err
{
  DEVIDD_SUCCESS, /* Success */
  DEVIDD_ERR_MEM, /* Memory error */
  DEVIDD_ERR_IO, /* Input/Output error */
  DEVIDD_ERR_EAGAIN, /* Ressource temporarily unavailable */
  DEVIDD_ERR_PERM, /* Operation not permitted */
  DEVIDD_ERR_INVALID_ARG, /* Invalid arguments */
  DEVIDD_ERR /* Generic error */
};

#endif /* !ERROR_HANDLER_H */
