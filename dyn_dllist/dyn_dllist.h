/*
 * `dyn_dllist.h` - C "dynamic" doubly-linked-list header-only library
 * Copyright (c) 2024 Marco Fontani - MFONTANI@cpan.org
 */

#ifndef DYN_DLLIST_VALUE_TYPE
#error "DYN_DLLIST_VALUE_TYPE must be defined before including dyn_dllist.h"
#endif

#ifndef DYN_DLLIST_TYPE_NAME
#error "DYN_DLLIST_TYPE_NAME must be defined before including dyn_dllist.h"
#endif

#ifdef DYN_DLLIST_IMPLEMENTATION
# define DYN_DLLIST_EXTERN
#else
# define DYN_DLLIST_EXTERN extern
#endif

#ifdef DYN_DLLIST__
#error "dyn_dllist.h uses DYN_DLLIST__ as a macro, but it's already defined"
#endif

#define DYN_DLLIST__(x,y) x##y

#ifdef DYN_DLLIST_
#error "dyn_dllist.h uses DYN_DLLIST_ as a macro, but it's already defined"
#endif

#define DYN_DLLIST_(x,y) DYN_DLLIST__(x,y)

#ifdef DYN_DLLIST_F
#error "dyn_dllist.h uses DYN_DLLIST_F as a macro, but it's already defined"
#endif

#define DYN_DLLIST_F(x, y) DYN_DLLIST_(x, y)

#ifdef DYN_DLLIST_IMPLEMENTATION

#ifdef DYN_DLLIST_MEMBER_NAME
# error "dyn_dllist.h uses DYN_DLLIST_MEMBER_NAME as a macro, but it's already defined"
#endif
#define DYN_DLLIST_MEMBER_NAME DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, __member)
struct DYN_DLLIST_MEMBER_NAME {
    DYN_DLLIST_VALUE_TYPE *p;
    struct DYN_DLLIST_MEMBER_NAME *next;
    struct DYN_DLLIST_MEMBER_NAME *prev;
};

struct DYN_DLLIST_TYPE_NAME {
    struct DYN_DLLIST_MEMBER_NAME *first;
    struct DYN_DLLIST_MEMBER_NAME *last;
    size_t size;
};

#else

struct DYN_DLLIST_TYPE_NAME;

#endif

DYN_DLLIST_EXTERN
struct DYN_DLLIST_TYPE_NAME *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _new)(void);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _free)(struct DYN_DLLIST_TYPE_NAME *list);
DYN_DLLIST_EXTERN
size_t DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _size)(struct DYN_DLLIST_TYPE_NAME *list);
DYN_DLLIST_EXTERN
int DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
int DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
int DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append1_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append1_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend1_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend1_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _remove_all)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _removenth_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, size_t n);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _removenth_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, size_t n);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _shift)(struct DYN_DLLIST_TYPE_NAME *list);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _pop)(struct DYN_DLLIST_TYPE_NAME *list);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _iterate_ftl)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param);
DYN_DLLIST_EXTERN
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _iterate_ltf)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param);
DYN_DLLIST_EXTERN
struct DYN_DLLIST_TYPE_NAME *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _grep)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param);
DYN_DLLIST_EXTERN
size_t DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _countif)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _nth_ftl)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param, size_t n);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _nth_ltf)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param, size_t n);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _first)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _last)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _peek_first)(struct DYN_DLLIST_TYPE_NAME *list);
DYN_DLLIST_EXTERN
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _peek_last)(struct DYN_DLLIST_TYPE_NAME *list);

#undef DYN_DLLIST_EXTERN

#ifdef DYN_DLLIST_IMPLEMENTATION

// Create a new doubly-linked list of this type. Remember to _free() it!
struct DYN_DLLIST_TYPE_NAME *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _new)(void)
{
    struct DYN_DLLIST_TYPE_NAME *list = malloc(sizeof(struct DYN_DLLIST_TYPE_NAME));
    if (!list)
    {
        perror("_new: malloc");
        exit(EXIT_FAILURE);
    }
    list->first = NULL;
    list->last = NULL;
    list->size = 0;
    return list;
}

// Free a doubly-linked list of this type, including inner members but NOT
// including the values in them. That's up to the caller!
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _free)(struct DYN_DLLIST_TYPE_NAME *list)
{
    if (!list)
        return;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *next = cur->next;
        free(cur);
        cur = next;
    }
    free(list);
}

// Returns the number of elements in the list. Doesn't need to iterate it.
size_t DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _size)(struct DYN_DLLIST_TYPE_NAME *list)
{
    if (!list)
        return 0;
    return list->size;
}

// Returns non-zero if val is in the list. Iterates first to last.
int DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list || !list->size)
        return 0;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    while (cur)
    {
        if (cur->p == val)
            return 1;
        cur = cur->next;
    }
    return 0;
}

