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

#define DYN_DLLIST_VALUE_TYPE struct basic_foo
#define DYN_DLLIST_TYPE_NAME bfoo_list
#define DYN_DLLIST_IMPLEMENTATION
#include "../../dyn_dllist.h"
#undef DYN_DLLIST_IMPLEMENTATION
#undef DYN_DLLIST_TYPE_NAME
#undef DYN_DLLIST_VALUE_TYPE

int list_voidp_equals(struct bfoo_list *list, struct basic_foo *val, void *param)
{
    return val == param;
}

int list_voidp_not_equals(struct bfoo_list *list, struct basic_foo *val, void *param)
{
    return val != param;
}

int list_voidp_remove_if_equals(struct bfoo_list *list, struct basic_foo *val, void *param)
{
    if (val == param)
    {
        bfoo_list_remove_all(list, val);
        return 1;   // Stops.
    }
    return 0;
}

int list_voidp_remove_if_not_equals(struct bfoo_list *list, struct basic_foo *val, void *param)
{
    if (val != param)
    {
        bfoo_list_remove_all(list, val);
        return 1;   // Stops.
    }
    return 0;
}

void test_basic_foo(void)
{
    tap_diag("Running tests for test_basic_foo()");
    struct basic_foo *f1 = basic_foo_new(1, 2);
    struct basic_foo *f2 = basic_foo_new(3, 4);
    struct basic_foo *f3 = basic_foo_new(5, 6);
    struct bfoo_list *list = bfoo_list_new();
    tap_is_int(bfoo_list_size(list), 0, "new list is empty");
    tap_is_int(bfoo_list_contains(list, f1), 0, "list does not contain f1");
    bfoo_list_append1_ftl(list, f1);
    tap_is_int(bfoo_list_size(list), 1, "list has size 1 after one append");
    tap_is_int(bfoo_list_contains(list, f1), 1, "list contains f1 after append");
    tap_is_int(bfoo_list_countif(list, list_voidp_equals, f1), 1, "list contains f1 once");
    bfoo_list_append1_ftl(list, f2);
    tap_is_int(bfoo_list_size(list), 2, "list has size 2 after two appends");
    tap_is_int(bfoo_list_contains(list, f1), 1, "list still contains f1 after append");
    tap_is_int(bfoo_list_countif(list, list_voidp_equals, f1), 1, "list contains f1 once");
    tap_is_int(bfoo_list_contains(list, f2), 1, "list contains f2 after append");
    tap_is_int(bfoo_list_countif(list, list_voidp_equals, f2), 1, "list contains f2 once");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1");
    tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2");
    bfoo_list_append1_ftl(list, f2);
    tap_is_int(bfoo_list_size(list), 2, "list still has size 2 after append1_ftl(f2)");
    {
        struct basic_foo *f = bfoo_list_shift(list);
        tap_is_voidp(f, f1, "shifted element is f1");
        tap_is_int(bfoo_list_size(list), 1, "list has size 1 after shift");
        tap_is_voidp(bfoo_list_peek_first(list), f2, "first element is f2 after shift");
        tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after shift");
    }
    bfoo_list_prepend1_ftl(list, f3);
    tap_is_int(bfoo_list_size(list), 2, "list has size 2 after prepend1_ftl(f3)");
    {
        struct basic_foo *f = bfoo_list_pop(list);
        tap_is_voidp(f, f2, "popped element is f2");
        tap_is_int(bfoo_list_size(list), 1, "list has size 1 after pop");
        tap_is_voidp(bfoo_list_peek_first(list), f3, "first element is f3 after pop");
        tap_is_voidp(bfoo_list_peek_last(list), f3, "last element is f3 after pop");
    }
    bfoo_list_prepend1_ftl(list, f1);
    bfoo_list_prepend1_ftl(list, f2);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after prepend1_ftl(f2)");
    {
        struct basic_foo *f = bfoo_list_pop(list);
        tap_is_voidp(f, f3, "popped element is f3");
        tap_is_int(bfoo_list_size(list), 2, "list has size 2 after pop");
        tap_is_voidp(bfoo_list_peek_first(list), f2, "first element is f2 after pop");
        tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after pop");
    }
    {
        struct basic_foo *f = bfoo_list_shift(list);
        tap_is_voidp(f, f2, "shifted element is f2");
        tap_is_int(bfoo_list_size(list), 1, "list has size 1 after shift");
        tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after shift");
        tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after shift");
    }
    bfoo_list_append(list, f2);
    tap_is_int(bfoo_list_size(list), 2, "list has size 2 after append(f2)");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after append(f2)");
    tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after append(f2)");
    bfoo_list_append(list, f2);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after append(f2)");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after append(f2)");
    tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after append(f2)");
    tap_is_int(bfoo_list_countif(list, list_voidp_equals, f2), 2, "list contains f2 twice");
    {
        struct bfoo_list *f2s = bfoo_list_grep(list, list_voidp_equals, f2);
        tap_is_int(bfoo_list_size(f2s), 2, "f2s has size 2");
        tap_is_voidp(bfoo_list_peek_first(f2s), f2, "first element of f2s is f2");
        tap_is_voidp(bfoo_list_peek_last(f2s), f2, "last element of f2s is f2");
        bfoo_list_free(f2s);
    }
    {
        struct basic_foo *f = bfoo_list_pop(list);
        tap_is_voidp(f, f2, "popped element is f2");
        tap_is_int(bfoo_list_size(list), 2, "list has size 2 after pop");
        tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after pop");
        tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after pop");
    }
    {
        struct basic_foo *f = bfoo_list_shift(list);
        tap_is_voidp(f, f1, "shifted element is f1");
        tap_is_int(bfoo_list_size(list), 1, "list has size 1 after shift");
        tap_is_voidp(bfoo_list_peek_first(list), f2, "first element is f2 after shift");
        tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after shift");
    }
    {
        struct basic_foo *f = bfoo_list_pop(list);
        tap_is_voidp(f, f2, "popped element is f2");
        tap_is_int(bfoo_list_size(list), 0, "list has size 0 after pop");
    }
    bfoo_list_append(list, f2);
    bfoo_list_append(list, f1);
    bfoo_list_append(list, f3);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after append(f1)");
    tap_is_int(bfoo_list_contains_ftl(list, f1), 1, "list contains [ltf] f1 after append(f1)");
    tap_is_int(bfoo_list_contains_ltf(list, f1), 1, "list contains [ftl] f1 after append(f1)");
    tap_is_int(bfoo_list_contains_ftl(list, f2), 1, "list contains [ltf] f2 after append(f1)");
    tap_is_int(bfoo_list_contains_ltf(list, f2), 1, "list contains [ftl] f2 after append(f1)");
    tap_is_int(bfoo_list_contains_ftl(list, f3), 1, "list contains [ltf] f3 after append(f1)");
    tap_is_int(bfoo_list_contains_ltf(list, f3), 1, "list contains [ftl] f3 after append(f1)");
    (void)bfoo_list_pop(list);
    (void)bfoo_list_shift(list);
    tap_is_int(bfoo_list_size(list), 1, "list has size 1 after pop and shift");
    tap_is_int(bfoo_list_contains_ftl(list, f2), 0, "list does not contain [ltf] f2 after pop and shift");
    tap_is_int(bfoo_list_contains_ltf(list, f2), 0, "list does not contain [ftl] f2 after pop and shift");
    tap_is_int(bfoo_list_contains_ftl(list, f3), 0, "list does not contain [ltf] f3 after pop and shift");
    tap_is_int(bfoo_list_contains_ltf(list, f3), 0, "list does not contain [ftl] f3 after pop and shift");
    tap_is_int(bfoo_list_contains_ftl(list, f1), 1, "list contains [ltf] f1 after pop and shift");
    tap_is_int(bfoo_list_contains_ltf(list, f1), 1, "list contains [ftl] f1 after pop and shift");
    bfoo_list_append1_ftl(list, f2);
    bfoo_list_append1_ftl(list, f3);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after append1_ftl(f2) and append1_ftl(f3)");
    bfoo_list_append1_ftl(list, f1);
    tap_is_int(bfoo_list_size(list), 3, "list has size 4 after append1_ftl(f1)");
    tap_is_int(bfoo_list_contains_ftl(list, f1), 1, "list contains [ltf] f1 after append1_ftl(f1)");
    tap_is_voidp(bfoo_list_peek_last(list), f3, "last element is f3 after append1_ftl(f1)");
    (void)bfoo_list_pop(list);
    (void)bfoo_list_pop(list);
    (void)bfoo_list_pop(list);
    tap_is_int(bfoo_list_size(list), 0, "list has size 0 after 3 pops");
    bfoo_list_append1_ltf(list, f1);
    bfoo_list_append1_ltf(list, f2);
    bfoo_list_append1_ltf(list, f3);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after append1_ltf(f1), append1_ltf(f2), append1_ltf(f3)");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after append1_ltf(f1), append1_ltf(f2), append1_ltf(f3)");
    tap_is_voidp(bfoo_list_peek_last(list), f3, "last element is f3 after append1_ltf(f1), append1_ltf(f2), append1_ltf(f3)");
    (void)bfoo_list_pop(list);
    (void)bfoo_list_pop(list);
    (void)bfoo_list_pop(list);
    tap_is_int(bfoo_list_size(list), 0, "list has size 0 after 3 pops");
    bfoo_list_prepend1_ltf(list, f1);
    bfoo_list_prepend1_ltf(list, f2);
    bfoo_list_prepend1_ltf(list, f3);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after prepend1_ltf(f1), prepend1_ltf(f2), prepend1_ltf(f3)");
    tap_is_voidp(bfoo_list_peek_first(list), f3, "first element is f3 after prepend1_ltf(f1), prepend1_ltf(f2), prepend1_ltf(f3)");
    tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after prepend1_ltf(f1), prepend1_ltf(f2), prepend1_ltf(f3)");
    bfoo_list_remove_all(list, f1);
    tap_is_int(bfoo_list_size(list), 2, "list has size 2 after remove_all(list, f1)");
    tap_is_voidp(bfoo_list_peek_first(list), f3, "first element is f3 after remove_all(list, f1)");
    tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after remove_all(list, f1)");
    (void)bfoo_list_pop(list);
    (void)bfoo_list_pop(list);
    tap_is_int(bfoo_list_size(list), 0, "list has size 0 after 2 pops");
    bfoo_list_append(list, f1);
    bfoo_list_append(list, f2);
    bfoo_list_append(list, f3);
    bfoo_list_append(list, f1);
    tap_is_int(bfoo_list_size(list), 4, "list has size 4 after append(f1), append(f2), append(f3), append(f1)");
    bfoo_list_removenth_ftl(list, f1, 0);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after removenth_ftl(list, f1, 0)");
    tap_is_voidp(bfoo_list_peek_first(list), f2, "first element is f2 after removenth_ftl(list, f1, 0)");
    tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after removenth_ftl(list, f1, 0)");
    bfoo_list_prepend(list, f1);
    tap_is_int(bfoo_list_size(list), 4, "list has size 4 after prepend(f1)");
    bfoo_list_removenth_ltf(list, f1, 0);
    tap_is_int(bfoo_list_size(list), 3, "list has size 3 after removenth_ltf(list, f1, 0)");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after removenth_ltf(list, f1, 0)");
    tap_is_voidp(bfoo_list_peek_last(list), f3, "last element is f3 after removenth_ltf(list, f1, 0)");
    bfoo_list_append(list, f1);
    tap_is_int(bfoo_list_size(list), 4, "list has size 4 after append(f1)");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after append(f1)");
    tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after append(f1)");
    {
        struct basic_foo *f = bfoo_list_nth_ftl(list, list_voidp_equals, f1, 0);
        tap_is_voidp(f, f1, "nth_ftl(list, list_voidp_equals, f1, 0) is f1");
    }
    {
        struct basic_foo *f = bfoo_list_nth_ftl(list, list_voidp_not_equals, f1, 0);
        tap_is_voidp(f, f2, "nth_ftl(list, list_voidp_not_equals, f1, 0) is f2");
    }
    {
        struct basic_foo *f = bfoo_list_nth_ltf(list, list_voidp_equals, f1, 0);
        tap_is_voidp(f, f1, "nth_ltf(list, list_voidp_equals, f1, 0) is f1");
    }
    {
        struct basic_foo *f = bfoo_list_nth_ltf(list, list_voidp_not_equals, f1, 0);
        tap_is_voidp(f, f3, "nth_ltf(list, list_voidp_not_equals, f1, 0) is f3");
    }
    // F1 F2 F3 F1
    tap_is_int(bfoo_list_size(list), 4, "list has still size 4");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1");
    tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1");
    {
        // F1 F2 F3 F1 -> F1 F3 F1, as it should stop at the first one.
        bfoo_list_iterate_ftl(list, list_voidp_remove_if_not_equals, f1);
        tap_is_int(bfoo_list_size(list), 3, "list has size 3 after iterate_ftl(list, list_voidp_remove_if_not_equals, f1)");
        tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after iterate_ftl(list, list_voidp_remove_if_not_equals, f1)");
        tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after iterate_ftl(list, list_voidp_remove_if_not_equals, f1)");
        // F1 F3 F1 -> F3 F1, as it should stop at the first one.
        (void)bfoo_list_shift(list);
        tap_is_int(bfoo_list_size(list), 2, "list has size 2 after shift");
        tap_is_voidp(bfoo_list_peek_first(list), f3, "first element is f3 after shift");
        (void)bfoo_list_shift(list);
        (void)bfoo_list_shift(list);
        tap_is_int(bfoo_list_size(list), 0, "list has size 0 after 2 shifts");
    }
    bfoo_list_append(list, f1);
    bfoo_list_append(list, f2);
    bfoo_list_append(list, f3);
    bfoo_list_append(list, f1);
    tap_is_int(bfoo_list_size(list), 4, "list has size 4 after append(f1), append(f2), append(f3), append(f1)");
    tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after append(f1), append(f2), append(f3), append(f1)");
    tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after append(f1), append(f2), append(f3), append(f1)");
    {
        // F1 F2 F3 F1 -> F1 F2 F1, as it should stop at the first one from the RIGHT.
        bfoo_list_iterate_ltf(list, list_voidp_remove_if_not_equals, f1);
        tap_is_int(bfoo_list_size(list), 3, "list has size 3 after iterate_ltf(list, list_voidp_remove_if_not_equals, f1)");
        tap_is_voidp(bfoo_list_peek_first(list), f1, "first element is f1 after iterate_ltf(list, list_voidp_remove_if_not_equals, f1)");
        tap_is_voidp(bfoo_list_peek_last(list), f1, "last element is f1 after iterate_ltf(list, list_voidp_remove_if_not_equals, f1)");
        // F1 F2 F1 -> F1 F2, as it should stop at the first one from the RIGHT.
        (void)bfoo_list_pop(list);
        tap_is_int(bfoo_list_size(list), 2, "list has size 2 after pop");
        tap_is_voidp(bfoo_list_peek_last(list), f2, "last element is f2 after pop");
        (void)bfoo_list_pop(list);
        (void)bfoo_list_pop(list);
        tap_is_int(bfoo_list_size(list), 0, "list has size 0 after 2 pops");
    }
    (void)bfoo_list_pop(list);
    tap_is_int(bfoo_list_size(list), 0, "list has size 0 after spurious pop");
    (void)bfoo_list_shift(list);
    tap_is_int(bfoo_list_size(list), 0, "list has size 0 after spurious shift");
    bfoo_list_free(list);
    basic_foo_free(f1);
    basic_foo_free(f2);
    basic_foo_free(f3);
}
