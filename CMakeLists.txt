cmake_minimum_required(VERSION 3.0)

set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
find_program(MEMORYCHECK_COMMAND valgrind)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt
    ON
    CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

project(warehouseforjob)
include(CTest)
add_subdirectory(code)
