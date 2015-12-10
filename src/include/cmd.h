#ifndef CMD_H
#define CMD_H

#include "list.h"

typedef struct cmd_t {
    const char *const name;
    int (*func)(int fd, list_t *params);
} cmd_t;

extern int exec_func(int fd, list_t *params);

#endif
