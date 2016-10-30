cmake_minimum_required(VERSION 2.8)

set(HEADERS mtrace.h)

install(DIRECTORY DESTINATION include)
install(FILES ${HEADERS} DESTINATION include)


