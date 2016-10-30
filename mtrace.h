extern "C"
{
#include <malloc.h>
}

#include <iostream>

namespace
{
    using malloc_hook = void*(*)(size_t, const void*);
    using free_hook = void(*)(void*, const void*);
    using realloc_hook = void*(*)(void*, size_t, const void*);
}

struct memcheck
{
    memcheck()
    {
        save_hooks();
        load_custom_hooks();
    }

    ~memcheck()
    {
        restore_hooks();
    }

    static void* malloc(size_t size, const void* caller)
    {
        restore_hooks();

        void* p = ::malloc(size);

        save_hooks();
        std::cout << "malloc " << size << std::endl;
        load_custom_hooks();
        return p;
    }

private:
    static void load_custom_hooks()
    {
        __malloc_hook = malloc;
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

malloc_hook memcheck::_old_malloc;
free_hook memcheck::_old_free;
realloc_hook memcheck::_old_realloc;

