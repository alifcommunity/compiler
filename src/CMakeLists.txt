
add_definitions( -DINSTALL_PREFIX=${CMAKE_INSTALL_PREFIX} )

# Info
set(Info_Title "Alif Compiler")
set(Info_Source "alif.cpp")
set(Info_App "alif")

# Working directory for Debugin & Testing
set(WD_Windows "C:\\Alif3")

# --[ Alif ] -------------------------------------------------------------------

link_directories(${AlifProject_BIN_DIR}/alif)

# --[ International Components for Unicode (ICU) ] -----------------------------

# /lib/x86_64-linux-gnu/libicuuc.a

if(UNIX)
	message("[*] Using installed copy of ICU Library on Linux.")

	find_package(ICU 60.0 COMPONENTS io i18n REQUIRED)

	if(ICU_FOUND)
		include_directories(${ICU_INCLUDE_DIRS})
		link_directories(${ICU_LIBRARY_DIRS})
	else()
		message("[!] Please install ICU +66.0 libraries.")
		return()
	endif()	
else()
	message("[*] No need for ICU on Windows. Skip..")
endif()

# --[ Boost ] -----------------------------------------------------------------

message("[*] Initializing Boost for ${Info_Title}...")

set(Boost_NO_WARN_NEW_VERSIONS ON)

if (WIN32)
	set(BOOST_ROOT ${Boost_Root})
	set(Boost_LIBRARY_DIR ${Boost_Lib})
	message("[*] Using Boost root: ${BOOST_ROOT}")
	message("[*] Using Boost lib: ${Boost_LIBRARY_DIR}")

	set(Boost_ARCHITECTURE 				"-x64")
	set(Boost_USE_STATIC_LIBS			ON)
	set(Boost_USE_MULTITHREADED			ON)
	set(Boost_USE_STATIC_RUNTIME		ON)
	# set(Boost_COMPILER				"-mgw8")

	# set(Boost_DETAILED_FAILURE_MSG	OFF)
	# set(Boost_DEBUG					OFF)	
	# set(Boost_THREADAPI             	"win32")
	# set(Boost_COMPILER				"-vc142")
	# add_definitions( -DBOOST_ALL_NO_LIB )
else()
    # set(Boost_DEBUG 					OFF)
    set(Boost_USE_STATIC_LIBS       	OFF)
    set(Boost_USE_MULTITHREADED			ON)
    set(Boost_USE_STATIC_RUNTIME		OFF)
    # set(Boost_ARCHITECTURE			"-x64")
    # set(Boost_THREADAPI				"pthread")
    # set(Boost_DETAILED_FAILURE_MSG	ON)
    # set(Boost_COMPILER				"-gcc")
	add_definitions( -DBOOST_ALL_NO_LIB )
endif()

message("[*] Searching for Boost libs...")

find_package(Boost ${Boost_Version_Needed} REQUIRED COMPONENTS date_time filesystem regex nowide program_options locale thread)

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
else()
	message("[!] Please install Boost lib.")
	return()
endif()

# --[ pThread ] ---------------------------------------------------------------

if(UNIX)
	message("[*] Using POSIX threading (pThread).")
	set(THREADS_PREFER_PTHREAD_FLAG ON)
	find_package(Threads REQUIRED)
endif (UNIX)

# --[ Target ] ------------------------------------------------------------------

add_executable(${Info_App} ${Info_Source})

if(UNIX)
	# message("[*] Setting working directory to ${WD_Linux}...")
	# set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WD_Linux})
	# set(EXECUTABLE_OUTPUT_PATH ${WD_Linux})
	# set_target_properties(${Info_App} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${WD_Linux})
	# set_target_properties(${Info_App} PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WD_Linux})
	# set_target_properties(${Info_App} PROPERTIES EXECUTABLE_OUTPUT_PATH_DEBUG ${WD_Linux})
	# set_target_properties(${Info_App} PROPERTIES CMAKE_EXECUTABLE_OUTPUT_PATH ${WD_Linux})
