/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <string.h>
#include <netinet/in.h>
#include "../slave_data.h"

/* Serialize: "data" to "pkg" */
slave_event_t slave_serialize(data_t *data, int8_t *pkg, struct sockaddr_in *addr)
{
    int i;

#ifdef SLAVE_DEBUG_PRINT
    printf("Slave Serialize: data\n");
    data_print(">>", data);
#endif

    memcpy(&addr->sin_addr, &data->dev_addr.sin_addr, sizeof(struct in_addr));

    for (i = 0; i < OPTIONS_NUM; i++)
    {
        /* Send values in network byte-order */
        uint32_t value = htonl(data->opts[i].value);
        memcpy(pkg, &value, sizeof(value));
        pkg += sizeof(value);
    }

#ifdef SLAVE_DEBUG_PRINT
    printf("Slave Serialize: pkg\n");
#endif

    return SLAVE_DATA;
}   

/* Deserialize: "pkg" to "data" */
slave_event_t slave_deserialize(data_t *data, int8_t *pkg, struct sockaddr_in *addr)
{
    int i;

#ifdef SLAVE_DEBUG_PRINT
    printf("Slave Deserialize: pkg \n");
#endif

    memcpy(&data->dev_addr.sin_addr, &addr->sin_addr, sizeof(struct in_addr));

    for (i = 0; i < OPTIONS_NUM; i++)
    {
        uint32_t value;

        memcpy(&value, pkg, sizeof(value));

        /* Translate from network to host byte-order */
        data->opts[i].value = ntohl(value);

        pkg += sizeof(value);
    }

#ifdef SLAVE_DEBUG_PRINT
    data_print("Slave Deserialize:", data);
#endif

    return SLAVE_DATA;
}
