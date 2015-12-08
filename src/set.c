#include "include/set.h"

#include <limits.h>
#include <assert.h>
#include <stdlib.h>

#define T set_t

T set_new (int hint, int cmp(const void *x, const void *y), unsigned int hash(const void *member)) {
    T set;
    static const int primes[] =
    { 509, 509, 1021, 3053, 4093, 8191, 16381, 32771, 65521, INT_MAX };
    assert(hint >= 0);
    int i = 1;
    while (primes[i] < hint)
        ++i;
    set = malloc(sizeof(*set) + primes[i-1] * sizeof(set->buckets[0]));
    set->size = primes[i-1];
    set->cmp = cmp ? cmp : cmpatom;
    set->hash = hash ? hash : hashatom;
    set->buckets = (struct member **)(set + 1);
    for (i = 0; i != set->size; ++i)
        set->buckets[i] = NULL;
    set->length = 0;
    set->timestamp = 0;
    return set;
}

void set_free (T *set) {
    assert(set && *set);
    if ((*set)->length > 0) {
        struct member *p, *q;
        for (int i = 0; i < (*set)->size; ++i)
            for (p = (*set)->buckets[i]; p; p = q) {
                q = p->link;
                free(p);
            }
    }
    free(*set);
}

int set_length (T set) {
    assert(set);
    return set->length;
}

void set_put (T set, void *member) {
    struct member *p;
    assert(set);
    assert(member);
    int i = (*set->hash)(member) % set->size;
    for (p = set->buckets[i]; p; p = p->link)
        if ((*set->cmp)(member, p->member) == 0)
            break;
    if (p == NULL) {
        p = malloc(sizeof(*p));
        p->member = member;
        p->link = set->buckets[i];
        set->buckets[i] = p;
        ++(set->length);
    } else
        p->member = member;
    ++(set->timestamp);
}

int set_member (T set, void *member) {
    struct member *p;
    assert(set);
    assert(member);
    int i = (*set->hash)(member) % set->size;
    for (p = set->buckets[i]; p; p = p->link)
        if ((*set->cmp)(member, p->member) == 0)
            break;
    return p != NULL;
}

struct member *set_remove (T set, void *member) {
    assert(set);
    assert(member);
    ++(set->timestamp);
    int i = (*set->hash)(member) % set->size;
    for (struct member **pp = &set->buckets[i]; *pp; pp = &(*pp)->link)
        if ((*set->cmp)(member, (*pp)->member) == 0) {
            struct member *p = *pp;
            *pp = p->link;
            member = p->member;
            free(p);
            --(set->length);
            return (void *)member;
        }
    return NULL;
}

void set_map (T set, void apply(void *member)) {
    assert(set);
    assert(apply);
    unsigned int stamp = set->timestamp;
    for (int i = 0; i != set->size; ++i)
        for (struct member *p = set->buckets[i]; p; p = p->link) {
            apply(p->member);
            assert(set->timestamp == stamp);
        }
}

T set_union (T s, T t);

T set_inter (T s, T t);

T set_minus (T s, T t);

T set_diff (T s, T t);
