#ifndef MEM_H
#define MEM_H

#include "list.h"

typedef struct {
    void *mem;
    list_t all_mems;
} mem_t;

#define mem_get(pos) (list_entry(pos, mem_t, all_mems))

extern int mem_init();
extern int mem_free();

#endif
