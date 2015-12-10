#include "include/timed.h"

#include <stdlib.h>

#include "include/list.h"

list_t timed_events;

int event_init() {
    C_INIT_LIST_HEAD(&timed_events);
    return 0;
}

int event_free() {
    list_t *pos, *p;
    list_for_each_safe(pos, p, &timed_events) {
        free(event_get(pos));
    }
    return 0;
}
