include_directories(..)

add_definitions(-Wno-deprecated-declarations)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(unit_tests mtrace.cc)
target_link_libraries(unit_tests ${GTEST_BOTH_LIBRARIES})
add_test(unit_tests unit_tests)
