// ==================================
// (C)2020 DRAGA Hassan.
// 	 	www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// The Alif Programming Language
// ==================================

#define UNICODE
#define _UNICODE

#include <fstream>      // O/I files
#include <iostream>     // O/I
#include <cstdio>       // Remove files
#include <vector>       // UTF8, Vectors
#include <stdlib.h>     // mblen, mbtowc, wchar_t(C)
#include <string>       // strings
#include <sstream>      // stringstream
#include <map>          // vectors
#include <ctime>        // Initializ time, for rand()
#include <algorithm>	// Standard replace()

	// Bin2C
	#include <ctype.h>
	//#include <stdio.h>
	//#include <stdlib.h>
	//#include <string.h>

#include <boost/algorithm/string/replace.hpp>   // replace '\' by '//'
#include <boost/lexical_cast.hpp>               // log
#include <boost/algorithm/string.hpp>			// to lower case for ScanAlifJavaScript()

using namespace std;

static const string VERSION = "2.0.14";
bool DEBUG = false; // Beta

// Alif includes

#include "lib/utf8.h"               // Lib : UTF-8
#include "general/global.cpp"       // Global Vars
#include "general/headers.hpp"      // Methods Headers
#include "general/log.cpp"          // Log Methods
#include "core/core.cpp"            // Core
#include "general/setup.cpp"        // Setup
#include "generator/generator.cpp"  // Code Generator
#include "parser/alif_parser.cpp"   // Parser
#include "lexer/alif_lexer.cpp"     // Lexer
#include "compile/compile.cpp"      // 3P Compiler

// --------------------------------------------------------
// Help
// --------------------------------------------------------

void ALIF_HELP()
{
        cout << endl;

		string Options =	" -workpath=[Working Path] : \n   Set Working Path, useful if compiler can't correctly found include files.\n"
							" -printlog : \n   Print Arabic Log on Screen, also save log into the file [logfile].\n"
							" -version : \n   Print the current Alif Compiler version.";
		
        #ifdef _WIN32

			cout << "Alif Compiler " << VERSION << " - Microsoft(R) Windows(R)" << endl;
			cout << "(C)2020 DRAGA Hassan - www.aliflang.org" << endl;
			cout << "--------------------------------------------" << endl << endl;
			cout << " Usage:  alif  [source.alif] [output.exe]  [logfile.txt]  [Options...]" << endl << 
					" Usage:  alif  [source.alifc] [output.exe]  [logfile.txt]  [Options...]" << endl << endl;
			cout << " Options : " << endl << endl << Options << endl << endl;

        #elif  __APPLE__

			cout << "Alif Compiler " << VERSION << " - Apple(R) macOS(R)" << endl;
			cout << "(C)2020 DRAGA Hassan - www.aliflang.org" << endl;
			cout << "------------------------------------------" << endl << endl;
			cout << " Usage:  alif  [source.alif]  [output.app]  [logfile.txt]  [Options...]" << endl << endl;
			cout << " Options : " << endl << endl << Options << endl << endl;
			//cout << "   iPhone Application Usage:  alif [source.alifi] [output.app] [logfile]" << endl;
			//cout << " " << endl;
			//cout << "   Windows Application Usage:  Unsuported. (You need Alif Compiler Windows Version)" << endl;
			//cout << "   Linux Application Usage:  Unsuported. (You need Alif Compiler Linux Version)" << endl;
			//cout << "   Android Application Usage:  Unsuported. (You need Alif Compiler Windows Version)" << endl;

        #else

			cout << "Alif Compiler " << VERSION << " - GNU Linux (GTK2_x86_64)" << endl; // x86_64
			cout << "(C)2020 DRAGA Hassan - www.aliflang.org" << endl;
			cout << "------------------------------------------" << endl << endl;
			cout << " Usage:  alif  [source.alif]  [output.bin]  [logfile.txt]  [Options...]" << endl << endl;
			cout << " Options : " << endl << endl << Options << endl << endl;
			//cout << "   Linux 32 Application Usage:  alif [source.alif] [output] [logfile] -32" << endl;
			//cout << " " << endl;
			//cout << "   Windows Application Usage:  Unsuported. (You need Alif Compiler Windows Version)" << endl;
			//cout << "   Mac OS X Application Usage:  Unsuported. (You need Alif Compiler Mac OS X Version)" << endl;
			//cout << "   Android Application Usage:  Unsuported. (You need Alif Compiler Windows Version)" << endl;
			//cout << "   iPhone Application Usage:  Unsuported. (You need Alif Compiler Mac OS X Version)" << endl;

        #endif
}

