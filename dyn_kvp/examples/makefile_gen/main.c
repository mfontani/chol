#include "inc.h"
#include "foo_kvp.h"

int main(void)
{
    struct foo *val = malloc(sizeof(struct foo));
    if (!val) {
        perror("malloc");
        return EXIT_FAILURE;
    }
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
