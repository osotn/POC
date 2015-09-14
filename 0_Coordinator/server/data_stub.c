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

static int do_cmd(char *cmd_name, char *addr, char *opts, char *buf, int buf_size, char *print_msg);

#define DATA_WRITE_CMD "python ./server/smarthome_data_update.py --addr %s --write%s --file ./etc/data.xml"
#define CFG_VALUE2NAME_CMD  "python ./server/smarthome_cfg_parse.py --addr %s%s --file ./etc/cfg.xml"
server_event_t server_serialize(data_t *data, char *script_str, int *str_size)
{
  int i, len;
  char opts[1000], buf[1000], *addr, *p;

  printf("Server Serialize:\n");
  
  addr = inet_ntoa(data->dev_addr.sin_addr);

  for (i = 0, len = 0; i < OPTIONS_NUM; i++)
    len += sprintf(opts + len, " %d %d", i, data->opts[i].value);

  if (do_cmd(DATA_WRITE_CMD, addr, opts, buf, sizeof(buf), "Write to data.xml"))
      return SERVER_ERROR;

  printf("Data update result: \"%s\"\n", buf);

  /* Output: {OK, FAILED}
   */
  if (strcmp(buf, "OK\n")) /* ? Python send "OK\n" om print "OK" */
  {
    printf("data parser failed: %s\n", buf);
    return SERVER_ERROR;
  }

  /* CFG: Translate to name */
  if (do_cmd(CFG_VALUE2NAME_CMD, addr, opts, buf, sizeof(buf), "CFG Translate values to name command"))
      return SERVER_ERROR;

  printf("Data update result: %s\n", buf);

  /* Output: {OK, FAILED} [<option_name> <option_value>]
   */

  p = strtok(buf, " ");

  if (strcmp(p, "OK"))
  {
    printf("data parser: %s\n", buf);
    return SERVER_ERROR;
  }

  while (*p++);

  strncpy(script_str, p, *str_size);

  printf("Server Serialize: script = \"%s\"\n", script_str);

  return SERVER_DATA;
}

#define CFG_NAME2VALUE_CMD  "python ./server/smarthome_cfg_parse.py --device %s --file ./etc/cfg.xml"
#define DATA_READ_CMD "python ./server/smarthome_data_update.py --addr %s --file ./etc/data.xml"
server_event_t server_deserialize(data_t *data, char *script_str, int *str_size)
{
  char cfg_buf[1000], data_buf[1000], *p_cfg, *p_data, *p;

  /* Disable all options */
  memset(data, 0, sizeof(*data));

  printf("Server Deserialize: script = \"%s\"\n", script_str);
  
  if (do_cmd(CFG_NAME2VALUE_CMD, NULL, script_str, cfg_buf, sizeof(cfg_buf), "Server Deserialize"))
      return SERVER_ERROR;

  printf("\tCfg parser result: %s\n", cfg_buf);

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

  if ((data->dev_addr.sin_addr.s_addr = inet_addr(p)) == INADDR_NONE)
  {
    printf("IP address string  %s is failed\n", p);
    return SERVER_ERROR;
  }

  /* Get states from data.xml */
  if (do_cmd(DATA_READ_CMD, NULL, p, data_buf, sizeof(data_buf), "Server Deserialize"))
      return SERVER_ERROR;

  /* Output: {OK, FAILED} [id=value]
   */

  printf("Data update result: %s\n", data_buf);

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
      data->opts[id].value = value;
  }

  /* new values, ha-ha it's a copy-past */
  while ((p = strtok_r(NULL, "=", &p_cfg)))
  {
    int id, value;

    id = atoi(p);
    p = strtok_r(NULL, " ", &p_cfg);
    value = atoi(p);

    if (id >= 0 && id < OPTIONS_NUM)
      data->opts[id].value = value;
  }

  data_print("Server Deserialize:", data);

  return SERVER_DATA;
}

static int do_cmd(char *cmd_name, char *addr, char *opts, char *buf, int buf_size, char *print_msg)
{
    char cmd[1000] = {};
    FILE *fp;
    
    if (addr)
        snprintf(cmd, sizeof(cmd), cmd_name, addr, opts);
    else
        snprintf(cmd, sizeof(cmd), cmd_name, opts);

    printf("%s: %s\n", print_msg, cmd);

    if (!(fp = popen(cmd, "r")))
    {
        perror("Error in popen\n");
        return -1;
    }

    if (!fgets(buf, buf_size, fp))
    {
        pclose(fp);
		return -1;
    }
    pclose(fp);
    return 0;
}
