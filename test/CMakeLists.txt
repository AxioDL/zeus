cmake_minimum_required(VERSION 3.0)
project(zeustest)

include_directories(../include)

add_executable(zeustest
    main.cpp)

target_link_libraries(zeustest
    Math)
