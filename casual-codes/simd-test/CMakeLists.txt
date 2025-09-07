project(simd-test CXX CUDA)

# set(CMAKE_CXX_STANDARD 11)
# set(CXX_STANDARD_REQUIRED TRUE)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_options(${PROJECT_NAME} PRIVATE -mavx2 -O3)

add_executable(cuda-simd-test cuda-simd-test.cu)

