cmake_minimum_required(VERSION 3.15.0)

# --[ Alif version ] --------------------------------------------------------------

project(AlifProject VERSION 3.0.2 DESCRIPTION "Alif language compiler.")
message("-- [ Alif version ${CMAKE_PROJECT_VERSION} ]")

# --[ Settings ] -------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_VERBOSE_MAKEFILE OFF)

# Default build
set(default_build_type "Release")
if(CMAKE_BUILD_TYPE_INITIALIZED_TO_DEFAULT)
    set(CMAKE_BUILD_TYPE Release)
endif()
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type (default Release)" FORCE)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
    message("[!] Build type changed to ${CMAKE_BUILD_TYPE}.")
    if(NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_CONFIGURATION_TYPES Release)
        message("[!] Configuration type changed to ${CMAKE_CONFIGURATION_TYPES}.")
    endif()
endif()

if(CMAKE_BUILD_TYPE STREQUAL Debug)  
    message("[!] Warning: Build type set to Debug. Make sure you have Debug version of Boost libs.")
endif()

# --[ Directories ] ----------------------------------------------------------------

# Alif Compiler Source Code
add_subdirectory(${AlifProject_SOURCE_DIR}/src Alif)
