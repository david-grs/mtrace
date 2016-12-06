#pragma once

#include <tuple>
#include <utility>

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

namespace detail
{
    template<typename T, typename F, std::size_t... Is>
    void for_each(T&& t, F f, std::integer_sequence<std::size_t, Is...>)
    {
        auto l = { (f(std::get<Is>(t)), 0)... };
    }

    template<typename... Ts, typename F>
    void for_each_in_tuple(std::tuple<Ts...>& t, F f)
    {
        detail::for_each(t, f, std::make_index_sequence<sizeof...(Ts)>{});
    }
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

    template <std::size_t I>
    auto& get()
    {
        return std::get<I>(_handlers);
    }

    template <std::size_t I>
    const auto& get() const
    {
        return std::get<I>(_handlers);
    }

    static void* malloc(size_t size, const void* caller)
    {
        restore_hooks();

        detail::for_each_in_tuple(_handlers, [&](auto& x) { x.pre_malloc(size); });
        void* p = ::malloc(size);
        detail::for_each_in_tuple(_handlers, [&](auto& x) { x.post_malloc(size, p); });

        save_hooks();
        load_custom_hooks();
        return p;
    }

    static void free(void* mem, const void* caller)
    {
        restore_hooks();

        detail::for_each_in_tuple(_handlers, [&](auto& x) { x.pre_free(mem); });
        ::free(mem);
        detail::for_each_in_tuple(_handlers, [&](auto& x) { x.post_free(mem); });

        save_hooks();
        load_custom_hooks();
    }

    static void* realloc(void* mem, size_t size, const void* caller)
    {
        restore_hooks();

        detail::for_each_in_tuple(_handlers, [&](auto& x) { x.pre_realloc(mem, size); });
        void* p = ::realloc(mem, size);
        detail::for_each_in_tuple(_handlers, [&](auto& x) { x.post_realloc(mem, size, p); });

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

    static std::tuple<Handlers...> _handlers;

    template <std::size_t I, typename... Ts>
    friend auto& std::get(const mtrace<Ts...>&);
};

template <typename... Handlers> malloc_hook mtrace<Handlers...>::_old_malloc;
template <typename... Handlers> free_hook mtrace<Handlers...>::_old_free;
template <typename... Handlers> realloc_hook mtrace<Handlers...>::_old_realloc;
template <typename... Handlers> std::tuple<Handlers...> mtrace<Handlers...>::_handlers;

namespace std
{
    template<size_t I, typename... Handlers>
    auto& get(const mtrace<Handlers...>& mt)
    {
        return std::get<I>(mt._handlers);
    }
}
