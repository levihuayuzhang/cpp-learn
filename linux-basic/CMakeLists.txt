project(linux-basic)

add_executable(debug src/debug-test.cpp)
target_compile_definitions(
    debug 
    PRIVATE
    $<$<CONFIG:DEBUG>:DEBUG>
)

aux_source_directory(src/calc CALC_SRC)
add_library(calc ${CALC_SRC})
target_include_directories(calc PRIVATE src/calc/include)

add_executable(calc-test src/calc/test/calc-test.cpp)
target_include_directories(calc-test PRIVATE src/calc/include)
target_link_libraries(calc-test calc)