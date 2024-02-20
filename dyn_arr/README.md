# `dyn_arr.h` - C "dynamic" array-like header-only library

This "macro-ed" header file creates definitions and implementations for a struct that contains a dynamic array of a given `DYN_ARR_VALUE_TYPE *`, and where the name for `struct`s and functions are "templated" via the given `DYN_ARR_VALUE_TYPE`, providing type safety.

The system only helps manage (via `_new` and `_free` suffixed function names) the lifecycle of the structure itself, NOT of the members you put in them. That is up to your code.

# Minimal usage

Say you've got just one `main.c` file, containing a struct declaration for which you'd like to also define an array-like struct type for, i.e.:

```c
struct foo { ... };
```

The `DYN_ARR_VALUE_TYPE` for this will be `struct foo`, and the array-like struct will contain pointers to many of those.

You'll have to pick a name for the new _struct name_ (and related _functions' prefix_). Let's pick `foo_arr` for it: the `DYN_ARR_VALUE_TYPE` you'll want to specify will be `foo_arr`.

You'll have to copy the `dyn_arr.h` file to the same directory as `main.c`, and write in `main.c`:

```c
#define DYN_ARR_VALUE_TYPE struct foo
#define DYN_ARR_TYPE_NAME foo_arr
#define DYN_ARR_IMPLEMENTATION
#include "dyn_arr.h"
#undef DYN_ARR_IMPLEMENTATION
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE
```

In your code, you'll then use `struct foo_arr *` to declare a variable of the newly created type, and will use functions named `foo_arr_*` to interact with it:

```c
// Create one of your pre-existing structs, somehow:
struct foo *val = create_a_foo(/*...*/); // or malloc(sizeof(struct foo)) or whatever
// Here's what the "dyn_arr.h" gives you:
// Create a pre-allocated array with 10 "members" in it.
// Leave 4 "members" to either side "free", so unshifts and pushes don't trigger a resize as often
// Automatically resize (doubling the "members") once either side only has 2 free slots.
struct foo_arr *arr = foo_arr_new(10, 4, 2);
// Append "val" to the "arr". Can return a new "arr" (not in this case, as there's ample space...)
arr = foo_arr_push(arr, val);
// Append another:
arr = foo_arr_push(arr, val);
// Do whatever else you want with it: unshift, pop, get, etc.
// Free the whole struct for the array (but NOT the contents!)
foo_arr_free(arr);
// You'll have to clean up your "struct foo *" MEMBERS yourself!
free(val);
```

See the [examples/](examples/) directory for more examples!

# Definition and Implementation

You can optionally put the definitions in one header file, and the implementation in a single file in your codebase:

## Definition

In one header file, you can grab/create the struct definitions at compile time. This can then be used by other `.c` files, which will share the definition:

```c
// Your pre-existing headers, giving access to "struct foo" definition:
#include "your_other_headers.h"
// What you need to add:
#define DYN_ARR_VALUE_TYPE struct foo
#define DYN_ARR_TYPE_NAME foo_arr
#include "dyn_arr.h"
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE
```

That `#include "dyn_arr.h"` will result in something like this being inserted in the header:

```c
struct foo_arr;
extern struct foo_arr *foo_arr_new(size_t size, size_t buffer_size, size_t min_buffer);
extern void foo_arr_free(struct foo_arr *arr);
// ...
```

Multiple such `#include "dyn_arr.h"` (for **different** `DYN_ARR_VALUE_TYPE`s!) can be placed in the same header file.
Remember to `#undef`, then `#define` again, properties for the new type(s).

## Implementation

Similarly to the definition, you'll likely want to put the implementation in one `.c` file:

```c
// Your pre-existing headers, giving access to "struct foo" definition:
#include "your_other_headers.h"
// What you need to add:
#define DYN_ARR_VALUE_TYPE struct foo
#define DYN_ARR_TYPE_NAME foo_arr
#define DYN_ARR_IMPLEMENTATION
#include "dyn_arr.h"
#undef DYN_ARR_IMPLEMENTATION
#undef DYN_ARR_TYPE_NAME
#undef DYN_ARR_VALUE_TYPE
```

The fuller implementation will instead be placed in the file by the `#include "dyn_arr.h"`, i.e. something like:

```c
struct foo_arr { size_t size; size_t buffer_size; /* ... */ struct foo *members[]; };
// ...
```

## Alternative generation methods

### Generate it once

You might opt to produce the definitions and/or implementations once, which might help code completion, and copy/paste them in the header file.
Both `gcc` or `tcc` or other compilers can help do this, via `-E` or similar flags. Here's one example of how to do it:

The definitions:

```bash
$ gcc -E -P -CC -D DYN_ARR_VALUE_TYPE='struct foo' -D DYN_ARR_TYPE_NAME=foo_arr dyn_arr.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$'
struct foo_arr;
extern
struct foo_arr *foo_arr_new(size_t size, size_t buffer_size, size_t min_buffer);
# ...
```

The implementation:

```bash
$ gcc -E -P -CC -D DYN_ARR_VALUE_TYPE='struct foo' -D DYN_ARR_TYPE_NAME=foo_arr -D DYN_ARR_IMPLEMENTATION dyn_arr.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$'
struct foo_arr { /* ... */ };
struct foo_arr *foo_arr_new(size_t size, size_t buffer_size, size_t min_buffer);
# ...
```

### Integrate it in your build system

You can instruct your build system to automatically generate the header file and the implementation file.

If you use a `Makefile`, you can use this as a starting point. This will rebuild the header file and the implementation file whenever the `Makefile` itself changes (as there might be changes in the `-D ...` defines) or the `dyn_arr.h` changes (as you might get a new version, or tweak it). You might want to ignore those generated files in your build system.

