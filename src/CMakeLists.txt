
# Info
set(Info_Title "Alif Compiler")
set(Info_Source "alif.cpp")
set(Info_App "alif")

# Working directory for Debugin & Testing
set(WD_Windows "C:\\Alif3")
set(WD_Linux "/bin")

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
	message("[!] Not settings needed for ICU on Windows.")
endif()

# --[ Boost ] -------------------------------------------------------------------

if(UNIX)

	# Linux
	message("[*] Using installed copy of Boost for Linux.")

else()

	# Windows
	message("[*] Using embedded copy of Boost for Windows.")
	include_directories(${AlifProject_SOURCE_DIR}/thirdparty/boost/include)
	link_directories(${AlifProject_SOURCE_DIR}/thirdparty/boost/lib)
	Set(Boost_INCLUDE_DIR ${AlifProject_SOURCE_DIR}/thirdparty/boost/include)
	Set(Boost_LIBRARY_DIR ${AlifProject_SOURCE_DIR}/thirdparty/boost/lib)
	Set(Boost_LIBRARY_DIR_RELEASE ${AlifProject_SOURCE_DIR}/thirdparty/boost/lib)

endif ()

if(CMAKE_BUILD_TYPE STREQUAL Release)
	message("[*] Initializing Boost for Release build...")
	set(Boost_DEBUG					OFF)
	set(Boost_USE_DEBUG_LIBS		OFF)
	set(Boost_USE_MULTITHREADED		ON)
	set(Boost_USE_RELEASE_LIBS		ON)
	set(Boost_USE_STATIC_LIBS		ON)
	set(Boost_USE_STATIC_RUNTIME	ON)
else()
	message("[*] Initializing Boost for Debug build...")
	set(Boost_DEBUG					OFF)
	set(Boost_USE_DEBUG_LIBS		ON)
	set(Boost_USE_MULTITHREADED		ON)
	set(Boost_USE_RELEASE_LIBS		ON)
	set(Boost_USE_STATIC_LIBS		ON)
	set(Boost_USE_STATIC_RUNTIME	OFF)
endif()

if(UNIX)
	message("[*] Searching for Boost Linux...")
	find_package(Boost 1.70.0 REQUIRED COMPONENTS program_options locale filesystem)
else()
	message("[*] Searching for Boost Windows...")
	find_package(Boost 1.70.0 REQUIRED COMPONENTS program_options locale filesystem)
endif ()

if(Boost_FOUND)
	include_directories(${Boost_INCLUDE_DIRS})
	link_directories(${Boost_LIBRARY_DIRS})
else()
	message("[!] Please install Boost +1.70 libraries (static).")
	return()
endif()

# if (NOT FO_BOOST_STATIC_LINK)
# 	message("[*] Boost static ?")
# 	add_definitions(-DBOOST_ALL_NO_LIB -DBOOST_ALL_DYN_LINK -DBOOST_LOG_DYN_LINK)
# endif()

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

	# using GCC
	if(UNIX)
		message("[*] Initializing '${Info_Title}' settings for GCC.")
	else()
		message("[!] Sorry. Alif don't support GCC/MinGW on Windows, Please install Microsoft C++ compiler.")
		return()
	endif ()

	# Flags
	# set_target_properties(${Info_App} PROPERTIES LINK_FLAGS_RELEASE -s)
	# set(CMAKE_CXX_FLAGS_RELEASE " -O3 ")
	# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -m64 ")

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
	install (DIRECTORY ${AlifProject_SOURCE_DIR}/aliflib DESTINATION lib)
endif (UNIX)