else()
	message("[*] Setting working directory to ${WD_Windows}...")
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WD_Windows})
	set(EXECUTABLE_OUTPUT_PATH ${WD_Windows})
	set_target_properties(${Info_App} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${WD_Windows})
	set_target_properties(${Info_App} PROPERTIES CMAKE_RUNTIME_OUTPUT_DIRECTORY ${WD_Windows})
	set_target_properties(${Info_App} PROPERTIES EXECUTABLE_OUTPUT_PATH_DEBUG ${WD_Windows})
	set_target_properties(${Info_App} PROPERTIES CMAKE_EXECUTABLE_OUTPUT_PATH ${WD_Windows})
endif ()

# --[ Compiler flags ] ----------------------------------------------------------

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

	# using Clang
	if(UNIX)
		message("[*] Initializing '${Info_Title}' settings for Clang.")
	else()
		message("[!] Sorry. Alif don't support Clang on Windows, Please use Microsoft C++ compiler.")
		return()
	endif ()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

	if (WIN32)

		# using MinGW/GCC Windows
		message("[*] Initializing '${Info_Title}' settings for MinGW/GCC (Windows).")

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj -fvisibility=hidden -w -O3 -std=gnu++17 -march=armv8-a -Wno-deprecated -municode")
		target_link_libraries(${Info_App} ${Boost_LIBRARIES} iconv pthread -static)

	else()

		# using GCC Linux
		message("[*] Initializing '${Info_Title}' settings for GCC (Linux).")

		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -w -O3 -std=gnu++17 -march=armv8-a -Wno-deprecated" )
		target_link_libraries(${Info_App} ${Boost_LIBRARIES} pthread)
		
	endif()

	# Flags
	# set_target_properties(${Info_App} PROPERTIES LINK_FLAGS_RELEASE -s)
	# set(CMAKE_CXX_FLAGS_RELEASE " -O3 ")
	# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -m64 ")
	#add_definitions(-DBOOST_SYSTEM_NO_DEPRECATED=1)
	
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Intel")

	# using Intel C++
	if(UNIX)
		message("[!] Sorry. Alif don't support Intel C++ on Linux, Please install GCC or Clang.")
	else()
		message("[!] Sorry. Alif don't support Intel C++ on Windows, Please install Microsoft C++ compiler.")
	endif ()

	return()

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

	# Microsoft build tools
	if(UNIX)
		message("[!] Sorry. Alif don't support Microsoft C++ compiler on Linux, Please install GCC or Clang.")
		return()
	else()
		message("[*] Initializing '${Info_Title}' settings for Microsoft C++ compiler.")
	endif ()

	# Flags
	add_link_options("/SUBSYSTEM:CONSOLE")
	set_target_properties(${Info_App} PROPERTIES LINK_FLAGS /SUBSYSTEM:CONSOLE)
	
endif()

# --[ Build ] -------------------------------------------------------------------

if(UNIX)
	target_link_libraries(${Info_App} ${ICU_LIBRARIES} 
	-licuuc -licudata -licui18n
	${Boost_LIBRARIES})
else()
	target_link_libraries(${Info_App} ${Boost_LIBRARIES})
endif()

# --[ Install ] -----------------------------------------------------------------

if(UNIX)
	message("[*] Initializing installation settings...")
	install (TARGETS ${Info_App}
			ARCHIVE DESTINATION lib
			LIBRARY DESTINATION lib
			RUNTIME DESTINATION bin)
	install (DIRECTORY aliflib DESTINATION lib)
endif (UNIX)

if(WIN32)
	message("[*] Initializing installation settings...")
	install (TARGETS ${Info_App} DESTINATION C:/Alif3)
	install (DIRECTORY aliflib DESTINATION C:/Alif3)
endif ()

# --[ Copy Examples ] ------------------------------------------------------------

file(COPY examples DESTINATION ${CMAKE_BINARY_DIR}/)
