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

#define ALIF_VERSION "3.0.33 (Beta)"
#define XSTR(a) STR(a)
#define STR(a) #a

// Stack ********************************************************

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#define _HAS_STD_BYTE 0 // Fixing 'byte': ambiguous symbol

// OS Include ***************************************************

#ifdef _WIN32
#define _WINSOCKAPI_
#include <direct.h> // getcwd
#include <tchar.h>
#include <windows.h>
#include <winsock2.h>
#define GetCurrentDir _getcwd
#include <stdlib.h>
#elif __APPLE__
// ...
#else
// ...
#endif

// C++ Include **************************************************

#include <algorithm> // Standard replace()
#include <chrono>    // Wait..
#include <codecvt> // warning C4244: 'argument': conversion from 'wchar_t' to 'const _Elem', possible loss of data
#include <cstdio>  // Remove files
#include <ctime>   // Initializ time, for rand()
#include <ctype.h> //
#include <fstream> // O/I files
#include <iostream> // O/I
#include <iterator> //
#include <locale> // warning C4244: 'argument': conversion from 'wchar_t' to 'const _Elem', possible loss of data
#include <map>    // vectors
#include <sstream>  // stringstream
#include <stdlib.h> // mblen, mbtowc, wchar_t(C)
#include <string>   // strings
#include <thread>   // Treading
#include <vector>   // UTF8, Vectors

// Thirdparty ***************************************************

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/beast/core/detail/base64.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>
#include <boost/program_options.hpp>

#include <boost/nowide/args.hpp>
#include <boost/nowide/fstream.hpp>
#include <boost/nowide/iostream.hpp>

// UTF-8
#include "utf8.h"

// Alif v2 ******************************************************

using namespace std;
bool DEBUG = false;
bool SyntaxOnly = false;
static const std::string VERSION = ALIF_VERSION;

// Global *******************************************************

// Alif v3

unsigned char *_base64_encode(const unsigned char *src, size_t len,
                              size_t *out_len);
unsigned char *_base64_decode(const unsigned char *src, size_t len,
                              size_t *out_len);
std::string base64_encode(std::string data);
std::string base64_decode(std::string data);

struct _settings {

  struct _os {

#ifdef _WIN32
    std::string exe_ext = ".exe";
    std::string path_sep = "\\";
#elif __APPLE__
    std::string exe_ext = ".app";
    std::string path_sep = "/";
#else
    std::string exe_ext = "";
    std::string path_sep = "/";
#endif
  } os;

} settings;

struct _argument {

  struct _input {

    std::string path = "";
    std::string filename = "";
    std::string extension = "";
    std::string fullpath = "";
    std::vector<std::string> includes;
  } input;

  struct _output {

    std::string path = "";
    std::string filename = "";
    std::string extension = "";
    std::string fullpath = "";
  } output;

  struct _log {

    std::string path = "";
    std::string filename = "";
    std::string extension = "";
    std::string fullpath = "";
  } log;

} argument;

struct _script {

  struct _main {

    bool is_set = false;
    std::string set_at_line = "0";
  } main;

} script;

// Old

// -----------------------------------------------------------
// Reserved Words
// -----------------------------------------------------------

// ألف أضف مكتبة رئيسية _س_ واجهة خاص
// نهاية كلما مجال دالة عدد نص كائن إذا و أو سطر إرجاع صنف أداة نقر زر نص
// ملصق إظهار إخفاء تدمير عنوان نص تجميد عرض محتوى ارتفاع أفصول أرتوب

static const std::string ALIF_RESERVED[] = {

    "ألف",  "أضف",  "مكتبة", "_س_",   "مجال", "إرجاع", "نهاية",
    "صنف",  "خاص",  "عدد",   "نص",    "كائن", "دالة",  "هدم",
    "بناء", "كلما", "إذا",   "أو",    "و",    "وإلا",  "سطر",
    "صحيح", "خطأ",  "كسر",   "متغير", "ثابت", "منطق",  "رئيسية"};

static const int ALIF_RESERVED_TOTAL = 33;

static const std::vector<std::string> DataType_v{"منطق", "نص", "عدد"};

// -----------------------------------------------------------
// Lexic Tokens Class
// Create Obj for every Alif Source files
// -----------------------------------------------------------

std::string TempToken[1024];
int TempTokenCount = 1;

class CLASS_TOKEN {
public:
  // ----------------------------------
  // Script Type
  // ----------------------------------

  // to block setting Alif-Namespace inside library's
  std::string ALIF_SCRIPT_TYPE;

  // ----------------------------------
  // File Path
  // ----------------------------------

  // usr/username/Desktop/myproject/src/mysource.alif
  // Used only by Lexer()
  std::string PATH_FULL_SOURCE;

  // ----------------------------------
  // Tokens
  // ----------------------------------

  std::string
      REAL_LINE[2048]; // The real Alif script line, without toknonisation !
  std::map<std::pair<int, int>, int>
      REAL_TOKEN_POSITION; // The real Alif script token position in real line !
                           // [Line Number, Token Number] = "real char number"

  bool TOKENS_PREDEFINED = false;

  std::map<std::pair<int, int>, std::string>
      TOKEN; // TOKEN [Line Number, Token Number] = "token"

  int TOTAL[2048];
  int TOTAL_LINES;
  int NUMBER;
  int Line;

  // ----------------------------------
  // Constructor
  // ----------------------------------

  CLASS_TOKEN() {
    TOTAL_LINES = 1;
    NUMBER = 1;
    Line = 1;
    ALIF_SCRIPT_TYPE = "";
    PATH_FULL_SOURCE = "";
  }
};

// ----------------------------------
// Alif Global Language Variables
// ----------------------------------

static std::string APP_TYPE = "PC_CONSOLE";

// ----------------------------------
// Flag
// ----------------------------------

static std::map<std::string, bool>
    ALIF_FLAG_FILE; // alif_flag[full_file_path] = true or false
static std::map<std::string, std::string>
    ALIF_FLAG_AT_LINE_FILE; // alif_flag[full_file_path] = at_line

// ----------------------------------
// صنف
// ----------------------------------

static std::map<std::string, bool>
    CLASS_CONSTRICTOR_FUN_IS_SET; // abc[class] = main_func true or false
static std::map<std::string, std::string>
    CLASS_CONSTRICTOR_FUN_AT_LINE; // abc[class] = main_func_at_line
static std::map<std::string, std::string>
    CLASS_CONSTRICTOR_FUN_ARG_TOTAL; // abc[class] = total args
static std::map<std::pair<std::string, int>, std::string>
    CLASS_CONSTRICTOR_FUN_ARG_TYPE; // abc[class][arg number] = arg type

static bool IsInsideClass = false;
static std::string TheClass;
static std::map<std::string, bool>
    CLASS_IS_SET; // abc['class name'] = already set ?
static std::map<std::string, std::string>
    CLASS_AT_LINE; // abc['class name'] = class at line

static std::map<std::pair<std::string, std::string>, bool>
    CLASS_FUN_IS_SET; // abc[class][fun] = class fun already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_FUN_AT_LINE; // abc[class][fun] = class fun line number
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_FUN_TYPE; // abc[class][fun] = class fun type
static std::map<std::pair<std::string, std::string>, bool>
    CLASS_FUN_PRIVATE; // abc[class][fun] = class fun private ?
static std::map<std::pair<std::string, std::string>, int>
    CLASS_FUN_ARG_TOTAL; // abc[class][fun] = global total args
static std::map<std::pair<std::string, int>, std::string>
    CLASS_FUN_ARG_TYPE; // abc[class + fun][arg number] = arg type

static std::map<std::pair<std::string, std::string>, bool>
    CLASS_G_VAR_IS_SET; // abc[class][var] = global var already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_G_VAR_AT_LINE; // abc[class][var] = global var line number
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_G_VAR_TYPE; // abc[class][var] = global var type
static std::map<std::pair<std::string, std::string>, bool>
    CLASS_G_VAR_PRIVATE; // abc[class][var] = global private ?
static std::map<std::pair<std::string, std::string>, bool>
    CLASS_G_VAR_IS_CONST; // abc[class][var] = global var Constant
static std::map<std::pair<std::string, std::string>, bool> CLASS_G_VAR_IS_ARRAY;

// ----------------------------------
// كائن
// ----------------------------------

static std::map<std::pair<std::string, std::string>, bool>
    OBJ_IS_SET; // abc[namespace + fun][obj] = already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    OBJ_AT_LINE; // abc[namespace + fun][obj] = line number
static std::map<std::pair<std::string, std::string>, std::string>
    OBJ_CLASS; // abc[namespace + fun][obj] = base class
static std::map<std::string, std::string>
    OBJ_GLOBAL_DECLARATION; // abc[class] = Obj1; Obj2;...

// ----------------------------------
// مجال
// ----------------------------------

static bool IsInsideNamespace = false;
static std::string TheNamespace;

int Namespace_Total = 0;
std::string Namespace_Total_Names[64];

// ----------------------------------
// مجال رئيسية
// ----------------------------------

// static bool 	MAIN_WIN_IS_SET = false;
// static std::string 	MAIN_WIN_AT_LINE;

// ----------------------------------
// مجال
// ----------------------------------

static std::map<std::string, bool>
    namespace_is_set; // abc['namespace_name'] = already set ?
static std::map<std::string, std::string>
    namespace_at_line; // abc['namespace_name'] = func_at_line
// static std::map<std::string, bool>		WIN_IS_WEB; 	//
// abc['namespace_name'] = Web UI ?

// ----------------------------------
// دالة
// ----------------------------------

static bool IsInsideFunction = false;
static std::string TheFunction;
static std::string TheFunction_TYPE;
static std::map<std::pair<std::string, std::string>, std::string>
    RETURN_FUN; // abc[win][fun] = return is ok ? "OK", "IF"
static bool IS_CONTROL_FUNCTION_NAME = false;

// ----------------------------------
// دالة رئيسية
// ----------------------------------

static std::map<std::string, bool>
    MAIN_FUN_IS_SET; // abc['namespace_name'] = main_func true or false
static std::map<std::string, std::string>
    MAIN_FUN_AT_LINE; // abc['namespace_name'] = main_func_at_line

// ----------------------------------
// دالة
// ----------------------------------

static std::map<std::string, bool>
    G_FUN_IS_SET; // abc[gfun] = global fun already set ?
static std::map<std::string, std::string>
    G_FUN_AT_LINE; // abc[gfun] = global fun line number
static std::map<std::string, std::string>
    G_FUN_TYPE; // abc[gfun] = global fun type
static std::map<std::string, int>
    G_FUN_ARG_TOTAL; // abc[gfun] = global total args
static std::map<std::pair<std::string, int>, std::string>
    G_FUN_ARG_TYPE; // abc[gfun][arg number] = arg type
int Global_TotalFucntion = 0;
static std::map<int, std::string>
    Global_FunctionNames; // abc[global fun number] = func name

static std::map<std::pair<std::string, std::string>, bool>
    L_FUN_IS_SET; // abc[window][fun] = local fun already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    L_FUN_AT_LINE; // abc[window][fun] = local fun line number
static std::map<std::pair<std::string, std::string>, std::string>
    L_FUN_TYPE; // abc[window][fun] = local fun type
static std::map<std::pair<std::string, std::string>, int>
    L_FUN_ARG_TOTAL; // abc[window][fun] = global total args
static std::map<std::pair<std::string, int>, std::string>
    L_FUN_ARG_TYPE; // abc[window + fun][arg number] = arg type

int Win_CurrentTotalFucntion = 0;
static std::map<std::string, int>
    Win_TotalFucntion; // abc[window] = total functions
static std::map<std::pair<std::string, int>, std::string>
    Win_FunctionNames; // abc[window][fun number] = func name

static std::string NEW_FUNCTION_ARG = "";

// -----------------------------------------------------------
// VARs
// -----------------------------------------------------------

// Local var

static std::map<std::pair<std::string, std::string>, bool>
    L_VAR_IS_SET; // abc[window + func][var] = local var already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    L_VAR_AT_LINE; // abc[window + func][var] = local var line number
static std::map<std::pair<std::string, std::string>, std::string>
    L_VAR_TYPE; // abc[window + func][var] = local var type
static std::map<std::pair<std::string, std::string>, bool>
    L_VAR_IS_CONST; // abc[window + func][var] = local var Constant
static std::map<std::pair<std::string, std::string>, bool> L_VAR_IS_ARRAY;

// Global var

static std::map<std::string, bool>
    G_VAR_IS_SET; // abc[var] = global var already set ?
static std::map<std::string, std::string>
    G_VAR_AT_LINE; // abc[var] = global var line number
static std::map<std::string, std::string>
    G_VAR_TYPE; // abc[var] = global var type
static std::map<std::string, bool>
    G_VAR_IS_CONST; // abc[var] = global var Constant
static std::map<std::string, bool> G_VAR_IS_ARRAY;
static int G_VAR_TOTAL = 0;                    // Total of global vars
static std::map<int, std::string> G_VAR_NAMES; // abc[var] = var_name

// -----------------------------------------------------------
// CONTROLS TABLE
// -----------------------------------------------------------

// static std::map<std::string, bool>
// CONTROL_WIN_IS_SET; 	// abc[win-control] = already set ? static
// std::map<std::string, std::string>
// CONTROL_WIN_AT_LINE;	// abc[win-control] = line number static
// std::map<std::pair<std::string, std::string>, bool>	CONTROL_IS_SET;
// // abc[window][control] = already set ? static
// std::map<std::pair<std::string, std::string>, std::string> CONTROL_AT_LINE;
// // abc[window][control] = line number static std::map<std::pair<std::string,
// std::string>, std::string>	CONTROL_TYPE;			//
// abc[window][control] = type

// ----------------------------------
// PARENTHESIS
// ----------------------------------

static int ALIF_PARENTHESIS_STATUS = 0;

// ----------------------------------
// IF
// ----------------------------------

static int ALIF_IF_STATUS = 0;
static bool IS_IF_SYNTAX = false;

// ----------------------------------
// LOOP
// ----------------------------------

static int ALIF_LOOP_STATUS = 0;

// -----------------------------------------------------------
// DEBUG / ERROR / LOG
// -----------------------------------------------------------

// Debug

bool DEBUG_PRINT_ON_SCREEN = false;
bool THIS_IS_ALIF_C_FILE = false;

// Error

bool ERROR_PRINT_ON_SCREEN = false;
bool ERROR_AS_JSON = false;
bool ERROR_AS_JSON64 = false;

// Log
std::string LOG_MESSAGE_FULL = "";

// -----------------------------------------------------------
// CPP BUFERs
// -----------------------------------------------------------

std::string CBUFER;
std::string CBUFER_ID;
std::string CBUFER_OBJ;
std::string ScriptSyntaxBuffer;
std::string ScriptBuffer;

// -----------------------------------------------------------
// C++ Code Inside Alif Code
// -----------------------------------------------------------

static bool LIB_INSIDE_CPP_CODE = false;
std::string LIB_LEXER_CG_BUFER;
std::string LIB_PARSER_CG_BUFER;

// -----------------------------------------------------------
// ALIF COMPILER VARs
// -----------------------------------------------------------

static std::map<std::string, bool> ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED;
static std::map<std::string, bool> ALREADY_PARSED_FILE_TOKENS_PREDEFINED;

std::string PATH_FULL_ALIF; // Desktop/myapp.alif
std::string PATH_FULL_BIN;  // Desktop/mybin.bin
std::string PATH_FULL_LOG;  // Desktop/myapp.log

std::string cc_path_full;  // Linux : 'gcc' | Win : c:\Alif\bin\gcc.exe
std::string PATH_FULL_CPP; // tmp/123456789.cpp
std::string PATH_FULL_OBJ; // tmp/123456789.o
std::string PATH_FULL_RC;  // tmp/123456789.rc
std::string
    PATH_FULL_ICO; // Mac: /usr/share/aliflang/alif.ics | Win: c:\Alif\alif.ico
#if __APPLE__
std::string PATH_FULL_PLIST;   // Mac only
std::string PATH_FULL_BIN_TMP; // Tmp/mybin.bin
#endif

std::string PATH_ABSOLUTE;
std::string PATH_WORKING;

std::string PATH_TEMP; // /usr/tmp
std::string RANDOM;    // 1234

std::string PATH_FULL_LIB_SETTING; // 'gcc' | c:\Alif\aliflib\aliflib.inf
// std::map<std::string, std::string> LIB_FILE_NAME;

// Complet path of Web UI window (HTML5) file
// Used by parser #window_web
// std::string PATH_FULL_WINDOW_WEB;

#ifdef _WIN32
std::string SEPARATION = "\\";
#else
std::string SEPARATION = "/";
#endif

// -----------------------------------------------------------
// Compile Extra commands
// -----------------------------------------------------------

std::string Compile_ExtraCompile = " "; // -I/...
std::string Compile_ExtraLink = " ";    // -L\"/...\" -lmylib...

void add_extra_arg_to_compiler(std::string cmd) {

  Compile_ExtraCompile.append(cmd);
  Compile_ExtraCompile.append(" ");
  // std::cout << "\n\n--------------\nAdd compile: |" << cmd <<
  // "|\n--------------\n";
}

void add_extra_arg_to_linker(std::string cmd) {

  Compile_ExtraLink.append(cmd);
  Compile_ExtraLink.append(" ");
  // std::cout << "\n\n--------------\nAdd link: |" << cmd <<
  // "|\n--------------\n";
}

void add_extra_arg_to_compiler_beginning(std::string cmd) {

  Compile_ExtraCompile.insert(0, (cmd + " "));
  // std::cout << "\n\n--------------\nAdd compile beginning: |" << cmd <<
  // "|\n--------------\n";
}

void add_extra_arg_to_linker_beginning(std::string cmd) {

  Compile_ExtraLink.insert(0, (cmd + " "));
  // std::cout << "\n\n--------------\nAdd link beginning: |" << cmd <<
  // "|\n--------------\n";
}

// -----------------------------------------------------------
// Python
// -----------------------------------------------------------

// -I/usr/include/python3.5
// -L\"/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu\" -lpython3.5

std::string PythonInclude_path = "";
std::string PythonLib_path = "";
std::string PythonLibName = "";

void PythonSetEnvirenment() {

  // -------------------------------
  // Python cmd: python3 -c "import sys; print(sys.path)"
  // Python includes	-> /usr/include/python3.5
  // Python Libs		->
  // /usr/lib/python3.5/config-3.5m-x86_64-linux-gnu python lib name	->
  // python3.5
  // -------------------------------

  // TODO: if thos var are empty, then get python paths by cmd.

  std::string buffer = " -I\"" + PythonInclude_path + "\" ";

  add_extra_arg_to_compiler(buffer);

  buffer = " -L\"" + PythonLib_path + "\" -l" + PythonLibName + " ";

  add_extra_arg_to_linker(buffer);
}

// Headers ******************************************************

// Core
std::string CHECK_CALL_FUN_ARG(
    bool CALL_FUN_GLOBAL,
    std::string CALL_WIN_OR_CLASS, // win1/class1 { fun1(int a) } | win2 { fun2{
                                   // عدد b; fun1(b) } } ==> win1
    std::string CALL_FUN, // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
                          // } } ==> fun1
    int CALL_IS_CLASS, // 0 = non class, 1 constructor, 2 = الدالة member, ل
                       // Message when new obj
    std::string FROM_WIN_OR_CLASS, // win1 { fun1(int a) } | win2 { fun2{ عدد b;
                                   // fun1(b) } } ==> win2
    std::string FROM_FUN, // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
                          // } } ==> fun2
    std::string SYNTAX[2048], int SYNTAX_LONG, CLASS_TOKEN *o_tokens);

// Parser
void SetNewVar(bool IsGlobal, std::string TempNS, std::string TmpFunction,
               std::string VarName, std::string VarDataType, bool IsConstant,
               bool IsArray, int Line, CLASS_TOKEN *o_tokens);

// void parser_NewWindowWeb(std::string Token[2048], CLASS_TOKEN *o_tokens);

// Lexer
void AlifLexerParser(std::string file, std::string target, bool is_first_file,
                     bool is_predefined);

// Log **********************************************************

void clean_str_json(std::string &s) {

  boost::algorithm::replace_all(s, "\r", "");
  boost::algorithm::replace_all(s, "\n", "");
  boost::algorithm::trim(s);

  if (ERROR_AS_JSON64) {

    // boost::beast::detail::base64::encode((void *)s.c_str(), (void
    // *)s.c_str(), s.length()); size_t len = 0; _base64_encode((const unsigned
    // char *) s.c_str(), s.length(), &len);
    s = base64_encode(s);
    boost::algorithm::replace_all(s, "\r", "");  // remove \r on windows
    boost::algorithm::replace_all(s, "\n", "");  // remove \n
    boost::algorithm::replace_all(s, "\\n", ""); // skip auto 72 char break
  } else {

    boost::algorithm::replace_all(s, "\\", "\\\\");
    boost::algorithm::replace_all(s, "\"", "\\\"");
  }
}

void ALIF_ERROR(std::string ERROR_DESCRIPTION) {
  cout << endl << ERROR_DESCRIPTION << endl << endl;

  boost::nowide::ofstream file;

  if (ERROR_AS_JSON) {

    clean_str_json(ERROR_DESCRIPTION);

    ERROR_DESCRIPTION =
        "{\"file\":\"\",\"line\":\"0\",\"msg\":\"" + ERROR_DESCRIPTION + "\"}";
  }

  file.open(PATH_FULL_LOG, ios::app);
  file << ERROR_DESCRIPTION;
  file.close();

  exit(EXIT_FAILURE);
}

void DEBUG_MESSAGE(std::string MESSAGE, CLASS_TOKEN *o_tokens) {
  LOG_MESSAGE_FULL.append(MESSAGE);

  if (DEBUG_PRINT_ON_SCREEN) {
    cout << MESSAGE;
  }
}

void LogMessage(std::string MESSAGE) {
  LOG_MESSAGE_FULL.append(MESSAGE);

  if (DEBUG_PRINT_ON_SCREEN) {
    cout << MESSAGE;
  }
}

void PATH_FULL_LOG_SAVE() {
  boost::nowide::ofstream file;

  file.open(PATH_FULL_LOG);
  file << LOG_MESSAGE_FULL;
  file.close();
}

void ErrorCode(std::string error_message, CLASS_TOKEN *o_tokens) {

  if (DEBUG)
    DEBUG_MESSAGE("\n ----------- DEBUGING ERROR ------------- \n", o_tokens);
  if (DEBUG)
    DEBUG_MESSAGE("Namespace : " + TheNamespace + " \n", o_tokens);
  if (DEBUG)
    DEBUG_MESSAGE("Class : " + TheClass + " \n", o_tokens);
  if (DEBUG)
    DEBUG_MESSAGE("Fun : " + TheFunction + " \n\n", o_tokens);

  std::string error_file = o_tokens->PATH_FULL_SOURCE;
  std::string error_line =
      o_tokens->Line > 0 ? boost::lexical_cast<std::string>(o_tokens->Line)
                         : "0";

  std::string ERR_MSG;

  if (ERROR_AS_JSON) {

    // JSON Format
    clean_str_json(error_message);
    clean_str_json(error_file);

    ERR_MSG = "{\"file\":\"" + error_file + "\",\"line\":\"" + error_line +
              "\",\"msg\":\"" + error_message + "\"}";
  } else {

    ERR_MSG = " الملف		: " + error_file + "\n";
    ERR_MSG.append(" السطر		: " + error_line + "\n");
    ERR_MSG.append("\n الخطأ		: " + error_message);
  }

  if (ERROR_PRINT_ON_SCREEN)
    cout << "---| Alif Debug |--------------" << endl
         << ERR_MSG << endl
         << "------------------------" << endl;

  LOG_MESSAGE_FULL.append(ERR_MSG);

  PATH_FULL_LOG_SAVE();

  // Alif Studio v2
  // Exit code must be 'success', to let IDE read from DEBUG_MESSAGE file
  // if exit_failur, so IDE read data from process output

  exit(EXIT_FAILURE);
}

// Core *********************************************************

// New

void str_arr_remove_elem(std::string *arr, int to_remove, int len) {

  for (int i = to_remove; i < len; ++i) {

    arr[i] = arr[i + 1];
  }

  arr[len] = "\0";
}

void err(std::string des) {

  std::cout << "[!] Error" << std::endl << des << std::endl;

  // IDE must redirect output to a log file.
  // no more log file like Alif v2 series.

  // boost::nowide::ofstream file;
  // file.open (PATH_FULL_LOG, ios::app);
  // 	file << des;
  // file.close();

  exit(EXIT_FAILURE);
}

std::string get_path(std::string str) { return ""; }

bool is_file_exists(std::string f) {

  // std::cout << "is_file_exists(" << f << ");" << std::endl;

  boost::nowide::ifstream infile(f);
  return infile.good();
}

void file_embed(std::string file_path, std::string &var,
                CLASS_TOKEN *o_tokens) {

  // This function can be completely be replaced in c++20 by <embed>
  // This function basically read a file and update ref by the file
  // content (const string).

  if (!is_file_exists(file_path))
    ErrorCode("ملف غير موجود : ' " + file_path + " ' ", o_tokens);

  boost::nowide::ifstream rBuffer(file_path);
  stringstream buf;
  buf << rBuffer.rdbuf();
  rBuffer.close();

  var.clear();
  // var.append(buf.str());
  var = buf.str();
}

// Old

int Generated_ID = 10000;
int ID_GENERATOR() {
  // return rand() % 10 + 10000;
  return Generated_ID++;
}

bool replace(std::string &str, const std::string &from, const std::string &to) {

  size_t start_pos = str.find(from);

  if (start_pos == std::string::npos)
    return false;

  str.replace(start_pos, from.length(), to);

  return true;
}

bool replace_end(std::string &str, const std::string &from,
                 const std::string &to) {

  size_t start_pos = str.rfind(from);

  if (start_pos == std::string::npos)
    return false;

  str.replace(start_pos, from.length(), to);

  return true;
}

std::string substr_utf8(std::string originalString, int Position, int MaxLen) {

  std::string resultString_start = originalString;
  const char *aStr_start = originalString.c_str();

  int len = 0;
  int byteCount = 0;

  if (Position > 0) {
    while (*aStr_start) {

      if ((*aStr_start & 0xc0) != 0x80) {
        len += 1;
      }

      if (len > Position) {
        resultString_start = resultString_start.substr(byteCount);
        break;
      }

      byteCount++;

      aStr_start++;
    }
  }

  if (MaxLen == 0) {

    return resultString_start;

  } else {

    // Initialization
    len = 0;
    byteCount = 0;

    std::string resultString = resultString_start;

    const char *aStr = resultString_start.c_str();

    while (*aStr) {

      if ((*aStr & 0xc0) != 0x80) {

        len += 1;
      }

      if (len > MaxLen) {

        resultString = resultString.substr(0, byteCount);
        break;
      }

      byteCount++;
      aStr++;
    }

    return resultString;
  }
}

std::string IntToString(int INT_VAL) {

  // Int --> String

  stringstream STRING_STREAM_BUFFER;
  STRING_STREAM_BUFFER << INT_VAL;
  std::string STRING_BUFFER = STRING_STREAM_BUFFER.str();
  return STRING_BUFFER;
}

std::string CONVERT_STRING_ARRAY_TO_STRING(std::string STRING_ARRAY_VAL[1024],
                                           int LONG) {
  // std::string[123] --> String
  stringstream STRING_STREAM_BUFFER;

  if (LONG < 1)
    return "";

  for (int p = 0; p <= LONG; p++) {
    STRING_STREAM_BUFFER << STRING_ARRAY_VAL[p];
  }

  std::string STRING_BUFFER = STRING_STREAM_BUFFER.str();
  return STRING_BUFFER;
}

std::string CONVERT_WCHAT_T_TO_STRING(wchar_t *WCHART_T_VAL) {
  // wchar_t --> String

  // const wstring W_STRING_BUFFER (WCHART_T_VAL);
  // const std::string STRING_BUFFER (W_STRING_BUFFER.begin(),
  // W_STRING_BUFFER.end()); return STRING_BUFFER;

  using convert_typeX = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_typeX, wchar_t> converterX;
  return converterX.to_bytes(WCHART_T_VAL);
}

std::string CONVERT_CHAR_TO_STRING(char *CHART_VAL) {
  // Char --> String
  std::string STRING_BUFFER(CHART_VAL);
  return STRING_BUFFER;
}

std::wstring s_to_ws(const std::string &str) {

  using convert_typeX = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_typeX, wchar_t> converterX;

  return converterX.from_bytes(str);
}

std::string ws_to_s(const std::wstring &wstr) {

  using convert_typeX = std::codecvt_utf8<wchar_t>;
  std::wstring_convert<convert_typeX, wchar_t> converterX;

  return converterX.to_bytes(wstr);
}

// -----------------------------------------------------------
// Bin2C
// -----------------------------------------------------------
/*
int useconst = 0;
int zeroterminated = 0;

int BinaryToC_myfgetc(FILE *f){

        int c = fgetc(f);
        if (c == EOF && zeroterminated){
                zeroterminated = 0;
                return 0;
        }
        return c;
}

char* BinaryToC_str2upr(char *s){

        char * r = (char *) calloc(strlen(s)+1, sizeof(char));

        int i = 0;
        while(*s){

                r[i] = toupper(*s);
                ++i;
                ++s;
        }
        return r;
}

void BinaryToC_process(const char *ifname, const char *ofname){

        FILE *ifile, *ofile;

        ifile = fopen(ifname, "rb");
        if (ifile == NULL) {
                //fprintf(stderr, "cannot open %s for reading\n", ifname);
                //exit(1);
                ALIF_ERROR("ERROR : cannot open file for reading -> " +
std::string(ifname));
        }

        ofile = fopen(ofname, "wb");
        if (ofile == NULL) {
                //fprintf(stderr, "cannot open %s for writing\n", ofname);
                //exit(1);
                ALIF_ERROR("ERROR : cannot open file for reading -> " +
std::string(ofname));
        }

        char buf[PATH_MAX], *p;
        const char *cp;

        if ((cp = strrchr(ifname, '/')) != NULL)
                ++cp;
        else {
                if ((cp = strrchr(ifname, '\\')) != NULL)
                ++cp;
                else
                cp = ifname;
        }

        strcpy(buf, cp);

        for (p = buf; *p != '\0'; ++p)

        if (!isalnum(*p))
                *p = '_';

        fprintf(ofile,
                        "#ifndef %s_H\n#define %s_H\nstatic %sunsigned char %s[]
= {\n", BinaryToC_str2upr(buf), BinaryToC_str2upr(buf), useconst ? "const " :
"", buf);

        int c, col = 1;
        while ((c = BinaryToC_myfgetc(ifile)) != EOF){

                if (col >= 78 - 6){
                        fputc('\n', ofile);
                        col = 1;
                }

                fprintf(ofile, "0x%.2x, ", c);
                col += 6;
        }

        fprintf(ofile, "\n};\n#endif\n");
        fclose(ifile);
        fclose(ofile);
}

void BinaryToC(std::string FileIn, std::string FileOut){

        std::string FileType = FileIn.substr(FileIn.find_last_of(".") + 1);

        if (FileType == "CSS" || FileType == "css" ||
                FileType == "JS" || FileType == "js" ||
                FileType == "TXT" || FileType == "txt"){
                // String data structure (zero terminated byte)
                zeroterminated = 1;
                useconst = 1;
        } else {
                // Raw data structure
                zeroterminated = 0;
                useconst = 1;
        }

        // bin2c -c myimage.png myimage_png.cpp
        // bin2c -z sometext.txt sometext_txt.cpp
        BinaryToC_process(FileIn.c_str(), FileOut.c_str());
}
*/

// -----------------------------------------------------------
// Bse64
// -----------------------------------------------------------

// https://github.com/ReneNyffenegger/cpp-base64/blob/master/base64.cpp

static const std::string base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                        "abcdefghijklmnopqrstuvwxyz"
                                        "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

// const std::string s = "...";
// std::string encoded = base64_encode(reinterpret_cast<const unsigned
// char*>(s.c_str()), s.length()); std::string decoded = base64_decode(encoded);

std::string base64_encode(unsigned char const *bytes_to_encode,
                          unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] =
          ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] =
          ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for (i = 0; (i < 4); i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i) {
    for (j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] =
        ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] =
        ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while ((i++ < 3))
      ret += '=';
  }

  return ret;
}
/*
std::string base64_decode(std::string const& encoded_string) {
size_t in_len = encoded_string.size();
int i = 0;
int j = 0;
int in_ = 0;
unsigned char char_array_4[4], char_array_3[3];
std::string ret;

while (in_len-- && ( encoded_string[in_] != '=') &&
is_base64(encoded_string[in_])) { char_array_4[i++] = encoded_string[in_];
in_++; if (i ==4) { for (i = 0; i <4; i++) char_array_4[i] =
base64_chars.find(char_array_4[i]) & 0xff;

        char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] &
0x30) >> 4); char_array_3[1] = ((char_array_4[1] & 0xf) << 4) +
((char_array_4[2] & 0x3c) >> 2); char_array_3[2] = ((char_array_4[2] & 0x3) <<
6) +   char_array_4[3];

        for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
        i = 0;
        }
}

if (i) {
        for (j = 0; j < i; j++)
        char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >>
4); char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c)
>> 2);

        for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
}

return ret;
}
*/

std::string BinaryToBase64(std::string FileIn) { return ""; }

// -----------------------------------------------------------
// CPP Set Variables Name
// -----------------------------------------------------------

static std::map<std::string, std::string> ID;         // abc[var] = V1000
static std::map<std::string, std::string> Global_ID;  // abc[var] = G_V1000
static std::map<std::string, std::string> Control_ID; // abc[var] = C_V1000
static std::map<std::string, std::string> Obj_ID;     // abc[var] = OBJ_V1000
static std::map<std::string, std::string>
    GlobalObj_ID; // abc[var] = G_OBJ_V1000

void SET_OBJ_C_NAME(std::string VAR) {
  if (Obj_ID[VAR] == "")
    Obj_ID[VAR] = "OBJ_" + IntToString(ID_GENERATOR());
}

void SET_GLOBAL_OBJ_C_NAME(std::string VAR) {
  if (GlobalObj_ID[VAR] == "")
    GlobalObj_ID[VAR] = "G_OBJ_" + IntToString(ID_GENERATOR());
}

void SET_C_NAME(std::string VAR) {
  if (ID[VAR] == "")
    ID[VAR] = "V_" + IntToString(ID_GENERATOR());
}

void SET_GLOBAL_C_NAME(std::string VAR) {
  if (Global_ID[VAR] == "")
    Global_ID[VAR] = "G_V_" + IntToString(ID_GENERATOR());
}

void SET_CONTROL_C_NAME(std::string VAR) {
  if (Control_ID[VAR] == "")
    Control_ID[VAR] = "C_V_" + IntToString(ID_GENERATOR());
}

bool IsInArray_v(const std::string &value,
                 const std::vector<std::string> &array) {

  return std::find(array.begin(), array.end(), value) != array.end();
}

bool IsDataType(const std::string &value) {

  return IsInArray_v(value, DataType_v);
}

// -----------------------------------------------------------
// Get UTF8 Total Leng
// -----------------------------------------------------------

