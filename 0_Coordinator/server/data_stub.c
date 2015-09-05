/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <string.h>
#include "../server_data.h"

server_event_t server_serialize(data_t *data, char *script_str,
  int *str_size)
{
  printf("Server Serialize: data = %p\n", data);

  strcpy(script_str, "==answer from slave to server==");

  printf("Server Serialize: script = \"%s\"\n", script_str);

  return SERVER_DATA;
}
   
server_event_t server_deserialize(data_t *data, char *script_str,
  int *str_size)
{
  printf("Server Deserialize: script = \"%s\"\n", script_str);
  
  /* Change data */
 
  printf("Server Deserialize: data = %p\n", data);

  return SERVER_DATA;
}
