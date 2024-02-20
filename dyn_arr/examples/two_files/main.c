#include "inc.h"
#include "foo_arr.h"

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
