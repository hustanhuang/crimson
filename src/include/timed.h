#ifndef TIMED_H
#define TIMED_H

#include "list.h"

typedef struct {
    unsigned int timeout;
    list_t all_events;
} event_t;

#define event_get(pos) (list_entry(pos, event_t, all_events))

extern int event_init();
extern int event_free();

#endif
