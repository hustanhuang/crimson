#include "include/funcs.h"

#include "include/db.h"
#include "include/obj.h"

int func_test(int fd, list_t *params) {
    db_put(obj_create("hehe", STRING_T));
    return 0;
}
