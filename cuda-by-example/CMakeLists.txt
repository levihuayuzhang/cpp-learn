# set(CMAKE_CUDA_HOST_COMPILER /usr/bin/clang++)

project(cuda-by_example
    LANGUAGES
    CXX C CUDA
)

set(CMAKE_CUDA_ARCHITECTURES 120) # for 5070ti laptop

# enable_language(CXX CUDA)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include(CTest)

include_directories(book-code/common)

add_executable(hello_world-cuda
    src/hello_world.cu
)
add_executable(quering-device
    src/quering_device.cu
)
add_executable(summing_vector
    src/summing_vectors.cu
)

add_executable(julia-set
    src/julia-set.cu
)
target_link_libraries(julia-set PRIVATE
    OpenGL::GL
    GLUT::GLUT
)

add_executable(ripple
    src/ripple.cu
)
target_link_libraries(ripple PRIVATE
    OpenGL::GL
    GLUT::GLUT
)