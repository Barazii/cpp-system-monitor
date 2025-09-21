cmake_minimum_required(VERSION 2.6)
project(monitor CXX)

# external libraries
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# path for includes files/headers
include_directories(${CURSES_INCLUDE_DIRS})
include_directories(include)

# Build a backend library from sources (exclude main and ncurses UI)
set(BACKEND_SOURCES
	src/format.cpp
	src/linux_parser.cpp
	src/process.cpp
	src/processor.cpp
	src/system.cpp
)

add_library(MONITOR_BACKEND ${BACKEND_SOURCES})

# Main application executable
add_executable(monitor
	src/main.cpp
	src/ncurses_display.cpp
)

# Tests executable
file(GLOB TEST_SOURCES "test/*.cpp")
add_executable(test_runner ${TEST_SOURCES})

# set the C++ standard
set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
set_property(TARGET test_runner PROPERTY CXX_STANDARD 17)
set_property(TARGET MONITOR_BACKEND PROPERTY CXX_STANDARD 17)

# link targets with libraries
target_link_libraries(monitor MONITOR_BACKEND ${CURSES_LIBRARIES} pthread)
target_link_libraries(test_runner MONITOR_BACKEND ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY} pthread)

# compile options
target_compile_options(monitor PRIVATE -Wall -Wextra)
target_compile_options(test_runner PRIVATE -Wall -Wextra)