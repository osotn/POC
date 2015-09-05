/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <string.h>
#include "../slave_data.h"

/* Serialize: "data" to "pkg" */
slave_event_t slave_serialize(data_t *data, int8_t *pkg, struct sockaddr_in *addr)
{
    size_t opt_value_size = sizeof(data->opts->value);
    uint32_t i;

    printf("Slave Serialize: data = %p\n", data);

// XXX Remove when server will be implemented and store valid data here 
#if 0
    memcpy(addr, &data->dev_addr, sizeof(struct sockaddr_in));
#endif
    for (i = 0; i < OPTIONS_NUM; i++)
    {
        memcpy(pkg, &data->opts[i].value, opt_value_size);
        pkg += opt_value_size;
    }
    printf("Slave Serialize: pkg = %p\n", pkg);

    return SLAVE_DATA;
}   

/* Deserialize: "pkg" to "data" */
slave_event_t slave_deserialize(data_t *data, int8_t *pkg, struct sockaddr_in *addr)
{
    size_t opt_value_size = sizeof(data->opts->value);
    uint32_t i;

    printf("Slave Deserialize: data = %p\n", data);
    
    memcpy(&data->dev_addr, addr, sizeof(struct sockaddr_in));
    
    for (i = 0; i < OPTIONS_NUM; i++)
    {
        memcpy(&data->opts[i].value, pkg, opt_value_size);
        pkg += opt_value_size;
    }

    printf("Slave Deserialize: pkg = %s\n", pkg);

    return SLAVE_DATA;
}
