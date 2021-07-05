cmake_minimum_required(VERSION 3.13.0)

# --[ Alif version ] ---------------------------------------------------------------

project(AlifProject VERSION 3.0.32 DESCRIPTION "Alif Programming Language - Compiler.")
message("-- [ Alif version ${CMAKE_PROJECT_VERSION} ]")

# --[ Uninstall ] ------------------------------------------------------------------

if(TARGET uninstall)
    message("[!] By default, CMake does not provide the 'uninstall' target.")
    message("[!] If you want to uninstall your project You need to delete the files listed in 'install_manifest.txt' file.")
    return()
endif()

# --[ C++ Compiler ] ---------------------------------------------------------------

if (WIN32)
    if(EXISTS "C:/Alif3/")

        # Windows - MinGW64 - Boost
        message("[*] Switch default compiler to 'C:/Alif3/gcc'")
        set(CMAKE_C_COMPILER                "C:/Alif3/gcc/bin/gcc.exe")
        set(CMAKE_C_COMPILER:FILEPATH       "C:/Alif3/gcc/bin/gcc.exe")
        set(CMAKE_CXX_COMPILER              "C:/Alif3/gcc/bin/g++.exe")
        set(CMAKE_CXX_COMPILER:FILEPATH     "C:/Alif3/gcc/bin/g++.exe")
        
    endif()
endif()

# --[ Boost ] ----------------------------------------------------------------------

set(Boost_Version_Needed "1.76.0")

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

        IF(EXISTS "C:/Alif3/")
            # Windows - MinGW64 - Boost
            set(Boost_Root "C:/Alif3/boost/include")
            set(Boost_Lib "C:/Alif3/boost/lib")
        endif()

    elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        # Remove this line to force building Alif compiler using MSVC.
        message("[!] Sorry. Alif don't support Microsoft C++ compiler on Windows, Please use MinGW64.")
        return()

        IF(EXISTS "C:/Alif3/")
            # Windows - Microsoft C++ - Boost
            set(Boost_Root "C:/Alif3/boost/include")
            set(Boost_Lib "C:/Alif3/boost/lib")
        endif()

    endif()
endif()

# --[ C++ ] ------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --[ Directories ] ----------------------------------------------------------------

# Alif Compiler Source Code
add_subdirectory(${AlifProject_SOURCE_DIR}/src Alif)
