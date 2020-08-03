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

// TODO: in linux.. maybe GTK2 need bcs is shared !
// Fresh Ubuntu :
// alif@alif-pc:~/Desktop$ alif test.alif test test.log -beta
// /usr/bin/ld: cannot find -lgtk-x11-2.0
// /usr/bin/ld: cannot find -lgdk-x11-2.0
// /usr/bin/ld: cannot find -lpangocairo-1.0
// /usr/bin/ld: cannot find -latk-1.0
// /usr/bin/ld: cannot find -lcairo
// /usr/bin/ld: cannot find -lgdk_pixbuf-2.0
// /usr/bin/ld: cannot find -lgio-2.0
// /usr/bin/ld: cannot find -lpangoft2-1.0
// /usr/bin/ld: cannot find -lpango-1.0
// /usr/bin/ld: cannot find -lgobject-2.0
// /usr/bin/ld: cannot find -lfontconfig
// /usr/bin/ld: cannot find -lfreetype
// /usr/bin/ld: cannot find -lgthread-2.0
// /usr/bin/ld: cannot find -lglib-2.0
// /usr/bin/ld: cannot find -lX11
// /usr/bin/ld: cannot find -lXxf86vm
// /usr/bin/ld: cannot find -lSM
// /usr/bin/ld: cannot find -lgtk-x11-2.0
// /usr/bin/ld: cannot find -lgdk-x11-2.0
// /usr/bin/ld: cannot find -lpangocairo-1.0
// /usr/bin/ld: cannot find -latk-1.0
// /usr/bin/ld: cannot find -lcairo
// /usr/bin/ld: cannot find -lgdk_pixbuf-2.0
// /usr/bin/ld: cannot find -lgio-2.0
// /usr/bin/ld: cannot find -lpangoft2-1.0
// /usr/bin/ld: cannot find -lpango-1.0
// /usr/bin/ld: cannot find -lgobject-2.0
// /usr/bin/ld: cannot find -lglib-2.0
// /usr/bin/ld: cannot find -lfontconfig
// /usr/bin/ld: cannot find -lfreetype
// collect2: error: ld returned 1 exit status
// ERROR [CORE001]: Can't Link generated code. 
// alif@alif-pc:~/Desktop$ 

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

	CLEAR_CMD = string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" + RANDOM + "_compile.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean compile log temp file. " << endl;
    }

	CLEAR_CMD = string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" + RANDOM + "_link.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean link log temp file. " << endl;
    }

	CLEAR_CMD = string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" + RANDOM + "_strip.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0)
    {
        //cout << endl << "Warning: Can't clean strip log temp file. " << endl;
    }
}

