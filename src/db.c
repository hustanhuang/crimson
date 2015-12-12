#include "include/db.h"

#include <string.h>
#include <assert.h>

#include "include/utils.h"

#include "include/hash.h"
#include "include/table.h"
#include "include/obj.h"

table_t all_objs;

int key_cmp(const void *x, const void *y) {
    return strcmp((const char*)x, (const char*)y);
}

unsigned int key_hash(const void *key) {
    return get_hash(key, strlen((const char*)key));
}

void db_free_obj(void *key, void **value) {
    assert(key);
    assert(value);
    obj_free(*value);
}

int db_init() {
    all_objs = table_new(65536, key_cmp, key_hash);
    return 0;
}

void db_destroy() {
    table_map(all_objs, db_free_obj);
    table_free(&all_objs);
}

void *db_put(obj_t *new_obj) {
    c_log(new_obj->name);
    printf("put obj : %p\n", new_obj);
    return table_put(all_objs, new_obj->name, new_obj);
}
