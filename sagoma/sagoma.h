/*
 * sagoma.h
 * #define SAGOMA to enable.
 * Otherwise, you get time taken only.
 */

// If you're testing sagoma.h, you'll want to define this:
// #define SAGOMA_ASSERT

// #define SAGOMA
// #define SAGOMA_ASSERT
// #define SAGOMA_DEBUG

#include <x86intrin.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#ifdef SAGOMA_ASSERT
#include <assert.h>
#endif
#include <sys/time.h>
#include <math.h>

#ifdef SAGOMA_DEBUG
#define SAGOMA_DEBUG_DECLARE_VOID const char *caller_func_name, const char *caller_file_name, int caller_line_number
#define SAGOMA_DEBUG_DECLARE_PARAMS(...) __VA_ARGS__ , SAGOMA_DEBUG_DECLARE_VOID
// #define SAGOMA_DEBUGF(msg, ...) fprintf(stderr, "SDBG: %-25s %15s:%-4d: " msg "\n", __func__, __FILE__, __LINE__, __VA_ARGS__)
#define SAGOMA_DEBUGF(msg, ...) (void)fprintf(stderr, "SDBG: %-25s " msg "\n", __func__, __VA_ARGS__)
#define SAGOMA_DEBUGC(msg, ...) SAGOMA_DEBUGF("from %-15s %15s:%-4d " msg, caller_func_name, caller_file_name, caller_line_number, __VA_ARGS__)
#else
#define SAGOMA_DEBUG_DECLARE_VOID void
#define SAGOMA_DEBUG_DECLARE_PARAMS(...) __VA_ARGS__
#define SAGOMA_DEBUGF(msg, ...) do { } while (0)
#define SAGOMA_DEBUGC(msg, ...) do { } while (0)
#endif

#define USECS_PER_SECOND    1000000
#define USECS_PER_SECOND_F  1000000.0L
#define MSECS_PER_SECOND    1000

// Togglable amount of MSECS to busy spin, to estimate CPU frequency:
#ifndef CPU_FREQ_ESTIMATE_WAIT_MSECS
#define CPU_FREQ_ESTIMATE_WAIT_MSECS 100
#endif

#ifndef SAGOMA_DONT_PRINT

// Get current "OS time", in microseconds.
static uint64_t sagoma__get_os_timer(void)
{
    static struct timeval timeval_now;
    gettimeofday(&timeval_now, 0);
    uint64_t ostimer = timeval_now.tv_sec * USECS_PER_SECOND + timeval_now.tv_usec;
    return ostimer;
}

// Estimate CPU frequency by measuring how many TSC units are executed in a
// given amount of time. #define CPU_FREQ_ESTIMATE_WAIT_MSECS to change the
// amount of time we wait. Default is 100ms.
static uint64_t sagoma__estimate_cpu_timer_frequency(void)
{
    static uint64_t cpu_freq = 0;
    if (cpu_freq != 0)
        return cpu_freq;
    uint64_t ms_to_wait = CPU_FREQ_ESTIMATE_WAIT_MSECS;
    uint64_t cpu_start = __rdtsc();
    uint64_t os_start = sagoma__get_os_timer();
    uint64_t os_elapsed = 0;
    uint64_t os_wait_time = USECS_PER_SECOND * ms_to_wait / MSECS_PER_SECOND;
    while (os_elapsed < os_wait_time)
        os_elapsed = sagoma__get_os_timer() - os_start;
    uint64_t cpu_end = __rdtsc();
    uint64_t cpu_elapsed = cpu_end - cpu_start;
    if (os_elapsed)
        cpu_freq = USECS_PER_SECOND * cpu_elapsed / os_elapsed;
    return cpu_freq;
}

#endif

#ifdef SAGOMA

struct SagomaBlock
{
    const char *name;           // What is this block called?
    uint64_t total_time;        // How long did it take to run this block?
    uint64_t exclusive_time;    // Total time, minus the time spent in child blocks.
    size_t entrance_count;      // How many times did we enter this block?
    size_t stack_count;         // Internal counter.
    size_t indent;              // Indentation to use...
#ifdef SAGOMA_TRACK_STATS
    uint64_t min_time;          // Minimum cycles taken to run this block.
    uint64_t max_time;          // Maximum cycles taken to run this block.
#endif
};

struct SagomaStack
{
    struct SagomaBlock *block;  // What block do I belong to?
    uint64_t start_tsc;         // __rdtsc at start
    uint64_t child_time;        // Time spent in child blocks.
#ifdef SAGOMA_TRACK_STATS
    uint64_t min_time;          // Minimum cycles taken to run this stack
    uint64_t max_time;          // Maximum cycles taken to run this stack
#endif
};

