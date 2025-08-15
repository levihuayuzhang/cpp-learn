cmake_minimum_required(VERSION 4.0.0)

project(cpp-learn LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 26)
set(CXX_STANDARD_REQUIRED TRUE)
# set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_GENERATOR "Ninja")
# set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

# if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#     if (APPLE)
#         message(STATUS "Clang detected on macOS — using libc++ (default on macOS)")
#     elseif (WIN32)
#         message(STATUS "Clang detected on Windows — forcing libstdc++ (MinGW/WSL mode)")
#         set(CMAKE_CXX_FLAGS "-stdlib=libstdc++ ${CMAKE_CXX_FLAGS}")
#     else()
#         message(STATUS "Clang detected on Linux — forcing libstdc++")
#         set(CMAKE_CXX_FLAGS "-stdlib=libstdc++ ${CMAKE_CXX_FLAGS}")
#     endif()
# else()
#     message(STATUS "Compiler is ${CMAKE_CXX_COMPILER_ID} — keeping default standard library")
# endif()

# if (UNIX AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT APPLE)
#     execute_process(
#         COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=include
#         OUTPUT_VARIABLE GCC_INCLUDE_DIR
#         OUTPUT_STRIP_TRAILING_WHITESPACE
#     )
#     if (EXISTS "${GCC_INCLUDE_DIR}/c++")
#         file(GLOB LIBSTDCXX_VER_DIR "${GCC_INCLUDE_DIR}/c++/[0-9]*")
#         if (LIBSTDCXX_VER_DIR)
#             list(GET LIBSTDCXX_VER_DIR 0 LIBSTDCXX_VER_DIR)
#             message(STATUS "Detected libstdc++ include dir: ${LIBSTDCXX_VER_DIR}")
#             include_directories(${LIBSTDCXX_VER_DIR})
#             include_directories(${LIBSTDCXX_VER_DIR}/x86_64-pc-linux-gnu)
#         endif()
#     endif()
# endif()

add_subdirectory(zeromq)
add_subdirectory(datastructure-algorithm)
add_subdirectory(protobuf-learn)
add_subdirectory(linux-basic)
add_subdirectory(cuda-test)

add_executable(stdlibtest tests/stdlib-test.cpp)