/*
 * `dyn_kvp.h` - C "dynamic" key/value "hash" library
 * Copyright (c) 2024 Marco Fontani - MFONTANI@cpan.org
 */

#ifndef DYN_KVP_VALUE_TYPE
# error "DYN_KVP_VALUE_TYPE must be defined before including dyn_kvp.h"
#endif

#ifndef DYN_KVP_TYPE_NAME
# error "DYN_KVP_TYPE_NAME must be defined before including dyn_kvp.h"
#endif

#ifdef DYN_KVP_HASH_FUNCTION_DEFINED
# error "dyn_kvp.h uses DYN_KVP_HASH_FUNCTION_DEFINED as a macro, but it's already defined"
#endif

#ifdef DYN_KVP_IMPLEMENTATION
# define DYN_KVP_EXTERN
# ifndef DYN_KVP_HASH_FUNCTION
#  ifndef DYN_KVP_KEY_TYPE
#   define DYN_KVP_HASH_FUNCTION(size, key) ((key) % (size))
#   define DYN_KVP_HASH_FUNCTION_DEFINED
#  else
#   error "DYN_KVP_HASH_FUNCTION must be defined before including dyn_kvp.h, if using a custom DYN_KVP_KEY_TYPE"
#  endif
# endif
#else
# define DYN_KVP_EXTERN extern
#endif

#ifdef DYN_KVP_KEY_TYPE_DEFINED
# error "dyn_kvp.h uses DYN_KVP_KEY_TYPE_DEFINED as a macro, but it's already defined"
#endif

#ifndef DYN_KVP_KEY_TYPE
# define DYN_KVP_KEY_TYPE unsigned long
# define DYN_KVP_KEY_TYPE_DEFINED
#endif

#ifdef DYN_KVP__
# error "dyn_kvp.h uses DYN_KVP__ as a macro, but it's already defined"
#endif

#define DYN_KVP__(x,y) x##y

#ifdef DYN_KVP_
# error "dyn_kvp.h uses DYN_KVP_ as a macro, but it's already defined"
#endif

#define DYN_KVP_(x,y) DYN_KVP__(x,y)

#ifdef DYN_KVP_F
# error "dyn_kvp.h uses DYN_KVP_F as a macro, but it's already defined"
#endif

#define DYN_KVP_F(x, y) DYN_KVP_(x, y)

#ifdef DYN_KVP_MEMBER_NAME
# error "dyn_kvp.h uses DYN_KVP_MEMBER_NAME as a macro, but it's already defined"
#endif
#define DYN_KVP_MEMBER_NAME DYN_KVP_F(DYN_KVP_TYPE_NAME, _kv)

#ifdef DYN_KVP_DEFINE_MEMBER_STRUCT
# error "dyn_kvp.h uses DYN_KVP_DEFINE_MEMBER_STRUCT as a macro, but it's already defined"
#endif
#ifndef DYN_KVP_IMPLEMENTATION
#define DYN_KVP_DEFINE_MEMBER_STRUCT(keytype, valuetype, structname) \
    struct structname { \
        keytype key; \
        valuetype *value; \
        struct structname *next; \
    }
DYN_KVP_DEFINE_MEMBER_STRUCT(DYN_KVP_KEY_TYPE, DYN_KVP_VALUE_TYPE, DYN_KVP_MEMBER_NAME);
#undef DYN_KVP_DEFINE_MEMBER_STRUCT
#endif

#ifdef DYN_KVP_IMPLEMENTATION

#ifdef DYN_KVP_DEFINE_STRUCT
# error "dyn_kvp.h uses DYN_KVP_DEFINE_STRUCT as a macro, but it's already defined"
#endif
#define DYN_KVP_DEFINE_STRUCT(member_structname, structname) \
    struct structname { \
        size_t size; \
        struct member_structname *hash[]; \
    }
DYN_KVP_DEFINE_STRUCT(DYN_KVP_MEMBER_NAME, DYN_KVP_TYPE_NAME);
#undef DYN_KVP_DEFINE_STRUCT

#else

struct DYN_KVP_TYPE_NAME;

#endif

