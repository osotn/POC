/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef _SLAVE_DATA_H_
#define _SLAVE_DATA_H_

#include "slave_interface.h"

slave_event_t slave_serialize(data_t *data, uint8_t *pkg);   
slave_event_t slave_deserialize(data_t *data, uint8_t *pkg); 

#endif /*_SLAVE_DATA_H_*/  