int CharCount_utf8(std::string LINE8, CLASS_TOKEN *o_tokens) {

  // ------------------------------------------------------
  // check ل invalid utf-8
  // ل a simple yes/no check,
  // there is also utf8::is_valid function
  // ------------------------------------------------------

  std::string::iterator LINE_END_IT =
      utf8::find_invalid(LINE8.begin(), LINE8.end());

  if (LINE_END_IT != LINE8.end()) {
    ALIF_ERROR("ERROR : Invalid UTF-8 encoding at line " +
               IntToString(o_tokens->TOTAL_LINES));
  }

  // ------------------------------------------------------
  // تحويل line UTF8 إلى UTF32 ل fix character size
  // is needed إلى count chars in line
  // ------------------------------------------------------

  // vector<int short> LINE32;
  std::string LINE32;
  utf8::utf8to32(LINE8.begin(), LINE_END_IT, back_inserter(LINE32));

  // ------------------------------------------------------
  // Count chars
  // ------------------------------------------------------

  int LINE_LEN = LINE32.length();
  int LINE_UNIT = 0;
  int CHAR_COUNT32 = 0;
  const char *LINE_C_STR = LINE32.c_str();

  while (LINE_UNIT < LINE_LEN) {
    LINE_UNIT += mblen(&LINE_C_STR[LINE_UNIT], LINE_LEN - LINE_UNIT);
    CHAR_COUNT32 += 1;
  }

  return CHAR_COUNT32;
}

// ====================================================

std::string GET_REAL_LINE_MID(int START, int TOKEN_POSITION,
                              CLASS_TOKEN *o_tokens) {
  if (DEBUG)
    DEBUG_MESSAGE("REAL_LINE : |" + o_tokens->REAL_LINE[o_tokens->Line] +
                      "| \n",
                  o_tokens);
  if (DEBUG)
    DEBUG_MESSAGE("START : " + IntToString(START) + " \n", o_tokens);
  if (DEBUG)
    DEBUG_MESSAGE("TOKEN_POSITION : " + IntToString(TOKEN_POSITION) + " \n",
                  o_tokens);
  if (DEBUG)
    DEBUG_MESSAGE("REAL_TOKEN_POSITION : " +
                      IntToString(o_tokens->REAL_TOKEN_POSITION[std::make_pair(
                          o_tokens->Line, TOKEN_POSITION)]) +
                      " \n",
                  o_tokens);

  if (START < 0 || TOKEN_POSITION < 1) // Exception !
    return o_tokens->REAL_LINE[o_tokens->Line];

  std::string BUFFER;

  BUFFER = substr_utf8(o_tokens->REAL_LINE[o_tokens->Line], START,
                       o_tokens->REAL_TOKEN_POSITION[std::make_pair(
                           o_tokens->Line, TOKEN_POSITION)]);

  if (BUFFER.empty())
    return o_tokens->REAL_LINE[o_tokens->Line]; // Empty, So tray to return full
                                                // real line !
  else
    return BUFFER; // Return the SubStr requested.
}

// ====================================================

bool IsValidStringFormat(std::string STRING, CLASS_TOKEN *o_tokens) {
  if (substr_utf8(STRING, 0, 1) != "\"")
    return false;

  if (substr_utf8(STRING, (CharCount_utf8(STRING, o_tokens) - 1), 1) != "\"")
    return false;

  return true;
}

/*
bool IsValidStringFormatOrStringVar(std::string STRING, CLASS_TOKEN *o_tokens){

        // TODO : We need add support for global class var, global var.

        if (L_VAR_IS_SET[std::make_pair(TheNamespace + TheFunction, STRING)] ||
                L_VAR_IS_SET[std::make_pair(TheClass + TheFunction, STRING)]){

                std::string CLASS_OR_WIN;
                if (IsInsideClass)
                        CLASS_OR_WIN = TheClass;
                else
                        CLASS_OR_WIN = TheNamespace;

                if(L_VAR_TYPE[std::make_pair(CLASS_OR_WIN + TheFunction,
STRING)] != "نص") return false;
        }
        else {

                if (substr_utf8(STRING, 0, 1) != "\"")
                        return false;

                if (substr_utf8(STRING, (CharCount_utf8(STRING, o_tokens) - 1),
1) != "\"") return false;
        }

        return true;
}
*/

// ====================================================

std::string REMOVE_DOUBLE_SPACE(std::string LINE_BUFFER,
                                CLASS_TOKEN *o_tokens) {
  std::string Char;
  std::string BUFFER;

  bool INSIDE = false;
  bool VALID_CHAR = false;
  bool LAST_IS_SPACE = false;

  // Remove Extra Spaces
  // and Trim line end.
  for (int I = 0; I < CharCount_utf8(LINE_BUFFER, o_tokens); I++) {
    Char = substr_utf8(LINE_BUFFER, I, 1);

    if (Char == "\"") {
      INSIDE = !INSIDE;
      BUFFER.append("\"");
    } else if (Char == "\xEF\xBB\xBF") // BOM Signe in first char of file
    {
      if (DEBUG)
        DEBUG_MESSAGE("[BOM]", o_tokens); // LOG
    }
    // else if (Char == "\u202E") // RIGHT-TO-LEFT OVERRIDE singe, إلى force RTL
    // {
    // 	if(DEBUG)
    // 		DEBUG_MESSAGE("[RTL]", o_tokens); // LOG
    // }
    else if (Char == " " || Char == "\t") {
      if (INSIDE) {
        BUFFER.append(Char);
      } else {
        if (VALID_CHAR) {
          if (!LAST_IS_SPACE) {
            BUFFER.append(" ");
            LAST_IS_SPACE = true;
          }
        }
      }
    } else {
      VALID_CHAR = true;
      LAST_IS_SPACE = false;
      BUFFER.append(Char);
    }
  }

  // Trim line end.
  for (int D = CharCount_utf8(BUFFER, o_tokens); D > 0; D--) {
    Char = substr_utf8(BUFFER, (D - 1), 1);

    if (Char != " " && Char != "\t") {
      BUFFER = substr_utf8(BUFFER, 0, D);
      break;
    }
  }

  return BUFFER;
}

// ====================================================

bool IsValidDigit(std::string DIGIT, bool FLOAT, CLASS_TOKEN *o_tokens) {
  int I = 0;
  int TOTAL_CHAR = CharCount_utf8(DIGIT, o_tokens);
  std::string Char;
  bool First = true;
  bool DECIMAL = false;

  //  100 --> TRUE
  // -100 --> TRUE
  // -10.25 --> TRUE
  // -10,25 --> TRUE
  // -10،25 --> TRUE

  if (CharCount_utf8(DIGIT, o_tokens) > 1 &&
      (substr_utf8(DIGIT, 0, 1) == "-" || substr_utf8(DIGIT, 0, 1) == "+")) {
    I = 1;
  }

  while (I < TOTAL_CHAR) {
    Char = substr_utf8(DIGIT, I, 1);

    if (Char == "." || Char == "," || Char == "،") {
      if (First) {
        return false; // .123
      }

      if (I == (TOTAL_CHAR - 1)) {
        return false; // 123.
      }

      if (!FLOAT) {
        return false; // 12.3, but need integer not float.
      }

      if (DECIMAL) {
        return false; // 1.2.3
      }

      DECIMAL = true;
    } else if (Char != "0" && Char != "1" && Char != "2" && Char != "3" &&
               Char != "4" && Char != "5" && Char != "6" && Char != "7" &&
               Char != "8" && Char != "9") {
      return false;
    }

    I++;
    First = false;
  }

  return true;
}

// ====================================================

bool CAN_ADD_OPERATION_HERE(std::string TOKEN_LAST) {
  // = 1 + 2 * (3 / 4 - 5) + 6
  // IF

  /*
  if (IS_IF_SYNTAX)
  {
          if ((TOKEN_LAST == "=") ||
                  (TOKEN_LAST == "+") ||
                  (TOKEN_LAST == "-") ||
                  (TOKEN_LAST == "*") ||
                  (TOKEN_LAST == "/") ||

                  (TOKEN_LAST != "<") ||
                  (TOKEN_LAST != ">") ||
                  (TOKEN_LAST != "و") ||
                  (TOKEN_LAST != "أو") ||

                  (TOKEN_LAST == "("))
                  return false;
  }
  else
  {
  */
  if ((TOKEN_LAST == "=") || (TOKEN_LAST == "+") || (TOKEN_LAST == "-") ||
      (TOKEN_LAST == "*") || (TOKEN_LAST == "/") || (TOKEN_LAST == "("))
    return false;

  //}

  return true;
}

bool CAN_ADD_VAR_HERE(std::string TOKEN_LAST) {
  // str / عدد = متغير + متغير * (var / متغير - var)

  /*
  if (IS_IF_SYNTAX)
  {
          if ((TOKEN_LAST != "+") &&
                  (TOKEN_LAST != "-") &&
                  (TOKEN_LAST != "*") &&
                  (TOKEN_LAST != "/") &&
                  (TOKEN_LAST != "=") &&

                  (TOKEN_LAST != "<") &&
                  (TOKEN_LAST != ">") &&
                  (TOKEN_LAST != "و") &&
                  (TOKEN_LAST != "أو") &&

                  (TOKEN_LAST != "("))
                  return false;
  }
  else
  {
  */
  if ((TOKEN_LAST != "+") && (TOKEN_LAST != "-") && (TOKEN_LAST != "*") &&
      (TOKEN_LAST != "/") && (TOKEN_LAST != "=") && (TOKEN_LAST != "("))
    return false;

  //}

  return true;
}

// ====================================================

bool CAN_ADD_FUN_HERE(std::string TOKEN_LAST) {
  // str / عدد = متغير + fun(var) * (fun(var) / fun(var, fun(var), var) - var)

  /*
  if (IS_IF_SYNTAX)
  {
          if ((TOKEN_LAST != "+") &&
                  (TOKEN_LAST != "-") &&
                  (TOKEN_LAST != "*") &&
                  (TOKEN_LAST != "/") &&
                  (TOKEN_LAST != "=") &&
                  (TOKEN_LAST != "،") &&
                  (TOKEN_LAST != ",") &&

                  (TOKEN_LAST != "<") &&
                  (TOKEN_LAST != ">") &&
                  (TOKEN_LAST != "و") &&
                  (TOKEN_LAST != "أو") &&

                  (TOKEN_LAST != "("))
                  return false;
  }
  */

  if ((TOKEN_LAST != "+") && (TOKEN_LAST != "-") && (TOKEN_LAST != "*") &&
      (TOKEN_LAST != "/") && (TOKEN_LAST != "=") && (TOKEN_LAST != "،") &&
      (TOKEN_LAST != ",") && (TOKEN_LAST != "("))
    return false;

  return true;
}

// ====================================================

bool CAN_ADD_PARENTHESIS_OPEN_HERE(std::string TOKEN_LAST) {
  // = ((1)) + (2 * (3 / (4) - 5) +( 6))
  // IF = (a + s < b * h) او (z + 2) != (x - 7) و (z = x)

  if (IS_IF_SYNTAX) {
    if ((TOKEN_LAST != "+") && (TOKEN_LAST != "-") && (TOKEN_LAST != "*") &&
        (TOKEN_LAST != "/") && (TOKEN_LAST != "=") &&

        (TOKEN_LAST != "<") && (TOKEN_LAST != ">") && (TOKEN_LAST != "و") &&
        (TOKEN_LAST != "أو") &&

        (TOKEN_LAST != "("))
      return true;
  } else {
    if ((TOKEN_LAST != "+") && (TOKEN_LAST != "-") && (TOKEN_LAST != "*") &&
        (TOKEN_LAST != "/") && (TOKEN_LAST != "=") && (TOKEN_LAST != "("))
      return false;
  }

  return true;
}

// ====================================================

std::string IsValidVar_Type = "عادم";

bool IsValidVar(std::string Var, CLASS_TOKEN *o_tokens) {
  IsValidVar_Type = "عادم";

  if (Var == "") {

    return false;
  } else if (Var == "صحيح" || Var == "خطأ") {

    IsValidVar_Type = "منطق";
    return true;
  } else if (IsValidStringFormat(Var, o_tokens)) {

    // STRING

    IsValidVar_Type = "نص";
    return true;
  } else if (IsValidDigit(Var, true, o_tokens)) {

    // DIGIT

    IsValidVar_Type = "عدد";
    return true;
  } else if ((substr_utf8(Var, 0, 1) == "_" &&
              G_VAR_IS_SET[(
                  substr_utf8(Var, 1, CharCount_utf8(Var, o_tokens)))])) {

    // Global متغير _

    IsValidVar_Type =
        G_VAR_TYPE[substr_utf8(Var, 1, CharCount_utf8(Var, o_tokens))];
    return true;
  } else if (L_VAR_IS_SET[std::make_pair(TheNamespace + TheFunction, Var)]) {

    // Local Var

    IsValidVar_Type =
        L_VAR_TYPE[std::make_pair(TheNamespace + TheFunction, Var)];
    return true;
  } else if (L_VAR_IS_SET[std::make_pair(TheClass + TheFunction, Var)]) {

    // Class Local Var

    IsValidVar_Type = L_VAR_TYPE[std::make_pair(TheClass + TheFunction, Var)];
    return true;
  } else if (G_VAR_IS_SET[(Var)]) {

    // Global Var

    IsValidVar_Type = G_VAR_TYPE[Var];
    return true;
  } else if (CLASS_G_VAR_IS_SET[std::make_pair(TheClass, Var)] &&
             IsInsideClass) {

    // Class Global Var

    IsValidVar_Type = CLASS_G_VAR_TYPE[std::make_pair(TheClass, Var)];
    return true;
  } else if (G_FUN_IS_SET[(Var)]) {

    // Global Function

    IsValidVar_Type = G_FUN_TYPE[Var];
    return true;
  } else if (L_FUN_IS_SET[std::make_pair(TheNamespace, Var)]) {

    // Local Function
    IsValidVar_Type = L_FUN_TYPE[std::make_pair(TheNamespace, Var)];

    return true;
  } else if (L_FUN_IS_SET[std::make_pair(TheClass, Var)]) {

    // Class Local Function

    IsValidVar_Type = L_FUN_TYPE[std::make_pair(TheClass, Var)];
    return true;
  }
  // else if (OBJ_IS_SET[std::make_pair("", Var)]){
  //
  // 	// Global OBJ
  //
  // 	IsValidVar_Type = "OBJ";
  // 	return true;
  // }

  // else if (CONTROL_WIN_IS_SET[Var]){
  // 	// namespace: (By UI)
  // 	IsValidVar_Type = "عادم";
  // 	return true;
  // }

  else if (namespace_is_set[Var]) {

    // namespace: (By Code)

    IsValidVar_Type = "عادم";
    return true;
  }

  // else if (CONTROL_IS_SET[std::make_pair(TheNamespace, Var)]){
  // 	// Control:
  // 	IsValidVar_Type = "عادم";
  // 	return true;
  // }

  else
    return false;
}

// ====================================================

bool CAN_ADD_PARENTHESIS_CLOSE_HERE(std::string TOKEN_LAST) {
  // = ((1)) + (2 * (3 / (4) - 5) + ( 6)) + ()

  if (TOKEN_LAST == "(") // إلى avoid ... () ...
    return false;

  if ((TOKEN_LAST == "=") || (TOKEN_LAST == "+") || (TOKEN_LAST == "-") ||
      (TOKEN_LAST == "*") || (TOKEN_LAST == "/"))
    return false;

  return true;
}

// ====================================================

bool CAN_ADD_DIGIT_HERE(std::string TOKEN_LAST) {
  // = 1 + 2 * (3 / 4 - 5) + 6

  /*
  if (IS_IF_SYNTAX)
  {
          if ((TOKEN_LAST != "+") &&
                  (TOKEN_LAST != "-") &&
                  (TOKEN_LAST != "*") &&
                  (TOKEN_LAST != "/") &&
                  (TOKEN_LAST != "=") &&

                  (TOKEN_LAST != "<") &&
                  (TOKEN_LAST != ">") &&
                  (TOKEN_LAST != "و") &&
                  (TOKEN_LAST != "أو") &&

                  (TOKEN_LAST != "("))
                  return false;
  }
  else
  {
  */
  if ((TOKEN_LAST != "+") && (TOKEN_LAST != "-") && (TOKEN_LAST != "*") &&
      (TOKEN_LAST != "/") && (TOKEN_LAST != "=") && (TOKEN_LAST != "("))
    return false;
  //}

  return true;
}

// ====================================================

std::string remove_quote(std::string STRING, CLASS_TOKEN *o_tokens) {
  std::string MESSAGE_BUFFER;

  if (CharCount_utf8(STRING, o_tokens) < 3) // "" Emty std::string
    return "";                              // 3 is the minimum msg, "a"

  MESSAGE_BUFFER = substr_utf8(STRING, 1, CharCount_utf8(STRING, o_tokens));
  MESSAGE_BUFFER = substr_utf8(MESSAGE_BUFFER, 0,
                               (CharCount_utf8(MESSAGE_BUFFER, o_tokens) - 1));

  return MESSAGE_BUFFER;
}

// ====================================================

void CheckForSameGlobalID(std::string Name, CLASS_TOKEN *o_tokens) {
  // check if same name with any already global declaration

  if (OBJ_IS_SET[std::make_pair("", Name)]) {
    // same name with Global Obj
    ErrorCode(
        "تشابه في الاسم مع كائن عام ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " +
            Name + " ' ",
        o_tokens);
  } else if (CLASS_IS_SET[Name]) {
    // same name with Class
    ErrorCode("تشابه في الاسم مع صنف ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " +
                  Name + " ' ",
              o_tokens);
  } else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "عدد") {
    // same name with Global INT
    ErrorCode("تشابه في الاسم مع متغير عددي ثم إنشائه مسبقا، المرجو تغيير "
              "الاسم : ' " +
                  Name + " ' ",
              o_tokens);
  } else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "نص") {
    // same name with Global STRING
    ErrorCode(
        "تشابه في الاسم مع متغير نصي ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " +
            Name + " ' ",
        o_tokens);
  } else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "منطق") {
    // same name with Global BOOL
    ErrorCode("تشابه في الاسم مع متغير منطقي ثم إنشائه مسبقا، المرجو تغيير "
              "الاسم : ' " +
                  Name + " ' ",
              o_tokens);
  } else if (G_FUN_IS_SET[(Name)]) {
    // same name with Global Function
    ErrorCode("تشابه في الاسم مع دالة عامة ثم إنشائها مسبقا، المرجو تغيير "
              "الاسم : ' " +
                  Name + " ' ",
              o_tokens);
  }

  else if (namespace_is_set[Name]) // || CONTROL_WIN_IS_SET[Name]
  {
    // same name with Namespace
    ErrorCode(
        "تشابه في الاسم مع مجال ثم إنشائها مسبقا، المرجو تغيير الاسم : ' " +
            Name + " ' ",
        o_tokens);
  }
}

bool IsValidName(std::string Name, CLASS_TOKEN *o_tokens) {

  std::string Char;
  bool First = true;

  for (int I = 0; I <= CharCount_utf8(Name, o_tokens); I++) {

    Char = substr_utf8(Name, I, 1);

    if (Char == "_" || // Allow, Example : ABC_XZ1
        Char == "0" || Char == "1" || Char == "2" || Char == "3" ||
        Char == "4" || Char == "5" || Char == "5" || Char == "6" ||
        Char == "8" || Char == "9" ||

        Char == "۰" || // 0, Zero In Arabic, ITS NOT A DOT!
        Char == "۱" || // 1
        Char == "۲" || // 2
        Char == "۳" || // 3
        Char == "۴" || // 4
        Char == "۵" || // 5
        Char == "۶" || // 6
        Char == "۷" || // 7
        Char == "۸" || // 8
        Char == "۹")   // 9
    {
      if (First) {
        ErrorCode("أول حرف في الإسم يجب أن يكون حرفا، و ليس رقم أو إشارة : ' " +
                      Char + " ' ",
                  o_tokens);
        return false;
      }
    } else if (Char == "." || // Allow DOT in متغير names ABC.DEF
               Char == "," || Char == "،" || // اشارة ،
               Char == "{" || Char == "}" || Char == "|" || Char == "~" ||
               // Char == "_" || Allow AB_C
               Char == "$" || Char == "#" || Char == ":" || Char == "!" ||
               Char == ";" || Char == "?" || Char == "'" || Char == "-" ||
               Char == "/" || Char == "=" || Char == "\\" || Char == "^" ||
               Char == "\"" || Char == "%" || Char == ")" || Char == "(" ||
               Char == "*" || Char == "+" || Char == "<" || Char == ">" ||
               Char == "@" || Char == "[" || Char == "]" || Char == "&" ||
               Char == " " ||        // Space
               Char == "	" || // Tab
               Char == "") {
      if (Char == " ") {
        if (!IS_CONTROL_FUNCTION_NAME) {
          ErrorCode("حرف غير مقبول : ' " + Char + " ' داخل ' " + Name + " ' ",
                    o_tokens);
          return false; // Exception !
        }

        IS_CONTROL_FUNCTION_NAME = false;
        return true;
      }

      ErrorCode("حرف غير مقبول : ' " + Char + " ' داخل ' " + Name + " ' ",
                o_tokens);
      return false;
    }

    First = false;
  } // End char loop.

  for (int i = 0; i <= ALIF_RESERVED_TOTAL; i++) {
    if (ALIF_RESERVED[i] == Name) {
      // if (Name == "رئيسية" && IsInsideClass)
      // continue;

      ErrorCode("عفوا، هذا الاسم محجوز في لغة ألف : ' " + Name + " ' ",
                o_tokens);
    }
  }

  IS_CONTROL_FUNCTION_NAME = false;
  return true;
}

// ====================================================

void ADD_FUN(bool GLOBAL, std::string NS_Name, std::string FUN_NAME,
             std::string TYPE, int Line, CLASS_TOKEN *o_tokens) {
  if (!IsValidName(FUN_NAME, o_tokens))
    ErrorCode("اسم غير مقبول : ' " + FUN_NAME + " ' ", o_tokens);

  // if same name as Namespace !
  if (NS_Name == FUN_NAME)
    ErrorCode("تشابه في الاسم بين الدالة و المجال ' " + FUN_NAME + " ' ",
              o_tokens);

  if (GLOBAL) {
    // if already exist global fun
    if (G_FUN_IS_SET[(FUN_NAME)]) {
      ErrorCode(
          "الدالة العامه ' " + FUN_NAME +
              " ' تم انشاؤها مسبقا في السطر : " + G_FUN_AT_LINE[(FUN_NAME)],
          o_tokens);
    }

    // if already any other type of global var exist
    CheckForSameGlobalID(FUN_NAME, o_tokens);

    G_FUN_TYPE[(FUN_NAME)] = TYPE;
    G_FUN_IS_SET[(FUN_NAME)] = true;
    G_FUN_AT_LINE[(FUN_NAME)] = IntToString(Line);

    Global_FunctionNames[Global_TotalFucntion] = FUN_NAME;
    Global_TotalFucntion++;

    if (TYPE == "عادم")
      RETURN_FUN[std::make_pair("", FUN_NAME)] = "OK";

    // *** Generate Code ***
    SET_GLOBAL_C_NAME(FUN_NAME);
    // *** *** *** *** *** ***
  } else {
    // if already exist local fun
    if (L_FUN_IS_SET[std::make_pair(NS_Name, FUN_NAME)]) {
      ErrorCode("الدالة ' " + FUN_NAME + " ' تم انشاؤها مسبقا في السطر : " +
                    L_FUN_AT_LINE[std::make_pair(NS_Name, FUN_NAME)],
                o_tokens);
    }

    L_FUN_TYPE[std::make_pair(NS_Name, FUN_NAME)] = TYPE;
    L_FUN_IS_SET[std::make_pair(NS_Name, FUN_NAME)] = true;
    L_FUN_AT_LINE[std::make_pair(NS_Name, FUN_NAME)] = IntToString(Line);

    if (TYPE == "عادم")
      RETURN_FUN[std::make_pair(NS_Name, FUN_NAME)] = "OK";

    // *** Generate Code ***
    SET_C_NAME(FUN_NAME);
    // *** *** *** *** *** ***
  }
}

void SetNewVar(bool IsGlobal, std::string TempNS, std::string TmpFunction,
               std::string VarName, std::string VarDataType, bool IsConstant,
               bool IsArray, int Line, CLASS_TOKEN *o_tokens) {

  if (!IsValidName(VarName, o_tokens))
    ErrorCode("اسم غير مقبول : ' " + VarName + " ' ", o_tokens);

  if (TmpFunction == VarName)
    ErrorCode("تشابه في الاسم بين المتغير و الدالة ' " + VarName + " ' ",
              o_tokens);

  if (IsGlobal) {
    if (G_VAR_IS_SET[(VarName)])
      ErrorCode("المتغير العام ' " + VarName +
                    " ' تم انشاؤه مسبقا في السطر : " + G_VAR_AT_LINE[(VarName)],
                o_tokens);

    CheckForSameGlobalID(VarName, o_tokens);

    G_VAR_TYPE[(VarName)] = VarDataType;
    G_VAR_IS_SET[(VarName)] = true;
    G_VAR_AT_LINE[(VarName)] = IntToString(Line);
    G_VAR_IS_CONST[(VarName)] = IsConstant;
    G_VAR_IS_ARRAY[(VarName)] = IsArray;
    G_VAR_NAMES[G_VAR_TOTAL] = VarName;
    G_VAR_TOTAL++;

    // *** Generate Code ***
    SET_GLOBAL_C_NAME(VarName);
    // *** *** *** *** *** ***
  } else {
    if (L_VAR_IS_SET[std::make_pair(TempNS + TmpFunction, VarName)]) {
      ErrorCode(
          "المتغير ' " + VarName + " ' تم انشاؤه مسبقا في السطر : " +
              L_VAR_AT_LINE[std::make_pair(TempNS + TmpFunction, VarName)],
          o_tokens);
    }

    L_VAR_TYPE[std::make_pair(TempNS + TmpFunction, VarName)] = VarDataType;
    L_VAR_IS_SET[std::make_pair(TempNS + TmpFunction, VarName)] = true;
    L_VAR_AT_LINE[std::make_pair(TempNS + TmpFunction, VarName)] =
        IntToString(Line);
    L_VAR_IS_CONST[std::make_pair(TempNS + TmpFunction, VarName)] = IsConstant;
    L_VAR_IS_ARRAY[std::make_pair(TempNS + TmpFunction, VarName)] = IsArray;

    // *** Generate Code ***
    SET_C_NAME(VarName);
    // *** *** *** *** *** ***
  }
}

void SetNewVarClass(bool IsGlobal, bool IsPrivate, std::string ClassName,
                    std::string FunctionName, std::string VarName,
                    std::string VarDataType, bool IsConstant, bool IsArray,
                    int Line, CLASS_TOKEN *o_tokens) {

  if (!IsValidName(VarName, o_tokens))
    ErrorCode("اسم غير مقبول : ' " + VarName + " ' ", o_tokens);

  if (!IsGlobal && IsPrivate)
    ErrorCode("يجب استخدام ' خاص ' خارج الدالة", o_tokens);

  if (ClassName == VarName)
    ErrorCode("تشابه في الاسم بين المتغير و الصنف ' " + VarName + " ' ",
              o_tokens);

  if (IsGlobal) {
    if (CLASS_G_VAR_IS_SET[std::make_pair(ClassName, VarName)]) {
      ErrorCode("المتغير العام ' " + VarName +
                    " ' تم انشاؤه مسبقا في السطر : " +
                    CLASS_G_VAR_AT_LINE[std::make_pair(ClassName, VarName)],
                o_tokens);
    }

    /*
    if (G_VAR_IS_SET[(VarName)])
    {
            ErrorCode("المتغير العام ' " + VarName + " ' تم انشاؤه مسبقا في
    السطر : " + CLASS_G_VAR_AT_LINE[std::make_pair(ClassName, VarName)],
    o_tokens);
    }
    */

    CLASS_G_VAR_TYPE[std::make_pair(ClassName, VarName)] = VarDataType;
    CLASS_G_VAR_IS_SET[std::make_pair(ClassName, VarName)] = true;
    CLASS_G_VAR_AT_LINE[std::make_pair(ClassName, VarName)] = IntToString(Line);
    CLASS_G_VAR_PRIVATE[std::make_pair(ClassName, VarName)] = IsPrivate;
    CLASS_G_VAR_IS_CONST[std::make_pair(ClassName, VarName)] = IsConstant;
    CLASS_G_VAR_IS_ARRAY[std::make_pair(ClassName, VarName)] = IsArray;

    // *** Generate Code ***
    SET_GLOBAL_C_NAME(VarName);
    // *** *** *** *** *** ***
  } else {
    if (L_VAR_IS_SET[std::make_pair(ClassName + FunctionName, VarName)]) {
      ErrorCode(
          "المتغير ' " + VarName + " ' تم انشاؤه مسبقا في السطر : " +
              L_VAR_AT_LINE[std::make_pair(ClassName + FunctionName, VarName)],
          o_tokens);
    }

    L_VAR_TYPE[std::make_pair(ClassName + FunctionName, VarName)] = VarDataType;
    L_VAR_IS_SET[std::make_pair(ClassName + FunctionName, VarName)] = true;
    L_VAR_AT_LINE[std::make_pair(ClassName + FunctionName, VarName)] =
        IntToString(Line);
    L_VAR_IS_CONST[std::make_pair(ClassName + FunctionName, VarName)] =
        IsConstant;
    L_VAR_IS_ARRAY[std::make_pair(ClassName + FunctionName, VarName)] = IsArray;

    // *** Generate Code ***
    SET_C_NAME(VarName);
    // *** *** *** *** *** ***
  }
}

std::string GetSyntaxDataType(std::string Token[1024], int Position,
                              CLASS_TOKEN *o_tokens) {

  // Token[Position] -> '='

  if (IsValidVar(Token[Position + 1], o_tokens))
    return IsValidVar_Type;

  return {};
}

void ADD_FUN_CLASS(bool PRIVATE, std::string CLASS_NAME, std::string FUN_NAME,
                   std::string TYPE, int Line, CLASS_TOKEN *o_tokens) {
  if (!IsValidName(FUN_NAME, o_tokens))
    ErrorCode("اسم غير مقبول : ' " + FUN_NAME + " ' ", o_tokens);

  // if already exist class fun
  if (CLASS_FUN_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)]) {
    ErrorCode("الصنف الدالة ' " + FUN_NAME + " ' تم انشاؤها مسبقا في السطر : " +
                  CLASS_FUN_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)],
              o_tokens);
  }

  // if already same Generated_ID with global class var.
  if (CLASS_G_VAR_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)]) {
    ErrorCode("تشابه في الاسم مع متغير ' " + FUN_NAME + " ' في السطر : " +
                  CLASS_G_VAR_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)] +
                  " ",
              o_tokens);
  }
  // else
  // if(DEBUG)DEBUG_MESSAGE("---C " + CLASS_NAME + " :F " + FUN_NAME + " E---",
  // o_tokens); // DEBUG

  // if same name as class !
  if (CLASS_NAME == FUN_NAME)
    ErrorCode("تشابه في الاسم بين الدالة و الصنف ' " + FUN_NAME +
                  " '، على العموم إن كنت تقصد دالة بناء الصنف استخدم التعريف ' "
                  "بناء '",
              o_tokens);

  CLASS_FUN_TYPE[std::make_pair(CLASS_NAME, FUN_NAME)] = TYPE;
  CLASS_FUN_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)] = true;
  CLASS_FUN_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)] = IntToString(Line);
  CLASS_FUN_PRIVATE[std::make_pair(CLASS_NAME, FUN_NAME)] = PRIVATE;

  if (TYPE == "عادم")
    RETURN_FUN[std::make_pair(CLASS_NAME, FUN_NAME)] = "OK";

  // *** Generate Code ***
  SET_C_NAME(FUN_NAME);
  // *** *** *** *** *** ***
}

// ====================================================

// Here, was void ADD_VAR() / void ADD_VAR_CLASS()
// Now, we use NewVar.cpp

// ====================================================

std::string C_LAST_ARG;

std::string CHECK_NEW_FUN_SYNTAX(
    bool GLOBAL, std::string SYNTAX[1024], int SYNTAX_LONG,
    std::string TempNS,      // fun1 { a = b + namespace:fun2(x) + z }
    std::string TmpFunction, // fun1 { a = b + namespace:fun2(x) + z }
    CLASS_TOKEN *o_tokens) {
  // function (int a, نص b)

  // SYNTAX : عدد 	| SYNTAX[3] : std::string
  // SYNTAX[1] : a	| SYNTAX[4] : b
  // SYNTAX[2] : ,	| SYNTAX[5] : )

  int TYPE = 0, VAR = 1, COMMA = 2;

  std::string CPP_CODE;
  NEW_FUNCTION_ARG = "";

  for (int p = 0; p <= SYNTAX_LONG; p += 3) {
    if (!o_tokens->TOKENS_PREDEFINED) {
      // Tokens not predifined, so we need only to set ARGs

      if (SYNTAX[TYPE] != "عدد" && SYNTAX[TYPE] != "نص" &&
          SYNTAX[TYPE] != "منطق" && SYNTAX[TYPE] != "مؤشر_دالة") {
        ErrorCode("نوع غير مقبول ' " + SYNTAX[TYPE] + " ' ", o_tokens);
      }

      if (!IsValidName(SYNTAX[VAR], o_tokens))
        ErrorCode("اسم غير مقبول ' " + SYNTAX[VAR] + " ' ", o_tokens);

      if (L_VAR_IS_SET[std::make_pair(TempNS + TmpFunction,
                                      SYNTAX[VAR])]) // الدالة (int a, نص a)
        ErrorCode("متغير محلي موجود مسبقا ' " + SYNTAX[VAR] + " ' ", o_tokens);

      if (SYNTAX[COMMA] != "،" && SYNTAX[COMMA] != "," && SYNTAX[COMMA] != ")")
        ErrorCode("فاصله غير صحيحه ' " + SYNTAX[COMMA] + " ' ", o_tokens);

      if (SYNTAX[COMMA] == ")" && COMMA < SYNTAX_LONG)
        ErrorCode("غير مكتمل ' " + IntToString(SYNTAX_LONG) + " ' ", o_tokens);

      // if(DEBUG)DEBUG_MESSAGE("[" + SYNTAX[TYPE] + "|" + SYNTAX[VAR] + "]",
      // o_tokens); // DEBUG

      // add always-local (bcs this var is in arg..) VAR to fun
      SetNewVar(false, TempNS, TmpFunction, SYNTAX[VAR], SYNTAX[TYPE], false,
                false, o_tokens->Line, o_tokens);

      // add local ARG-VAR as ARGUMENT to fun
      if (GLOBAL) {
        G_FUN_ARG_TYPE[std::make_pair(
            TmpFunction, G_FUN_ARG_TOTAL[TmpFunction])] = SYNTAX[TYPE];
        G_FUN_ARG_TOTAL[TmpFunction]++;
      } else {
        L_FUN_ARG_TYPE[std::make_pair(
            TempNS + TmpFunction,
            L_FUN_ARG_TOTAL[std::make_pair(TempNS, TmpFunction)])] =
            SYNTAX[TYPE];
        L_FUN_ARG_TOTAL[std::make_pair(TempNS, TmpFunction)]++;
      }

      TYPE += 3, VAR += 3, COMMA += 3; // For ARG non-predifined loop
    } else {
      // Tokens already predifined, so we need show log, and return C++ code

      if (SYNTAX[TYPE] == "عدد") {
        if (DEBUG)
          DEBUG_MESSAGE("[INT " + SYNTAX[VAR] + "]", o_tokens); // DEBUG

        // *** Generate Code ***
        // always need pointer to one elem array
        CPP_CODE.append(" double " + ID[SYNTAX[VAR]] + " ");
        NEW_FUNCTION_ARG.append(" double " + ID[SYNTAX[VAR]] + " ");
        // *** *** *** *** *** ***
      } else if (SYNTAX[TYPE] == "نص") {
        if (DEBUG)
          DEBUG_MESSAGE("[STRING " + SYNTAX[VAR] + "]", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" std::string " + ID[SYNTAX[VAR]] + " ");
        NEW_FUNCTION_ARG.append(" std::string " + ID[SYNTAX[VAR]] + " ");
        // *** *** *** *** *** ***

      } else if (SYNTAX[TYPE] == "منطق") {
        if (DEBUG)
          DEBUG_MESSAGE("[BOOL " + SYNTAX[VAR] + "]", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" bool " + ID[SYNTAX[VAR]] + " ");
        NEW_FUNCTION_ARG.append(" bool " + ID[SYNTAX[VAR]] + " ");
        // *** *** *** *** *** ***
      } else if (SYNTAX[TYPE] == "مؤشر_دالة") {
        if (DEBUG)
          DEBUG_MESSAGE("[POINTER " + SYNTAX[VAR] + "]", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" void(* " + ID[SYNTAX[VAR]] + " )() ");
        NEW_FUNCTION_ARG.append(" void(* " + ID[SYNTAX[VAR]] + " )() ");
        // *** *** *** *** *** ***
      } else {
        // Exception !
        ErrorCode("نوع غير مقبول ' " + SYNTAX[TYPE] + " ' ", o_tokens);
      }

      TYPE += 3, VAR += 3,
          COMMA += 3; // For ARG predifined / C++ loop, and need by comma

      // *** Generate Code ***
      if (COMMA <= SYNTAX_LONG) {
        CPP_CODE.append(" , ");
        NEW_FUNCTION_ARG.append(" , ");
      }
      // *** *** *** *** *** ***
    }
  }

  // if(DEBUG)DEBUG_MESSAGE("[NS: "+ TempNS +"|Fun: " + TmpFunction + "|Looocal:
  // " + IntToString(L_FUN_ARG_TOTAL[std::make_pair(TempNS, TmpFunction)]) +
  // "|Glooobal: " + IntToString(G_FUN_ARG_TOTAL[TmpFunction]) + "]", o_tokens);
  // // DEBUG

  if (o_tokens->TOKENS_PREDEFINED)
    return CPP_CODE;
  else
    return "";
}

