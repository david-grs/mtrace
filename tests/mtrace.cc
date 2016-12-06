#include "mtrace.h"
#include "malloc_chrono.h"

#include <iostream>
#include <memory>

int main()
{
  mtrace<malloc_chrono> mt;

  {
    auto uptr = std::make_unique<int>(5);

    std::cout << "malloc=" << malloc_chrono::elapsed_time_malloc().count() << std::endl;
    std::cout << "free=" << malloc_chrono::elapsed_time_free().count() << std::endl;
  }

  std::cout << "malloc=" << malloc_chrono::elapsed_time_malloc().count() << std::endl;
  std::cout << "free=" << malloc_chrono::elapsed_time_free().count() << std::endl;
}

