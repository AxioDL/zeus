cmake_minimum_required(VERSION 3.10 FATAL_ERROR) # because of c++17
project(zeustest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(zeustest main.cpp)
target_link_libraries(zeustest zeus)
