/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */

#include "server_data.h"
#include "slave_data.h"

static int handler(server_event_t event, data_t *command, data_t *answer)
{
    /* TODO log - event data ...
     */ 

    if (event == SERVER_DATA)
    {
        slave_event_t slave_event = slave_update(command, answer);

        if (slave_event != SLAVE_DATA)
        {
            /* XXX Error message */
            return -1;
        }
    }

    return 0;
}  

int coordinator_run(void)
{
    /* Inject callbacks to slave interface */
    slave_init(slave_serialize, slave_deserialize);

    /* Inject callbacks to and run server interface */
    return server_run(handler, server_serialize, server_deserialize);
}
