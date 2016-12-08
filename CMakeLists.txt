cmake_minimum_required(VERSION 2.8)

set(HEADERS mtrace.h tsc_chrono.h malloc_chrono.h malloc_counter.h malloc_printer.h)

add_subdirectory(tests)

install(DIRECTORY DESTINATION include/mtrace)
install(FILES ${HEADERS} DESTINATION include/mtrace)

add_definitions(-Wno-deprecated-declarations)
add_executable(example example.cc)
