#ifndef OBJ_H
#define OBJ_H

typedef enum {
    STRING_T,
    LIST_T,
    HASH_T,
    SET_T
} obj_e;

typedef struct {
    char *name;
    obj_e type;
    void *obj;
} obj_t;

extern obj_t *obj_create(char *name, obj_e type);
extern void obj_free(obj_t *obj);

#endif
