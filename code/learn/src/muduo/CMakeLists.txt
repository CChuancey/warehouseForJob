include_directories(inc)

add_library(mutexlock src/mutex_lock.cpp)

add_subdirectory(testing)
