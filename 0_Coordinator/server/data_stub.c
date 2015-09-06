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

server_event_t server_deserialize(data_t *data, char *script_str,
  int *str_size)
{
  char *str, *p, *dev_name;
  char cmd[500], buf[500];
  FILE *fp;

  printf("Server Deserialize: script = \"%s\"\n", script_str);

  /* script_str example:  "kitchen_led left on" */

  str = malloc(strlen(script_str)+1);
  strcpy(str, script_str);

  dev_name = p = strtok(str, " ");

  /* XXX test */
  dev_name = "kitchen_led";

  sprintf(cmd, "python ./server/smarthome_cfg_parse.py --device %s --file ./etc/cfg.xml",
    dev_name);

  if ((fp = popen(cmd, "r")) == NULL)
  {
    printf("Error in cmd %s\n", cmd);
    free(str);
    return SERVER_ERROR;
  }
  
  while (fgets(buf, sizeof(buf), fp) != NULL)
    printf("OUTPUT: %s", buf);

  pclose(fp);
 
  printf("Server Deserialize: data = %p\n", data);


  free(str);
  return SERVER_DATA;
}
