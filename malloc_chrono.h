#pragma once

#include "tsc_chrono.h"

#include <chrono>

class malloc_chrono
{
public:
	explicit malloc_chrono()
	{
		tsc_chrono::init();
	}

	void pre_malloc(size_t)
	{
		_chrono.start();
	}

	void post_malloc(size_t, const void*)
	{
		_elapsed_time_malloc += _chrono.elapsed();
	}

	void pre_free(const void*)
	{
		_chrono.start();
	}

	void post_free(const void*)
	{
		_elapsed_time_free += _chrono.elapsed();
	}

	void pre_realloc(const void*, size_t)
	{
		_chrono.start();
	}

	void post_realloc(const void*, size_t, const void*)
	{
		_elapsed_time_realloc += _chrono.elapsed();
	}

	std::chrono::nanoseconds malloc_time() const
	{
		return tsc_chrono::from_cycles(static_cast<int64_t>(_elapsed_time_malloc));
	}

	std::chrono::nanoseconds free_time() const
	{
		return tsc_chrono::from_cycles(static_cast<int64_t>(_elapsed_time_free));
	}

	std::chrono::nanoseconds realloc_time() const
	{
		return tsc_chrono::from_cycles(static_cast<int64_t>(_elapsed_time_realloc));
	}

private:
	double _elapsed_time_malloc = {};
	double _elapsed_time_free = {};
	double _elapsed_time_realloc = {};

	tsc_chrono _chrono;
};

