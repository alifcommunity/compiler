/*
  The Alif Programming Language
  Version 3.x Series
  (C)2021 Hassan DRAGA
  www.aliflang.org

  This file is part of Alif compiler.

  Alif compiler is free software; you can redistribute it and/or modify it
  under the terms of the GNU General Public License as published by the Free
  Software Foundation; either version 3, or (at your option) any later
  version.

  Alif compiler is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
  for more details.

  You should have received a copy of the GNU General Public License
  along with Alif compiler; see the file COPYING3. If not see
  <http://www.gnu.org/licenses/>.
*/


#ifdef _WIN32

// ------------------------------------------------
// Alif Compiler For Windows
// ------------------------------------------------

void CLEAN() {
    if (DEBUG) return;

    std::string CLEAR_CMD;

    if (!THIS_IS_ALIF_C_FILE) {
        CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" +
                                RANDOM + "_strip.bat\" ");
        if (system(CLEAR_CMD.c_str()) != 0) {
            // cout << endl << "Warning: Can't clean strip Bat temp file. " <<
            // endl;
        }

        CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" +
                                RANDOM + "_strip.log\" ");
        if (system(CLEAR_CMD.c_str()) != 0) {
            // cout << endl << "Warning: Can't clean strip log temp file. " <<
            // endl;
        }
    }
    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_resource.bat\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean resource Bat temp file. " <<
        // endl;
    }

    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_resource.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean resource log temp file. " <<
        // endl;
    }

    CLEAR_CMD = std::string("del \"") + PATH_FULL_CPP + std::string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean CPP temp file. " << endl;
    }

    CLEAR_CMD = std::string("del \"") + PATH_FULL_RC + std::string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean RC temp file. " << endl;
    }

    CLEAR_CMD = std::string("del \"" + PATH_FULL_RC + ".res\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean compiled RES temp file. " <<
        // endl;
    }

    CLEAR_CMD = std::string("del \"") + PATH_FULL_OBJ + std::string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
    }

    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_compile.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean compile log temp file. " <<
        // endl;
    }

    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_link.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean link log temp file. " << endl;
    }

    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_compile.bat\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean compile Bat temp file. " <<
        // endl;
    }

    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_linker.bat\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean link Bat temp file. " << endl;
    }
}

