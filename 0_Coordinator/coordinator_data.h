/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef COORDINATOR_DATA_H
#define COORDINATOR_DATA_H

#include <stdint.h>
#include <netinet/in.h>

#define OPTIONS_NUM 8

typedef struct data_opt_t
{
  char name[100];
  char str_value[100];
  int32_t  value;
} data_opt_t;

typedef struct data_t
{
  char dev_name[100];
  struct sockaddr_in dev_addr;
  data_opt_t opts[OPTIONS_NUM];
} data_t;

#endif/*COORDINATOR_DATA_H*/
