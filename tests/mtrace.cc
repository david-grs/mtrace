#include "mtrace.h"
#include "malloc_chrono.h"
#include "malloc_counter.h"

#include <gtest/gtest.h>

#include <memory>

TEST(mtrace, counter)
{
    mtrace<malloc_counter> mc;

    {
        auto uptr = std::make_unique<int>(5);
        (void)uptr;

        EXPECT_EQ(1, std::get<0>(mc).malloc_calls());
        EXPECT_EQ(0, std::get<0>(mc).free_calls());
        EXPECT_EQ(0, std::get<0>(mc).realloc_calls());
    }

    EXPECT_EQ(1, std::get<0>(mc).malloc_calls());
    EXPECT_EQ(1, std::get<0>(mc).free_calls());
    EXPECT_EQ(0, std::get<0>(mc).realloc_calls());
}

TEST(mtrace, chrono)
{
    mtrace<malloc_chrono> mc;
    int64_t malloc_time;

    {
        auto uptr = std::make_unique<int>(5);
        (void)uptr;

        malloc_time = std::get<0>(mc).elapsed_time_malloc().count();
        EXPECT_GE(malloc_time, 1);
        EXPECT_EQ(0, std::get<0>(mc).elapsed_time_free().count());
        EXPECT_EQ(0, std::get<0>(mc).elapsed_time_realloc().count());
    }


    EXPECT_EQ(malloc_time, std::get<0>(mc).elapsed_time_malloc().count());

    int64_t free_time = std::get<0>(mc).elapsed_time_free().count();
    EXPECT_GE(free_time, 1);
    EXPECT_EQ(0, std::get<0>(mc).elapsed_time_realloc().count());
}

