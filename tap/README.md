# `tap.h` & `tap.c` - a minimalistic Test Anything Protocol for C

A couple _very_ simple/minimalistic `.c` and `.h` file to write TAP-like tests in C.

# Usage

```c
#include "tap.h"
void my_run_tests(void)
{
    int one = 1;
    int two = 2;
    if (!tap_is_int(one, two, "one == two"))
        tap_diag("one=%d != two=%d?!", one, two);
    char *foo = "foo";
    char *bar = foo;
    if (!tap_is_voidp(foo, bar, "foo == bar"))
        tap_diag("foo='%s' != bar='%s'?!", foo, bar);
    tap_done_testing();
}
// Maybe call it from main?
int main(void)
{
    my_run_tests();
}
```

This can then be compiled, and ran via prove:

```bash
prove ./program :: --params-for-program
```

# Functions & Macros

## `tap_done_testing`

You **have** to call this once you're done sending `tap_is_*` or `tap_diag`, so that `prove` can know your tests are over, and your program hasn't crashed (which signifies a failure).

## `tap_diag(fmt, args)`

A macro to standardize a printf-like strings, which also prints out "where" in the code the diagnostic is coming from.

```c
tap_diag("foo='%s' != bar='%s'?!", foo, bar);
```

Prints:

```
# foo='...' != bar='..' [ at foo.c:1234 ]
```

## `tap_is_int`

Sugar to check whether two `int` values are the same. Returns the result of the comparison, so it can be checked in order to provide further diagnostics in case the two values don't match, i.e.:

```c
int one1 = 1;
int one2 = one1;
if (!tap_is_int(one1, one2, "one1 == one2"))
    tap_diag("one1=%d != one2=%d?!", one1, one2);
```

If the comparison works, it prints:

```
ok 1 one1 == one2 [ at foo.c:1234 ]
```

If it fails (i.e. assume `int one2 = 2;`, above), it prints:

```
not ok 1 one1 == one2 (got 1, wanted 2) [ at foo.c:1234 ]
# one1=1 != one2=2?!
```

## `tap_is_voidp`

Sugar to check whether two `void *` values point to the same place. Returns the result of the comparison.

```c
char *foo = "foo";
char *bar = foo;
if (!tap_is_voidp(foo, bar, "foo == bar"))
    tap_diag("foo='%s' != bar='%s'?!", foo, bar);
```

If the comparison works, it prints:

```
ok 1 foo == bar [ at foo.c:1234 ]
```

If it fails (i.e. assume `char *bar = "bar";`, above), it prints:

```
not ok 1 foo == bar (got 0x12345, wanted 0xf0f0f0) [ at foo.c:1234 ]
# foo='foo' != bar='bar'?!
```

# LICENSE

The MIT License (MIT)

Copyright (c) 2024 Marco Fontani, MFONTANI@cpan.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