DYN_KVP_EXTERN
struct DYN_KVP_MEMBER_NAME **DYN_KVP_F(DYN_KVP_MEMBER_NAME, _new)(struct DYN_KVP_TYPE_NAME *hash);
DYN_KVP_EXTERN
void DYN_KVP_F(DYN_KVP_MEMBER_NAME, _free)(struct DYN_KVP_MEMBER_NAME **hash);
DYN_KVP_EXTERN
struct DYN_KVP_TYPE_NAME *DYN_KVP_F(DYN_KVP_TYPE_NAME, _new)(size_t size);
DYN_KVP_EXTERN
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _free)(struct DYN_KVP_TYPE_NAME *hash);
DYN_KVP_EXTERN
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _set)(struct DYN_KVP_TYPE_NAME *hash, DYN_KVP_KEY_TYPE key, DYN_KVP_VALUE_TYPE *val);
DYN_KVP_EXTERN
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _del)(struct DYN_KVP_TYPE_NAME *hash, DYN_KVP_KEY_TYPE key);
DYN_KVP_EXTERN
DYN_KVP_VALUE_TYPE *DYN_KVP_F(DYN_KVP_TYPE_NAME, _get)(struct DYN_KVP_TYPE_NAME *hash, DYN_KVP_KEY_TYPE key);
DYN_KVP_EXTERN
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _stats_printf)(struct DYN_KVP_TYPE_NAME *hash);

#undef DYN_KVP_EXTERN

#ifdef DYN_KVP_IMPLEMENTATION

// Free an "exploded" NULL-terminated array from a KVP of this type:
void DYN_KVP_F(DYN_KVP_MEMBER_NAME, _free)(struct DYN_KVP_MEMBER_NAME **hash)
{
    if (!hash)
        return;
    for (int i = 0; hash[i] != NULL; i++)
        free(hash[i]);
    free(hash);
}

#ifndef DYN_KVP_REALLOC
# define DYN_KVP_REALLOC(result, type, number) \
    do { \
        type *tmp_realloc = realloc(result, sizeof(type) * (number)); \
        if (tmp_realloc == NULL) { \
            perror("realloc failure"); \
            fprintf(stderr, "Realloc failure at %s:%d\n", __FILE__, __LINE__); \
            abort(); \
        } \
        result = tmp_realloc; \
    } while (0)
#endif

// Create a NULL-terminated array from a KVP of this type.
// Remember to _free() it!
struct DYN_KVP_MEMBER_NAME **DYN_KVP_F(DYN_KVP_MEMBER_NAME, _new)(struct DYN_KVP_TYPE_NAME *hash)
{
    struct DYN_KVP_MEMBER_NAME **new_hash = NULL;
    int found = 0;
    DYN_KVP_REALLOC(new_hash, struct DYN_KVP_MEMBER_NAME *, found + 1);
    new_hash[found] = NULL;
    for (int i = 0; i < hash->size; i++)
        for (struct DYN_KVP_MEMBER_NAME *p = hash->hash[i]; p; p = p->next)
        {
            DYN_KVP_REALLOC(new_hash[found], struct DYN_KVP_MEMBER_NAME, 1);
            new_hash[found]->key = p->key;
            new_hash[found]->value = p->value;
            new_hash[found]->next = NULL;
            found++;
            DYN_KVP_REALLOC(new_hash, struct DYN_KVP_MEMBER_NAME *, found + 1);
            new_hash[found] = NULL;
        }
    return new_hash;
}

// Create a new KVP of this type, hashed over "size" slots.
// Remember to _free() it!
struct DYN_KVP_TYPE_NAME *DYN_KVP_F(DYN_KVP_TYPE_NAME, _new)(size_t size)
{
    struct DYN_KVP_TYPE_NAME *hash = NULL;
    hash = malloc(sizeof(struct DYN_KVP_TYPE_NAME) + size * sizeof(void *));
    if (!hash)
    {
        perror("malloc failure");
        fprintf(stderr, "Malloc failure at %s:%d\n", __FILE__, __LINE__);
        abort();
    }
    hash->size = size;
    for (int i = 0; i < size; i++)
        hash->hash[i] = NULL;
    return hash;
}

// Free a KVP of this type.
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _free)(struct DYN_KVP_TYPE_NAME *hash)
{
    if (!hash)
        return;
    for (int i = 0; i < hash->size; i++)
    {
        struct DYN_KVP_MEMBER_NAME *pnext = NULL;
        for (struct DYN_KVP_MEMBER_NAME *p = hash->hash[i]; p; p = pnext)
        {
            pnext = p->next;
            free(p);
        }
    }
    free(hash);
}

