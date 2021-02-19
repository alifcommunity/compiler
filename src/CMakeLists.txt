
# Info
set(Info_Title "Alif Compiler")
set(Info_Source "alif.cpp")
set(Info_App "alif")

# --[ Alif ] -------------------------------------------------------------------

# include_directories(${AlifProject_SOURCE_DIR}/thirdparty/utf-8)
# include_directories(${AlifProject_SOURCE_DIR}/thirdparty/webui)

link_directories(${AlifProject_BIN_DIR}/alif)

# --[ Boost ] -------------------------------------------------------------------

if(UNIX)

	# Linux
	message("[*] Using installed copy of Boost for Linux.")

else()

	# Windows
	message("[*] Using embeded copy of Boost for Windows.")
	include_directories(${AlifProject_SOURCE_DIR}/thirdparty/boost/include)
	link_directories(${AlifProject_SOURCE_DIR}/thirdparty/boost/lib)
	Set(Boost_INCLUDE_DIR ${AlifProject_SOURCE_DIR}/thirdparty/boost/include)
	Set(Boost_LIBRARY_DIR_RELEASE ${AlifProject_SOURCE_DIR}/thirdparty/boost/lib)

endif (UNIX)

if(UNIX)
	message("[*] Searching for Boost Linux...")
	find_package(Boost 1.70.0)
else()
	message("[*] Searching for Boost Windows...") # REQUIRED COMPONENTS program_options locale filesystem
	find_package(Boost 1.70.0)
endif (UNIX)

set(Boost_DEBUG 				OFF)
set(Boost_USE_STATIC_LIBS       ON)
set(Boost_USE_MULTITHREADED     OFF)
set(Boost_USE_STATIC_RUNTIME    ON)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
else()
	message("[!] Please install Boost +1.70 libraries (static).")
	return()
endif()

# --[ Target ] ------------------------------------------------------------------

add_executable(${Info_App} ${Info_Source})

# --[ Compiler flags ] ----------------------------------------------------------

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

	if(UNIX)
		# using Clang
		message("[*] Initializing '${Info_Title}' settings for Clang.")
	else()
		message("[!] Sorry. Alif don't support Clang on Windows, Please install MinGW-64, and build using 'cmake .. -G \"MinGW Makefiles\"'. ")
		return()
	endif (UNIX)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	# using GCC
	message("[*] Initializing '${Info_Title}' settings for GCC.")

	# Flags
	set_target_properties(${Info_App} PROPERTIES LINK_FLAGS_RELEASE -s)
	set(CMAKE_CXX_FLAGS_RELEASE " -O3 ")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -m64 ")

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")

	# using Intel C++
	if(UNIX)
		message("[!] Sorry. Alif don't support Intel C++ on Linux, Please install GCC or Clang.")
	else()
		message("[!] Sorry. Alif don't support Intel C++ on Windows, Please install MinGW-64.")
	endif (UNIX)

	return()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

	# # Microsoft build tools
	# message("[!] Sorry. Alif don't support Microsoft C++ on Windows, Please install MinGW-64, and build using 'cmake .. -G \"MinGW Makefiles\"'. ")
	# return()

	# Microsoft build tools
	message("[*] Initializing '${Info_Title}' settings for MSVC.")

	# Flags
	add_link_options("/SUBSYSTEM:CONSOLE")
	set_target_properties(${Info_App} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
	
endif()

# --[ Build ] -------------------------------------------------------------------

if(UNIX)
	target_link_libraries(${Info_App} ${Boost_LIBRARIES})
else()
	target_link_libraries(${Info_App} ${Boost_LIBRARIES})
endif()
