#pragma once

#include <chrono>
#include <thread>
#include <cmath>
#include <type_traits>

namespace detail
{

static inline uint64_t rdtsc()
{
    uint32_t rax, rdx;
    __asm__ __volatile__("rdtsc" : "=a"(rax), "=d"(rdx));
    return ((uint64_t)rdx << 32) + (uint64_t)rax;
}

static inline uint64_t rdtscp()
{
    uint32_t rax, rdx, rcx;
    __asm__ __volatile__("rdtscp" : "=a"(rax), "=d"(rdx), "=c"(rcx));
    return ((uint64_t)rdx << 32) + (uint64_t)rax;
}

static inline uint64_t rdtscp(int& chip, int& core)
{
    uint32_t rax, rdx, rcx;
    __asm__ __volatile__("rdtscp" : "=a"(rax), "=d"(rdx), "=c"(rcx));
    chip = (rcx & 0xFFF000) >> 12;
    core = rcx & 0xFFF;
    return ((uint64_t)rdx << 32) + (uint64_t)rax;
}

struct tsc
{
    static double& get_freq_ghz()
    {
        static double tsc_freq_ghz = .0;
        return tsc_freq_ghz;
    }
};

inline void init()
{
    double& tsc_freq_ghz = detail::tsc::get_freq_ghz();
    if (tsc_freq_ghz)
        return;

    using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
                                     std::chrono::high_resolution_clock,
                                     std::chrono::steady_clock>;

    int chip, core, chip2, core2;

    auto start = Clock::now();
    uint64_t rdtsc_start = detail::rdtscp(chip, core);

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    uint64_t rdtsc_end = detail::rdtscp(chip2, core2);
    auto end = Clock::now();

    if (core != core2 || chip != chip2)
        throw std::runtime_error("please set this executable to a specific core");

    auto duration_ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    uint64_t cycles = rdtsc_end - rdtsc_start;

    tsc_freq_ghz = (double)cycles / duration_ns.count();
}

}

struct tsc_chrono
{
    tsc_chrono() =default;

    static void init()
    {
        detail::init();
    }

    void start()
    {
        m_start = detail::rdtsc();
    }

    void restart()
    {
        start();
    }

    int64_t elapsed() const
    {
        return detail::rdtsc() - m_start;
    }

    int64_t elapsed_and_restart()
    {
        uint64_t now = detail::rdtsc();
        int64_t ts = now - m_start;
        m_start = now;
        return ts;
    }

    std::chrono::nanoseconds elapsed_time() const
    {
        const int64_t cycles = detail::rdtsc() - m_start;
        return from_cycles(cycles);
    }

    static std::chrono::nanoseconds from_cycles(int64_t cycles)
    {
        return std::chrono::nanoseconds(std::llround(cycles / detail::tsc::get_freq_ghz()));
    }

    template <typename _DurationT>
    static int64_t to_cycles(_DurationT duration)
    {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(duration).count() * detail::tsc::get_freq_ghz();
    }

   private:
    uint64_t m_start;
};
