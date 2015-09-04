/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include "../slave_interface.h"

slave_serialize_cb_t serialize_cb;
slave_deserialize_cb_t deserialize_cb;

int slave_init(slave_serialize_cb_t serialize_cb_,
    slave_deserialize_cb_t deserialize_cb_)
{
  serialize_cb = serialize_cb_;
  deserialize_cb = deserialize_cb_;
  return 0;
}

int slave_update(data_t *command, data_t *answer)
{
  slave_event_t event;
  int8_t pkg[32];

  printf("Slave Interface Stub: Send command = %p to slave\n", command);
  event = serialize_cb(command, pkg); 
  
  printf("Send pkg p = %p\n", (uint8_t *)pkg);
  printf("Recv pkg\n");

  /* Change answer */

  event = deserialize_cb(answer, pkg);
  printf("Slave Interface Stub: Recv answer = %p from slave\n", answer);

  return event;
}