void compile_win64() {
    // stringstream GCC_COMPILE_CMD;
    // stringstream GCC_RESOURCE_CMD;
    // stringstream GCC_LINK_CMD;

    std::string CMD;

    std::string LOG_PATH;
    std::string LOG_LINE8;
    std::string LOG_LINE8_Buffer;

    // Need file Bach to set Path=...
    // for GCC Envirenment.
    boost::nowide::ofstream FILE_BATCH;
    std::string FILE_BATCH_PATH;

    // Compile: WebUI - TDM (GCC) 9.2 C++17 64Bit - Windows

    // ALIF_ERROR("PATH_FULL_CPP: " + PATH_FULL_CPP);

    // CMD =	"@echo off\n"
    // 		"Set PATH=" + PATH_ABSOLUTE + "\\gcc\\bin\n"
    // 		" \"" + cc_path_full + "\" -Wa,-mbig-obj -fvisibility=hidden
    // -Wall -O3 -std=gnu++17 -O3 -DNDEBUG -c -o \"" + PATH_FULL_OBJ + "\" "
    // " -I\"" +
    // PATH_ABSOLUTE + "\\boost\\include\" " 		" -I\"" + PATH_ABSOLUTE
    // +
    // "\\aliflib\" " 		" \"" + PATH_FULL_CPP + "\" " 		" 2> \""
    // + PATH_TEMP +
    // "\\alifcompiler_" + RANDOM + "_compile.log\" ";

    CMD =
        "@echo off\n"
        "Set PATH=" +
        PATH_ABSOLUTE +
        "\\gcc\\bin\n"
        " \"" +
        cc_path_full + "\" -c -o \"" + PATH_FULL_OBJ +
        "\" "
        " -I\"" +
        PATH_ABSOLUTE +
        "\\boost\\include\" "
        " -I\"" +
        PATH_ABSOLUTE +
        "\\aliflib\" "
        " \"" +
        PATH_FULL_CPP + "\" " + Compile_ExtraCompile + " 2> \"" + PATH_TEMP +
        "\\alifcompiler_" + RANDOM + "_compile.log\" ";

    if (DEBUG) LogMessage("Compile script (Windows) : " + CMD + " \n");

    FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_compile.bat";

    FILE_BATCH.open(FILE_BATCH_PATH);
    FILE_BATCH << CMD << endl;
    FILE_BATCH.close();

    system(("\"" + FILE_BATCH_PATH + "\"").c_str());

    // ----------------------
    // Read Log File
    // ----------------------

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    LOG_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_compile.log";

    boost::nowide::ifstream FILE_STREAM_COMPILER(LOG_PATH);

    if (!FILE_STREAM_COMPILER.is_open()) {
        ALIF_ERROR("ERROR: Could not open compile log file: " + LOG_PATH);
        AlifExitFailure();
    }

    while (getline(FILE_STREAM_COMPILER, LOG_LINE8)) {
        LOG_LINE8_Buffer.append(LOG_LINE8);
    }

    FILE_STREAM_COMPILER.close();

    if (!LOG_LINE8_Buffer.empty()) {
        // Error while compiling source code

        if (!DEBUG)
            boost::replace_all(LOG_LINE8_Buffer, PATH_FULL_CPP + ":", "");

        ALIF_ERROR(
            "Please report this error to the Alif Community: "
            "https://github.com/alifcommunity/compiler/issues \n\nAlif "
            "Compiler " +
            VERSION + " - Windows\n\nFile : " + PATH_FULL_ALIF +
            "\nError: " + LOG_LINE8_Buffer);
        AlifExitFailure();
    }

    // ------------------------------------------------------
    // Resource
    // ------------------------------------------------------

    CMD =
        "@echo off\n"
        "Set PATH=" +
        PATH_ABSOLUTE +
        "\\gcc\\bin\n"
        //"SLEEP 1 \n"
        "\"" +
        PATH_ABSOLUTE +
        "\\gcc\\bin\\windres.exe\""
        " -J rc -O coff -i \"" +
        PATH_FULL_RC + "\" -o \"" + PATH_FULL_RC +
        ".res\" --define _UNICODE --define NOPCH 2> \"" + PATH_TEMP +
        "\\alifcompiler_" + RANDOM + "_resource.log\"";

    FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_resource.bat";

    FILE_BATCH.open(FILE_BATCH_PATH);
    FILE_BATCH << CMD << endl;
    FILE_BATCH.close();

    system(("\"" + FILE_BATCH_PATH + "\"").c_str());

    // ------------------------------------------------------
    // Link
    // ------------------------------------------------------

    // CMD =	"@echo off\n"
    // 		"Set PATH=" + PATH_ABSOLUTE + "\\gcc\\bin\n"
    // 		//"SLEEP 1 \n"
    // 		"\"" + cc_path_full + "\""
    // 		" -Os -static-libgcc -static-libstdc++ -static -m64
    // -finput-charset=utf-8 -mthreads -o \"" + PATH_FULL_BIN + "\" \"" +
    // PATH_FULL_RC + ".res\" \"" + PATH_FULL_OBJ + "\" -L\"" + PATH_ABSOLUTE +
    // "\\boost\\lib\" -L\"" + PATH_ABSOLUTE + "\\aliflib\" -lwebui
    // -lboost_thread-mgw8-mt-s-x64-1_76 -lboost_date_time-mgw8-mt-s-x64-1_76
    // -lboost_filesystem-mgw8-mt-s-x64-1_76 -lboost_regex-mgw8-mt-s-x64-1_76
    // -lws2_32 -lwsock32 "
    // 		+ GUI_CMD + " 2> \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
    // "_link.log\"";

    CMD =
        "@echo off\n"
        "Set PATH=" +
        PATH_ABSOLUTE +
        "\\gcc\\bin\n"
        //"SLEEP 1 \n"
        "\"" +
        cc_path_full + "\" -o \"" + PATH_FULL_BIN + "\" \"" + PATH_FULL_RC +
        ".res\" \"" + PATH_FULL_OBJ + "\" -L\"" + PATH_ABSOLUTE +
        "\\boost\\lib\" -L\"" + PATH_ABSOLUTE + "\\aliflib\" " +
        Compile_ExtraLink + " 2> \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
        "_link.log\"";

    if (DEBUG) LogMessage("Link script (Windows) : " + CMD + " \n");

    FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_linker.bat";

    FILE_BATCH.open(FILE_BATCH_PATH);
    FILE_BATCH << CMD << endl;
    FILE_BATCH.close();

    system(("\"" + FILE_BATCH_PATH + "\"").c_str());

    // Read Log File
    // std::string path = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_link.log";
    // boost::replace_all(Buffer, "C:\\Alif\\bin/ld.exe:", "");

    // ----------------------
    // Read Log File
    // ----------------------

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    LOG_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_link.log";

    boost::nowide::ifstream FILE_STREAM_LINKER(LOG_PATH);

    if (!FILE_STREAM_LINKER.is_open()) {
        ALIF_ERROR("ERROR: Could not open linker log file : " + LOG_PATH);
        AlifExitFailure();
    }

    while (getline(FILE_STREAM_LINKER, LOG_LINE8)) {
        LOG_LINE8_Buffer.append(LOG_LINE8);
    }

    FILE_STREAM_LINKER.close();

    if (!LOG_LINE8_Buffer.empty()) {
        // Error while compiling source code

        boost::replace_all(LOG_LINE8_Buffer, "C:\\Alif\\bin/ld.exe:", "");

        ALIF_ERROR(
            "Please report this error to the Alif Community: "
            "https://github.com/alifcommunity/compiler/issues \n\nAlif "
            "Compiler " +
            VERSION + " - Windows\n\nFile : " + PATH_FULL_ALIF +
            "\nError (Linker): " + LOG_LINE8_Buffer +
            "\nLinker command (Windows): \n" + CMD);
        AlifExitFailure();
    }

    // ------------------------------------------------------
    // Striping
    // ------------------------------------------------------

    if (!THIS_IS_ALIF_C_FILE) {
        CMD =
            "@echo off\n"
            "Set PATH=" +
            PATH_ABSOLUTE +
            "\\gcc\\bin\n"
            "\"" +
            PATH_ABSOLUTE +
            "\\gcc\\bin\\strip.exe\""
            " --strip-all \"" +
            PATH_FULL_BIN + "\" 2> \"" + PATH_TEMP + "\\alifcompiler_" +
            RANDOM + "_strip.log\"";

        FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_strip.bat";

        FILE_BATCH.open(FILE_BATCH_PATH);
        FILE_BATCH << CMD << endl;
        FILE_BATCH.close();

        system(("\"" + FILE_BATCH_PATH + "\"").c_str());

        /*
        FILE_BATCH_PATH = PATH_ABSOLUTE + "\\_alif\\tmp\\" + RAND +
        "_strip.bat"; FILE_BATCH.open (FILE_BATCH_PATH);

        FILE_BATCH << "@echo off \n\n"
                                        << "Set PATH=" + PATH_ABSOLUTE +
        "\\_alif\\bin" + "\n\n" // SET PATH_ABSOLUTE

                                        << "\"" + PATH_ABSOLUTE +
        "\\_alif\\bin\\strip.exe\"" // Strip PATH_ABSOLUTE
                                        << " " // SPACE
                                        << "--strip-all"
                                        << " " // SPACE
                                        << "\"" + PATH_FULL_BIN + "\\" +
        PATH_BIN_FILE_NAME // EXE PATH_ABSOLUTE
                                        << ".exe\""
                                        << " " // SPACE
                                        << "2> \"" << PATH_FULL_CPP <<
        ".exe.strip.log\" "; // ERRORS OUTPUTE PATH_ABSOLUTE

                                        //<< " \n\n pause"; // DEBUG

        FILE_BATCH.close();
        */

        // system(("\"" + FILE_BATCH_PATH + "\"").c_str());

        //_wsystem(CMD_BATCH.c_str());

        // Read log (PATH_FULL_CPP.exe.log) if error..
    }

    // ------------------------------------------------------
    // Clean
    // ------------------------------------------------------

    CLEAN();
}

