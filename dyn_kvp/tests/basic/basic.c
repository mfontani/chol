#include "basic.h"
#include "../../../tap/tap.h"
#include <assert.h>
#include <limits.h>

struct basic_foo *basic_foo_new(int a, int b)
{
    struct basic_foo *f = malloc(sizeof(struct basic_foo));
    assert(f != NULL);
    f->a = a;
    f->b = b;
    return f;
}
void basic_foo_free(struct basic_foo *f)
{
    assert(f != NULL);
    free(f);
}

#define DYN_KVP_VALUE_TYPE struct basic_foo
#define DYN_KVP_TYPE_NAME bfoo_kvp
#define DYN_KVP_IMPLEMENTATION
#include "../../dyn_kvp.h"
#undef DYN_KVP_IMPLEMENTATION
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE

void test_basic_foo(void)
{
    tap_diag("Running tests for test_basic_foo()");
    {
        struct bfoo_kvp *hash = bfoo_kvp_new(8);
        tap_is_int(bfoo_kvp_nkeys(hash), 0, "bfoo_kvp_nkeys() returns 0 after bfoo_kvp_new()");
        bfoo_kvp_del(hash, 1);  // no-op
        tap_is_int(bfoo_kvp_nkeys(hash), 0, "bfoo_kvp_nkeys() returns 0 after _del()");
        bfoo_kvp_free(hash);
    }
    {
        struct bfoo_kvp *hash = bfoo_kvp_new(8);
        struct basic_foo *f1 = basic_foo_new(1, 2);
        bfoo_kvp_set(hash, 1, f1);
        tap_is_int(bfoo_kvp_nkeys(hash), 1, "bfoo_kvp_nkeys() returns 1 after bfoo_kvp_set()");
        tap_is_voidp(bfoo_kvp_get(hash, 1), f1,
            "bfoo_kvp_get(1) returns the same pointer that was passed to bfoo_kvp_set()");
        bfoo_kvp_del(hash, 1);
        tap_is_int(bfoo_kvp_nkeys(hash), 0, "bfoo_kvp_nkeys() returns 0 after bfoo_kvp_del()");
        tap_is_voidp(bfoo_kvp_get(hash, 1), NULL,
            "bfoo_kvp_get(1) returns NULL after bfoo_kvp_del(1)");
        bfoo_kvp_del(hash, 1); // no-op
        tap_is_int(bfoo_kvp_nkeys(hash), 0, "bfoo_kvp_nkeys() returns 0 after second bfoo_kvp_del()");
        tap_is_voidp(bfoo_kvp_get(hash, 1), NULL,
            "bfoo_kvp_get(1) returns NULL after second bfoo_kvp_del(1)");
        bfoo_kvp_free(hash);
        basic_foo_free(f1);
    }
    struct basic_foo *f1 = basic_foo_new(1, 2);
    struct basic_foo *f2 = basic_foo_new(3, 4);
    struct basic_foo *f3 = basic_foo_new(5, 6);
    struct bfoo_kvp *hash = bfoo_kvp_new(8);
    bfoo_kvp_set(hash, 1, f1);
    bfoo_kvp_set(hash, 3, f2);
    bfoo_kvp_set(hash, 5, f3);
    tap_is_int(bfoo_kvp_nkeys(hash), 3, "bfoo_kvp_nkeys() returns 3 after 3 bfoo_kvp_set()s");
    tap_is_voidp(bfoo_kvp_get(hash, 1), f1,
        "bfoo_kvp_get(1) returns the same pointer that was passed to bfoo_kvp_set()");
    tap_is_voidp(bfoo_kvp_get(hash, 3), f2,
        "bfoo_kvp_get(3) returns the same pointer that was passed to bfoo_kvp_set()");
    tap_is_voidp(bfoo_kvp_get(hash, 5), f3,
        "bfoo_kvp_get(5) returns the same pointer that was passed to bfoo_kvp_set()");
    {
        tap_is_int(bfoo_kvp_nkeys(hash), 3, "bfoo_kvp_nkeys() returns 3 after 3 bfoo_kvp_set()s");
        assert(bfoo_kvp_nkeys(hash) == 3);
        struct bfoo_kvp_kv **exploded = bfoo_kvp_kv_new(hash);
        assert(exploded != NULL);
        assert(exploded[0] != NULL);
        // The order of these depends on the hash used!
        tap_is_int(exploded[0]->key, 1, "exploded[0]->key == 1");
        tap_is_voidp(exploded[0]->value, f1, "exploded[0]->value == f1");
        assert(exploded[1] != NULL);
        tap_is_int(exploded[1]->key, 3, "exploded[1]->key == 3");
        tap_is_voidp(exploded[1]->value, f2, "exploded[1]->value == f2");
        assert(exploded[2] != NULL);
        tap_is_int(exploded[2]->key, 5, "exploded[2]->key == 5");
        tap_is_voidp(exploded[2]->value, f3, "exploded[2]->value == f3");
        tap_is_voidp(exploded[3], NULL, "exploded[3] == NULL");
        bfoo_kvp_kv_free(exploded);
    }
    bfoo_kvp_free(hash);
    {
        hash = bfoo_kvp_new(1);
        bfoo_kvp_set(hash, 1, f1);
        bfoo_kvp_set(hash, 2, f2);
        bfoo_kvp_set(hash, 3, f3);
        struct bfoo_kvp_kv **exploded = bfoo_kvp_kv_new(hash);
        assert(exploded != NULL);
        assert(exploded[0] != NULL);
        // All have the same slot in the hash, so they get prepended.
        tap_is_int(exploded[0]->key, 3, "exploded[0]->key == 3");
        tap_is_voidp(exploded[0]->value, f3, "exploded[0]->value == f3");
        assert(exploded[1] != NULL);
        tap_is_int(exploded[1]->key, 2, "exploded[1]->key == 2");
        tap_is_voidp(exploded[1]->value, f2, "exploded[1]->value == f2");
        assert(exploded[2] != NULL);
        tap_is_int(exploded[2]->key, 1, "exploded[2]->key == 1");
        tap_is_voidp(exploded[2]->value, f1, "exploded[2]->value == f1");
        tap_is_voidp(exploded[3], NULL, "exploded[3] == NULL");
        bfoo_kvp_kv_free(exploded);
    }
    bfoo_kvp_free(hash);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
