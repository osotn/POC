/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef _SERVER_DATA_H_
#define _SERVER_DATA_H_

#include "server_interface.h"

server_event_t server_serialize(data_t *data, char *script_str,
  int *str_size);   
server_event_t server_deserialize(data_t *data, char *script_str,
  int *str_size);   

#endif/*_SERVER_DATA_H_*/