#ifndef SAGOMA_BLOCK_CAPACITY
#define SAGOMA_BLOCK_CAPACITY 64
#endif

#ifndef SAGOMA_STACK_CAPACITY
#define SAGOMA_STACK_CAPACITY 8
#endif

#endif

struct Sagoma
{
    const char *name;             // What is this profile called?
    uint64_t start_tsc;     // __rdtsc at start
    uint64_t end_tsc;       // __rdtsc at end, 0 if not ended yet
#ifdef SAGOMA
    struct SagomaBlock blocks[SAGOMA_BLOCK_CAPACITY];   // The blocks we're profiling
    struct SagomaStack stacks[SAGOMA_STACK_CAPACITY];   // The stacks we're profiling
    size_t block_count;    // How many blocks have we profiled?
    size_t stack_count;    // How many stacks have we profiled?
#endif
};

static void sagoma_init_(SAGOMA_DEBUG_DECLARE_PARAMS(struct Sagoma *s, const char *name))
{
    memset(s, 0, sizeof(*s));
    s->name = name;
    s->start_tsc = __rdtsc();
#ifdef SAGOMA
#ifdef SAGOMA_TRACK_STATS
    for (int i = 0; i < SAGOMA_BLOCK_CAPACITY; i++)
    {
        s->blocks[i].min_time = UINT64_MAX;
        s->blocks[i].max_time = 0;
    }
    for (int i = 0; i < SAGOMA_STACK_CAPACITY; i++)
    {
        s->stacks[i].min_time = UINT64_MAX;
        s->stacks[i].max_time = 0;
    }
#endif
#endif
    SAGOMA_DEBUGC("--> [%s] start_tsc=%lu", s->name, s->start_tsc);
}

#define SAGOMA__(x,y) x##y
#define SAGOMA_(x,y) SAGOMA__(x, y)
#define SAGOMAV(x) SAGOMA_(x, tmp)

#define sagoma_declare(varname) struct Sagoma varname
#ifdef SAGOMA_DEBUG
#define sagoma_init_only(varname, name) \
    sagoma_init_(&(varname), name, __func__, __FILE__, __LINE__)
#else
#define sagoma_init_only(varname, name) \
    sagoma_init_(&(varname), name)
#endif
#define sagoma_init(varname, name) \
    sagoma_declare(varname); \
    sagoma_init_only(varname, name)

#ifdef SAGOMA
static size_t sagoma__next_block_index_(SAGOMA_DEBUG_DECLARE_PARAMS(struct Sagoma *s))
{
#ifdef SAGOMA_ASSERT
    assert(s->block_count < SAGOMA_BLOCK_CAPACITY);
#endif
    SAGOMA_DEBUGC("block_count=%zu, now %zu", s->block_count, s->block_count + 1);
    return s->block_count++;
}

#ifdef SAGOMA_DEBUG
#define sagoma__next_block_index(s) sagoma__next_block_index_(s, __func__, __FILE__, __LINE__)
#else
#define sagoma__next_block_index(s) sagoma__next_block_index_(s)
#endif

// Note: The string pointed to by 'name' should stay in scope
// as long as the profile object is used.
static void sagoma__block_start_(SAGOMA_DEBUG_DECLARE_PARAMS(struct Sagoma *s, const char *name, size_t block_index))
{
#ifdef SAGOMA_ASSERT
    assert(block_index < SAGOMA_BLOCK_CAPACITY);
#endif
    struct SagomaBlock *block = &s->blocks[block_index];
    block->name = name;
    block->entrance_count++;
    block->stack_count++;
#ifdef SAGOMA_ASSERT
    assert(block->stack_count < SAGOMA_STACK_CAPACITY);
#endif
    block->indent = s->stack_count;
    s->stacks[s->stack_count++] = (struct SagomaStack){
        .block = block,
        .start_tsc = __rdtsc(),
        .child_time = 0,
#ifdef SAGOMA_TRACK_STATS
        .min_time = UINT64_MAX,
        .max_time = 0,
#endif
    };
#ifdef SAGOMA_DEBUG
    SAGOMA_DEBUGC("block[%zu]=%s [indent %zu] {entrance_count=%zu->%zu, stack_count=%zu->%zu} s {stack_count=%zu->%zu}",
        block_index,
        block->name,
        block->indent,
        block->entrance_count-1, block->entrance_count,
        block->stack_count-1, block->stack_count,
        s->stack_count-1, s->stack_count);
    if (s->stack_count > 1)
    {
        for (size_t fc = s->stack_count - 2; fc >= 0; fc--)
        {
            SAGOMA_DEBUGF("... parent fc %zu: %s",
                fc,
                s->stacks[fc].block->name);
            if (fc == 0)
                break;
        }
    }
#endif
}

