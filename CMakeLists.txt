cmake_minimum_required(VERSION 3.21.2)

project(chess-cpp)

add_executable(chessengine)

set_target_properties(chessengine PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_sources(chessengine PRIVATE main.cc)

target_compile_options(chessengine PRIVATE -Wall -Wextra -Werror -pedantic -fno-builtin -fsanitize=address)

target_compile_options(chessengine PRIVATE -O0 -g -fno-inline)

target_link_libraries(chessengine -fsanitize=address)

target_include_directories(chessengine PRIVATE "headers/")

add_subdirectory(src)
add_subdirectory(tests)