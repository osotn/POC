/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <string.h>
#include "../slave_data.h"

#if 1
  /* XXX TEST */
  static struct sockaddr_in  s_addr;
  static int32_t s_opts[OPTIONS_NUM] = {};
#endif


/* Serialize: "data" to "pkg" */
slave_event_t slave_serialize(data_t *data, int8_t *pkg, struct sockaddr_in *addr)
{
    int i;
    size_t opt_value_size = sizeof(data->opts->value);
#ifdef SLAVE_DEBUG_PRINT
    printf("Slave Serialize: data\n");
#endif
#if 1
    /* XXX TEST */
    s_addr = data->dev_addr;
    for (i = 0; i < OPTIONS_NUM; i++)
      s_opts[i] = data->opts[i].value;
#endif

    memcpy(&data->dev_addr.sin_addr, &addr->sin_addr, sizeof(struct in_addr));

    for (i = 0; i < OPTIONS_NUM; i++)
    {
        memcpy(pkg, &data->opts[i].value, opt_value_size);
        pkg += opt_value_size;
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
    size_t opt_value_size = sizeof(data->opts->value);
#ifdef SLAVE_DEBUG_PRINT
    printf("Slave Deserialize: pkg \n");
#endif
#if 1
    /* XXX TEST */
    data->dev_addr = s_addr;
    for (i = 0; i < OPTIONS_NUM; i++)
      data->opts[i].value = s_opts[i];
#else    
    memcpy(&addr->sin_addr, &data->dev_addr.sin_addr, sizeof(struct in_addr));
    
    for (i = 0; i < OPTIONS_NUM; i++)
    {
        memcpy(&data->opts[i].value, pkg, opt_value_size);
        pkg += opt_value_size;
    }
#endif
#ifdef SLAVE_DEBUG_PRINT
    data_print("Slave Deserialize:", data);
#endif
    return SLAVE_DATA;
}
