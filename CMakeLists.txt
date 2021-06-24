cmake_minimum_required(VERSION 3.13.0)

# --[ Alif version ] ---------------------------------------------------------------

project(AlifProject VERSION 3.0.21 DESCRIPTION "Alif Programming Language - Compiler.")
message("-- [ Alif version ${CMAKE_PROJECT_VERSION} ]")

# --[ Boost ] ----------------------------------------------------------------------

set(Boost_Version_Needed "1.70.0")

# --[ C++ ] ------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --[ Directories ] ----------------------------------------------------------------

# Alif Compiler Source Code
add_subdirectory(${AlifProject_SOURCE_DIR}/src Alif)