// --------------------------------------------------------
// Main
// --------------------------------------------------------

//int main(int argc, char** argv) // Standard

#ifdef _WIN32
	//int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
	int main(int argc, char** argv) // Standard
	//int _tmain( int argc, _TCHAR* argv[] )
	//int WinMain(int argc, _TCHAR* argv[])
	//int wmain(int argc, char **argv)
	//int winMain(int argc, wchar_t **argv)
#elif  __APPLE__
	int main(int argc, char** argv) // Standard
#else
	int main(int argc, char** argv) // Standard
#endif
{
    //setlocale(LC_ALL, "en_US.UTF-8");
	setlocale(LC_ALL, "ar_SA.UTF-8");

    // ------------------------------------------------------
    // Check cmd
    // ------------------------------------------------------

	string ARG_BUFFER;

	// alif --version
	if (argc == 2)
	{
		ARG_BUFFER = argv[1];

		if (ARG_BUFFER == "--version" || 
			ARG_BUFFER == "-version" || 
			ARG_BUFFER == "--v" || 
			ARG_BUFFER == "-v") // --version
		{
			cout << "Alif Compiler " << VERSION << endl;
			exit(EXIT_SUCCESS);
		}
		else
		{
			ALIF_HELP();
			exit(EXIT_FAILURE);
		}
	}

    if (argc < 4)
    {
		ALIF_HELP();
        exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------
    // Pars CMD
    // ------------------------------------------------------
	
	//check -o, -m32, -m64, -silent, -path=".." ...

	// alif  [source] [output] [logfile]
	PATH_FULL_ALIF = argv[1];
	PATH_FULL_BIN = argv[2];
	PATH_FULL_LOG = argv[3];
	
	if (argc > 4)
	{
		// alif  [source] [output] [logfile] [option1=val1] [option2=val2]

		for (int i = 4; i < argc; i++)
		{
			ARG_BUFFER = argv[i];

			if (ARG_BUFFER == "--version" || ARG_BUFFER == "-version") // --version
			{
				cout << "Alif Compiler " << VERSION << endl << endl;
				//exit(EXIT_SUCCESS);
			}
			else if (ARG_BUFFER.substr(0,10) == "-workpath=") // -workpath="aaa/bbb/ccc/"
			{
				// Setting Working Path.

				PATH_WORKING = GET_PATH_WITHOUT_LAST_SEPARATION(ARG_BUFFER.substr(10));
			}
			else if (ARG_BUFFER == "-beta") // -beta
			{
				// Setting Beta Log

				DEBUG = true;
			}
			else if (ARG_BUFFER == "-printlog") // -beta
			{
				// Setting Log On Screen

				ERROR_PRINT_ON_SCREEN = true;
			}
			else
			{
				ALIF_ERROR("ERROR: Unknown Option : " + ARG_BUFFER);
				exit(EXIT_FAILURE);
			}
		}
	}
	
	// ------------------------------------------------------
	// Check Files extention
	// ------------------------------------------------------

	if (PATH_FULL_ALIF.substr(PATH_FULL_ALIF.find_last_of(".") + 1) == "alifc"){
		THIS_IS_ALIF_C_FILE = true;
	}
	else if (PATH_FULL_ALIF.substr(PATH_FULL_ALIF.find_last_of(".") + 1) != "alif")
	{
		ALIF_ERROR("Unsupported file extention : ' " + PATH_FULL_ALIF + " ' ");
		exit(EXIT_FAILURE);
	}

	// ------------------------------------------------------
	// Check Setup Installation
	// ------------------------------------------------------
	
	if (!CHECK_SETUP())
	{
		ALIF_ERROR("Please re-install Alif Compiler, or download the latest version from \n\nwww.aliflang.org");
		exit(EXIT_FAILURE);
	}

	// ------------------------------------------------------
	// Alif Library
	// ------------------------------------------------------

	if (THIS_IS_ALIF_C_FILE)
	{
		// This is C++ pure file
		// there no Alif Source code
		// so, we must call compile method directly

		std::ifstream  src(PATH_FULL_ALIF, std::ios::binary);
		std::ofstream  dst(PATH_FULL_CPP, std::ios::binary);
		dst << src.rdbuf();

		//PATH_FULL_CPP = PATH_WORKING + PATH_FULL_ALIF; // This is more quickly than copy files !
		//boost::filesystem::copy_file(PATH_FULL_ALIF, PATH_FULL_CPP, boost::filesystem::copy_option::overwrite_if_exists);
	}
	else
	{
		// This is Alif Source Code

		// ------------------------------------------------------
		// Alif Lib Setting (to fix arab file name problem)
		// ------------------------------------------------------

		ALIF_LIB_SETTING();

		// ------------------------------------------------------
		// CPP CODE INITIALIZATION
		// ------------------------------------------------------
		
		// Set basic value to variables
		// to run a minimal app (Title, With, Heint, WebUI..)
		CG_INITIALIZATION();

		// ------------------------------------------------------
		// ALIF VARIABLES INITIALIZATION
		// ------------------------------------------------------
		
		// Set Initialisation of general alif variables..
		// Main window set, alif flag, inside fun or win..
		ALIF_VAR_INITIALIZATION_FOR_NEW_SOURCE_FILE(true);

		// ------------------------------------------------------
		// The Alif Standard Library
		// ------------------------------------------------------
		
		if(DEBUG)LogMessage("\n ----------- The Alif Standard Library ------------- \n");

			AlifLexerParser("alifstandardlib", "ALIFLIB", false, false); // Global Tokens Predefinition.
			AlifLexerParser("alifstandardlib", "ALIFLIB", false, true);  // Full Syntaxt Checking.

		if(DEBUG)LogMessage("\n ----------- Standard Library FINISH --------------- \n");

		// ------------------------------------------------------
		// Lexer / Parcer for User source code
		// ------------------------------------------------------
		
		// Read source file..
		// LEXER	:	Create a list of tokens
		// PARSER	:	Read list of tokens, Generate a C++ code

		AlifLexerParser(PATH_FULL_ALIF, "ALIF", true, false);
	}

	// ------------------------------------------------------
	// Check if App Runing
	// ------------------------------------------------------

	#ifdef _WIN32

		string CLEAR_BIN_CMD = string("@echo off & del /s \"") + PATH_FULL_BIN + string("\" >nul 2>&1");

		if (system(CLEAR_BIN_CMD.c_str()) != 0)
		{
			cout << endl << "Warning: Can't execut Windows remove EXE output command" << endl;
		}

		//boost::filesystem::remove(PATH_FULL_BIN);
		//std::remove(PATH_FULL_BIN);

	#elif  __APPLE__
		
		string CLEAR_BIN_CMD = string("rm -rf \"") + PATH_FULL_BIN + string("\" 2> /dev/null");

		if (system(CLEAR_BIN_CMD.c_str()) != 0)
		{
			cout << endl << "Warning: Can't execut macOS remove App output command" << endl;
		}

	#else
		
		string CLEAR_BIN_CMD = string("rm -f \"") + PATH_FULL_BIN + string("\" 2> /dev/null");

		if (system(CLEAR_BIN_CMD.c_str()) != 0)
		{
			cout << endl << "Warning: Can't execut Linux remove Bin output command" << endl;
		}

	#endif

	#ifdef __APPLE__
		if (CHECK_FILE_EXISTE(PATH_FULL_BIN + "/Contents/MacOS/alif"))
	#else
		if (CHECK_FILE_EXISTE(PATH_FULL_BIN))
	#endif
	{
		ALIF_ERROR("\nأسف، لم تنجح عملية مسح هدا الملف، جرب غلق التطبيق: " + PATH_FULL_BIN);
		exit(EXIT_FAILURE);
	}

	// ------------------------------------------------------
	// Save debug message
	// ------------------------------------------------------
	
	if (!THIS_IS_ALIF_C_FILE)
	{
		if(DEBUG)
			PATH_FULL_LOG_SAVE();
	}
	
	// ++++++++
	// Debug
	//PATH_FULL_LOG_SAVE();
	//ALIF_ERROR("Compiling... ");
	//exit(EXIT_SUCCESS);
	// ++++++++
	
#ifdef _WIN32
	// ======================================================

	if (!THIS_IS_ALIF_C_FILE)
	{
		// Get GENERATED C++ CODE

		ofstream FILE_CPP;
		FILE_CPP.open (PATH_FULL_CPP); // .c_str());

		if (!FILE_CPP.is_open())
		{
			ALIF_ERROR("ERROR: Could not create CPP temp file. " + PATH_FULL_CPP);
			exit(EXIT_FAILURE);
		}

		FILE_CPP << CODE_GENERATOR_GET_PC_GUI();
		FILE_CPP.close();
	}

	// app icon
	if (!PATH_FULL_ICO.empty())
		boost::replace_all(PATH_FULL_ICO, "\\", "\\\\");

	// GENERATE Windows Resource CODE
	ofstream FILE_RESOURCE;
	FILE_RESOURCE.open (PATH_FULL_RC); // .c_str());
	if (!FILE_RESOURCE.is_open())
	{
		ALIF_ERROR("ERROR: Could not create RC temp files. ");
		exit(EXIT_FAILURE);
	}
		FILE_RESOURCE << "#include <wx/msw/wx.rc> \n";
		FILE_RESOURCE << "aaaa ICON \"" + PATH_FULL_ICO + "\"";
	FILE_RESOURCE.close();

	// Compile
	COMPILE_WIN_32BIT();

	// ======================================================

#elif  __APPLE__

	// app icon
	if (!PATH_FULL_ICO.empty())
		boost::replace_all(PATH_FULL_ICO, "\\", "\\\\");

	if (!THIS_IS_ALIF_C_FILE)
	{
		// GENERATE C++ CODE
		
		ofstream FILE_CPP;
		FILE_CPP.open (PATH_FULL_CPP); // .c_str());
		if (!FILE_CPP.is_open())
		{
			ALIF_ERROR("ERROR [F004]: Could not create CPP temp files. ");
			exit(EXIT_FAILURE);
		}
			FILE_CPP << CODE_GENERATOR_GET_PC_GUI();
		FILE_CPP.close();
	}

	// Compile
	COMPILE_MAC_64BIT();

	// ======================================================
	
#else // Linux

	if (!THIS_IS_ALIF_C_FILE)
	{
		// GENERATE C++ CODE
		
		ofstream FILE_CPP;
		FILE_CPP.open (PATH_FULL_CPP); // .c_str());
		if (!FILE_CPP.is_open())
		{
			ALIF_ERROR("ERROR [F004]: Could not create CPP temp files. ");
			exit(EXIT_FAILURE);
		}
			FILE_CPP << CODE_GENERATOR_GET_PC_GUI();
		FILE_CPP.close();
	}

	// Compile
	COMPILE_LINUX_64BIT();

	// ======================================================

#endif
	
	// Hamdo li ALLAH =)
    exit(EXIT_SUCCESS);
}

