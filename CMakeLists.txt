cmake_minimum_required(VERSION 3.17)
project(alifc)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost REQUIRED)

add_executable(alifc alif.cpp)
target_include_directories(alifc PUBLIC ${Boost_INCLUDE_DIRS})
