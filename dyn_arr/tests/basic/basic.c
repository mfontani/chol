#include "basic.h"
#include "../../../tap/tap.h"
#include <assert.h>

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

#define DYN_ARR_VALUE_TYPE struct basic_foo
#define DYN_ARR_TYPE_NAME bfoo_arr
#define DYN_ARR_IMPLEMENTATION
#include "../../dyn_arr.h"
#undef DYN_ARR_IMPLEMENTATION
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE

void test_basic_foo(void)
{
    tap_diag("Running tests for test_basic_foo()");
    struct basic_foo *f1 = basic_foo_new(1, 2);
    struct basic_foo *f2 = basic_foo_new(3, 4);
    struct basic_foo *f3 = basic_foo_new(5, 6);
    struct bfoo_arr *arr = bfoo_arr_new(32, 4, 2);
    tap_is_int(bfoo_arr_length(arr), 0, "new arr is empty (length 0)");
    tap_is_int(bfoo_arr_empty(arr), 1, "new arr is empty (empty 1)");
    tap_is_int(bfoo_arr_size(arr), 32, "new arr size is as given");
    tap_is_voidp(bfoo_arr_front(arr), NULL, "arr front is NULL");
    tap_is_voidp(bfoo_arr_back(arr), NULL, "arr back is NULL");
    tap_is_int(arr->istart, 3, "arr istart is 3");
    tap_is_int(arr->iend, 3, "arr iend is 3");
    arr = bfoo_arr_push(arr, f1);
    tap_is_int(arr->istart, 3, "arr istart is still 3 after push");
    tap_is_int(arr->iend, 3, "arr iend is 3 after push");
    tap_is_int(bfoo_arr_length(arr), 1, "arr has length 1 after push");
    tap_is_int(bfoo_arr_empty(arr), 0, "arr is not empty after push");
    tap_is_int(bfoo_arr_size(arr), 32, "arr size is still 32 after push");
    tap_is_voidp(bfoo_arr_front(arr), f1, "arr front is f1 after push");
    tap_is_voidp(bfoo_arr_back(arr), f1, "arr back is f1 after push");
    tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after push");
    arr = bfoo_arr_push(arr, f2);
    tap_is_int(arr->istart, 3, "arr istart is still 3 after push");
    tap_is_int(arr->iend, 4, "arr iend is 4 after push");
    tap_is_int(bfoo_arr_length(arr), 2, "arr has length 2 after push");
    tap_is_int(bfoo_arr_empty(arr), 0, "arr is not empty after push");
    tap_is_int(bfoo_arr_size(arr), 32, "arr size is still 32 after push");
    tap_is_voidp(bfoo_arr_front(arr), f1, "arr front is f1 after push");
    tap_is_voidp(bfoo_arr_back(arr), f2, "arr back is f2 after push");
    tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after push");
    tap_is_voidp(bfoo_arr_get(arr, 1), f2, "arr get(1) is f2 after push");
    arr = bfoo_arr_push(arr, f3);
    tap_is_int(arr->istart, 3, "arr istart is still 3 after push");
    tap_is_int(arr->iend, 5, "arr iend is 5 after push");
    tap_is_int(bfoo_arr_length(arr), 3, "arr has length 3 after push");
    tap_is_int(bfoo_arr_empty(arr), 0, "arr is not empty after push");
    tap_is_int(bfoo_arr_size(arr), 32, "arr size is still 32 after push");
    tap_is_voidp(bfoo_arr_front(arr), f1, "arr front is f1 after push");
    tap_is_voidp(bfoo_arr_back(arr), f3, "arr back is f3 after push");
    tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after push");
    tap_is_voidp(bfoo_arr_get(arr, 1), f2, "arr get(1) is f2 after push");
    tap_is_voidp(bfoo_arr_get(arr, 2), f3, "arr get(2) is f3 after push");
    {
        struct basic_foo *p1 = bfoo_arr_pop(arr);
        tap_is_voidp(p1, f3, "popped element is f3");
        tap_is_int(arr->istart, 3, "arr istart is still 3 after pop");
        tap_is_int(arr->iend, 4, "arr iend is now 4 after pop");
        tap_is_int(bfoo_arr_length(arr), 2, "arr has length 2 after pop");
        tap_is_int(bfoo_arr_empty(arr), 0, "arr is not empty after pop");
        tap_is_int(bfoo_arr_size(arr), 32, "arr size is still 32 after pop");
        tap_is_voidp(bfoo_arr_front(arr), f1, "arr front is f1 after pop");
        tap_is_voidp(bfoo_arr_back(arr), f2, "arr back is f2 after pop");
        tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1 after pop");
        tap_is_voidp(bfoo_arr_get(arr, 1), f2, "arr get(1) is f2 after pop");
    }
    {
        struct basic_foo *p2 = bfoo_arr_shift(arr);
        tap_is_voidp(p2, f1, "shifted element is f1");
        tap_is_int(bfoo_arr_length(arr), 1, "arr has length 1 after shift");
        tap_is_int(bfoo_arr_empty(arr), 0, "arr is not empty after shift");
        tap_is_int(bfoo_arr_size(arr), 32, "arr size is still 32 after shift");
        tap_is_voidp(bfoo_arr_front(arr), f2, "arr front is f2 after shift");
        tap_is_voidp(bfoo_arr_back(arr), f2, "arr back is f2 after shift");
        tap_is_voidp(bfoo_arr_get(arr, 0), f2, "arr get(0) is f2 after shift");
    }
    {
        bfoo_arr_clear(arr);
        tap_is_int(bfoo_arr_length(arr), 0, "arr has length 0 after clear");
        tap_is_int(bfoo_arr_empty(arr), 1, "arr is empty after clear");
        tap_is_int(bfoo_arr_size(arr), 32, "arr size is still 32 after clear");
        tap_is_voidp(bfoo_arr_front(arr), NULL, "arr front is NULL after clear");
        tap_is_voidp(bfoo_arr_back(arr), NULL, "arr back is NULL after clear");
    }
    {
        struct bfoo_arr *orig = arr;
        for (int i = 0; i < 32; i++) {
            tap_diag("Pushing element %d", i);
            tap_is_int(bfoo_arr_length(arr), i, "arr length is i before push");
            struct bfoo_arr *newarr = bfoo_arr_push(arr, f1);
            // 32
            // 4 leeway, then items 0..31
            // min_buffer 2, so when i == 27 (26+1 + 4 = 31 items, just over
            // min_buffer), we should double the size
            if (i >= 27) { // 32 total, 4 leeway either side
                tap_is_int(bfoo_arr_size(newarr), 64, "newarr size is doubled after push");
                tap_is_int(orig != newarr, 1, "newarr is a new pointer after push");
            } else {
                tap_is_int(bfoo_arr_size(newarr), 32, "newarr size is still 32 after push");
                tap_is_int(orig == newarr, 1, "arr is the same pointer after push");
            }
            arr = newarr;
        }
        bfoo_arr_clear(arr);
        tap_is_int(bfoo_arr_length(arr), 0, "arr has length 0 after clear");
        tap_is_int(bfoo_arr_empty(arr), 1, "arr is empty after clear");
        tap_is_int(bfoo_arr_size(arr), 64, "arr size is now 64 after clear");
        tap_is_int(orig != arr, 1, "arr is a new pointer after loop");
    }
    {
        arr = bfoo_arr_unshift(arr, f2);
        tap_is_int(bfoo_arr_length(arr), 1, "arr has length 1 after unshift");
        tap_is_int(bfoo_arr_empty(arr), 0, "arr is not empty after unshift");
        tap_is_int(bfoo_arr_size(arr), 64, "arr size is still 64 after unshift");
        tap_is_int(arr->istart, 3, "arr istart is 3 after unshift");
        tap_is_int(arr->iend, 3, "arr iend is 3 after unshift");
        bfoo_arr_clear(arr);
        tap_is_int(bfoo_arr_length(arr), 0, "arr has length 0 after clear");
        tap_is_int(bfoo_arr_empty(arr), 1, "arr is empty after clear");
        tap_is_int(bfoo_arr_size(arr), 64, "arr size is now 64 after clear");
    }
    {
        arr = bfoo_arr_push(arr, f1);
        arr = bfoo_arr_push(arr, f2);
        arr = bfoo_arr_push(arr, f3);
        tap_is_int(bfoo_arr_length(arr), 3, "arr has length 3");
        tap_is_int(arr->istart, 3, "arr istart is 3");
        tap_is_int(arr->iend, 5, "arr iend is 5");
        tap_is_voidp(bfoo_arr_get(arr, 0), f1, "arr get(0) is f1");
        tap_is_voidp(bfoo_arr_get(arr, 1), f2, "arr get(1) is f2");
        tap_is_voidp(bfoo_arr_get(arr, 2), f3, "arr get(2) is f3");
        struct bfoo_arr *newarr = bfoo_arr_resize(arr, 20, 6, 2);
        tap_is_int(bfoo_arr_length(newarr), 3, "newarr has length 3 after resize");
        tap_is_voidp(bfoo_arr_get(newarr, 0), f1, "newarr get(0) is f1");
        tap_is_voidp(bfoo_arr_get(newarr, 1), f2, "newarr get(1) is f2");
        tap_is_voidp(bfoo_arr_get(newarr, 2), f3, "newarr get(2) is f3");
        tap_is_int(newarr->istart, 5, "newarr istart is 5 after resize");
        tap_is_int(newarr->iend, 7, "newarr iend is 7 after resize");
        bfoo_arr_remove_all(newarr, f2);
        tap_is_int(bfoo_arr_length(newarr), 2, "newarr has length 2 after remove_all");
        tap_is_voidp(bfoo_arr_get(newarr, 0), f1, "newarr get(0) is f1 after remove_all");
        tap_is_voidp(bfoo_arr_get(newarr, 1), f3, "newarr get(1) is f3 after remove_all");
        bfoo_arr_del(newarr, 0);
        tap_is_int(bfoo_arr_length(newarr), 1, "newarr has length 1 after del");
        tap_is_voidp(bfoo_arr_get(newarr, 0), f3, "newarr get(0) is f3 after del");
        bfoo_arr_free(newarr);
        bfoo_arr_clear(arr);
    }
    bfoo_arr_free(arr);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
