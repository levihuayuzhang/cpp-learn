
add_executable(template-test1 test1.cpp)
add_executable(template-class template-class.cpp)
add_executable(sfinae-test sfinae-test.cpp)
add_executable(variadic-template variadic-template.cpp)

add_executable(type-limitation-extraction type-limitation-extraction.cpp)
# target_compile_options(type-limitation-extraction PRIVATE -std=c++17) # not using c++20 concept.
set_target_properties(type-limitation-extraction PROPERTIES CXX_STANDARD 17)

add_executable(perfect-forwarding perfect-forwarding.cpp)

add_executable(concept-test concept-test.cpp)
set_target_properties(concept-test PROPERTIES CXX_STANDARD 20)
# set_property(TARGET concept-test PROPERTY CXX_STANDARD 20)
add_executable(concept-test2 concept-test2.cpp)
set_target_properties(concept-test2 PROPERTIES CXX_STANDARD 20)

add_executable(lambda-test lambda-test.cpp)
add_executable(lambda-capture-list lambda-capture-list.cpp)
add_executable(function-object function-object.cpp)
set_target_properties(function-object PROPERTIES CXX_STANDARD 20)