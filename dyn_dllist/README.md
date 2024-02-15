# `dyn_dllist.h` - C "dynamic" doubly-linked-list header-only library

This "macro-ed" header file creates definitions and implementations for a doubly-linked list that has a given `DYN_DLLIST_VALUE_TYPE *` as "value", and where the name for `struct`s and functions are "templated" via the given `DYN_DLLIST_TYPE_NAME`, providing type safety.

The system only helps manage (via `_new` and `_free` suffixed function names) the lifecycle of the list itself, NOT of the members you put in them. That is up to your code.

# Minimal usage

Say you've got just one `main.c` file, containing a struct declaration for which you'd like to also define a doubly-linked list type for, i.e.:

```c
struct foo { ... };
```

The `DYN_DLLIST_VALUE_TYPE` for this will be `struct foo`, and the doubly-linked list will contain a pointer to one.

You'll have to pick a name for the new _struct name_ (and related _functions' prefix_). Let's pick `foo_list` for it: the `DYN_DLLIST_TYPE_NAME` you'll want to specify will be `foo_list`.

You'll have to copy the `dyn_dllist.h` file to the same directory as `main.c`, and write in `main.c`:

```c
#define DYN_DLLIST_VALUE_TYPE struct foo
#define DYN_DLLIST_TYPE_NAME foo_list
#define DYN_DLLIST_IMPLEMENTATION
#include "dyn_dllist.h"
#undef DYN_DLLIST_IMPLEMENTATION
#undef DYN_DLLIST_TYPE_NAME
#undef DYN_DLLIST_VALUE_TYPE
```

In your code, you'll then use `struct foo_list *` to declare a variable of the newly created type, and will use functions named `foo_list_*` to interact with it:

```c
// Create one of your pre-existing structs, somehow:
struct foo *val = create_a_foo(/*...*/); // or malloc(sizeof(struct foo)) or whatever
// Here's what the "dyn_dllist.h" gives you:
struct foo_list *list = foo_list_new();
// Append "val" to the list:
foo_list_append(list, val);
// Do stuff with it
// Remove "val" from the list, wherever it is and however many there are in it:
foo_list_remove_all(list, val);
// Free the whole list (but NOT the contents!)
foo_list_free(list);
// You'll have to clean up your "struct foo *" MEMBERS yourself!
free(val);
```

See the `examples/` directory for more examples!

# Definition and Implementation

You can optionally put the definitions in one header file, and the implementation in a single file in your codebase:

## Definition

In one header file, you can grab/create the struct definitions at compile time. This can then be used by other `.c` files, which will share the definition:

```c
// Your pre-existing headers, giving access to "struct foo" definition:
#include "your_other_headers.h"
// What you need to add:
#define DYN_DLLIST_VALUE_TYPE struct foo
#define DYN_DLLIST_TYPE_NAME foo_list
#include "dyn_dllist.h"
#undef DYN_DLLIST_TYPE_NAME
#undef DYN_DLLIST_VALUE_TYPE
```

That `#include "dyn_dllist.h"` will result in something like this being inserted in the header:

```c
struct foo_list;
extern struct foo_list *foo_list_new(void);
extern void foo_list_free(struct foo_list *list);
// ...
```

Multiple such `#include "dyn_dllist.h"` (for **different** `DYN_DLLIST_VALUE_TYPE`s!) can be placed in the same header file.
Remember to `#undef`, then `#define` again, properties for the new type(s).

## Implementation

Similarly to the definition, you'll likely want to put the implementation in one `.c` file:

```c
// Your pre-existing headers, giving access to "struct foo" definition:
#include "your_other_headers.h"
// What you need to add:
#define DYN_DLLIST_VALUE_TYPE struct foo
#define DYN_DLLIST_TYPE_NAME foo_list
#define DYN_DLLIST_IMPLEMENTATION
#include "dyn_dllist.h"
#undef DYN_DLLIST_IMPLEMENTATION
#undef DYN_DLLIST_TYPE_NAME
#undef DYN_DLLIST_VALUE_TYPE
```

