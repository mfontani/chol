# `dyn_kvp.h` - C "dynamic" key/value "hash" library

This "macro-ed" header file creates definitions and implementations for a key/value "hash" that has a default `unsigned int` key (overridable via `DYN_KVP_KEY_TYPE`), a given `DYN_KVP_VALUE_TYPE *` as "value", and where the name for `struct`s and functions are "templated" via the given `DYN_KVP_TYPE_NAME`, providing type safety.

The system only helps manage (via `_new` and `_free` suffixed function names) the lifecycle of the "hash" itself, NOT of the members you put in them. That is up to your code.

# Minimal usage

Say you've got just one `main.c` file, containing a struct declaration for which you'd like to also define a key/value type for, i.e.:

```c
struct foo { ... };
```

The `DYN_KVP_VALUE_TYPE` for this will be `struct foo`, and the hash will contain a pointer to one.

You'll have to pick a name for the new _struct name_ (and related _functions' prefix_). Let's pick `foo_kvp` for it: the `DYN_KVP_TYPE_NAME` you'll want to specify will be `foo_kvp`.

You'll have to copy the `dyn_kvp.h` file to the same directory as `main.c`, and write in `main.c`:

```c
// Struct definitions:
#define DYN_KVP_VALUE_TYPE struct foo
#define DYN_KVP_TYPE_NAME foo_kvp
#include "dyn_kvp.h"
// Actual implementation:
#define DYN_KVP_IMPLEMENTATION
#include "dyn_kvp.h"
#undef DYN_KVP_IMPLEMENTATION
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE
```

In your code, you'll then use `struct foo_kvp *` to declare a variable of the newly created type, and will use functions named `foo_kvp_*` (or `foo_kvp__member_*`, for the "explosion" of it) to interact with it:

```c
// Create one of your pre-existing structs, somehow:
struct foo *val = create_a_foo(/*...*/); // or malloc(sizeof(struct foo)) or whatever
// Here's what the "dyn_kvp.h" gives you. Create a hash with 8 buckets:
struct foo_kvp *hash = foo_kvp_new(8);
// Set "1234" to that value:
foo_kvp_set(1234, val);
// Do stuff with it
// Remove "val" from the hash:
foo_kvp_del(1234);
// Free the whole hash (but NOT the contents!)
foo_kvp_free(hash);
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
#define DYN_KVP_VALUE_TYPE struct foo
#define DYN_KVP_TYPE_NAME foo_kvp
#include "dyn_kvp.h"
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE
```

That `#include "dyn_dll.h"` will result in something like this being inserted in the header:

```c
struct foo_kvp;
extern struct foo_kvp *foo_kvp_new(void);
extern void foo_kvp_free(struct foo_kvp *hash);
// ...
```

Multiple such `#include "dyn_kvp.h"` (for **different** `DYN_KVP_VALUE_TYPE`s!) can be placed in the same header file.
Remember to `#undef`, then `#define` again, properties for the new type(s).

## Implementation

Similarly to the definition, you'll likely want to put the implementation in one `.c` file:

```c
// Your pre-existing headers, giving access to "struct foo" definition:
#include "your_other_headers.h"
// Your "kvp" header, which gives you access to the foo_kvp struct declaration:
#include "foo_kvp.h"
// What you need to add:
#define DYN_KVP_VALUE_TYPE struct foo
#define DYN_KVP_TYPE_NAME foo_kvp
#define DYN_KVP_IMPLEMENTATION
#include "dyn_kvp.h"
#undef DYN_KVP_IMPLEMENTATION
#undef DYN_KVP_TYPE_NAME
#undef DYN_KVP_VALUE_TYPE
```

The fuller implementation will instead be placed in the file by the `#include "dyn_kvp.h"`, i.e. something like:

```c
// ...
struct foo_kvp *foo_kvp_new(size_t size);
// ...
```

## Alternative generation methods

### Generate it once

You might opt to produce the definitions and/or implementations once, which might help code completion, and copy/paste them in the header file.
Both `gcc` or `tcc` or other compilers can help do this, via `-E` or similar flags. Here's one example of how to do it:

