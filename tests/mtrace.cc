#include "mtrace.h"
#include "malloc_chrono.h"
#include "malloc_counter.h"
#include "malloc_printer.h"

#include <gtest/gtest.h>

#include <memory>

TEST(mtrace, counter)
{
	mtrace<malloc_counter> mt;
	const malloc_counter& hdl = mt.handler();

	{
		auto uptr = std::make_unique<int>(5);
		(void)uptr;

		EXPECT_EQ(1, hdl.malloc_calls());
		EXPECT_EQ(0, hdl.free_calls());
		EXPECT_EQ(0, hdl.realloc_calls());
	}

	EXPECT_EQ(1, hdl.malloc_calls());
	EXPECT_EQ(1, hdl.free_calls());
	EXPECT_EQ(0, hdl.realloc_calls());
}

TEST(mtrace, chrono)
{
	mtrace<malloc_chrono> mt;
	const malloc_chrono& hdl = mt.handler();

	int64_t malloc_time;

	{
		auto uptr = std::make_unique<int>(5);
		(void)uptr;

		malloc_time = hdl.malloc_time().count();
		EXPECT_GE(malloc_time, 1);
		EXPECT_EQ(0, hdl.free_time().count());
		EXPECT_EQ(0, hdl.realloc_time().count());
	}


	EXPECT_EQ(malloc_time, hdl.malloc_time().count());

	int64_t free_time = hdl.free_time().count();
	EXPECT_GE(free_time, 1);
	EXPECT_EQ(0, hdl.realloc_time().count());
}


