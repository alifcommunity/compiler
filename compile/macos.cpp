// ==================================
// (C)2019 DRAGA Hassan.
// 	www.aliflang.org
// ----------------------------------
// Alif Programming Language
// ==================================
#pragma once
#define UNICODE
#define _UNICODE
using namespace std;
// ----------------------------------
#include <sstream> // system, stringstream
// ----------------------------------

void CLEAN()
{
    if(DEBUG)
		return;

    string CLEAR_CMD = string("rm -rf \"") + PATH_FULL_CPP + string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean CPP temp file. " << endl;
    }

    CLEAR_CMD = string("rm -rf \"") + PATH_FULL_OBJ + string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
    }

	CLEAR_CMD = string("rm -rf \"") + PATH_FULL_CPP + string(".log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean compile log temp file. " << endl;
    }

	CLEAR_CMD = string("rm -rf \"") + PATH_FULL_BIN_TMP + string(".log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean link log temp file. " << endl;
    }

	CLEAR_CMD = string("rm -rf \"") + PATH_FULL_BIN_TMP + string(".strip.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean strip log temp file. " << endl;
    }
}

// ----------------------------------

void COMPILE_MAC_64BIT() //string RAND, string PATH_FULL_ICO, string APP_PLIST)
{
	string MAC_CMD;

	// ------------------------------------------------------			
	// macOS Compile
	// SDK 10.12 - CMD 10.11 - Minimum 10.9
	// ------------------------------------------------------

	// clang++ -mmacosx-version-min=10.9 -m64 -O2 -std=c++11 -c -o 
	// "alifstudio.o" -D__WXOSX_COCOA__ -Wall -Wundef -Wunused-parameter 
	// -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations 
	// -D_FILE_OFFSET_BITS=64 -DwxDEBUG_LEVEL=0 -I"/Library/Application Support/Aliflang/Alif_Lib_WX/lib/wx/include/osx_cocoa-unicode-static-3.1-alif" 
	// -I"/Library/Application Support/Aliflang/Alif_Lib_WX/include" -fno-common "Src/alifstudio.cpp"

	MAC_CMD =	"clang++ -mmacosx-version-min=10.9 -m64 -O2 -std=c++11 -c -o "
				"\"" + PATH_FULL_OBJ + "\" -D__WXOSX_COCOA__ -Wall -Wundef -Wunused-parameter "
				"-Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations "
				"-D_FILE_OFFSET_BITS=64 -DwxDEBUG_LEVEL=0 -I\"/Library/Application Support/Aliflang/Alif_Lib_WX/lib/wx/include/osx_cocoa-unicode-static-3.1-alif\" "
				"-I\"/Library/Application Support/Aliflang/Alif_Lib_WX/include\" -fno-common \"" + PATH_FULL_CPP + "\" "
				"2> \"" + PATH_FULL_CPP + ".log\" ";
	
	if (system(MAC_CMD.c_str()) != 0)
    {
        cout << endl << "ERROR [CORE001]: Can't compile generated code. " << endl;
        exit(EXIT_FAILURE);
    }

	//TODO: Read log for error..

	// ------------------------------------------------------			
	// Resource
	// ------------------------------------------------------

	// Nothing in macOS
	// All Resource are inside .app folder

	// ------------------------------------------------------	
	// Link
	// ------------------------------------------------------

	string GUI_CMD = "";

	if (APP_TYPE == "PC_GUI")
		GUI_CMD = " ............. ";

	// clang++ -m64 -mmacosx-version-min=10.9 -o "alifstudio" "alifstudio.o" 
	// -L"/Library/Application Support/Aliflang/Alif_Lib_WX/lib" -framework IOKit 
	// -framework Carbon -framework Cocoa -framework AudioToolbox -framework System -framework OpenGL 
	// -lwx_osx_cocoau_alif_aui-3.1 -lwx_osx_cocoau_alif_propgrid-3.1 -lwx_osx_cocoau_alif_adv-3.1 
	// -lwx_osx_cocoau_alif_stc-3.1  -lwx_osx_cocoau_alif_core-3.1  -lwx_baseu_alif-3.1  -lwxscintilla_alif-3.1   
	// -lwxtiff_alif-3.1 -lwxjpeg_alif-3.1 -lwxpng_alif-3.1  -framework WebKit -lwxzlib_alif-3.1 -lwxregexu_alif-3.1 
	// -lwxexpat_alif-3.1 -framework IOKit -framework Carbon -framework Cocoa -framework AudioToolbox -framework System 
	// -framework OpenGL  -framework Security -lpthread -liconv  -framework Security -lpthread -liconv

	MAC_CMD = 	"clang++ -m64 -mmacosx-version-min=10.9 -o \"" + PATH_FULL_BIN_TMP + "\" \"" + PATH_FULL_OBJ + "\" "
				"-L\"/Library/Application Support/Aliflang/Alif_Lib_WX/lib\" -framework IOKit "
				"-framework Carbon -framework Cocoa -framework AudioToolbox -framework System -framework OpenGL "
				"-lwx_osx_cocoau_alif_aui-3.1 -lwx_osx_cocoau_alif_propgrid-3.1 -lwx_osx_cocoau_alif_adv-3.1 "
				"-lwx_osx_cocoau_alif_stc-3.1  -lwx_osx_cocoau_alif_core-3.1  -lwx_baseu_alif-3.1  -lwxscintilla_alif-3.1 "
				"-lwxtiff_alif-3.1 -lwxjpeg_alif-3.1 -lwxpng_alif-3.1  -framework WebKit -lwxzlib_alif-3.1 -lwxregexu_alif-3.1 "
				"-lwxexpat_alif-3.1 -framework IOKit -framework Carbon -framework Cocoa -framework AudioToolbox -framework System "
				"-framework OpenGL -framework Security -lpthread -liconv  -framework Security -lpthread -liconv "
				"2> \"" + PATH_FULL_BIN_TMP + ".log\" ";
	
	if (system(MAC_CMD.c_str()) != 0)
    {
        cout << endl << "ERROR [CORE001]: Can't Link generated code. " << endl;
        exit(EXIT_FAILURE);
    }

	// ------------------------------------------------------	
	// Striping
	// ------------------------------------------------------

	MAC_CMD = 	"strip \"" + PATH_FULL_BIN_TMP + "\" 2> \"" + PATH_FULL_BIN_TMP + ".strip.log\" ";

	if (system(MAC_CMD.c_str()) != 0)
    {
        cout << endl << "ERROR [CORE001]: Can't Strip. " << endl;
        exit(EXIT_FAILURE);
    }
	
	// ------------------------------------------------------	
	// Generate bundle Application
	// ------------------------------------------------------

	if (APP_TYPE == "PC_GUI")
	{
		//SetFile -t APPL MyAlif
		
		//mkdir -p MyAlif.app
		//mkdir -p MyAlif.app/Contents
		//mkdir -p MyAlif.app/Contents/MacOS
		//mkdir -p MyAlif.app/Contents/Resources
		
		//cp MyAlif MyAlif.app/Contents/MacOS/alif
		
		//cp alif.icns MyAlif.app/Contents/Resources/alif.icns
		
		//cp Info.plist MyAlif.app/Contents/Info.plist
		
		//touch MyAlif.app
		
		MAC_CMD = "SetFile -t APPL \"" + PATH_FULL_BIN_TMP + "\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "mkdir -p \"" + PATH_FULL_BIN + "\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "mkdir -p \"" + PATH_FULL_BIN + "/Contents\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "mkdir -p \"" + PATH_FULL_BIN + "/Contents/MacOS\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "mkdir -p \"" + PATH_FULL_BIN + "/Contents/Resources\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "cp \"" + PATH_FULL_BIN_TMP + "\" \"" + PATH_FULL_BIN + "/Contents/MacOS/alif\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "cp \"" + PATH_FULL_ICO + "\" \"" + PATH_FULL_BIN + "/Contents/Resources/alif.icns\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "cp \"" + PATH_FULL_PLIST + "\" \"" + PATH_FULL_BIN + "/Contents/Info.plist\" ";
		system(MAC_CMD.c_str());
		
		MAC_CMD = "touch \"" + PATH_FULL_BIN + "\" ";
		system(MAC_CMD.c_str());

		CLEAN();
	}
	else
	{
		// macOS Console

		MAC_CMD = "cp \"" + PATH_FULL_BIN_TMP + "\" \"" + PATH_FULL_BIN + "\" ";
		system(MAC_CMD.c_str());
	}
}
