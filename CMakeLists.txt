cmake_minimum_required(VERSION 3.8)

project(stack_inspector)

set(CMAKE_CXX_STANDARD 17)

if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU")
	set(cxx_compile_options "-g -Wno-deprecated-declarations -Wall -Wextra -Wpedantic -Wconversion -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef -Wold-style-cast -Wshadow -Wdouble-promotion")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${cxx_compile_options}")
include(gtest.cmake)

set(HEADERS mtrace.h tsc_chrono.h malloc_chrono.h malloc_counter.h malloc_printer.h)

add_executable(example example.cc ${HEADERS})
add_subdirectory(tests)

install(DIRECTORY DESTINATION include/mtrace)
install(FILES ${HEADERS} DESTINATION include/mtrace)

