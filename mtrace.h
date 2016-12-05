#pragma once

extern "C"
{
#include <malloc.h>
}

namespace
{
    using malloc_hook = void*(*)(size_t, const void*);
    using free_hook = void(*)(void*, const void*);
    using realloc_hook = void*(*)(void*, size_t, const void*);
}

template <typename Handler>
struct mtrace
{
    mtrace()
    {
        save_hooks();
        load_custom_hooks();
    }

    ~mtrace()
    {
        restore_hooks();
    }

    static void* malloc(size_t size, const void* caller)
    {
        restore_hooks();

        Handler::pre_malloc(size);
        void* p = ::malloc(size);
        Handler::post_malloc(size, p);

        save_hooks();
        load_custom_hooks();
        return p;
    }

    static void free(void* mem, const void* caller)
    {
        restore_hooks();

        Handler::pre_free(mem);
        ::free(mem);
        Handler::post_free(mem);

        save_hooks();
        load_custom_hooks();
    }

    static void* realloc(void* mem, size_t size, const void* caller)
    {
        restore_hooks();

        Handler::pre_realloc(mem, size);
        void* p = ::realloc(mem, size);
        Handler::post_realloc(mem, size, p);

        save_hooks();
        load_custom_hooks();
        return p;
    }

private:
    static void load_custom_hooks()
    {
        __malloc_hook = malloc;
        __free_hook = free;
        __realloc_hook = realloc;
    }

    static void save_hooks()
    {
        _old_malloc = __malloc_hook;
        _old_free = __free_hook;
        _old_realloc = __realloc_hook;
    }

    static void restore_hooks()
    {
        __malloc_hook = _old_malloc;
        __free_hook = _old_free;
        __realloc_hook = _old_realloc;
    }

    static malloc_hook  _old_malloc;
    static free_hook    _old_free;
    static realloc_hook _old_realloc;
};

using mtrace_printer = mtrace<printer_handler>;

template <typename Handler> malloc_hook mtrace<Handler>::_old_malloc;
template <typename Handler> free_hook mtrace<Handler>::_old_free;
template <typename Handler> realloc_hook mtrace<Handler>::_old_realloc;

