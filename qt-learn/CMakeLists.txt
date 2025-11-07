cmake_minimum_required(VERSION 4.1.0)

set(CMAKE_CXX_STANDARD 26)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

project(qt-guide)

# use qt installed from maintanancetool
# set(QT_VERSION 6.10.0)
# if(APPLE)
#     message(STATUS "Building on macOS...")
#     # set(CMAKE_PREFIX_PATH ~/Qt/${QT_VERSION}/macos)
#     set(Qt6_DIR ~/Qt/${QT_VERSION}/macos/lib/cmake/Qt6)
#     # set(Qt6Widgets_DIR ~/Qt/${QT_VERSION}/macos/lib/cmake/Qt6Widgets)
# elseif(UNIX AND NOT APPLE)
#     message(STATUS "Building on Linux...")
# elseif(WIN32)
#     message(STATUS "Building on Windows...")
# endif()

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

file(GLOB source *.cpp)
add_executable(qt-guide ${source})
target_link_libraries(qt-guide PRIVATE Qt6::Core Qt6::Widgets)
