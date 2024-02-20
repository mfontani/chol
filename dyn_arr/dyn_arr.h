/*
 * `dyn_arr.h` - C "dynamic" array-like header-only library
 * Copyright (c) 2024 Marco Fontani - MFONTANI@cpan.org
 */

#ifndef DYN_ARR_VALUE_TYPE
#error "DYN_ARR_VALUE_TYPE must be defined before including dyn_arr.h"
#endif

#ifndef DYN_ARR_TYPE_NAME
#error "DYN_ARR_TYPE_NAME must be defined before including dyn_arr.h"
#endif

#ifdef DYN_ARR_IMPLEMENTATION
# define DYN_ARR_EXTERN
#else
# define DYN_ARR_EXTERN extern
#endif

#ifdef DYN_ARR__
#error "dyn_arr.h uses DYN_ARR__ as a macro, but it's already defined"
#endif

#define DYN_ARR__(x,y) x##y

#ifdef DYN_ARR_
#error "dyn_arr.h uses DYN_ARR_ as a macro, but it's already defined"
#endif

#define DYN_ARR_(x,y) DYN_ARR__(x,y)

#ifdef DYN_ARR_F
#error "dyn_arr.h uses DYN_ARR_F as a macro, but it's already defined"
#endif

#define DYN_ARR_F(x, y) DYN_ARR_(x, y)

#ifdef DYN_ARR_IMPLEMENTATION

struct DYN_ARR_TYPE_NAME {
    // Setup data:
    size_t size;        // Allocated size for "members"
    size_t buffer_size; // Size of the left/right initial "buffer"
    size_t min_buffer;  // Minimum size of the left/right "buffer" to trigger a resize
    // Implementation data:
    size_t length;      // Used size for "members"
    size_t istart;      // Index of the first element
    size_t iend;        // Index of the last element
    DYN_ARR_VALUE_TYPE *members[];  // of size "size"
};

#else

struct DYN_ARR_TYPE_NAME;

#endif

DYN_ARR_EXTERN
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _new)(size_t size, size_t buffer_size, size_t min_buffer);
DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _free)(struct DYN_ARR_TYPE_NAME *arr);
DYN_ARR_EXTERN
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _resize)(struct DYN_ARR_TYPE_NAME *arr, size_t new_size, size_t new_buffer_size, size_t new_min_buffer);
DYN_ARR_EXTERN
size_t DYN_ARR_F(DYN_ARR_TYPE_NAME, _length)(struct DYN_ARR_TYPE_NAME *arr);
DYN_ARR_EXTERN
size_t DYN_ARR_F(DYN_ARR_TYPE_NAME, _size)(struct DYN_ARR_TYPE_NAME *arr);
DYN_ARR_EXTERN
int DYN_ARR_F(DYN_ARR_TYPE_NAME, _empty)(struct DYN_ARR_TYPE_NAME *arr);
DYN_ARR_EXTERN
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _push)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *value);
DYN_ARR_EXTERN
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _unshift)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *value);
DYN_ARR_EXTERN
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _front)(struct DYN_ARR_TYPE_NAME *arr);
DYN_ARR_EXTERN
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _back)(struct DYN_ARR_TYPE_NAME *arr);
DYN_ARR_EXTERN
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _get)(struct DYN_ARR_TYPE_NAME *arr, size_t idx);
DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _set)(struct DYN_ARR_TYPE_NAME *arr, size_t idx, DYN_ARR_VALUE_TYPE *value);
DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _del)(struct DYN_ARR_TYPE_NAME *arr, size_t idx);
DYN_ARR_EXTERN
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _shift)(struct DYN_ARR_TYPE_NAME *arr);
DYN_ARR_EXTERN
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _pop)(struct DYN_ARR_TYPE_NAME *arr);

DYN_ARR_EXTERN
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _fill)(struct DYN_ARR_TYPE_NAME *arr, size_t size, DYN_ARR_VALUE_TYPE *value);

DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _remove_all)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *value);

DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _unique)(struct DYN_ARR_TYPE_NAME *arr);

DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _clear)(struct DYN_ARR_TYPE_NAME *arr);

DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _iterate_ftl)(struct DYN_ARR_TYPE_NAME *arr, int (*func)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *val, void *param), void *param);
DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _iterate_ltf)(struct DYN_ARR_TYPE_NAME *arr, int (*func)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *val, void *param), void *param);

DYN_ARR_EXTERN
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _status)(struct DYN_ARR_TYPE_NAME *arr);

#undef DYN_ARR_EXTERN

#ifdef DYN_ARR_IMPLEMENTATION