// ====================================================

std::string CheckForSyntax(
    std::string OBJECTIF_TYPE, // OBJECTIF_TYPE
    bool ACCEPT_REF_WIN_WIDGET, // Accept Using Reference إلى namespace:Controls
    bool ACCEPT_REF_WIN_FUN, // Accept Using Reference إلى namespace:Function
    bool ACCEPT_REF_GLOBAL_FUN, // Accept Using Reference إلى Global Functions
    bool ACCEPT_REF_LOCAL_FUN,  // Accept Using Reference إلى Local Functions
    bool ACCEPT_REF_GLOBAL_VAR, // Accept Using Reference إلى Global VAR
    bool ACCEPT_REF_LOCAL_VAR,  // Accept Using Reference إلى Local VAR
    bool ACCEPT_STR_TO_INT,     // Accept Convertion من نص إلى Int
    bool ACCEPT_INT_TO_STRING,  // Accept Convertion من عدد إلى String
    std::string SYNTAX[1024],   // SYNTAX[] std::string
    int SYNTAX_LONG,            // SYNTAX_LONG int
    std::string
        TMP_NAMESPACE_OR_CLASS, // a = b + namespace:fun(2+2) + class:fun(x)
    std::string TmpFunction,    // a = b + win/class:fun(2+2)
    CLASS_TOKEN *o_tokens) {
  // Note : WX Crash if On creating using reference إلى Controls, BUT OK ل
  // Functions.

  // SYNTAX = '='
  // SYNTAX[1] = ...

  // TmpFunction				: = a + b + TMP_FUN_NAME(X, Y,
  // Z)
  // +
  // ... TMP_NAMESPACE_OR_CLASS		: إلى get Generated_ID of tmp الدالة
  // name OBJECTIF_TYPE		: INT.ToString or STRING.ToInt

  std::string CPP_CODE;

  if (SYNTAX[0] != "=") {
    ErrorCode("بناء غير صحيح : علة : " +
                  CONVERT_STRING_ARRAY_TO_STRING(SYNTAX, SYNTAX_LONG),
              o_tokens);
  }

  // Clear Syntax Array, needfor C++ Code _س_
  // for (int p = 0; p <= SYNTAX_LONG; p++)
  // TMP_SYNTAX[p].clear(); // SYNTAX[p] = ""; SYNTAX[p] = 0;

  for (int p = 1; p <= SYNTAX_LONG; p++) {
    // ----------------------
    // _س_
    // ----------------------

    if (SYNTAX[p] == "_س_") {
      // xyz = _س_ ...C++...@ Alif @...C++... _س_

      if (DEBUG)
        DEBUG_MESSAGE(" {_س_ Syntax START} ", o_tokens); // DEBUG

      bool C_FOUND = false;

      for (int z = p + 1; z <= SYNTAX_LONG; z++) {
        if (SYNTAX[z] == "_س_") // End C++ Code
        {
          if (DEBUG)
            DEBUG_MESSAGE(" {_س_ Syntax END} ", o_tokens); // DEBUG
          C_FOUND = true;
          p = z;
          break;
        } else if (SYNTAX[z] == "@") // @ Start ...
        {
          // xyz = ...C++...@ Alif @...C++...

          if (DEBUG)
            DEBUG_MESSAGE(" {Syntax@} ", o_tokens); // DEBUG

          std::string TempToken[1024];
          TempToken[0] = "=";     // CheckForSyntax() Need this.
          int TempTokenCount = 1; // CheckForSyntax() Need this.

          bool AT_FOUND = false;

          for (int c = z + 1; c < SYNTAX_LONG; c++) {
            if (SYNTAX[c] == "@") {
              z = c;
              AT_FOUND = true;
              break;
            }

            TempToken[TempTokenCount] = SYNTAX[c];
            TempTokenCount++;
          }

          if (!AT_FOUND)
            ErrorCode("نهايه شفرة غير موجود داخل البناء ' @ '", o_tokens);

          std::string SYNTAX_BUFFER = CheckForSyntax(
              "C++",     // OBJECTIF_TYPE
              true,      // Accept Using Reference إلى namespace:Controls
              true,      // Accept Using Reference إلى namespace:Function
              true,      // Accept Using Reference إلى Global Functions
              true,      // Accept Using Reference إلى Local Functions
              true,      // Accept Using Reference إلى Global VAR
              true,      // Accept Using Reference إلى Local VAR
              true,      // Accept Convertion من نص إلى Int
              true,      // Accept Convertion من عدد إلى String
              TempToken, // SYNTAX[] std::string
              (TempTokenCount - 1), // SYNTAX_LONG int
              TheNamespace,         // TMP_WINDOW_NAME
              TheFunction,          // TMP_FUNCTION_NAME
              o_tokens);

          if (DEBUG)
            DEBUG_MESSAGE(" {Syntax@} ", o_tokens); // DEBUG

          // *** C++ ***
          CPP_CODE.append(" " + SYNTAX_BUFFER + " ");
          // *** *** *** *** *** ***

          // @ End.
        } else if (SYNTAX[z] != "") {
          CPP_CODE.append(SYNTAX[z]);
          if (DEBUG)
            DEBUG_MESSAGE(" {" + SYNTAX[z] + "} ", o_tokens); // DEBUG
        }
      }

      if (C_FOUND)
        continue; // Point إلى next بعد _س_
      else
        ErrorCode("نهايه شفرة غير موجود داخل البناء ' _س_ '", o_tokens);
    }

    // ----------------------
    // OPERATION
    // ----------------------

    if (SYNTAX[p] == "+" || SYNTAX[p] == "-" || SYNTAX[p] == "*" ||
        SYNTAX[p] == "/") {
      if (p > 0) {
        if (!CAN_ADD_OPERATION_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة عملية هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);
      }

      if (p >= SYNTAX_LONG) {
        if (DEBUG)
          DEBUG_MESSAGE("######################", o_tokens); // DEBUG
        for (int p = 1; p <= SYNTAX_LONG; p++)
          if (DEBUG)
            DEBUG_MESSAGE("[" + SYNTAX[p] + "] ", o_tokens); // DEBUG
        ErrorCode("لا يمكن الإنتهاء بعملية ' " + SYNTAX[p] + " ' ", o_tokens);
      }

      if (OBJECTIF_TYPE == "نص") // only acceptable operation in نص is '+'
      {
        if (SYNTAX[p] != "+")
          ErrorCode("لا يمكن إضافة عملية داخل نص ' " + SYNTAX[p] + " ' ",
                    o_tokens);
      }

      if (OBJECTIF_TYPE == "منطق")
        ErrorCode("لا يمكن أجراء عملية في متغير منطقي ' " + SYNTAX[p] + " ' ",
                  o_tokens);

      if (substr_utf8(SYNTAX[p - 1], 0, 1) == "\"" &&
          substr_utf8(SYNTAX[p + 1], 0, 1) == "\"") // "test" + "hello"
        ErrorCode("لا يمكن إضافة نص إلى نص ' " + SYNTAX[p - 1] + " " +
                      SYNTAX[p] + " " + SYNTAX[p + 1] + " ' ",
                  o_tokens);

      if (DEBUG)
        DEBUG_MESSAGE("[OPERATION " + SYNTAX[p] + "] ", o_tokens); // DEBUG

      // *** Generate Code ***
      CPP_CODE.append(" " + SYNTAX[p] + " ");
      // *** *** *** *** *** ***
    }

    // ----------------------
    // قوس OPEN
    // ----------------------

    else if (SYNTAX[p] == "(") {
      if (p > 0)
        if (!CAN_ADD_PARENTHESIS_OPEN_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة قوس مفتوح هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      ALIF_PARENTHESIS_STATUS++;

      if (DEBUG)
        DEBUG_MESSAGE("[PARENTHESIS Open (] ", o_tokens); // DEBUG

      // *** Generate Code ***
      CPP_CODE.append(" ( ");
      // *** *** *** *** *** ***
    }

    // ----------------------
    // قوس CLOSE
    // ----------------------

    else if (SYNTAX[p] == ")") {
      if (ALIF_PARENTHESIS_STATUS < 1)
        ErrorCode("غلق قوس ليس مفتوحا ' " + SYNTAX[p] + " ' ", o_tokens);

      if (p > 0)
        if (!CAN_ADD_PARENTHESIS_CLOSE_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة غلق قوس هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      ALIF_PARENTHESIS_STATUS--;

      if (DEBUG)
        DEBUG_MESSAGE("[PARENTHESIS CLOSE )] ", o_tokens); // DEBUG

      // *** Generate Code ***
      CPP_CODE.append(" ) ");
      // *** *** *** *** *** ***
    }

    // ----------------------
    // Local VARIABLE INT
    // ----------------------

    else if (L_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS + TmpFunction,
                                       SYNTAX[p])] == "عدد") {
      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (OBJECTIF_TYPE == "عدد") {
        if (DEBUG)
          DEBUG_MESSAGE("[LOCAL-INT (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
        // *** *** *** *** *** ***
      } else if (OBJECTIF_TYPE == "نص") {
        if (ACCEPT_INT_TO_STRING) {
          if (DEBUG)
            DEBUG_MESSAGE("[LOCAL-INT (" + SYNTAX[p] + ").ToString] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" alifcore_IntToString(" + ID[SYNTAX[p]] + ") ");
          // *** *** *** *** *** ***
        } else {
          ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        }
      } else if (OBJECTIF_TYPE == "C++") {
        if (DEBUG)
          DEBUG_MESSAGE("[C++ Local INT (" + SYNTAX[p] + ")] ",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                      " ' ل ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // -----------------------------------
    // Local VARIABLE STRING
    // -----------------------------------

    else if (L_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS + TmpFunction,
                                       SYNTAX[p])] == "نص") {
      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (OBJECTIF_TYPE == "عدد") {
        // if(DEBUG)DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] + ").ToInt] ",
        // o_tokens); // DEBUG
        ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ", o_tokens);
      } else if (OBJECTIF_TYPE == "نص") {
        if (DEBUG)
          DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] + ")] ",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
        // *** *** *** *** *** ***
      } else if (OBJECTIF_TYPE == "C++") {
        if (DEBUG)
          DEBUG_MESSAGE("[C++ Local STRING (" + SYNTAX[p] + ")] ",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                      " ' ل ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // -----------------------------------
    // Local VARIABLE Bool
    // -----------------------------------

    else if (L_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS + TmpFunction,
                                       SYNTAX[p])] == "منطق") {
      // if (p != 1)
      //{
      // ErrorCode("أمر غير معروف ' " + SYNTAX[p] + " ' ", o_tokens);
      //}

      if (SYNTAX[p + 1] != "") {
        // bool A = B + wrong
        ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] +
                      " '، على العموم لا يمكن أجراء أية عملية على متغير منطقي ",
                  o_tokens);
      } else if (OBJECTIF_TYPE == "C++") {
        if (DEBUG)
          DEBUG_MESSAGE("[C++ Local Bool (" + SYNTAX[p] + ")] ",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
        // *** *** *** *** *** ***

        continue;
      } else if (OBJECTIF_TYPE != "منطق") {
        ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " +
                      SYNTAX[p] + " ' ",
                  o_tokens);
      }
      // else
      //{
      // ErrorCode("علة : استثناء في متغير منطقي ' " + SYNTAX[p] + " ' ",
      // o_tokens);
      //}

      if (DEBUG)
        DEBUG_MESSAGE("[LOCAL-BOOL ' " + SYNTAX[p] + " '] ", o_tokens); // DEBUG

      // *** Generate Code ***
      CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
      // *** *** *** *** *** ***
    }

    // -----------------------------------
    // Local Func Void Pointer
    // -----------------------------------

    else if (L_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS + TmpFunction,
                                       SYNTAX[p])] == "مؤشر_دالة") {
      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (OBJECTIF_TYPE == "عدد") {
        ErrorCode("لا يمكن تحويل مؤشر_دالة إلى عدد ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      } else if (OBJECTIF_TYPE == "نص") {
        ErrorCode("لا يمكن تحويل مؤشر_دالة إلى نص ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      } else if (OBJECTIF_TYPE == "مؤشر_دالة") {
        if (DEBUG)
          DEBUG_MESSAGE("[LOCAL-POINTER-FUNC (" + SYNTAX[p] + ")] ",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
        // *** *** *** *** *** ***
      } else if (OBJECTIF_TYPE == "C++") {
        if (DEBUG)
          DEBUG_MESSAGE("[C++ Local POINTER-FUNC (" + SYNTAX[p] + ")] ",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                      " ' ل ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // ----------------------
    // Global VARIABLE (all type)
    // Global VARIABLE without _
    // ----------------------

    else if (substr_utf8(SYNTAX[p], 0, 1) == "_" || G_VAR_IS_SET[(SYNTAX[p])]) {
      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      std::string G_VAR_WITHOUT_;

      if (G_VAR_IS_SET[(SYNTAX[p])])
        G_VAR_WITHOUT_ = SYNTAX[p];
      else
        G_VAR_WITHOUT_ =
            substr_utf8(SYNTAX[p], 1, CharCount_utf8(SYNTAX[p], o_tokens));

      if (!G_VAR_IS_SET[(G_VAR_WITHOUT_)])
        ErrorCode("ليس متغير عام ' " + SYNTAX[p] + " ' ", o_tokens);

      if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "عدد") {
        if (OBJECTIF_TYPE == "عدد") {
          if (DEBUG)
            DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
          // *** *** *** *** *** ***
        } else if (OBJECTIF_TYPE == "نص") {
          if (ACCEPT_INT_TO_STRING) {
            if (DEBUG)
              DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ + ").ToString] ",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" alifcore_IntToString(" +
                            Global_ID[G_VAR_WITHOUT_] + ") ");
            // *** *** *** *** *** ***
          } else {
            ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                      o_tokens);
          }
        } else if (OBJECTIF_TYPE == "C++") {
          if (DEBUG)
            DEBUG_MESSAGE("[C++ Global عدد (" + G_VAR_WITHOUT_ + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
          // *** *** *** *** *** ***
        } else {
          ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                        " ' ل ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        }
      } else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "نص") {
        if (OBJECTIF_TYPE == "عدد") {
          if (ACCEPT_INT_TO_STRING) // or ACCEPT_STR_TO_INT !!
          {
            // if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" + G_VAR_WITHOUT_ +
            // ").ToInt] ", o_tokens); // DEBUG
            ErrorCode("لا يمكن تحويل نص إلى عدد ' " + G_VAR_WITHOUT_ + " ' ",
                      o_tokens);
          } else {
            ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                      o_tokens);
          }
        } else if (OBJECTIF_TYPE == "نص") {
          if (DEBUG)
            DEBUG_MESSAGE("[GLOBAL-STRING (" + G_VAR_WITHOUT_ + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
          // *** *** *** *** *** ***
        } else if (OBJECTIF_TYPE == "C++") {
          if (DEBUG)
            DEBUG_MESSAGE("[C++ Global نص (" + G_VAR_WITHOUT_ + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
          // *** *** *** *** *** ***
        } else {
          ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                        " ' ل ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        }
      } else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "منطق") {
        if (OBJECTIF_TYPE != "منطق" && OBJECTIF_TYPE != "C++") {
          ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " +
                        G_VAR_WITHOUT_ + " ' ",
                    o_tokens);
        }

        if (DEBUG)
          DEBUG_MESSAGE("[GLOBAL-BOOL ' " + G_VAR_WITHOUT_ + " '] ",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("علة : نوع المتغير العام غير معروف ' " +
                      G_VAR_TYPE[(G_VAR_WITHOUT_)] + " ' ل ' " +
                      G_VAR_WITHOUT_ + " ' ",
                  o_tokens);
      }
    }

    // -----------------------------------
    // True / False
    // -----------------------------------

    else if (SYNTAX[p] == "صحيح") {
      if (p != 1)
        ErrorCode("لا يمكن وضع ( منطق ) هنا ' " + SYNTAX[p] + " ' ", o_tokens);

      if (SYNTAX[p + 1] != "") {
        // bool A = TRUE + wrong
        ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] +
                      " '، على العموم لا يمكن أجراء أية عملية على متغير منطقي ",
                  o_tokens);
      }

      if (OBJECTIF_TYPE == "منطق") {
        if (DEBUG)
          DEBUG_MESSAGE("[TRUE] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" true ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " +
                      SYNTAX[p] + " ' ",
                  o_tokens);
      }
    } else if (SYNTAX[p] == "خطأ") {
      if (p != 1)
        ErrorCode("لا يمكن وضع ( منطق ) هنا ' " + SYNTAX[p] + " ' ", o_tokens);

      if (SYNTAX[p + 1] != "") {
        // bool A = FALSE + wrong
        ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] +
                      " '، على العموم لا يمكن أجراء أية عملية على متغير منطقي ",
                  o_tokens);
      }

      if (OBJECTIF_TYPE == "منطق") {
        if (DEBUG)
          DEBUG_MESSAGE("[FALSE] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" false ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " +
                      SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // -----------------------------------
    // New Line سطر
    // -----------------------------------

    else if (SYNTAX[p] == "سطر") {
      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة سطر جديد هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (OBJECTIF_TYPE == "عدد") {
        // if(DEBUG)DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] + ").ToInt] ",
        // o_tokens); // DEBUG
        ErrorCode("لا يمكن تحويل سطر جديد إلى عدد ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      } else if (OBJECTIF_TYPE == "نص") {
        if (DEBUG)
          DEBUG_MESSAGE("[NewLine] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" ALIFCORE_NEW_LINE ");
        // *** *** *** *** *** ***
      } else if (OBJECTIF_TYPE == "منطق") {
        ErrorCode("لا يمكن تحويل سطر جديد إلى منطق ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      } else if (OBJECTIF_TYPE == "C++") {
        if (DEBUG)
          DEBUG_MESSAGE("[C++_NewLine] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" (\"\n\") ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                      " ' ل ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // ----------------------
    // Class Global/Local Obj
    // ----------------------

    else if (OBJ_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS + TmpFunction,
                                       SYNTAX[p])] ||
             OBJ_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])] ||
             OBJ_IS_SET[std::make_pair("", SYNTAX[p])]) {
      // a = obj:mem + c
      // b = obj:mem_func(a, b) + c

      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة كائن هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      // if (SYNTAX[p + 1] != ":")
      // ErrorCode("يجب إضافة ':' بعد ' " + SYNTAX[p] + " ' ", o_tokens);
      // if (SYNTAX[p + 2] == "")
      // ErrorCode("يجب إضافة إسم منتمي للصنف بعد ' " + SYNTAX[p] + " : ' ",
      // o_tokens);

      if (SYNTAX[p + 1] != ":")
        ErrorCode("يجب وضع نقطتين ':' بين الكائن و المنتمي، أي بعد ' " +
                      SYNTAX[p] + " ' ",
                  o_tokens);
      if (SYNTAX[p + 2] == "بناء")
        ErrorCode("لا يمكن استدعاء دالة ' بناء() '، هته الدالة تستدعى بشكل آلي "
                  "عند إنشاء الكائن",
                  o_tokens);
      if (SYNTAX[p + 2] == "هدم")
        ErrorCode("لا يمكن استدعاء دالة ' هدم() '، هته الدالة تستدعى بشكل آلي "
                  "عند الحاجة إلى تدمير هذا الكائن",
                  o_tokens);
      if (SYNTAX[p + 2] == "")
        ErrorCode("يجب وضع اسم المنتمي بعد ' " + SYNTAX[p] + ":' ", o_tokens);

      // C++, allow create Obj on global, global-class, local, but using it only
      // on func.
      std::string OBJ_ID;
      bool IS_GLOBAL_OBJ = false;
      if (OBJ_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS + TmpFunction,
                                    SYNTAX[p])]) {
        // Local Obj.
        // Namespace -> Func -> Obj. Or
        // Class  -> Func -> Obj.
        OBJ_ID = TMP_NAMESPACE_OR_CLASS + TmpFunction;
      } else if (OBJ_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                           SYNTAX[p])] &&
                 TMP_NAMESPACE_OR_CLASS != "") {
        // Global Class Obj.
        // Class -> Obj.
        OBJ_ID = TMP_NAMESPACE_OR_CLASS;
      } else {
        // Global Obj.
        // Global-Area -> Obj.
        OBJ_ID = "";
        IS_GLOBAL_OBJ = true;
      }

      std::string TK_CLASS = OBJ_CLASS[std::make_pair(OBJ_ID, SYNTAX[p])];

      if (!CLASS_G_VAR_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])] &&
          !CLASS_FUN_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
        ErrorCode("الصنف ' " + TK_CLASS + " ' ليس فيه أي منتمي معرف باسم ' " +
                      SYNTAX[p + 2] + " ' ",
                  o_tokens);

      std::string MEMBER_TYPE;

      if (CLASS_G_VAR_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])]) {
        // متغير member

        // a = obj : mem + c

        if (CLASS_G_VAR_PRIVATE[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
          ErrorCode("لا يمكن استدعاء المتغير ' " + SYNTAX[p + 2] +
                        " ' المنتمي للصنف ' " + TK_CLASS +
                        " ' لأنه من نوع ' خاص ' ",
                    o_tokens);

        MEMBER_TYPE = CLASS_G_VAR_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])];

        if (MEMBER_TYPE == "عادم")
          ErrorCode("لا يمكن إضافة منتمي عادم ' " + SYNTAX[p] + " : " +
                        SYNTAX[p + 2] + " ' ",
                    o_tokens);

        if (MEMBER_TYPE == "عدد") {
          if (OBJECTIF_TYPE == "عدد") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(INT)] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + "." +
                              Global_ID[SYNTAX[p + 2]] + " ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(INT)] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." +
                              Global_ID[SYNTAX[p + 2]] + " ");
              // *** *** *** *** *** ***
            }
          } else if (OBJECTIF_TYPE == "نص") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(INT).ToString] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" alifcore_IntToString(" +
                              GlobalObj_ID[SYNTAX[p]] + "." +
                              Global_ID[SYNTAX[p + 2]] + ") ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(INT).ToString] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" alifcore_IntToString(" + Obj_ID[SYNTAX[p]] +
                              "." + Global_ID[SYNTAX[p + 2]] + ") ");
              // *** *** *** *** *** ***
            }
          } else if (OBJECTIF_TYPE == "منطق") {
            ErrorCode("لا يمكن تحويل عدد إلى منطق ' " + SYNTAX[p] + " ':'" +
                          SYNTAX[p + 2] + " ' ",
                      o_tokens);
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + " ' ",
                      o_tokens);
          }
        } else if (MEMBER_TYPE == "نص") {
          if (OBJECTIF_TYPE == "عدد") {
            ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ",
                      o_tokens);
          } else if (OBJECTIF_TYPE == "نص") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(STRING)] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + "." +
                              Global_ID[SYNTAX[p + 2]] + " ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(STRING)] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." +
                              Global_ID[SYNTAX[p + 2]] + " ");
              // *** *** *** *** *** ***
            }
          } else if (OBJECTIF_TYPE == "منطق") {
            ErrorCode("لا يمكن تحويل نص إلى منطق ' " + SYNTAX[p] + " ':'" +
                          SYNTAX[p + 2] + " ' ",
                      o_tokens);
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + " ' ",
                      o_tokens);
          }
        } else if (MEMBER_TYPE == "منطق") {
          if (OBJECTIF_TYPE != "منطق") {
            ErrorCode("لا يمكن تحويل منتمي منطق إلى " + OBJECTIF_TYPE + " ' " +
                          SYNTAX[p] + " ' ",
                      o_tokens);
          } else if (OBJECTIF_TYPE == "منطق") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(BOOL)] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + "." +
                              Global_ID[SYNTAX[p + 2]] + " ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(BOOL)] ",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." +
                              Global_ID[SYNTAX[p + 2]] + " ");
              // *** *** *** *** *** ***
            }
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + " ' ",
                      o_tokens);
          }
        } else {
          ErrorCode("علة : نوع المنتمي غير معروف ' " + MEMBER_TYPE + " ' ل ' " +
                        SYNTAX[p] + " ' ",
                    o_tokens);
        }

        p += 2; // point إلى next token بعد obj:mem
      } else if (CLASS_FUN_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])]) {
        // Function member

        // b = obj : memf (a, b)

        if (CLASS_FUN_PRIVATE[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
          ErrorCode("الدالة منتمي ' " + SYNTAX[p + 2] + " ' داخل الصنف ' " +
                        TK_CLASS + " ' خاص ",
                    o_tokens);

        if (CLASS_FUN_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])] == "عادم")
          ErrorCode("لا يمكن إضافة منتمي عادم ' " + SYNTAX[p] + " : " +
                        SYNTAX[p + 2] + " ' داخل الصنف ' " + TK_CLASS + " ' ",
                    o_tokens);

        if (SYNTAX[p + 3] != "(")
          ErrorCode("يجب إضافة '(' بعد ' " + SYNTAX[p] + ":" + SYNTAX[p + 2] +
                        " ' ",
                    o_tokens);

        MEMBER_TYPE = CLASS_FUN_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])];

        int TMP_FUN_LONG = p + 4; // a + b + 'p'obj : memf (a, b) + c
        int OPEN_PARENTIZE = 0;

        while (TMP_FUN_LONG <= SYNTAX_LONG) {
          if (SYNTAX[TMP_FUN_LONG] ==
              "(") // مفتوح inside الدالة args : fun( a + (b))
            OPEN_PARENTIZE++;
          else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                   OPEN_PARENTIZE > 0) // Close inside الدالة args
            OPEN_PARENTIZE--;
          else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                   OPEN_PARENTIZE < 1) // Close final الدالة call
          {
            if (TMP_FUN_LONG < SYNTAX_LONG) {
              // a = fun( a + (b)) + 123
              // str / عدد = متغير + (fun(var)) * (fun(var) / fun(var, fun(var),
              // var) - var)

              if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "-") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "*") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "/") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "،") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != ",") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != ")")) {
                ErrorCode("نص غير معروف بعد نداء ' " + SYNTAX[p] + " : " +
                              SYNTAX[p + 2] + "()' : ' " +
                              SYNTAX[TMP_FUN_LONG + 1] + " ' ",
                          o_tokens);
              }
            } else if (TMP_FUN_LONG == SYNTAX_LONG) {
              // a = obj:fun( a + (b))
              if (SYNTAX[TMP_FUN_LONG] != ")" ||
                  SYNTAX[SYNTAX_LONG] != ")") // double check!
                ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] + " : " +
                              SYNTAX[p + 2] + "()' بالإشارة ')' ",
                          o_tokens);
            }

            break;
          }

          TMP_FUN_LONG++;
        }

        if (SYNTAX[TMP_FUN_LONG] != ")") // Double check!
          ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] + " : " +
                        SYNTAX[p + 2] + "()' بالإشارة ')' ",
                    o_tokens);

        std::string TempToken[1024];
        int TempTokenCount = 0;
        for (int i = p + 4; i <= TMP_FUN_LONG; i++) {
          if (SYNTAX[i] != "") {
            TempToken[TempTokenCount] = SYNTAX[i];
            TempTokenCount++;
          }
        }

        if (MEMBER_TYPE == "عدد") {
          if (OBJECTIF_TYPE == "عدد") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-INT) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                              ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-INT) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + ".ClassFUNCTION_" +
                              ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            }

            CPP_CODE.append(CHECK_CALL_FUN_ARG(false, TK_CLASS, SYNTAX[p + 2],
                                               2, // 2 = الدالة member
                                               TheNamespace, TheFunction,
                                               TempToken, (TempTokenCount - 1),
                                               o_tokens));

            if (DEBUG)
              DEBUG_MESSAGE(")] ", o_tokens); // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" ) ");
            // *** *** *** *** *** ***

          } else if (OBJECTIF_TYPE == "نص") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-INT) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" alifcore_IntToString(" +
                              GlobalObj_ID[SYNTAX[p]] + ".ClassFUNCTION_" +
                              ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-INT) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" alifcore_IntToString(" + Obj_ID[SYNTAX[p]] +
                              ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            }

            CPP_CODE.append(CHECK_CALL_FUN_ARG(false, TK_CLASS, SYNTAX[p + 2],
                                               2, // 2 = الدالة member
                                               TheNamespace, TheFunction,
                                               TempToken, (TempTokenCount - 1),
                                               o_tokens));

            if (DEBUG)
              DEBUG_MESSAGE(").IntFunToString] ", o_tokens); // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" )) ");
            // *** *** *** *** *** ***
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] +
                          "()' ",
                      o_tokens);
          }
        } else if (MEMBER_TYPE == "نص") {
          if (OBJECTIF_TYPE == "عدد") {
            ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " : " +
                          SYNTAX[p + 2] + "()' ",
                      o_tokens);
          } else if (OBJECTIF_TYPE == "نص") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-STRING) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                              ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-STRING) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + ".ClassFUNCTION_" +
                              ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            }

            CPP_CODE.append(CHECK_CALL_FUN_ARG(false, TK_CLASS, SYNTAX[p + 2],
                                               2, // 2 = الدالة member
                                               TheNamespace, TheFunction,
                                               TempToken, (TempTokenCount - 1),
                                               o_tokens));

            if (DEBUG)
              DEBUG_MESSAGE(")] ", o_tokens); // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" ) ");
            // *** *** *** *** *** ***
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] +
                          "()' ",
                      o_tokens);
          }
        } else if (MEMBER_TYPE == "منطق") {
          if (OBJECTIF_TYPE != "منطق") {
            ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " +
                          SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ",
                      o_tokens);
          } else if (OBJECTIF_TYPE == "منطق") {
            if (IS_GLOBAL_OBJ) {
              if (DEBUG)
                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-BOOL) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                              ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            } else {
              if (DEBUG)
                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" +
                                  SYNTAX[p + 2] + " '(Func-BOOL) (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + ".ClassFUNCTION_" +
                              ID[SYNTAX[p + 2]] + "( ");
              // *** *** *** *** *** ***
            }

            CPP_CODE.append(CHECK_CALL_FUN_ARG(false, TK_CLASS, SYNTAX[p + 2],
                                               2, // 2 = الدالة member
                                               TheNamespace, TheFunction,
                                               TempToken, (TempTokenCount - 1),
                                               o_tokens));

            if (DEBUG)
              DEBUG_MESSAGE(")] ", o_tokens); // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" ) ");
            // *** *** *** *** *** ***
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] +
                          "()' ",
                      o_tokens);
          }
        } else {
          ErrorCode("علة : نوع المنتمي غير معروف ' " + MEMBER_TYPE + " ' ل ' " +
                        SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ",
                    o_tokens);
        }

        p = TMP_FUN_LONG; // point إلى next token بعد fun(...)
      }
    }

    // ----------------------
    // Class Global VARIABLE
    // ----------------------

    // class
    //   g1 = 123
    //   g2 = g1<--

    else if (CLASS_G_VAR_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                               SYNTAX[p])] &&
             IsInsideClass) {
      // Class Global area

      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (CLASS_G_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])] ==
          "عدد") {
        if (OBJECTIF_TYPE == "عدد") {
          if (DEBUG)
            DEBUG_MESSAGE("[CLASS-GLOBAL-INT (" + SYNTAX[p] + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        } else if (OBJECTIF_TYPE == "نص") {
          if (ACCEPT_INT_TO_STRING) {
            if (DEBUG)
              DEBUG_MESSAGE("[CLASS-GLOBAL-INT (" + SYNTAX[p] + ").ToString] ",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" alifcore_IntToString(" + Global_ID[SYNTAX[p]] +
                            ") ");
            // *** *** *** *** *** ***
          } else {
            ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                      o_tokens);
          }
        } else if (OBJECTIF_TYPE == "منطق") {
          ErrorCode("لا يمكن تحويل عدد إلى منطق : ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        } else if (OBJECTIF_TYPE == "C++") {
          if (DEBUG)
            DEBUG_MESSAGE("[C++ CLASS-GLOBAL-INT (" + SYNTAX[p] + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        } else {
          ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                        " ' ل ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        }
      } else if (CLASS_G_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                                 SYNTAX[p])] == "نص") {
        if (OBJECTIF_TYPE == "عدد") {
          // if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" + SYNTAX[p] + ").ToInt]
          // ", o_tokens); // DEBUG
          ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        } else if (OBJECTIF_TYPE == "نص") {
          if (DEBUG)
            DEBUG_MESSAGE("[CLASS-GLOBAL-STRING (" + SYNTAX[p] + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        } else if (OBJECTIF_TYPE == "منطق") {
          ErrorCode("لا يمكن تحويل نص إلى منطق : ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        } else if (OBJECTIF_TYPE == "C++") {
          if (DEBUG)
            DEBUG_MESSAGE("[C++ CLASS-GLOBAL-STRING (" + SYNTAX[p] + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        } else {
          ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                        " ' ل ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        }
      } else if (CLASS_G_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                                 SYNTAX[p])] == "منطق") {
        if (OBJECTIF_TYPE == "C++") {
          if (DEBUG)
            DEBUG_MESSAGE("[C++ CLASS-GLOBAL-BOOL (" + SYNTAX[p] + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        } else if (OBJECTIF_TYPE != "منطق") {
          ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " +
                        SYNTAX[p] + " ' ",
                    o_tokens);
        } else if (OBJECTIF_TYPE == "منطق") {
          if (DEBUG)
            DEBUG_MESSAGE("[CLASS-GLOBAL-BOOL (" + SYNTAX[p] + ")] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        } else {
          ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                        " ' ل ' " + SYNTAX[p] + " ' ",
                    o_tokens);
        }
      } else {
        ErrorCode("علة : نوع المتغير العام غير معروف ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // ----------------------
    // Global VARIABLE without _
    // ----------------------

    // g1 = 123
    // g2 = _g1 + 'g1' <--

    /*
    else if (G_VAR_IS_SET[(SYNTAX[p])])
    {
            ...
    }
    */

    // -----------------------------------
    // TXT
    // -----------------------------------

    else if (substr_utf8(SYNTAX[p], 0, 1) == "\"") {
      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة نص هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (!IsValidStringFormat(SYNTAX[p], o_tokens))
        ErrorCode("ليس بنص صحيح ' " + SYNTAX[p] + " ' ", o_tokens);

      if (OBJECTIF_TYPE == "عدد") {
        // if(DEBUG)DEBUG_MESSAGE("[STRING_MSG (" + SYNTAX[p] + ").ToInt] ",
        // o_tokens); // DEBUG
        ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ", o_tokens);
      } else if (OBJECTIF_TYPE == "نص") {
        if (DEBUG)
          DEBUG_MESSAGE("[STRING_MSG (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" (" + SYNTAX[p] + ") ");
        // *** *** *** *** *** ***
      } else if (OBJECTIF_TYPE == "منطق") {
        ErrorCode("لا يمكن تحويل نص إلى منطق ' " + SYNTAX[p] + " ' ", o_tokens);
      } else if (OBJECTIF_TYPE == "C++") {
        if (DEBUG)
          DEBUG_MESSAGE("[C++ نص (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" (" + SYNTAX[p] + ") ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                      " ' ل ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // -----------------------------------
    // DIGIT
    // -----------------------------------

    else if (IsValidDigit(SYNTAX[p], true, o_tokens)) {
      if (p > 0)
        if (!CAN_ADD_DIGIT_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن إضافة رقم هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (OBJECTIF_TYPE == "عدد") {
        if (DEBUG)
          DEBUG_MESSAGE("[DIGIT " + SYNTAX[p] + "] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + SYNTAX[p] + " ");
        // *** *** *** *** *** ***
      } else if (OBJECTIF_TYPE == "نص") {
        if (ACCEPT_INT_TO_STRING) {
          if (DEBUG)
            DEBUG_MESSAGE("[DIGIT (" + SYNTAX[p] + ").ToString] ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" alifcore_IntToString(" + SYNTAX[p] + ") ");
          // *** *** *** *** *** ***
        } else {
          ErrorCode("لا يمكن تحويل رقم إلى نص : ' " + SYNTAX[p] + " '",
                    o_tokens);
        }
      } else if (OBJECTIF_TYPE == "منطق") {
        ErrorCode("لا يمكن تحويل رقم إلى منطق ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      } else if (OBJECTIF_TYPE == "C++") {
        if (DEBUG)
          DEBUG_MESSAGE("[C++ رقم (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

        // *** Generate Code ***
        CPP_CODE.append(" " + SYNTAX[p] + " ");
        // *** *** *** *** *** ***
      } else {
        ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                      " ' ل ' " + SYNTAX[p] + " ' ",
                  o_tokens);
      }
    }

    // -----------------------------------
    // Function / Class-Function
    // -----------------------------------

    else if (CLASS_FUN_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                             SYNTAX[p])] ||
             G_FUN_IS_SET[(SYNTAX[p])] ||
             L_FUN_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                         SYNTAX[p])]) // call الدالة (a, b)
    {
      if (p > 0)
        if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
          ErrorCode("لا يمكن وضع دالة هنا ' " + SYNTAX[p - 1] + " " +
                        SYNTAX[p] + " ' ",
                    o_tokens);

      if (!IsInsideFunction)
        ErrorCode("يجب مناداة على الدالة من داخل دالة ' " + SYNTAX[p] + "()' ",
                  o_tokens);

      if (OBJECTIF_TYPE == "مؤشر_دالة") {

        // ... function_name -> to an pointer-func

        std::string FUN_TYPE;
        bool IS_LOCAL_FUN = false;
        IS_LOCAL_FUN =
            L_FUN_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];

        if (IS_LOCAL_FUN) {

          FUN_TYPE =
              L_FUN_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];
          if (DEBUG)
            DEBUG_MESSAGE("[REFERENCE-OF-LOCAL-FUNCTION-" + FUN_TYPE + " ' " +
                              SYNTAX[p] + " ']",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" NS_" + ID[TheNamespace] + "::FUNCTION_" +
                          ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        } else {

          FUN_TYPE = G_FUN_TYPE[(SYNTAX[p])];
          if (DEBUG)
            DEBUG_MESSAGE("[REFERENCE-OF-GLOBAL-FUNCTION-" + FUN_TYPE + " ' " +
                              SYNTAX[p] + " ']",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          CPP_CODE.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + " ");
          // *** *** *** *** *** ***
        }
      } else {

        // ... function_name(...) -> to an anythings except pointers

        if (SYNTAX[p + 1] == "")
          ErrorCode("يجب اضافه '(' بعد ' " + SYNTAX[p] + " ' ", o_tokens);

        if (SYNTAX[p + 1] != "(")
          ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " ' ", o_tokens);

        bool ThisIsClassFunction = false;
        if (CLASS_FUN_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])])
          ThisIsClassFunction = true;

        // if(ThisIsClassFunction)
        // 	ErrorCode("_TEST_TEST_TEST_' " + SYNTAX[p + 1] + " ' ",
        // o_tokens);

        int TMP_FUN_LONG = p + 2; // a + b + 'p'fun ( c + (1 * 2) ) + c
        int OPEN_PARENTIZE = 0;

        while (TMP_FUN_LONG <= SYNTAX_LONG) {
          if (SYNTAX[TMP_FUN_LONG] ==
              "(") // مفتوح inside الدالة args : fun( a + (b))
            OPEN_PARENTIZE++;
          else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                   OPEN_PARENTIZE > 0) // Close inside الدالة args
            OPEN_PARENTIZE--;
          else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                   OPEN_PARENTIZE < 1) // Close final الدالة call
          {
            if (TMP_FUN_LONG < SYNTAX_LONG) {
              // a = fun( a + (b)) + 123
              // str / عدد = متغير + (fun(var)) * (fun(var) / fun(var, fun(var),
              // var) - var)

              if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "-") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "*") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "/") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != "،") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != ",") &&
                  (SYNTAX[TMP_FUN_LONG + 1] != ")")) {
                ErrorCode("نص غير معروف بعد نداء ' " + SYNTAX[p] + "()' : ' " +
                              SYNTAX[TMP_FUN_LONG + 1] + " ' ",
                          o_tokens);
              }
            } else if (TMP_FUN_LONG == SYNTAX_LONG) {
              // a = fun( a + (b))
              if (SYNTAX[TMP_FUN_LONG] != ")") // double check!
                ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] +
                              "()' بالإشارة ')' ",
                          o_tokens);
            }

            break;
          }

          TMP_FUN_LONG++;
        }

        if (SYNTAX[TMP_FUN_LONG] != ")") // Double check!
          ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] +
                        "()' بالإشارة ')' ",
                    o_tokens);
        // ErrorCode("===== |" + SYNTAX[TMP_FUN_LONG - 1] + "| =====",
        // o_tokens);

        std::string TempToken[1024];
        int TempTokenCount = 0;
        for (int i = p + 2; i <= TMP_FUN_LONG; i++) {
          if (SYNTAX[i] != "") {
            TempToken[TempTokenCount] = SYNTAX[i];
            TempTokenCount++;
          }
        }

        bool IS_LOCAL_FUN = false;
        IS_LOCAL_FUN =
            L_FUN_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];

        std::string FUN_TYPE;

        if (IS_LOCAL_FUN) {
          FUN_TYPE =
              L_FUN_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];

          if (FUN_TYPE == "عادم")
            ErrorCode("الدالة المحلية ' " + SYNTAX[p] +
                          "()' من نوع عادم, لدى لا يمكن تحويلها إلى " +
                          OBJECTIF_TYPE,
                      o_tokens);
        } else {
          FUN_TYPE = G_FUN_TYPE[(SYNTAX[p])];

          if (FUN_TYPE == "عادم")
            ErrorCode("الدالة العامة ' " + SYNTAX[p] +
                          "()' من نوع عادم, لدى لا يمكن تحويل إلى " +
                          OBJECTIF_TYPE,
                      o_tokens);
        }

        std::string CG_BUFFER;

        if (OBJECTIF_TYPE == "عدد") {
          if (FUN_TYPE == "عدد") {
            if (IS_LOCAL_FUN) {
              // Call a local fun int

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-INT ' " + SYNTAX[p] +
                                  " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" NS_" + ID[TheNamespace] + "::FUNCTION_" +
                               ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  false, TMP_NAMESPACE_OR_CLASS, SYNTAX[p], 0,
                  TMP_NAMESPACE_OR_CLASS, TheFunction, TempToken,
                  (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ");
              // *** *** *** *** *** ***
            } else {
              // Call a Global fun int

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-INT ' " + SYNTAX[p] +
                                  " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS, TheFunction,
                  TempToken, (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ");
              // *** *** *** *** *** ***
            }
          } else if (FUN_TYPE == "نص") {
            if (IS_LOCAL_FUN)
              ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] +
                            "()' من نص إلى عدد ",
                        o_tokens);
            else
              ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] +
                            "()' من نص إلى عدد ",
                        o_tokens);
          } else if (FUN_TYPE == "منطق") {
            if (IS_LOCAL_FUN)
              ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] +
                            "()' من منطق إلى عدد ",
                        o_tokens);
            else
              ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] +
                            "()' من منطق إلى عدد ",
                        o_tokens);
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + "()' ",
                      o_tokens);
          }
        } else if (OBJECTIF_TYPE == "نص") {
          if (FUN_TYPE == "عدد") {
            if (IS_LOCAL_FUN) {
              // Call a local fun int.ToString

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-INT.ToString ' " +
                                  SYNTAX[p] + " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" alifcore_IntToString( OBJ_CLASS_WINDOW_" +
                               ID[TheNamespace] + "::FUNCTION_" +
                               ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  false, TMP_NAMESPACE_OR_CLASS, SYNTAX[p], 0,
                  TMP_NAMESPACE_OR_CLASS, TheFunction, TempToken,
                  (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ) ");
              // *** *** *** *** *** ***
            } else {
              // Call a Global fun int.ToString

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-INT.ToString ' " +
                                  SYNTAX[p] + " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" alifcore_IntToString( FUNCTION_" +
                               Global_ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS, TheFunction,
                  TempToken, (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ) ");
              // *** *** *** *** *** ***
            }
          } else if (FUN_TYPE == "نص") {
            if (IS_LOCAL_FUN) {
              // Call a local fun std::string

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-STRING ' " + SYNTAX[p] +
                                  " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" NS_" + ID[TheNamespace] + "::FUNCTION_" +
                               ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  false, TMP_NAMESPACE_OR_CLASS, SYNTAX[p], 0,
                  TMP_NAMESPACE_OR_CLASS, TheFunction, TempToken,
                  (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ");
              // *** *** *** *** *** ***

              if (DEBUG)
                DEBUG_MESSAGE(") ", o_tokens); // DEBUG
            } else {
              // Call a Global fun int

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-STRING ' " + SYNTAX[p] +
                                  " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS, TheFunction,
                  TempToken, (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ");
              // *** *** *** *** *** ***
            }
          } else if (FUN_TYPE == "منطق") {
            if (IS_LOCAL_FUN)
              ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] +
                            "()' من منطق إلى نص ",
                        o_tokens);
            else
              ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] +
                            "()' من منطق إلى نص ",
                        o_tokens);
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + "()' ",
                      o_tokens);
          }
        } else if (OBJECTIF_TYPE == "منطق") {
          if (FUN_TYPE == "عدد") {
            if (IS_LOCAL_FUN)
              ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] +
                            "()' من عدد إلى منطق ",
                        o_tokens);
            else
              ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] +
                            "()' من عدد إلى منطق ",
                        o_tokens);
          } else if (FUN_TYPE == "نص") {
            if (IS_LOCAL_FUN)
              ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] +
                            "()' من نص إلى منطق ",
                        o_tokens);
            else
              ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] +
                            "()' من نص إلى منطق ",
                        o_tokens);
          } else if (FUN_TYPE == "منطق") {
            if (IS_LOCAL_FUN) {
              // Call a local fun bool

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-BOOL ' " + SYNTAX[p] +
                                  " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" NS_" + ID[TheNamespace] + "::FUNCTION_" +
                               ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  false, TMP_NAMESPACE_OR_CLASS, SYNTAX[p], 0,
                  TMP_NAMESPACE_OR_CLASS, TheFunction, TempToken,
                  (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ");
              // *** *** *** *** *** ***
            } else {
              // Call a Global fun bool

              if (DEBUG)
                DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-BOOL ' " + SYNTAX[p] +
                                  " '] (",
                              o_tokens); // DEBUG

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
              // *** *** *** *** *** ***

              CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                  true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS, TheFunction,
                  TempToken, (TempTokenCount - 1), o_tokens));

              // *** Generate Code ***
              // Buffer
              CG_BUFFER.append(" ) ");
              // *** *** *** *** *** ***
            }
          } else {
            ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                          " ' ل ' " + SYNTAX[p] + "()' ",
                      o_tokens);
          }
        } else if (OBJECTIF_TYPE == "C++") {

          // xType: We igiore type of function.. this is C++ targed
          // user must take care of this!.

          if (IS_LOCAL_FUN) {

            // Call a local fun xType

            if (DEBUG)
              DEBUG_MESSAGE("[C++ CALL-LOCAL-FUNCTION-xType ' " + SYNTAX[p] +
                                " '] (",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Buffer
            CG_BUFFER.append(" NS_" + ID[TheNamespace] + "::FUNCTION_" +
                             ID[SYNTAX[p]] + "( ");
            // *** *** *** *** *** ***

            CG_BUFFER.append(
                CHECK_CALL_FUN_ARG(false, TMP_NAMESPACE_OR_CLASS, SYNTAX[p], 0,
                                   TMP_NAMESPACE_OR_CLASS, TheFunction,
                                   TempToken, (TempTokenCount - 1), o_tokens));

            // *** Generate Code ***
            // Buffer
            CG_BUFFER.append(" ) ");
            // *** *** *** *** *** ***
          } else {
            // Call a Global fun xType

            if (DEBUG)
              DEBUG_MESSAGE("[C++ CALL-GLOBAL-FUNCTION-xType ' " + SYNTAX[p] +
                                " '] (",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Buffer
            CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
            // *** *** *** *** *** ***

            CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS, TheFunction,
                TempToken, (TempTokenCount - 1), o_tokens));

            // *** Generate Code ***
            // Buffer
            CG_BUFFER.append(" ) ");
            // *** *** *** *** *** ***
          }
        }

        // *** Generate Code ***
        CPP_CODE.append(" " + CG_BUFFER + " ");
        // *** *** *** *** *** ***

        p = TMP_FUN_LONG; // point to next token after fun(...)
      }
    }

    // -----------------------------------
    // : (Operator between members) v3
    // -----------------------------------

    else if (namespace_is_set[SYNTAX[p]] || SYNTAX[p + 1] == ":") {

      // abc = namespace:local_func(a, b)	| namespace:my_func(a, b)
      // abc = namespace:local_var		| namespace:my_var

      string CTR_WIN;
      string CTR_CONTROL;
      string CTR_OPTION;
      string CTR_OPTION_TYPE;
      string CTR_OPTION_CPP_END;
      // int CTR_ARG;
      int CTR_LAST_TOKEN_NUMBER = 0;

      if (namespace_is_set[SYNTAX[p]]) {

        // abc = namespace:local_func(a, b)	| namespace:my_func(a, b)
        // abc = namespace:local_var		| namespace:my_var

        //	namespace_name	:		local_func		( ... )
        //	p				+1		+2
        //+3

        if (SYNTAX[p + 1] != ":")
          ErrorCode("يجب اضافه ' : ' بعد ' " + SYNTAX[p] + " ' ", o_tokens);

        if (SYNTAX[p + 2] == "")
          ErrorCode("يجب اضافه عضو تابع ل ' " + SYNTAX[p] + " ' بعد ':' ",
                    o_tokens);

        if (SYNTAX[p + 3] == "")
          ErrorCode("يجب اضافه ':' أو '()' بعد ' " + SYNTAX[p] + " " +
                        SYNTAX[p + 1] + " " + SYNTAX[p + 2] + " ' ",
                    o_tokens);

        if (SYNTAX[p + 3] != "(" && SYNTAX[p + 3] != ":")
          ErrorCode("أمر غير معروف ' " + SYNTAX[p + 3] +
                        " ', يجب اضافه ':' أو '()' ",
                    o_tokens);

        if (SYNTAX[p + 3] == "(") {

          if (L_FUN_IS_SET[std::make_pair(SYNTAX[p], SYNTAX[p + 2])]) {

            // ---------------------------------------------------------------------------------
            // namespace : member_function ()
            // ---------------------------------------------------------------------------------

            // abc = namespace:local_func(a, b) | namespace:my_func(a, b)

            if (SYNTAX[p + 3] != "(")
              ErrorCode("من اجل نداء الدالة ' " + SYNTAX[p + 2] +
                            " ' يجب اضافه '()' بعد ' " + SYNTAX[p] + " " +
                            SYNTAX[p + 1] + " " + SYNTAX[p + 2] + " ' ",
                        o_tokens);

            std::string FUN_TYPE;
            std::string CPP_END;

            FUN_TYPE =
                L_FUN_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];

            if (FUN_TYPE == "عادم")
              ErrorCode("الدالة المحلية ' " + SYNTAX[p] +
                            "()' من نوع عادم, لدى لا يمكن تحويلها إلى " +
                            OBJECTIF_TYPE,
                        o_tokens);

            if (OBJECTIF_TYPE == "عدد") {
              if (FUN_TYPE == "عدد") {
                if (DEBUG)
                  DEBUG_MESSAGE("[NS'" + SYNTAX[p] + " ':LOCAL_INT_FUNC'" +
                                    SYNTAX[p + 2] + " '( ",
                                o_tokens); // DEBUG

                // *** Generate Code ***
                // Buffer
                CPP_END = " ";
                CPP_CODE.append(" NS_" + ID[SYNTAX[p]] + "::FUNCTION_" +
                                ID[SYNTAX[p + 2]] + "( ");
                // *** *** *** *** *** ***
              } else {
                ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p + 2] +
                              "()' من " + FUN_TYPE + " إلى عدد ",
                          o_tokens);
              }
            } else if (OBJECTIF_TYPE == "نص") {
              if (FUN_TYPE == "نص") {
                if (DEBUG)
                  DEBUG_MESSAGE("[NS'" + SYNTAX[p] + " ':LOCAL_STRING_FUNC'" +
                                    SYNTAX[p + 2] + " '( ",
                                o_tokens); // DEBUG

                // *** Generate Code ***
                // Buffer
                CPP_END = " ";
                CPP_CODE.append(" NS_" + ID[SYNTAX[p]] + "::FUNCTION_" +
                                ID[SYNTAX[p + 2]] + "( ");
                // *** *** *** *** *** ***
              } else if (FUN_TYPE == "عدد") {
                if (DEBUG)
                  DEBUG_MESSAGE("[NS'" + SYNTAX[p] + " ':LOCAL_INT_FUNC'" +
                                    SYNTAX[p + 2] + " '().ToString( ",
                                o_tokens); // DEBUG

                // *** Generate Code ***
                // Buffer
                CPP_END = " ) ";
                CPP_CODE.append(" alifcore_IntToString( OBJ_CLASS_WINDOW_" +
                                ID[SYNTAX[p]] + "::FUNCTION_" +
                                ID[SYNTAX[p + 2]] + "( ");
                // *** *** *** *** *** ***
              } else {
                ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p + 2] +
                              "()' من " + FUN_TYPE + " إلى نص ",
                          o_tokens);
              }
            } else if (OBJECTIF_TYPE == "منطق") {
              if (FUN_TYPE == "منطق") {
                if (DEBUG)
                  DEBUG_MESSAGE("[NS'" + SYNTAX[p] + " ':LOCAL_BOOL_FUNC'" +
                                    SYNTAX[p + 2] + " '( ",
                                o_tokens); // DEBUG

                // *** Generate Code ***
                // Buffer
                CPP_END = " ";
                CPP_CODE.append(" NS_" + ID[SYNTAX[p]] + "::FUNCTION_" +
                                ID[SYNTAX[p + 2]] + "( ");
                // *** *** *** *** *** ***
              } else {
                ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p + 2] +
                              "()' من " + FUN_TYPE + " إلى منطق ",
                          o_tokens);
              }
            } else
              ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                            " ' ل ' " + SYNTAX[p + 2] + " ' ",
                        o_tokens);

            // abc = (p)namespace:local_func(a, b) + x + y

            int TMP_FUN_LONG = p + 4;
            int OPEN_PARENTIZE = 0;

            // Get Local Function Args
            while (TMP_FUN_LONG <= SYNTAX_LONG) {
              if (SYNTAX[TMP_FUN_LONG] ==
                  "(") // مفتوح inside الدالة args : fun( a + (b))
                OPEN_PARENTIZE++;
              else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                       OPEN_PARENTIZE > 0) // Close inside الدالة args
                OPEN_PARENTIZE--;
              else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                       OPEN_PARENTIZE < 1) // Close final الدالة call
              {
                if (TMP_FUN_LONG < SYNTAX_LONG) {
                  // abc = fun( a + (b)) + 123
                  // abc = x + (fun(var)) * (fun(var) / fun(var, fun(var), var)
                  // - var)

                  if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
                      (SYNTAX[TMP_FUN_LONG + 1] != "-") &&
                      (SYNTAX[TMP_FUN_LONG + 1] != "*") &&
                      (SYNTAX[TMP_FUN_LONG + 1] != "/") &&
                      (SYNTAX[TMP_FUN_LONG + 1] != "،") &&
                      (SYNTAX[TMP_FUN_LONG + 1] != ",") &&
                      (SYNTAX[TMP_FUN_LONG + 1] != ")")) {
                    ErrorCode("نص غير معروف بعد نداء ' " + SYNTAX[p + 2] +
                                  "()' : ' " + SYNTAX[TMP_FUN_LONG + 1] + " ' ",
                              o_tokens);
                  }
                } else if (TMP_FUN_LONG == SYNTAX_LONG) {
                  // a = fun( a + (b))
                  if (SYNTAX[TMP_FUN_LONG] != ")" ||
                      SYNTAX[SYNTAX_LONG] != ")") // double check!
                    ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p + 2] +
                                  "()' بالإشارة ')' ",
                              o_tokens);
                }

                break;
              }

              TMP_FUN_LONG++;
            }

            if (SYNTAX[TMP_FUN_LONG] != ")") // Double check!
              ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p + 2] +
                            "()' بالإشارة ')' ",
                        o_tokens);

            std::string TempToken[1024];
            int TempTokenCount = 0;
            for (int i = p + 4; i <= TMP_FUN_LONG; i++) {
              if (SYNTAX[i] != "") {
                TempToken[TempTokenCount] = SYNTAX[i];
                TempTokenCount++;
              }
            }

            // Check local fun Args : fun (a + c, 2 * (b - 1))
            CPP_CODE.append(CHECK_CALL_FUN_ARG(
                false, SYNTAX[p], SYNTAX[p + 2], 0, TheNamespace, TheFunction,
                TempToken, (TempTokenCount - 1), o_tokens));

            if (DEBUG)
              DEBUG_MESSAGE(")] \n\n", o_tokens); // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" ) " + CPP_END + " ");
            // *** *** *** *** *** ***

            // Point to last token of full func call
            p = TMP_FUN_LONG;

            continue;
          }

          // TODO: Add support for local var
          // if (L_VAR_IS_SET[...]) <-- abc[namespace][var] = local var already
          // set ?

          else
            ErrorCode("النافذة ' " + SYNTAX[p] +
                          " ' لا تحتوي على دالة محليه بإسم ' " + SYNTAX[p + 2] +
                          " ' ",
                      o_tokens);

          // Exception!
          continue;
        } else {

          // Exception !
          ErrorCode("يجب اضافه ':' أو '()' بعد ' " + SYNTAX[p] + " " +
                        SYNTAX[p + 1] + " " + SYNTAX[p + 2] + " ' ",
                    o_tokens);
        }
      } else {
        // Exception !
        ErrorCode("أمر غير معروف ' " + SYNTAX[p] +
                      " ', كان من المتوقع أن يكون إسم مجال",
                  o_tokens);
      }

      // Point to last token of full namespace:member.
      p = CTR_LAST_TOKEN_NUMBER;
    }

    // -----------------------------------
    // : (Operator between members) v2
    // -----------------------------------

    // else if (CONTROL_WIN_IS_SET[SYNTAX[p]] ||
    // CONTROL_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])])
    // {
    // 	// abc = namespace:option() 				|
    // namespace:show()
    // 	// abc = namespace:Option					|
    // namespace:with
    // 	// abc = namespace:local_func(a, b)		| namespace:my_func(a,
    // b)
    // 	// abc = namespace:Control:Option()		|
    // namespace:ctr:disable()
    // 	// abc = namespace:Control:Option			|
    // namespace:ctr:text

    // 	// abc = Control:Option()				| ctr:disable()
    // 	// abc = Control:Option					| ctr:text

    // 	std::string CTR_WIN;
    // 	std::string CTR_CONTROL;
    // 	std::string CTR_OPTION;
    // 	std::string CTR_OPTION_TYPE;
    // 	std::string CTR_OPTION_CPP_END;
    // 	//int CTR_ARG;
    // 	int CTR_LAST_TOKEN_NUMBER;

    // 	if (CONTROL_WIN_IS_SET[SYNTAX[p]] || namespace_is_set [SYNTAX[p]])
    // 	{
    // 		// abc = namespace:option() 				|
    // namespace:show()
    // 		// abc = namespace:Option | namespace:with
    // 		// abc = namespace:local_func(a, b)		|
    // namespace:my_func(a, b)
    // 		// abc = namespace:Control:Option()		|
    // namespace:ctr:disable()
    // 		// abc = namespace:Control:Option			|
    // namespace:ctr:text

    // 			if (SYNTAX[p + 1] != ":")
    // 				ErrorCode("يجب اضافه ' : ' بعد ' " + SYNTAX[p] +
    // "
    // '
    // ", o_tokens);

    // 			if (SYNTAX[p + 2] == "")
    // 				ErrorCode("يجب اضافه عضو تابع ل ' " + SYNTAX[p] + "
    // ' بعد
    // ':'
    // ", o_tokens);

    // 			if (SYNTAX[p + 3] == "")
    // 				ErrorCode("يجب اضافه ':' أو '()' بعد ' " + SYNTAX[p] +
    // "
    // "
    // + SYNTAX[p
    // + 1] + " " + SYNTAX[p + 2] + " ' ", o_tokens);

    // 			if (SYNTAX[p + 3] != "(" && SYNTAX[p + 3] != ":")
    // 				ErrorCode("أمر غير معروف ' " + SYNTAX[p + 3] + " ',
    // يجب اضافه
    // ':' أو
    // '()' ", o_tokens);

    // 			if (SYNTAX[p + 3] == "(")
    // 			{
    // 				//
    // ---------------------------------------------------------------------------------
    // 				// namespace : Options ()
    // 				//
    // ---------------------------------------------------------------------------------

    // 				// abc = namespace:option()
    // | namespace:show()
    // 				// abc = namespace:local_func(a, b) |
    // namespace:my_func(a, b)

    // 				// -----------------------
    // 				// abc = namespace:win-option()
    // 				// -----------------------

    // 				if (SYNTAX[p + 2] == "إظهار" ||
    // 					SYNTAX[p + 2] == "إخفاء" ||
    // 					SYNTAX[p + 2] == "تدمير")
    // 				{
    // 					ErrorCode("لا يمكن إستعمال خاصية ' " + SYNTAX[p + 2]
    // + " ' هنا، لأنها من نوع عادم" + OBJECTIF_TYPE, o_tokens);
    // 				}

    // 				// -----------------------
    // 				// abc = namespace:win-local_func()
    // 				// -----------------------

    // 				else if (L_FUN_IS_SET[std::make_pair(SYNTAX[p],
    // SYNTAX[p
    // + 2])])
    // 				{
    // 					if (SYNTAX[p + 3] != "(")
    // 						ErrorCode("من اجل نداء الدالة ' " + SYNTAX[p + 2] +
    // " ' يجب اضافه
    // '()' بعد ' " + SYNTAX[p] + " " + 										SYNTAX[p + 1] + "
    // "
    // + SYNTAX[p
    // + 2] + " ' ", o_tokens);

    // 					std::string FUN_TYPE;
    // 					std::string CPP_END;

    // 					FUN_TYPE =
    // L_FUN_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];

    // 					if (FUN_TYPE == "عادم")
    // 						ErrorCode("الدالة المحلية ' " + SYNTAX[p] + "()'
    // من نوع عادم, لدى لا يمكن تحويلها إلى " + OBJECTIF_TYPE, o_tokens);

    // 					if (OBJECTIF_TYPE == "عدد")
    // 					{
    // 						if (FUN_TYPE == "عدد")
    // 						{
    // 							if(DEBUG)DEBUG_MESSAGE("[NS'" + SYNTAX[p]
    // +
    // "
    // ':LOCAL_INT_FUNC'"
    // + SYNTAX[p + 2] + " '( ", o_tokens); // DEBUG

    // 							// *** Generate Code ***
    // 							// Buffer
    // 							CPP_END = " ";
    // 							CPP_CODE.append(" NS_" +
    // ID[SYNTAX[p]]
    // +
    // "::FUNCTION_"
    // + ID[SYNTAX[p + 2]] + "( ");
    // 							// *** *** *** *** ***
    // ***
    // 						}
    // 						else
    // 						{
    // 							ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p
    // + 2]
    // +
    // "()' من " + FUN_TYPE + " إلى عدد ", o_tokens);
    // 						}
    // 					}
    // 					else if (OBJECTIF_TYPE == "نص")
    // 					{
    // 						if (FUN_TYPE == "نص")
    // 						{
    // 							if(DEBUG)DEBUG_MESSAGE("[NS'"
    // + SYNTAX[p]
    // +
    // "
    // ':LOCAL_STRING_FUNC'" + SYNTAX[p + 2] + " '( ", o_tokens); // DEBUG

    // 							// *** Generate Code ***
    // 							// Buffer
    // 							CPP_END = " ";
    // 							CPP_CODE.append(" NS_" +
    // ID[SYNTAX[p]]
    // +
    // "::FUNCTION_"
    // + ID[SYNTAX[p + 2]] + "( ");
    // 							// *** *** *** *** ***
    // ***
    // 						}
    // 						else if (FUN_TYPE == "عدد")
    // 						{
    // 							if(DEBUG)DEBUG_MESSAGE("[NS'" + SYNTAX[p]
    // +
    // "
    // ':LOCAL_INT_FUNC'"
    // + SYNTAX[p + 2] + " '().ToString( ", o_tokens); // DEBUG

    // 							// *** Generate Code ***
    // 							// Buffer
    // 							CPP_END = " ) ";
    // 							CPP_CODE.append("
    // alifcore_IntToString( OBJ_CLASS_WINDOW_"
    // + ID[SYNTAX[p]] + "::FUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
    // 							// *** *** *** *** ***
    // ***
    // 						}
    // 						else
    // 						{
    // 							ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p
    // + 2]
    // +
    // "()' من " + FUN_TYPE + " إلى نص ", o_tokens);
    // 						}
    // 					}
    // 					else if (OBJECTIF_TYPE == "منطق")
    // 					{
    // 						if (FUN_TYPE == "منطق")
    // 						{
    // 							if(DEBUG)DEBUG_MESSAGE("[NS'" +
    // SYNTAX[p]
    // +
    // "
    // ':LOCAL_BOOL_FUNC'"
    // + SYNTAX[p + 2] + " '( ", o_tokens); // DEBUG

    // 							// *** Generate Code ***
    // 							// Buffer
    // 							CPP_END = " ";
    // 							CPP_CODE.append(" NS_" +
    // ID[SYNTAX[p]]
    // +
    // "::FUNCTION_"
    // + ID[SYNTAX[p + 2]] + "( ");
    // 							// *** *** *** *** ***
    // ***
    // 						}
    // 						else
    // 						{
    // 							ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p
    // + 2]
    // +
    // "()' من " + FUN_TYPE + " إلى منطق ", o_tokens);
    // 						}
    // 					}
    // 					else
    // 						ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE
    // + " ' ل ' " + SYNTAX[p + 2] + " ' ", o_tokens);

    // 					// abc = (p)namespace:local_func(a, b) +
    // x
    // +
    // y

    // 					int TMP_FUN_LONG = p + 4;
    // 					int OPEN_PARENTIZE = 0;

    // 					// Get Local Function Args
    // 					while (TMP_FUN_LONG <= SYNTAX_LONG)
    // 					{
    // 						if (SYNTAX[TMP_FUN_LONG] == "(") // مفتوح inside
    // الدالة args : fun( a + (b))
    // OPEN_PARENTIZE++; 						else if
    // (SYNTAX[TMP_FUN_LONG]
    // ==
    // ")" && OPEN_PARENTIZE > 0) // Close inside الدالة args
    // OPEN_PARENTIZE--; 						else if
    // (SYNTAX[TMP_FUN_LONG] == ")" && OPEN_PARENTIZE < 1) // Close final الدالة
    // call
    // 						{
    // 							if (TMP_FUN_LONG <
    // SYNTAX_LONG)
    // 							{
    // 								// abc = fun( a
    // + (b))
    // + 123
    // 								// abc = x + (fun(var)) * (fun(var) /
    // fun(var, fun(var), var)
    // - var)

    // 								if ((SYNTAX[TMP_FUN_LONG
    // + 1]
    // !=
    // "+")
    // && 									(SYNTAX[TMP_FUN_LONG + 1]
    // !=
    // "-")
    // && (SYNTAX[TMP_FUN_LONG + 1] != "*")
    // && 									(SYNTAX[TMP_FUN_LONG + 1]
    // !=
    // "/")
    // && (SYNTAX[TMP_FUN_LONG + 1] != "،")
    // && 									(SYNTAX[TMP_FUN_LONG + 1]
    // !=
    // ",")
    // && (SYNTAX[TMP_FUN_LONG + 1] !=
    // ")"))
    // 								{
    // 									ErrorCode("نص غير معروف بعد نداء ' " + SYNTAX[p + 2]
    // +
    // "()' : ' " + SYNTAX[TMP_FUN_LONG + 1] + " ' ", o_tokens);
    // 								}
    // 							}
    // 							else if (TMP_FUN_LONG ==
    // SYNTAX_LONG)
    // 							{
    // 								// a = fun( a +
    // (b)) 								if (SYNTAX[TMP_FUN_LONG] != ")"
    // || SYNTAX[SYNTAX_LONG]
    // !=
    // ")") //
    // double check! 									ErrorCode("يجب إنهاء نداء الدالة ' "
    // + SYNTAX[p
    // + 2] + "()' بالإشارة ')' ", o_tokens);
    // 							}

    // 							break;
    // 						}

    // 						TMP_FUN_LONG++;
    // 					}

    // 					if (SYNTAX[TMP_FUN_LONG] != ")") //
    // Double
    // check! 						ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p
    // + 2]
    // +
    // "()' بالإشارة ')' ", o_tokens);

    // 					std::string TempToken[1024];
    // 					int TempTokenCount = 0;
    // 					for (int i = p + 4; i <= TMP_FUN_LONG;
    // i++)
    // 					{
    // 						if (SYNTAX[i] != "")
    // 						{
    // 							TempToken[TempTokenCount]
    // = SYNTAX[i];
    // TempTokenCount++;
    // 						}
    // 					}

    // 					// Check local fun Args : fun (a + c, 2
    // * (b
    // - 1)) 					CPP_CODE.append(
    // CHECK_CALL_FUN_ARG( false,
    // SYNTAX[p], SYNTAX[p + 2],
    // 0,
    // TheNamespace, TheFunction, TempToken, (TempTokenCount
    // - 1), o_tokens));

    // 					if(DEBUG)DEBUG_MESSAGE(")] \n\n",
    // o_tokens);
    // // DEBUG

    // 					// *** Generate Code ***
    // 					CPP_CODE.append(" ) " + CPP_END + " ");
    // 					// *** *** *** *** *** ***

    // 					// Point to last token of full func call
    // 					p = TMP_FUN_LONG;

    // 					continue;
    // 				}
    // 				//
    // ---------------------------------------------------------------------------------
    // 				else
    // 					ErrorCode("النافذة ' " + SYNTAX[p] + " ' لا
    // تحتوي على دالة محليه بإسم ' " + SYNTAX[p + 2] + " ' ", o_tokens);

    // 				// Exception!
    // 				continue;
    // 			}
    // 			/*
    // 			else if (SYNTAX[p + 3] == "=")
    // 			{
    // 				//
    // ---------------------------------------------------------------------------------
    // 				// abc = namespace : Value
    // 				//
    // ---------------------------------------------------------------------------------

    // 				// abc = namespace:Title
    // 				// abc = namespace:With
    // 				// abc = namespace:Heint
    // 				// abc = namespace:X
    // 				// abc = namespace:Y
    // 				// abc = namespace:Color
    // 				// abc = namespace:Background

    // 				// bool A = namespace:Is_Show

    // 				if (SYNTAX[p + 4] == "")
    // 						ErrorCode("يجب اضافه قيمة بعد
    // '='
    // ", o_tokens);

    // 				std::string VALUE_TYPE;
    // 				std::string VALUE_CPP_END;

    // 				if (SYNTAX[p + 2] == "عنوان")
    // 				{
    // 					// namespace:Title =

    // 					if(DEBUG)DEBUG_MESSAGE("[NS'" + SYNTAX[p] + "
    // ':VALUE'"
    // + SYNTAX[p
    // + 2] + "(SetWindowTitle)' = ", o_tokens); // DEBUG

    // 					// *** Generate Code ***
    // 					if (!IsInsideNamespace)
    // 						// namespace : Title =
    // 						CPP_GLOBAL_FUN.append(CG_WIN_MEMBER(SYNTAX[p],
    // "SetLabel
    // ((
    // ")); 					else
    // 						// namespace : Title =
    // 						cpp_AddScript(TheFunction,
    // CG_WIN_MEMBER(SYNTAX[p], "SetLabel
    // ((
    // "));
    // 					// *** *** *** *** *** ***
    // 					VALUE_CPP_END = " )); \n } \n";
    // 					// *** *** *** *** *** ***
    // 				}
    // 				else
    // 				{
    // 					ErrorCode("النافذة ' " + SYNTAX[p] + " ' لا تحتوي على
    // خاصيه باسم
    // '
    // "
    // + SYNTAX[p + 2] + " ' ", o_tokens);
    // 				}

    // 				// namespace : Option = ...

    // 				TempTokenCount = 0;
    // 				for (int p = 3; p <= o_tokens->TOTAL[o_tokens->Line]; p++) //
    // |
    // =
    // a
    // * b + 2 / (c) + 1 |
    // 				{
    // 					if (SYNTAX[p] != "")
    // 					{
    // 						TempToken[TempTokenCount] =
    // SYNTAX[p]; TempTokenCount++;
    // 					}
    // 				}

    // 				CheckForSyntax(VALUE_TYPE,	// OBJECTIF_TYPE
    // 								true, 		//
    // Accept
    // Using Reference to namespace:Controls true, 		// Accept Using
    // Reference to namespace:Function true,
    // // Accept Using Reference to Global Functions
    // true, 		// Accept Using Reference to Local Functions
    // true,
    // // Accept Using Reference to Global VAR
    // true, 		// Accept Using Reference to Local VAR
    // false,
    // // Accept Convertion from String To Int
    // 								true, 		// Accept
    // Convertion from Int To String
    // TempToken,
    // // SYNTAX[] std::string (TempTokenCount - 1),	// SYNTAX_LONG int
    // TheNamespace,		// TMP_WINDOW_NAME
    // TheFunction,
    // // TMP_FUNCTION_NAME
    // o_tokens);

    // 				// *** Generate Code ***
    // 				if (!IsInsideNamespace)
    // 					// namespace : Option =
    // 					CPP_GLOBAL_FUN.append(VALUE_CPP_END);
    // 				else
    // 					// namespace : Option =
    // 					cpp_AddScript(TheFunction,
    // VALUE_CPP_END);
    // 				// *** *** *** *** *** ***

    // 				continue;
    // 			}
    // 			*/
    // 			else if (SYNTAX[p + 3] == ":")
    // 			{
    // 				//
    // ---------------------------------------------------------------------------------
    // 				// abc = namespace : Control : Options
    // 				//
    // ---------------------------------------------------------------------------------

    // 				// abc = namespace:Control:Option()	|
    // namespace:ctr:disable()
    // 				// abc = namespace:Control:Option |
    // namespace:ctr:text

    // 				if (!CONTROL_IS_SET[std::make_pair(SYNTAX[p],
    // SYNTAX[p
    // + 2])]) 					ErrorCode("النافذة ' " +
    // SYNTAX[p] + " ' لا تحتوي على اداه باسم ' "
    // + SYNTAX[p + 2] + " ' ", o_tokens);

    // 				//if (SYNTAX[p + 5] == "")
    // 					//ErrorCode("يجب اضافه '=' أو '()' بعد ' " + SYNTAX[p] +
    // "
    // :
    // "
    // + SYNTAX[p + 2] + " : " + SYNTAX[p + 4] + " ' ", o_tokens);

    // 				//if (SYNTAX[p + 5] != "=" && SYNTAX[p + 5] !=
    // "(")
    // 					//ErrorCode("أمر غير معروف ' " + SYNTAX[p + 5]
    // +
    // "
    // '
    // ", o_tokens);

    // 				//if (SYNTAX[p + 6] == "")
    // 					//ErrorCode("يجب اضافه قيمة بعد ' " + SYNTAX[p] + " :
    // "
    // + SYNTAX[p
    // + 2] + " : " + SYNTAX[p + 4] + " " + SYNTAX[p + 6] + " ' ", o_tokens);

    // 				CTR_WIN = SYNTAX[p];
    // 				CTR_CONTROL = SYNTAX[p + 2];
    // 				CTR_OPTION = SYNTAX[p + 4];
    // 				//CTR_ARG = 6;
    // 				CTR_LAST_TOKEN_NUMBER = p + 4;
    // 			}
    // 			else
    // 			{
    // 				// Exception !
    // 				ErrorCode("يجب اضافه ':' أو '()' بعد ' " + SYNTAX[p] +
    // "
    // "
    // + SYNTAX[p
    // + 1] + " " + SYNTAX[p + 2] + " ' ", o_tokens);
    // 			}
    // 	}
    // 	else if (CONTROL_IS_SET[std::make_pair(TheNamespace, SYNTAX[p])])
    // 	{
    // 			//
    // ---------------------------------------------------------------------------------
    // 			// Control : Options
    // 			//
    // ---------------------------------------------------------------------------------

    // 			// abc = Control:Option()		| ctr:disable()
    // 			// abc = Control:Option			| ctr:text

    // 			if (SYNTAX[p + 1] == "")
    // 				ErrorCode("يجب اضافة ':' بعد ' " + SYNTAX[p] + "
    // '
    // ", o_tokens);

    // 			if (SYNTAX[p + 1] != ":")
    // 				ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " ',
    // يجب اضافة
    // ':' بعد ' " + SYNTAX[p] + " ' ", o_tokens);

    // 			if (SYNTAX[p + 2] == "")
    // 				ErrorCode("يجب اضافة خاصية بعد ' " + SYNTAX[p] +
    // "
    // :
    // '
    // ", o_tokens);

    // 			//if (SYNTAX[p + 3] == "")
    // 				//ErrorCode("يجب اضافة '=' أو '()' بعد ' " + SYNTAX[p] +
    // "
    // :
    // "
    // + SYNTAX[p + 2] + " ' ", o_tokens);

    // 			//if (SYNTAX[p + 3] != "=" && SYNTAX[p + 3] != "(")
    // 				//ErrorCode("أمر غير معروف ' " + SYNTAX[p + 5] +
    // "
    // '
    // ", o_tokens);

    // 			//if (SYNTAX[p + 4] == "")
    // 				//ErrorCode("يجب اضافة قيمة بعد ' " + SYNTAX[p] + " :
    // "
    // + SYNTAX[p
    // + 2] + " " + SYNTAX[p + 3] + " ' ", o_tokens);

    // 			CTR_WIN = TheNamespace;
    // 			CTR_CONTROL = SYNTAX[p];
    // 			CTR_OPTION = SYNTAX[p + 2];
    // 			//CTR_ARG = 4;
    // 			CTR_LAST_TOKEN_NUMBER = p + 2;
    // 	}
    // 	else
    // 	{
    // 			// Exception !
    // 			ErrorCode("أمر غير معروف ' " + SYNTAX[p] + " ', يجب
    // اضافة مجال", o_tokens);
    // 	}

    // 	//
    // ---------------------------------------------------------------------------------
    // 	// Check Control Options if valid
    // 	// For Control:Option AND namespace:Control:Option
    // 	//
    // ---------------------------------------------------------------------------------

    // 	if (CTR_OPTION == "نص") // abc = Control:نص
    // 	{
    // 		// GetValue not working Label, need
    // 		std::string GetValueFix = "GetValue";
    // 		if (CONTROL_TYPE[std::make_pair(CTR_WIN, CTR_CONTROL)] ==
    // "ملصق") 			GetValueFix = "GetLabelText";

    // 		CTR_OPTION_TYPE = "نص";

    // 		if (OBJECTIF_TYPE == "عدد")
    // 		{
    // 			if(DEBUG)DEBUG_MESSAGE("[NS'" + CTR_WIN + " ':CTR'" +
    // CTR_CONTROL
    // +
    // "
    // ':OPTION'" + CTR_OPTION + "(Text).ToInt()]' ", o_tokens); // DEBUG

    // 			// *** Generate Code ***
    // 			CPP_CODE.append(" (alifcore_StringToInt(OBJ_CTR_" +
    // ID[CTR_WIN]
    // +
    // "_"
    // + Control_ID[CTR_CONTROL] + "->" + GetValueFix + "())) ");
    // 			// *** *** *** *** *** ***
    // 		}
    // 		else if (OBJECTIF_TYPE == "نص")
    // 		{
    // 			if(DEBUG)DEBUG_MESSAGE("[NS'" + CTR_WIN + " ':CTR'" +
    // CTR_CONTROL
    // +
    // "
    // ':OPTION'" + CTR_OPTION + "(Text)]' ", o_tokens); // DEBUG

    // 			// *** Generate Code ***
    // 			CPP_CODE.append(" (OBJ_CTR_" + ID[CTR_WIN] + "_" +
    // Control_ID[CTR_CONTROL] + "->" + GetValueFix + "()) ");
    // 			// *** *** *** *** *** ***
    // 		}
    // 		else if (OBJECTIF_TYPE == "منطق")
    // 		{
    // 			ErrorCode("لا يمكن تحويل نص إلى منطق ", o_tokens);
    // 		}
    // 		else
    // 		{
    // 			ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
    // " ' ل ' " + CTR_CONTROL + " ' ", o_tokens);
    // 		}
    // 	}
    // 	else
    // 	{
    // 		ErrorCode("الأداة ' " + CTR_CONTROL + " ' لا تحتوي على خاصيه
    // باسم
    // '
    // "
    // + CTR_OPTION + " ' ", o_tokens);
    // 	}

    // 	// Point to last token of full namespace:ctrl:option
    // 	p = CTR_LAST_TOKEN_NUMBER;
    // }

    // -----------------------------------
    // ERROR
    // -----------------------------------

    else {
      // if(DEBUG)DEBUG_MESSAGE("\n \n *** \n TMP_NAMESPACE_OR_CLASS : " +
      // TMP_NAMESPACE_OR_CLASS + " \n", o_tokens);
      // if(DEBUG)DEBUG_MESSAGE("TmpFunction : " + TmpFunction + " \n *** \n",
      // o_tokens);

      // if (Control_ID[SYNTAX[p]] != "")
      // {
      // 	// show error description
      // 	// when, (no-win) CTR:OPTION
      // 	// in global area.

      // 	ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] + " '، على
      // العموم، إن كنت تقصد أداة، لا تنسى أن تحدد إسم النافذة قبل إسم الأداة ",
      // o_tokens);
      // }

      // أو
      if (SYNTAX[p] == "او" || SYNTAX[p] == "ٱو" || SYNTAX[p] == "آو" ||
          SYNTAX[p] == "والا" || SYNTAX[p] == "وإلا" || SYNTAX[p] == "وألا") {
        ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] +
                      " '، هل تقصد ' أو ' ؟ ",
                  o_tokens);
      } else {

        if (DEBUG)
          DEBUG_MESSAGE("\nTMP_NAMESPACE_OR_CLASS --> '" +
                            TMP_NAMESPACE_OR_CLASS + "'\n ",
                        o_tokens); // DEBUG
        if (DEBUG)
          DEBUG_MESSAGE("TmpFunction --> '" + TmpFunction + "'\n ",
                        o_tokens); // DEBUG
        if (DEBUG)
          DEBUG_MESSAGE("SYNTAX[p] --> '" + SYNTAX[p] + "'\n ",
                        o_tokens); // DEBUG

        ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] + " ' ", o_tokens);
      }
    }
  }

  if (ALIF_PARENTHESIS_STATUS > 0 && !IS_IF_SYNTAX)
    ErrorCode("غلق قوس مفقود ')' ", o_tokens);

  IS_IF_SYNTAX = false;

  return CPP_CODE;
}

