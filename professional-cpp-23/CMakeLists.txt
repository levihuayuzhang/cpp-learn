cmake_minimum_required(VERSION 4.4)

# use clang++ as compiler in order to use `import std` with clangd.
project(professional-cpp-23
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

set(CMAKE_CXX_MODULE_STD TRUE)

add_executable(01_helloworld
    src/01_helloworld.cpp
)
