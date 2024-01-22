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
        tap_is_int(bfoo_kvp_exists(hash, 1), 0, "bfoo_kvp_exists(1) returns 0 after bfoo_kvp_new()");
        bfoo_kvp_del(hash, 1);  // no-op
        tap_is_int(bfoo_kvp_exists(hash, 1), 0, "bfoo_kvp_exists(1) returns 0 after bfoo_kvp_del()");
        tap_is_int(bfoo_kvp_nkeys(hash), 0, "bfoo_kvp_nkeys() returns 0 after _del()");
        bfoo_kvp_free(hash);
    }
    {
        struct bfoo_kvp *hash = bfoo_kvp_new(8);
        struct basic_foo *f1 = basic_foo_new(1, 2);
        tap_is_int(bfoo_kvp_exists(hash, 1), 0, "bfoo_kvp_exists(1) returns 0 after bfoo_kvp_new()");
        bfoo_kvp_set(hash, 1, f1);
        tap_is_int(bfoo_kvp_exists(hash, 1), 1, "bfoo_kvp_exists(1) returns 1 after bfoo_kvp_set()");
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
    tap_is_int(bfoo_kvp_exists(hash, 1), 1, "bfoo_kvp_exists(1) returns 1 after bfoo_kvp_set()");
    tap_is_int(bfoo_kvp_exists(hash, 3), 1, "bfoo_kvp_exists(3) returns 1 after bfoo_kvp_set()");
    tap_is_int(bfoo_kvp_exists(hash, 5), 1, "bfoo_kvp_exists(5) returns 1 after bfoo_kvp_set()");
    tap_is_int(bfoo_kvp_exists(hash, 2), 0, "bfoo_kvp_exists(2) returns 0 after bfoo_kvp_set()");
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
    tap_is_int(bfoo_kvp_nkeys(hash), 3, "bfoo_kvp_nkeys() returns 3 after 3 bfoo_kvp_set()s");
    {
        struct bfoo_kvp *copy = bfoo_kvp_copy(hash, 4);
        assert(copy != NULL);
        tap_is_int(bfoo_kvp_nkeys(copy), 3, "bfoo_kvp_nkeys() returns 3 after bfoo_kvp_copy()");
        tap_is_voidp(bfoo_kvp_get(copy, 1), f1,
            "bfoo_kvp_get(1) returns the same pointer that was passed to bfoo_kvp_set()");
        tap_is_voidp(bfoo_kvp_get(copy, 2), f2,
            "bfoo_kvp_get(3) returns the same pointer that was passed to bfoo_kvp_set()");
        tap_is_voidp(bfoo_kvp_get(copy, 3), f3,
            "bfoo_kvp_get(5) returns the same pointer that was passed to bfoo_kvp_set()");
        {
            struct bfoo_kvp_stats stats = bfoo_kvp_stats(copy);
            tap_is_int(stats.nkeys, 3, "bfoo_kvp_stats.nkeys == 3");
            tap_is_int(stats.size, 4, "bfoo_kvp_stats.size == 4");
            tap_is_int(stats.nempty, 1, "bfoo_kvp_stats.nempty == 1");
            tap_is_int(stats.nused, 3, "bfoo_kvp_stats.nused == 3");
            tap_is_int(stats.ncollisions, 0, "bfoo_kvp_stats.ncollisions == 0");
            tap_is_int(stats.ninchainmin, 1, "bfoo_kvp_stats.ninchainmin == 1");
            tap_is_int(stats.ninchainmax, 1, "bfoo_kvp_stats.ninchainmax == 1");
        }
        bfoo_kvp_clear(copy);
        tap_is_int(bfoo_kvp_nkeys(copy), 0, "bfoo_kvp_nkeys() returns 0 after bfoo_kvp_clear()");
        tap_is_int(bfoo_kvp_nkeys(hash), 3, "bfoo_kvp_nkeys() returns 3 after bfoo_kvp_clear(copy)");
        bfoo_kvp_free(copy);
    }
    tap_is_int(bfoo_kvp_nkeys(hash), 3, "bfoo_kvp_nkeys() returns 3 after 3 bfoo_kvp_copy()s");
    bfoo_kvp_clear(hash);
    tap_is_int(bfoo_kvp_nkeys(hash), 0, "bfoo_kvp_nkeys() returns 0 after bfoo_kvp_clear()");
    {
        struct bfoo_kvp *hash1 = bfoo_kvp_new(8);
        struct bfoo_kvp *hash2 = bfoo_kvp_new(8);
        bfoo_kvp_set(hash1, 1, f1);
        bfoo_kvp_set(hash1, 2, f2);
        bfoo_kvp_set(hash1, 3, f3);
        bfoo_kvp_set(hash2, 1, f3);
        bfoo_kvp_set(hash2, 10, f1);
        bfoo_kvp_set(hash2, 20, f2);
        bfoo_kvp_set(hash2, 30, f3);
        {
            struct bfoo_kvp *merged_12 = bfoo_kvp_merge(hash1, hash2, 4);
            assert(merged_12 != NULL);
            tap_is_int(bfoo_kvp_nkeys(merged_12), 6, "bfoo_kvp_nkeys() returns 6 after bfoo_kvp_merge()[1, 2]");
            tap_is_voidp(bfoo_kvp_get(merged_12, 1), f3, "bfoo_kvp_get(1) returns the override (f3) from hash2");
            tap_is_voidp(bfoo_kvp_get(merged_12, 2), f2, "bfoo_kvp_get(2) returns the original (f2) from hash1");
            tap_is_voidp(bfoo_kvp_get(merged_12, 3), f3, "bfoo_kvp_get(3) returns the original (f3) from hash1");
            tap_is_voidp(bfoo_kvp_get(merged_12, 10), f1, "bfoo_kvp_get(10) returns the original (f1) from hash2");
            tap_is_voidp(bfoo_kvp_get(merged_12, 20), f2, "bfoo_kvp_get(20) returns the original (f2) from hash2");
            tap_is_voidp(bfoo_kvp_get(merged_12, 30), f3, "bfoo_kvp_get(30) returns the original (f3) from hash2");
            bfoo_kvp_free(merged_12);
        }
        {
            // NOLINTNEXTLINE(readability-suspicious-call-argument)
            struct bfoo_kvp *merged_21 = bfoo_kvp_merge(hash2, hash1, 4);
            assert(merged_21 != NULL);
            tap_is_int(bfoo_kvp_nkeys(merged_21), 6, "bfoo_kvp_nkeys() returns 6 after bfoo_kvp_merge()[2, 1]");
            tap_is_voidp(bfoo_kvp_get(merged_21, 1), f1, "bfoo_kvp_get(1) returns the override (f1) from hash1");
            tap_is_voidp(bfoo_kvp_get(merged_21, 2), f2, "bfoo_kvp_get(2) returns the original (f2) from hash1");
            tap_is_voidp(bfoo_kvp_get(merged_21, 3), f3, "bfoo_kvp_get(3) returns the original (f3) from hash1");
            tap_is_voidp(bfoo_kvp_get(merged_21, 10), f1, "bfoo_kvp_get(10) returns the original (f1) from hash2");
            tap_is_voidp(bfoo_kvp_get(merged_21, 20), f2, "bfoo_kvp_get(20) returns the original (f2) from hash2");
            tap_is_voidp(bfoo_kvp_get(merged_21, 30), f3, "bfoo_kvp_get(30) returns the original (f3) from hash2");
            bfoo_kvp_free(merged_21);
        }
        bfoo_kvp_free(hash2);
        bfoo_kvp_free(hash1);
    }
    {
        struct bfoo_kvp *copy = bfoo_kvp_copy(hash, 2);
        assert(copy != NULL);
        tap_is_int(bfoo_kvp_nkeys(copy), 0, "bfoo_kvp_nkeys() returns 0 after bfoo_kvp_copy()");
        tap_is_voidp(bfoo_kvp_get(copy, 1), NULL, "bfoo_kvp_get(1) returns NULL after bfoo_kvp_copy()");
        bfoo_kvp_set(copy, 1, f1);
        bfoo_kvp_set(copy, 2, f2);
        bfoo_kvp_set(copy, 5, f3);
        tap_is_int(bfoo_kvp_nkeys(copy), 3, "bfoo_kvp_nkeys() returns 3 after 3 bfoo_kvp_set()s");
        tap_is_voidp(bfoo_kvp_get(copy, 1), f1,
            "bfoo_kvp_get(1) returns the same pointer that was passed to bfoo_kvp_set()");
        tap_is_voidp(bfoo_kvp_get(copy, 2), f2,
            "bfoo_kvp_get(2) returns the same pointer that was passed to bfoo_kvp_set()");
        tap_is_voidp(bfoo_kvp_get(copy, 5), f3,
            "bfoo_kvp_get(5) returns the same pointer that was passed to bfoo_kvp_set()");
        struct bfoo_kvp_stats stats = bfoo_kvp_stats(copy);
        tap_is_int(stats.nkeys, 3, "bfoo_kvp_stats.nkeys == 3");
        tap_is_int(stats.size, 2, "bfoo_kvp_stats.size == 2");
        tap_is_int(stats.nempty, 0, "bfoo_kvp_stats.nempty == 0");
        tap_is_int(stats.nused, 2, "bfoo_kvp_stats.nused == 2");
        tap_is_int(stats.ncollisions, 1, "bfoo_kvp_stats.ncollisions == 1");
        tap_is_int(stats.ninchainmin, 1, "bfoo_kvp_stats.ninchainmin == 1");
        tap_is_int(stats.ninchainmax, 2, "bfoo_kvp_stats.ninchainmax == 2");
        // TODO: once I have tap_is_double or similar.
        tap_diag("stats.avg_chain_len = %Lf", stats.avg_chain_len); // 1.5
        tap_diag("stats.load_factor   = %f", stats.load_factor);    // 1.0
        bfoo_kvp_free(copy);
    }
    bfoo_kvp_free(hash);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