// Returns non-zero if val is in the list. Iterates last to first.
int DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list || !list->size)
        return 0;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->last;
    while (cur)
    {
        if (cur->p == val)
            return 1;
        cur = cur->prev;
    }
    return 0;
}

// Returns non-zero if val is in the list. Iterates first to last.
int DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list || !list->size)
        return 0;
    return DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ftl)(list, val);
}

// Appends val at the end of the list. Might already be in the list!
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list)
        return;
    struct DYN_DLLIST_MEMBER_NAME *new = malloc(sizeof(struct DYN_DLLIST_MEMBER_NAME));
    if (!new)
    {
        perror("_append: malloc");
        exit(EXIT_FAILURE);
    }
    new->p = val;
    new->next = NULL;
    new->prev = NULL;
    list->size++;
    if (!list->first)
    {
        list->first = new;
        list->last = new;
        return;
    }
    list->last->next = new;
    new->prev = list->last;
    list->last = new;
}

// Appends val at the end of the list, but only if it's not already in it.
// Uses contains_ftl() to check its presence.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append1_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list)
        return;
    if (DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ftl)(list, val))
        return;
    DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append)(list, val);
}

// Appends val at the end of the list, but only if it's not already in it.
// Uses contains_ltf() to check its presence.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append1_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list)
        return;
    if (DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ltf)(list, val))
        return;
    DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append)(list, val);
}

// Prepends val at the start of the list. Might already be in the list!
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list)
        return;
    struct DYN_DLLIST_MEMBER_NAME *new = malloc(sizeof(struct DYN_DLLIST_MEMBER_NAME));
    if (!new)
    {
        perror("_append: malloc");
        exit(EXIT_FAILURE);
    }
    new->p = val;
    new->next = NULL;
    new->prev = NULL;
    list->size++;
    if (!list->first)
    {
        list->first = new;
        list->last = new;
        return;
    }
    list->first->prev = new;
    new->next = list->first;
    list->first = new;
}

// Prepends val at the start of the list, but only if it's not already in it.
// Uses contains_ftl() to check its presence.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend1_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list)
        return;
    if (DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ftl)(list, val))
        return;
    DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend)(list, val);
}

// Prepends val at the start of the list, but only if it's not already in it.
// Uses contains_ltf() to check its presence.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend1_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list)
        return;
    if (DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _contains_ltf)(list, val))
        return;
    DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _prepend)(list, val);
}

// Removes all instances of val from the list.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _remove_all)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val)
{
    if (!list || !list->size)
        return;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *cur_next = cur->next;
        if (cur->p == val)
        {
            if (cur->prev)
                cur->prev->next = cur->next;
            else
                list->first = cur->next;
            if (cur->next)
                cur->next->prev = cur->prev;
            else
                list->last = cur->prev;
            free(cur);
            list->size--;
        }
        cur = cur_next;
    }
}

// Removes the nth instance of val from the list, iterating it first to last.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _removenth_ftl)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, size_t n)
{
    if (!list || !list->size || n >= list->size)
        return;
    size_t count = 0;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    while (cur)
    {
        if (cur->p == val)
        {
            if (count == n)
            {
                if (cur->prev)
                    cur->prev->next = cur->next;
                else
                    list->first = cur->next;
                if (cur->next)
                    cur->next->prev = cur->prev;
                else
                    list->last = cur->prev;
                free(cur);
                list->size--;
                return;
            }
            count++;
        }
        cur = cur->next;
    }
}

// Removes the nth instance of val from the list, iterating it last to first
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _removenth_ltf)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, size_t n)
{
    if (!list || !list->size || n >= list->size)
        return;
    size_t count = 0;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->last;
    while (cur)
    {
        if (cur->p == val)
        {
            if (count == n)
            {
                if (cur->prev)
                    cur->prev->next = cur->next;
                else
                    list->first = cur->next;
                if (cur->next)
                    cur->next->prev = cur->prev;
                else
                    list->last = cur->prev;
                free(cur);
                list->size--;
                return;
            }
            count++;
        }
        cur = cur->prev;
    }
}

// Removes, returning it, the FIRST element of the list.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _shift)(struct DYN_DLLIST_TYPE_NAME *list)
{
    if (!list || !list->size)
        return NULL;
    struct DYN_DLLIST_MEMBER_NAME *ret = list->first;
    if (list->first)
    {
        list->first = list->first->next;
        if (list->first)
            list->first->prev = NULL;
        else
            list->last = NULL;
    }
    if (ret)
    {
        DYN_DLLIST_VALUE_TYPE *val = ret->p;
        free(ret);
        list->size--;
        return val;
    }
    return NULL;
}

