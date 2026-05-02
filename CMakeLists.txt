cmake_minimum_required(VERSION 3.0.0)
project(overlay VERSION 0.1.0)

add_executable(overlay example/main.cpp src/overlay.cpp)
target_include_directories(overlay PRIVATE ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(overlay d2d1.lib Dwmapi.lib Dwrite)