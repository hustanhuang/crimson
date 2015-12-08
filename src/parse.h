#ifndef PARSE_H
#define PARSE_H

#include "list.h"
#include "sds.h"

typedef struct {
    sds value;
    list_t params;
} Param;

#define param_entry(pos) list_entry(pos, Param, params)
#define param_value(pos) (param_entry(pos)->value)

extern int param_add(list_t *head, const char *token);

extern void param_free(list_t *head);

extern int parse(int fd, char buff[], int n);

#endif
