/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef SERVER_DATA_H
#define SERVER_DATA_H

#include "server_interface.h"

server_event_t server_serialize(data_t *data, char *script_str, int *str_size);
server_event_t server_deserialize(data_t *data, char *script_str,
    int *str_size);

#endif/*SERVER_DATA_H*/
