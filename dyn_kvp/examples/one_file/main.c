#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

struct foo {
    int a;
    int b;
};

#define DYN_KVP_VALUE_TYPE struct foo
#define DYN_KVP_TYPE_NAME foo_kvp
#include "../../dyn_kvp.h"
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE

#define DYN_KVP_VALUE_TYPE struct foo
#define DYN_KVP_TYPE_NAME foo_kvp
#define DYN_KVP_IMPLEMENTATION
#include "../../dyn_kvp.h"
#undef DYN_KVP_IMPLEMENTATION
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE

int main(void)
{
    struct foo *val = malloc(sizeof(struct foo));
    if (!val) {
        perror("malloc");
        return EXIT_FAILURE;
    }
    val->a = 1;
    val->b = 2;
    struct foo_kvp *hash = foo_kvp_new(8);
    // Populate val...
    foo_kvp_set(hash, 1, val);
    // Do stuff with it; later on...
    foo_kvp_del(hash, 1);
    // Clean things up:
    foo_kvp_free(hash);
    // You'll have to clean up the MEMBERS yourself!
    free(val);
}
