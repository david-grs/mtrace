#include "mtrace.h"
#include "malloc_chrono.h"
#include "malloc_counter.h"
#include "malloc_printer.h"

#include <gtest/gtest.h>

#include <memory>

TEST(mtrace, counter)
{
	mtrace<malloc_counter> mc;

	{
		auto uptr = std::make_unique<int>(5);
		(void)uptr;

		EXPECT_EQ(1, mc.handler().malloc_calls());
		EXPECT_EQ(0, mc.handler().free_calls());
		EXPECT_EQ(0, mc.handler().realloc_calls());
	}

	EXPECT_EQ(1, mc.handler().malloc_calls());
	EXPECT_EQ(1, mc.handler().free_calls());
	EXPECT_EQ(0, mc.handler().realloc_calls());
}

TEST(mtrace, chrono)
{
	mtrace<malloc_chrono> mc;
	int64_t malloc_time;

	{
		auto uptr = std::make_unique<int>(5);
		(void)uptr;

		malloc_time = mc.handler().malloc_time().count();
		EXPECT_GE(malloc_time, 1);
		EXPECT_EQ(0, mc.handler().free_time().count());
		EXPECT_EQ(0, mc.handler().realloc_time().count());
	}


	EXPECT_EQ(malloc_time, mc.handler().malloc_time().count());

	int64_t free_time = mc.handler().free_time().count();
	EXPECT_GE(free_time, 1);
	EXPECT_EQ(0, mc.handler().realloc_time().count());
}


