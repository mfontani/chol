#include "charp_key.h"
#include "../../../tap/tap.h"
#include <assert.h>
#include <limits.h>

struct custom_foo *custom_foo_new(int a, int b)
{
    struct custom_foo *f = malloc(sizeof(struct custom_foo));
    assert(f != NULL);
    f->a = a;
    f->b = b;
    return f;
}
void custom_foo_free(struct custom_foo *f)
{
    assert(f != NULL);
    free(f);
}

// Hash the "char *" key using this fuction:
size_t custom_foo_hash(size_t size, const char *key)
{
    size_t hash = 0;
    for (const char *p = key; *p != '\0'; p++) {
        hash += *p;
    }
    return hash % size;
}

#define DYN_KVP_VALUE_TYPE struct custom_foo
#define DYN_KVP_TYPE_NAME cfoo_kvp
#define DYN_KVP_KEY_TYPE const char *
#define DYN_KVP_HASH_FUNCTION custom_foo_hash
#define DYN_KVP_IMPLEMENTATION
#include "../../dyn_kvp.h"
#undef DYN_KVP_IMPLEMENTATION
#undef DYN_KVP_HASH_FUNCTION
#undef DYN_KVP_KEY_TYPE
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE

void test_charp_foo(void)
{
    tap_diag("Running tests for test_charp_foo()");
    {
        struct cfoo_kvp *hash = cfoo_kvp_new(8);
        tap_is_int(cfoo_kvp_nkeys(hash), 0, "cfoo_kvp_nkeys() returns 0 after cfoo_kvp_new()");
        tap_is_int(cfoo_kvp_exists(hash, "foo"), 0, "cfoo_kvp_exists(foo) returns 0 after cfoo_kvp_new()");
        cfoo_kvp_del(hash, "foo");  // no-op
        tap_is_int(cfoo_kvp_exists(hash, "foo"), 0, "cfoo_kvp_exists(foo) returns 0 after cfoo_kvp_del()");
        tap_is_int(cfoo_kvp_nkeys(hash), 0, "cfoo_kvp_nkeys() returns 0 after _del()");
        cfoo_kvp_free(hash);
    }
    {
        struct cfoo_kvp *hash = cfoo_kvp_new(8);
        struct custom_foo *c1 = custom_foo_new(1, 2);
        cfoo_kvp_set(hash, "foo", c1);
        tap_is_int(cfoo_kvp_nkeys(hash), 1, "cfoo_kvp_nkeys() returns 1 after cfoo_kvp_set()");
        tap_is_int(cfoo_kvp_exists(hash, "foo"), 1, "cfoo_kvp_exists(foo) returns 1 after cfoo_kvp_set()");
        tap_is_int(cfoo_kvp_exists(hash, "bar"), 0, "cfoo_kvp_exists(bar) returns 0 after cfoo_kvp_set()");
        tap_is_voidp(cfoo_kvp_get(hash, "foo"), c1, "cfoo_kvp_get(foo) returns c1 after cfoo_kvp_set()");
        tap_is_voidp(cfoo_kvp_get(hash, "bar"), NULL, "cfoo_kvp_get(bar) returns NULL after cfoo_kvp_set()");
        cfoo_kvp_del(hash, "bar");  // no-op
        tap_is_int(cfoo_kvp_nkeys(hash), 1, "cfoo_kvp_nkeys() returns 1 after _del()");
        cfoo_kvp_set(hash, "bar", c1);
        tap_is_int(cfoo_kvp_nkeys(hash), 2, "cfoo_kvp_nkeys() returns 2 after cfoo_kvp_set()");
        tap_is_int(cfoo_kvp_exists(hash, "bar"), 1, "cfoo_kvp_exists(bar) returns 1 after cfoo_kvp_set()");
        cfoo_kvp_set(hash, "bar", NULL);
        tap_is_int(cfoo_kvp_nkeys(hash), 2, "cfoo_kvp_nkeys() returns 2 after cfoo_kvp_set(NULL)");
        tap_is_int(cfoo_kvp_exists(hash, "bar"), 1, "cfoo_kvp_exists(bar) returns 1 after cfoo_kvp_set(NULL)");
        cfoo_kvp_del(hash, "bar");
        tap_is_int(cfoo_kvp_nkeys(hash), 1, "cfoo_kvp_nkeys() returns 1 after _del()");
        tap_is_int(cfoo_kvp_exists(hash, "bar"), 0, "cfoo_kvp_exists(bar) returns 0 after _del()");
        cfoo_kvp_free(hash);
        custom_foo_free(c1);
    }
}
