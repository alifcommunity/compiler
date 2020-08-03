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
#include <chrono> // Wait..
#include <thread> // Wait..
// ----------------------------------

void CLEAN()
{
	if(DEBUG)
		return;

	string CLEAR_CMD;

	if (!THIS_IS_ALIF_C_FILE)
	{

		CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_strip.bat\" ");
		if (system(CLEAR_CMD.c_str()) != 0)
		{
			//cout << endl << "Warning: Can't clean strip Bat temp file. " << endl;
		}

		CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_strip.log\" ");
		if (system(CLEAR_CMD.c_str()) != 0)
		{
			//cout << endl << "Warning: Can't clean strip log temp file. " << endl;
		}
	}
		CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_resource.bat\" ");
		if (system(CLEAR_CMD.c_str()) != 0)
		{
			//cout << endl << "Warning: Can't clean resource Bat temp file. " << endl;
		}

		CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_resource.log\" ");
		if (system(CLEAR_CMD.c_str()) != 0)
		{
			//cout << endl << "Warning: Can't clean resource log temp file. " << endl;
		}

		CLEAR_CMD = string("del \"") + PATH_FULL_CPP + string("\" ");
		if (system(CLEAR_CMD.c_str()) != 0)
		{
			//cout << endl << "Warning: Can't clean CPP temp file. " << endl;
		}

		CLEAR_CMD = string("del \"") + PATH_FULL_RC + string("\" ");
		if (system(CLEAR_CMD.c_str()) != 0)
		{
			//cout << endl << "Warning: Can't clean RC temp file. " << endl;
		}

		CLEAR_CMD = string("del \"" + PATH_FULL_RC + ".res\" ");
		if (system(CLEAR_CMD.c_str()) != 0)
		{
			//cout << endl << "Warning: Can't clean compiled RES temp file. " << endl;
		}

    CLEAR_CMD = string("del \"") + PATH_FULL_OBJ + string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
    }

	CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_compile.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean compile log temp file. " << endl;
    }

	CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_link.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean link log temp file. " << endl;
    }

	CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_compile.bat\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean compile Bat temp file. " << endl;
    }

	CLEAR_CMD = string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_linker.bat\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean link Bat temp file. " << endl;
    }
}

