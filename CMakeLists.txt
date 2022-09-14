cmake_minimum_required(VERSION 3.0)

set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
find_program(MEMORYCHECK_COMMAND valgrind)

project(warehouseforjob)
include(CTest)
add_subdirectory(code)