```Makefile
foo_arr.h: Makefile dyn_arr.h
	@rm -f $@
	@printf '// Automatically generated from %s.\n' "$^" > $@
	@printf '// Automatically generated on   %s\n' "$$(date)" >> $@
	@printf '#ifndef FOO_ARR_H\n#define FOO_ARR_H\n' >> $@
	@gcc -E -P -CC -D DYN_ARR_VALUE_TYPE='struct foo' -D DYN_ARR_TYPE_NAME=foo_arr dyn_arr.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$$' >> $@
	@printf '#endif\n' >> $@
	@chmod -w $@

foo_arr.c: Makefile dyn_arr.h foo_arr.h
	@rm -f $@
	@printf '// Automatically generated from %s.\n' "$^" > $@
	@printf '// Automatically generated on   %s\n' "$$(date)" >> $@
	@printf '#include "your_project_other_includes.h"\n' >> $@
	@printf '#include "foo_arr.h"\n' >> $@
	@gcc -E -P -CC -D DYN_ARR_VALUE_TYPE=struct foo' -D DYN_ARR_TYPE_NAME=foo_arr -D DYN_ARR_IMPLEMENTATION dyn_arr.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$$' >> $@
	@chmod -w $@
```

# Functions

The header also declares, and makes available, functions to interact with the array-like struct: push, pop, etc.

All functions here are mentioned by suffix. Your `-D DYN_ARR_TYPE_NAME=foo_arr` will create functions whose name _starts_ with the value you picked `foo_arr`, but whose suffix is constant, i.e. the `_new` will be available to you as `foo_arr_new`.

## `_new` & `_free`

`_new` creates a new array-like struct, ready to accept (via `_push`, etc) `DYN_ARR_VALUE_TYPE` items in it.

You'll have to specify:

- a `size_t size` for how many _member pointers_ you'd like preallocated
- a `size_t buffer_size` for how many _member pointers_ should be left "untouched" to start with, at either end
- a `size_t min_buffer` to provide a minimum trigger at which point the array will be reallocated with twice the `size` to ensure more members can fit in it.

Once done with the struct, remember to use the `_free` function on it.

Note that the `_free` function will **not** free the `DYN_ARR_VALUE_TYPE` values which you might have placed in it. You'll have to do that yourself, separately!

## `_empty`, `_length`, and `_size`

- `_empty` returns whether the array has any items in it or not.
- `_length` returns the amount of **actually filled** "member slots" in the array.
- `_size` returns the amount of actual "member slots" in it.

Neither needs to iterate the array to provide its answer.

## `_push` and `_unshift`

These operations insert a new member at the end of, and at the start of, the array, respectively.

        v-- unshift adds here
    ...[ ] [1] [2] [3] [ ]...
                        ^--- push adds here

If the operation would cause the amount of "buffer" at either end to go at or below the initial `min_buffer`, the array will be resized to twice the size, and a new one returned.

## `_get`

This operation returns the member item at the given `size_t idx`, or `NULL` if the `idx` is out of bounds (`0`..`_length`).

This does _not_ remove the member from the array. It merely _gets its value_.

## `_set`

This operation sets the member item at the given `size_t idx`. It _does nothing_ if the `idx` is out of bounds (`0`..`_length`).

You can only `_set` an `idx` if it previously contained something. Else, see `_push` or `_unshift`.

This does _not_ free the previous member from the array. That's up to you to do.

## `_del`

This operation removes the member at the given `size_t idx` from the array, and shifts the contents to the left as a result, to fill the gap.

This does _not_ free the now-gone member from the array. That's up to you to do.

## `_front` and `_back`

These operations return the member item at the start of, and at the end of, the array, respectively.

            v-- front returns this
    ...[ ] [1] [2] [3] [ ]...
                    ^--- back returns this

## `_shift` and `_pop`

These operations _remove_ **and** _return_ a member item from the end of, or the start of, the array, respectively.

            v-- shift removes and returns this
    ...[ ] [1] [2] [3] [ ]...
                    ^--- pop removes and returns this

The array's `_length` is modified as a byproduct of this operation.

## `_unique`

This somewhat useful function removes, _shifting left_ the remaining members as required, any duplicate member item in the array.

## `_fill`

This somewhat useful function fills, _appending_ if the array is not empty, `size` amount of `value`s to the array. Uses `_push` internally.
The array's `_length` is modified as a byproduct of this operation.

As per `_push`, if the operation would cause the amount of "buffer" at either end to go at or below the initial `min_buffer`, the array will be resized to twice the size, and a new one returned.

## `_clear`

This function removes all elements from the array. The array's `_length` is modified as a byproduct of this operation.
The member items are **not** freed by this operation. You'll have to do that yourself.

## `_remove_all`

This somewhat useful function removes, _shifting left_ the remaining members as required, any member item which matches the given `value` in the array.

## `_iterate_ftl` and `_iterate_ltf`

Iterates the array, calling a given "func" (with user-provided parameter "param") for each item "val" in the list, **stopping** if the "func" returns non-zero. The array is **not** modified by the iteration. Variations:

- `_iterate_ftl` iterates the array from first to last.
- `_iterate_ltf` iterates the array from last to first.

## `_status`

This not-so-useful function prints out the "status" of the array to STDOUT, with an overview and one line per either empty or filled member. Useful for debugging the innards of this header file.

# LICENSE

The MIT License (MIT)

Copyright (c) 2024 Marco Fontani, MFONTANI@cpan.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