void COMPILE_LINUX_64BIT() // string RAND)
{
    string LINUX_CMD;

    string LOG_PATH;
	string LOG_LINE8;
	string LOG_LINE8_Buffer;

    // ------------------------------------------------------
    // Compile - WX3 Master - GTK2 - x86_64 - Optimized
    // ------------------------------------------------------

    // Compiling...
    // g++ -c -o "Bin/Linux_64/alifstudio.o" -Wno-undef -Wno-unused-parameter -D__WXGTK__ 
    // -Wno-ctor-dtor-privacy -Woverloaded-virtual  -D_FILE_OFFSET_BITS=64 -DwxDEBUG_LEVEL=0  
    // -I"/usr/local/include/aliflibwx" -I"Src" -pthread -m64 -DG_DISABLE_CAST_CHECKS $cmd 
    // -m64 -std=c++11 "Src/alifstudio.cpp"
 
    LINUX_CMD = "g++ -c -o \"" + PATH_FULL_OBJ + "\" -Wno-undef -Wno-unused-parameter "
                      "-D__WXGTK__ -Wno-ctor-dtor-privacy -Woverloaded-virtual -D_FILE_OFFSET_BITS=64 "
                      "-DwxDEBUG_LEVEL=0 -I\"/usr/local/include/aliflibwx\" -pthread -m64 -DG_DISABLE_CAST_CHECKS -O2 -m64 -std=c++11 " + Compile_ExtraCompile + " \"" +
                      PATH_FULL_CPP + "\" 2> \"" + PATH_TEMP + "/alifcompiler_" + RANDOM + "_compile.log\" ";

    system(LINUX_CMD.c_str());

    // ----------------------
	// Read Log File
	// ----------------------

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

	LOG_PATH = PATH_TEMP + "/alifcompiler_" + RANDOM + "_compile.log";

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

		ALIF_ERROR("Please report this error to the Alif Community : www.aliflang.org/community \n\nAlif Compiler " + VERSION + " - Linux C++11\n\nFile : " + PATH_FULL_ALIF + "\nError: " + LOG_LINE8_Buffer);
		exit(EXIT_FAILURE);
	}

    // ------------------------------------------------------
    // Resource
    // ------------------------------------------------------

    // ...

    // ------------------------------------------------------
    // Link
    // ------------------------------------------------------

    string GUI_CMD = "";

	if (APP_TYPE == "PC_GUI")
		GUI_CMD = " ............. ";

    // g++ -o "Bin/Linux_64/alifstudio" "Bin/Linux_64/alifstudio.o" -L"/usr/local/lib/aliflibwx" -pthread -m64 -static-libgcc -static-libstdc++ -lwx_gtk2u_alif_webview-3.1 -lwx_gtk2u_alif_propgrid-3.1 -lwx_gtk2u_alif_aui-3.1 -lwebkitgtk-1.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lpangoft2-1.0 -lpango-1.0 -lfontconfig -lfreetype -lsoup-2.4 -lgio-2.0 -lgobject-2.0 -ljavascriptcoregtk-1.0 -lglib-2.0 -lwx_gtk2u_alif_stc-3.1 -lwx_gtk2u_alif_core-3.1 -lwx_baseu_alif-3.1 -lwxscintilla_alif-3.1 -lwxtiff_alif-3.1 -lwxjpeg_alif-3.1 -lwxpng_alif-3.1  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lfontconfig -lfreetype -lgthread-2.0 -pthread -lglib-2.0 -lX11 -lXxf86vm -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lwxzlib_alif-3.1 -lwxregexu_alif-3.1 -lwxexpat_alif-3.1 -ldl -lm  -ldl -lm $cmd

    // -L\"/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu" -lpython3.5
    
    LINUX_CMD = "g++ -o \"" + PATH_FULL_BIN + "\" \"" + PATH_FULL_OBJ + "\"  "
                      "-L\"/usr/local/lib/aliflibwx\" -lwx_gtk2u_alif_webview-3.1 -lwx_gtk2u_alif_propgrid-3.1 -lwx_gtk2u_alif_aui-3.1 -lwebkitgtk-1.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lpangoft2-1.0 -lpango-1.0 -lfontconfig -lfreetype -lsoup-2.4 -lgio-2.0 -lgobject-2.0 -ljavascriptcoregtk-1.0 -lglib-2.0 -lwx_gtk2u_alif_stc-3.1 -lwx_gtk2u_alif_core-3.1 -lwx_baseu_alif-3.1 -lwxscintilla_alif-3.1 -lwxtiff_alif-3.1 -lwxjpeg_alif-3.1 -lwxpng_alif-3.1  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lfontconfig -lfreetype -lgthread-2.0 -pthread -lglib-2.0 -lX11 -lXxf86vm -lSM -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lwxzlib_alif-3.1 -lwxregexu_alif-3.1 -lwxexpat_alif-3.1 "
                      "-pthread -m64 -static-libgcc -static-libstdc++ -ldl -lm -ldl -lm -O2 -L\"/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu\" " + Compile_ExtraLink;

    if (system(LINUX_CMD.c_str()) != 0)
    {
        // cout << endl << "Bug: Can't Link generated code. " << endl;
        //exit(EXIT_FAILURE);

        if(DEBUG)
            cout << endl << "Debuging: " << LINUX_CMD.c_str() << endl;
            
        ALIF_ERROR("BUG: Please report this error to the Alif Community : www.aliflang.org/community \nAlif Compiler " + VERSION + " - Linux C++11\n\nFile : " + PATH_FULL_ALIF + "\nError: Linking Librarys error.");
		exit(EXIT_FAILURE);
    }

    // ------------------------------------------------------	
	// Striping
	// ------------------------------------------------------

    LINUX_CMD = " strip --strip-all --strip-unneeded \"" + PATH_FULL_BIN + "\" ";

    if (system(LINUX_CMD.c_str()) != 0){

        // Striping error..
    }

    // ------------------------------------------------------

    // Debugging Mode :

    // gdb -silent -ex 'set logging redirect off' -ex 'printf "\nAlif Compiler 1.0 - GNU Debugger\n\n"' -ex 'run' -ex 'printf "\nBacktrace : \n"' -ex 'bt' -ex 'printf "\nEnd.\n"' -ex 'detach' -ex 'quit' ./AlifStudio > AlifDebug.txt

    CLEAN();
}
