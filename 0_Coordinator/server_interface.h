/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef _SERVER_INTERFACE_H_
#define _SERVER_INTERFACE_H_

#include "data.h"

typedef enum {
    SERVER_DATA,
    SERVER_ERROR,
} server_event_t;
 
/* Callbacks */
typedef server_event_t(*server_serialize_cb_t)(data_t *data, char *script_str,
  int *str_size);   
typedef server_event_t(*server_deserialize_cb_t)(data_t *data,
  char *script_str, int *str_size);
typedef int(*server_handle_cb_t)(server_event_t event, data_t *command,
  data_t *answer);

/* Methods */
int server_run(server_handle_cb_t handle_cb, server_serialize_cb_t serialize_cb,
  server_deserialize_cb_t deserialize_cb);

#endif/*_SERVER_INTERFACE_H_*/
