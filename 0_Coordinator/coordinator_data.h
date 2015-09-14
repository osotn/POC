/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef COORDINATOR_DATA_H
#define COORDINATOR_DATA_H

#include <stdint.h>
#include <stdbool.h>
#include <netinet/in.h>

#define OPTIONS_NUM 8
#define NAME_LEN 100

#if 1
typedef struct data_opt_t
{
  int32_t value;
} data_opt_t;
#else
// XXX Do we need struct? Probably yes.
typedef int32_t data_opt_t;
#endif
typedef struct data_t
{
  struct sockaddr_in dev_addr;
  data_opt_t opts[OPTIONS_NUM];
} data_t;

#endif/*COORDINATOR_DATA_H*/
