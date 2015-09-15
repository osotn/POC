/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <string.h>
#include "../server_interface.h"

int server_run(server_handle_cb_t handle_cb, server_serialize_cb_t serialize_cb,
    server_deserialize_cb_t deserialize_cb)
{
    int i = 0;
    struct sockaddr_in client_addr = {},
    /* XXX To test: ncat -vv localhost port -u*/
    /* TODO Remove port and addr */
    serv_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(4000),
        .sin_addr.s_addr = htonl(INADDR_LOOPBACK)
    };
    int sockfd, enable = 1, len, addr_len = sizeof(struct sockaddr_in);

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        perror("socket");
        return -1;
    }

    /* TODO We do really need reusable address ? */ 
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        perror("Error");
        return -1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
    {
        perror("bind");
        return -1;
    }

    while (1)
    {
        data_t cmd, answer;
        server_event_t event;
        char cmd_script_str[1000] = {};
        char answer_script_str[1000];
        int script_str_len, size = sizeof(answer_script_str);

        // TODO test cmd from script
#if 0
        if ((len = recvfrom(sockfd, cmd_script_str, sizeof(cmd_script_str), 0,
            (struct sockaddr *)&client_addr, (socklen_t*)&addr_len)) <= 0)
        {
            if (len < 0)
                error("recv");
        }
#else
        strcpy(cmd_script_str, "kitchen_led left on");
#endif
        script_str_len = strlen(cmd_script_str) + 1;
#ifdef SERVER_DEBUG_PRINT
        printf("%d. Server_interface Stub: Received cmd from server:\n", i++);
        printf("\t cmd = \"%s\"\n", cmd_script_str);
#endif
        event = deserialize_cb(&cmd, cmd_script_str, &script_str_len);

        handle_cb(event, &cmd, &answer);

        event = serialize_cb(&answer, answer_script_str, &size);
#ifdef SERVER_DEBUG_PRINT
        printf("Server_interface Stub: Received answer to server:\n");
        printf("\t answer = \"%s\"\n", answer_script_str);
#endif
        // TODO test cmd from script
#if 0
        if ((len = sendto(sockfd, answer_script_str, sizeof(answer_script_str), 0,
            (struct sockaddr *)&client_addr, addr_len)) < 0)
        {
            perror("send");
            return 1;
        }
#endif
#ifdef SERVER_DEBUG_PRINT
        printf("---------------------------------------------------------\n\n");
#endif
        sleep(2);
    }
    return 0;
}
