#include "basic.h"
#include "../../../tap/tap.h"

void test_basic_foo_add(void)
{
    tap_diag("Running tests for test_basic_foo_add()");
    struct basic_foo *f1 = basic_foo_new(1, 2);
    struct basic_foo *f2 = basic_foo_new(3, 4);
    struct basic_foo *f3 = basic_foo_new(5, 6);
    struct bfoo_arr *arr = bfoo_arr_new(32, 4, 2);
    {
        arr = bfoo_arr_push(arr, f1);
        arr = bfoo_arr_push(arr, f2);
        arr = bfoo_arr_push(arr, f3);
        tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after push");
        tap_is_voidp(bfoo_arr_get(arr, 1), f2, "arr get(1) is f2 after push");
        tap_is_voidp(bfoo_arr_get(arr, 2), f3, "arr get(2) is f3 after push");
        arr = bfoo_arr_unshift(arr, f3);
        tap_is_voidp(bfoo_arr_get(arr, 0), f3, "arr get(0) is f3 after unshift");
        tap_is_voidp(bfoo_arr_get(arr, 1), f1, "arr get(1) is f1 after unshift");
        tap_is_voidp(bfoo_arr_get(arr, 2), f2, "arr get(2) is f2 after unshift");
        tap_is_voidp(bfoo_arr_get(arr, 3), f3, "arr get(3) is f3 after unshift");
        bfoo_arr_clear(arr);
    }
    {
        arr = bfoo_arr_fill(arr, 3, f1);
        tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after fill");
        tap_is_voidp(bfoo_arr_get(arr, 1), f1, "arr get(1) is f2 after fill");
        tap_is_voidp(bfoo_arr_get(arr, 2), f1, "arr get(2) is f3 after fill");
        tap_is_voidp(bfoo_arr_get(arr, 3), NULL, "arr get(3) is NULL after fill");
        bfoo_arr_clear(arr);
    }
    {
        arr = bfoo_arr_fill(arr, 3, f1);
        tap_is_int(bfoo_arr_length(arr), 3, "arr has length 3 after fill");
        bfoo_arr_unique(arr);
        tap_is_int(bfoo_arr_length(arr), 1, "arr has length 1 after unique");
        tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after unique");
        tap_is_voidp(bfoo_arr_get(arr, 1), NULL, "arr get(1) is NULL after unique");
        bfoo_arr_clear(arr);
    }
    {
        tap_is_int(bfoo_arr_length(arr), 0, "arr has length 0");
        bfoo_arr_set(arr, 0, f1);
        tap_is_voidp(bfoo_arr_get(arr, 0), NULL, "arr get(0) is NULL after set");
        bfoo_arr_push(arr, f1);
        bfoo_arr_set(arr, 0, f2);
        tap_is_voidp(bfoo_arr_get(arr, 0), f2, "arr get(0) is f2 after set");
        bfoo_arr_clear(arr);
    }
    bfoo_arr_free(arr);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
