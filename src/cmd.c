#include "include/cmd.h"

#include <unistd.h>
#include <string.h>

#include "include/list.h"
#include "include/utils.h"
#include "include/parse.h"

#include "include/funcs.h"

int func_shut(int fd, list_t *params) {
    request_quit = 1;
    const char *const msg = "shutdown\r\n";
    size_t msglen = strlen(msg);
    exit_if(write(fd, msg, msglen) <= 0);
    return 0;
}

int func_nil(int fd, list_t *params) {
    const char *const msg = "(error) ERR unknown command\r\n";
    size_t msglen = strlen(msg);
    exit_if(write(fd, msg, msglen) <= 0);
    return 0;
}

static cmd_t cmds[] = {
    {"set",         func_set},
    {"get",         func_get},
    {"shutdown",    func_shut},
    {NULL,          func_nil}
};

int exec_func(int fd, list_t *params) {
    const char *func_name = param_value(params->next);
    if (params->next == params)
        return 0;
    cmd_t *cmd_iter = cmds;
    while (cmd_iter->name) {
        if (strcmp(func_name, cmd_iter->name) == 0)
            break;
        ++cmd_iter;
    }
    cmd_iter->func(fd, params);

    param_free(params);

    if (request_quit)
        raise(SIGINT);
    return 0;
}