// ====================================================

std::string CHECK_CALL_FUN_ARG(
    bool CALL_FUN_GLOBAL,
    std::string CALL_WIN_OR_CLASS, // win1/class1 { fun1(int a) } | win2 { fun2{
                                   // عدد b; fun1(b) } } ==> win1
    std::string CALL_FUN, // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
                          // } } ==> fun1
    int CALL_IS_CLASS, // 0 = non class, 1 constructor, 2 = الدالة member, ل
                       // Message when new obj
    std::string FROM_WIN_OR_CLASS, // win1 { fun1(int a) } | win2 { fun2{ عدد b;
                                   // fun1(b) } } ==> win2
    std::string FROM_FUN, // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
                          // } } ==> fun2
    std::string SYNTAX[1024], int SYNTAX_LONG, CLASS_TOKEN *o_tokens) {
  // SYNTAX[]			: 	1+1, 	b*2, 	("test" + s) 	)
  // G_FUN_ARG_TYPE 	: 	INT, 	INT, 	STRING

  int CURRENT_ARG_NUMBER = 0;

  int p = 0;
  std::string ARG[1024];
  int ARG_LONG = 1;

  ARG[0] = "=";

  int CALL_ARG_TOTAL = 0;

  std::string CPP_CODE;

  // if(DEBUG)DEBUG_MESSAGE("-[Function to call: " +
  // CONVERT_STRING_ARRAY_TO_STRING(SYNTAX, SYNTAX_LONG) + " ]-", o_tokens); //
  // DEBUG ErrorCode(" Yep1! ", o_tokens); if(DEBUG)DEBUG_MESSAGE("_---_|" +
  // IntToString(G_FUN_ARG_TOTAL[CALL_FUN]) + "\n" + IntToString(SYNTAX_LONG) +
  // "|_---_", o_tokens); // DEBUG if(DEBUG)DEBUG_MESSAGE("=__=|" + SYNTAX +
  // "|_---_", o_tokens); // DEBUG

  if (CALL_FUN_GLOBAL) {
    // Call Global Fun

    CALL_ARG_TOTAL = G_FUN_ARG_TOTAL[CALL_FUN];

    // check args
    if (CALL_ARG_TOTAL > 0 && SYNTAX_LONG < 1)
      ErrorCode("الدالة العامة ' " + CALL_FUN + "()' تأخد " +
                    IntToString(CALL_ARG_TOTAL) + " خاصية",
                o_tokens);

    // الدالة العامة () without args
    if (CALL_ARG_TOTAL == 0 && SYNTAX_LONG == 0)
      return "";
  } else {
    // Call Local Fun

    CALL_ARG_TOTAL =
        L_FUN_ARG_TOTAL[std::make_pair(CALL_WIN_OR_CLASS, CALL_FUN)];

    // if(DEBUG)DEBUG_MESSAGE("[CHEEECK -> NS: "+ CALL_WIN_OR_CLASS +"|Fun: " +
    // CALL_FUN + "|Looocal: " +
    // IntToString(L_FUN_ARG_TOTAL[std::make_pair(CALL_WIN_OR_CLASS, CALL_FUN)])
    // + "|Glooobal: " + IntToString(G_FUN_ARG_TOTAL[CALL_FUN]) + "]",
    // o_tokens);
    // // DEBUG

    // check args
    if (CALL_ARG_TOTAL > 0 && SYNTAX_LONG < 1)
      ErrorCode("الدالة المحلية ' " + CALL_FUN + "()' تأخد " +
                    IntToString(CALL_ARG_TOTAL) + " خاصية",
                o_tokens);

    // الدالة المحلية () without args
    if (CALL_ARG_TOTAL == 0 && SYNTAX_LONG == 0)
      return "";
  }

  // الدالة (int a, عدد b) with args
  while (p <= SYNTAX_LONG) // using while is bcs need 'if p < G_FUN_ARG_TOTAL'
                           // بعد while finish
  {
    // if(DEBUG)DEBUG_MESSAGE("\n --[" + SYNTAX[p] + "|" +
    // G_FUN_ARG_TYPE[std::make_pair(CALL_FUN, p)] + "]-- \n", o_tokens); //
    // DEBUG

    if (SYNTAX[p] != "," && SYNTAX[p] != "،" && p < SYNTAX_LONG) {
      if (DEBUG)
        DEBUG_MESSAGE("-[ New ARG: " + SYNTAX[p] + " ]-", o_tokens); // DEBUG

      ARG[ARG_LONG] = SYNTAX[p];
      ARG_LONG++;
    } else {
      // if(DEBUG)DEBUG_MESSAGE("-[" +
      // CONVERT_STRING_ARRAY_TO_STRING(ARG,ARG_LONG) + "]-", o_tokens); //
      // DEBUG ErrorCode("|G_FUN_ARG_TYPE TmpFunction: " + TmpFunction + " p : "
      // + IntToString(p) + "|", o_tokens );

      if (ARG_LONG < 2) // الدالة ( , ...) ['0'] =, [1] 'user arg', [2] null ل
                        // incrumentation
        ErrorCode("إحدى الخصائص فارغة", o_tokens);

      if (CALL_FUN_GLOBAL) {
        // نداء a الدالة العامة ( ... check ARG n ...)

        // الدالة (a,a,a) ل (a,a)
        if ((CURRENT_ARG_NUMBER + 1) > CALL_ARG_TOTAL)
          ErrorCode("خصائص أكثر من الازم، الدالة العامة ' " + CALL_FUN +
                        "()' تأخد فقط " + IntToString(CALL_ARG_TOTAL) +
                        " خاصية ",
                    o_tokens);

        // Current خاصية OBJECTIF_TYPE
        CPP_CODE.append(CheckForSyntax(
            G_FUN_ARG_TYPE[std::make_pair(CALL_FUN, CURRENT_ARG_NUMBER)],
            true,           // Accept Using Reference إلى namespace:Controls
            true,           // Accept Using Reference إلى namespace:Function
            true,           // Accept Using Reference إلى Global Functions
            true,           // Accept Using Reference إلى Local Functions
            true,           // Accept Using Reference إلى Global VAR
            true,           // Accept Using Reference إلى Local VAR
            false,          // Accept Convertion من نص إلى Int
            true,           // Accept Convertion من عدد إلى String
            ARG,            // SYNTAX[] std::string
            (ARG_LONG - 1), // SYNTAX_LONG int
            FROM_WIN_OR_CLASS, // TMP_WINDOW_NAME
            FROM_FUN,          // TMP_FUNCTION_NAME
            o_tokens));
      } else {
        // نداء a الدالة المحلية ( ... check ARG n ...)

        // الدالة (a,a,a) ل (a,a)
        if ((CURRENT_ARG_NUMBER + 1) > CALL_ARG_TOTAL) {
          if (CALL_IS_CLASS == 1) // constructor
            ErrorCode("خصائص أكثر من ألازم، دالة بناء الصنف ' " +
                          CALL_WIN_OR_CLASS + " ' تأخد فقط " +
                          IntToString(CALL_ARG_TOTAL) + " خاصية ",
                      o_tokens);
          else if (CALL_IS_CLASS == 2) // الدالة member
            ErrorCode("خصائص أكثر من ألازم، الدالة ' " + CALL_FUN +
                          " ' المنتمية للصنف ' " + CALL_WIN_OR_CLASS +
                          " ' تأخد فقط " + IntToString(CALL_ARG_TOTAL) +
                          " خاصية ",
                      o_tokens);
          else
            ErrorCode("خصائص أكثر من ألازم، الدالة ' " + CALL_FUN +
                          " ' تأخد فقط " + IntToString(CALL_ARG_TOTAL) +
                          " خاصية ",
                      o_tokens);
        }

        // if(DEBUG)DEBUG_MESSAGE("-[ " + CONVERT_STRING_ARRAY_TO_STRING(ARG,
        // ARG_LONG) + " ]-", o_tokens); // DEBUG ErrorCode(" Yep! ", o_tokens);

        // Current خاصية OBJECTIF_TYPE
        CPP_CODE.append(CheckForSyntax(
            L_FUN_ARG_TYPE[std::make_pair(CALL_WIN_OR_CLASS + CALL_FUN,
                                          CURRENT_ARG_NUMBER)],
            true,           // Accept Using Reference إلى namespace:Controls
            true,           // Accept Using Reference إلى namespace:Function
            true,           // Accept Using Reference إلى Global Functions
            true,           // Accept Using Reference إلى Local Functions
            true,           // Accept Using Reference إلى Global VAR
            true,           // Accept Using Reference إلى Local VAR
            false,          // Accept Convertion من نص إلى Int
            true,           // Accept Convertion من عدد إلى String
            ARG,            // SYNTAX[] std::string
            (ARG_LONG - 1), // SYNTAX_LONG int
            FROM_WIN_OR_CLASS, // TMP_WINDOW_NAME
            FROM_FUN,          // TMP_FUNCTION_NAME
            o_tokens));
      }

      ARG[0] = "=";
      ARG_LONG = 1; // Point إلى next arg writed بالإشارة user
      CURRENT_ARG_NUMBER++; // Point إلى next يدعى الدالة arg

      if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL) {
        if (DEBUG)
          DEBUG_MESSAGE(", ", o_tokens); // DEBUG
        CPP_CODE.append(" , ");
      }
    }

    p++;
  }

  if (CALL_FUN_GLOBAL) {
    // الدالة (a,a,a) ل (a,a)
    if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL)
      ErrorCode("خصائص قليلة, الدالة العامة ' " + CALL_FUN + "()' تأخد " +
                    IntToString(CALL_ARG_TOTAL) + " خاصية، ثم إنشاء فقط " +
                    IntToString(CURRENT_ARG_NUMBER) + " خاصية ",
                o_tokens);
  } else {
    // الدالة (a,a,a) ل (a,a)
    if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL) {
      if (CALL_IS_CLASS == 1) // constructor
        ErrorCode("خصائص قليلة, دالة بناء الصنف ' " + CALL_WIN_OR_CLASS +
                      "()' تأخد " + IntToString(CALL_ARG_TOTAL) +
                      " خاصية، ثم إنشاء فقط " +
                      IntToString(CURRENT_ARG_NUMBER) + " خاصية ",
                  o_tokens);
      else if (CALL_IS_CLASS == 2) // الدالة member
        ErrorCode("خصائص قليلة, الدالة منتمي ' " + CALL_FUN +
                      " ' داخل الصنف ' " + CALL_WIN_OR_CLASS + "()' تأخد " +
                      IntToString(CALL_ARG_TOTAL) + " خاصية، ثم إنشاء فقط " +
                      IntToString(CURRENT_ARG_NUMBER) + " خاصية ",
                  o_tokens);
      else
        ErrorCode("خصائص قليلة, الدالة المحلية ' " + CALL_FUN + "()' تأخد " +
                      IntToString(CALL_ARG_TOTAL) + " خاصية، ثم إنشاء فقط " +
                      IntToString(CURRENT_ARG_NUMBER) + " خاصية ",
                  o_tokens);
    }
  }

  return CPP_CODE;
}

