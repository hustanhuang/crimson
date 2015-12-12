#include "include/funcs.h"

#include <unistd.h>
#include <string.h>

#include "include/parse.h"
#include "include/utils.h"
#include "include/config.h"
#include "include/list.h"

#include "include/db.h"
#include "include/obj.h"

#include "include/c_string.h"

int check_paranum(int fd, list_t *params, int expected) {
    const char *msg = "(error) ERR wrong number of arguments\r\n";
    size_t msglen = strlen(msg);

    int param_num = 0;
    list_t *pos;
    list_for_each(pos, params)
        ++param_num;

    if (param_num - 1 != expected) {
        exit_if(write(fd, msg, msglen) <= 0);
        return 1;
    } else
        return 0;
}

int func_set(int fd, list_t *params) {
    if (check_paranum(fd, params, 2))
        return 0;
    obj_t *obj_ptr = obj_create(param_value(params->next->next), STRING_T);
    c_string_set(obj_ptr->obj, param_value(params->prev));
    c_string_get(obj_ptr->obj);

    obj_free(db_put(obj_ptr));

    const char *msg = "OK\r\n";
    size_t msglen = strlen(msg);
    exit_if(write(fd, msg, msglen) <= 0);

    return 0;
}

int func_get(int fd, list_t *params) {
    if (check_paranum(fd, params, 1))
        return 0;
    obj_t *obj_ptr = db_get(param_value(params->next->next));
    if (obj_ptr == NULL) {
        const char *msg = "(nil)\r\n";
        size_t msglen = strlen(msg);
        exit_if(write(fd, msg, msglen) <= 0);
        return 0;
    }

    char buff[MAX_BUFF_LEN];
    bzero(buff, sizeof(buff));
    switch (obj_ptr->type) {
        case STRING_T:
            sprintf(buff, "%s\r\n", c_string_get(obj_ptr->obj));
            exit_if(write(fd, buff, strlen(buff)) <= 0);
            break;
        case LIST_T:
            break;
        case HASH_T:
            break;
        case SET_T:
            break;
        default:
            break;
    }
    return 0;
}

int func_del(int fd, list_t *params) {
    if (check_paranum(fd, params, 1))
        return 0;
    obj_t *obj_ptr = db_del(param_value(params->next->next));
    obj_free(obj_ptr);

    const char *msg = "OK\r\n";
    size_t msglen = strlen(msg);
    exit_if(write(fd, msg, msglen) <= 0);

    return 0;
}

int func_exists(int fd, list_t *params) {
    int exists_num = 0;

    for (list_t *pos = params->next->next; pos != params; pos = pos->next) {
        c_log(param_value(pos));
        if (db_get(param_value(pos)))
            ++exists_num;
    }

    char msg[MAX_BUFF_LEN];
    sprintf(msg, "%d\r\n", exists_num);
    size_t msglen = strlen(msg);
    exit_if(write(fd, msg, msglen) <= 0);
    return 0;
}
