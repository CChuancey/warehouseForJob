
add_executable(project src/main.cpp)

add_custom_command(
  TARGET project
  POST_BUILD
  COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action
          memcheck --track-origins=yes
  COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")