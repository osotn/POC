/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <stdio.h>
#include <errno.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "../slave_interface.h"

static slave_serialize_cb_t serialize_cb;
static slave_deserialize_cb_t deserialize_cb;

#define PKG_LEN (OPTIONS_NUM * sizeof(int32_t))

// XXX For debug only
#if 0
static void print_opt(data_t *data,  struct sockaddr_in *dev_addr)
{
    int i;

    printf("\n\tOUTPUT: dev_name %s addr %s port %d\n", data->dev_name,
        inet_ntoa(dev_addr->sin_addr), ntohs(dev_addr->sin_port));
    for (i = 0; i < 3; i++)
    {
        data_opt_t *option = &data->opts[i];
        printf("\tOUTPUT: %d name  %s\n", i, option->name);
        printf("\tOUTPUT: %d str_v %s\n", i, option->str_value);
        printf("\tOUTPUT: %d value %d\n", i, option->value);
    }
    printf("\n");
}
#endif

slave_event_t slave_update(data_t *command, data_t *answer)
{
    struct timeval timeout = {
        .tv_sec = 0,
        .tv_usec = 10000
    };
    struct sockaddr_in dev_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(5005),
    };
    int8_t pkg[PKG_LEN] = {};
    int len = PKG_LEN, addr_len = sizeof(struct sockaddr_in);
    int sockfd;
    
    if (!serialize_cb || !deserialize_cb)
        return SLAVE_INIT_ERROR;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
    {
        perror("\n Error : Could not create socket \n");
        return SLAVE_NETWORK_ERROR;
    }

    if (serialize_cb(command, pkg, &dev_addr) != SLAVE_DATA)
        return SLAVE_DATA_ERROR;

    printf("Slave Interface Stub: Send command = '%p' to slave\n", pkg);
// XXX For debug only
#if 0
    print_opt(command, &dev_addr);
#endif

    if ((len = sendto(sockfd, pkg, sizeof(pkg), 0,
        (struct sockaddr *)&dev_addr, addr_len)) < 0)
    {
        perror("send");
        return SLAVE_NETWORK_ERROR;
    }
    
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0)
    {
        perror("Error");
        return SLAVE_NETWORK_ERROR;
    }

    if ((len = recvfrom(sockfd, pkg, sizeof(pkg), 0,
        (struct sockaddr *)&dev_addr, (socklen_t *)&addr_len)) <= 0)
    {
        perror("recv");
        return SLAVE_TIMEOUT_ERROR;
    }
    
    printf("Slave Interface Stub: Recv answer = '%p' from slave\n", pkg);
    if (deserialize_cb(answer, pkg, &dev_addr) != SLAVE_DATA)
        return SLAVE_DATA_ERROR;

    return SLAVE_DATA;
}

int slave_init(slave_serialize_cb_t serialize_cb_,
    slave_deserialize_cb_t deserialize_cb_)
{
    serialize_cb = serialize_cb_;
    deserialize_cb = deserialize_cb_;
    return 0;
}
