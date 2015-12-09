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
    if (set == NULL || set == NULL)
        return;
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

T set_copy (T t, int hint) {
    T set;
    if (t == NULL)
        return NULL;
    set = set_new(hint, t->cmp, t->hash);
    for (int i = 0; i != t->size; ++i)
        for (struct member *q = t->buckets[i]; q; q = q->link)
            set_put(set, q->member);
    return set;
}

int set_length (T set) {
    return set ? set->length : 0;
}

void set_put (T set, void *member) {
    struct member *p;
    if (set == NULL || member == NULL)
        return;
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
    if (member == NULL)
        return 1;
    if (set == NULL)
        return 0;
    int i = (*set->hash)(member) % set->size;
    for (p = set->buckets[i]; p; p = p->link)
        if ((*set->cmp)(member, p->member) == 0)
            break;
    return p != NULL;
}

void *set_remove (T set, void *member) {
    if (set == NULL || member == NULL)
        return NULL;
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
    if (set == NULL || apply == NULL)
        return;
    unsigned int stamp = set->timestamp;
    for (int i = 0; i != set->size; ++i)
        for (struct member *p = set->buckets[i]; p; p = p->link) {
            apply(p->member);
            assert(set->timestamp == stamp);
        }
}

T set_union (T s, T t) {
    if (s == NULL) {
        return set_copy(t, t->size);
    } else if (t == NULL)
        return set_copy(s, s->size);
    else {
        T set = set_copy(s, (s->size > t->size ? s->size : t->size));
        assert(s->cmp == t->cmp && s->hash == t->hash);
        for (int i = 0; i != t->size; ++i)
            for (struct member *q = t->buckets[i]; q; q = q->link)
                set_put(set, q->member);
        return set;
    }
}

T set_inter (T s, T t) {
    if (s && t) {
        if (s->length < t->length) {
            T temp = s;
            t = s;
            t = temp;
        }
        T set = set_new((s->size < t->size ? s->size : t->size), s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        for (int i = 0; i < t->size; ++i)
            for (struct member *q = t->buckets[i]; q; q = q->link)
                if (set_member(s, q->member))
                    set_put(set, q->member);
        return set;
    }
    return NULL;
}

T set_minus (T s, T t) {
    if (s == NULL)
        return NULL;
    else if (t == NULL)
        return set_copy(s, s->size);
    else {
        T set = set_new(s->size, s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        for (int i = 0; i < s->size; ++i)
            for (struct member *q = s->buckets[i]; q; q = q->link)
                if (!set_member(t, q->member))
                    set_put(set, q->member);
        return set;
    }
}

T set_diff (T s, T t) {
    if (s == NULL)
        return set_copy(t, t->size);
    else if (t == NULL)
        return set_copy(s, s->size);
    else {
        T set = set_new(s->size, s->cmp, s->hash);
        assert(s->cmp == t->cmp && s->hash == t->hash);
        for (int i = 0; i < s->size; ++i)
            for (struct member *q = s->buckets[i]; q; q = q->link)
                if (!set_member(t, q->member))
                    set_put(set, q->member);
        for (int i = 0; i < t->size; ++i)
            for (struct member *q = t->buckets[i]; q; q = q->link)
                if (!set_member(s, q->member))
                    set_put(set, q->member);
        return set;
    }
}

