cmake_minimum_required(VERSION 3.13)
project(directed_acyclic_graph)

set(CMAKE_CXX_STANDARD 14)

add_executable(test_graph tests/test_graph.cpp da_graph.cpp)
