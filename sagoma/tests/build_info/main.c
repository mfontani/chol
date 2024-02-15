#include "../../../tap/tap.h"

#define SAGOMA
#define SAGOMA_ASSERT
#define SAGOMA_TRACK_STATS
#define SAGOMA_CLOCK_CYCLES
// #define SAGOMA_DEBUG
// #define SAGOMA_DONT_PRINT
#include "../../sagoma.h"

void test_build_info(void)
{
    tap_diag("Running test for sagoma__build__info()");
    {
        struct Sagoma sagoma = {0};
        struct SagomaInfo info = {0};
        info.longest_block_name = 2;
        uint64_t cpufreq = sagoma__estimate_cpu_timer_frequency();
        sagoma__build__info(&info, &sagoma, cpufreq, -1);
        tap_is_int(info.longest_block_name, 2, "longest_block_name is 2");
        tap_is_int(info.longest_indent, 0, "longest_indent is 0");
        tap_is_int(info.longest_entrance_count, 0, "longest_entrance_count is 0");
        tap_is_int(info.width_max_entrance_count, 1, "width_max_entrance_count is 1");
        tap_is_int(info.longest_cycles_total_time, 0, "longest_cycles_total_time is 0");
        tap_is_int(info.width_max_total_time_s, 1, "width_max_total_time_s is 1");
        tap_is_int(info.width_max_total_time_us, 1, "width_max_total_time_us is 1");
    }
    {
        uint64_t cpufreq = sagoma__estimate_cpu_timer_frequency();
        // tap_diag("cpufreq = %lu", cpufreq);
        struct Sagoma sagoma = {0};
        sagoma.block_count = 1;
        sagoma.blocks[0].name = "block";
        sagoma.blocks[0].total_time = cpufreq;
        sagoma.blocks[0].exclusive_time = cpufreq;
        sagoma.blocks[0].entrance_count = 1;
        sagoma.blocks[0].stack_count = 1;
        sagoma.blocks[0].indent = 0;
        sagoma.blocks[0].min_time = cpufreq;
        sagoma.blocks[0].max_time = cpufreq;
        struct SagomaInfo info = {0};
        info.longest_block_name = 2;
        sagoma__build__info(&info, &sagoma, cpufreq, -1);
        tap_is_int(info.longest_block_name, 5, "longest_block_name is 5 for 'block'");
        tap_is_int(info.longest_indent, 0, "longest_indent is 0");
        tap_is_int(info.longest_entrance_count, 1, "longest_entrance_count is 1");
        tap_is_int(info.width_max_entrance_count, 1, "width_max_entrance_count is 1");
        tap_is_ulong(info.longest_cycles_total_time, cpufreq, "longest_cycles_total_time is cpufreq");
        // 1 cpufreq = 1s = 7w
        tap_is_int(info.width_max_total_time_s, 1, "width_max_total_time_s is 1");
        // 1 cpufreq = 1,000,000us = 7w
        tap_is_int(info.width_max_total_time_us, 7, "width_max_total_time_us is 7");
    }
}
