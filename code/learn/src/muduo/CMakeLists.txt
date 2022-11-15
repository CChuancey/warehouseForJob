include_directories(inc)

set(CMAKE_BUILD_TYPE Debug)

add_library(mutexlock src/mutex_lock.cpp src/mutex_lock_guard.cpp
                      src/thread.cpp)

add_executable(locktest src/main.cpp src/inventory.cpp src/request.cpp)
target_link_libraries(locktest pthread mutexlock)

add_subdirectory(testing)