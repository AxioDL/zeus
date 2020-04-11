cmake_minimum_required(VERSION 3.10 FATAL_ERROR) # because of c++17
project(zeustest)

if (NOT MSVC)
  set(CMAKE_CXX_STANDARD 20)
  set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

add_executable(zeustest main.cpp)
target_link_libraries(zeustest zeus)
