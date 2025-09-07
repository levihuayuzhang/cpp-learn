cmake_minimum_required(VERSION 4.0.0)

# set(CMAKE_MESSAGE_LOG_LEVEL VERBOSE)
# set(CMAKE_GENERATOR "Ninja")
# set(CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
# set(CMAKE_VERBOSE_MAKEFILE TRUE)

set(CMAKE_CXX_STANDARD 26)
set(CXX_STANDARD_REQUIRED TRUE)
# set(CMAKE_CXX_EXTENSIONS OFF)

project(cpp-learn LANGUAGES CXX)

# using LLVM (with libc++)
if(UNIX)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
        # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -lc++abi")
        set(CMAKE_CXX_EXTENSIONS OFF)

        if(CMAKE_SYSTEM_NAME MATCHES "Darwin" AND NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
            message(VERBOSE "Running in MacOS (Darwin) but not using AppleClang...")
            execute_process(
                COMMAND uname -a
                OUTPUT_VARIABLE UNAME_OUTPUT
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            message(VERBOSE "${UNAME_OUTPUT}")

            # for compiler to find system headers (like time.h) when using compiler from llvm not Apple (--sysroot)
            execute_process(
                COMMAND xcrun --show-sdk-path
                OUTPUT_VARIABLE APPLE_SDK_PATH
                OUTPUT_STRIP_TRAILING_WHITESPACE
            )
            set(CMAKE_OSX_SYSROOT ${APPLE_SDK_PATH})
            message(VERBOSE "Apple SDK PATH has been set to: ${APPLE_SDK_PATH}")
        endif()
    endif()
endif()

add_subdirectory(datastructure-algorithm)
add_subdirectory(cppcia)
add_subdirectory(casual-codes)
add_subdirectory(leetcuda-practices)
