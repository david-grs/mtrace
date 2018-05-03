#include "mtrace.h"
#include "malloc_chrono.h"

#include <iostream>

int main()
{
	mtrace<malloc_chrono> mt;
	const malloc_chrono& hdl = mt.handler();

	{
		auto uptr = std::make_unique<int>(5);

		std::cout << "after unique_ptr ctor: malloc_time=" << hdl.malloc_time().count() << "ns "
				  << "free_time=" << hdl.free_time().count() << "ns" << std::endl;
	}

	std::cout << "after unique_ptr dtor: malloc_time=" << hdl.malloc_time().count() << "ns "
			  << "free_time=" << hdl.free_time().count() << "ns" << std::endl;

	return 0;
}
