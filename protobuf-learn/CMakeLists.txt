project(protobuf-learn)

find_package(Protobuf REQUIRED)

aux_source_directory(src SRC_PROTOBUF_LEARN)

add_executable(protobuf-learn ${SRC_PROTOBUF_LEARN})

# if(Protobuf_FOUND)
#     protobuf_generate(
#         TARGET protobuf-learn
#         LANGUAGE cpp
#         PROTOS src/test.proto
#         IMPORT_DIRS src
#     )
# endif()

# target_link_libraries(protobuf-learn PRIVATE protobuf::libprotobuf)
# target_link_libraries(protobuf-learn PRIVATE protobuf)
