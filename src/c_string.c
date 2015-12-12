#include "include/c_string.h"

#include "include/parse.h"

#define T c_string_t

T c_string_new() {
    struct c_string_t *new_p = malloc(sizeof(*new_p));
    new_p->content = sdsnnew(NULL, 0);
    return new_p;
}

void c_string_free(T *p) {
    sdsfree((*p)->content);
    free(*p);
}
