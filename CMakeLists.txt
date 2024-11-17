cmake_minimum_required(VERSION 3.10)

# Project name
project(S6502)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/S6502.cc
    src/Bus.cc
)

# Fetch GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
    DOWNLOAD_EXTRACT_TIMESTAMP True
)
FetchContent_MakeAvailable(googletest)

# Executable for the main test
add_executable(S6502Test
    test/S6502Test.cc
    ${SOURCES}
)

# Link GoogleTest
target_link_libraries(S6502Test gtest_main)

# Compiler options
target_compile_options(S6502Test PRIVATE -Wall -Wextra -pedantic)

# Enable testing
enable_testing()

# Add tests
add_test(NAME S6502Test COMMAND S6502Test)