// Removes, returning it, the LAST element of the list.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _pop)(struct DYN_DLLIST_TYPE_NAME *list)
{
    if (!list || !list->size)
        return NULL;
    struct DYN_DLLIST_MEMBER_NAME *ret = list->last;
    if (list->last)
    {
        list->last = list->last->prev;
        if (list->last)
            list->last->next = NULL;
        else
            list->first = NULL;
    }
    if (ret)
    {
        DYN_DLLIST_VALUE_TYPE *val = ret->p;
        free(ret);
        list->size--;
        return val;
    }
    return NULL;
}

// Iterate the list from first to last, calling func(list, val, param) for each val in the list.
// It's SAFE to remove "val" from "list".
// If func returns non-zero, the iteration is stopped.
// "param" is passed to func as-is.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _iterate_ftl)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param)
{
    if (!list || !func || !list->size)
        return;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *cur_next = cur->next;
        if (func(list, cur->p, param))
            return;
        cur = cur_next;
    }
}

// Iterate the list from last to first, calling func(list, val, param) for each val in the list.
// It's SAFE to remove "val" from "list".
// If func returns non-zero, the iteration is stopped.
// "param" is passed to func as-is.
void DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _iterate_ltf)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param)
{
    if (!list || !func || !list->size)
        return;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->last;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *cur_prev = cur->prev;
        if (func(list, cur->p, param))
            return;
        cur = cur_prev;
    }
}


// Returns a new list containing all the elements of "list" for which the
// function "func" returns non-zero.
// "param" is passed to func as-is.
struct DYN_DLLIST_TYPE_NAME *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _grep)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param)
{
    struct DYN_DLLIST_TYPE_NAME *new_list = DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _new)();
    if (!list || !func || !list->size)
        return new_list;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *cur_next = cur->next;
        DYN_DLLIST_VALUE_TYPE *elem = cur->p;
        if (func(list, elem, param))
            DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _append)(new_list, elem);
        cur = cur_next;
    }
    return new_list;
}

// Returns the number of elements of "list" for which the function "func"
// returns non-zero.
// "param" is passed to func as-is.
size_t DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _countif)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param)
{
    size_t count = 0;
    if (!list || !func || !list->size)
        return count;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *cur_next = cur->next;
        if (func(list, cur->p, param))
            count++;
        cur = cur_next;
    }
    return count;
}

// Returns the "n"th element of "list", iterated first to last, for which the
// function "func" returns non-zero.
// "param" is passed to func as-is.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _nth_ftl)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param, size_t n)
{
    if (!list || !func || !list->size || n >= list->size)
        return NULL;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->first;
    size_t count = 0;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *cur_next = cur->next;
        DYN_DLLIST_VALUE_TYPE *elem = cur->p;
        if (func(list, elem, param))
        {
            if (count == n)
                return elem;
            count++;
        }
        cur = cur_next;
    }
    return NULL;
}

// Returns the "n"th element of "list", iterated last to first, for which the
// function "func" returns non-zero.
// "param" is passed to func as-is.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _nth_ltf)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param, size_t n)
{
    if (!list || !func || !list->size || n >= list->size)
        return NULL;
    struct DYN_DLLIST_MEMBER_NAME *cur = list->last;
    size_t count = 0;
    while (cur)
    {
        struct DYN_DLLIST_MEMBER_NAME *cur_prev = cur->prev;
        DYN_DLLIST_VALUE_TYPE *elem = cur->p;
        if (func(list, elem, param))
        {
            if (count == n)
                return elem;
            count++;
        }
        cur = cur_prev;
    }
    return NULL;
}

// Returns the first element of "list" (iterated first to last) for which the
// function "func" returns non-zero.
// "param" is passed to func as-is.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _first)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param)
{
    if (!list || !func || !list->size)
        return NULL;
    return DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _nth_ftl)(list, func, param, 0);
}

// Returns the first element of "list" (iterated last to first) for which the
// function "func" returns non-zero.
// "param" is passed to func as-is.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _last)(struct DYN_DLLIST_TYPE_NAME *list, int (*func)(struct DYN_DLLIST_TYPE_NAME *list, DYN_DLLIST_VALUE_TYPE *val, void *param), void *param)
{
    if (!list || !func || !list->size)
        return NULL;
    return DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _nth_ltf)(list, func, param, 0);
}

// Returns the "first" element of "list".
// Does not remove it from the list.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _peek_first)(struct DYN_DLLIST_TYPE_NAME *list)
{
    if (!list || !list->size || !list->first)
        return NULL;
    return list->first->p;
}

// Returns the "last" element of "list".
// Does not remove it from the list.
DYN_DLLIST_VALUE_TYPE *DYN_DLLIST_F(DYN_DLLIST_TYPE_NAME, _peek_last)(struct DYN_DLLIST_TYPE_NAME *list)
{
    if (!list || !list->size || !list->last)
        return NULL;
    return list->last->p;
}

#undef DYN_DLLIST_MEMBER_NAME
#endif

#undef DYN_DLLIST_F
#undef DYN_DLLIST_
#undef DYN_DLLIST__
