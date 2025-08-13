project(zeromq)

find_package(PkgConfig REQUIRED)
set(ENV{PKG_CONFIG_PATH} "/opt/homebrew/Cellar/zeromq/4.3.5_2/lib/pkgconfig/libzmq.pc:")
pkg_check_modules(PC_ZeroMQ libzmq)

# aux_source_directory(src SRC_ZEROMQ)

add_executable(zeromq-server src/server.cpp)
add_executable(zeromq-client src/client.cpp)

include_directories(/opt/homebrew/Cellar/cppzmq/4.11.0/include/ /opt/homebrew/Cellar/zeromq/4.3.5_2/include/)
# link_directories(/opt/homebrew/Cellar/zeromq/4.3.5_2/lib/)

link_libraries(libzmq)