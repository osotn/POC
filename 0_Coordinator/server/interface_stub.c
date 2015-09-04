/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <unistd.h>
#include "../server_interface.h"

int server_run(server_handle_cb_t handle_cb, server_serialize_cb_t serialize_cb,
  server_deserialize_cb_t deserialize_cb)
{
  int i=0;

  while (1)
  {
    data_t cmd, answer;
    server_event_t event;
    char cmd_script_str[] = "==cmd from script==";
    char answer_script_str[100];
    int size = 100;

    printf("%d. Server_interface Stub: Received cmd from server:\n", i++);
    printf("\t cmd = \"%s\"\n", cmd_script_str);
    event = deserialize_cb(&cmd, cmd_script_str, 0);

    handle_cb(event, &cmd, &answer);

    event = serialize_cb(&answer, answer_script_str, &size);  
    printf("Server_interface Stub: Received answer to server:\n");
    printf("\t answer = \"%s\"\n", answer_script_str);

    sleep(2);
  }

  return 0;
}
