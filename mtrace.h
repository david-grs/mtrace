#pragma once

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

template <class Handler>
struct mtrace
{
	explicit mtrace() :
		mtrace(Handler{})
	{ }

	explicit mtrace(Handler&& handler)
	{
		_handler = std::move(handler);

		save_hooks();
		load_custom_hooks();
	}

	~mtrace()
	{
		restore_hooks();
	}

	const Handler& handler() const { return _handler; }

	static void* malloc(size_t size, const void* /*caller*/)
	{
		restore_hooks();

		_handler.pre_malloc(size);
		void* p = ::malloc(size);
		_handler.post_malloc(size, p);

		save_hooks();
		load_custom_hooks();
		return p;
	}

	static void free(void* mem, const void* /*caller*/)
	{
		restore_hooks();

		_handler.pre_free(mem);
		::free(mem);
		_handler.post_free(mem);

		save_hooks();
		load_custom_hooks();
	}

	static void* realloc(void* mem, size_t size, const void* /*caller*/)
	{
		restore_hooks();

		_handler.pre_realloc(mem, size);
		void* p = ::realloc(mem, size);
		_handler.post_realloc(mem, size, p);

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

	static Handler _handler;
};

template <class Handler> malloc_hook mtrace<Handler>::_old_malloc;
template <class Handler> free_hook mtrace<Handler>::_old_free;
template <class Handler> realloc_hook mtrace<Handler>::_old_realloc;
template <class Handler> Handler mtrace<Handler>::_handler;
