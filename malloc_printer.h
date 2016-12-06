#pragma once

#include <iostream>

struct malloc_printer
{
    void pre_malloc(size_t /*size*/) {}
    void post_malloc(size_t size, const void* mem)
    {
        std::cout << "malloc " << size << " bytes at " << mem << std::endl;
    }

    void pre_free(const void* /*mem*/) {}
    void post_free(const void* mem)
    {
        std::cout << "free " << mem << std::endl;
    }

    void pre_realloc(const void* /*mem*/, size_t /*size*/) {}
    void post_realloc(const void* mem, size_t size, const void* new_mem)
    {
        std::cout << "realloc " << size << " bytes from " << mem << " to " << new_mem << std::endl;
    }
};