The fuller implementation will instead be placed in the file by the `#include "dyn_dllist.h"`, i.e. something like:

```c
struct foo_list__member { foo *p; struct foo_list__member *next; struct foo_list__member *prev; };
typedef struct foo_list { struct foo_list__member *first; struct foo_list__member *last; } foo_list;
struct foo_list *foo_list_new(void);
// ...
```

## Alternative generation methods

### Generate it once

You might opt to produce the definitions and/or implementations once, which might help code completion, and copy/paste them in the header file.
Both `gcc` or `tcc` or other compilers can help do this, via `-E` or similar flags. Here's one example of how to do it:

The definitions:

```bash
$ gcc -E -P -CC -D DYN_DLLIST_VALUE_TYPE='struct foo' -D DYN_DLLIST_TYPE_NAME=foo_list dyn_dllist.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$'
struct foo_list;
extern
struct foo_list *foo_list_new(void);
# ...
```

The implementation:

```bash
$ gcc -E -P -CC -D DYN_DLLIST_VALUE_TYPE='struct foo' -D DYN_DLLIST_TYPE_NAME=foo_list -D DYN_DLLIST_IMPLEMENTATION dyn_dllist.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$'
struct foo_list { /* ... */ };
struct foo_list *foo_list_new(void);
# ...
```

### Integrate it in your build system

You can instruct your build system to automatically generate the header file and the implementation file.

If you use a `Makefile`, you can use this as a starting point. This will rebuild the header file and the implementation file whenever the `Makefile` itself changes (as there might be changes in the `-D ...` defines) or the `dyn_dllist.h` changes (as you might get a new version, or tweak it). You might want to ignore those generated files in your build system.

```Makefile
foo_list.h: Makefile dyn_dllist.h
	@rm -f $@
	@printf '// Automatically generated from %s.\n' "$^" > $@
	@printf '// Automatically generated on   %s\n' "$$(date)" >> $@
	@printf '#ifndef FOO_LIST_H\n#define FOO_LIST_H\n' >> $@
	@gcc -E -P -CC -D DYN_DLLIST_VALUE_TYPE='struct foo' -D DYN_DLLIST_TYPE_NAME=foo_list dyn_dllist.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$$' >> $@
	@printf '#endif\n' >> $@
	@chmod -w $@

foo_list.c: Makefile dyn_dllist.h foo_list.h
	@rm -f $@
	@printf '// Automatically generated from %s.\n' "$^" > $@
	@printf '// Automatically generated on   %s\n' "$$(date)" >> $@
	@printf '#include "your_project_other_includes.h"\n' >> $@
	@printf '#include "char_data_dllist.h"\n' >> $@
	@gcc -E -P -CC -D DYN_DLLIST_VALUE_TYPE=struct foo' -D DYN_DLLIST_TYPE_NAME=foo_list -D DYN_DLLIST_IMPLEMENTATION dyn_dllist.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$$' >> $@
	@chmod -w $@
```

# Functions

The header also declares, and makes available, functions to interact with the doubly-linked list: add, remove, etc.

All functions here are mentioned by suffix. Your `-D DYN_DLLIST_TYPE_NAME=foo_list` will create functions whose name _starts_ with the value you picked `foo_list`, but whose suffix is constant, i.e. the `_new` will be available to you as `foo_list_new`.

## `_new` & `_free`

`_new` creates a new doubly-linked list, ready to accept (via `_append`, etc) `DYN_DLLIST_VALUE_TYPE` items in it.

Once done with the list, remember to use the `_free` function on it.

Note that the `_free` function will **not** free the `DYN_DLLIST_VALUE_TYPE` values which you might have placed in the list. You'll have to do that yourself, separately!

## `_size`

Returns the number of items in the list, without having to iterate the list. Upkeep performed at every insertion and deletion.

## `_peek_first`, `_peek_last`

Returns the item in the list's "first" or "last" position, respectively.
The item is **not** removed from the list. See `_shift` and `_pop` for that.

## `_contains`, `_contains_ftl`, `contains_ltf`

Returns `0` if the given item cannot be found on the list, or non-`0` if it can. There are variations:

