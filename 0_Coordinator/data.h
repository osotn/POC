/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef _DATA_H_
#define _DATA_H_

#include <stdint.h>

typedef struct data_opt_t
{
  char name[100];
  char str_value[100];
  uint32_t  value;
} data_opt_t;

typedef struct data_t
{
  char dev_name[100];
  uint32_t dev_addr;
  data_opt_t opts[7];  
} data_t;

#endif/*_DATA_H_*/
