#include "basic.h"
#include "../tap/tap.h"
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

#define DYN_DLL_VALUE_TYPE struct basic_foo
#define DYN_DLL_TYPE_NAME bfoo_list
#define DYN_DLL_IMPLEMENTATION
#include "../../dyn_dll.h"
#undef DYN_DLL_IMPLEMENTATION
#undef DYN_DLL_TYPE_NAME
#undef DYN_DLL_VALUE_TYPE

int list_voidp_equals(struct bfoo_list *list, struct basic_foo *val, void *param)
{
    return val == param;
}

void test_basic_foo(void)
{
    diag("Running tests for test_basic_foo()");
    struct basic_foo *f1 = basic_foo_new(1, 2);
    struct basic_foo *f2 = basic_foo_new(3, 4);
    struct basic_foo *f3 = basic_foo_new(5, 6);
    struct bfoo_list *list = bfoo_list_new();
    test_is_int(bfoo_list_size(list), 0, "new list is empty");
    test_is_int(bfoo_list_contains(list, f1), 0, "list does not contain f1");
    bfoo_list_append1_ftl(list, f1);
    test_is_int(bfoo_list_size(list), 1, "list has size 1 after one append");
    test_is_int(bfoo_list_contains(list, f1), 1, "list contains f1 after append");
    test_is_int(bfoo_list_countif(list, list_voidp_equals, f1), 1, "list contains f1 once");
    bfoo_list_append1_ftl(list, f2);
    test_is_int(bfoo_list_size(list), 2, "list has size 2 after two appends");
    test_is_int(bfoo_list_contains(list, f1), 1, "list still contains f1 after append");
    test_is_int(bfoo_list_countif(list, list_voidp_equals, f1), 1, "list contains f1 once");
    test_is_int(bfoo_list_contains(list, f2), 1, "list contains f2 after append");
    test_is_int(bfoo_list_countif(list, list_voidp_equals, f2), 1, "list contains f2 once");
    test_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1");
    test_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2");
    bfoo_list_append1_ftl(list, f2);
    test_is_int(bfoo_list_size(list), 2, "list still has size 2 after append1_ftl(f2)");
    {
        struct basic_foo *f = bfoo_list_shift(list);
        test_is_voidp(f, f1, "shifted element is f1");
        test_is_int(bfoo_list_size(list), 1, "list has size 1 after shift");
        test_is_voidp(bfoo_list_peek_first(list), f2, "first element is f2 after shift");
        test_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after shift");
    }
    bfoo_list_prepend1_ftl(list, f3);
    test_is_int(bfoo_list_size(list), 2, "list has size 2 after prepend1_ftl(f3)");
    {
        struct basic_foo *f = bfoo_list_pop(list);
        test_is_voidp(f, f2, "popped element is f2");
        test_is_int(bfoo_list_size(list), 1, "list has size 1 after pop");
        test_is_voidp(bfoo_list_peek_first(list), f3, "first element is f3 after pop");
        test_is_voidp(bfoo_list_peek_last(list), f3, "last element is f3 after pop");
    }
    bfoo_list_prepend1_ftl(list, f1);
    bfoo_list_prepend1_ftl(list, f2);
    test_is_int(bfoo_list_size(list), 3, "list has size 3 after prepend1_ftl(f2)");
    {
        struct basic_foo *f = bfoo_list_pop(list);
        test_is_voidp(f, f3, "popped element is f3");
        test_is_int(bfoo_list_size(list), 2, "list has size 2 after pop");
        test_is_voidp(bfoo_list_peek_first(list), f2, "first element is f2 after pop");
        test_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after pop");
    }
    {
        struct basic_foo *f = bfoo_list_shift(list);
        test_is_voidp(f, f2, "shifted element is f2");
        test_is_int(bfoo_list_size(list), 1, "list has size 1 after shift");
        test_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after shift");
        test_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after shift");
    }
    bfoo_list_append(list, f2);
    test_is_int(bfoo_list_size(list), 2, "list has size 2 after append(f2)");
    test_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after append(f2)");
    test_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after append(f2)");
    bfoo_list_append(list, f2);
    test_is_int(bfoo_list_size(list), 3, "list has size 3 after append(f2)");
    test_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after append(f2)");
    test_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after append(f2)");
    test_is_int(bfoo_list_countif(list, list_voidp_equals, f2), 2, "list contains f2 twice");
    {
        struct bfoo_list *f2s = bfoo_list_grep(list, list_voidp_equals, f2);
        test_is_int(bfoo_list_size(f2s), 2, "f2s has size 2");
        test_is_voidp(bfoo_list_peek_first(f2s), f2, "first element of f2s is f2");
        test_is_voidp(bfoo_list_peek_last(f2s), f2, "last element of f2s is f2");
        bfoo_list_free(f2s);
    }
    {
        struct basic_foo *f = bfoo_list_pop(list);
        test_is_voidp(f, f2, "popped element is f2");
        test_is_int(bfoo_list_size(list), 2, "list has size 2 after pop");
        test_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after pop");
        test_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after pop");
    }
    {
        struct basic_foo *f = bfoo_list_shift(list);
        test_is_voidp(f, f1, "shifted element is f1");
        test_is_int(bfoo_list_size(list), 1, "list has size 1 after shift");
        test_is_voidp(bfoo_list_peek_first(list), f2, "first element is f2 after shift");
        test_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after shift");
    }
    {
        struct basic_foo *f = bfoo_list_pop(list);
        test_is_voidp(f, f2, "popped element is f2");
        test_is_int(bfoo_list_size(list), 0, "list has size 0 after pop");
    }
    bfoo_list_free(list);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