// ====================================================

void FINAL_CURRENT_FILE_CODE_CHECKING(CLASS_TOKEN *o_tokens) {
  // if(DEBUG)DEBUG_MESSAGE("FINAL_CURRENT_FILE_CODE_CHECKING()... \n",
  // o_tokens); // DEBUG

  // Function()
  if (IsInsideFunction)
    ErrorCode("يجب إغلاق الدالة : " + TheFunction, o_tokens);

  // Namespace()
  if (IsInsideNamespace && TheNamespace != "")
    ErrorCode("يجب إغلاق النافذة : " + TheNamespace, o_tokens);

  // Class()
  if (IsInsideClass)
    ErrorCode("يجب إغلاق الصنف : " + TheClass, o_tokens);

  // IF
  if (ALIF_IF_STATUS > 0)
    ErrorCode("مازال هناك " + IntToString(ALIF_IF_STATUS) +
                  " شروط مازالت مفتوحه",
              o_tokens);

  // WHILE
  if (ALIF_LOOP_STATUS > 0)
    ErrorCode("مازال هناك " + IntToString(ALIF_LOOP_STATUS) +
                  " كلما مازالت مفتوحه",
              o_tokens);

  // #Alif
  if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
    ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى",
              o_tokens);
}

// ====================================================

void FINAL_APPLICATION_CODE_CHECKING(CLASS_TOKEN *o_tokens, bool FIRST_FILE) {
  if (FIRST_FILE) {
    if (DEBUG)
      DEBUG_MESSAGE("FINAL_APPLICATION_CODE_CHECKING()... \n",
                    o_tokens); // DEBUG

    // Main Namespace()
    // if (!MAIN_WIN_IS_SET && !CONTROL_WIN_IS_SET["رئيسية"])
    // ErrorCode("النافذة الرئيسية غير موجودة", o_tokens);

    // TODO: IDE dont catch the error log.

    // if (APP_TYPE == "PC_GUI")
    // {
    // 	// if (!MAIN_WIN_IS_SET)
    // 	// 	ErrorCode("هذا التطبيق ذي واجهة رسومية، لكن النافذة الرئيسية غير
    // محددة، هل نسيت ' مجال رئيسية ' ؟", o_tokens);
    // }

    // #Alif
    if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
      ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى",
                o_tokens);

    // Main
    if (!script.main.is_set)
      ErrorCode(
          "يجب إنشاء الدالة الرئيسية، لأنها تعتبر المدخل الرئيسي لهذا التطبيق.",
          o_tokens);
  }
}

// Setup ********************************************************

#ifdef _WIN32

std::string GET_WORKING_PATH_WIN32() {
  char working_directory[MAX_PATH + 1];
  GetCurrentDirectoryA(sizeof(working_directory),
                       working_directory); // **** win32 specific ****
  return working_directory;
}

#elif __APPLE__

#include <unistd.h>
#define GetCurrentDir getcwd

#else

#include <unistd.h>
#define GetCurrentDir getcwd

#endif

std::string GET_WORKING_PATH() {
  // https://msdn.microsoft.com/en-us/library/sf98bd4y.aspx

  char *buffer;

  if ((buffer = GetCurrentDir(NULL, 0)) == NULL) {
    return NULL;
  } else {
    // printf( "%s \nLength: %d\n", buffer, strnlen(buffer) );
    // free(buffer);

    return buffer;
  }
}

std::string GET_PATH_WITHOUT_FILE(std::string PATH) {
  // In: /abc/def/test.xx
  // Out: /abc/def/

#ifdef _WIN32
  int POSITION = PATH.find_last_of("\\/");
#else
  int POSITION = PATH.find_last_of("//");
#endif

  if (POSITION < 1) // To avoid ABC/DEF/ or Relative Path
  {
    return "";
  } else {
#ifdef _WIN32
    return PATH.substr(0, PATH.find_last_of("\\/"));
#else
    return PATH.substr(0, PATH.find_last_of("//"));
#endif
  }
}

bool is_path(std::string PATH_OR_FILE) {
  // 'myfile.x'               --> false.
  // '/abc/test/myfile.x'     --> true.

#ifdef _WIN32
  int POSITION = PATH_OR_FILE.find_last_of("\\/");
#else
  int POSITION = PATH_OR_FILE.find_last_of("//");
#endif

  if (POSITION < 1)
    return false;
  else
    return true;
}

std::string GET_PATH_WITHOUT_LAST_SEPARATION(std::string PATH) {
  // In : '/abc/test/folder/'
  // Out : '/abc/test/folder'

  std::string Last_Char = PATH.substr(PATH.length() - 1); // PATH.back()

#ifdef _WIN32
  if (Last_Char == "\\")
    return PATH.substr(0, PATH.length() - 1);
  else
    return PATH;
#else
  if (Last_Char == "/")
    return PATH.substr(0, PATH.length() - 1);
  else
    return PATH;
#endif
}

int Temp_File_Rand() {
  srand(time(NULL));
  return rand() % 10 + 1000;
}

/*
bool CHECK_FOLDER_EXISTE(std::string PATH)
{
        #ifdef _WIN32
                //
        #else
                struct stat info;

                if( stat( PATH, &info ) != 0 )
                        return false; // cannot access
                else if( info.st_mode & S_IFDIR )
                        return true; // is a directory
                else
                        return false; // is no directory
        #endif
}
*/

bool CHECK_SETUP() // std::string ARGV_0, std::string OUTPUT)
{
  if (DEBUG)
    RANDOM = "0000";
  else
    RANDOM = IntToString(Temp_File_Rand());

#ifdef _WIN32

  // -------------------
  // Get Absolute Path
  // -------------------

  TCHAR WIN32_PATH_BUFFER[MAX_PATH];

  if (!GetModuleFileNameW(NULL, WIN32_PATH_BUFFER, MAX_PATH)) {
    ALIF_ERROR(
        "Sorry, Can't get Absolut Path from Win API \n"
        "Please add the path where 'alif.exe' existe, for example : \n\n"
        "alif [source.alif] [output.exe] [logfile.txt] -workpath=C:\\Alif ");
  }

  wstring WIN32_PATH_BUFFER_W(&WIN32_PATH_BUFFER[0]);

  // PATH_ABSOLUTE = std::string(WIN32_PATH_BUFFER_W.begin(),
  // WIN32_PATH_BUFFER_W.end());
  PATH_ABSOLUTE = ws_to_s(WIN32_PATH_BUFFER_W);

  PATH_ABSOLUTE = PATH_ABSOLUTE.substr(0, PATH_ABSOLUTE.find_last_of("\\/"));

  // -------------------
  // Get Working Path
  // -------------------

  if (PATH_WORKING.empty()) {
    PATH_WORKING = GET_WORKING_PATH_WIN32();

    if (PATH_WORKING == PATH_ABSOLUTE) {
      // Tray a seconde solution !
      PATH_WORKING = GET_PATH_WITHOUT_FILE(PATH_FULL_ALIF);
    }
  }

  // -------------------
  // Get Temp Path
  // -------------------

  // C:\Users\Me\AppData\Local\Temp

  ostringstream TEMP_BUFFER;
  TEMP_BUFFER << getenv("Temp");
  PATH_TEMP = TEMP_BUFFER.str();

  // -------------------
  // Other Path
  // -------------------

  cc_path_full = PATH_ABSOLUTE + "\\gcc\\bin\\x86_64-w64-mingw32-g++.exe";
  PATH_FULL_CPP = PATH_TEMP + "\\alifcompiler_" + RANDOM + ".cpp";
  PATH_FULL_OBJ = PATH_TEMP + "\\alifcompiler_" + RANDOM + ".o";
  PATH_FULL_RC = PATH_TEMP + "\\alifcompiler_" + RANDOM + ".rc";
  PATH_FULL_ICO = PATH_ABSOLUTE + "\\aliflib\\alif.ico";

  // PATH_FULL_LIB_SETTING = PATH_ABSOLUTE + "\\aliflib\\aliflib.inf";

#elif __APPLE__

  // Mac OS X

  // alif_1.0-1.pkg
  //		/usr/local/bin/alif
  //		/Library/Application
  // Support/Aliflang/Alif_Compiler/aliflib/aliflib.inf
  // /Library/Application Support/Aliflang/Alif_Compiler/linux_alif_version.inf
  // /Library/Application
  // Support/Aliflang/Alif_Compiler/Alif_Arabic_Programming_Language
  //		/Library/Application Support/Aliflang/Alif_Compiler/alif.icns
  //		/Library/Application Support/Aliflang/Alif_Compiler/Info.plist

  // aliflibwx_3.1-1.pkg
  //		/Library/Application
  // Support/Aliflang/Alif_Lib_WX/include/[wx][boost][utf8]
  // /Library/Application Support/Aliflang/Alif_Lib_WX/lib/[.a]
  // /Library/Application
  // Support/Aliflang/Alif_Lib_WX/linux_aliflibwx_version.inf

  // alifstudio_1.0-1.pkg
  //		/Applications/Alif Studio.app
  //		/Library/Application
  // Support/Aliflang/Alif_Studio/linux_alifstudio_version.inf
  //		/Library/Application Support/Aliflang/Alif_Studio/alifstudio.png
  //		/Library/Application Support/Aliflang/Alif_Studio/mupdate

  // -------------------
  // Get Absolute Path
  // -------------------

  PATH_ABSOLUTE = XSTR(INSTALL_PREFIX) "/bin";

  // -------------------
  // Get Working Path
  // -------------------

  if (PATH_WORKING.empty()) {
    PATH_WORKING = GET_WORKING_PATH();

    if (PATH_WORKING == PATH_ABSOLUTE) {
      // Tray a seconde solution !
      PATH_WORKING = GET_PATH_WITHOUT_FILE(PATH_FULL_ALIF);
    }
  }

  // -------------------
  // Get Temp Path
  // -------------------

  PATH_TEMP = getenv("TMPDIR");

  // -------------------
  // Other Path
  // -------------------

  PATH_FULL_BIN_TMP = PATH_TEMP + "/alifcompiler_bin_" + RANDOM;

  cc_path_full = "clang++";
  PATH_FULL_CPP = PATH_TEMP + "/alifcompiler_" + RANDOM + ".cpp";
  PATH_FULL_OBJ = PATH_TEMP + "/alifcompiler_" + RANDOM + ".o";
  PATH_FULL_RC = "";
  PATH_FULL_ICO =
      "/Library/Application Support/Aliflang/Alif_Compiler/alif.icns";

  PATH_FULL_LIB_SETTING =
      "/Library/Application Support/Aliflang/Alif_Compiler/aliflib/aliflib.inf";

  // PATH_FULL_PLIST = "/Library/Application
  // Support/Aliflang/Alif_Compiler/Info.plist";

#else

  // Linux

  // aliflang_1.0-1.deb
  //		/usr/local/bin/alif.bin
  //		/usr/local/lib/aliflib/aliflib.inf
  //		/usr/local/share/aliflang/copyright
  //		/usr/local/share/aliflang/linux_alif_version.inf
  //		/usr/local/share/aliflang/Alif_Arabic_Programming_Language

  // aliflibwx_3.1-1.deb
  //		/usr/local/include/aliflibwx/[wx][boost][utf8]
  //		/usr/local/lib/aliflibwx/[.a]
  //		/usr/local/share/aliflibwx/linux_aliflibwx_version.inf
  //		/usr/share/doc/aliflibwx/copyright (txt)

  // alifstudio_1.0-1.deb
  //		/usr/local/bin/alifstudio.bin
  //		/usr/local/bin/mupdate.bin
  //		/usr/local/share/alifstudio/copyright
  //		/usr/local/share/alifstudio/linux_alifstudio_version.inf
  //		/usr/share/alifstudio/alifstudio.ico
  //		/usr/share/alifstudio/alifstudio.png
  //		/usr/share/applications/'Alif Studio'

  // -------------------
  // Get Absolute Path
  // -------------------

  PATH_ABSOLUTE = XSTR(INSTALL_PREFIX) "/bin";

  // -------------------
  // Get Working Path
  // -------------------

  if (PATH_WORKING.empty()) {
    PATH_WORKING = GET_WORKING_PATH();

    if (PATH_WORKING == PATH_ABSOLUTE) {
      // Tray a seconde solution !
      PATH_WORKING = GET_PATH_WITHOUT_FILE(PATH_FULL_ALIF);
    }
  }

  // -------------------
  // Get Temp Path
  // -------------------
  using namespace boost::filesystem;
  path path = temp_directory_path() / unique_path();
  create_directories(path);
  PATH_TEMP = path.string();

  // -------------------
  // Other Path
  // -------------------

  cc_path_full = "g++";
  PATH_FULL_CPP = PATH_TEMP + "/alifcompiler_" + RANDOM + ".cpp";
  PATH_FULL_OBJ = PATH_TEMP + "/alifcompiler_" + RANDOM + ".o";
  PATH_FULL_RC = "";
  PATH_FULL_ICO = "";

  // PATH_FULL_LIB_SETTING = "/usr/local/lib/aliflib/aliflib.inf";

#endif

  // TODO: check installation..
  // trim all var..
  // check gcc installation.. return false;
  // tray create file to check for errors..
  // if mac, be sure PATH_FULL_BIN end with ".app"

  if (PATH_FULL_BIN == PATH_FULL_LOG) {
    ALIF_ERROR("ERROR: The binary file and the log are the same.");
  }

  if (PATH_WORKING.empty()) {
    ALIF_ERROR("ERROR: Can't get Working Path. \n");
    return false;
  }

  // Fix GNU LD Bug.
  if (PATH_FULL_BIN.find("Windows") != std::string::npos) {
    ALIF_ERROR("ERROR: Alif can't run from a path contain the word 'Windows', "
               "do a GNU LD Bug. \nPath: " +
               PATH_FULL_BIN +
               "\nPlease rename the folder 'Windows' to samthing else and tray "
               "again. \n");
    return false;
  }

  /*
  cout << "PATH_ABSOLUTE		: " << PATH_ABSOLUTE << endl;
  cout << "PATH_WORKING		: " << PATH_WORKING << endl << endl;
  cout << "PATH_FULL_ALIF		: " << PATH_FULL_ALIF << endl;
  cout << "PATH_FULL_BIN		: " << PATH_FULL_BIN << endl;
  cout << "PATH_FULL_LOG		: " << PATH_FULL_LOG << endl << endl;
  cout << "cc_path_full		: " << cc_path_full << endl;
  cout << "PATH_FULL_CPP		: " << PATH_FULL_CPP << endl;
  cout << "PATH_FULL_OBJ		: " << PATH_FULL_OBJ << endl;
  cout << "PATH_FULL_RC		: " << PATH_FULL_RC << endl;
  cout << "PATH_FULL_ICO		: " << PATH_FULL_ICO << endl << endl;
  #if  __APPLE__
          cout << "PATH_FULL_BIN_TMP	: " << PATH_FULL_BIN_TMP << endl;
          cout << "PATH_FULL_PLIST		: " << PATH_FULL_PLIST << endl;
  #endif
  exit(EXIT_FAILURE);
  */

  return true;
}

