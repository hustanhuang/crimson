#ifndef DB_H
#define DB_H

#include "table.h"
#include "obj.h"

extern table_t all_objs;

extern int db_init();

extern void db_destroy();

extern void *db_put(obj_t *new_obj);

#endif
