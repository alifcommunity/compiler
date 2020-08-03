// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

#ifdef _WIN32

	// ------------------------------------------------
	// Alif Compiler For Windows
	// ------------------------------------------------

	#include "win32.cpp" // GCC Compiler in Windows

#elif  __APPLE__

	// ------------------------------------------------
	// Alif Compiler For Mac OS X
	// ------------------------------------------------

	#include "macos.cpp" // GCC Compiler in Mac OS X

#else

	// ------------------------------------------------
	// Alif Compiler For Linux
	// ------------------------------------------------

	#include "linux.cpp" // GCC Compiler in Linux

#endif