// Generator ****************************************************

// Alif 3
std::string code_core;
std::string code_stack;
std::string code;
std::string code_entry_point;

enum code_t : unsigned short { STACK, CODE, ENTRY_POINT };

void code_add(code_t type, std::string c) {

  if (type == STACK)
    code_stack.append(c);
  if (type == CODE)
    code.append(c);
  if (type == ENTRY_POINT)
    code_entry_point.append(c);
}

// - - - - - - - - - - - - - - - - - -

// General (Old)
std::string CPP_ID_DECLARATION;
std::string CPP_GLOBAL;
std::string CPP_OBJ_DECLARATION;
// std::string CPP_FUN;
std::string CPP_GLOBAL_FUN;
std::string CPP_CLASS;

// Namespace
std::map<std::pair<std::string, std::string>, std::string>
    CPP_WINDOW; // [window][option] = value

// Platformes

// PC Console
std::string CODE_GENERATOR_GET_PC_CONSOLE() { return ""; }

// PC GUI

// ----------------------------------
// C++ Add Code to the current Function
// ----------------------------------

/*
void Script(std::string Code){

        // CPP_ID_DECLARATION
        // CPP_GLOBAL
        // CPP_OBJ_DECLARATION
        // Windows const..
        // Event..
        // 		CPP_CLASS
        // 		CPP_GLOBAL_FUN
        // CPP_GLOBAL_FUN

        if (IsInsideClass){

                // Local Class Function
                CPP_CLASS.append(NEW_CODE);
        }
        else
        {
                // Local Namespace Function
                // Local Namespace Main Function

                if (FUN != "رئيسية")
                {
                        // Normal Function
                        CPP_GLOBAL_FUN.append(NEW_CODE);
                }
                else
                {
                        // Main Function
                        CBUFER = CPP_WINDOW[std::make_pair(TheNamespace,
"LOAD")]; CPP_WINDOW[std::make_pair(TheNamespace, "LOAD")] = CBUFER + NEW_CODE;
                }
        }
}
*/

void cpp_AddScript(std::string FUN, std::string NEW_CODE) {

  // code.append(NEW_CODE);

  // if (IsInsideClass)
  // 	CPP_CLASS.append(NEW_CODE);
  // else
  // 	CPP_GLOBAL_FUN.append(NEW_CODE);

  if (IsInsideClass) {
    // Local Class Function
    CPP_CLASS.append(NEW_CODE);
  } else {
    // Local Namespace Function
    // Local Namespace Main Function

    if (FUN != "رئيسية") {
      // Normal Function
      CPP_GLOBAL_FUN.append(NEW_CODE);
    } else {
      // Main Function
      // CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "LOAD")];
      // CPP_WINDOW[std::make_pair(TheNamespace, "LOAD")] = CBUFER + NEW_CODE;

      code_entry_point.append(NEW_CODE);
    }
  }
}

// ----------------------------------
// WinX Load()
// ----------------------------------

std::string CG_WINX_LOAD() {
  std::string CODE;
  std::string BUFFER;

  for (int i = 1; i <= Namespace_Total; i++) {
    BUFFER = R"( 

	// ========================================================================
	// ------------------------------------------------
	// Namespace X )" +
             ID[Namespace_Total_Names[i]] + R"( Load()
	// ------------------------------------------------

	void WINDOW_LOAD_)" +
             ID[Namespace_Total_Names[i]] + R"(()
	{
		)" +
             CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "LOAD")] + R"(
	}
	// ========================================================================

			)";

    CODE.append(BUFFER);
  }

  return CODE;
}

// ----------------------------------
// Application Destroy()
// ----------------------------------

std::string CG_APP_DESTROY() {
  std::string CODE;
  std::string BUFFER;

  BUFFER = R"( 
		// ------------------------------------------------
		// Application Destroy
		// ------------------------------------------------
		)";

  CODE.append(BUFFER);

  for (int i = 1; i <= Namespace_Total; i++) {
    BUFFER = R"( 
			// Namespace X )" +
             ID[Namespace_Total_Names[i]] + R"( Destroy
			// ---------
			//#ifdef __APPLE__
				//if (WINDOW_IS_CONSTRUCTION_)" +
             ID[Namespace_Total_Names[i]] + R"() OBJ_CLASS_WINDOW_)" +
             ID[Namespace_Total_Names[i]] + R"(->EndModal(0);
			//#else
				if (WINDOW_IS_CONSTRUCTION_)" +
             ID[Namespace_Total_Names[i]] + R"() OBJ_CLASS_WINDOW_)" +
             ID[Namespace_Total_Names[i]] + R"(->Destroy();
			//#endif
			)";

    CODE.append(BUFFER);
  }

  BUFFER =
      R"( 
		// ---------
		// Win Main Destroy
		// ---------
		//#ifdef __APPLE__
			//if (WINDOW_IS_CONSTRUCTION_)" +
      ID["رئيسية"] + R"() OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(->EndModal(0);
		//#else
			if (WINDOW_IS_CONSTRUCTION_)" +
      ID["رئيسية"] + R"() OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(->Destroy();
		//#endif
		// ------------------------------------------------
		)";

  CODE.append(BUFFER);

  return CODE;
}

// ----------------------------------
// Namespace Show ()
// ----------------------------------

std::string CG_WIN_SHOW(std::string WIN) {
  std::string CODE = R"(
		// -------------------------------
		// Win Show ()" +
                     WIN + R"()
		// -------------------------------
		if (!WINDOW_IS_CONSTRUCTION_)" +
                     ID[WIN] + R"()
		{
			OBJ_CLASS_WINDOW_)" +
                     ID[WIN] + R"( = new NS_)" + ID[WIN] + R"(();
			OBJ_CLASS_WINDOW_)" +
                     ID[WIN] + R"(->SetLayoutDirection(wxLayout_RightToLeft);
			WINDOW_IS_CONSTRUCTION_)" +
                     ID[WIN] + R"( = true;
		}
		OBJ_CLASS_WINDOW_)" +
                     ID[WIN] + R"(->Show(true);
		OBJ_CLASS_WINDOW_)" +
                     ID[WIN] + R"(->SetFocus();
		WINDOW_IS_SHOW_)" +
                     ID[WIN] + R"( = true;
		WINDOW_LOAD_)" +
                     ID[WIN] + R"(();
		// -------------------------------		
					)";

  return CODE;
}

// ----------------------------------
// Namespace Center ()
// ----------------------------------

std::string CG_WIN_CENTER(std::string WIN) {
  std::string CODE = R"(
		// -------------------------------
		// Win Center ()" +
                     WIN + R"()
		// -------------------------------
		if (WINDOW_IS_CONSTRUCTION_)" +
                     ID[WIN] + R"()
		{
			OBJ_CLASS_WINDOW_)" +
                     ID[WIN] + R"(->Center();
		}
		// -------------------------------		
		)";

  return CODE;
}

// ----------------------------------
// Namespace Hide ()
// ----------------------------------

std::string CG_WIN_HIDE(std::string WIN) {
  std::string CODE = R"(
		// -----------------------------------------------
		// Win Hide
		// -----------------------------------------------
		if (WINDOW_IS_SHOW_)" +
                     ID[WIN] + R"()
		{
			OBJ_CLASS_WINDOW_)" +
                     ID[WIN] + R"(->Show(false);
			WINDOW_IS_SHOW_)" +
                     ID[WIN] + R"( = false;
		}
		// -----------------------------------------------		
					)";

  return CODE;
}

// ----------------------------------
// Namespace Destroy ()
// ----------------------------------

std::string CG_WIN_DESTROY(std::string WIN) {
  std::string CODE =
      R"(
		// -----------------------------------------------
		// Win Destroy
		// -----------------------------------------------
		if (WINDOW_IS_CONSTRUCTION_)" +
      ID[WIN] + R"()
		{
			//#ifdef __APPLE__
				//OBJ_CLASS_WINDOW_)" +
      ID[WIN] +
      R"(->EndModal(0); // Hum, maybe this is only for wxDialog, and not wxFrame !
			//#else
				OBJ_CLASS_WINDOW_)" +
      ID[WIN] + R"(->Destroy();
			//#endif
			WINDOW_IS_CONSTRUCTION_)" +
      ID[WIN] + R"( = false;
		}
		// -----------------------------------------------	
		)";

  return CODE;
}

// ----------------------------------
// namespace : members
// ----------------------------------

// std::string CG_WIN_MEMBER(std::string WIN, std::string MEMBER_CODE)
// {
// 	// Used by Operator:xx:yy
// 	std::string CODE = R"(
// 	// -----------------------------------------------
// 	// Win Members
// 	// -----------------------------------------------
// 	if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"()
// 	{
// 		OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->)" + MEMBER_CODE + R"( )";
// 	return CODE;
// }

// ----------------------------------
// namespace : members
// ----------------------------------
/*

// We will need this, if we set a way to delete a control by alif code.

std::string CG_CONTROL_MEMBER(std::string WIN, std::string CONTROL, std::string
MEMBER_CODE)
{
        std::string CODE = R"(
        // -----------------------------------------------
        // Control Members
        // -----------------------------------------------
        //if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"()
        //{
                OBJ_)" + ID[WIN] + R"(_)" + ID[CONTROL] + R"(->)" + MEMBER_CODE
+ R"( )";

        return CODE;
}
*/

// ----------------------------------
// C++ CODE INITIALIZATION
// ----------------------------------

void CG_INITIALIZATION() {
  //رئيسية

  SET_C_NAME("رئيسية");
  SET_CONTROL_C_NAME("AlifUIWeb");

  // x = افصول
  // y = ارتوب
  CPP_WINDOW[std::make_pair("رئيسية", "نص")] = " (رئيسية) مجال بدون عنوان";
#ifdef __APPLE__
  CPP_WINDOW[std::make_pair("رئيسية", "افصول")] = "10";
  CPP_WINDOW[std::make_pair("رئيسية", "ارتوب")] =
      "25"; // Because the macOS task-bar is on top screen.
#else
  CPP_WINDOW[std::make_pair("رئيسية", "افصول")] = "0";
  CPP_WINDOW[std::make_pair("رئيسية", "ارتوب")] = "0";
#endif
  CPP_WINDOW[std::make_pair("رئيسية", "عرض")] = "400";
  CPP_WINDOW[std::make_pair("رئيسية", "ارتفاع")] = "400";

  CPP_WINDOW[std::make_pair("رئيسية", "شكل")] =
      "wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | "
      "wxCLIP_CHILDREN";
  // wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
  // wxMAXIMIZE_BOX

  Namespace_Total = 0;
  Namespace_Total_Names[0] = "رئيسية";
  L_FUN_TYPE[std::make_pair("رئيسية", "رئيسية")] = "عادم";
}

// ----------------------------------
// C++ CODE INITIALIZATION
// ----------------------------------

std::string CG_WINX_CODE() {
  std::string CODE;
  std::string BUFFER;

  for (int i = 1; i <= Namespace_Total; i++) {
    BUFFER =
        R"( 
	// ========================================================================
	// Namespace X )" +
        ID[Namespace_Total_Names[i]] + R"( 
	// ========================================================================

	// --------------------
	// Namespace )" +
        ID[Namespace_Total_Names[i]] + R"( Definition
	// --------------------

	class NS_)" +
        ID[Namespace_Total_Names[i]] + R"( : public wxFrame
	{
		public:
			void OnClose(wxCloseEvent& event);
			NS_)" +
        ID[Namespace_Total_Names[i]] + R"( ();
			virtual ~NS_)" +
        ID[Namespace_Total_Names[i]] + R"(();
			
			// --------------------
			// Namespace )" +
        ID[Namespace_Total_Names[i]] + R"( Functions Declaration
			// --------------------
			
			// +++++++++ >
				)" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i],
                                  "FUN_DECLARATION")] +
        R"(
			// +++++++++ >

		private:
			DECLARE_NO_COPY_CLASS(NS_)" +
        ID[Namespace_Total_Names[i]] + R"()
			DECLARE_EVENT_TABLE()
	};

	NS_)" +
        ID[Namespace_Total_Names[i]] + R"( *OBJ_CLASS_WINDOW_)" +
        ID[Namespace_Total_Names[i]] + R"(;
	NS_)" +
        ID[Namespace_Total_Names[i]] + R"(::~NS_)" +
        ID[Namespace_Total_Names[i]] + R"((){}

	// --------------------
	// Namespace )" +
        ID[Namespace_Total_Names[i]] + R"( Constructor
	// --------------------

	NS_)" +
        ID[Namespace_Total_Names[i]] + R"( :: NS_)" +
        ID[Namespace_Total_Names[i]] + R"(() : 
		wxFrame(NULL, ID_WINDOW_)" +
        ID[Namespace_Total_Names[i]] + R"(, 
		(")" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "نص")] + R"("),
		wxPoint()" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "افصول")] +
        R"(, )" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "ارتوب")] + R"(),
		wxSize()" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "عرض")] + R"(, )" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "ارتفاع")] +
        R"(), )" + CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "شكل")] +
        R"( ) 
		// wxMAXIMIZE_BOX | wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT | wxRESIZE_BORDER
	{
		wxPanel *P = new wxPanel(this, wxID_ANY);
		
		// --------------------
		// )" +
        ID[Namespace_Total_Names[i]] + R"( Controls Constructor
		// --------------------
		
		// +++++++++ >
			)" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i],
                                  "CTR_CONSTRUCTOR")] +
        R"(
		// +++++++++ >
		
		// ------------------------------------------------
	}

	// ------------------------------------------------
	// Namespace Winx Close
	// ------------------------------------------------

	void NS_)" +
        ID[Namespace_Total_Names[i]] + R"(::OnClose(wxCloseEvent& event)
	{
		event.Skip();

		WINDOW_IS_SHOW_)" +
        ID[Namespace_Total_Names[i]] + R"( = false;
		WINDOW_IS_CONSTRUCTION_)" +
        ID[Namespace_Total_Names[i]] + R"( = false;
		
		// +++++++++ >
			)" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "CLOSE")] + R"(
		// +++++++++ >
	}

	// ------------------------------------------------
	// Event Table
	// ------------------------------------------------

	BEGIN_EVENT_TABLE(NS_)" +
        ID[Namespace_Total_Names[i]] + R"(, wxFrame)
		EVT_CLOSE(NS_)" +
        ID[Namespace_Total_Names[i]] + R"(::OnClose)
		
		// +++++++++ >
			)" +
        CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "EVENT")] + R"(
		// +++++++++ >
		
	END_EVENT_TABLE()

	// ========================================================================
	)";

    CODE.append(BUFFER);
  }

  return CODE;
}

// -----------------------------------------------------------
// C++ CODE
// -----------------------------------------------------------

// TODO: So, when tray GetValue() of control of destroyed Win
// the app not continue the fucntion... so, solution is be sure
// the windows is constructed before control:GetValue() [ MyControl:نص ]

// 	// ------------------------------------------------
// 	// Generated by Alif Compiler )" + VERSION + R"(
// 	// www.aliflang.org
// 	// ------------------------------------------------

std::string CODE_GENERATOR_GET_PC_GUI() {

  return R"( 
			/*
			Generated by Alif Compiler v)" +
         VERSION + R"(
			www.aliflang.org
			*/

			// --[ Core ] -------------------------------------
			)" +
         code_core + R"(
			
			// ++++++++++++++++++++++++++++++++++++++++++++++++
			//                    Alif 2.x
			//
			// --[ CPP_ID_DECLARATION ]------------------------
				)" +
         CPP_ID_DECLARATION + R"(
			// --[ CPP_GLOBAL ]--------------------------------
				)" +
         CPP_GLOBAL + R"(
			// --[ CPP_OBJ_DECLARATION ]-----------------------
				)" +
         CPP_OBJ_DECLARATION + R"(
			// --[ CPP_WINDOW[][FUN_DECLARATION] ]-------------
				)" +
         CPP_WINDOW[std::make_pair("", "FUN_DECLARATION")] + R"(
			// --[ CPP_WINDOW[][LOAD] ]------------------------
				)" +
         CPP_WINDOW[std::make_pair("", "LOAD")] + R"(
			// --[ CPP_WINDOW[رئيسية][FUN_DECLARATION] ]------
				)" +
         CPP_WINDOW[std::make_pair("رئيسية", "FUN_DECLARATION")] + R"(
			// --[ CPP_WINDOW[رئيسية][LOAD] ]-----------------
				)" +
         CPP_WINDOW[std::make_pair("رئيسية", "LOAD")] + R"(
			// --[ CPP_CLASS ]---------------------------------
				)" +
         CPP_CLASS + R"(
			// --[ CPP_FUN ]-----------------------------------
				// CPP_FUN
			// --[ CPP_GLOBAL_FUN ]----------------------------
				)" +
         CPP_GLOBAL_FUN + R"(
			// ++++++++++++++++++++++++++++++++++++++++++++++++		

			// --[ Stack ] ------------------------------------
			)" +
         code_stack + R"(

			// --[ Code ] -------------------------------------
			)" +
         code + R"(

			// --[ Entry point ] ------------------------------
			void _alif_main(){

				alifcore_initialization();

				)" +
         code_entry_point + R"(
				
			}

			#ifdef _WIN32
				int wmain(int argc, wchar_t* argv[]) { _alif_main(); return 0;}
				int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow) { _alif_main(); return 0;}
			#elif  __APPLE__
				int main(int argc, char** argv) { _alif_main(); return 0;}
			#else
				int main(int argc, char** argv) { _alif_main(); return 0;}
			#endif
			
			// ------------------------------------------------
			// Generated by Alif Compiler v)" +
         VERSION + R"(
			// www.aliflang.org
			// ------------------------------------------------
		)";
}

// Phone Android
std::string CODE_GENERATOR_GET_PHONE_ANDROID() { return ""; }

// Parser *******************************************************

#include "alif_macro.hpp"
#include "alif_namespace.hpp"

//#include "alif_window.hpp"
//#include "alif_control.hpp"

#include "alif_bool.hpp"
#include "alif_class.hpp"
#include "alif_end.hpp"
#include "alif_function.hpp"
#include "alif_function_call.hpp"
#include "alif_function_class.hpp"
#include "alif_if.hpp"
#include "alif_if_else.hpp"
#include "alif_int.hpp"
#include "alif_loop.hpp"
#include "alif_next_break.hpp"
#include "alif_obj.hpp"
#include "alif_obj_new.hpp"
#include "alif_operator.hpp"
#include "alif_return.hpp"
#include "alif_string.hpp"
#include "alif_var.hpp"
#include "alif_var_g.hpp"
#include "alif_var_g_class.hpp"

