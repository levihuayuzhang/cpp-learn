project(clang-xray-test)

set(CMAKE_C_COMPILER clang)
set(CMAKE_CXX_COMPILER clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fxray-instrument -fxray-instruction-threshold=1 -g -O2")

set(CLANG_XRAY_LIB_PATH /usr/lib/clang/20/lib/linux/)
link_directories(${CLANG_XRAY_LIB_PATH})

add_executable(clang-xray-test ./main.cpp)

target_link_libraries(clang-xray-test PRIVATE clang_rt.xray-x86_64 clang_rt.xray-basic-x86_64 clang_rt.xray-fdr-x86_64)