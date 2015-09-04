/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef _SLAVE_INTERFACE_H_
#define _SLAVE_INTERFACE_H_

#include <stdint.h>
#include "data.h"

typedef enum {
    SLAVE_DATA,
    SLAVE_ERROR,
} slave_event_t;

/* Callbacks */
typedef slave_event_t(*slave_serialize_cb_t)(data_t *data, uint8_t *pkg);   
typedef slave_event_t(*slave_deserialize_cb_t)(data_t *data, uint8_t *pkg);

/* Methods */
int slave_init(slave_serialize_cb_t serialize_cb,
  slave_deserialize_cb_t deserialize_cb);
int slave_update(data_t *command, data_t *answer);

#endif/*_SLAVE_INTERFACE_H_*/
