#pragma once

#include <iostream>

struct printer_handler
{
    static void pre_malloc(size_t /*size*/) {}
    static void post_malloc(size_t size, const void* mem)
    {
        std::cout << "malloc " << size << " bytes at " << mem << std::endl;
    }

    static void pre_free(const void* /*mem*/) {}
    static void post_free(const void* mem)
    {
        std::cout << "free " << mem << std::endl;
    }

    static void pre_realloc(const void* /*mem*/, size_t /*size*/) {}
    static void post_realloc(const void* mem, size_t size, const void* new_mem)
    {
        std::cout << "realloc " << size << " bytes from " << mem << " to " << new_mem << std::endl;
    }
};
