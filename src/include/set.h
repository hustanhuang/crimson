#ifndef SET_H
#define SET_H

struct set_t {
    int size;
    int (*cmp)(const void *x, const void *y);
    unsigned int (*hash)(const void *member);
    int length;
    unsigned int timestamp;

    struct member {
        struct member *link;
        void *member;
    } **buckets;
};

#define T set_t
typedef struct T *T;

extern T        set_new     (int hint, int cmp(const void *x, const void *y), unsigned int hash(const void *member));
extern void     set_free    (T *set);
extern T        set_copy    (T t, int hint);

extern int      set_length  (T set);

extern void     set_put     (T set, void *member);
extern int      set_member  (T set, void *member);
extern struct member    *set_remove (T set, void *member);

extern void     set_map   (T set, void apply(void *member));

extern T        set_union   (T s, T t);
extern T        set_inter   (T s, T t);
extern T        set_minus   (T s, T t);
extern T        set_diff    (T s, T t);

#undef T

static int cmpatom(const void *x, const void *y) {
    return x != y;
}

static unsigned int hashatom(const void *member) {
    return (unsigned long)member >> 2;
}

#endif
