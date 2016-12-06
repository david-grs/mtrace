#include "mtrace.h"
#include "malloc_chrono.h"

#include <iostream>

int main()
{
    mtrace<malloc_chrono> mt;

    {
        auto uptr = std::make_unique<int>(5);
        std::cout << "after unique_ptr ctor: malloc=" << std::get<0>(mt).malloc_time().count() << " free=" << std::get<0>(mt).free_time().count() << std::endl;
    }

    std::cout << "after unique_ptr dtor: malloc=" << std::get<0>(mt).malloc_time().count() << " free=" << std::get<0>(mt).free_time().count() << std::endl;
}