void ALIF_PARSER(CLASS_TOKEN *o_tokens) {

  if (!o_tokens->TOKENS_PREDEFINED) {

    // Check for tokens not predifined

    if (ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED[o_tokens->PATH_FULL_SOURCE]) {

      if (DEBUG)
        DEBUG_MESSAGE("\n Already Parsed(Not-Predifined) [" +
                          o_tokens->PATH_FULL_SOURCE + "] \n",
                      o_tokens);
      return;
    } else
      ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED[o_tokens->PATH_FULL_SOURCE] =
          true;
  } else {

    // Check for tokens already predifined

    if (ALREADY_PARSED_FILE_TOKENS_PREDEFINED[o_tokens->PATH_FULL_SOURCE]) {

      if (DEBUG)
        DEBUG_MESSAGE("\n Already Parsed(Predifined) [" +
                          o_tokens->PATH_FULL_SOURCE + "] \n",
                      o_tokens);
      return;
    } else
      ALREADY_PARSED_FILE_TOKENS_PREDEFINED[o_tokens->PATH_FULL_SOURCE] = true;
  }

  // For every line
  for (o_tokens->Line = 1; o_tokens->Line <= o_tokens->TOTAL_LINES;
       o_tokens->Line++) {

    // Check total tokens on this current line
    if (o_tokens->TOTAL[o_tokens->Line] < 1 ||
        o_tokens->TOTAL[o_tokens->Line] > 1024) {

      // Empty line, or the index is memory-non-allocated
      continue;
    }

    std::string Token[2048];

    // Clear TempToken[1024] on every line
    // this is for fixing TempToken[p + 1] -> last token from last line!
    for (int clear_p = 0; clear_p <= 1023; clear_p++)
      TempToken[clear_p] = "";

    for (o_tokens->NUMBER = 1;
         o_tokens->NUMBER <= o_tokens->TOTAL[o_tokens->Line];
         o_tokens->NUMBER++) {
      // -----------------------------------------------------------------
      // Line Number 		: o_tokens->Line
      // Token Number 	: o_tokens->NUMBER
      // Token 			: o_tokens->TOKEN[std::make_pair(o_tokens->Line,
      // o_tokens->NUMBER)]
      // -----------------------------------------------------------------
      if (o_tokens->TOKEN[std::make_pair(o_tokens->Line, o_tokens->NUMBER)] !=
          "")
        Token[o_tokens->NUMBER] =
            o_tokens->TOKEN[std::make_pair(o_tokens->Line, o_tokens->NUMBER)];
    }

    if (Token[1] == "")
      continue;

    if (DEBUG)
      DEBUG_MESSAGE(IntToString(o_tokens->Line) + ": ", o_tokens); // DEBUG

    // ------------------------------------------------------------------------

    if (Token[1] == "#") {

      // Macros
      parser_macro_ui(Token, o_tokens);

      // --------------------------------------------
      /*
      if (Token[2] == "واجهة"){

              // This macro basicaly read html file
              // and save it into a const std string.

              // #واجهة رئيسية "UI_WEB_1"

              if (Token[3] == "")
              ErrorCode("يجب تحديد اسم النافذة", o_tokens);

              if (!IsValidName(Token[3], o_tokens))
                      ErrorCode("اسم غير مقبول : ' " + Token[3] + " ' ",
      o_tokens);

              if (Token[4] == "")
                      ErrorCode("يجب تحديد اسم الملف", o_tokens);

              if(!IsValidStringFormat(Token[4], o_tokens))
                      ErrorCode("خطأ في كتابة إسم الملف: "+ Token[4], o_tokens);

              // Get PATH_FULL_WINDOW_WEB
              AlifLexerParser(remove_quote(Token[4], o_tokens), "ALIFUIW",
      false, o_tokens->TOKENS_PREDEFINED);

              //parser_NewWindowWeb(Token, o_tokens);
      }
      else if (Token[2] == "أضف"		||	// #اضف "test.ALIF"
                      //Token[2] == "واجهة"	||	// #واجهة "test.ALIFUI"
                      Token[2] == "مكتبة" )		// #مكتبة "test.ALIF"
      {
              if (Token[3] == "")
                      ErrorCode("مسار الملف غير محدد" + Token[2], o_tokens);

              if (!IsValidStringFormat(Token[3], o_tokens))
                      ErrorCode("مسار الملف غير صائب : ' " + Token[3] + " ' ",
      o_tokens);

              if (Token[4] != "")
                      ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ",
      o_tokens);

              if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
                      ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة '
      #ألف ' في الأعلى", o_tokens);

              if (Token[2] == "أضف")
              {
                      // Include Local File for aditional user code.

                      //if (Token[4] != "")
                      //	ErrorCode("أمر غير معروف : ' " + Token[4] + " '
      ", o_tokens);

                      if(!IsValidStringFormat(Token[3], o_tokens))
                              ErrorCode("خطأ في كتابة إسم الملف: "+ Token[3],
      o_tokens);

                      if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF]
      ... \n\n", o_tokens); AlifLexerParser(remove_quote(Token[3], o_tokens),
      "ALIF", false, o_tokens->TOKENS_PREDEFINED);
              }
              // else if (Token[2] == "واجهة")
              // {
              // 	// Include local file, for user UI code.

              // 	//if (Token[4] != "")
              // 	//	ErrorCode("أمر غير معروف : ' " + Token[4] + " '
      ", o_tokens);

              // 	if(!IsValidStringFormat(Token[3], o_tokens))
              // 		ErrorCode("خطأ في كتابة إسم الملف: "+ Token[3],
      o_tokens);

              // 	if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " .
      ALIF UI]
      ... \n\n", o_tokens);
              // 	AlifLexerParser(remove_quote(Token[3], o_tokens),
      "ALIFUI", false, o_tokens->TOKENS_PREDEFINED);
              // }
              else if (Token[2] == "مكتبة")
              {
                      // Include Library from Lib folder, or local folder.

                      // if (remove_quote(Token[3], o_tokens) == "رسالة" ||
                      // 	remove_quote(Token[3], o_tokens) == "الرسالة")
                      // 	ErrorCode("ثم دمج مكتبة ' رسالة ' مع مكتبات ألف
      القياسية الرئيسية، لدى يقوم المترجم باستعمالها بشكل آلي، المرجو إزالة هذا
      السطر ", o_tokens);

                      // Python lib need 3 other var to setup
                      / * if(remove_quote(Token[3], o_tokens) == "البايثون"){

                              //if (Token[7] != "")
                              //	ErrorCode("أمر غير معروف : ' " +
      Token[7]
      + " ' ", o_tokens);

                              // #مكتبة "البايثون" "/usr/include/python3.5"4
      "/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu"6 "python3.5"6 ...7

                              if (Token[4] == "")
                                      ErrorCode("يجب تحديد مسار عناوين
      البايثون", o_tokens); if(!IsValidStringFormat(Token[4], o_tokens))
                                      ErrorCode("خطأ في كتابة مسار عناوين
      البايثون: "+ Token[4], o_tokens);

                              if (Token[5] == "")
                                      ErrorCode("يجب تحديد مسار مكتبات
      البايثون", o_tokens); if(!IsValidStringFormat(Token[5], o_tokens))
                                      ErrorCode("خطأ في كتابة مسار مكتبات
      البايثون: "+ Token[5], o_tokens);

                              if (Token[6] == "")
                                      ErrorCode("يجب تحديد إسم مكتبة البايثون",
      o_tokens); if(!IsValidStringFormat(Token[6], o_tokens)) ErrorCode("خطأ في
      كتابة إسم مكتبة البايثون: "+ Token[6], o_tokens);

                              // Setup Python lib env.
                              PythonSetEnvirenment(	remove_quote(Token[4],
      o_tokens),	// /usr/include/python3.5 remove_quote(Token[5],
      o_tokens),	// /usr/lib/python3.5/config-3.5m-x86_64-linux-gnu
                                                                              remove_quote(Token[6],
      o_tokens));	// python3.5 } * /

                      else {

                              if(remove_quote(Token[3], o_tokens) == "البايثون")
                                      PythonSetEnvirenment();

                              // Other librarys check..
                              // Example: #مكتبة "الوقت"

                              if (Token[4] != "")
                                      ErrorCode("أمر غير معروف : ' " + Token[4]
      + " ' ", o_tokens);
                      }

                      if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF
      LIB] ... \n\n", o_tokens); AlifLexerParser(remove_quote(Token[3],
      o_tokens), "ALIFLIB", false, o_tokens->TOKENS_PREDEFINED);
              }
              continue;
      }
      */

    }

    // ---------------------------------------------------------------------------------
    // General Erros
    // ---------------------------------------------------------------------------------

    else if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
      ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى",
                o_tokens);

    else if (Token[1] == "خاص" && !IsInsideClass) // ERROR
      ErrorCode("يجب استعمال ' خاص ' داخل صنف", o_tokens);
    else if (Token[1] == "خاص" && Token[2] == "")
      ErrorCode("يجب تحديد نوع المتغير بعد ' خاص ' ", o_tokens);
    else if (Token[1] == "خاص" &&
             (Token[2] != "عدد" && Token[2] != "نص" && Token[2] != "منطق" &&
              Token[2] != "دالة")) // ERROR
      ErrorCode("أمر غير معروف : ' " + Token[2] +
                    " ' ، على العموم أمر ' خاص ' يمكن استعماله فقط مع متغيرات "
                    "من نوع حرف، عدد، منطق، أو دالة",
                o_tokens);

    // ---------------------------------------------------------------------------------
    // _س_ ... C++ ... _س_
    // ---------------------------------------------------------------------------------

    else if (Token[1] == "_س_") {

      if (!o_tokens->TOKENS_PREDEFINED)
        continue;

      if (!LIB_INSIDE_CPP_CODE) {

        if (DEBUG)
          DEBUG_MESSAGE("{_س_ START} ", o_tokens); // DEBUG

        LIB_INSIDE_CPP_CODE = true;
        LIB_PARSER_CG_BUFER = "";

        for (int p = 2; p <= o_tokens->TOTAL[o_tokens->Line];
             p++) // _س_ ...C++...@ Alif @...C++... _س_
        {
          if (Token[p] == "_س_") // End C++ Code
          {
            if (DEBUG)
              DEBUG_MESSAGE("{_س_ END} ", o_tokens); // DEBUG
            LIB_INSIDE_CPP_CODE = false;

            // *** Generate Code ***
            if (IsInsideClass) {
              // Class Area
              CPP_CLASS.append(" " + LIB_PARSER_CG_BUFER + " ");
              // code_add(CODE, LIB_PARSER_CG_BUFER);
            } else if (!IsInsideNamespace && !IsInsideFunction) {
              // Global Area
              CPP_GLOBAL.append(" " + LIB_PARSER_CG_BUFER + " ");
              // code_add(CODE, LIB_PARSER_CG_BUFER);
            } else if (!IsInsideNamespace && IsInsideFunction) {
              // Global Function
              CPP_GLOBAL_FUN.append(" " + LIB_PARSER_CG_BUFER + " ");
              // code_add(CODE, LIB_PARSER_CG_BUFER);
            } else if (IsInsideNamespace && IsInsideFunction) {
              // Local Function
              cpp_AddScript(TheFunction, " " + LIB_PARSER_CG_BUFER + " ");
              // code_add(CODE, LIB_PARSER_CG_BUFER);
            } else {
              ErrorCode("علة: لم تنجح عملية ايجاد مكان شفرة سي++", o_tokens);
            }
            // *** *** *** *** *** ***
          } else if (Token[p] == "@") // @ Start ...
          {
            // ...C++...@ Alif @...C++...

            if (DEBUG)
              DEBUG_MESSAGE("{@} ", o_tokens); // DEBUG

            TempTokenCount = 1; // CheckForSyntax() Need this.
            TempToken[0] = "="; // CheckForSyntax() Need this.

            bool AT_FOUND = false;

            for (int c = p + 1; c < o_tokens->TOTAL[o_tokens->Line]; c++) {
              if (Token[c] == "@") {
                p = c;
                AT_FOUND = true;
                break;
              }

              TempToken[TempTokenCount] = Token[c];
              TempTokenCount++;
            }

            if (!AT_FOUND)
              ErrorCode("نهايه شفرة سي++ غير موجوده ' @ '", o_tokens);

            std::string CLASS_OR_WIN;
            if (IsInsideClass)
              CLASS_OR_WIN = TheClass;
            else
              CLASS_OR_WIN = TheNamespace;

            ScriptSyntaxBuffer = CheckForSyntax(
                "C++",     // OBJECTIF_TYPE
                true,      // Accept Using Reference to namespace:Controls
                true,      // Accept Using Reference to namespace:Function
                true,      // Accept Using Reference to Global Functions
                true,      // Accept Using Reference to Local Functions
                true,      // Accept Using Reference to Global VAR
                true,      // Accept Using Reference to Local VAR
                true,      // Accept Convertion from String To Int
                true,      // Accept Convertion from Int To String
                TempToken, // SYNTAX[] std::string
                (TempTokenCount - 1), // SYNTAX_LONG int
                CLASS_OR_WIN,         // TMP_WINDOW_NAME
                TheFunction,          // TMP_FUNCTION_NAME
                o_tokens);

            if (DEBUG)
              DEBUG_MESSAGE("{@} ", o_tokens); // DEBUG

            // *** C++ ***
            LIB_PARSER_CG_BUFER.append(" " + ScriptSyntaxBuffer + " ");
            // *** *** *** *** *** ***

            // @ End.
          } else if (Token[p] != "") {
            // Add C++ Code to the buffer
            LIB_PARSER_CG_BUFER.append(Token[p]);
            if (DEBUG)
              DEBUG_MESSAGE("{" + Token[p] + "} ", o_tokens); // DEBUG
          }
        }
      } else {
        if (DEBUG)
          DEBUG_MESSAGE("{_س_ END} ", o_tokens); // DEBUG
        LIB_INSIDE_CPP_CODE = false;
      }
    }

    else if (LIB_INSIDE_CPP_CODE) {
      // Continue to adding C++ Code to the Buffer

      for (int p = 1; p <= o_tokens->TOTAL[o_tokens->Line];
           p++) // _س_ ...C++...@ Alif @...C++... _س_
      {
        if (Token[p] == "_س_") // End C++ Code
        {
          if (DEBUG)
            DEBUG_MESSAGE("{_س_ END} ", o_tokens); // DEBUG
          LIB_INSIDE_CPP_CODE = false;

          // if(DEBUG)DEBUG_MESSAGE("\n\n GENERATOR -------> |" +
          // LIB_PARSER_CG_BUFER + "| \n\n", o_tokens); // DEBUG

          // *** Generate Code ***
          if (IsInsideClass) {
            // Class Area
            CPP_CLASS.append(" " + LIB_PARSER_CG_BUFER + " ");
            // code_add(CODE, LIB_PARSER_CG_BUFER);
          } else if (!IsInsideNamespace && !IsInsideFunction) {
            // Global Area
            CPP_GLOBAL.append(" " + LIB_PARSER_CG_BUFER + " ");
            // code_add(CODE, LIB_PARSER_CG_BUFER);
          } else if (!IsInsideNamespace && IsInsideFunction) {
            // Global Function
            CPP_GLOBAL_FUN.append(" " + LIB_PARSER_CG_BUFER + " ");
            // code_add(CODE, LIB_PARSER_CG_BUFER);
          } else if (IsInsideNamespace && IsInsideFunction) {
            // Local Function
            cpp_AddScript(TheFunction, " " + LIB_PARSER_CG_BUFER + " ");
            // code_add(CODE, LIB_PARSER_CG_BUFER);
          } else {
            ErrorCode("علة: لم تنجح عملية ايجاد مكان شفرة سي++", o_tokens);
          }
          // *** *** *** *** *** ***
        } else if (Token[p] == "@") // @ Start ...
        {
          // ...C++...@ Alif @...C++...

          if (DEBUG)
            DEBUG_MESSAGE("{@} ", o_tokens); // DEBUG

          TempTokenCount = 1; // CheckForSyntax() Need this.
          TempToken[0] = "="; // CheckForSyntax() Need this.

          bool AT_FOUND = false;

          for (int c = p + 1; c < o_tokens->TOTAL[o_tokens->Line]; c++) {
            if (Token[c] == "@") {
              p = c;
              AT_FOUND = true;
              break;
            }

            TempToken[TempTokenCount] = Token[c];
            TempTokenCount++;
          }

          if (!AT_FOUND)
            ErrorCode("نهايه شفرة سي++ غير موجوده ' @ '", o_tokens);

          std::string CLASS_OR_WIN;
          if (IsInsideClass)
            CLASS_OR_WIN = TheClass;
          else
            CLASS_OR_WIN = TheNamespace;

          ScriptSyntaxBuffer = CheckForSyntax(
              "C++",     // OBJECTIF_TYPE
              true,      // Accept Using Reference to namespace:Controls
              true,      // Accept Using Reference to namespace:Function
              true,      // Accept Using Reference to Global Functions
              true,      // Accept Using Reference to Local Functions
              true,      // Accept Using Reference to Global VAR
              true,      // Accept Using Reference to Local VAR
              true,      // Accept Convertion from String To Int
              true,      // Accept Convertion from Int To String
              TempToken, // SYNTAX[] std::string
              (TempTokenCount - 1), // SYNTAX_LONG int
              CLASS_OR_WIN,         // TMP_WINDOW_NAME
              TheFunction,          // TMP_FUNCTION_NAME
              o_tokens);

          if (DEBUG)
            DEBUG_MESSAGE("{@} ", o_tokens); // DEBUG

          // *** C++ ***
          LIB_PARSER_CG_BUFER.append(" " + ScriptSyntaxBuffer + " ");
          // *** *** *** *** *** ***

          // @ End.
        } else if (Token[p] != "") {
          // Add C++ Code to the buffer
          LIB_PARSER_CG_BUFER.append(Token[p]);
          if (DEBUG)
            DEBUG_MESSAGE("{" + Token[p] + "} ", o_tokens); // DEBUG
        }
      }
    }

    else if (Token[1] == "@") {
      if (!LIB_INSIDE_CPP_CODE)
        ErrorCode("يجب تحديد ' _س_ ' قبل ' @ '", o_tokens);

      ErrorCode("لا يمكن بدء السطر بالإشارة ' @ '", o_tokens);
    }

    // ---------------------------------------------------------------------------------

    // else if (Token[1] == "أداة"){
    // 	parser_Control(Token, o_tokens);
    // }

    else if (Token[1] == "مجال") {

      // parser_NewWindow(Token, o_tokens);
      parser_new_namespace(Token, o_tokens);
    }

    /*
    else if (Token[1] == "عدد"		||
                    Token[1] == "نص"		||
                    Token[1] == "منطق"		||
                    Token[1] == "منطق"		||
                    Token[1] == "خاص"	||
                    Token[1] == "ثابت")
            CheckForNewVar(Token, o_tokens);
    */

    else if (Token[1] == "عدد" || (Token[1] == "خاص" && Token[2] == "عدد")) {
      parser_Int(Token, o_tokens);
    }

    else if (Token[1] == "نص" || (Token[1] == "خاص" && Token[2] == "نص")) {
      parser_String(Token, o_tokens);
    }

    else if (Token[1] == "منطق" || (Token[1] == "خاص" && Token[2] == "منطق")) {
      parser_bool(Token, o_tokens);
    }

    else if (Token[1] == "صنف") {
      parser_Class(Token, o_tokens);
    }

    else if ((Token[1] == "دالة" && IsInsideClass) ||
             (Token[1] == "خاص" && Token[2] == "دالة" && IsInsideClass)) {
      parser_FunctionClass(Token, o_tokens);
    }

    else if (Token[1] == "دالة") {
      parser_Function(Token, o_tokens);
    }

    else if (Token[1] == "إذا" || Token[1] == "أو") {
      parser_IfOr(Token, o_tokens);
    }

    else if (Token[1] == "وإلا") {
      parser_IfElse(Token, o_tokens);
    }

    else if (Token[1] == "كلما") {
      parser_While(Token, o_tokens);
    }

    else if (Token[1] == "التالي" || Token[1] == "كسر") {
      parser_NextBreak(Token, o_tokens);
    }

    else if (Token[1] == "إرجاع") {
      parser_Return(Token, o_tokens);
    }

    else if (Token[1] == "كائن") {
      parser_ObjNew(Token, o_tokens);
    }

    else if (OBJ_IS_SET[std::make_pair(
                 TheNamespace + TheFunction,
                 Token[1])] || // Namespace -> Func. 	Local Obj.
             OBJ_IS_SET[std::make_pair(
                 TheClass + TheFunction,
                 Token[1])] || // Class -> Func. 	Local Obj.
             OBJ_IS_SET[std::make_pair(TheClass,
                                       Token[1])] || // Class.
                                                     // Global Class Obj.
             OBJ_IS_SET[std::make_pair("",
                                       Token[1])]) { // Global Area. Global Obj.
      parser_Obj(Token, o_tokens);
    }

    else if (L_VAR_IS_SET[std::make_pair(TheNamespace + TheFunction,
                                         Token[1])] ||
             L_VAR_IS_SET[std::make_pair(TheClass + TheFunction, Token[1])]) {
      parser_Var(Token, o_tokens);
    }

    else if (CLASS_G_VAR_IS_SET[std::make_pair(TheClass, Token[1])] &&
             IsInsideClass) {
      parser_VarGlobalClass(Token, o_tokens);
    }

    else if (substr_utf8(Token[1], 0, 1) == "_" || G_VAR_IS_SET[(Token[1])]) {
      parser_VarGlobal(Token, o_tokens);
    }

    // Call function (Global / Local / Class-fun)

    else if (G_FUN_IS_SET[(Token[1])] ||
             L_FUN_IS_SET[std::make_pair(TheNamespace, Token[1])] ||
             CLASS_FUN_IS_SET[std::make_pair(TheClass, Token[1])]) {

      parser_FunctionCall(Token, o_tokens);
    }

    else if (                      // CONTROL_WIN_IS_SET[Token[1]] ||
        namespace_is_set[Token[1]] // ||
        // CONTROL_IS_SET[std::make_pair(TheNamespace, Token[1])] ||
        //(Token[1] == "رئيسية" && MAIN_WIN_IS_SET)
    ) {

      parser_TwoPointOperator(Token, o_tokens);
    }

    else if (Token[1] == "نهاية") {
      parser_End(Token, o_tokens);
    }

    // #######################################################

    else {
      if (!o_tokens->TOKENS_PREDEFINED) {
        if (Control_ID[Token[1]] != "") {
          // show error description
          // when, (no-win) CTR:OPTION
          // in global area.

          ErrorCode("أمر غير معروف : ' " + Token[1] +
                        " '، على العموم، إن كنت تقصد أداة، لا تنسى أن تحدد إسم "
                        "النافذة قبل إسم الأداة  ",
                    o_tokens);
        }
        // مجال
        else if (Token[1] == "مجال" || Token[1] == "المجال" ||
                 Token[1] == "النافذة" || Token[1] == "نافدت" ||
                 Token[1] == "نافذت" || Token[1] == "نافذ" ||
                 Token[1] == "نافد") {
          ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' مجال ' ؟ ",
                    o_tokens);
        }
        // نص
        else if (Token[1] == "كلمة" || Token[1] == "حروف" ||
                 Token[1] == "الحرف" || Token[1] == "نص" ||
                 Token[1] == "الحروف" || Token[1] == "جملة") {
          ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' نص ' ؟ ",
                    o_tokens);
        }
        // إذا
        else if (Token[1] == "ادا" || Token[1] == "إدا" || Token[1] == "أدا" ||
                 Token[1] == "اذا" || Token[1] == "أذا") {
          ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' إذا ' ؟ ",
                    o_tokens);
        }
        // أو
        else if (Token[1] == "او" || Token[1] == "ٱو" || Token[1] == "آو" ||
                 Token[1] == "والا" ||
                 // Token[1] == "وإلا" ||
                 Token[1] == "وألا") {
          ErrorCode("أمر غير معروف : ' " + Token[1] +
                        " '، هل تقصد ' وإلا ' أو تقصد ' أو ' ؟ ",
                    o_tokens);
        }
        // أداة
        else if (Token[1] == "اداة" || Token[1] == "اداه" ||
                 Token[1] == "ادات" || Token[1] == "آداة" ||
                 Token[1] == "آداه" || Token[1] == "آدات" ||
                 Token[1] == "أداه" || Token[1] == "أدات" ||
                 Token[1] == "ٱداة" || Token[1] == "ٱداه" ||
                 Token[1] == "ٱدات") {
          ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' أداة ' ؟ ",
                    o_tokens);
        }
        // أضف
        else if (Token[1] == "اضف" || Token[1] == "ٱضف" || Token[1] == "آضف" ||
                 Token[1] == "أظف" || Token[1] == "اظف" || Token[1] == "ٱظف" ||
                 Token[1] == "آظف" || Token[1] == "إظف" || Token[1] == "إضف") {
          ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' #أضف ' ؟ ",
                    o_tokens);
        }
        // خاص
        else if (Token[1] == "خص" || Token[1] == "حاص" || Token[1] == "الخاص" ||
                 Token[1] == "الحاص") {
          ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' خاص ' ؟ ",
                    o_tokens);
        }
        // إرجاع
        else if (Token[1] == "رجوع" || Token[1] == "الرجوع" ||
                 Token[1] == "return" || Token[1] == "أرجاع" ||
                 Token[1] == "ارجاع") {
          ErrorCode("أمر غير معروف : ' " + Token[1] +
                        " '، هل تقصد ' إرجاع ' ؟ ",
                    o_tokens);
        }

        // Yes, Unknow Token.. but meybe the definition
        // of this unknown token is in the next lines
        // so lets Parse all Tokens first, then re-parse again
      } else {

        // else
        ErrorCode("أمر غير معروف : ' " + Token[1] + " ' ", o_tokens);
      }
    }

    // #######################################################

    if (o_tokens->TOKENS_PREDEFINED)
      if (DEBUG)
        DEBUG_MESSAGE("\n\n", o_tokens);

  } // End Line Loop

  // -------------------------
  // Check current file
  // if (o_tokens->TOKENS_PREDEFINED)
  FINAL_CURRENT_FILE_CODE_CHECKING(o_tokens);
  // -------------------------
}

// Lexer ********************************************************

// ----------------------------------
// INITIALIZATION
// ----------------------------------

void ALIF_VAR_INITIALIZATION_FOR_NEW_SOURCE_FILE(bool FIRST_FILE) {
  // ----------------------------------
  // مجال
  // ----------------------------------

  // if (FIRST_FILE)
  // {
  // 	MAIN_WIN_IS_SET = false;
  // 	MAIN_WIN_AT_LINE = "0";
  // }

  IsInsideNamespace = false;
  TheNamespace = "";

  // ----------------------------------
  // دالة
  // ----------------------------------

  IsInsideFunction = false;
  TheFunction = "";

  // ----------------------------------
  // PARENTHESIS / IF
  // ----------------------------------

  ALIF_PARENTHESIS_STATUS = 0;

  ALIF_IF_STATUS = 0;

  // ----------------------------------
  // Code
  // ----------------------------------

  ScriptSyntaxBuffer = "";
  ScriptBuffer = "";
}

// ------------------------------------------------------
// Read ALif Lib Setting file
// ------------------------------------------------------

// void ALIF_LIB_SETTING()
// {
// 	ifstream FILE_STREAM(PATH_FULL_LIB_SETTING.c_str());

// 	if (!FILE_STREAM.is_open())
// 	{
// 		ALIF_ERROR("ERROR [F001]: Could not open " +
// PATH_FULL_LIB_SETTING); 		exit(EXIT_FAILURE);
// 	}

// 	std::string LINE8;

// 	while (getline(FILE_STREAM, LINE8))
// 	{
// 		// ------------------------------------------------------
// 		// Ignore blank lines
// 		// ------------------------------------------------------

// 		if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") ||
// (LINE8 == "") || (LINE8 == " ")) 			continue;

// 		// ------------------------------------------------------
// 		// Remove bad line-break character
// 		// Windows: \r\n
// 		// Linux: \n
// 		// MacOS: \r
// 		// ------------------------------------------------------

// 		size_t LINE_FIX_LAST_POS = LINE8.find_last_not_of("\r\n");

// 		if (LINE_FIX_LAST_POS != std::string::npos)
// 		{
// 			LINE8.substr(0, LINE_FIX_LAST_POS + 1).swap(LINE8);
// 		}

// 		// ------------------------------------------------------
// 		// Remove extra spaces
// 		// ------------------------------------------------------

// 		//LINE8 = REMOVE_DOUBLE_SPACE(LINE8, &OBJ_CLASS_TOKEN);

// 		//if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") ||
// (LINE8 == "") || (LINE8 == " "))
// 			//continue;

// 		// ------------------------------------------------------
// 		// Split
// 		// ------------------------------------------------------

// 		// رسالة|risalah

// 		LIB_FILE_NAME[LINE8.substr(0, LINE8.find("|"))] =
// LINE8.substr(LINE8.find("|") + 1);
// 	}

// 	FILE_STREAM.close();
// }

void ADD_TOKEN(std::string TOKEN_CHAR, bool NEW_TOKEN, bool NEW_TOKEN_AFTER,
               int REAL_CHAR_NUMBER, CLASS_TOKEN *o_tokens) {
  if (NEW_TOKEN) {
    // New Token
    if (o_tokens->TOKEN[std::make_pair(
            o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] !=
        "") {
      o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
    }

    if (TOKEN_CHAR != "") {
      if (o_tokens->TOTAL[o_tokens->TOTAL_LINES] < 1) {
        o_tokens->TOTAL[o_tokens->TOTAL_LINES] = 1;
      }

      o_tokens->TOKEN[std::make_pair(o_tokens->TOTAL_LINES,
                                     o_tokens->TOTAL[o_tokens->TOTAL_LINES])] =
          TOKEN_CHAR;

      // Set Real Token position in the real line
      o_tokens->REAL_TOKEN_POSITION[std::make_pair(
          o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] =
          REAL_CHAR_NUMBER + CharCount_utf8(TOKEN_CHAR, o_tokens);

      if (NEW_TOKEN_AFTER &&
          o_tokens->TOKEN[std::make_pair(
              o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] !=
              "") {
        o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
      }
    }
  } else if (TOKEN_CHAR != "") {
    // New Char
    if (o_tokens->TOTAL[o_tokens->TOTAL_LINES] < 1) {
      o_tokens->TOTAL[o_tokens->TOTAL_LINES] = 1;
    }

    (o_tokens->TOKEN[std::make_pair(o_tokens->TOTAL_LINES,
                                    o_tokens->TOTAL[o_tokens->TOTAL_LINES])])
        .append(TOKEN_CHAR);

    // Set Real Token position in the real line
    o_tokens->REAL_TOKEN_POSITION[std::make_pair(
        o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] =
        REAL_CHAR_NUMBER + CharCount_utf8(TOKEN_CHAR, o_tokens);

    if (NEW_TOKEN_AFTER &&
        o_tokens->TOKEN[std::make_pair(
            o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] !=
            "") {
      o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
    }
  }
}

void AlifLexerParser(std::string file, std::string target, bool is_first_file,
                     bool is_predefined) {

  // Alif lexer

  CLASS_TOKEN OBJ_CLASS_TOKEN;

  // ------------------------------------------------------
  // ALIF FILE EXTENTION
  // ------------------------------------------------------

  int POS = file.find_last_of(".");
  std::string EXTENTION;

  // Get extention
  if (POS > 0)
    EXTENTION = file.substr(POS + 1);
  else
    EXTENTION = "";

  // check extention
  if (EXTENTION == "") {

    // #include "file"
    // #include "/foo/folder/file"

    if (target == "ALIF") {

      if (is_path(file))
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = file + ".alif";
      else
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE =
            PATH_WORKING + SEPARATION + file + ".alif";
    } else if (target == "ALIFLIB") {

      // #lib "file"
      // #lib "/foo/folder/file"

#ifdef _WIN32
      if (is_path(file))
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = file + ".alif";
      else
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE =
            PATH_ABSOLUTE + "\\aliflib\\" + file + ".alif";
#elif __APPLE__
      if (is_path(file))
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = file + ".alif";
      else
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE =
            XSTR(INSTALL_PREFIX) "/lib/aliflib/" + file + ".alif";
#else
      if (is_path(file))
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = file + ".alif";
      else
        OBJ_CLASS_TOKEN.PATH_FULL_SOURCE =
            XSTR(INSTALL_PREFIX) "/lib/aliflib/" + file + ".alif";
#endif
    } else
      ErrorCode("علة: نوع ملف غير معروف : ' " + target + " ' ",
                &OBJ_CLASS_TOKEN);
  } else if (EXTENTION == "alif" || EXTENTION == "ALIF" || EXTENTION == "ألف") {

    // #include "file.alif"
    // #include "/foo/folder/file.alif"

    if (target != "ALIF")
      ErrorCode("يجب إستعمال #اضف لترجمة هدا الملف : ' " + file + " ' ",
                &OBJ_CLASS_TOKEN);

    if (is_path(file))
      OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = file;
    else
      OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_WORKING + SEPARATION + file;
  } else
    ErrorCode("امتداد الملف غير مقبول : ' " + file + " ' ", &OBJ_CLASS_TOKEN);

  // check file existe
  if (!is_file_exists(OBJ_CLASS_TOKEN.PATH_FULL_SOURCE)) {

    // Try second path
    std::string second_path = argument.input.path; // [/home/folder/]
    second_path.append(file);                      // [test]

    if (is_file_exists(second_path)) { // [/home/folder/test]

      OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = second_path;
    } else if (is_file_exists(second_path +
                              ".alif")) { // [/home/folder/test.alif]

      OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = second_path + ".alif";
    } else {

      // Search in all include path's
      bool found = false;
      for (auto inc_path : argument.input.includes) {

        // std::cout << "Look: |" << inc_path << "| ";

        if (is_file_exists(inc_path + file)) {

          OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = inc_path + file;
          found = true;
          break;
        } else if (is_file_exists(inc_path + file + ".alif")) {

          OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = inc_path + file + ".alif";
          found = true;
          break;
        }
      }

      if (!found)
        ErrorCode("ملف غير موجود : ' " + file + " ' ", &OBJ_CLASS_TOKEN);
    }
  }

  // if (!is_file_exists(OBJ_CLASS_TOKEN.PATH_FULL_SOURCE)) {
  // 	SHOW_FILE_AND_LINE = false;
  // 	ErrorCode("ملف غير موجود : ' " + OBJ_CLASS_TOKEN.PATH_FULL_SOURCE + " '
  // ", &OBJ_CLASS_TOKEN);
  // }

  // ------------------------------------------------------
  // Current File Type
  // ------------------------------------------------------

  OBJ_CLASS_TOKEN.ALIF_SCRIPT_TYPE = target;

  // ------------------------------------------------------
  // ALIF VARIABLES INITIALIZATION
  // ------------------------------------------------------

  // Initialisation
  ALIF_VAR_INITIALIZATION_FOR_NEW_SOURCE_FILE(is_first_file);

  // ------------------------------------------------------
  // Log File Start
  // ------------------------------------------------------

  // if(DEBUG)DEBUG_MESSAGE("----------- DEBUGING START ----------- \n",
  // &OBJ_CLASS_TOKEN);

  // ------------------------------------------------------
  // Read Source file (UTF8 File name)
  // ------------------------------------------------------

  // if (!is_file_exists(OBJ_CLASS_TOKEN.PATH_FULL_SOURCE)) {
  // 	ALIF_ERROR("ERROR [F001]: Could not open " +
  // OBJ_CLASS_TOKEN.PATH_FULL_SOURCE); 	exit(EXIT_FAILURE);
  // }

  boost::nowide::ifstream FILE_STREAM(OBJ_CLASS_TOKEN.PATH_FULL_SOURCE);

  std::string LINE8;

  bool INSIDE_STRING_CPP = false;

  while (getline(FILE_STREAM, LINE8)) {
    // ------------------------------------------------------
    // Ignore blank lines
    // ------------------------------------------------------

    if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") ||
        (LINE8 == "") || (LINE8 == " ")) {
      OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES] = 0;
      OBJ_CLASS_TOKEN.TOTAL_LINES++;
      continue;
    }

    // ------------------------------------------------------
    // Remove bad line-break character
    // Windows: \r\n
    // Linux: \n
    // MacOS: \r
    // ------------------------------------------------------

    size_t LINE_FIX_LAST_POS = LINE8.find_last_not_of("\r\n");

    if (LINE_FIX_LAST_POS != std::string::npos) {
      LINE8.substr(0, LINE_FIX_LAST_POS + 1).swap(LINE8);
    }

    // ------------------------------------------------------
    // Remove extra spaces
    // ------------------------------------------------------

    LINE8 = REMOVE_DOUBLE_SPACE(LINE8, &OBJ_CLASS_TOKEN);

    if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") ||
        (LINE8 == "") || (LINE8 == " ")) {
      OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES] = 0;
      OBJ_CLASS_TOKEN.TOTAL_LINES++;
      continue;
    }

    // Set Real Line
    OBJ_CLASS_TOKEN.REAL_LINE[OBJ_CLASS_TOKEN.TOTAL_LINES] = LINE8;

    // ------------------------------------------------------
    // Char loop
    // ------------------------------------------------------

    int CHAR_NUMBER = 0;
    int LINE_CHAR_TOTAL = 0;
    std::string Char;
    bool INSIDE_STRING = false;

    OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES] = 0;

    LINE_CHAR_TOTAL = CharCount_utf8(LINE8, &OBJ_CLASS_TOKEN);

    while (CHAR_NUMBER < LINE_CHAR_TOTAL) // '<=' is wrong!
    {
      Char = substr_utf8(LINE8, CHAR_NUMBER, 1);
      // ---------------------------------------------

      if (Char == "۰")
        Char = "0";
      else if (Char == "۱")
        Char = "1";
      else if (Char == "۲")
        Char = "2";
      else if (Char == "۳")
        Char = "3";
      else if (Char == "۴")
        Char = "4";
      else if (Char == "۵")
        Char = "5";
      else if (Char == "۶")
        Char = "6";
      else if (Char == "۷")
        Char = "7";
      else if (Char == "۸")
        Char = "8";
      else if (Char == "۹")
        Char = "9";
      else if (Char == "“")
        Char = "\"";
      else if (Char == "”")
        Char = "\"";
      else if (Char == "‘")
        Char = "'";

      // Comments
      // if ( (Char == "\\") && (substr_utf8(LINE8, (CHAR_NUMBER + 1), 1) ==
      // "\\") && !INSIDE_STRING)
      if ((CHAR_NUMBER == 0) && !INSIDE_STRING && (Char == "'")) // '
      {
        goto NEXT_LINE;
      }

      // Macro OS
      /*
      if ((CHAR_NUMBER == 0) && !INSIDE_STRING && (Char == "#") &&
      (substr_utf8(LINE8, 1, 1) == "إذا"))
      {

              if(substr_utf8(LINE8, 2, 1) == "لينكس"){

                      // #إذا لينكس ...

                      #ifdef _WIN32
                              goto NEXT_LINE;
                      #elif  __APPLE__
                              goto NEXT_LINE;
                      #else
                              CHAR_NUMBER = 3; // Point to after : #إذا لينكس
                              Char = substr_utf8(LINE8, CHAR_NUMBER, 1);
                      #endif
              }
              else if(substr_utf8(LINE8, 2, 1) == "ويندوز"){

                      // #إذا ويندوز ...

                      #ifdef _WIN32
                              CHAR_NUMBER = 3; // Point to after : #إذا ويندوز
                              Char = substr_utf8(LINE8, CHAR_NUMBER, 1);
                      #elif  __APPLE__
                              goto NEXT_LINE;
                      #else
                              goto NEXT_LINE;
                      #endif
              }
              else if(substr_utf8(LINE8, 2, 1) == "ماك"){

                      // #إذا ماك ...

                      #ifdef _WIN32
                              goto NEXT_LINE;
                      #elif  __APPLE__
                              CHAR_NUMBER = 3; // Point to after : #إذا ماك
                              Char = substr_utf8(LINE8, CHAR_NUMBER, 1);
                      #else
                              goto NEXT_LINE;
                      #endif
              }
              else {

                      ALIF_ERROR("Alif Lexer error: The Alif macro is invalide
      (" + substr_utf8(LINE8, 2, 1) + ") at line " +
      std::to_string(OBJ_CLASS_TOKEN.TOTAL_LINES));
              }
      }
      */

      // -------------------------------------------------------------

      // _س_

      if ((Char == "_" && (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "س") &&
           (substr_utf8(LINE8, CHAR_NUMBER + 2, 1) == "_")) ||
          // (Char == "_" && (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "ج") &&
          // (substr_utf8(LINE8, CHAR_NUMBER + 2, 1) == "_")) ||
          (Char == "@" &&
           (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) != "@"))) // Skip '@@'
      {
        std::string CompletChar = "";

        if (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "س")
          CompletChar = "_س_";

        if (Char != "@") {
          CHAR_NUMBER = CHAR_NUMBER + 2; // Point to after : _س_
        }

        if (!INSIDE_STRING_CPP) {
          if (Char != "@") {
            ADD_TOKEN(CompletChar, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

            // if(DEBUG)DEBUG_MESSAGE("<NEW start:_س_> " , &OBJ_CLASS_TOKEN); //
            // DEBUG
          } else {
            ADD_TOKEN("@", true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

            // if(DEBUG)DEBUG_MESSAGE("<NEW start:@> " , &OBJ_CLASS_TOKEN); //
            // DEBUG
          }

          INSIDE_STRING_CPP = true;
          LIB_LEXER_CG_BUFER = "";
        } else {
          INSIDE_STRING_CPP = false;
          ADD_TOKEN(LIB_LEXER_CG_BUFER, true, true, CHAR_NUMBER,
                    &OBJ_CLASS_TOKEN);

          if (Char != "@") {
            ADD_TOKEN(CompletChar, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

            // if(DEBUG)DEBUG_MESSAGE("<NEW END:_س_>|" + LIB_LEXER_CG_BUFER + "|
            // " , &OBJ_CLASS_TOKEN); // DEBUG
          } else {
            ADD_TOKEN("@", true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

            // if(DEBUG)DEBUG_MESSAGE("<NEW END:@>|" + LIB_LEXER_CG_BUFER + "| "
            // , &OBJ_CLASS_TOKEN); // DEBUG
          }

          LIB_LEXER_CG_BUFER = "";
        }
      } else if (INSIDE_STRING_CPP) {
        // This char is inside quote _س_

        LIB_LEXER_CG_BUFER.append(Char);
      } else if ((Char == "'" && !INSIDE_STRING && !INSIDE_STRING_CPP) ||
                 (Char == "-" &&
                  substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "-" &&
                  !INSIDE_STRING && !INSIDE_STRING_CPP)) {
        goto NEXT_LINE;
      } else if ((Char == "\"" && substr_utf8(LINE8, (CHAR_NUMBER - 1), 1) !=
                                      "\\") || // [salam"]
                 (Char == "\"" && substr_utf8(LINE8, (CHAR_NUMBER - 2), 2) ==
                                      "\\\\") // [salam\\"]
      ) {
        // [salam"]		-> Yes
        // [salam\"]	-> No
        // [salam\\"]	-> Yes

        /*
        if (INSIDE_STRING_CPP)
        {
                // This char is inside quote _س_ ..... _س_
                // Add this char to this current token
                ADD_TOKEN("\"", false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
        }
        */
        if (INSIDE_STRING) {
          // String End
          // Add char, and new token after
          ADD_TOKEN("\"", false, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
          INSIDE_STRING = false;
        } else {
          // String Start
          // New token, add char
          ADD_TOKEN("\"", true, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
          INSIDE_STRING = true;
        }
      }
      /*
      else if (Char == "@")
      {
              if (INSIDE_STRING)
              {
                      // Inside String
                      // Add char, continue
                      ADD_TOKEN("@", false, false, CHAR_NUMBER,
      &OBJ_CLASS_TOKEN);
              }
              else
              {
                      // New token, add char
                      ADD_TOKEN("@", true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
              }
      }
      */
      else if (INSIDE_STRING) {
        // This char is inside quote " " OR _س_
        // Add this char to this current token
        ADD_TOKEN(Char, false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
      } else if (Char == " ") // Space
      {
        // ignore space!
        // Point to next token
        ADD_TOKEN("", true, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
      } else if (Char == "-") // || Char == "+")
      {
        // If is '-123', must be in the same token
        // else, '-', and '123', in different token

        if (IsValidDigit(substr_utf8(LINE8, CHAR_NUMBER + 1, 1), false,
                         &OBJ_CLASS_TOKEN)) {
          // FORCED Point to next token
          // if(OBJ_CLASS_TOKEN.TOKEN[std::make_pair(OBJ_CLASS_TOKEN.TOTAL_LINES,
          // OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES])] != "")
          //{
          // OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES]++;
          //}

          // Add char '-', next is digit, so no new token
          ADD_TOKEN(Char, true, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
        } else {
          // Add char
          // Point to next token
          ADD_TOKEN(Char, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
        }
      } else if (Char == "@" || // C++ Code
                 Char == "#" || // Hash
                 Char == ":" || // Operator
                 Char == "=" || Char == "+" ||
                 // Char == "-" ||
                 Char == "*" || Char == "&" || // New line
                 Char == "(" || Char == ")" || Char == "[" || Char == "]" ||
                 Char == "{" || Char == "}" || Char == "!" || Char == "<" ||
                 Char == ">" || Char == ";" || // To show error !
                 Char == "،" ||                // اشارة ،
                 Char == ",") {
        // Add char
        // Point to next token
        ADD_TOKEN(Char, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
      } else {
        // Add char
        ADD_TOKEN(Char, false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
      }
      // ======================================================
      CHAR_NUMBER++;
    } // End char loop.

  NEXT_LINE:

    if (INSIDE_STRING_CPP)
      // ADD_TOKEN("\n", false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
      LIB_LEXER_CG_BUFER.append("\n");

    OBJ_CLASS_TOKEN.TOTAL_LINES++;
  } // End Line loop.
  // ------------------------------------------------------

  if (is_first_file) {
    // This is the first file (main.alif)
    // this file, need Tokens Predefinetion

    // ------------------------------------------------------
    // Parser - Tokens Predefinetion
    // ------------------------------------------------------
    if (DEBUG)
      DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION START --- \n",
                    &OBJ_CLASS_TOKEN);
    OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = false;
    // Read list of tokens
    ALIF_PARSER(&OBJ_CLASS_TOKEN);
    if (DEBUG)
      DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION END --- \n\n",
                    &OBJ_CLASS_TOKEN);

    // ------------------------------------------------------
    // Parser - Normal way
    // ------------------------------------------------------
    if (DEBUG)
      DEBUG_MESSAGE(" ----------- DEBUGING START ----------- \n",
                    &OBJ_CLASS_TOKEN);
    OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = true;
    // Read list of tokens
    ALIF_PARSER(&OBJ_CLASS_TOKEN);
    // Check final application code
    FINAL_APPLICATION_CODE_CHECKING(&OBJ_CLASS_TOKEN, is_first_file);
    if (DEBUG)
      DEBUG_MESSAGE("\n ----------- DEBUGING FINISH ------------- \n",
                    &OBJ_CLASS_TOKEN);
  } else {
    // This is a seconde file (mylib.alif)
    // so, check if this file need Tokens Predefinetion

    if (!is_predefined) {
      // This file did not have any Tokens Predefinetion
      // so, lets start one..

      // ------------------------------------------------------
      // Parser - Tokens Predefinetion
      // ------------------------------------------------------
      if (DEBUG)
        DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION START --- \n",
                      &OBJ_CLASS_TOKEN);
      OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = false;
      // Read list of tokens
      ALIF_PARSER(&OBJ_CLASS_TOKEN);
      if (DEBUG)
        DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION END --- \n\n",
                      &OBJ_CLASS_TOKEN);
    } else {
      // This file have already Tokens Predefinetion
      // so, lets parse in normal way..

      // ------------------------------------------------------
      // Parser - Normal way
      // ------------------------------------------------------
      if (DEBUG)
        DEBUG_MESSAGE(" ----------- DEBUGING START ----------- \n",
                      &OBJ_CLASS_TOKEN);
      OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = true;
      // Read list of tokens
      ALIF_PARSER(&OBJ_CLASS_TOKEN);
      // Check final application code
      FINAL_APPLICATION_CODE_CHECKING(&OBJ_CLASS_TOKEN, is_first_file);
      if (DEBUG)
        DEBUG_MESSAGE("\n ----------- DEBUGING FINISH ------------- \n",
                      &OBJ_CLASS_TOKEN);
    }
  }

  FILE_STREAM.close();
}

// Compiler *****************************************************

#ifdef _WIN32

// ------------------------------------------------
// Alif Compiler For Windows
// ------------------------------------------------

void CLEAN() {

  if (DEBUG)
    return;

  std::string CLEAR_CMD;

  if (!THIS_IS_ALIF_C_FILE) {

    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_strip.bat\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
      // cout << endl << "Warning: Can't clean strip Bat temp file. " << endl;
    }

    CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                            "_strip.log\" ");
    if (system(CLEAR_CMD.c_str()) != 0) {
      // cout << endl << "Warning: Can't clean strip log temp file. " << endl;
    }
  }
  CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                          "_resource.bat\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean resource Bat temp file. " << endl;
  }

  CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                          "_resource.log\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean resource log temp file. " << endl;
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
    // cout << endl << "Warning: Can't clean compiled RES temp file. " << endl;
  }

  CLEAR_CMD = std::string("del \"") + PATH_FULL_OBJ + std::string("\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
  }

  CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                          "_compile.log\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean compile log temp file. " << endl;
  }

  CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                          "_link.log\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean link log temp file. " << endl;
  }

  CLEAR_CMD = std::string("del \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
                          "_compile.bat\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean compile Bat temp file. " << endl;
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

  CMD = "@echo off\n"
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

  if (DEBUG)
    LogMessage("Compile script (Windows) : " + CMD + " \n");

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
    exit(EXIT_FAILURE);
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
        "https://github.com/alifcommunity/compiler/issues \n\nAlif Compiler " +
        VERSION + " - Windows\n\nFile : " + PATH_FULL_ALIF +
        "\nError: " + LOG_LINE8_Buffer);
    exit(EXIT_FAILURE);
  }

  // ------------------------------------------------------
  // Resource
  // ------------------------------------------------------

  CMD = "@echo off\n"
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

  CMD = "@echo off\n"
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

  if (DEBUG)
    LogMessage("Link script (Windows) : " + CMD + " \n");

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
    exit(EXIT_FAILURE);
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
        "https://github.com/alifcommunity/compiler/issues \n\nAlif Compiler " +
        VERSION + " - Windows\n\nFile : " + PATH_FULL_ALIF +
        "\nError (Linker): " + LOG_LINE8_Buffer +
        "\nLinker command (Windows): \n" + CMD);
    exit(EXIT_FAILURE);
  }

  // ------------------------------------------------------
  // Striping
  // ------------------------------------------------------

  if (!THIS_IS_ALIF_C_FILE) {

    CMD = "@echo off\n"
          "Set PATH=" +
          PATH_ABSOLUTE +
          "\\gcc\\bin\n"
          "\"" +
          PATH_ABSOLUTE +
          "\\gcc\\bin\\strip.exe\""
          " --strip-all \"" +
          PATH_FULL_BIN + "\" 2> \"" + PATH_TEMP + "\\alifcompiler_" + RANDOM +
          "_strip.log\"";

    FILE_BATCH_PATH = PATH_TEMP + "\\alifcompiler_" + RANDOM + "_strip.bat";

    FILE_BATCH.open(FILE_BATCH_PATH);
    FILE_BATCH << CMD << endl;
    FILE_BATCH.close();

    system(("\"" + FILE_BATCH_PATH + "\"").c_str());

    /*
    FILE_BATCH_PATH = PATH_ABSOLUTE + "\\_alif\\tmp\\" + RAND + "_strip.bat";
    FILE_BATCH.open (FILE_BATCH_PATH);

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
  if (DEBUG)
    return;

  std::string CLEAR_CMD =
      std::string("rm -rf \"") + PATH_FULL_CPP + std::string("\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean CPP temp file. " << endl;
  }

  CLEAR_CMD = std::string("rm -rf \"") + PATH_FULL_OBJ + std::string("\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
  }

  CLEAR_CMD = std::string("rm -rf \"") + PATH_FULL_CPP + std::string(".log\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean compile log temp file. " << endl;
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

void COMPILE_MAC_64BIT() // std::string RAND, std::string PATH_FULL_ICO,
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
    cout << endl << "ERROR [CORE001]: Can't compile generated code. " << endl;
    exit(EXIT_FAILURE);
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

  if (APP_TYPE == "PC_GUI")
    GUI_CMD = " ............. ";

  // clang++ -m64 -mmacosx-version-min=10.9 -o "alifstudio" "alifstudio.o"
  // -L"/Library/Application Support/Aliflang/Alif_Lib_WX/lib" -framework IOKit
  // -framework Carbon -framework Cocoa -framework AudioToolbox -framework
  // System -framework OpenGL -lwx_osx_cocoau_alif_aui-3.1
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
    exit(EXIT_FAILURE);
  }

  // ------------------------------------------------------
  // Striping
  // ------------------------------------------------------

  MAC_CMD = "strip \"" + PATH_FULL_BIN_TMP + "\" 2> \"" + PATH_FULL_BIN_TMP +
            ".strip.log\" ";

  if (system(MAC_CMD.c_str()) != 0) {
    cout << endl << "ERROR [CORE001]: Can't Strip. " << endl;
    exit(EXIT_FAILURE);
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
  if (DEBUG)
    return;

  std::string CLEAR_CMD =
      std::string("rm -rf \"") + PATH_FULL_CPP + std::string("\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean CPP temp file. " << endl;
  }

  CLEAR_CMD = std::string("rm -rf \"") + PATH_FULL_OBJ + std::string("\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean OBJ temp file. " << endl;
  }

  CLEAR_CMD = std::string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" + RANDOM +
                          "_compile.log\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean compile log temp file. " << endl;
  }

  CLEAR_CMD = std::string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" + RANDOM +
                          "_link.log\" ");
  if (system(CLEAR_CMD.c_str()) != 0) {
    // cout << endl << "Warning: Can't clean link log temp file. " << endl;
  }

  CLEAR_CMD = std::string("rm -rf \"" + PATH_TEMP + "/alifcompiler_" + RANDOM +
                          "_strip.log\" ");
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

  // Compiling...
  // g++ -c -o "Bin/Linux_64/alifstudio.o" -Wno-undef -Wno-unused-parameter
  // -D__WXGTK__ -Wno-ctor-dtor-privacy -Woverloaded-virtual
  // -D_FILE_OFFSET_BITS=64 -DwxDEBUG_LEVEL=0 -I"/usr/local/include/aliflibwx"
  // -I"Src" -pthread -m64 -DG_DISABLE_CAST_CHECKS $cmd -m64 -std=c++11
  // "Src/alifstudio.cpp"

  // -std=gnu++17 -m64 -finput-charset=utf-8 -O3 -pthread -DNDEBUG
  // -fvisibility=hidden -lfto -fno-fat-lto-objects -DBOOST_ALL_NO_LIB
  // -DBOOST_FILESYSTEM_DYN_LINK

  // LINUX_CMD = "g++ -Wall -std=gnu++17 -m64 -finput-charset=utf-8 -O3 -pthread
  // -DNDEBUG -fvisibility=hidden -c -o \"" + PATH_FULL_OBJ + "\" "
  // " -I\"/usr/local/include\" "		// Standard "
  // -I\"/usr/local/lib/aliflib\" "	// Alif lib 			" \"" +
  // PATH_FULL_CPP
  // + "\"
  // " 			" 2> \"" + PATH_TEMP + "/alifcompiler_" + RANDOM +
  // "_compile.log\" ";

  LINUX_CMD = "g++ -c -o \"" + PATH_FULL_OBJ + "\" \"" + PATH_FULL_CPP +
              "\" -I \"" XSTR(INSTALL_PREFIX) "/lib/aliflib\" " +
              Compile_ExtraCompile + " 2> \"" + PATH_TEMP + "/alifcompiler_" +
              RANDOM + "_compile.log\" ";

  if (DEBUG)
    LogMessage("Compile command (Linux) : " + LINUX_CMD + " \n");
  // std::cout << "\n\n--------------\nCompile: |" << LINUX_CMD <<
  // "|\n--------------\n";

  // LINUX_CMD = "g++ -c -o \"" + PATH_FULL_OBJ + "\" -Wno-undef
  // -Wno-unused-parameter "
  // 				"-D__WXGTK__ -Wno-ctor-dtor-privacy
  // -Woverloaded-virtual -D_FILE_OFFSET_BITS=64 "
  // 				"-DwxDEBUG_LEVEL=0
  // -I\"/usr/local/include/aliflibwx\" -pthread -m64 -DG_DISABLE_CAST_CHECKS
  // -O2 -m64 -std=c++11 " + Compile_ExtraCompile + " \"" +
  // PATH_FULL_CPP + "\" 2> \"" + PATH_TEMP +
  // "/alifcompiler_" + RANDOM
  // + "_compile.log\" ";

  system(LINUX_CMD.c_str());

  // ----------------------
  // Read Log File
  // ----------------------

  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  LOG_PATH = PATH_TEMP + "/alifcompiler_" + RANDOM + "_compile.log";

  boost::nowide::ifstream FILE_STREAM_COMPILER(LOG_PATH);

  if (!FILE_STREAM_COMPILER.is_open()) {
    ALIF_ERROR("ERROR: Could not open compile log file : " + LOG_PATH);
    exit(EXIT_FAILURE);
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
        "https://github.com/alifcommunity/compiler/issues \n\nAlif Compiler " +
        VERSION + " - Linux\n\nFile : " + PATH_FULL_ALIF +
        "\nError: " + LOG_LINE8_Buffer + "\nCompile command: " + LINUX_CMD);
    exit(EXIT_FAILURE);
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

  // g++ -o "Bin/Linux_64/alifstudio" "Bin/Linux_64/alifstudio.o"
  // -L"/usr/local/lib/aliflibwx" -pthread -m64 -static-libgcc -static-libstdc++
  // -lwx_gtk2u_alif_webview-3.1 -lwx_gtk2u_alif_propgrid-3.1
  // -lwx_gtk2u_alif_aui-3.1 -lwebkitgtk-1.0 -lgtk-x11-2.0 -lgdk-x11-2.0
  // -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lpangoft2-1.0
  // -lpango-1.0 -lfontconfig -lfreetype -lsoup-2.4 -lgio-2.0 -lgobject-2.0
  // -ljavascriptcoregtk-1.0 -lglib-2.0 -lwx_gtk2u_alif_stc-3.1
  // -lwx_gtk2u_alif_core-3.1 -lwx_baseu_alif-3.1 -lwxscintilla_alif-3.1
  // -lwxtiff_alif-3.1 -lwxjpeg_alif-3.1 -lwxpng_alif-3.1  -lgtk-x11-2.0
  // -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0
  // -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lfontconfig -lfreetype
  // -lgthread-2.0 -pthread -lglib-2.0 -lX11 -lXxf86vm -lSM -lgtk-x11-2.0
  // -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0
  // -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
  // -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
  // -lwxzlib_alif-3.1 -lwxregexu_alif-3.1 -lwxexpat_alif-3.1 -ldl -lm  -ldl -lm
  // $cmd

  // -L\"/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu" -lpython3.5

  // LINUX_CMD = "g++ -o \"" + PATH_FULL_BIN + "\" \"" + PATH_FULL_OBJ + "\"  "
  // 				"-L\"/usr/local/lib/aliflibwx\"
  // -lwx_gtk2u_alif_webview-3.1 -lwx_gtk2u_alif_propgrid-3.1
  // -lwx_gtk2u_alif_aui-3.1 -lwebkitgtk-1.0 -lgtk-x11-2.0 -lgdk-x11-2.0
  // -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lpangoft2-1.0
  // -lpango-1.0 -lfontconfig -lfreetype -lsoup-2.4 -lgio-2.0 -lgobject-2.0
  // -ljavascriptcoregtk-1.0 -lglib-2.0 -lwx_gtk2u_alif_stc-3.1
  // -lwx_gtk2u_alif_core-3.1 -lwx_baseu_alif-3.1 -lwxscintilla_alif-3.1
  // -lwxtiff_alif-3.1 -lwxjpeg_alif-3.1 -lwxpng_alif-3.1 -lgtk-x11-2.0
  // -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0
  // -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lfontconfig -lfreetype
  // -lgthread-2.0 -pthread -lglib-2.0 -lX11 -lXxf86vm -lSM -lgtk-x11-2.0
  // -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0
  // -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
  // -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype
  // -lwxzlib_alif-3.1 -lwxregexu_alif-3.1 -lwxexpat_alif-3.1 "
  // 				"-pthread -m64 -static-libgcc -static-libstdc++
  // -ldl -lm -ldl -lm -O2 -L\"/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu\"
  // " + Compile_ExtraLink;

  // LINUX_CMD =	"g++ -Os -m64 -finput-charset=utf-8 -pthread -o \"" +
  // PATH_FULL_BIN + "\" \"" + PATH_FULL_OBJ + "\" "
  // 			"-L\"/usr/local/lib/aliflib\" -lwebui
  // -lboost_filesystem";

  LINUX_CMD = "g++ -o \"" + PATH_FULL_BIN + "\" \"" + PATH_FULL_OBJ +
              "\" -L\"" XSTR(INSTALL_PREFIX) "/lib/aliflib\" " +
              Compile_ExtraLink + " "; // TODO: Add 2 > log

  if (DEBUG)
    LogMessage("Link command (Linux) : " + LINUX_CMD + " \n");

  // std::cout << "\n\n--------------\nLink: |" << LINUX_CMD <<
  // "|\n--------------\n";
  // g++
  // -Os -m64 -finput-charset=utf-8 -pthread -lboost_filesystem -lboost_thread
  // -Os -m64 -finput-charset=utf-8 -pthread -lboost_filesystem -lboost_thread
  // -L"/usr/local/lib/aliflib" -o "test" "/tmp/alifcompiler_1007.o"

  if (system(LINUX_CMD.c_str()) != 0) {
    // cout << endl << "Bug: Can't Link generated code. " << endl;
    // exit(EXIT_FAILURE);

    if (DEBUG)
      cout << endl << "Debuging: " << LINUX_CMD.c_str() << endl;

    ALIF_ERROR(
        "Please report this error to the Alif Community: "
        "https://github.com/alifcommunity/compiler/issues \nAlif Compiler " +
        VERSION + " - Linux\n\nFile : " + PATH_FULL_ALIF +
        "\nError: Linking Librarys error.\nLink command: " + LINUX_CMD);
    exit(EXIT_FAILURE);
  }

  // ------------------------------------------------------
  // Striping
  // ------------------------------------------------------

  LINUX_CMD = " strip --strip-all --strip-unneeded \"" + PATH_FULL_BIN + "\" ";

  if (system(LINUX_CMD.c_str()) != 0) {

    // Striping error..
  }

  // ------------------------------------------------------

  // Debugging Mode :

  // gdb -silent -ex 'set logging redirect off' -ex 'printf "\nAlif Compiler 1.0
  // - GNU Debugger\n\n"' -ex 'run' -ex 'printf "\nBacktrace : \n"' -ex 'bt' -ex
  // 'printf "\nEnd.\n"' -ex 'detach' -ex 'quit' ./AlifStudio > AlifDebug.txt

  CLEAN();
}

#endif

// Tools ********************************************************

void set_input_file(std::string file) {

  boost::filesystem::path p(file);

  std::string p_buf = p.parent_path().string();
  if (p_buf.empty())
    p_buf = boost::filesystem::current_path().string();

  argument.input.path = p_buf + settings.os.path_sep;
  argument.input.filename = p.stem().string();
  argument.input.extension = p.extension().string();

  // Check root path
  if (argument.input.path == settings.os.path_sep)
    argument.input.path = "";

  // Check input extention
  if (argument.input.extension != ".alif")
    err("Unsupported file extention.\n"
        "Extention: " +
        argument.input.extension +
        "\n"
        "File: " +
        argument.input.path + argument.input.filename +
        argument.input.extension);

  argument.input.fullpath =
      argument.input.path + argument.input.filename + argument.input.extension;
}

void set_output_file(std::string file) {

  boost::filesystem::path p(file);

  argument.output.path = p.parent_path().string() + settings.os.path_sep;
  argument.output.filename = p.stem().string();
  argument.output.extension = p.extension().string();

  // Check root path
  if (argument.output.path == settings.os.path_sep)
    argument.output.path = "";

  // Check output extention
  if (argument.output.extension == "")
    argument.output.extension = settings.os.exe_ext;
  else if (argument.output.extension != settings.os.exe_ext)
    err("Unsupported output extention.\n"
        "Extention: " +
        argument.output.extension +
        "\n"
        "File: " +
        argument.output.path + argument.output.filename +
        argument.output.extension);

  argument.output.fullpath = argument.output.path + argument.output.filename +
                             argument.output.extension;
}

void set_log_file(std::string file) {

  boost::filesystem::path p(file);

  argument.log.path = p.parent_path().string() + settings.os.path_sep;
  argument.log.filename = p.stem().string();
  argument.log.extension = p.extension().string();

  // Check root path
  if (argument.log.path == settings.os.path_sep)
    argument.log.path = "";

  // Check log extention
  // if(argument.log.extension == "")
  // 	argument.log.extension = ".log";
  // if(argument.log.extension == "")
  // 	argument.log.extension = settings.os.exe_ext;
  // else if(argument.log.extension != settings.os.exe_ext)
  // err("Unsupported log extention.\n"
  // 	"Extention: " + argument.log.extension + "\n"
  // 	"File: " + argument.log.path +
  // 	argument.outplogut.filename + argument.log.extension);

  // Check log
  if (argument.log.filename == "")
    err("Log file name is empty.");

  argument.log.fullpath =
      argument.log.path + argument.log.filename + argument.log.extension;

  // Empty file
  boost::nowide::ofstream ofile(argument.log.fullpath);
  if (ofile) {
    ofile << "";
    ofile.close();
  }
}

static const unsigned char base64_table[65] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned char *_base64_encode(const unsigned char *src, size_t len,
                              size_t *out_len) {

  unsigned char *out, *pos;
  const unsigned char *end, *in;
  size_t olen;
  int line_len;

  olen = len * 4 / 3 + 4; /* 3-byte blocks to 4-byte */
  olen += olen / 72;      /* line feeds */
  olen++;                 /* nul termination */
  if (olen < len)
    return NULL; /* integer overflow */
  out = (unsigned char *)malloc(olen);
  if (out == NULL)
    return NULL;

  end = src + len;
  in = src;
  pos = out;
  line_len = 0;
  while (end - in >= 3) {
    *pos++ = base64_table[in[0] >> 2];
    *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
    *pos++ = base64_table[in[2] & 0x3f];
    in += 3;
    line_len += 4;
    if (line_len >= 72) {
      *pos++ = '\n';
      line_len = 0;
    }
  }

  if (end - in) {
    *pos++ = base64_table[in[0] >> 2];
    if (end - in == 1) {
      *pos++ = base64_table[(in[0] & 0x03) << 4];
      *pos++ = '=';
    } else {
      *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      *pos++ = base64_table[(in[1] & 0x0f) << 2];
    }
    *pos++ = '=';
    line_len += 4;
  }

  if (line_len)
    *pos++ = '\n';

  *pos = '\0';
  if (out_len)
    *out_len = pos - out;
  return out;
}

