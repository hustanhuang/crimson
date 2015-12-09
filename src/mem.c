#include "include/mem.h"

#include <stdlib.h>

#include "include/list.h"

list_t allocated_mems;

int mem_init() {
    C_INIT_LIST_HEAD(&allocated_mems);
    return 0;
}

int mem_free() {
    list_t *pos, *p;
    list_for_each_safe(pos, p, &allocated_mems) {
        free(mem_get(pos)->mem);
        free(mem_get(pos));
    }
    return 0;
}
