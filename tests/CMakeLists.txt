include_directories(..)

add_executable(tests mtrace.cc)

find_package (Threads)
target_link_libraries(tests gtest ${CMAKE_THREAD_LIBS_INIT})

add_test(tests tests)
