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
    obj_free(db_put(obj_ptr));

    const char *msg = "OK\r\n";
    size_t msglen = strlen(msg);
    exit_if(write(fd, msg, msglen) <= 0);

    return 0;
}

int func_get(int fd, list_t *params) {
    if (check_paranum(fd, params, 1))
        return 0;
    obj_t *obj_ptr = obj_create(param_value(params->next->next), STRING_T);
    obj_free(db_put(obj_ptr));
    return 0;
}
