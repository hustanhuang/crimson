#include "parse.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utils.h"
#include "list.h"
#include "sds.h"

void param_add(list_t *head, const char *token) {
    Param *new_para = malloc(sizeof(Param));
    new_para->value = sdsnew(token);
    list_insert(&new_para->params, head);
}

void param_free(list_t *head) {
    list_t *pos, *p;
    list_for_each_safe(pos, p, head) {
        sdsfree(param_value(pos));
        free(param_entry(pos));
    }
}

int parse(int fd, char buff[], int n) {
    int i = n;
    while (i != 0) {
        if (isprint(buff[i - 1]))
            break;
        --i;
    }
    buff[i] = '\0';

    C_LIST_HEAD(params);

    /* the parser need to be improved to deal with ' and " */
    const char *spaces = "\x09\x0a\x0b\x0c\x0d\x20";
    char *token;
    while( (token = strsep(&buff, spaces)) != NULL)
        if (*token)
            param_add(&params, token);

    /* procession commands here */
    list_t *pos;
    list_for_each(pos, &params) {
        c_prt(param_value(pos), strlen(param_value(pos)));
        param_value(pos) = sdscat(param_value(pos), "\r\n");
        exit_if(write(fd, param_value(pos), strlen(param_value(pos)) + 1) <= 0);
    }

    param_free(&params);

    return 0;
}
