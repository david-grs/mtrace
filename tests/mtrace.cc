#include "mtrace.h"
#include "malloc_chrono.h"

#include <iostream>
#include <memory>

int main()
{
    mtrace<malloc_chrono> mt;

    {
        auto uptr = std::make_unique<int>(5);
        (void)uptr;

        std::cout << "malloc=" << std::get<0>(mt).elapsed_time_malloc().count() << std::endl;
        std::cout << "free=" << std::get<0>(mt).elapsed_time_free().count() << std::endl;
    }

    std::cout << "malloc=" << std::get<0>(mt).elapsed_time_malloc().count() << std::endl;
    std::cout << "free=" << std::get<0>(mt).elapsed_time_free().count() << std::endl;
}