#elif __APPLE__

// ------------------------------------------------
// Alif Compiler For Mac OS X
// ------------------------------------------------

void CLEAN() {
    if (DEBUG) return;

    std::string CLEAR_CMD =
        std::string("rm -rf \"") + PATH_FULL_CPP + std::string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean CPP temp file. " << endl;
    }

    CLEAR_CMD = std::string("rm -rf \"") + PATH_FULL_OBJ + std::string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
    }

    CLEAR_CMD =
        std::string("rm -rf \"") + PATH_FULL_CPP + std::string(".log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean compile log temp file. " <<
        // endl;
    }

    CLEAR_CMD =
        std::string("rm -rf \"") + PATH_FULL_BIN_TMP + std::string(".log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean link log temp file. " << endl;
    }

    CLEAR_CMD = std::string("rm -rf \"") + PATH_FULL_BIN_TMP +
                std::string(".strip.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean strip log temp file. " << endl;
    }
}

// ----------------------------------

void COMPILE_MAC_64BIT()  // std::string RAND, std::string PATH_FULL_ICO,
                          // std::string APP_PLIST)
{
    std::string MAC_CMD;

    // ------------------------------------------------------
    // macOS Compile
    // SDK 10.12 - CMD 10.11 - Minimum 10.9
    // ------------------------------------------------------

    // clang++ -mmacosx-version-min=10.9 -m64 -O2 -std=c++11 -c -o
    // "alifstudio.o" -D__WXOSX_COCOA__ -Wall -Wundef -Wunused-parameter
    // -Wno-ctor-dtor-privacy -Woverloaded-virtual -Wno-deprecated-declarations
    // -D_FILE_OFFSET_BITS=64 -DwxDEBUG_LEVEL=0 -I"/Library/Application
    // Support/Aliflang/Alif_Lib_WX/lib/wx/include/osx_cocoa-unicode-static-3.1-alif"
    // -I"/Library/Application Support/Aliflang/Alif_Lib_WX/include" -fno-common
    // "Src/alifstudio.cpp"

    MAC_CMD = "clang++ -mmacosx-version-min=10.9 -m64 -O2 -std=c++11 -c -o "
            "\"" +
            PATH_FULL_OBJ +
            "\" -D__WXOSX_COCOA__ -Wall -Wundef -Wunused-parameter "
            "-Wno-ctor-dtor-privacy -Woverloaded-virtual "
            "-Wno-deprecated-declarations -D_FILE_OFFSET_BITS=64 "
            "-I" XSTR(INSTALL_PREFIX) "/lib/aliflib" // Alif lib path on macOS
                                      "  -fno-common \"" +
            PATH_FULL_CPP +
            "\" "
            "2> \"" +
            PATH_FULL_CPP + ".log\" ";

    if (system(MAC_CMD.c_str()) != 0) {
        cout << endl
             << "ERROR [CORE001]: Can't compile generated code. " << endl;
        AlifExitFailure();
    }

    // TODO: Read log for error..

    // ------------------------------------------------------
    // Resource
    // ------------------------------------------------------

    // Nothing in macOS
    // All Resource are inside .app folder

    // ------------------------------------------------------
    // Link
    // ------------------------------------------------------

    std::string GUI_CMD = "";

    if (APP_TYPE == "PC_GUI") GUI_CMD = " ............. ";

    // clang++ -m64 -mmacosx-version-min=10.9 -o "alifstudio" "alifstudio.o"
    // -L"/Library/Application Support/Aliflang/Alif_Lib_WX/lib" -framework
    // IOKit -framework Carbon -framework Cocoa -framework AudioToolbox
    // -framework System -framework OpenGL -lwx_osx_cocoau_alif_aui-3.1
    // -lwx_osx_cocoau_alif_propgrid-3.1 -lwx_osx_cocoau_alif_adv-3.1
    // -lwx_osx_cocoau_alif_stc-3.1  -lwx_osx_cocoau_alif_core-3.1
    // -lwx_baseu_alif-3.1  -lwxscintilla_alif-3.1 -lwxtiff_alif-3.1
    // -lwxjpeg_alif-3.1 -lwxpng_alif-3.1  -framework WebKit -lwxzlib_alif-3.1
    // -lwxregexu_alif-3.1 -lwxexpat_alif-3.1 -framework IOKit -framework Carbon
    // -framework Cocoa -framework AudioToolbox -framework System -framework
    // OpenGL  -framework Security -lpthread -liconv  -framework Security
    // -lpthread -liconv

    MAC_CMD = "clang++ -m64 -mmacosx-version-min=10.9 -o \"" + PATH_FULL_BIN_TMP +
            "\" \"" + PATH_FULL_OBJ +
            "\" "
            " -framework IOKit "
            "-framework Carbon -framework Cocoa -framework AudioToolbox "
            "-framework System -framework OpenGL "
            " -L" XSTR(INSTALL_PREFIX) "/lib/aliflib -lwebui_linux_x86_64 "
                                       "-framework WebKit  "
                                       "-framework IOKit -framework Carbon "
                                       "-framework Cocoa -framework "
                                       "AudioToolbox -framework System "
                                       "-framework OpenGL -framework "
                                       "Security -lpthread -liconv  -framework "
                                       "Security -lpthread -liconv "
                                       " -lboost_filesystem -lboost_iostreams "
                                       "2> \"" +
            PATH_FULL_BIN_TMP + ".log\" ";

    if (system(MAC_CMD.c_str()) != 0) {
        cout << endl << "ERROR [CORE001]: Can't Link generated code. " << endl;
        AlifExitFailure();
    }

    // ------------------------------------------------------
    // Striping
    // ------------------------------------------------------

    MAC_CMD = "strip \"" + PATH_FULL_BIN_TMP + "\" 2> \"" + PATH_FULL_BIN_TMP +
              ".strip.log\" ";

    if (system(MAC_CMD.c_str()) != 0) {
        cout << endl << "ERROR [CORE001]: Can't Strip. " << endl;
        AlifExitFailure();
    }

    // ------------------------------------------------------
    // Generate bundle Application
    // ------------------------------------------------------

    if (APP_TYPE == "PC_GUI") {
        // SetFile -t APPL MyAlif

        // mkdir -p MyAlif.app
        // mkdir -p MyAlif.app/Contents
        // mkdir -p MyAlif.app/Contents/MacOS
        // mkdir -p MyAlif.app/Contents/Resources

        // cp MyAlif MyAlif.app/Contents/MacOS/alif

        // cp alif.icns MyAlif.app/Contents/Resources/alif.icns

        // cp Info.plist MyAlif.app/Contents/Info.plist

        // touch MyAlif.app

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

        MAC_CMD = "cp \"" + PATH_FULL_BIN_TMP + "\" \"" + PATH_FULL_BIN +
                  "/Contents/MacOS/alif\" ";
        system(MAC_CMD.c_str());

        MAC_CMD = "cp \"" + PATH_FULL_ICO + "\" \"" + PATH_FULL_BIN +
                  "/Contents/Resources/alif.icns\" ";
        system(MAC_CMD.c_str());

        MAC_CMD = "cp \"" + PATH_FULL_PLIST + "\" \"" + PATH_FULL_BIN +
                  "/Contents/Info.plist\" ";
        system(MAC_CMD.c_str());

        MAC_CMD = "touch \"" + PATH_FULL_BIN + "\" ";
        system(MAC_CMD.c_str());

        CLEAN();
    } else {
        // macOS Console

        MAC_CMD = "cp \"" + PATH_FULL_BIN_TMP + "\" \"" + PATH_FULL_BIN + "\" ";
        system(MAC_CMD.c_str());
    }
}

