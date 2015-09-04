/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <string.h>
#include "../slave_data.h"

slave_event_t slave_serialize(data_t *data, uint8_t *pkg)
{
  printf("Slave Serialize: data = %p\n", data);

  /* Change pkg */

  printf("Slave Serialize: pkg = %p\n", pkg);

  return SLAVE_DATA;
}   

slave_event_t slave_deserialize(data_t *data, uint8_t *pkg)
{
  printf("Slave Deserialize: data = %p\n", data);

  /* Change data */

  printf("Slave Deserialize: pkg = %p\n", pkg);

  return SLAVE_DATA;  
}
