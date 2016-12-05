#pragma once

#include "chrono.h"

#include <chrono>

struct malloc_chrono
{
    static void pre_malloc(size_t)
    {
        _chrono.start();
    }

    static void post_malloc(size_t, const void*)
    {
        _elapsed_time_malloc += _chrono.elapsed();
    }

    static void pre_free(const void*)
    {
        _chrono.start();
    }

    static void post_free(const void*)
    {
        _elapsed_time_free += _chrono.elapsed();
    }

    static void pre_realloc(const void*, size_t)
    {
        _chrono.start();
    }

    static void post_realloc(const void*, size_t, const void*)
    {
        _elapsed_time_realloc += _chrono.elapsed();
    }

    static std::chrono::nanoseconds elapsed_time_malloc()
    {
        return chrono::from_cycles(_elapsed_time_malloc);
    }

    static std::chrono::nanoseconds elapsed_time_free()
    {
        return chrono::from_cycles(_elapsed_time_free);
    }

    static std::chrono::nanoseconds elapsed_time_realloc()
    {
        return chrono::from_cycles(_elapsed_time_realloc);
    }

    static void clear()
    {
        _elapsed_time_malloc = {};
        _elapsed_time_free = {};
        _elapsed_time_realloc = {};
    }

private:
    static chrono _chrono;
    static double _elapsed_time_malloc;
    static double _elapsed_time_free;
    static double _elapsed_time_realloc;
};

