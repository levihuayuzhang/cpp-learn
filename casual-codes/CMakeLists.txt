
add_subdirectory(zeromq)
add_subdirectory(protobuf-learn)
add_subdirectory(linux-basic)
add_subdirectory(cuda-test)
add_subdirectory(simd-test)
add_subdirectory(toy-threadpool)
#add_subdirectory(clang-xray-test)
add_subdirectory(thread-test)
add_subdirectory(template-tests)

add_executable(stdlibtest tests/stdlib-test.cpp)