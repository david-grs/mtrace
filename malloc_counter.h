#pragma once

struct malloc_counter
{
    int malloc_calls() const { return _mallocs; }
    int free_calls() const { return _frees; }
    int realloc_calls() const { return _reallocs; }

    void pre_malloc(size_t /*size*/) {}
    void post_malloc(size_t size, const void* mem)
    {
        ++_mallocs;
    }

    void pre_free(const void* /*mem*/) {}
    void post_free(const void* mem)
    {
        ++_frees;
    }

    void pre_realloc(const void* /*mem*/, size_t /*size*/) {}
    void post_realloc(const void* mem, size_t size, const void* new_mem)
    {
        ++_reallocs;
    }

private:
    int _mallocs = {};
    int _frees = {};
    int _reallocs = {};
};
