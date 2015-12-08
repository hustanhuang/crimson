#include "../include/parse.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/utils.h"
#include "../include/list.h"
#include "../include/sds.h"

int param_add(list_t *head, const char *token) {
    if (!*token)
        return 0;
    Param *new_para = malloc(sizeof(Param));
    new_para->value = sdsnew(token);
    list_insert(&new_para->params, head);
    return sdslen(new_para->value);
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

    int single_quote = 0,
        double_quote = 0;
    for (i = 0; i != n; ++i) {
        if (isspace(buff[i]) && !single_quote && !double_quote) {
            buff[i] = '\0';
        } else if (buff[i] == '\'' && !double_quote) {
            buff[i] = '\0';
            single_quote = !single_quote;
        } else if (buff[i] == '\"' && !single_quote) {
            buff[i] = '\0';
            double_quote = !double_quote;
        }
    }

    if (single_quote || double_quote) {
        /* quote mark matching failed */
        return 0;
    }

    for (int i = 0; i < n; ++i)
        i += param_add(&params, buff + i);

    /* processing command */
    list_t *pos;
    list_for_each(pos, &params) {
        c_prt(param_value(pos), strlen(param_value(pos)));
        param_value(pos) = sdscat(param_value(pos), "\r\n");
        exit_if(write(fd, param_value(pos), strlen(param_value(pos)) + 1) <= 0);
    }

    param_free(&params);

    return 0;
}
