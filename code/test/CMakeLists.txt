
add_executable(testsForCode src/main.cpp)

add_test(tester testsForCode)
add_custom_command(
  TARGET testsForCode
  POST_BUILD
  COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action
          memcheck --track-origins=yes
  COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

set_property(TARGET testsForCode PROPERTY CXX_STANDARD 17)
