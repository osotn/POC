/*   _^_   Smart_Home project
 *  /|o|\      (c) 2015
 *  =====
 */

#include <string.h>
#include "../server_data.h"

#define UPDATE "./server/smarthome_data_update.py"
#define PARSE "./server/smarthome_cfg_parse.py"

static int do_cmd(char *cmd_name, char *addr, char *opts, char *buf,
    int buf_size, char *print_msg);
static void get_opts_value(data_t *data, char *p_strtok){}

#define DATA_WRITE_CMD "python UPDATE --addr %s --write%s --file ./etc/data.xml"
#define CFG_VALUE2NAME_CMD  "python PARSE --addr %s%s --file ./etc/cfg.xml"
server_event_t server_serialize(data_t *data, char *script_str, int *str_size)
{
    int i, len;
    char opts[BUF_TMP_SIZE], buf[BUF_TMP_SIZE], *addr, *p;
#ifdef SERVER_DEBUG_PRINT
    printf("Server Serialize:\n");
#endif
    addr = inet_ntoa(data->dev_addr.sin_addr);

    for (i = 0, len = 0; i < OPTIONS_NUM; i++)
        len += sprintf(opts + len, " %d %d", i, data->opts[i].value);

    if (do_cmd(DATA_WRITE_CMD, addr, opts, buf, sizeof(buf),
        "Write to data.xml"))
    {
        return SERVER_ERROR;
    }
#ifdef SERVER_DEBUG_PRINT
    printf("Data update result: \"%s\"\n", buf);
#endif
    /* Output: {OK, FAILED}
     */
    if (strcmp(buf, "OK\n")) /* ? Python send "OK\n" om print "OK" */
    {
        printf("data parser failed: %s\n", buf);
        return SERVER_ERROR;
    }

    /* CFG: Translate to name */
    if (do_cmd(CFG_VALUE2NAME_CMD, addr, opts, buf, sizeof(buf),
        "CFG Translate values to name command"))
    {
        return SERVER_ERROR;
    }
#ifdef SERVER_DEBUG_PRINT
    printf("Data update result: %s\n", buf);
#endif
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
#ifdef SERVER_DEBUG_PRINT
    printf("Server Serialize: script = \"%s\"\n", script_str);
#endif

    /* CFG: Translate to name */
    if (do_cmd(CFG_VALUE2NAME_CMD, addr, opts, buf, sizeof(buf),
        "CFG Translate values to name command"))
    {
        return SERVER_ERROR;
    }
#ifdef SERVER_DEBUG_PRINT
    printf("Data update result: %s\n", buf);
#endif

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

#define CFG_NAME2VALUE_CMD  "python PARSE --device %s --file ./etc/cfg.xml"
#define DATA_READ_CMD "python UPDATE --addr %s --file ./etc/data.xml"
server_event_t server_deserialize(data_t *data, char *script_str,
    int *str_size)
{
    char cfg_buf[BUF_TMP_SIZE], data_buf[BUF_TMP_SIZE], *p_cfg, *p_data, *p;

    /* Disable all options */
    memset(data, 0, sizeof(*data));

#ifdef SERVER_DEBUG_PRINT
    printf("Server Deserialize: script = \"%s\"\n", script_str);
#endif

    if (do_cmd(CFG_NAME2VALUE_CMD, NULL, script_str, cfg_buf, sizeof(cfg_buf),
        "Server Deserialize"))
    {
        return SERVER_ERROR;
    }

#ifdef SERVER_DEBUG_PRINT
    printf("\tCfg parser result: %s\n", cfg_buf);
#endif

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
    if (do_cmd(DATA_READ_CMD, NULL, p, data_buf, sizeof(data_buf),
        "Server Deserialize"))
    {
        return SERVER_ERROR;
    }

#ifdef SERVER_DEBUG_PRINT
    printf("Data update result: %s\n", data_buf);
#endif

    /* Output: {OK, FAILED} [id=value]
     */
    p = strtok_r(data_buf, " ", &p_data);

    /* ip address */
    p = strtok_r(NULL, " ", &p_cfg);

    if ((data->dev_addr.sin_addr.s_addr = inet_addr(p)) == INADDR_NONE)
    {
        printf("IP address string  %s is failed\n", p);
        return SERVER_ERROR;
    }

    /* Get states from data.xml */
    if (do_cmd(DATA_READ_CMD, NULL, p, data_buf, sizeof(data_buf),
        "Server Deserialize"))
    {
        return SERVER_ERROR;
    }

    printf("Data update result: %s\n", data_buf);
    
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
#ifdef SERVER_DEBUG_PRINT
    data_print("Server Deserialize:", data);
#endif
 
    get_opts_value(data, p_data);

    /* Get new values */
    get_opts_value(data, p_cfg);
    
    data_print("Server Deserialize:", data);

    return SERVER_DATA;
}

static int do_cmd(char *cmd_name, char *addr, char *opts, char *buf,
    int buf_size, char *print_msg)
{
    char cmd[BUF_TMP_SIZE] = {};
    FILE *fp;

    if (addr)
        snprintf(cmd, sizeof(cmd), cmd_name, addr, opts);
    else
        snprintf(cmd, sizeof(cmd), cmd_name, opts);
#ifdef SERVER_DEBUG_PRINT
    printf("%s: %s\n", print_msg, cmd);
#endif
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