- `_contains_ftl` iterates from `first` to `last`. Use it if the item is likely to exist, and to be found towards the front.
- `_contains_ltf` iterates from `last` to `first`. Use it if the item is likely to exist, and to be found towards the back.
- `_contains` uses `_contains_ftl`. Note it does one more function call than the other, more specific, versions.

If it's unlikely that the item is in the list, it doesn't matter which one you pick: the list has to be fully traversed to find out if it's in it.

## `_append`, `_append1_ftl`, `_append1_ltf`

Appends the given element at the **end** of the list. Variations:

- `_append` always appends the item, even if it's already on the list. If you use this, the list might contain the same value multiple times.
- `_append1_ftl` only appends the item if it's **not** already in the list. Uses `_contains_ftl` to find it. Use it if it's likely the item can be found towards the start of the list.
- `_append1_ltf` only appends the item if it's **not** already in the list. Uses `_contains_ltf` to find it. Use it if it's likely the item can be found towards the end of the list.

Use either `_append1_*` variation if you don't think the item is in the list, as the list has to be fully traversed to find out if it's in it.

## `_prepend`, `_prepend1_ftl`, `_prepend1_ltf`

Prepends the given element at the **start** of the list. Variations:

- `_prepend` always prepends the item, even if it's already on the list. If you use this, the list might contain the same value multiple times.
- `_prepend1_ftl` only prepends the item if it's **not** already in the list. Uses `_contains_ftl` to find it. Use it if it's likely the item can be found towards the start of the list.
- `_prepend1_ltf` only prepends the item if it's **not** already in the list. Uses `_contains_ltf` to find it. Use it if it's likely the item can be found towards the end of the list.

Use either `_prepend1_*` variation if you don't think the item is in the list, as the list has to be fully traversed to find out if it's in it.

## `_remove_all`, `_removenth_ftl`, `_removenth_ltf`

Removes a given element from the list. Variations:

- `_remove_all` removes **all** elements which match the given one from the list.
- `_removenth_ftl` removes the `nth` element which matches the given one from the list, iterating it first to last.
- `_removenth_ltf` removes the `nth` element which matches the given one from the list, iterating it last to first.

## `_shift`, `_pop`

Removes, **and returns**, a given element from the list:

- `_shift` removes **and returns** the **first** item in the list
- `_pop` removes **and returns** the **last** item in the list

## `_iterate_ftl` and `_iterate_ltf`

Iterates the list, calling a given "func" (with user-provided parameter "param") for each item "val" found, **stopping** if the "func" returns non-zero. The list is **not** modified by the iteration. Variations:

- `_iterate_ftl` iterates the list from first to last.
- `_iterate_ltf` iterates the list from last to first.

Note that within the "func" it should be safe to remove the given "val" from the list, if you wish to.

## `_countif`

Fully iterates the list, and returns the **count** of items "val" for which the given "func" (with user-provided parameter "param") returned non-zero.

## `_grep`

Returns a **new dll** created by iterating the original list from **from first to last**, calling the given "func" (with user-provided parameter "param") for each item "val" found, and `_append`ing "val" to the new list IFF the "func" returns non-zero.

As such, the new dll will contain matching items while retaining the order in which they were present in the original dllist.

Note that the original list is **not** modified, unless the "func" modifies it. It's also safe to remove the given "val" from the list, if you wish to.

## `_nth_ftl`, `_nth_ltf`, `_first`, `_last`

Returns the Nth item "val" from the list for which the given "func" (with user-provided parameter "param") returns non-zero. The list is **not** modified. Variations:

- `_nth_ftl` iterates the list first to last, returning the nth matching item
- `_nth_ltf` iterates the list last to first, returning the nth matching item
- `_first` uses `_nth_ftl`, returning the first matching item from first to last. Note it does one more function call than the other, more specific, version.
- `_last` uses `_nth_ltf`, returning the 'first' matching item from last to first, that is, the "last" matching item. Note it does one more function call than the other, more specific, version.

# LICENSE

The MIT License (MIT)

Copyright (c) 2024 Marco Fontani, MFONTANI@cpan.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
