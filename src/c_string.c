#include "include/c_string.h"

#include "include/parse.h"

#define T c_string_t

T c_string_new() {
    T new_p = malloc(sizeof(*new_p));
    new_p->content = sdsnnew(NULL, 0);
    return new_p;
}

void c_string_free(T p) {
    sdsfree(p->content);
    free(p);
}

void c_string_set(T p, const char *s) {
    sdsfree(sdscpy(p->content, s));
}

#undef T
