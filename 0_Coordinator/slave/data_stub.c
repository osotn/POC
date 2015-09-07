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
    int i;
    size_t opt_value_size = sizeof(data->opts->value);

    printf("Slave Serialize: data = %p\n", data);

    memcpy(&data->dev_addr.sin_addr, &addr->sin_addr, sizeof(struct in_addr));

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
    int i;
    size_t opt_value_size = sizeof(data->opts->value);

    printf("Slave Deserialize: data = %p\n", data);
    
    memcpy(&addr->sin_addr, &data->dev_addr.sin_addr, sizeof(struct in_addr));
    
    for (i = 0; i < OPTIONS_NUM; i++)
    {
        memcpy(&data->opts[i].value, pkg, opt_value_size);
        pkg += opt_value_size;
    }

    printf("Slave Deserialize: pkg = %p\n", pkg);

    return SLAVE_DATA;
}
