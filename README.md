# chol - my C Header-Only Libraries

C header-only libraries I've created, used, and which might be useful to others.

See each subdirectory for more information, usage, etc.

# Nota Bene

Most should be in working condition, but might lack tests or examples or might just be of a bad quality.

Use at your peril, and/or submit issues and pull requests.

# What is included?

## `dyn_dllist`

A C "dynamic" doubly-linked list header-only library.

You choose the type of the member item and the new struct name. You get a bunch of functions you can use to interact with that new struct type: create one, free one, prepend or append an item, iterate the list, or even grep it returning a new one.

See [dyn_dllist/README.md](dyn_dllist/README.md) for more information.

## `dyn_kvp`

A C "dynamic" key/value pair (aka "hash" or "dict") library.

You choose the type of the "value" of the pair (the "key" defaults to "unsigned int") and the new struct name. Optionally you can also choose the type of the "key" of the pair and provide a hashing function. You get a bunch of functions you can use to interact with that new struct type: create one, free one, set or get or delete an item from the hash, but also perform intersection or merging into new structures.

See [dyn_kvp/README.md](dyn_kvp/README.md) for more information.

## `tap`

This is mostly for internal use by this repository, but might be useful to others.

A minimalistic Test Anything Protocol for C.

You include the header, and gain the ability to call functions like `test_is_int(a, b, "test name")` and ultimately `tap_done_testing()` once done.

You gain the ability to `prove ./a.out :: some parameters` and have a nice output for your tests' failures.

See [tap/README.md](tap/README.md) for more information.

# LICENSE

The MIT License (MIT)

Copyright (c) 2024 Marco Fontani, MFONTANI@cpan.org

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
