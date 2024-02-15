# `sagoma.h` - a simple C block profiler

A somewhat simple, but powerful, profiler which doesn't waste too many resources.

Use it to profile what you need. As little or as much as you need.

NOTA BENE: The more features you enable, the more impact the profiler _will_ have on your code!

# NOTA BENE

This is a work in progress. Much needs documented still.

# Configuration

If you'd like profiling to be "fully" performed, you'll want to `#define SAGOMA` before `#include "sagoma.h"`. In that case, you'll need to compile with `-lm`.

Otherwise, it'll only print the overall run time for the `sagoma` section. You can opt out of that via `#define SAGOMA_DONT_PRINT`.

Other configuration options are:

- `#define SAGOMA_ASSERT` - asserts that there's as many `sagoma_block` as `sagoma_block_done`, etc. You likely want to enable this while you're setting up the profiler in your code.
- `#define SAGOMA_TRACK_STATS` - use this if you'd like to keep track of the min/max time taken in each block. This doesn't come for free.
- `#define SAGOMA_CLOCK_CYCLES` - use this if you'd like to look at the "real" `__rdtsc()` "time" spent for each block
- `#define CPU_FREQ_ESTIMATE_WAIT_MSECS 100` how many milliseconds to spend estimating the CPU clock time. About 100ms does it. Only called once per program run, when stats are printed.

# Minimal "easy" usage

## `sagoma_this` and `sagoma_this_block`

The `sagoma_this(code)` outputs the total time of the `code`. You can add `sagoma_this_block("block name", code)` inside it, to separately keep track of sub-blocks.

<details>
<summary>Example `sagoma_this` and `sagoma_this_block` code and output.</summary>

```c
#include "sagoma.h"
void a_function(void)
{
    sagoma_this(
        (void)fprintf(stderr, "a_function called!\n");
    );
}
int main(void)
{
    // This takes 100ms. Best done outside of any profiling:
    sagoma__estimate_cpu_timer_frequency();
    // ... other code
    sagoma_this(
        for (int i = 0; i < 2; i++)
            a_function();
    );
    // ... other code
}
```

This will output the total time taken by each `sagoma_this` call. Assuming you've saved it as `1.c` and compiled it:

```
$ ./a.out
a_function called!
@1.c:4: Total: 0.000023503 s = 23.5026 us
a_function called!
@1.c:4: Total: 0.000000392 s = 0.3919 us
@1.c:13: Total: 0.000048389 s = 48.3887 us
```

</details>

If you'd like many more details, you'll have to `#define SAGOMA`.

<details>
<summary>Example with `#define SAGOMA`</summary>

```c
#define SAGOMA
#include "sagoma.h"
void a_function(void)
{
    sagoma_this(
        (void)fprintf(stderr, "a_function called!\n");
    );
}
int main(void)
{
    // This takes 100ms. Best done outside of any profiling:
    sagoma__estimate_cpu_timer_frequency();
    // ... other code
    sagoma_this(
        for (int i = 0; i < 2; i++)
            a_function();
    );
    // ... other code
}
```

This will output the total time taken by each `sagoma_this` call _as well as_ children time for each `sagoma_this_block`. Assuming you've saved it as `2.c` and compiled it, this time with `-lm`:

```
$ ./a.out
a_function called!
@2.c:5: Total:        0.000020019 s = 20.0193 us
 0: @2.c:5            0.000019985 s = 19.9846 us  99.83%
a_function called!
@2.c:5: Total:        0.000020623 s = 20.6229 us
 0: @2.c:5         2x 0.000020565 s = 20.5648 us  99.72%                                        = 0.000010282 s = 10.2824 us/it
@2.c:14: Total:        0.000069644 s = 69.6438 us
 0: @2.c:14            0.000069584 s =  69.5844 us  99.91%
```

</details>

<details>
<summary>Even more details with `#define SAGOMA_CLOCK_CYCLES`:</summary>

```c
#define SAGOMA
#define SAGOMA_CLOCK_CYCLES
#include "sagoma.h"
void a_function(void)
{
    sagoma_this(
        (void)fprintf(stderr, "a_function called!\n");
    );
}
int main(void)
{
    // This takes 100ms. Best done outside of any profiling:
    sagoma__estimate_cpu_timer_frequency();
    // ... other code
    sagoma_this(
        for (int i = 0; i < 2; i++)
            a_function();
    );
    // ... other code
}
```

This will output something similar to the previous example, _but_ with added cycle counts. Assuming you've saved it as `3.c` and compiled it, this time with `-lm`:

```
$ ./a.out
a_function called!
@3.c:6: Total:          69702C 0.000023270 s = 23.2696 us
 0: @3.c:6              69564C 0.000023223 s = 23.2235 us  99.80%
a_function called!
@3.c:6: Total:          98094C 0.000032748 s = 32.7480 us
 0: @3.c:6         2x   97798C 0.000032649 s =  32.6492 us  99.70%                                           =  48899C 0.000016325 s = 16.3246 us/it
@3.c:15: Total:         284660C 0.000095032 s = 95.0319 us
 0: @3.c:15             284390C 0.000094942 s =  94.9417 us  99.91%
```

