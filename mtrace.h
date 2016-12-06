#pragma once

extern "C"
{
#include <malloc.h>
}

#include <initializer_list>

namespace
{
    using malloc_hook = void*(*)(size_t, const void*);
    using free_hook = void(*)(void*, const void*);
    using realloc_hook = void*(*)(void*, size_t, const void*);
}

template <typename... Handlers>
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

    static void test()
    {
        std::initializer_list<int>{(Handlers::pre_malloc(0), 0)... };
    }

    static void* malloc(size_t size, const void* caller)
    {
        restore_hooks();

        std::initializer_list<int>{(Handlers::pre_malloc(size), 0)... };
        void* p = ::malloc(size);
        std::initializer_list<int>{(Handlers::post_malloc(size, p), 0)... };

        save_hooks();
        load_custom_hooks();
        return p;
    }

    static void free(void* mem, const void* caller)
    {
        restore_hooks();

        std::initializer_list<int>{(Handlers::pre_free(mem), 0)... };
        ::free(mem);
        std::initializer_list<int>{(Handlers::post_free(mem), 0)... };

        save_hooks();
        load_custom_hooks();
    }

    static void* realloc(void* mem, size_t size, const void* caller)
    {
        restore_hooks();

        std::initializer_list<int>{(Handlers::pre_realloc(mem, size), 0)... };
        void* p = ::realloc(mem, size);
        std::initializer_list<int>{(Handlers::post_realloc(mem, size, p), 0)... };

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

template <typename... Handlers> malloc_hook mtrace<Handlers...>::_old_malloc;
template <typename... Handlers> free_hook mtrace<Handlers...>::_old_free;
template <typename... Handlers> realloc_hook mtrace<Handlers...>::_old_realloc;

