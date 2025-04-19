cmake_minimum_required(VERSION 2.6)
project(monitor)

# Find required packages
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

include_directories(${CURSES_INCLUDE_DIRS})
include_directories(include)
file(GLOB SOURCES "src/*.cpp")

# Main executable
add_executable(monitor ${SOURCES})
set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
target_link_libraries(monitor ${CURSES_LIBRARIES})
target_compile_options(monitor PRIVATE -Wall -Wextra)

# Tests
enable_testing()
file(GLOB TEST_SOURCES "test/*.cpp")
add_executable(test_runner ${TEST_SOURCES} src/format.cpp src/linux_parser.cpp)
target_link_libraries(test_runner ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
set_property(TARGET test_runner PROPERTY CXX_STANDARD 17)
add_test(NAME test_runner COMMAND test_runner --log_level=test_suite)