The definitions:

```bash
$ gcc -E -P -CC -D DYN_KVP_VALUE_TYPE='struct foo' -D DYN_KVP_TYPE_NAME=foo_kvp dyn_kvp.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$'
# ...
extern
struct foo_kvp *foo_kvp_new(size_t size);
# ...
```

The implementation:

```bash
$ gcc -E -P -CC -D DYN_KVP_VALUE_TYPE='struct foo' -D DYN_KVP_TYPE_NAME=foo_kvp -D DYN_KVP_IMPLEMENTATION dyn_kvp.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$'
# ...
struct foo_kvp *foo_kvp_new(void)
{
# ...
}
# ...
```

### Integrate it in your build system

You can instruct your build system to automatically generate the header file and the implementation file.

If you use a `Makefile`, you can use this as a starting point. This will rebuild the header file and the implementation file whenever the `Makefile` itself changes (as there might be changes in the `-D ...` defines) or the `dyn_kvp.h` changes (as you might get a new version, or tweak it). You might want to ignore those generated files in your build system.

```Makefile
foo_kvp.h: Makefile dyn_kvp.h
	@rm -f $@
	@printf '// Automatically generated from %s.\n' "$^" > $@
	@printf '// Automatically generated on   %s\n' "$$(date)" >> $@
	@printf '#ifndef FOO_KVP_H\n#define FOO_KVP_H\n' >> $@
	@gcc -E -P -CC -D DYN_KVP_VALUE_TYPE='struct foo' -D DYN_KVP_TYPE_NAME=foo_kvp dyn_kvp.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$$' >> $@
	@printf '#endif\n' >> $@
	@chmod -w $@

foo_kvp.c: Makefile dyn_kvp.h foo_kvp.h
	@rm -f $@
	@printf '// Automatically generated from %s.\n' "$^" > $@
	@printf '// Automatically generated on   %s\n' "$$(date)" >> $@
	@printf '#include "your_project_other_includes.h"\n' >> $@
	@printf '#include "foo_kvp.h"\n' >> $@
	@gcc -E -P -CC -D DYN_KVP_VALUE_TYPE=struct foo' -D DYN_KVP_TYPE_NAME=foo_kvp -D DYN_KVP_IMPLEMENTATION dyn_kvp.h | perl -0777 -pe's,/[*].*?[*]/,,gs' | grep -v -e '^\s*$$' >> $@
	@chmod -w $@
```

# Hash Functions

The header also declares, and makes available, functions to interact with the key/value hash: set, del, get, etc.

All functions here are mentioned by suffix. Your `-D DYN_KVP_TYPE_NAME=foo_kvp` will create functions whose name _starts_ with the value you picked `foo_kvp`, but whose suffix is constant, i.e. the `_new` will be available to you as `foo_kvp_new`.

## `_new` & `_free`

`_new` creates a new key/value hash, ready to accept (via `_set`, etc) `DYN_KVP_VALUE_TYPE` items in it.

Once done with the hash, remember to use the `_free` function on it.

Note that the `_free` function will **not** free the `DYN_KVP_VALUE_TYPE` values which you might have placed in the hash. You'll have to do that yourself, separately!

## `_set`, `_get` and `_del`

The backbone of a hash.

- `_set` lets you set (or override, if already present) a given "key" with a new "value".
- `_get` returns the (possibly NULL, if not in the hash) "value" corresponding to a given "key".
- `_del` removes the (possibly not found, but it doesn't matter) "value" corresponding to a given "key". It doesn't "free" the value. That's up to you!

# Member Functions

It's often useful to "get" all key/values from a hash, to then do whatever one wants.

A `struct foo_kvp_kv` is provided, which contains the "key", "value", and a useless (to you) "next" pointer.

You can `_kv_get` to get a `struct foo_kvp_kv **` which contains all the key/values in the hash, ended by a `NULL` pointer.

Once done with it, you can `_kv_free` it.

# LICENSE

The MIT License (MIT)

Copyright (c) 2024 Marco Fontani, MFONTANI@cpan.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.