#pragma once

#include "tsc_chrono.h"

#include <chrono>

struct malloc_chrono
{
    static void init()
    {
        tsc_chrono::init();
    }

    static void pre_malloc(size_t)
    {
        _chrono().start();
    }

    static void post_malloc(size_t, const void*)
    {
        _data().elapsed_time_malloc += _chrono().elapsed();
    }

    static void pre_free(const void*)
    {
        _chrono().start();
    }

    static void post_free(const void*)
    {
        _data().elapsed_time_free += _chrono().elapsed();
    }

    static void pre_realloc(const void*, size_t)
    {
        _chrono().start();
    }

    static void post_realloc(const void*, size_t, const void*)
    {
        _data().elapsed_time_realloc += _chrono().elapsed();
    }

    static std::chrono::nanoseconds elapsed_time_malloc()
    {
        return tsc_chrono::from_cycles(_data().elapsed_time_malloc);
    }

    static std::chrono::nanoseconds elapsed_time_free()
    {
        return tsc_chrono::from_cycles(_data().elapsed_time_free);
    }

    static std::chrono::nanoseconds elapsed_time_realloc()
    {
        return tsc_chrono::from_cycles(_data().elapsed_time_realloc);
    }

    static void clear()
    {
        _data().clear();
    }

private:
    struct data
    {
        void clear()
        {
            elapsed_time_malloc = {};
            elapsed_time_free = {};
            elapsed_time_realloc = {};
        }

        double elapsed_time_malloc = {};
        double elapsed_time_free = {};
        double elapsed_time_realloc = {};
    };

    static data& _data()
    {
        static data d;
        return d;
    }

    static tsc_chrono& _chrono()
    {
        static tsc_chrono c;
        return c;
    }
};

