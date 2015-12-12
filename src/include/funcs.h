#ifndef FUNCS_H
#define FUNCS_H

#include "list.h"

extern int func_set(int fd, list_t *params);
extern int func_get(int fd, list_t *params);
extern int func_del(int fd, list_t *params);
extern int func_exists(int fd, list_t *params);

#endif
