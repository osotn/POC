/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef SLAVE_DATA_H
#define SLAVE_DATA_H

#include "slave_interface.h"

slave_event_t slave_serialize(data_t *data, int8_t *pkg);
slave_event_t slave_deserialize(data_t *data, int8_t *pkg);

#endif/*SLAVE_DATA_H*/