// Create a new "array" of this type. Remember to _free() it!
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _new)(size_t size, size_t buffer_size, size_t min_buffer)
{
    struct DYN_ARR_TYPE_NAME *arr = malloc(
        sizeof(struct DYN_ARR_TYPE_NAME) +
        sizeof(DYN_ARR_VALUE_TYPE *) * size +
        0
    );
    if (!arr)
    {
        perror("_new: malloc[arr]");
        exit(EXIT_FAILURE);
    }
    if (min_buffer == 0)
        min_buffer = 1;
    if (buffer_size == 0)
        buffer_size = 1;
    if (buffer_size < min_buffer)
        buffer_size = min_buffer + 2;
    if (size < buffer_size)
        size = buffer_size * 2;
    arr->size = size;
    arr->buffer_size = buffer_size;
    arr->min_buffer = min_buffer;
    arr->length = 0;
    arr->istart = buffer_size - 1;
    arr->iend = arr->istart;
    for (size_t i = 0; i < size; i++)
        arr->members[i] = NULL;
    return arr;
}

// Free an "array" of this type, including inner members but NOT including the
// values in them. That's up to the caller!
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _free)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return;
    free(arr);
}

// Create a new array with the given parameters, copying the values from the
// old one. Remember to _free() it!
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _resize)(struct DYN_ARR_TYPE_NAME *arr, size_t new_size, size_t new_buffer_size, size_t new_min_buffer)
{
    struct DYN_ARR_TYPE_NAME *new_arr = DYN_ARR_F(DYN_ARR_TYPE_NAME, _new)(new_size, new_buffer_size, new_min_buffer);
    size_t j = new_arr->istart;
    for (size_t i = arr->istart; i <= arr->iend; i++, j++)
        new_arr->members[j] = arr->members[i];
    new_arr->iend = new_arr->istart + arr->length - 1;
    new_arr->length = arr->length;
    return new_arr;
}

// Returns the amount of actually filled "member slots" in the array.
// Doesn't need to iterate it.
size_t DYN_ARR_F(DYN_ARR_TYPE_NAME, _length)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return 0;
    return arr->length;
}

// Returns whether the array is empty.
// Doesn't need to iterate it.
int DYN_ARR_F(DYN_ARR_TYPE_NAME, _empty)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return 0;
    return !arr->length;
}

// Returns the amount of possible "member slots" in the array.
// Doesn't need to iterate it.
size_t DYN_ARR_F(DYN_ARR_TYPE_NAME, _size)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return 0;
    return arr->size;
}

// Push a value at the end of the array.
// Note this could return a NEW array!
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _push)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *value)
{
    if (!arr)
        return NULL;
    if (arr->iend + 1 >= arr->size - arr->min_buffer)
    {
        struct DYN_ARR_TYPE_NAME *new_arr = DYN_ARR_F(DYN_ARR_TYPE_NAME, _resize)(arr, arr->size * 2, arr->buffer_size, arr->min_buffer);
        DYN_ARR_F(DYN_ARR_TYPE_NAME, _free)(arr);
        arr = new_arr;
    }
    if (arr->length)
        arr->iend++;
    arr->members[arr->iend] = value;
    arr->length++;
    return arr;
}

// Prepend (unshift) a value at the beginning of the array.
// Note this could return a NEW array!
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _unshift)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *value)
{
    if (!arr)
        return NULL;
    if (arr->istart <= arr->min_buffer - 1)
    {
        struct DYN_ARR_TYPE_NAME *new_arr = DYN_ARR_F(DYN_ARR_TYPE_NAME, _resize)(arr, arr->size * 2, arr->buffer_size, arr->min_buffer);
        DYN_ARR_F(DYN_ARR_TYPE_NAME, _free)(arr);
        arr = new_arr;
    }
    if (arr->length)
        arr->istart--;
    arr->members[arr->istart] = value;
    arr->length++;
    return arr;
}

// Get the _first_ value from the array.
// Could be NULL if the index is out of bounds.
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _front)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr || !arr->length)
        return NULL;
    return arr->members[arr->istart];
}

// Get the _last_ value from the array.
// Could be NULL if the index is out of bounds.
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _back)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr || !arr->length)
        return NULL;
    return arr->members[arr->istart + arr->length - 1];
}

// Get the value at index "idx" from the array.
// Could be NULL if the index is out of bounds.
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _get)(struct DYN_ARR_TYPE_NAME *arr, size_t idx)
{
    if (!arr)
        return NULL;
    if (idx >= arr->length)
        return NULL;
    return arr->members[arr->istart + idx];
}

// Set the value at index "idx" from the array (assumes "idx" is valid).
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _set)(struct DYN_ARR_TYPE_NAME *arr, size_t idx, DYN_ARR_VALUE_TYPE *value)
{
    if (!arr)
        return;
    if (idx >= arr->length)
        return;
    arr->members[arr->istart + idx] = value;
}

// Delete the value at index "idx" from the array (assumes "idx" is valid).
// The array is shifted to the left as a result, to fill the gap.
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _del)(struct DYN_ARR_TYPE_NAME *arr, size_t idx)
{
    if (!arr)
        return;
    if (idx >= arr->length)
        return;
    // Shift the array
    size_t j = arr->istart + idx + 1;
    while (j <= arr->iend)
    {
        arr->members[j - 1] = arr->members[j];
        j++;
    }
    arr->members[arr->iend] = NULL;
    arr->iend--;
    arr->length--;
}

