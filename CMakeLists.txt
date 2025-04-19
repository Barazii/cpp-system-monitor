cmake_minimum_required(VERSION 2.6)
project(monitor CXX)

# external libraries
set(CURSES_NEED_NCURSES TRUE)
find_package(Curses REQUIRED)
find_package(Boost COMPONENTS unit_test_framework REQUIRED)

# path for includes files/headers
include_directories(${CURSES_INCLUDE_DIRS})
include_directories(include)

# collect the source files
file(GLOB SOURCES "src/*.cpp")
file(GLOB TEST_SOURCES "test/*.cpp")

# executables (for main program and tests)
add_executable(monitor ${SOURCES})
add_executable(test_runner ${TEST_SOURCES} src/format.cpp src/linux_parser.cpp)

# set the C++ standard
set_property(TARGET monitor PROPERTY CXX_STANDARD 17)
set_property(TARGET test_runner PROPERTY CXX_STANDARD 17)

# link executables (which contain the sources) with libraries
target_link_libraries(monitor ${CURSES_LIBRARIES})
target_link_libraries(test_runner ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

# compile options
target_compile_options(monitor PRIVATE -Wall -Wextra)