#else

// ------------------------------------------------
// Alif Compiler For Linux
// ------------------------------------------------

void CLEAN() {
    if (DEBUG) return;

    std::string CLEAR_CMD =
        std::string("rm -rf \"") + PATH_FULL_CPP + std::string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean CPP temp file. " << endl;
    }

    CLEAR_CMD = std::string("rm -rf \"") + PATH_FULL_OBJ + std::string("\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
    }

    CLEAR_CMD = std::string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" +
                            RANDOM + "_compile.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean compile log temp file. " <<
        // endl;
    }

    CLEAR_CMD = std::string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" +
                            RANDOM + "_link.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean link log temp file. " << endl;
    }

    CLEAR_CMD = std::string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" +
                            RANDOM + "_strip.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
        // cout << endl << "Warning: Can't clean strip log temp file. " << endl;
    }
}

void compile_linux64() {
    std::string LINUX_CMD;

    std::string LOG_PATH;
    std::string LOG_LINE8;
    std::string LOG_LINE8_Buffer;

    // ------------------------------------------------------
    // Compile - WX3 Master - GTK2 - x86_64 - Optimized
    // ------------------------------------------------------

    // -std=gnu++17 -m64 -finput-charset=utf-8 -O3 -pthread -DNDEBUG
    // -fvisibility=hidden -lfto -fno-fat-lto-objects -DBOOST_ALL_NO_LIB
    // -DBOOST_FILESYSTEM_DYN_LINK

    // LINUX_CMD = "g++ -Wall -std=gnu++17 -m64 -finput-charset=utf-8 -O3
    // -pthread -DNDEBUG -fvisibility=hidden -c -o \"" + PATH_FULL_OBJ + "\" "
    // " -I\"/usr/local/include\" "		// Standard "
    // -I\"/usr/local/lib/aliflib\" "	// Alif lib 			" \"" +
    // PATH_FULL_CPP
    // + "\"
    // " 			" 2> \"" + PATH_TEMP + "/alifcompiler_" + RANDOM
    // +
    // "_compile.log\" ";

    LINUX_CMD = "g++ -c -o \"" + PATH_FULL_OBJ + "\" \"" + PATH_FULL_CPP +
                "\" -I \"" XSTR(INSTALL_PREFIX) "/lib/aliflib\" " +
                Compile_ExtraCompile + " 2> \"" + PATH_TEMP + "/alifcompiler_" +
                RANDOM + "_compile.log\" ";

    if (DEBUG) LogMessage("Compile command (Linux) : " + LINUX_CMD + " \n");
    // std::cout << "\n\n--------------\nCompile: |" << LINUX_CMD <<
    // "|\n--------------\n";

    system(LINUX_CMD.c_str());

    // ----------------------
    // Read Log File
    // ----------------------

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));

    LOG_PATH = PATH_TEMP + "/alifcompiler_" + RANDOM + "_compile.log";

    boost::nowide::ifstream FILE_STREAM_COMPILER(LOG_PATH);

    if (!FILE_STREAM_COMPILER.is_open()) {
        ALIF_ERROR("ERROR: Could not open compile log file : " + LOG_PATH);
        AlifExitFailure();
    }

    while (getline(FILE_STREAM_COMPILER, LOG_LINE8)) {
        LOG_LINE8_Buffer.append(LOG_LINE8);
    }

    FILE_STREAM_COMPILER.close();

    if (!LOG_LINE8_Buffer.empty()) {
        // Error while compiling source code

        if (!DEBUG)
            boost::replace_all(LOG_LINE8_Buffer, PATH_FULL_CPP + ":", "");

        ALIF_ERROR(
            "Please report this error to the Alif Community: "
            "https://github.com/alifcommunity/compiler/issues \n\nAlif "
            "Compiler " +
            VERSION + " - Linux\n\nFile : " + PATH_FULL_ALIF +
            "\nError: " + LOG_LINE8_Buffer + "\nCompile command: " + LINUX_CMD);
        AlifExitFailure();
    }

    // ------------------------------------------------------
    // Resource
    // ------------------------------------------------------

    // ...

    // ------------------------------------------------------
    // Link
    // ------------------------------------------------------

    std::string GUI_CMD = "";

    // if (APP_TYPE == "PC_GUI")
    // 	GUI_CMD = " ............. ";

    // -L\"/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu" -lpython3.5

    // LINUX_CMD =	"g++ -Os -m64 -finput-charset=utf-8 -pthread -o \"" +
    // PATH_FULL_BIN + "\" \"" + PATH_FULL_OBJ + "\" "
    // 			"-L\"/usr/local/lib/aliflib\" -lwebui
    // -lboost_filesystem";

    LINUX_CMD = "g++ -o \"" + PATH_FULL_BIN + "\" \"" + PATH_FULL_OBJ +
                "\" -L\"" XSTR(INSTALL_PREFIX) "/lib/aliflib\" " +
                Compile_ExtraLink + " ";  // TODO: Add 2 > log

    if (DEBUG) LogMessage("Link command (Linux) : " + LINUX_CMD + " \n");

    // std::cout << "\n\n--------------\nLink: |" << LINUX_CMD <<
    // "|\n--------------\n";
    // g++
    // -Os -m64 -finput-charset=utf-8 -pthread -lboost_filesystem -lboost_thread
    // -Os -m64 -finput-charset=utf-8 -pthread -lboost_filesystem -lboost_thread
    // -L"/usr/local/lib/aliflib" -o "test" "/tmp/alifcompiler_1007.o"

    if (system(LINUX_CMD.c_str()) != 0) {
        // cout << endl << "Bug: Can't Link generated code. " << endl;
        // AlifExitFailure();

        if (DEBUG) cout << endl << "Debuging: " << LINUX_CMD.c_str() << endl;

        ALIF_ERROR(
            "Please report this error to the Alif Community: "
            "https://github.com/alifcommunity/compiler/issues \nAlif "
            "Compiler " +
            VERSION + " - Linux\n\nFile : " + PATH_FULL_ALIF +
            "\nError: Linking Librarys error.\nLink command: " + LINUX_CMD);
        AlifExitFailure();
    }

    // ------------------------------------------------------
    // Striping
    // ------------------------------------------------------

    LINUX_CMD =
        " strip --strip-all --strip-unneeded \"" + PATH_FULL_BIN + "\" ";

    if (system(LINUX_CMD.c_str()) != 0) {
        // Striping error..
    }

    // ------------------------------------------------------

    // Debugging Mode :

    // gdb -silent -ex 'set logging redirect off' -ex 'printf "\nAlif
    // Compiler 1.0
    // - GNU Debugger\n\n"' -ex 'run' -ex 'printf "\nBacktrace : \n"' -ex 'bt'
    // -ex 'printf "\nEnd.\n"' -ex 'detach' -ex 'quit' ./AlifStudio >
    // AlifDebug.txt

    CLEAN();
}

#endif

