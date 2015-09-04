/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef COORDINATOR_DATA_H
#define COORDINATOR_DATA_H

#include <stdint.h>

typedef struct data_opt_t
{
  char name[100];
  char str_value[100];
  int32_t  value;
} data_opt_t;

typedef struct data_t
{
  char dev_name[100];
  uint32_t dev_addr;
  data_opt_t opts[8];
} data_t;

#endif/*COORDINATOR_DATA_H*/
