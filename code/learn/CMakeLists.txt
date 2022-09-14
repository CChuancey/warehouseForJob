add_executable(helloworld src/helloworld.cpp)
target_link_libraries(helloworld pthread)
# add_test(hellotest helloworld)

add_executable(copycontrol src/copycontrol.cpp)
add_test(copycontroltest copycontrol)

add_executable(ste src/main.cpp)

# 在生成exe后执行内存检查
add_custom_command(
  TARGET copycontrol
  POST_BUILD
  COMMAND ${CMAKE_CTEST_COMMAND} --force-new-ctest-process --test-action
          memcheck --track-origins=yes
  COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
  WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")

set_target_properties(helloworld PROPERTIES CXX_STANDARD 17)
