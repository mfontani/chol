#include "basic.h"
#include "../../../tap/tap.h"

void test_basic_foo_remove(void)
{
    tap_diag("Running tests for test_basic_foo_remove()");
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
        struct basic_foo *p1 = bfoo_arr_pop(arr);
        tap_is_voidp(p1, f3, "popped element is f3");
        p1 = bfoo_arr_pop(arr);
        tap_is_voidp(p1, f2, "popped element is f2");
        p1 = bfoo_arr_pop(arr);
        tap_is_voidp(p1, f1, "popped element is f1");
        tap_is_int(bfoo_arr_length(arr), 0, "arr has length 0 after pops");
    }
    {
        arr = bfoo_arr_push(arr, f1);
        arr = bfoo_arr_push(arr, f2);
        arr = bfoo_arr_push(arr, f3);
        tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after push");
        tap_is_voidp(bfoo_arr_get(arr, 1), f2, "arr get(1) is f2 after push");
        tap_is_voidp(bfoo_arr_get(arr, 2), f3, "arr get(2) is f3 after push");
        struct basic_foo *p1 = bfoo_arr_shift(arr);
        tap_is_voidp(p1, f1, "shifted element is f1");
        p1 = bfoo_arr_shift(arr);
        tap_is_voidp(p1, f2, "shifted element is f2");
        p1 = bfoo_arr_shift(arr);
        tap_is_voidp(p1, f3, "shifted element is f3");
        tap_is_int(bfoo_arr_length(arr), 0, "arr has length 0 after shifts");
    }
    bfoo_arr_free(arr);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
