project(linux-basic)

add_executable(debug src/debug-test.cpp)
target_compile_definitions(
    debug 
    PRIVATE
    $<$<CONFIG:DEBUG>:DEBUG>
)

# # g++ not support libc++, only libstdc++
# # clang++ default to libstdc++, libc++ can be enabled by follow
# # for cross platforms porting
# if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
#     set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
#     set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")
#     # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/c++/v1")
#     # set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib/llvm-15/lib")
# endif()

aux_source_directory(src/calc CALC_SRC)
# add_library(calc STATIC ${CALC_SRC})
add_library(calc SHARED ${CALC_SRC})
target_include_directories(calc PRIVATE src/calc/include)

add_executable(calc-test src/calc/test/calc-test.cpp)
target_include_directories(calc-test PRIVATE src/calc/include)

# -fsanitize=address,undefined
# -fsanitize=thread
# -fsanitize=memory (clang only)
# target_compile_options(calc-test PRIVATE -fsanitize=address,undefined)
# target_link_options(calc-test PRIVATE -fsanitize=address,undefined)
# target_compile_options(calc-test PRIVATE -fsanitize=memory)
# target_link_options(calc-test PRIVATE -fsanitize=memory)

target_link_libraries(calc-test calc)