project(zmq-learn)

# aux_source_directory(src SRC_ZEROMQ)

add_executable(zmq-server src/server.cpp)
add_executable(zmq-client src/client.cpp)

target_link_libraries(zmq-server zmq)
target_link_libraries(zmq-client zmq)