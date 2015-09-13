/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../server_data.h"

server_event_t server_serialize(data_t *data, char *script_str,
  int *str_size)
{
  printf("Server Serialize: data = %p\n", data);

  /* XXX */
  strcpy(script_str, "==answer from slave to server==");

  /* TODO 1. Write data to data.xml - alredy ready in smarthome_data_update.py
   *          ? - how define end of package, slave need to set enable option.
   *
   *      2. Translate values to names, update smarthome_cfg_parse.py
   */

  printf("Server Serialize: script = \"%s\"\n", script_str);

  return SERVER_DATA;
}

#define CFG_PARSER_CMD  "python ./server/smarthome_cfg_parse.py --device %s --file ./etc/cfg.xml"
#define DATA_PARSER_CMD "python ./server/smarthome_data_update.py --addr %s --file ./etc/data.xml"
server_event_t server_deserialize(data_t *data, char *script_str,
  int *str_size)
{
  char cmd[1000], cfg_buf[1000], data_buf[1000], *p_cfg, *p_data, *p;
  FILE *fp;

  /* Disable all options */
  memset(data, 0, sizeof(*data));

  printf("Server Deserialize: script = \"%s\"\n", script_str);
  
  snprintf(cmd, sizeof(cmd), CFG_PARSER_CMD, script_str);

  if (!(fp = popen(cmd, "r")))
  {
    perror("Error in popen\n");
    return SERVER_ERROR;
  }

  if(!fgets(cfg_buf, sizeof(cfg_buf), fp))
  {
    pclose(fp);
    return SERVER_ERROR;
  }
  pclose(fp);

  printf("Cfg parser result: %s\n", cfg_buf);

  /* Output: {OK, FAILED} <dev_ip> [id=value]
   */

  p = strtok_r(cfg_buf, " ", &p_cfg);

  if (strcmp(p, "OK"))
  {
    printf("cfg parser: %s\n", cfg_buf);
    return SERVER_ERROR;
  }

  /* ip address */
  p = strtok_r(NULL, " ", &p_cfg);

  if (-1 == (data->dev_addr.sin_addr.s_addr = inet_addr(p)))
  {
    printf("IP address string  %s is failed\n", p);
    return SERVER_ERROR;
  }

  /* Get states from data.xml */
  snprintf(cmd, sizeof(cmd), DATA_PARSER_CMD, p);

  if (!(fp = popen(cmd, "r")))
  {
    perror("Error in popen\n");
    return SERVER_ERROR;
  }

  if(!fgets(data_buf, sizeof(data_buf), fp))
  {
    pclose(fp);
    return SERVER_ERROR;
  }
  pclose(fp);

  /* Output: {OK, FAILED} [id=value]
   */

  p = strtok_r(data_buf, " ", &p_data);

  if (strcmp(p, "OK"))
  {
    printf("data parser: %s\n", data_buf);
    return SERVER_ERROR;
  }
  
  while ((p = strtok_r(NULL, "=", &p_data)))
  {
    int id, value;

    id = atoi(p);
    p = strtok_r(NULL, " ", &p_data);
    value = atoi(p);

    if (id >= 0 && id < OPTIONS_NUM)
    {
      data->opts[id].value = value;
      data->opts[id].enable = true;
    }
  }

  /* new values */
  while ((p = strtok_r(NULL, "=", &p_cfg)))
  {
    int id, value;

    id = atoi(p);
    p = strtok_r(NULL, " ", &p_cfg);
    value = atoi(p);

    if (id >= 0 && id < OPTIONS_NUM)
    {
      data->opts[id].value = value;
      data->opts[id].enable = true;
    }
  }

  printf("Server Deserialize: data:\n");
  printf("=========================\n");
  printf("ip = %s\n", inet_ntoa(data->dev_addr.sin_addr));
  {
    int i = 0;
    for (i=0; i<OPTIONS_NUM; i++)
    {
      if (data->opts[i].enable)
        printf("\t %d = %d\n", i, data->opts[i].value);
    }
  }
  printf("=========================\n");

  return SERVER_DATA;
}
