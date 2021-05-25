cmake_minimum_required(VERSION 3.13.0)

# --[ Alif version ] --------------------------------------------------------------

project(AlifProject VERSION 3.0.19 DESCRIPTION "Alif Programming Language - Compiler.")
message("-- [ Alif version ${CMAKE_PROJECT_VERSION} ]")

# --[ Boost ] ----------------------------------------------------------------------

set(Boost_Version_Needed "1.70.0")

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

        # Boost GCC/MinGW
        # Example: C:/boost_1_60_0
        set(Boost_Root "C:/Alif3/boost/include")
        set(Boost_Lib "C:/Alif3/boost/lib")

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        # Boost Microsoft C++
        # Example: C:/boost_1_60_0
        set(Boost_Root "C:/boost_1_75_0")
        set(Boost_Lib "C:/boost_1_75_0/lib64-msvc-14.2")

    endif()
endif()

# --[ C++ ] ------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --[ Directories ] ----------------------------------------------------------------

# Alif Compiler Source Code
add_subdirectory(${AlifProject_SOURCE_DIR}/src Alif)
