add_executable(mutex_lock_test mutex_lock_test.cpp)

target_link_libraries(mutex_lock_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(mutex_lock_test)
