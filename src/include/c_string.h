#ifndef C_STRING_H
#define C_STRING_H

#include "sds.h"

struct c_string_t {
    sds content;
};

#define T c_string_t
typedef struct T *T;

extern T c_string_new();
extern void c_string_free();

#endif
