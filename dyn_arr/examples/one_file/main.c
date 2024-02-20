#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct foo {
    int a;
    int b;
};

#define DYN_ARR_VALUE_TYPE struct foo
#define DYN_ARR_TYPE_NAME foo_arr
#include "../../dyn_arr.h"
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE

#define DYN_ARR_VALUE_TYPE struct foo
#define DYN_ARR_TYPE_NAME foo_arr
#define DYN_ARR_IMPLEMENTATION
#include "../../dyn_arr.h"
#undef DYN_ARR_IMPLEMENTATION
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE

int main(void)
{
    struct foo *val = malloc(sizeof(struct foo));
    if (!val) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    val->a = 1;
    val->b = 2;
    struct foo_arr *arr = foo_arr_new(8, 2, 1);
    foo_arr_push(arr, val);
    // Do stuff with it; later on...
    foo_arr_del(arr, 0);
    // Clean things up:
    foo_arr_free(arr);
    // You'll have to clean up the MEMBERS yourself!
    free(val);
}
