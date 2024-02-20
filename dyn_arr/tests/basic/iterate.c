#include "basic.h"
#include "../../../tap/tap.h"

int sum_all(struct bfoo_arr *arr, struct basic_foo *it, void *sum)
{
    int *isum = (int *)sum;
    *isum += it->a;
    return 0;
}

int sum_even(struct bfoo_arr *arr, struct basic_foo *it, void *sum)
{
    if (it->a % 2 == 0) {
        int *isum = (int *)sum;
        *isum += it->a;
    }
    return 0;
}

int sum_odd(struct bfoo_arr *arr, struct basic_foo *it, void *sum)
{
    if (it->a % 2 == 1) {
        int *isum = (int *)sum;
        *isum += it->a;
    }
    return 0;
}

int sum_stop_at_sum_over_2(struct bfoo_arr *arr, struct basic_foo *it, void *sum)
{
    int *isum = (int *)sum;
    *isum += it->a;
    if (*isum > 2) {
        return 1;
    }
    return 0;
}

void test_basic_foo_iterate(void)
{
    tap_diag("Running tests for test_basic_foo_iterate()");
    struct basic_foo *f1 = basic_foo_new(1, 2);
    struct basic_foo *f2 = basic_foo_new(2, 4);
    struct basic_foo *f3 = basic_foo_new(5, 6);
    struct bfoo_arr *arr = bfoo_arr_new(32, 4, 2);
    arr = bfoo_arr_push(arr, f1);
    arr = bfoo_arr_push(arr, f2);
    arr = bfoo_arr_push(arr, f3);
    {
        int sum = 0;
        bfoo_arr_iterate_ftl(arr, sum_all, &sum);
        tap_is_int(sum, 8, "sum of all a elements is 8");
        sum = 0;
        bfoo_arr_iterate_ftl(arr, sum_even, &sum);
        tap_is_int(sum, 2, "sum of even a elements is 2");
        sum = 0;
        bfoo_arr_iterate_ftl(arr, sum_odd, &sum);
        tap_is_int(sum, 6, "sum of odd a elements is 6");
        sum = 0;
        bfoo_arr_iterate_ftl(arr, sum_stop_at_sum_over_2, &sum);
        tap_is_int(sum, 3, "sum of a elements over 2 stops at 3");
    }
    {
        int sum = 0;
        bfoo_arr_iterate_ltf(arr, sum_all, &sum);
        tap_is_int(sum, 8, "sum of all a elements is 8");
        sum = 0;
        bfoo_arr_iterate_ltf(arr, sum_even, &sum);
        tap_is_int(sum, 2, "sum of even a elements is 2");
        sum = 0;
        bfoo_arr_iterate_ltf(arr, sum_odd, &sum);
        tap_is_int(sum, 6, "sum of odd a elements is 6");
        sum = 0;
        bfoo_arr_iterate_ltf(arr, sum_stop_at_sum_over_2, &sum);
        tap_is_int(sum, 5, "sum of a elements over 2 stops at 5");
    }
    bfoo_arr_free(arr);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
