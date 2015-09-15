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
static char *opts_is_valid(char *print_msg, char *buf, char **p_data);
static void get_opts_value(data_t *data, char **p_strtok);

#define DATA_WRITE_CMD "python "UPDATE" --addr %s --write%s --file ./etc/data.xml"
#define CFG_VALUE2NAME_CMD  "python "PARSE" --addr %s%s --file ./etc/cfg.xml"
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

    /* Output: {OK, FAILED} */
    if (strcmp(buf, "OK\n")) /* ? Python send "OK\n" */
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

    /* Output: {OK, FAILED} [<option_name> <option_value>] */
    if (!(p = opts_is_valid("data parser", buf, NULL)))
        return SERVER_ERROR;

    while (*p++);

    strncpy(script_str, p, *str_size);
#ifdef SERVER_DEBUG_PRINT
    printf("Server Serialize: script = \"%s\"\n", script_str);
#endif
    return SERVER_DATA;
}

#define CFG_NAME2VALUE_CMD  "python "PARSE" --device %s --file ./etc/cfg.xml"
#define DATA_READ_CMD "python "UPDATE" --addr %s --file ./etc/data.xml"
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

    /* Output: {OK, FAILED} <dev_ip> [id=value] */
    if (!(p = opts_is_valid("cfg parser", cfg_buf, &p_cfg)))
        return SERVER_ERROR;

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
    /* Output: {OK, FAILED} [id=value] */
    if (!(p = opts_is_valid("data parser", data_buf, &p_data)))
        return SERVER_ERROR;

    get_opts_value(data, &p_data);

    /* new values */
    get_opts_value(data, &p_cfg);
    
#ifdef SERVER_DEBUG_PRINT
    data_print("Server Deserialize:", data);
#endif
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

static char *opts_is_valid(char *print_msg, char *buf, char **p_data)
{
    char *p = p_data ? strtok_r(buf, " ", p_data) : strtok(buf, " ");

    if (strcmp(p, "OK"))
    {
        printf("%s: %s\n", print_msg, buf);
        return NULL;
    }
    return p;
}

static void get_opts_value(data_t *data, char **p_strtok)
{
    char *p;
    int id, value;

    while ((p = strtok_r(NULL, "=", p_strtok)))
    {
        id = atoi(p);
        p = strtok_r(NULL, " ", p_strtok);
        value = atoi(p);

        if (id >= 0 && id < OPTIONS_NUM)
            data->opts[id].value = value;
    }
}