// Shift the array to the left, removing (and returning!) the first element.
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _shift)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return NULL;
    if (!arr->length)
        return NULL;
    DYN_ARR_VALUE_TYPE *value = arr->members[arr->istart];
    arr->members[arr->istart] = NULL;
    arr->length--;
    if (arr->length)
        arr->istart++;
    return value;
}

// Remove, and return, the last element from the array.
DYN_ARR_VALUE_TYPE *DYN_ARR_F(DYN_ARR_TYPE_NAME, _pop)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return NULL;
    if (!arr->length)
        return NULL;
    DYN_ARR_VALUE_TYPE *value = arr->members[arr->iend];
    arr->members[arr->iend] = NULL;
    arr->length--;
    if (arr->length)
        arr->iend--;
    return value;
}

// Iterate the array from first to last, calling "func" for each element, and
// passing "param" to "func" for each iteration.
// If "func" returns a non-zero value, the iteration is stopped.
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _iterate_ftl)(struct DYN_ARR_TYPE_NAME *arr, int (*func)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *val, void *param), void *param)
{
    if (!arr)
        return;
    if (!func)
        return;
    if (!arr->length)
        return;
    for (size_t i = arr->istart; i <= arr->iend; i++)
        if (func(arr, arr->members[i], param))
            return;
}

// Iterate the array from last to first, calling "func" for each element, and
// passing "param" to "func" for each iteration.
// If "func" returns a non-zero value, the iteration is stopped.
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _iterate_ltf)(struct DYN_ARR_TYPE_NAME *arr, int (*func)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *val, void *param), void *param)
{
    if (!arr)
        return;
    if (!func)
        return;
    if (!arr->length)
        return;
    for (size_t i = arr->iend; i >= arr->istart; i--)
        if (func(arr, arr->members[i], param))
            return;
}

// Pre-fills the array with "size" elements, all set to "value".
// If the array already has elements, they are pushed at the end.
// Shortcut for readability. Uses _push internally.
struct DYN_ARR_TYPE_NAME *DYN_ARR_F(DYN_ARR_TYPE_NAME, _fill)(struct DYN_ARR_TYPE_NAME *arr, size_t size, DYN_ARR_VALUE_TYPE *value)
{
    if (!arr)
        return NULL;
    for (size_t i = 0; i < size; i++)
        arr = DYN_ARR_F(DYN_ARR_TYPE_NAME, _push)(arr, value);
    return arr;
}

// Remove all the elements from the array that match the given value.
// The array is shifted to the left after every removal, to fill the gap.
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _remove_all)(struct DYN_ARR_TYPE_NAME *arr, DYN_ARR_VALUE_TYPE *value)
{
    if (!arr)
        return;
    if (!arr->length)
        return;
    size_t i = arr->istart;
    while (i <= arr->iend)
    {
        if (arr->members[i] == value)
        {
            // Remove this value
            arr->members[i] = NULL;
            // Shift the array
            size_t j = i + 1;
            while (j <= arr->iend)
            {
                arr->members[j - 1] = arr->members[j];
                j++;
            }
            arr->iend--;
            arr->length--;
        }
        else
        {
            i++;
        }
    }
}

// Remove all the duplicate elements from the array.
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _unique)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return;
    if (!arr->length)
        return;
    size_t i = arr->istart;
    while (i <= arr->iend)
    {
        size_t j = i + 1;
        while (j <= arr->iend)
        {
            if (arr->members[i] == arr->members[j])
            {
                // Remove this value
                arr->members[j] = NULL;
                // Shift the array
                size_t k = j + 1;
                while (k <= arr->iend)
                {
                    arr->members[k - 1] = arr->members[k];
                    k++;
                }
                arr->iend--;
                arr->length--;
            }
            else
            {
                j++;
            }
        }
        i++;
    }
}

// Remove all the elements from the array.
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _clear)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
        return;
    if (!arr->length)
        return;
    for (size_t i = arr->istart; i <= arr->iend; i++)
        arr->members[i] = NULL;
    arr->length = 0;
    arr->istart = arr->buffer_size - 1;
    arr->iend = arr->istart;
}

// Print the status/dump of the array to stdout.
void DYN_ARR_F(DYN_ARR_TYPE_NAME, _status)(struct DYN_ARR_TYPE_NAME *arr)
{
    if (!arr)
    {
        printf("Array is NULL\n");
        return;
    }
    printf("Array: %p, length/size %zu/%zu, min_buffer/buffer_size %zu/%zu istart %zu iend %zu:\n",
        (void *)arr, arr->length, arr->size, arr->min_buffer, arr->buffer_size, arr->istart, arr->iend);
    for (size_t i = 0; i < arr->size; i++)
    {
        printf(" %3zu", i);
        if (i == arr->istart)
            printf(" [ ");
        else
            printf("   ");
        if (i == arr->iend)
            printf("]");
        else
            printf(" ");
        printf(" %p", (void *)arr->members[i]);
        if (i == arr->istart)
            printf(" <--- istart");
        if (i == arr->iend)
            printf(" <--- iend");
        printf("\n");
    }
}

#endif

#undef DYN_ARR_F
#undef DYN_ARR_
#undef DYN_ARR__
