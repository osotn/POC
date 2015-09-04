/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef SLAVE_INTERFACE_H
#define SLAVE_INTERFACE_H

#include "coordinator_data.h"

typedef enum {
    SLAVE_DATA,
    SLAVE_ERROR,
} slave_event_t;

/* Callbacks */
typedef slave_event_t(*slave_serialize_cb_t)(data_t *data, int8_t *pkg); 
typedef slave_event_t(*slave_deserialize_cb_t)(data_t *data, int8_t *pkg);

/* Methods */
int slave_init(slave_serialize_cb_t serialize_cb,
  slave_deserialize_cb_t deserialize_cb);
int slave_update(data_t *command, data_t *answer);

#endif/*SLAVE_INTERFACE_H*/
