cmake_minimum_required(VERSION 2.8)

set(HEADERS mtrace.h tsc_chrono.h malloc_chrono.h malloc_printer.h)

install(DIRECTORY DESTINATION include/mtrace)
install(FILES ${HEADERS} DESTINATION include/mtrace)