// Set a "key" to "val" in a KVP of this type.
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _set)(struct DYN_KVP_TYPE_NAME *hash, DYN_KVP_KEY_TYPE key, DYN_KVP_VALUE_TYPE *val)
{
    if (!hash)
        return;
    unsigned int hash_index = DYN_KVP_HASH_FUNCTION(hash->size, key);
    for (struct DYN_KVP_MEMBER_NAME *p = hash->hash[hash_index]; p; p = p->next)
        if (p->key == key)
        {
            p->value = val;
            return;
        }
    struct DYN_KVP_MEMBER_NAME *p = malloc(sizeof(struct DYN_KVP_MEMBER_NAME));
    if (!p)
    {
        perror("malloc");
        abort();
    }
    p->key = key;
    p->value = val;
    p->next = hash->hash[hash_index];
    hash->hash[hash_index] = p;
    return;
}

// Delete a "key", if found, from a KVP of this type.
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _del)(struct DYN_KVP_TYPE_NAME *hash, DYN_KVP_KEY_TYPE key)
{
    if (!hash)
        return;
    unsigned int hash_index = DYN_KVP_HASH_FUNCTION(hash->size, key);
    struct DYN_KVP_MEMBER_NAME *prev = NULL;
    for (struct DYN_KVP_MEMBER_NAME *p = hash->hash[hash_index]; p; p = p->next)
    {
        if (p->key == key)
        {
            if (prev)
                prev->next = p->next;
            else
                hash->hash[hash_index] = p->next;
            free(p);
            return;
        }
        prev = p;
    }
}

// Get a "key", if found, from a KVP of this type.
// Returns NULL if not found.
DYN_KVP_VALUE_TYPE *DYN_KVP_F(DYN_KVP_TYPE_NAME, _get)(struct DYN_KVP_TYPE_NAME *hash, DYN_KVP_KEY_TYPE key)
{
    if (!hash)
        return NULL;
    unsigned int hash_index = DYN_KVP_HASH_FUNCTION(hash->size, key);
    for (struct DYN_KVP_MEMBER_NAME *p = hash->hash[hash_index]; p; p = p->next)
        if (p->key == key)
            return p->value;
    return NULL;
}

#ifdef DYN_KVP_HASH_FUNCTION_DEFINED
# undef DYN_KVP_HASH_FUNCTION_DEFINED
# undef DYN_KVP_HASH_FUNCTION
#endif

// Print stats to STOUT for a KVP of this type.
void DYN_KVP_F(DYN_KVP_TYPE_NAME, _stats_printf)(struct DYN_KVP_TYPE_NAME *hash)
{
    if (!hash)
    {
        printf("No hash passed!\n");
        return;
    }
    size_t bytes_used = sizeof(struct DYN_KVP_TYPE_NAME) + hash->size * sizeof(void *);
    size_t total_bytes_used = 0UL;
    unsigned long nempty = 0UL;
    unsigned long nused = 0UL;
    unsigned long ninchain = 0UL;
    unsigned long ninchainmin = ULONG_MAX;
    unsigned long ninchainmax = 0UL;
    int foundMin = 0;
    for (int i = 0; i < hash->size; i++)
    {
        if (hash->hash[i] == NULL)
        {
            nempty++;
            continue;
        }
        nused++;
        unsigned long inchain = 0UL;
        for (struct DYN_KVP_MEMBER_NAME *p = hash->hash[i]; p; p = p->next)
        {
            inchain++;
            bytes_used += sizeof(struct DYN_KVP_MEMBER_NAME);
            total_bytes_used += sizeof(DYN_KVP_VALUE_TYPE);
#ifndef DYN_KVP_KEY_TYPE_DEFINED
            total_bytes_used += sizeof(DYN_KVP_KEY_TYPE);
#endif
        }
        ninchain += inchain;
        if (inchain > ninchainmax)
            ninchainmax = inchain;
        if (inchain < ninchainmin)
        {
            ninchainmin = inchain;
            foundMin = 1;
        }
    }
    if (!foundMin)
        ninchainmin = ninchainmax;
    printf("%s(%p): %zu[+%zu=%zu] bytes used, %3lu/%3lu (%6.2f%%) slots used. Min %lu, Max %lu, Avg %.2f.\r\n",
        __func__, hash, bytes_used, total_bytes_used, bytes_used + total_bytes_used,
        nused, hash->size, nused * 100.0 / hash->size,
        ninchainmin, ninchainmax, nused ? 1.0 * ninchain / nused * 1.0 : 0.0);
}

#undef DYN_KVP_MEMBER_NAME
#endif

#undef DYN_KVP_F
#undef DYN_KVP_
#undef DYN_KVP__
#ifdef DYN_KVP_KEY_TYPE_DEFINED
# undef DYN_KVP_KEY_TYPE_DEFINED
# undef DYN_KVP_KEY_TYPE
#endif
#undef DYN_KVP_MEMBER_NAME
