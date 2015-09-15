/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include "coordinator_data.h"

int data_print(char *msg_line, data_t *data)
{
    int i;

    printf("%s\n", msg_line);
    printf("===========data==========\n");
    printf("ip = %s\n", inet_ntoa(data->dev_addr.sin_addr));
  
    for (i = 0; i < OPTIONS_NUM; i++)
      printf("\t %d = %d\n", i, data->opts[i].value);

    printf("=========================\n");

    return 0;
}
