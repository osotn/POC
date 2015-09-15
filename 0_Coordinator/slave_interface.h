/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef SLAVE_INTERFACE_H
#define SLAVE_INTERFACE_H

#include "coordinator_data.h"

typedef enum {
    SLAVE_DATA = 0,
    SLAVE_INIT_ERROR = 1,
    SLAVE_DATA_ERROR = 2,
    SLAVE_NETWORK_ERROR = 3,
    SLAVE_TIMEOUT_ERROR = 4,
} slave_event_t;

/* Callbacks */
typedef slave_event_t(*slave_serialize_cb_t)(data_t *data, int8_t *pkg,
    struct sockaddr_in *addr);
typedef slave_event_t(*slave_deserialize_cb_t)(data_t *data, int8_t *pkg,
    struct sockaddr_in *addr);

/* Methods */
int slave_init(slave_serialize_cb_t serialize_cb,
    slave_deserialize_cb_t deserialize_cb);
slave_event_t slave_update(data_t *command, data_t *answer);

#endif/*SLAVE_INTERFACE_H*/
