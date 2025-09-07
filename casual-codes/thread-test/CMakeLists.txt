project(thread-test)

# find_package(Threads REQUIRED)

add_executable(thread-test-1 main.cpp)

# target_link_libraries(thread-test-1 Threads::Threads)
target_link_libraries(thread-test-1 PRIVATE pthread)

