#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct foo {
    int a;
    int b;
};

#define DYN_DLLIST_VALUE_TYPE struct foo
#define DYN_DLLIST_TYPE_NAME foo_list
#define DYN_DLLIST_IMPLEMENTATION
#include "../../dyn_dllist.h"
#undef DYN_DLLIST_IMPLEMENTATION
#undef DYN_DLLIST_TYPE_NAME
#undef DYN_DLLIST_VALUE_TYPE

int main(void)
{
    struct foo *val = malloc(sizeof(struct foo));
    if (!val) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    struct foo_list *list = foo_list_new();
    // Populate val...
    foo_list_append(list, val);
    // Do stuff with it; later on...
    foo_list_remove_all(list, val);
    // Clean things up:
    foo_list_free(list);
    // You'll have to clean up the MEMBERS yourself!
    free(val);
}