#ifdef SAGOMA_DEBUG
#define sagoma__block_start(s, name, block_index) sagoma__block_start_(s, name, block_index, __func__, __FILE__, __LINE__)
#else
#define sagoma__block_start(s, name, block_index) sagoma__block_start_(s, name, block_index)
#endif

#define sagoma_block(s, name) sagoma__block_start(&(s), #name, \
        ({ static int i_##name = -1; if (i_##name == -1) i_##name = sagoma__next_block_index(&(s)); i_##name; }))
#define sagoma_blockv(s, vname, name) sagoma__block_start(&(s), name, \
        ({ static int i_##vname = -1; if (i_##vname == -1) i_##vname = sagoma__next_block_index(&(s)); i_##vname; }))

static void sagoma__block_done_(SAGOMA_DEBUG_DECLARE_PARAMS(struct Sagoma *s))
{
    uint64_t now_tsc = __rdtsc();
#ifdef SAGOMA_ASSERT
    assert(s->stack_count);
#endif
    struct SagomaStack *stack = &s->stacks[--s->stack_count];
    uint64_t total_time = now_tsc - stack->start_tsc;
    uint64_t exclusive_time = total_time - stack->child_time;
    SAGOMA_DEBUGC("Pstack_count=%zu->%zu total_time=%lu=%lu-%lu exclusive_time=%lu=%lu-%lu",
        s->stack_count+1, s->stack_count,
        total_time, now_tsc, stack->start_tsc,
        exclusive_time, total_time, stack->child_time);
    if (s->stack_count)
    {
        s->stacks[s->stack_count - 1].child_time += total_time;
        SAGOMA_DEBUGC("Stack %zu (%s) child time += %lu", s->stack_count - 1, s->stacks[s->stack_count - 1].block->name, total_time);
#ifdef SAGOMA_TRACK_STATS
        if (exclusive_time < s->stacks[s->stack_count - 1].min_time)
        {
            SAGOMA_DEBUGC("Stack %zu (%s) min_time = %lu -> %lu",
                s->stack_count - 1, s->stacks[s->stack_count - 1].block->name,
                s->stacks[s->stack_count - 1].min_time, exclusive_time);
            s->stacks[s->stack_count - 1].min_time = exclusive_time;
        }
        if (exclusive_time > s->stacks[s->stack_count - 1].max_time)
        {
            SAGOMA_DEBUGC("Stack %zu (%s) max_time = %lu -> %lu",
                s->stack_count - 1, s->stacks[s->stack_count - 1].block->name,
                s->stacks[s->stack_count - 1].max_time, exclusive_time);
            s->stacks[s->stack_count - 1].max_time = exclusive_time;
        }
#endif
    }
    stack->block->exclusive_time += exclusive_time;
    SAGOMA_DEBUGC("Stack %s exclusive time += %lu", stack->block->name, exclusive_time);
    stack->block->stack_count--;
    if (stack->block->stack_count == 0)
    {
        stack->block->total_time += total_time;
#ifdef SAGOMA_TRACK_STATS
        if (exclusive_time < stack->block->min_time)
        {
            SAGOMA_DEBUGC("Block %s min_time = %lu -> %lu",
                stack->block->name,
                stack->block->min_time, exclusive_time);
            stack->block->min_time = exclusive_time;
        }
        if (exclusive_time > stack->block->max_time)
        {
            SAGOMA_DEBUGC("Block %s max_time = %lu -> %lu",
                stack->block->name,
                stack->block->max_time, exclusive_time);
            stack->block->max_time = exclusive_time;
        }
#endif
    }
}
#ifdef SAGOMA_DEBUG
#define sagoma_block_done(s) sagoma__block_done_(&(s), __func__, __FILE__, __LINE__)
#else
#define sagoma_block_done(s) sagoma__block_done_(&(s))
#endif
#else

// These are callable even if profiling is disabled:

#define sagoma_block(s, name)
#define sagoma_blockv(s, vname, name)
#define sagoma_block_done(s)

#endif

// Always callable, even if profiling is disabled.
static void sagoma__end_(SAGOMA_DEBUG_DECLARE_PARAMS(struct Sagoma *s))
{
    s->end_tsc = __rdtsc();
    SAGOMA_DEBUGC("--> end_tsc=%lu", s->end_tsc);
}

#ifdef SAGOMA_DEBUG
#define sagoma_end(s) sagoma__end_(&(s), __func__, __FILE__, __LINE__)
#else
#define sagoma_end(s) sagoma__end_(&(s))
#endif

#ifndef SAGOMA_DONT_PRINT

// The profile info "as a whole" shows these details / contains this:
struct SagomaInfo
{
    // Track the "longest" stuff, to ensure nicely sized output:
    uint64_t longest_block_name;
    uint64_t longest_indent;
#ifdef SAGOMA
    uint64_t longest_entrance_count;
    uint64_t longest_cycles_total_time;
    uint64_t longest_cycles_exclusive_time;
    uint64_t longest_cycles_per_iteration;
#ifdef SAGOMA_CLOCK_CYCLES
    uint64_t longest_cycles_min_time;
    uint64_t longest_cycles_max_time;
#endif
    // Their "max width" for the integer part, for seconds and microseconds:
    int width_max_entrance_count;
    int width_max_total_time_s;
    int width_max_total_time_us;
#ifdef SAGOMA_CLOCK_CYCLES
    int width_max_total_time_c;
#endif
    int width_max_exclusive_time_s;
    int width_max_exclusive_time_us;
#ifdef SAGOMA_CLOCK_CYCLES
    int width_max_exclusive_time_c;
#endif
    int width_max_per_iteration_s;
    int width_max_per_iteration_us;
#ifdef SAGOMA_CLOCK_CYCLES
    int width_max_per_iteration_c;
#endif
#ifdef SAGOMA_TRACK_STATS
    int width_max_min_time;
    int width_max_max_time;
#endif
#endif
};

#define SAGOMA_TSC_TO_S(tsc)  ((long double)((long double)(tsc) * 1.0L / cpufreq))
#define SAGOMA_TSC_TO_US(tsc) ((long double)((tsc) * USECS_PER_SECOND_F / cpufreq))

#ifdef SAGOMA
static void sagoma__print__block(SAGOMA_DEBUG_DECLARE_PARAMS(struct SagomaInfo *info, struct SagomaBlock *block, uint64_t total_time, uint64_t cpufreq, int colored))
{
#ifdef SAGOMA_DEBUG
    (void)caller_func_name;
    (void)caller_file_name;
    (void)caller_line_number;
#endif
    long double percent_exclusive = 100.0L * block->exclusive_time / total_time;
    long double percent_total     = 100.0L * block->total_time     / total_time;
    printf("%2zu: ", block->indent);
    printf("%-*s%s%-*s%-*s",
        (int)block->indent, "",
        block->name,
        (int)(info->longest_block_name - strlen(block->name)), "",
        (int)(info->longest_indent - block->indent), "");
    if (block->entrance_count > 1)
    {
        if (colored && block->entrance_count >= info->longest_entrance_count)
            printf("\033[33m");
        printf("%*dx", info->width_max_entrance_count+1, (int)block->entrance_count);
        if (colored && block->entrance_count >= info->longest_entrance_count)
            printf("\033[0m");
    }
    else
        printf("%-*s ", info->width_max_entrance_count+1, "");
#ifdef SAGOMA_CLOCK_CYCLES
    printf(" %*luC", info->width_max_total_time_c+1, block->total_time);
#endif
    printf(" %*.9Lf s = %*.4Lf us",
        info->width_max_total_time_s + 10,
        SAGOMA_TSC_TO_S(block->total_time),
        info->width_max_total_time_us + 5,
        SAGOMA_TSC_TO_US(block->total_time));
    printf(" %-*s%6.2Lf%%%-*s",
            (int)block->indent, "",
            percent_total,
            (int)(info->longest_indent - block->indent), "");
    if (block->total_time != block->exclusive_time)
    {
        int color_it = 0;
        if (colored && info->longest_cycles_exclusive_time > 0)
        {
            if (block->exclusive_time >= info->longest_cycles_exclusive_time)
                color_it = 1;
        }
        if (color_it)
            printf("\033[33m");
        printf(" E:");
#ifdef SAGOMA_CLOCK_CYCLES
        printf(" %*luC", info->width_max_exclusive_time_c, block->exclusive_time);
#endif
        printf(" %*.9Lf s = %*.4Lf us (%6.2Lf%%)",
            info->width_max_exclusive_time_s + 10,
            SAGOMA_TSC_TO_S(block->exclusive_time),
            info->width_max_exclusive_time_us + 5,
            SAGOMA_TSC_TO_US(block->exclusive_time),
            percent_exclusive);
        if (color_it)
            printf("\033[0m");
    }
    else if (block->entrance_count > 1)
    {
        //       E:
        printf("   ");
#ifdef SAGOMA_CLOCK_CYCLES
        printf(" %*s ", info->width_max_exclusive_time_c, "");
#endif
        //       .9Lf s = .4Lf us (%6.2Lf%%)
        printf(" %-*s     %-*s     %-6s  ",
            info->width_max_exclusive_time_s + 10, "",
            info->width_max_exclusive_time_us + 5, "",
            "");
    }
    if (block->entrance_count > 1)
    {
        // Several "highest" can be highlighted, if we're coloring:
        // - highest clocks/seconds/microseconds per iteration
        // - highest min clocks/seconds
        // - highest max clocks/seconds
        int color_per_it = 0;
#ifdef SAGOMA_TRACK_STATS
        int color_min = 0;
        int color_max = 0;
#endif
        size_t per_iteration = block->total_time / block->entrance_count;
        if (colored && info->longest_cycles_per_iteration > 0 && per_iteration >= info->longest_cycles_per_iteration)
            color_per_it = 1;
        printf(" =");
#ifdef SAGOMA_CLOCK_CYCLES
        printf(" %s%*zu%sC",
            color_per_it ? "\033[33m" : "",
            info->width_max_per_iteration_c, per_iteration,
            color_per_it ? "\033[0m" : "");
#endif
#ifdef SAGOMA_TRACK_STATS
        if (block->min_time != block->max_time)
        {
            if (colored)
            {
                if (info->longest_cycles_min_time > 0 && block->min_time >= info->longest_cycles_min_time)
                    color_min = 1;
                if (info->longest_cycles_max_time > 0 && block->max_time >= info->longest_cycles_max_time)
                    color_max = 1;
            }
#ifndef SAGOMA_CLOCK_CYCLES
            printf(" C:");
#endif
            printf("[%s%*lu%s-%s%-*lu%s]",
                color_min ? "\033[33m" : "",
                info->width_max_min_time, block->min_time,
                color_min ? "\033[0m" : "",
                color_max ? "\033[33m" : "",
                info->width_max_max_time, block->max_time,
                color_max ? "\033[0m" : "");
        }
#endif
        printf(" %s%*.9Lf%s s = %s%*.4Lf%s us/it",
            color_per_it ? "\033[33m" : "",
            info->width_max_per_iteration_s + 10,
            SAGOMA_TSC_TO_S(block->total_time) / block->entrance_count,
            color_per_it ? "\033[0m" : "",
            color_per_it ? "\033[33m" : "",
            info->width_max_per_iteration_us + 5,
            SAGOMA_TSC_TO_US(block->total_time) / block->entrance_count,
            color_per_it ? "\033[0m" : "");
#ifdef SAGOMA_TRACK_STATS
        if (block->min_time != block->max_time)
        {
            printf(" [%s%*.9Lf%s..%s%*.9Lf%s]",
                color_min ? "\033[33m" : "",
                info->width_max_per_iteration_s + 10,
                SAGOMA_TSC_TO_S(block->min_time),
                color_min ? "\033[0m" : "",
                color_max ? "\033[33m" : "",
                info->width_max_per_iteration_s + 10,
                SAGOMA_TSC_TO_S(block->max_time),
                color_max ? "\033[0m" : "");
        }
#endif
    }
    printf("\n");
}

static void sagoma__build__info(SAGOMA_DEBUG_DECLARE_PARAMS(struct SagomaInfo *info, struct Sagoma *s, uint64_t cpufreq, int ignore_index))
{
#ifdef SAGOMA_DEBUG
    (void)caller_func_name;
    (void)caller_file_name;
    (void)caller_line_number;
#endif
#ifdef SAGOMA_ASSERT
    assert(s->stack_count == 0);
#endif
    for (size_t i = 0; i < SAGOMA_BLOCK_CAPACITY; i++)
    {
        if (ignore_index >= 0 && ignore_index == (int)i)
            continue;
        struct SagomaBlock *block = &s->blocks[i];
        if (block->name == NULL)
            continue;
        size_t block_name_len = strlen(block->name);
        if (block_name_len > info->longest_block_name)
        {
            info->longest_block_name = block_name_len;
        }
        if (block->indent > info->longest_indent)
            info->longest_indent = block->indent;
#ifdef SAGOMA_CLOCK_CYCLES
        if (block->total_time > info->longest_cycles_total_time)
            info->longest_cycles_total_time = block->total_time;
#endif
#ifdef SAGOMA_TRACK_STATS
        if (block->min_time != block->max_time)
        {
            if (block->min_time > info->longest_cycles_min_time)
                info->longest_cycles_min_time = block->min_time;
            if (block->max_time > info->longest_cycles_max_time)
                info->longest_cycles_max_time = block->max_time;
        }
#endif
        if (block->total_time > info->longest_cycles_total_time)
            info->longest_cycles_total_time = block->total_time;
#ifdef SAGOMA_CLOCK_CYCLES
        if (block->total_time != block->exclusive_time)
            if (block->exclusive_time > info->longest_cycles_exclusive_time)
                info->longest_cycles_exclusive_time = block->exclusive_time;
#endif
        if (block->entrance_count > info->longest_entrance_count)
            info->longest_entrance_count = block->entrance_count;
        if (block->entrance_count > 1)
        {
            size_t cycles_per_iteration = block->total_time / block->entrance_count;
            if (cycles_per_iteration > info->longest_cycles_per_iteration)
                info->longest_cycles_per_iteration = cycles_per_iteration;
        }
    }
    // Having calculated it all in cycles, produce a max width for each:
#define SAGOMA_W(thing) (int)((thing) > 0L ? roundl(log10l((thing))) + 1L : 1L)
    info->width_max_entrance_count = SAGOMA_W(info->longest_entrance_count);
    size_t longest_total_time_s = (size_t)SAGOMA_TSC_TO_S(info->longest_cycles_total_time);
    info->width_max_total_time_s = SAGOMA_W(longest_total_time_s);
    size_t longest_total_time_us = (size_t)SAGOMA_TSC_TO_US(info->longest_cycles_total_time);
    info->width_max_total_time_us = SAGOMA_W(longest_total_time_us);
#ifdef SAGOMA_CLOCK_CYCLES
    info->width_max_total_time_c = SAGOMA_W(info->longest_cycles_total_time);
#endif
    size_t longest_exclusive_time_s = (size_t)SAGOMA_TSC_TO_S(info->longest_cycles_exclusive_time);
    info->width_max_exclusive_time_s = SAGOMA_W(longest_exclusive_time_s);
    size_t longest_exclusive_time_us = (size_t)SAGOMA_TSC_TO_US(info->longest_cycles_exclusive_time);
    info->width_max_exclusive_time_us = SAGOMA_W(longest_exclusive_time_us);
#ifdef SAGOMA_CLOCK_CYCLES
    info->width_max_exclusive_time_c = SAGOMA_W(info->longest_cycles_exclusive_time);
#endif
    size_t longest_seconds_per_iteration = (size_t)SAGOMA_TSC_TO_S(info->longest_cycles_per_iteration);
    info->width_max_per_iteration_s = SAGOMA_W(longest_seconds_per_iteration);
    size_t longest_microseconds_per_iteration = (size_t)SAGOMA_TSC_TO_US(info->longest_cycles_per_iteration);
    info->width_max_per_iteration_us = SAGOMA_W(longest_microseconds_per_iteration);
#ifdef SAGOMA_CLOCK_CYCLES
    info->width_max_per_iteration_c = SAGOMA_W(info->longest_cycles_per_iteration);
#endif
#ifdef SAGOMA_TRACK_STATS
    info->width_max_min_time = SAGOMA_W(info->longest_cycles_min_time);
    info->width_max_max_time = SAGOMA_W(info->longest_cycles_max_time);
#endif
#undef SAGOMA_W
}
#endif

#endif

// Always callable, to give an overview of time taken overall.
static void sagoma__print_(SAGOMA_DEBUG_DECLARE_PARAMS(struct Sagoma *s, unsigned long ignore_time, int ignore_index, int colored))
{
    // DWIM ending sagoma if it hasn't been ended yet.
    if (!s->end_tsc)
#ifdef SAGOMA_DEBUG
        sagoma__end_(s, caller_func_name, caller_file_name, caller_line_number);
#else
        sagoma__end_(s);
#endif

#ifdef SAGOMA_DONT_PRINT
    (void)ignore_time;
    (void)ignore_index;
    (void)colored;
    return;
#else

    static uint64_t cpufreq = 0;
    if (!cpufreq)
        cpufreq = sagoma__estimate_cpu_timer_frequency();
#ifdef SAGOMA_ASSERT
    assert(cpufreq != 0);
#endif

    struct SagomaInfo info = {0};
    const char *title = ": Total:";
    info.longest_block_name = strlen(s->name) + strlen(title);
#ifdef SAGOMA
#ifdef SAGOMA_DEBUG
    sagoma__build__info(&info, s, cpufreq, ignore_index, caller_func_name, caller_file_name, caller_line_number);
#else
    sagoma__build__info(&info, s, cpufreq, ignore_index);
#endif
#endif

    // - the name and title, in width max title
    // - filler to put into longest block name
    // - max indent, in spaces
    printf("%s%s%-*s%-*s",
        s->name, title,
        (int)(info.longest_block_name - strlen(s->name) - strlen(title)), "",
        (int)info.longest_indent, "");
    // - width of "Nx T:"
    uint64_t total_time = s->end_tsc - s->start_tsc - ignore_time;
    long double total_time_s = SAGOMA_TSC_TO_S(total_time);
    long double total_time_us = SAGOMA_TSC_TO_US(total_time);
#ifdef SAGOMA
    printf("    %-*s ", info.width_max_entrance_count+1, "");
#ifdef SAGOMA_CLOCK_CYCLES
    printf(" %*luC", info.width_max_total_time_c+1, total_time);
#endif
    printf(" %*.9Lf s = %.4Lf us", info.width_max_total_time_s + 10, total_time_s, total_time_us);
    if (ignore_time > 0)
        printf(" [ignoring %lu = %.4Lf us]", ignore_time, SAGOMA_TSC_TO_US(ignore_time));
#else
    printf(" %.9Lf s = %.4Lf us", total_time_s, total_time_us);
#endif
    printf("\n");
#ifndef SAGOMA
    return;
#endif

#ifdef SAGOMA
    for (size_t i = 0; i < SAGOMA_BLOCK_CAPACITY; i++)
    {
        if (ignore_index >= 0 && (int)i == ignore_index)
            continue;
        struct SagomaBlock *block = &s->blocks[i];
        if (block->name == NULL)
            continue;
#ifdef SAGOMA_DEBUG
        sagoma__print__block(&info, block, total_time, cpufreq, colored, caller_func_name, caller_file_name, caller_line_number);
#else
        sagoma__print__block(&info, block, total_time, cpufreq, colored);
#endif
    }
#undef SAGOMA_BLOCK_CAPACITY
#endif
#endif
}

#ifdef SAGOMA_DEBUG
#define sagoma_print(s, ignore_time, ignore_index) sagoma__print_(&(s), ignore_time, ignore_index, 0, __func__, __FILE__, __LINE__)
#define sagoma_print_color(s, ignore_time, ignore_index) sagoma__print_(&(s), ignore_time, ignore_index, 1, __func__, __FILE__, __LINE__)
#else
#define sagoma_print(s, ignore_time, ignore_index) sagoma__print_(&(s), ignore_time, ignore_index, 0)
#define sagoma_print_color(s, ignore_time, ignore_index) sagoma__print_(&(s), ignore_time, ignore_index, 1)
#endif

#ifdef SAGOMA
#define sagoma_init_loop(s, name) \
    static sagoma_declare(s); \
    static int SAGOMAV(sagoma_init_done_) = 0; \
    if (!SAGOMAV(sagoma_init_done_)) {\
        SAGOMAV(sagoma_init_done_) = 1; \
        sagoma_init_only(s, name); \
    } else { \
        uint64_t SAGOMAV(now_tsc_) = __rdtsc(); \
        if ((s).end_tsc) { \
            uint64_t SAGOMAV(now_delta_) = SAGOMAV(now_tsc_) - (s).end_tsc; \
            (s).start_tsc += SAGOMAV(now_delta_); \
        } \
    } \
    do { } while (0)
#define sagoma_print_loop(s, when) \
    do { \
        sagoma_end(s); \
        if (when) { \
            sagoma_print(s, 0, -1); \
        } \
    } while (0)
#define sagoma_print_loop_color(s, when) \
    do { \
        sagoma_end(s); \
        if (when) { \
            sagoma_print_color(s, 0, -1); \
        } \
    } while (0)
#else
#define sagoma_init_loop(s, name) sagoma_init(s, name)
#define sagoma_print_loop(s, when) sagoma_print(s, 0, -1)
#define sagoma_print_loop_color(s, when) sagoma_print_color(s, 0, -1)
#endif

// This is the somewhat "verbose" API:
// - sagoma_init(varname, name) declares and initializes a Sagoma struct and
//   makes it available as 'varname' for subsequent sagoma_* calls, which take
//   'varname' as their first argument.
//   "internally", but still part of the API:
//   - sagoma_declare(varname) declares a Sagoma struct
//   - sagoma_init_only(varname, name) initializes the Sagoma struct
// - sagoma_block(varname, name) starts a block you want to profile. The
//   'varname' is the same as the one you used in sagoma_init.
//   'name' is the name of the block you want to profile, but it's a variable
//   name, so that different blocks can have different names and the same name
//   can be reused in different scopes, with no warnings/errors about reusing
//   the same underlying variable name.
// - sagoma_block_done(varname) ends the block you started with sagoma_block.
// - sagoma_end(varname) ends the profiling session, but that's also done by:
// - sagoma_print(varname, ignore_time, ignore_index) (and sagoma_print_color)
//   prints the profiling info for the Sagoma struct you initialized with
//   sagoma_init and for the blocks you started with sagoma_block.

// This is the verbose "loop" API:
// - sagoma_init_loop(varname, name) declares and initializes a Sagoma struct
//   for use within a function that you want to analyze the inside bits of,
//   but you don't want to take into consideration the time the program takes
//   when outside of it.
//   This internally declares a static Sagoma struct, and a static "boolean"
//   variable, to keep track of whether the Sagoma struct has been initialized,
//   and how much time was spent outside of the function, to ensure that
//   statistics reflect the time spent inside the function only.
// - you then use sagoma_block() and sagoma_block_done() as usual
// - if you want to print statistics every time the function is called, you can
//   just use sagoma_print() or sagoma_print_color(), as usual.
// - sagoma_print_loop(varname, when) (and sagoma_print_loop_color) instead
//   prints the profiling info for the Sagoma struct you initialized with
//   sagoma_init_loop only when 'when' is true.
//   As an example, you might only print statistics if a global boolean was
//   toggled, or every few iterations. This makes it easier to do that.

// This is the "simple" API, which doesn't let you configure variable names.
// You can use this pretty much once per function, and you can't have multiple
// profiling sessions in the same function.
// Or you could use it globally (see sagoma_declare) and pepper your code as
// you wish.
// - sagoma_ez() declares and initializes a Sagoma struct named "sagoma".
//   If you need different declaration and initialization, do that with, in a C
//   file somewhere, in a global scope:
//   - sagoma_declare(sagoma);
//   ... and then in a function, like main():
//   - extern sagoma_declare(sagoma);
//   - sagoma_init_only(sagoma, name);
// - sagoma_ez_block("name") starts a block you want to profile. The "name" is
//   optional, but beware that this augments the "name" with the file name and
//   line number, so it should help you more easily find the location of the
//   block in your code - so if you don't provide one, you'll get a stray space,
//   which will likely mis-align the output.
// - sagoma_ez_block_done() ends the block you started with sagoma_ez_block().
// - sagoma_ez_end() ends the profiling session, but that's also done by:
// - sagoma_ez_print() (and sagoma_ez_print_color) prints the profiling info
//   for the Sagoma struct you initialized with sagoma_ez() and for the blocks
//   you started with sagoma_ez_block()..sagoma_ez_block_end().

#define SAGOMAS_(str) #str
#define SAGOMAS(str) SAGOMAS_(str)
#define sagoma_ez(name) sagoma_init(sagoma, name "@" __FILE__ ":" SAGOMAS(__LINE__))
#define sagoma_ez_end() sagoma_end(sagoma)
#define sagoma_ez_print() sagoma_print(sagoma, 0, -1)
#define sagoma_ez_print_color() sagoma_print_color(sagoma, 0, -1)
#ifdef SAGOMA
#define sagoma_ez_block(name) sagoma__block_start(&sagoma, name "@" __FILE__ ":" SAGOMAS(__LINE__), ({ static int SAGOMA_(sagoma_tmp_i_, __LINE__) = -1; if (SAGOMA_(sagoma_tmp_i_, __LINE__) == -1) SAGOMA_(sagoma_tmp_i_, __LINE__) = sagoma__next_block_index(&sagoma); SAGOMA_(sagoma_tmp_i_, __LINE__); }))
#define sagoma_ez_block_done() sagoma_block_done(sagoma)
#define sagoma_ez_loop(name) sagoma_init_loop(sagoma, name "@" __FILE__ ":" SAGOMAS(__LINE__))
#else
#define sagoma_ez_block(name)
#define sagoma_ez_block_done()
#define sagoma_ez_loop(name) sagoma_ez(name)
#endif
#define sagoma_ez_print_loop(when) sagoma_print_loop(sagoma, when)
#define sagoma_ez_print_loop_color(when) sagoma_print_loop_color(sagoma, when)

// _MORE_ easy: wrap your existing code around this call, and it'll time it.
// You can also interleave some sagoma_ez_block() and sagoma_ez_block_done() in
// it, and it'll work.
#define sagoma_this(code) \
    sagoma_ez_loop(); \
    sagoma_ez_block(); \
    do { code; } while (0); \
    sagoma_ez_block_done(); \
    sagoma_ez_print_loop_color(1);

// Or interleave this, which is probably simpler:
#define sagoma_this_block(name, code) \
    sagoma_ez_block(name); \
    do { code; } while (0); \
    sagoma_ez_block_done();

#undef USECS_PER_SECOND
#undef MSECS_PER_SECOND
#undef USECS_PER_SECOND_F
#undef CPU_FREQ_ESTIMATE_WAIT_MSECS
