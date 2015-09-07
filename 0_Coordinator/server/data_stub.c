/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../server_data.h"

server_event_t server_serialize(data_t *data, char *script_str,
  int *str_size)
{
  printf("Server Serialize: data = %p\n", data);

  strcpy(script_str, "==answer from slave to server==");

  printf("Server Serialize: script = \"%s\"\n", script_str);

  return SERVER_DATA;
}

static server_event_t parse_line(int i, data_t *data, char *line)
{
    static int opts_num = 0;
    char addr[16];
// For debug only
#if 1
    printf("\tOUTPUT: %d %s\n", i, line);
#endif
    if (!i)
    {
        sscanf(line, "%s %s %d", data->dev_name, addr, &opts_num);
        if (!inet_aton(addr, &data->dev_addr.sin_addr))
        {
            printf("Invalid address\n");
            return SERVER_ERROR;
        }
    }//XXX Shoud we use i < opts_num ?
    else if (0 < i < OPTIONS_NUM)
    {
        int j = i - 1;
        
        sscanf(line, "%s %d %s", data->opts[j].name, &data->opts[j].value,
              data->opts[j].str_value);
    }
    else
    {
        return SERVER_ERROR;
    }
    return SERVER_DATA;
}

#define PARSER_CMD "python ./server/smarthome_cfg_parse.py --device %s --file ./etc/cfg.xml"
server_event_t server_deserialize(data_t *data, char *script_str,
  int *str_size)
{
  char *script_cmd, *dev_name;
  char cmd[sizeof(PARSER_CMD) + NAME_LEN], line_buf[2 * NAME_LEN];
  int line = 0, script_str_len = *str_size ? *str_size : strlen(script_str) + 1;
  FILE *fp;

  printf("Server Deserialize: script = \"%s\"\n", script_str);
#if 0
  if (!(script_cmd = malloc(script_str_len)))
      return SERVER_ERROR;
  
  strncpy(script_cmd, script_str, script_str_len);

  if (!(dev_name = strtok(script_cmd, " ")))
#endif
  if (!(dev_name = strtok(script_str, " ")))
  {
#if 0
      free(script_cmd);
#endif
      return SERVER_ERROR;
  }

  snprintf(cmd, sizeof(cmd), PARSER_CMD, dev_name);

  if (!(fp = popen(cmd, "r")))
  {
    perror("Error in popen\n");
#if 0
    free(script_cmd);
#endif
    return SERVER_ERROR;
  }
  
  while (fgets(line_buf, sizeof(line_buf), fp))
  {
    if (parse_line(line++, data, line_buf) != SERVER_DATA)
        return SERVER_ERROR;
  }

  pclose(fp);
 
  printf("Server Deserialize: data = %p\n", data);
#if 0
  free(script_cmd);
#endif
  return SERVER_DATA;
}