unsigned char *_base64_decode(const unsigned char *src, size_t len,
                              size_t *out_len) {

  unsigned char dtable[256], *out, *pos, block[4], tmp;
  size_t i, count, olen;
  int pad = 0;

  memset(dtable, 0x80, 256);
  for (i = 0; i < sizeof(base64_table) - 1; i++)
    dtable[base64_table[i]] = (unsigned char)i;
  dtable['='] = 0;

  count = 0;
  for (i = 0; i < len; i++) {
    if (dtable[src[i]] != 0x80)
      count++;
  }

  if (count == 0 || count % 4)
    return NULL;

  olen = count / 4 * 3;
  pos = out = (unsigned char *)malloc(olen);
  if (out == NULL)
    return NULL;

  count = 0;
  for (i = 0; i < len; i++) {
    tmp = dtable[src[i]];
    if (tmp == 0x80)
      continue;

    if (src[i] == '=')
      pad++;
    block[count] = tmp;
    count++;
    if (count == 4) {
      *pos++ = (block[0] << 2) | (block[1] >> 4);
      *pos++ = (block[1] << 4) | (block[2] >> 2);
      *pos++ = (block[2] << 6) | block[3];
      count = 0;
      if (pad) {
        if (pad == 1)
          pos--;
        else if (pad == 2)
          pos -= 2;
        else {
          /* Invalid padding */
          if (out)
            free(out);
          return NULL;
        }
        break;
      }
    }
  }

  *out_len = pos - out;
  return out;
}

std::string base64_encode(std::string data) {

  // Encode

  std::string buf = "";
  size_t len = 0;
  unsigned char *pbuf = NULL;

  pbuf =
      _base64_encode((const unsigned char *)data.c_str(), data.length(), &len);
  buf = reinterpret_cast<char *>(pbuf);
  // alifcore_trim(buf);

  // alifcore_free(pbuf);

  // std::cout << "Encode: [" << data << "] -> [" << buf << "] (" << len << "
  // Bytes) \n";

  return buf;
}

std::string base64_decode(std::string data) {

  // Decode

  std::string buf = "";
  size_t len = 0;
  unsigned char *pbuf = NULL;

  pbuf =
      _base64_decode((const unsigned char *)data.c_str(), data.length(), &len);
  buf = reinterpret_cast<char *>(pbuf);
  // alifcore_trim(buf);

  // alifcore_free(pbuf);

  // std::cout << "Decode: [" << data << "] -> [" << buf << "] (" << len << "
  // Bytes) \n";

  return buf;
}

// Main *********************************************************

// void alif() {
// 	// Check Files extention
// 	if (argument.input.extension != ".alif"){
// 		err("Unsupported file extention.\n"
// 			"Extention: " + argument.input.extension + "\n"
// 			"File: " + argument.input.fullpath);
// 	}
// 	boost::nowide::ifstream ifile(argument.input.fullpath);
// 	if(!ifile.is_open()) {
// 		err("Can't open input source file.\n"
// 			"File: " + argument.input.fullpath);
// 	}
// 	std::cout << "--> File opened: "<< argument.input.fullpath << '\n';
// 	std::string line;
// 	while(getline(ifile, line)) {
// 		std::cout << "Line: " << line << '\n';
// 	}
// }

void alif() {

  // Check Files extention
  if (argument.input.extension != ".alif")
    err("Unsupported file extention.\n"
        "Extention: " +
        argument.input.extension +
        "\n"
        "File: " +
        argument.input.fullpath);

  // Check output
  if (argument.output.fullpath == "")
    set_output_file(argument.input.path + argument.input.filename +
                    settings.os.exe_ext);

  // Check log
  if (argument.log.fullpath == "") {

    if (ERROR_AS_JSON)
      set_log_file(argument.input.path + argument.input.filename +
                   argument.input.extension + ".json");
    else
      set_log_file(argument.input.path + argument.input.filename +
                   argument.input.extension + ".log");
  }

  // Beta test log
  // cout << "argument.input.path: " << argument.input.path << endl;
  // cout << "argument.input.filename: " << argument.input.filename << endl;
  // cout << "argument.input.extension: " << argument.input.extension << endl;
  // cout << "argument.input.fullpath: " << argument.input.fullpath << endl;
  // cout << "argument.output.path: " << argument.output.path << endl;
  // cout << "argument.output.filename: " << argument.output.filename << endl;
  // cout << "argument.output.extension: " << argument.output.extension << endl;
  // cout << "argument.output.fullpath: " << argument.output.fullpath << endl;

  // Temporary Immigrate setting from Alif v3 style to Alif v2.
  PATH_FULL_ALIF = argument.input.fullpath;
  PATH_FULL_BIN = argument.output.fullpath;
  // PATH_FULL_LOG	= argument.input.fullpath + ".log";
  PATH_FULL_LOG = argument.log.fullpath;

  // Check Setup Installation
  if (!CHECK_SETUP()) {

    err("Please re-install Alif Compiler, or download the latest version from "
        "\n\nwww.aliflang.org");
    exit(EXIT_FAILURE);
  }

// --- Read Core Script --------------------
#ifdef _WIN32
  std::string core_path = PATH_ABSOLUTE + "\\aliflib\\alifcore.cc";
#else
  std::string core_path = XSTR(INSTALL_PREFIX) "/lib/aliflib/alifcore.cc";
#endif
  boost::nowide::ifstream alifcore_if(core_path);
  stringstream alifcore_ss;
  alifcore_ss << alifcore_if.rdbuf();
  code_core = alifcore_ss.str();
  if (code_core == "") {

    err("The core file is missing: [" + core_path +
        "]\nPlease re-install Alif Compiler, or download the latest version "
        "from http://www.aliflang.org");
    exit(EXIT_FAILURE);
  }

  // Alif Lib Setting (to fix arab file name problem)
  // ALIF_LIB_SETTING();

  // CPP CODE INITIALIZATION
  // Set basic value to variables
  // to run a minimal app (Title, With, Heint, WebUI..)
  CG_INITIALIZATION();

  // ALIF VARIABLES INITIALIZATION
  // Set Initialisation of general alif variables..
  // Main window set, alif flag, inside fun or win..
  ALIF_VAR_INITIALIZATION_FOR_NEW_SOURCE_FILE(true);

  // The Alif Standard Library
  if (DEBUG)
    LogMessage("\n ----------- The Alif Standard Library ------------- \n");
  AlifLexerParser("alifstandardlib", "ALIFLIB", false,
                  false); // Global Tokens Predefinition.
  AlifLexerParser("alifstandardlib", "ALIFLIB", false,
                  true); // Full Syntaxt Checking.
  if (DEBUG)
    LogMessage("\n ----------- Standard Library FINISH --------------- \n");

  // Lexer / Parcer for User source code
  // Read source file..
  // LEXER	:	Create a list of tokens
  // PARSER	:	Read list of tokens, Generate a C++ code
  AlifLexerParser(PATH_FULL_ALIF, "ALIF", true, false);

// Check if App Runing
#ifdef _WIN32
  std::string CLEAR_BIN_CMD = std::string("@echo off & del /s \"") +
                              PATH_FULL_BIN + std::string("\" >nul 2>&1");
  if (system(CLEAR_BIN_CMD.c_str()) != 0)
    cout << endl
         << "Warning: Can't execut Windows remove EXE output command" << endl;
    // boost::nowide::remove(PATH_FULL_BIN);
    // std::remove(PATH_FULL_BIN);
#elif __APPLE__
  std::string CLEAR_BIN_CMD =
      std::string("rm -rf \"") + PATH_FULL_BIN + std::string("\" 2> /dev/null");
  if (system(CLEAR_BIN_CMD.c_str()) != 0)
    cout << endl
         << "Warning: Can't execut macOS remove App output command" << endl;
#else
  std::string CLEAR_BIN_CMD =
      std::string("rm -f \"") + PATH_FULL_BIN + std::string("\" 2> /dev/null");
  if (system(CLEAR_BIN_CMD.c_str()) != 0)
    cout << endl
         << "Warning: Can't execut Linux remove Bin output command" << endl;
#endif

#ifdef __APPLE__
  if (is_file_exists(PATH_FULL_BIN + "/Contents/MacOS/alif"))
#else
  if (is_file_exists(PATH_FULL_BIN))
#endif
  {
    ALIF_ERROR("\nأسف، لم تنجح عملية مسح هدا الملف، جرب غلق التطبيق: " +
               PATH_FULL_BIN);
    exit(EXIT_FAILURE);
  }

  // Save debug message
  if (!THIS_IS_ALIF_C_FILE)
    if (DEBUG)
      PATH_FULL_LOG_SAVE();

  // ++++++++
  // Debug
  // PATH_FULL_LOG_SAVE();
  // ALIF_ERROR("Compiling... ");
  // exit(EXIT_SUCCESS);
  // ++++++++

  if (SyntaxOnly)
    return; // This is syntax only checking no compile needed

#ifdef _WIN32

  if (!THIS_IS_ALIF_C_FILE) {

    // Get GENERATED C++ CODE

    boost::nowide::ofstream FILE_CPP;
    FILE_CPP.open(PATH_FULL_CPP); // .c_str());

    if (!FILE_CPP.is_open()) {

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
  boost::nowide::ofstream FILE_RESOURCE;
  FILE_RESOURCE.open(PATH_FULL_RC); // .c_str());
  if (!FILE_RESOURCE.is_open()) {

    ALIF_ERROR("ERROR: Could not create RC temp files. ");
    exit(EXIT_FAILURE);
  }
  FILE_RESOURCE << "aaaa ICON \"" + PATH_FULL_ICO + "\"";
  FILE_RESOURCE.close();

  // Compile
  compile_win64();

#elif __APPLE__
  // app icon
  if (!PATH_FULL_ICO.empty())
    boost::replace_all(PATH_FULL_ICO, "\\", "\\\\");

  if (!THIS_IS_ALIF_C_FILE) {

    // GENERATE C++ CODE

    boost::nowide::ofstream FILE_CPP;
    FILE_CPP.open(PATH_FULL_CPP); // .c_str());
    if (!FILE_CPP.is_open()) {

      ALIF_ERROR("ERROR [F004]: Could not create CPP temp files. ");
      exit(EXIT_FAILURE);
    }
    FILE_CPP << CODE_GENERATOR_GET_PC_GUI();
    FILE_CPP.close();
  }

  // Compile
  COMPILE_MAC_64BIT();

#else // Linux

  if (!THIS_IS_ALIF_C_FILE) {

    // GENERATE C++ CODE

    boost::nowide::ofstream FILE_CPP;
    FILE_CPP.open(PATH_FULL_CPP); // .c_str());
    if (!FILE_CPP.is_open()) {

      ALIF_ERROR("ERROR [F004]: Could not create CPP temp files. ");
      exit(EXIT_FAILURE);
    }
    FILE_CPP << CODE_GENERATOR_GET_PC_GUI();
    FILE_CPP.close();
  }

  // Compile
  compile_linux64();

#endif
}

// Entry point **************************************************

#ifdef _WIN32
int wmain(int argc, wchar_t *argv[])
// int wmain(int argc, char **argv)
#elif __APPLE__
int main(int argc, char **argv)
#else
int main(int argc, char **argv)
#endif
{
#ifdef _WIN32
  // Create and install global locale
  std::locale::global(boost::locale::generator().generate(""));
#endif

  // Make boost.filesystem use it
  boost::filesystem::path::imbue(std::locale());

  try {

    // Argument parsing
    boost::program_options::options_description desc("Allowed options");
    desc.add_options()("h", "Produce help message")(
        "v", "Print compiler version")("debug",
                                       "Add debugging info to the log file")(
        "debug-screen",
        "Add debugging info to the log file, And print it on the screen")(
        "syntax-only", "Check only the Alif code syntax (no compile)")(
        "o", boost::program_options::value<std::string>(), "Set output file")(
        "log", boost::program_options::value<std::string>(), "Set log file")(
        "log-json", boost::program_options::value<std::string>(),
        "Set log file as JSON format")(
        "log-json-base64", boost::program_options::value<std::string>(),
        "Like 'log-json' but data is base64 encoded")(
        "input", boost::program_options::value<vector<std::string>>(),
        "Set input file (only one)")(
        "include-path,I", boost::program_options::value<vector<std::string>>(),
        "Add include path")(
        "clib,L", boost::program_options::value<vector<std::string>>(),
        "Add extra C/C++ libs");

    boost::program_options::positional_options_description optional_desc;
    optional_desc.add("input", -1);

    boost::program_options::variables_map vm;
#ifdef _WIN32
    boost::program_options::store(
        boost::program_options::wcommand_line_parser(argc, argv)
            .options(desc)
            .positional(optional_desc)
            .run(),
        vm);
#elif __APPLE__
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(desc)
            .positional(optional_desc)
            .run(),
        vm);
#else
    boost::program_options::store(
        boost::program_options::command_line_parser(argc, argv)
            .options(desc)
            .positional(optional_desc)
            .run(),
        vm);
#endif
    boost::program_options::notify(vm);

    // Version
    if (vm.count("v")) {

      cout << "Alif Compiler v" << ALIF_VERSION << endl;
      return 0;
    }

    // Help
    if (vm.count("h") || !vm.count("input")) {

      cout << "Alif Compiler v" << ALIF_VERSION << endl;
      cout << "Copyright (C)2021 DRAGA Hassan - www.aliflang.org" << endl;
      cout << "This is free software; see the source for copying conditions.  "
              "There is NO"
           << endl;
      cout << "warranty; not even for MERCHANTABILITY or FITNESS FOR A "
              "PARTICULAR PURPOSE."
           << endl
           << endl;
      cout << desc << "\n";
      return 0;
    }

    // Input file
    if (vm.count("input")) {

      if (vm["input"].as<vector<std::string>>().size() > 1) {

        cout << "Error: You need to set only one input source file." << endl;
        cout << "Input files: ";

        for (auto const &i : vm["input"].as<vector<std::string>>())
          std::cout << i << ' ';

        return 1;
      }

      set_input_file(vm["input"].as<vector<std::string>>()[0]);
    } else {

      cout << "Error: No input source file set." << endl;
      return 1;
    }

    // Output
    if (vm.count("o")) {

      set_output_file(vm["o"].as<std::string>());
    }

    // Log
    if (vm.count("log")) {

      ERROR_AS_JSON = false;
      set_log_file(vm["log"].as<std::string>());
    }

    // Log JSON
    if (vm.count("log-json")) {

      ERROR_AS_JSON = true;
      ERROR_AS_JSON64 = false;
      set_log_file(vm["log-json"].as<std::string>());
    }

    // Log JSON b64
    if (vm.count("log-json-base64")) {

      ERROR_AS_JSON = true;
      ERROR_AS_JSON64 = true;
      set_log_file(vm["log-json-base64"].as<std::string>());
    }

    // debug
    if (vm.count("debug")) {

      DEBUG = true;
    }

    // Print debug on the screen
    if (vm.count("debug-screen")) {

      DEBUG = true;
      DEBUG_PRINT_ON_SCREEN = true;
    }

    // Syntax only
    if (vm.count("syntax-only")) {

      SyntaxOnly = true;
    }

    // Include path
    if (vm.count("include-path")) {

      for (auto const &i : vm["include-path"].as<vector<std::string>>())
        argument.input.includes.push_back(i + settings.os.path_sep);
    }

    // Extra lib (linker)
    if (vm.count("clib")) {

      for (auto const &i : vm["clib"].as<vector<std::string>>())
        add_extra_arg_to_linker("-l" + i + " ");
    }
  } catch (exception &e) {

    cerr << "Error: " << e.what() << "\n";
    return 1;
  } catch (...) {

    cerr << "Exception: Unknown type.\n";
    return 1;
  }

  // Start parsing
  alif();

  // Al Hamdo li ALLAH =)
  exit(EXIT_SUCCESS);
}

/*
int wmain(int argc, char **argv)
{
        boost::nowide::args a(argc,argv); // Fix arguments - make them UTF-8
        boost::nowide::ifstream f(argv[1]); // argv[1] - is UTF-8

        if(!f) {
                // the console can display UTF-8
                boost::nowide::cerr << "Can't open " << argv[1] << std::endl;
                return 1;
        }
        int total_lines = 0;
        while(f) {
                if(f.get() == '\n')
                        total_lines++;
        }
        f.close();
        // the console can display UTF-8
        boost::nowide::cout << "File " << argv[1] << " has " << total_lines << "
lines" << std::endl; return 0;
}
*/
