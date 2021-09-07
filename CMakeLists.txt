cmake_minimum_required(VERSION 3.0.2)
project(quemu)

add_compile_options(-Wall -Wextra -Wshadow -Werror -pedantic)
set(CMAKE_CXX_STANDARD 17)

set(SOURCES src/circuit.cc
            src/gate.cc
            src/state.cc)

include_directories(include)
add_library(quemu SHARED ${SOURCES})
