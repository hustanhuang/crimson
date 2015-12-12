#include "include/obj.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "include/utils.h"

#include "include/c_string.h"
#include "include/c_list.h"
#include "include/c_hash.h"
#include "include/c_set.h"

obj_t *obj_create(char *name, obj_e type) {
    obj_t *obj = malloc(sizeof(*obj));
    obj->name = calloc(sizeof(char), strlen(name) + 1);
    strcpy(obj->name, name);
    obj->type = type;
    switch (type) {
        case STRING_T:
            obj->obj = c_string_new();
            break;
        case LIST_T:
            obj->obj = NULL;
            break;
        case HASH_T:
            obj->obj = NULL;
            break;
        case SET_T:
            obj->obj = NULL;
            break;
        default:
            obj->obj = NULL;
            break;
    }
    return obj;
}

void obj_free(obj_t *obj) {
    if (obj == NULL)
        return;
    assert(obj->name);
    free(obj->name);
    assert(obj->obj);
    switch (obj->type) {
        case STRING_T:
            c_string_free(obj->obj);
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
    free(obj);
    return;
}
