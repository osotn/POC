/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <string.h>
#include "../slave_data.h"

/* Serialize: "data" to "pkg" */
slave_event_t slave_serialize(data_t *data, int8_t *pkg)
{
    printf("Slave Serialize: data = %p\n", data);

    /* Change data */
    
    printf("Slave Serialize: pkg = %p\n", pkg);

    return SLAVE_DATA;
}   

/* Deserialize: "pkg" to "data" */
slave_event_t slave_deserialize(data_t *data, int8_t *pkg)
{
    printf("Slave Deserialize: data = %p\n", data);

    // XXX Test message
    strcpy(pkg, "==== Test message ====");

    printf("Slave Deserialize: pkg = %s\n", pkg);

    return SLAVE_DATA;
}
