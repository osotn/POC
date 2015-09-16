/*   _^_   Smart_Home project  
 *  /|o|\      (c) 2015
 *  =====
 */
#ifndef COORDINATOR_DATA_H
#define COORDINATOR_DATA_H

#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_DEBUG_PRINT
#define SLAVE_DEBUG_PRINT

#define OPTIONS_NUM 8
//XXX We don't care about buffers yet, so 1000 is always more than enough
#define BUF_TMP_SIZE 1000

// XXX Do we need struct? Probably yes.
#if 1
typedef struct data_opt_t
{
    int32_t value;
} data_opt_t;
#else
typedef int32_t data_opt_t;
#endif
typedef struct data_t
{
    struct sockaddr_in dev_addr;
    data_opt_t opts[OPTIONS_NUM];
} data_t;

int data_print(char *msg_line, data_t *data);

#endif/*COORDINATOR_DATA_H*/
