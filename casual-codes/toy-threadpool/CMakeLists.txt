project(toy-threadpool)

# find_package(Threads REQUIRED)

aux_source_directory(./ TOY_THREADPOOL_SRC)
add_executable(toy-threadpool ${TOY_THREADPOOL_SRC})

# target_link_libraries(toy-threadpool PRIVATE Threads::Threads)
target_link_libraries(toy-threadpool PRIVATE pthread)

# target_compile_options(toy-threadpool PRIVATE
#     $<$<CONFIG:Debug>:-fsanitize=address,undefined -fno-omit-frame-pointer -g>
# )
# # target_link_options(toy-threadpool PRIVATE
# #     $<$<CONFIG:Debug>:-fsanitize=address,undefined>
# # )

target_compile_options(toy-threadpool PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=thread>
)
target_link_options(toy-threadpool PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=thread>
)