</details>

The above might not look like much, and doesn't look much like C, either.

Note though that the `a_function` times _properly_ captured the amount taken inside the blocks, and are unaffected by the time spent outside that function. Note also that multiple `sagoma_this` can coexist in the same file.

## `sagoma_ez` functions

The `sagoma_ez`, `sagoma_ez_end`, `sagoma_ez_print`, `sagoma_ez_print_color`, `sagoma_ez_block`, `sagoma_ez_block_done`, `sagoma_ez_loop`, `sagoma_ez_print_loop` and `sagoma_ez_print_loop_color` macros offer quite a bit more customization than the previous ones (which internally use these).

<details>
<summary>Overview of the `sagoma_ez*` functions:</summary>

- `sagoma_ez("whole profile name")` creates a new `Sagoma` structure called `sagoma`, and starts a profile block for it.
- `sagoma_ez_end()` ends the `sagoma` profile block. `sagoma_ez_print` or `sagoma_ez_print_color` will also close it for you if you need to.
- `sagoma_ez_print()` and `sagoma_ez_print_color()` will output statistics without and with color, respectively.

If you also want to profile blocks, you can use these _between_ `sagoma_ez` and `sagoma_ez_end`:

- `sagoma_ez_block("block name")` to start a block
- `sagoma_ez_block_done()` to end the block

Be sure to `#define SAGOMA_ASSERT` and ensure the amount of `sagoma_ez_block_done` corresponds to the amount of `sagoma_ez_block` opened!

If you're profiling the innards of a function that gets called from the outside multiple times, and you'd only like to print statistics for it under certain conditions, you'll instead want to:

- use `sagoma_ez_loop("whole profile name")` _instead of_ `sagoma_ez("whole profile name")` to set up the `sagoma` profile in such a way that it's declared only once, and so that any time spent "outside" the function isn't counted as part of the profile; then
- use `sagoma_ez_print_loop(when)` or `sagoma_ez_print_loop_color(when)` instead of `sagoma_ez_print` or `sagoma_ez_print_color`. You'll have to provide an _expression_, `when`, to decide _when_ the statistics should be printed. As an example, in a game loop you might want to print statistics every second, or every 10th call of the function.

</details>

## The `sagoma` functions

The real workhorse is the `sagoma*` functions/macros, which allow you to slice and dice the profiler any which way you want _including_ providing a global profile, interleave blocks, etc.

You can use `sagoma_init(varname, name)` to declare and initialize a Sagoma struct, and make it available as `varname` for subsequent `sagoma_*` calls, which take `varname` as their first argument.

Internally, but still part of the "API", there's also:

- `sagoma_declare(varname)` declares a Sagoma struct
- `sagoma_init_only(varname, name)` initializes the Sagoma struct

To profile a block, you can use:

- `sagoma_block(varname, name)` to start the block you want to profile. The `varname` is the same as the one you used in `sagoma_init`. `name` is the name of the block you want to profile, but it's a variable name, so that different blocks can have different names and the same name can be reused in different scopes, with no warnings/errors about reusing the same underlying variable name.
- `sagoma_block_done(varname)` ends the block you started with `sagoma_block`.
- `sagoma_end(varname)` ends the profiling session, but that's also done by:
- `sagoma_print(varname, ignore_time, ignore_index)` (and `sagoma_print_color`, for colored output) prints the profiling info for the Sagoma struct you initialized with `sagoma_init` and for the blocks you started with `sagoma_block`.

To profile something (i.e. a function) that gets called _from the outside_ multiple times, you can use the verbose "loop" API:

- `sagoma_init_loop(varname, name)` (use it instead of `sagoma_init`!) declares and initializes a Sagoma struct for use within a function that you want to analyze the inside bits of, but you don't want to take into consideration the time the program takes when outside of it. This internally declares a static Sagoma struct, and a static "boolean" variable, to keep track of whether the Sagoma struct has been initialized, and how much time was spent outside of the function, to ensure that statistics reflect the time spent inside the function only.
- you then use `sagoma_block(...)` and `sagoma_block_done(...)`, as above
- if you want to print statistics every time the function is called, you can just use `sagoma_print(...)` or `sagoma_print_color(...)`, as above.
- `sagoma_print_loop(varname, when)` (and `sagoma_print_loop_color`, for colored output) instead prints the profiling info for the Sagoma struct you initialized with `sagoma_init_loop` only when `when` is true. As an example, you might only print statistics if a global boolean was toggled, or every few iterations. This makes it easier to do that.

See the [tests/](tests/) and [examples/](examples/) subdirectories for more examples.

# LICENSE

The MIT License (MIT)

Copyright (c) 2024 Marco Fontani, MFONTANI@cpan.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