void COMPILE_WIN_32BIT() // string RAND)
{
	//stringstream GCC_COMPILE_CMD;
	//stringstream GCC_RESOURCE_CMD;
	//stringstream GCC_LINK_CMD;
	
	string CMD;

	string LOG_PATH;
	string LOG_LINE8;
	string LOG_LINE8_Buffer;

	// Need file Bach to set Path=...
	// for GCC Envirenment.
	ofstream FILE_BATCH;
	string FILE_BATCH_PATH;

	// ------------------------------------------------------			
	// Compile : WX 3.1 - TDM (GCC) 5.1 C++11 32Bit
	// ------------------------------------------------------

	// "%CURRENT_PATH%\Bin\Win_32\installer\bin\g++" -finput-charset=utf-8 -c -o "%CURRENT_PATH%\Bin\Win_32\win_alifstudio.o" 
	// -O2 -mthreads -DHAVE_W32API_H -D__WXMSW__ -DwxDEBUG_LEVEL=0 -DNDEBUG -I. -I "%CURRENT_PATH%\Bin\Win_32\installer\include" 
	// -W -Wno-deprecated-declarations -DNOPCH -Wno-ctor-dtor-privacy -Os -s -fno-keep-inline-dllexport 
	// -std=gnu++11 -m32 -MT"%CURRENT_PATH%\Bin\Win_32\win_alifstudio.o" 
	// -MF"%CURRENT_PATH%\Bin\Win_32\win_alifstudio.o.d" -MD -MP "Src\alifstudio.cpp"

	CMD =	"@echo off\n"
			"Set PATH=" + PATH_ABSOLUTE + "\\bin\n"
			"\"" + PATH_FULL_GCC + "\" -finput-charset=utf-8 -c -o \"" + PATH_FULL_OBJ + "\""
			" -O2 -mthreads -DHAVE_W32API_H -D__WXMSW__ -DwxDEBUG_LEVEL=0 -DNDEBUG"
			" -I \"" + PATH_ABSOLUTE + "\\include\""
			" -W -Wno-deprecated-declarations -DNOPCH -Wno-ctor-dtor-privacy -Os -s -fno-keep-inline-dllexport"
			" -std=gnu++11 -m32 \"" + PATH_FULL_CPP + "\" 2> \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_compile.log\"";

	FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_compile.bat";

	FILE_BATCH.open (FILE_BATCH_PATH);
		FILE_BATCH << CMD << endl;
	FILE_BATCH.close();
	
	system(("\"" + FILE_BATCH_PATH + "\"").c_str());

	// ----------------------
	// Read Log File
	// ----------------------

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	LOG_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_compile.log";

	ifstream FILE_STREAM_COMPILER(LOG_PATH.c_str());

	if (!FILE_STREAM_COMPILER.is_open())
	{
		ALIF_ERROR("ERROR: Could not open compile log file : " + LOG_PATH);
		exit(EXIT_FAILURE);
	}

	while (getline(FILE_STREAM_COMPILER, LOG_LINE8))
	{
		LOG_LINE8_Buffer.append(LOG_LINE8);
	}

	if (!LOG_LINE8_Buffer.empty())
	{
		// Error while compiling source code

		if(!DEBUG)
			boost::replace_all(LOG_LINE8_Buffer, PATH_FULL_CPP + ":", "");

		ALIF_ERROR("Please report this error to the Alif Community : www.aliflang.org/community \n\nAlif Compiler " + VERSION + " - TDM-GCC (32Bit) C++11\n\nFile : " + PATH_FULL_ALIF + "\nError: " + LOG_LINE8_Buffer);
		exit(EXIT_FAILURE);
	}
	 
	// ------------------------------------------------------			
	// Resource
	// ------------------------------------------------------

	//if (!THIS_IS_ALIF_C_FILE)
	//{
		// "%CURRENT_PATH%\Bin\Win_32\installer\bin\windres.exe" -I "%CURRENT_PATH%\Bin\Win_32\installer\include" 
		// -J rc -O coff -i "%CURRENT_PATH%\Res\win_alifstudio.rc" 
		// -o "%CURRENT_PATH%\Bin\Win_32\win_alifstudio.res" --define __WXMSW__ --define wxDEBUG_LEVEL=0 
		// --define NDEBUG --define _UNICODE --define NOPCH
		
		CMD =	"@echo off\n"
				"Set PATH=" + PATH_ABSOLUTE + "\\bin\n"
				//"SLEEP 1 \n"
				"\"" + PATH_ABSOLUTE + "\\bin\\windres.exe\""
				" -I \"" + PATH_ABSOLUTE + "\\include\" -J rc -O coff"
				" -i \"" + PATH_FULL_RC + "\" -o \"" + PATH_FULL_RC + ".res\""
				" --define __WXMSW__ --define wxDEBUG_LEVEL=0 --define NDEBUG --define _UNICODE --define NOPCH 2> \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_resource.log\"";
		
		FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_resource.bat";
		
		FILE_BATCH.open (FILE_BATCH_PATH);
			FILE_BATCH << CMD << endl;
		FILE_BATCH.close();
		
		system(("\"" + FILE_BATCH_PATH + "\"").c_str());

	//}

	// ------------------------------------------------------	
	// Link
	// ------------------------------------------------------

	string GUI_CMD = "";

	if (APP_TYPE == "PC_GUI")
		GUI_CMD = " -luxtheme -Wl,--subsystem,windows -mwindows ";

	// "%CURRENT_PATH%\Bin\Win_32\installer\bin\g++" 
	// -I "%CURRENT_PATH%\Bin\Win_32\installer\include" 
	// -Os -static-libgcc -static-libstdc++ -m32 -finput-charset=utf-8 
	// -o "%CURRENT_PATH%\Bin\Win_32\alifstudio.exe" "%CURRENT_PATH%\Bin\Win_32\win_alifstudio.res" 
	// "%CURRENT_PATH%\Bin\Win_32\win_alifstudio.o" -mthreads -L"%CURRENT_PATH%\Bin\Win_32\installer\lib" 
	// -Wl,--subsystem,windows -mwindows 
	// -lwxmsw31u_alif_lib_gl -lopengl32 -lglu32 -lwxmsw31u_alif_lib_aui -lwxmsw31u_alif_lib_propgrid 
	// -lwxmsw31u_alif_lib_adv -lwxmsw31u_alif_lib_stc -lwxmsw31u_alif_lib_core  -lwxbase31u_alif_lib 
	// -lwxscintilla -lwxtiff -lwxjpeg -lwxpng -lwxzlib -lwxregexu -lwxexpat -lkernel32 -luser32 -lgdi32 
	// -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4 
	// -ladvapi32 -lversion -lwsock32 -lwininet -loleacc -luxtheme
	
	CMD =	"@echo off\n"
			"Set PATH=" + PATH_ABSOLUTE + "\\bin\n"
			//"SLEEP 1 \n"
			"\"" + PATH_FULL_GCC + "\""
			" -I \"" + PATH_ABSOLUTE + "\\include\""
			" -Os -static-libgcc -static-libstdc++ -m32 -finput-charset=utf-8"
			" -o \"" + PATH_FULL_BIN + "\" \"" + PATH_FULL_RC + ".res\""
			" \"" + PATH_FULL_OBJ + "\" -mthreads"
			" -L \"" + PATH_ABSOLUTE + "\\lib\" "
			" -lwxmsw31u_alif_lib_gl -lopengl32 -lglu32 -lwxmsw31u_alif_lib_webview -lwxmsw31u_alif_lib_aui -lwxmsw31u_alif_lib_propgrid"
			" -lwxmsw31u_alif_lib_adv -lwxmsw31u_alif_lib_stc -lwxmsw31u_alif_lib_core  -lwxbase31u_alif_lib"
			" -lwxscintilla -lwxtiff -lwxjpeg -lwxpng -lwxzlib -lwxregexu -lwxexpat -lkernel32 -luser32 -lgdi32"
			" -lcomdlg32 -lwinspool -lwinmm -lshell32 -lshlwapi -lcomctl32 -lole32 -loleaut32 -luuid -lrpcrt4"
			" -ladvapi32 -lversion -lwsock32 -lwininet -loleacc " + GUI_CMD + " 2> \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_link.log\"";
	
	FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_linker.bat";
	
	FILE_BATCH.open (FILE_BATCH_PATH);
		FILE_BATCH << CMD << endl;
	FILE_BATCH.close();
	
	system(("\"" + FILE_BATCH_PATH + "\"").c_str());

	// Read Log File
	//std::string path = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_link.log";
	//boost::replace_all(Buffer, "C:\\Alif\\bin/ld.exe:", "");

	// ----------------------
	// Read Log File
	// ----------------------

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	LOG_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_link.log";

	ifstream FILE_STREAM_LINKER(LOG_PATH.c_str());

	if (!FILE_STREAM_LINKER.is_open())
	{
		ALIF_ERROR("ERROR: Could not open linker log file : " + LOG_PATH);
		exit(EXIT_FAILURE);
	}

	while (getline(FILE_STREAM_LINKER, LOG_LINE8))
	{
		LOG_LINE8_Buffer.append(LOG_LINE8);
	}

	if (!LOG_LINE8_Buffer.empty())
	{
		// Error while compiling source code

		boost::replace_all(LOG_LINE8_Buffer, "C:\\Alif\\bin/ld.exe:", "");

		ALIF_ERROR("Please report this error to the Alif Community : www.aliflang.org/community \n\nAlif Compiler " + VERSION + " - TDM-GCC (32Bit) C++11\n\nFile : " + PATH_FULL_ALIF + "\nError (Linker): " + LOG_LINE8_Buffer);
		exit(EXIT_FAILURE);
	}
	
	// ------------------------------------------------------	
	// Striping
	// ------------------------------------------------------

	if (!THIS_IS_ALIF_C_FILE)
	{
		// "%ALIF_CURRENT_PATH%\Win\_alif\bin\strip" --strip-all "Win\alifstudio.exe" 
		
		CMD =	"@echo off\n"
				"Set PATH=" + PATH_ABSOLUTE + "\\bin\n"
				"\"" + PATH_ABSOLUTE + "\\bin\\strip.exe\""
				" --strip-all \"" + PATH_FULL_BIN + "\" 2> \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM + "_strip.log\"";
		
		FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_strip.bat";
		
		FILE_BATCH.open (FILE_BATCH_PATH);
			FILE_BATCH << CMD << endl;
		FILE_BATCH.close();
		
		system(("\"" + FILE_BATCH_PATH + "\"").c_str());

		/*
		FILE_BATCH_PATH = PATH_ABSOLUTE + "\\_alif\\tmp\\" + RAND + "_strip.bat";
		FILE_BATCH.open (FILE_BATCH_PATH);
		
		FILE_BATCH << "@echo off \n\n"
						<< "Set PATH=" + PATH_ABSOLUTE + "\\_alif\\bin" + "\n\n" // SET PATH_ABSOLUTE
						
						<< "\"" + PATH_ABSOLUTE + "\\_alif\\bin\\strip.exe\"" // Strip PATH_ABSOLUTE
						<< " " // SPACE
						<< "--strip-all"
						<< " " // SPACE
						<< "\"" + PATH_FULL_BIN + "\\" + PATH_BIN_FILE_NAME // EXE PATH_ABSOLUTE
						<< ".exe\""
						<< " " // SPACE
						<< "2> \"" << PATH_FULL_CPP << ".exe.strip.log\" "; // ERRORS OUTPUTE PATH_ABSOLUTE
						
						//<< " \n\n pause"; // DEBUG
						
		FILE_BATCH.close();
		*/

		//system(("\"" + FILE_BATCH_PATH + "\"").c_str());

		//_wsystem(CMD_BATCH.c_str());
		
		// Read log (PATH_FULL_CPP.exe.log) if error..
	}
	
	// ------------------------------------------------------			
	// Clean
	// ------------------------------------------------------
	
	CLEAN();
}
