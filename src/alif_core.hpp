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

// Global *******************************************************

// Alif v3

void core_add_object(string ns, string cl, string fn, string ob){
    
    std::map<
        std::pair<
            std::pair<std::string, std::string>, 
            std::pair<std::string, std::string>
        >,
        std::string
    > t;
    t[make_pair(make_pair("a", "a"), make_pair("a", "a"))] = "A";
    cout << t[make_pair(make_pair("a", "a"), make_pair("a", "a"))];
}

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

bool AlifFailure = false;

void AlifExitFailure(){

  AlifFailure = true;
  exit(EXIT_FAILURE);
}

// دالة
bool IsInsideFunction = false;
std::string TheFunction;
std::string TheFunctionType;
std::map<std::pair<std::string, std::string>, std::string> RETURN_FUN; // abc[win][fun] = return is ok ? "OK", "IF"

// -- Alif v2 (Old) ------------------------------------------

using namespace std;
bool DEBUG = false;
bool SyntaxOnly = false;
static const std::string VERSION = ALIF_VERSION;

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
        REAL_LINE[2048];  // The real Alif script line, without toknonisation !
    std::map<std::pair<int, int>, int>
        REAL_TOKEN_POSITION;  // The real Alif script token position in real
                              // line ! [Line Number, Token Number] = "real char
                              // number"

    bool TOKENS_PREDEFINED = false;

    std::map<std::pair<int, int>, std::string>
        TOKEN;  // TOKEN [Line Number, Token Number] = "token"

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
    ALIF_FLAG_FILE;  // alif_flag[full_file_path] = true or false
static std::map<std::string, std::string>
    ALIF_FLAG_AT_LINE_FILE;  // alif_flag[full_file_path] = at_line

// ----------------------------------
// صنف
// ----------------------------------

static std::map<std::string, bool>
    CLASS_CONSTRICTOR_FUN_IS_SET;  // abc[class] = main_func true or false
static std::map<std::string, std::string>
    CLASS_CONSTRICTOR_FUN_AT_LINE;  // abc[class] = main_func_at_line
static std::map<std::string, std::string>
    CLASS_CONSTRICTOR_FUN_ARG_TOTAL;  // abc[class] = total args
static std::map<std::pair<std::string, int>, std::string>
    CLASS_CONSTRICTOR_FUN_ARG_TYPE;  // abc[class][arg number] = arg type

static bool IsInsideClass = false;
static std::string TheClass;
static std::map<std::string, bool>
    CLASS_IS_SET;  // abc['class name'] = already set ?
static std::map<std::string, std::string>
    CLASS_AT_LINE;  // abc['class name'] = class at line

static std::map<std::pair<std::string, std::string>, bool>
    CLASS_FUN_IS_SET;  // abc[class][fun] = class fun already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_FUN_AT_LINE;  // abc[class][fun] = class fun line number
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_FUN_TYPE;  // abc[class][fun] = class fun type
static std::map<std::pair<std::string, std::string>, bool>
    CLASS_FUN_PRIVATE;  // abc[class][fun] = class fun private ?
static std::map<std::pair<std::string, std::string>, int>
    CLASS_FUN_ARG_TOTAL;  // abc[class][fun] = global total args
static std::map<std::pair<std::string, int>, std::string>
    CLASS_FUN_ARG_TYPE;  // abc[class + fun][arg number] = arg type

static std::map<std::pair<std::string, std::string>, bool>
    CLASS_G_VAR_IS_SET;  // abc[class][var] = global var already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_G_VAR_AT_LINE;  // abc[class][var] = global var line number
static std::map<std::pair<std::string, std::string>, std::string>
    CLASS_G_VAR_TYPE;  // abc[class][var] = global var type
static std::map<std::pair<std::string, std::string>, bool>
    CLASS_G_VAR_PRIVATE;  // abc[class][var] = global private ?
static std::map<std::pair<std::string, std::string>, bool>
    CLASS_G_VAR_IS_CONST;  // abc[class][var] = global var Constant
static std::map<std::pair<std::string, std::string>, bool> CLASS_G_VAR_IS_ARRAY;

// ----------------------------------
// كائن
// ----------------------------------

static std::map<std::pair<std::string, std::string>, bool>
    OBJ_IS_SET;  // abc[namespace + fun][obj] = already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    OBJ_AT_LINE;  // abc[namespace + fun][obj] = line number
static std::map<std::pair<std::string, std::string>, std::string>
    OBJ_CLASS;  // abc[namespace + fun][obj] = base class
static std::map<std::string, std::string>
    OBJ_GLOBAL_DECLARATION;  // abc[class] = Obj1; Obj2;...

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
    namespace_is_set;  // abc['namespace_name'] = already set ?
static std::map<std::string, std::string>
    namespace_at_line;  // abc['namespace_name'] = func_at_line
// static std::map<std::string, bool>		WIN_IS_WEB; 	//
// abc['namespace_name'] = Web UI ?

// ----------------------------------
// دالة
// ----------------------------------




static bool IS_CONTROL_FUNCTION_NAME = false;

// ----------------------------------
// دالة رئيسية
// ----------------------------------

static std::map<std::string, bool>
    MAIN_FUN_IS_SET;  // abc['namespace_name'] = main_func true or false
static std::map<std::string, std::string>
    MAIN_FUN_AT_LINE;  // abc['namespace_name'] = main_func_at_line

// ----------------------------------
// دالة
// ----------------------------------

static std::map<std::string, bool>
    G_FUN_IS_SET;  // abc[gfun] = global fun already set ?
static std::map<std::string, std::string>
    G_FUN_AT_LINE;  // abc[gfun] = global fun line number
static std::map<std::string, std::string>
    G_FUN_TYPE;  // abc[gfun] = global fun type
static std::map<std::string, int>
    G_FUN_ARG_TOTAL;  // abc[gfun] = global total args
static std::map<std::pair<std::string, int>, std::string>
    G_FUN_ARG_TYPE;  // abc[gfun][arg number] = arg type
int Global_TotalFucntion = 0;
static std::map<int, std::string>
    Global_FunctionNames;  // abc[global fun number] = func name

static std::map<std::pair<std::string, std::string>, bool>
    L_FUN_IS_SET;  // abc[namespace_name][fun] = local fun already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    L_FUN_AT_LINE;  // abc[namespace_name][fun] = local fun line number
static std::map<std::pair<std::string, std::string>, std::string>
    L_FUN_TYPE;  // abc[namespace_name][fun] = local fun type
static std::map<std::pair<std::string, std::string>, int>
    L_FUN_ARG_TOTAL;  // abc[namespace_name][fun] = global total args
static std::map<std::pair<std::string, int>, std::string>
    L_FUN_ARG_TYPE;  // abc[namespace_name + fun][arg number] = arg type

int Win_CurrentTotalFucntion = 0;
static std::map<std::string, int>
    Win_TotalFucntion;  // abc[namespace_name] = total functions
static std::map<std::pair<std::string, int>, std::string>
    Win_FunctionNames;  // abc[namespace_name][fun number] = func name

static std::string NEW_FUNCTION_ARG = "";

// -----------------------------------------------------------
// VARs
// -----------------------------------------------------------

// Local var

static std::map<std::pair<std::string, std::string>, bool>
    L_VAR_IS_SET;  // abc[namespace_name + func][var] = local var already set ?
static std::map<std::pair<std::string, std::string>, std::string>
    L_VAR_AT_LINE;  // abc[namespace_name + func][var] = local var line number
static std::map<std::pair<std::string, std::string>, std::string>
    L_VAR_TYPE;  // abc[namespace_name + func][var] = local var type
static std::map<std::pair<std::string, std::string>, bool>
    L_VAR_IS_CONST;  // abc[namespace_name + func][var] = local var Constant
static std::map<std::pair<std::string, std::string>, bool> L_VAR_IS_ARRAY;

// Global var

static std::map<std::string, bool>
    G_VAR_IS_SET;  // abc[var] = global var already set ?
static std::map<std::string, std::string>
    G_VAR_AT_LINE;  // abc[var] = global var line number
static std::map<std::string, std::string>
    G_VAR_TYPE;  // abc[var] = global var type
static std::map<std::string, bool>
    G_VAR_IS_CONST;  // abc[var] = global var Constant
static std::map<std::string, bool> G_VAR_IS_ARRAY;
static int G_VAR_TOTAL = 0;                     // Total of global vars
static std::map<int, std::string> G_VAR_NAMES;  // abc[var] = var_name

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

std::string PATH_FULL_ALIF;  // Desktop/myapp.alif
std::string PATH_FULL_BIN;   // Desktop/mybin.bin
std::string PATH_FULL_LOG;   // Desktop/myapp.log

std::string cc_path_full;   // Linux : 'gcc' | Win : c:\Alif\bin\gcc.exe
std::string PATH_FULL_CPP;  // tmp/123456789.cpp
std::string PATH_FULL_OBJ;  // tmp/123456789.o
std::string PATH_FULL_RC;   // tmp/123456789.rc
std::string
    PATH_FULL_ICO;  // Mac: /usr/share/aliflang/alif.ics | Win: c:\Alif\alif.ico
#if __APPLE__
std::string PATH_FULL_PLIST;    // Mac only
std::string PATH_FULL_BIN_TMP;  // Tmp/mybin.bin
#endif

std::string PATH_ABSOLUTE;
std::string PATH_WORKING;

std::string PATH_TEMP;  // /usr/tmp
std::string RANDOM;     // 1234

std::string PATH_FULL_LIB_SETTING;  // 'gcc' | c:\Alif\aliflib\aliflib.inf
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

std::string Compile_ExtraCompile = " ";  // -I/...
std::string Compile_ExtraLink = " ";     // -L\"/...\" -lmylib...

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
    std::string CALL_WIN_OR_CLASS,  // win1/class1 { fun1(int a) } | win2 {
                                    // fun2{ عدد b; fun1(b) } } ==> win1
    std::string CALL_FUN,  // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
                           // } } ==> fun1
    int CALL_IS_CLASS,  // 0 = non class, 1 constructor, 2 = الدالة member, ل
                        // Message when new obj
    std::string FROM_WIN_OR_CLASS,  // win1 { fun1(int a) } | win2 { fun2{ عدد
                                    // b; fun1(b) } } ==> win2
    std::string FROM_FUN,  // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
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
        // *)s.c_str(), s.length()); size_t len = 0; _base64_encode((const
        // unsigned char *) s.c_str(), s.length(), &len);
        s = base64_encode(s);
        boost::algorithm::replace_all(s, "\r", "");   // remove \r on windows
        boost::algorithm::replace_all(s, "\n", "");   // remove \n
        boost::algorithm::replace_all(s, "\\n", "");  // skip auto 72 char break
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

        ERROR_DESCRIPTION = "{\"file\":\"\",\"line\":\"0\",\"msg\":\"" +
                            ERROR_DESCRIPTION + "\"}";
    }

    file.open(PATH_FULL_LOG, ios::app);
    file << ERROR_DESCRIPTION;
    file.close();

    AlifExitFailure();
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
        DEBUG_MESSAGE("\n ----------- ERROR --------------------------------- \n",
                      o_tokens);
    if (DEBUG) DEBUG_MESSAGE("Namespace : " + TheNamespace + " \n", o_tokens);
    if (DEBUG) DEBUG_MESSAGE("Class : " + TheClass + " \n", o_tokens);
    if (DEBUG) DEBUG_MESSAGE("Fun : " + TheFunction + " \n\n", o_tokens);

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

    AlifExitFailure();
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

    AlifExitFailure();
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

    if (start_pos == std::string::npos) return false;

    str.replace(start_pos, from.length(), to);

    return true;
}

bool replace_end(std::string &str, const std::string &from,
                 const std::string &to) {
    size_t start_pos = str.rfind(from);

    if (start_pos == std::string::npos) return false;

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

    if (LONG < 1) return "";

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
// Bse64
// -----------------------------------------------------------

// https://github.com/ReneNyffenegger/cpp-base64/blob/master/base64.cpp

static const std::string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
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
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) +
                              ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) +
                              ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++) ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] =
            ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] =
            ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++) ret += base64_chars[char_array_4[j]];

        while ((i++ < 3)) ret += '=';
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

// -----------------------------------------------------------
// CPP Set Variables Name
// -----------------------------------------------------------

static std::map<std::string, std::string> ID;          // abc[var] = V1000
static std::map<std::string, std::string> Global_ID;   // abc[var] = G_V1000
static std::map<std::string, std::string> Control_ID;  // abc[var] = C_V1000
static std::map<std::string, std::string> Obj_ID;      // abc[var] = OBJ_V1000
static std::map<std::string, std::string>
    GlobalObj_ID;  // abc[var] = G_OBJ_V1000

void SET_OBJ_C_NAME(std::string VAR) {
    if (Obj_ID[VAR] == "") Obj_ID[VAR] = "OBJ_" + IntToString(ID_GENERATOR());
}

void SET_GLOBAL_OBJ_C_NAME(std::string VAR) {
    if (GlobalObj_ID[VAR] == "")
        GlobalObj_ID[VAR] = "G_OBJ_" + IntToString(ID_GENERATOR());
}

void SET_C_NAME(std::string VAR) {
    if (ID[VAR] == "") ID[VAR] = "V_" + IntToString(ID_GENERATOR());
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
        DEBUG_MESSAGE(
            "REAL_LINE : |" + o_tokens->REAL_LINE[o_tokens->Line] + "| \n",
            o_tokens);
    if (DEBUG) DEBUG_MESSAGE("START : " + IntToString(START) + " \n", o_tokens);
    if (DEBUG)
        DEBUG_MESSAGE("TOKEN_POSITION : " + IntToString(TOKEN_POSITION) + " \n",
                      o_tokens);
    if (DEBUG)
        DEBUG_MESSAGE(
            "REAL_TOKEN_POSITION : " +
                IntToString(o_tokens->REAL_TOKEN_POSITION[std::make_pair(
                    o_tokens->Line, TOKEN_POSITION)]) +
                " \n",
            o_tokens);

    if (START < 0 || TOKEN_POSITION < 1)  // Exception !
        return o_tokens->REAL_LINE[o_tokens->Line];

    std::string BUFFER;

    BUFFER = substr_utf8(o_tokens->REAL_LINE[o_tokens->Line], START,
                         o_tokens->REAL_TOKEN_POSITION[std::make_pair(
                             o_tokens->Line, TOKEN_POSITION)]);

    if (BUFFER.empty())
        return o_tokens->REAL_LINE[o_tokens->Line];  // Empty, So tray to return
                                                     // full real line !
    else
        return BUFFER;  // Return the SubStr requested.
}

// ====================================================

bool IsValidStringFormat(std::string STRING, CLASS_TOKEN *o_tokens) {
    if (substr_utf8(STRING, 0, 1) != "\"") return false;

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
        } else if (Char == "\xEF\xBB\xBF")  // BOM Signe in first char of file
        {
            if (DEBUG) DEBUG_MESSAGE("[BOM]", o_tokens);  // LOG
        }
        // else if (Char == "\u202E") // RIGHT-TO-LEFT OVERRIDE singe, إلى force
        // RTL
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
                return false;  // .123
            }

            if (I == (TOTAL_CHAR - 1)) {
                return false;  // 123.
            }

            if (!FLOAT) {
                return false;  // 12.3, but need integer not float.
            }

            if (DECIMAL) {
                return false;  // 1.2.3
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

        IsValidVar_Type =
            L_VAR_TYPE[std::make_pair(TheClass + TheFunction, Var)];
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

    if (TOKEN_LAST == "(")  // إلى avoid ... () ...
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

    if (CharCount_utf8(STRING, o_tokens) < 3)  // "" Emty std::string
        return "";                             // 3 is the minimum msg, "a"

    MESSAGE_BUFFER = substr_utf8(STRING, 1, CharCount_utf8(STRING, o_tokens));
    MESSAGE_BUFFER = substr_utf8(
        MESSAGE_BUFFER, 0, (CharCount_utf8(MESSAGE_BUFFER, o_tokens) - 1));

    return MESSAGE_BUFFER;
}

// ====================================================

void CheckForSameGlobalID(std::string Name, CLASS_TOKEN *o_tokens) {
    // check if same name with any already global declaration

    if (OBJ_IS_SET[std::make_pair("", Name)]) {
        // same name with Global Obj
        ErrorCode(
            "تشابه في الاسم مع كائن عام ثم إنشائه مسبقا، المرجو تغيير الاسم : "
            "' " +
                Name + " ' ",
            o_tokens);
    } else if (CLASS_IS_SET[Name]) {
        // same name with Class
        ErrorCode(
            "تشابه في الاسم مع صنف ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " +
                Name + " ' ",
            o_tokens);
    } else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "عدد") {
        // same name with Global INT
        ErrorCode(
            "تشابه في الاسم مع متغير عددي ثم إنشائه مسبقا، المرجو تغيير "
            "الاسم : ' " +
                Name + " ' ",
            o_tokens);
    } else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "نص") {
        // same name with Global STRING
        ErrorCode(
            "تشابه في الاسم مع متغير نصي ثم إنشائه مسبقا، المرجو تغيير الاسم : "
            "' " +
                Name + " ' ",
            o_tokens);
    } else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "منطق") {
        // same name with Global BOOL
        ErrorCode(
            "تشابه في الاسم مع متغير منطقي ثم إنشائه مسبقا، المرجو تغيير "
            "الاسم : ' " +
                Name + " ' ",
            o_tokens);
    } else if (G_FUN_IS_SET[(Name)]) {
        // same name with Global Function
        ErrorCode(
            "تشابه في الاسم مع دالة عامة ثم إنشائها مسبقا، المرجو تغيير "
            "الاسم : ' " +
                Name + " ' ",
            o_tokens);
    }

    else if (namespace_is_set[Name])  // || CONTROL_WIN_IS_SET[Name]
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

        if (Char == "_" ||  // Allow, Example : ABC_XZ1
            Char == "0" || Char == "1" || Char == "2" || Char == "3" ||
            Char == "4" || Char == "5" || Char == "5" || Char == "6" ||
            Char == "8" || Char == "9" ||

            Char == "۰" ||  // 0, Zero In Arabic, ITS NOT A DOT!
            Char == "۱" ||  // 1
            Char == "۲" ||  // 2
            Char == "۳" ||  // 3
            Char == "۴" ||  // 4
            Char == "۵" ||  // 5
            Char == "۶" ||  // 6
            Char == "۷" ||  // 7
            Char == "۸" ||  // 8
            Char == "۹")    // 9
        {
            if (First) {
                ErrorCode(
                    "أول حرف في الإسم يجب أن يكون حرفا، و ليس رقم أو إشارة : "
                    "' " +
                        Char + " ' ",
                    o_tokens);
                return false;
            }
        } else if (Char == "." ||  // Allow DOT in متغير names ABC.DEF
                   Char == "," || Char == "،" ||  // اشارة ،
                   Char == "{" || Char == "}" || Char == "|" || Char == "~" ||
                   // Char == "_" || Allow AB_C
                   Char == "$" || Char == "#" || Char == ":" || Char == "!" ||
                   Char == ";" || Char == "?" || Char == "'" || Char == "-" ||
                   Char == "/" || Char == "=" || Char == "\\" || Char == "^" ||
                   Char == "\"" || Char == "%" || Char == ")" || Char == "(" ||
                   Char == "*" || Char == "+" || Char == "<" || Char == ">" ||
                   Char == "@" || Char == "[" || Char == "]" || Char == "&" ||
                   Char == " " ||     // Space
                   Char == "	" ||  // Tab
                   Char == "") {
            if (Char == " ") {
                if (!IS_CONTROL_FUNCTION_NAME) {
                    ErrorCode("حرف غير مقبول : ' " + Char + " ' داخل ' " +
                                  Name + " ' ",
                              o_tokens);
                    return false;  // Exception !
                }

                IS_CONTROL_FUNCTION_NAME = false;
                return true;
            }

            ErrorCode("حرف غير مقبول : ' " + Char + " ' داخل ' " + Name + " ' ",
                      o_tokens);
            return false;
        }

        First = false;
    }  // End char loop.

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
            ErrorCode("الدالة العامه ' " + FUN_NAME +
                          " ' تم انشاؤها مسبقا في السطر : " +
                          G_FUN_AT_LINE[(FUN_NAME)],
                      o_tokens);
        }

        // if already any other type of global var exist
        CheckForSameGlobalID(FUN_NAME, o_tokens);

        G_FUN_TYPE[(FUN_NAME)] = TYPE;
        G_FUN_IS_SET[(FUN_NAME)] = true;
        G_FUN_AT_LINE[(FUN_NAME)] = IntToString(Line);

        Global_FunctionNames[Global_TotalFucntion] = FUN_NAME;
        Global_TotalFucntion++;

        if (TYPE == "عادم") RETURN_FUN[std::make_pair("", FUN_NAME)] = "OK";

        // *** Generate Code ***
        SET_GLOBAL_C_NAME(FUN_NAME);
        // *** *** *** *** *** ***
    } else {
        // if already exist local fun
        if (L_FUN_IS_SET[std::make_pair(NS_Name, FUN_NAME)]) {
            ErrorCode("الدالة ' " + FUN_NAME +
                          " ' تم انشاؤها مسبقا في السطر : " +
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
            ErrorCode(
                "المتغير العام ' " + VarName +
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
            ErrorCode("المتغير ' " + VarName +
                          " ' تم انشاؤه مسبقا في السطر : " +
                          L_VAR_AT_LINE[std::make_pair(TempNS + TmpFunction,
                                                       VarName)],
                      o_tokens);
        }

        L_VAR_TYPE[std::make_pair(TempNS + TmpFunction, VarName)] = VarDataType;
        L_VAR_IS_SET[std::make_pair(TempNS + TmpFunction, VarName)] = true;
        L_VAR_AT_LINE[std::make_pair(TempNS + TmpFunction, VarName)] =
            IntToString(Line);
        L_VAR_IS_CONST[std::make_pair(TempNS + TmpFunction, VarName)] =
            IsConstant;
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
            ErrorCode(
                "المتغير العام ' " + VarName +
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
        CLASS_G_VAR_AT_LINE[std::make_pair(ClassName, VarName)] =
            IntToString(Line);
        CLASS_G_VAR_PRIVATE[std::make_pair(ClassName, VarName)] = IsPrivate;
        CLASS_G_VAR_IS_CONST[std::make_pair(ClassName, VarName)] = IsConstant;
        CLASS_G_VAR_IS_ARRAY[std::make_pair(ClassName, VarName)] = IsArray;

        // *** Generate Code ***
        SET_GLOBAL_C_NAME(VarName);
        // *** *** *** *** *** ***
    } else {
        if (L_VAR_IS_SET[std::make_pair(ClassName + FunctionName, VarName)]) {
            ErrorCode("المتغير ' " + VarName +
                          " ' تم انشاؤه مسبقا في السطر : " +
                          L_VAR_AT_LINE[std::make_pair(ClassName + FunctionName,
                                                       VarName)],
                      o_tokens);
        }

        L_VAR_TYPE[std::make_pair(ClassName + FunctionName, VarName)] =
            VarDataType;
        L_VAR_IS_SET[std::make_pair(ClassName + FunctionName, VarName)] = true;
        L_VAR_AT_LINE[std::make_pair(ClassName + FunctionName, VarName)] =
            IntToString(Line);
        L_VAR_IS_CONST[std::make_pair(ClassName + FunctionName, VarName)] =
            IsConstant;
        L_VAR_IS_ARRAY[std::make_pair(ClassName + FunctionName, VarName)] =
            IsArray;

        // *** Generate Code ***
        SET_C_NAME(VarName);
        // *** *** *** *** *** ***
    }
}

std::string GetSyntaxDataType(std::string Token[1024], int Position,
                              CLASS_TOKEN *o_tokens) {
    // Token[Position] -> '='

    if (IsValidVar(Token[Position + 1], o_tokens)) return IsValidVar_Type;

    return {};
}

void ADD_FUN_CLASS(bool PRIVATE, std::string CLASS_NAME, std::string FUN_NAME,
                   std::string TYPE, int Line, CLASS_TOKEN *o_tokens) {
    if (!IsValidName(FUN_NAME, o_tokens))
        ErrorCode("اسم غير مقبول : ' " + FUN_NAME + " ' ", o_tokens);

    // if already exist class fun
    if (CLASS_FUN_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)]) {
        ErrorCode("الصنف الدالة ' " + FUN_NAME +
                      " ' تم انشاؤها مسبقا في السطر : " +
                      CLASS_FUN_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)],
                  o_tokens);
    }

    // if already same Generated_ID with global class var.
    if (CLASS_G_VAR_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)]) {
        ErrorCode(
            "تشابه في الاسم مع متغير ' " + FUN_NAME + " ' في السطر : " +
                CLASS_G_VAR_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)] + " ",
            o_tokens);
    }
    // else
    // if(DEBUG)DEBUG_MESSAGE("---C " + CLASS_NAME + " :F " + FUN_NAME + "
    // E---", o_tokens); // DEBUG

    // if same name as class !
    if (CLASS_NAME == FUN_NAME)
        ErrorCode(
            "تشابه في الاسم بين الدالة و الصنف ' " + FUN_NAME +
                " '، على العموم إن كنت تقصد دالة بناء الصنف استخدم التعريف ' "
                "بناء '",
            o_tokens);

    CLASS_FUN_TYPE[std::make_pair(CLASS_NAME, FUN_NAME)] = TYPE;
    CLASS_FUN_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)] = true;
    CLASS_FUN_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)] = IntToString(Line);
    CLASS_FUN_PRIVATE[std::make_pair(CLASS_NAME, FUN_NAME)] = PRIVATE;

    if (TYPE == "عادم") RETURN_FUN[std::make_pair(CLASS_NAME, FUN_NAME)] = "OK";

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
    std::string TempNS,       // fun1 { a = b + namespace:fun2(x) + z }
    std::string TmpFunction,  // fun1 { a = b + namespace:fun2(x) + z }
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

            if (L_VAR_IS_SET[std::make_pair(
                    TempNS + TmpFunction,
                    SYNTAX[VAR])])  // الدالة (int a, نص a)
                ErrorCode("متغير محلي موجود مسبقا ' " + SYNTAX[VAR] + " ' ",
                          o_tokens);

            if (SYNTAX[COMMA] != "،" && SYNTAX[COMMA] != "," &&
                SYNTAX[COMMA] != ")")
                ErrorCode("فاصله غير صحيحه ' " + SYNTAX[COMMA] + " ' ",
                          o_tokens);

            if (SYNTAX[COMMA] == ")" && COMMA < SYNTAX_LONG)
                ErrorCode("غير مكتمل ' " + IntToString(SYNTAX_LONG) + " ' ",
                          o_tokens);

            // if(DEBUG)DEBUG_MESSAGE("[" + SYNTAX[TYPE] + "|" + SYNTAX[VAR] +
            // "]", o_tokens); // DEBUG

            // add always-local (bcs this var is in arg..) VAR to fun
            SetNewVar(false, TempNS, TmpFunction, SYNTAX[VAR], SYNTAX[TYPE],
                      false, false, o_tokens->Line, o_tokens);

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

            TYPE += 3, VAR += 3, COMMA += 3;  // For ARG non-predifined loop
        } else {
            // Tokens already predifined, so we need show log, and return C++
            // code

            if (SYNTAX[TYPE] == "عدد") {
                if (DEBUG)
                    DEBUG_MESSAGE("[INT " + SYNTAX[VAR] + "]",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                // always need pointer to one elem array
                CPP_CODE.append(" double " + ID[SYNTAX[VAR]] + " ");
                NEW_FUNCTION_ARG.append(" double " + ID[SYNTAX[VAR]] + " ");
                // *** *** *** *** *** ***
            } else if (SYNTAX[TYPE] == "نص") {
                if (DEBUG)
                    DEBUG_MESSAGE("[STRING " + SYNTAX[VAR] + "]",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" std::string " + ID[SYNTAX[VAR]] + " ");
                NEW_FUNCTION_ARG.append(" std::string " + ID[SYNTAX[VAR]] +
                                        " ");
                // *** *** *** *** *** ***
            } else if (SYNTAX[TYPE] == "منطق") {
                if (DEBUG)
                    DEBUG_MESSAGE("[BOOL " + SYNTAX[VAR] + "]",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" bool " + ID[SYNTAX[VAR]] + " ");
                NEW_FUNCTION_ARG.append(" bool " + ID[SYNTAX[VAR]] + " ");
                // *** *** *** *** *** ***
            } else if (SYNTAX[TYPE] == "مؤشر_دالة") {
                if (DEBUG)
                    DEBUG_MESSAGE("[POINTER " + SYNTAX[VAR] + "]",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" void(* " + ID[SYNTAX[VAR]] + " )() ");
                NEW_FUNCTION_ARG.append(" void(* " + ID[SYNTAX[VAR]] + " )() ");
                // *** *** *** *** *** ***
            } else {
                // Exception !
                ErrorCode("نوع غير مقبول ' " + SYNTAX[TYPE] + " ' ", o_tokens);
            }

            TYPE += 3, VAR += 3,
                COMMA += 3;  // For ARG predifined / C++ loop, and need by comma

            // *** Generate Code ***
            if (COMMA <= SYNTAX_LONG) {
                CPP_CODE.append(" , ");
                NEW_FUNCTION_ARG.append(" , ");
            }
            // *** *** *** *** *** ***
        }
    }

    // if(DEBUG)DEBUG_MESSAGE("[NS: "+ TempNS +"|Fun: " + TmpFunction +
    // "|Looocal: " + IntToString(L_FUN_ARG_TOTAL[std::make_pair(TempNS,
    // TmpFunction)]) +
    // "|Glooobal: " + IntToString(G_FUN_ARG_TOTAL[TmpFunction]) + "]",
    // o_tokens);
    // // DEBUG

    if (o_tokens->TOKENS_PREDEFINED)
        return CPP_CODE;
    else
        return "";
}

// ====================================================

std::string CheckForSyntax(
    std::string OBJECTIF_TYPE,  // OBJECTIF_TYPE
    bool
        ACCEPT_REF_WIN_WIDGET,  // Accept Using Reference إلى namespace:Controls
    bool ACCEPT_REF_WIN_FUN,  // Accept Using Reference إلى namespace:Function
    bool ACCEPT_REF_GLOBAL_FUN,  // Accept Using Reference إلى Global Functions
    bool ACCEPT_REF_LOCAL_FUN,  // Accept Using Reference إلى Local Functions
    bool ACCEPT_REF_GLOBAL_VAR,  // Accept Using Reference إلى Global VAR
    bool ACCEPT_REF_LOCAL_VAR,   // Accept Using Reference إلى Local VAR
    bool ACCEPT_STR_TO_INT,      // Accept Convertion من نص إلى Int
    bool ACCEPT_INT_TO_STRING,   // Accept Convertion من عدد إلى String
    std::string SYNTAX[1024],    // SYNTAX[] std::string
    int SYNTAX_LONG,             // SYNTAX_LONG int
    std::string
        TMP_NAMESPACE_OR_CLASS,  // a = b + namespace:fun(2+2) + class:fun(x)
    std::string TmpFunction,     // a = b + win/class:fun(2+2)
    CLASS_TOKEN *o_tokens) {
    // Note : WX Crash if On creating using reference إلى Controls, BUT OK ل
    // Functions.

    // SYNTAX = '='
    // SYNTAX[1] = ...

    // TmpFunction				: = a + b + TMP_FUN_NAME(X, Y,
    // Z)
    // +
    // ... TMP_NAMESPACE_OR_CLASS		: إلى get Generated_ID of tmp
    // الدالة name OBJECTIF_TYPE		: INT.ToString or STRING.ToInt

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
                DEBUG_MESSAGE(" {_س_ Syntax START} ", o_tokens);  // DEBUG

            bool C_FOUND = false;

            for (int z = p + 1; z <= SYNTAX_LONG; z++) {
                if (SYNTAX[z] == "_س_")  // End C++ Code
                {
                    if (DEBUG)
                        DEBUG_MESSAGE(" {_س_ Syntax END} ", o_tokens);  // DEBUG
                    C_FOUND = true;
                    p = z;
                    break;
                } else if (SYNTAX[z] == "@")  // @ Start ...
                {
                    // xyz = ...C++...@ Alif @...C++...

                    if (DEBUG) DEBUG_MESSAGE(" {Syntax@} ", o_tokens);  // DEBUG

                    std::string TempToken[1024];
                    TempToken[0] = "=";      // CheckForSyntax() Need this.
                    int TempTokenCount = 1;  // CheckForSyntax() Need this.

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
                        ErrorCode("نهايه شفرة غير موجود داخل البناء ' @ '",
                                  o_tokens);

                    std::string SYNTAX_BUFFER = CheckForSyntax(
                        "C++",  // OBJECTIF_TYPE
                        true,  // Accept Using Reference إلى namespace:Controls
                        true,  // Accept Using Reference إلى namespace:Function
                        true,  // Accept Using Reference إلى Global Functions
                        true,  // Accept Using Reference إلى Local Functions
                        true,  // Accept Using Reference إلى Global VAR
                        true,  // Accept Using Reference إلى Local VAR
                        true,  // Accept Convertion من نص إلى Int
                        true,  // Accept Convertion من عدد إلى String
                        TempToken,             // SYNTAX[] std::string
                        (TempTokenCount - 1),  // SYNTAX_LONG int
                        TheNamespace,          // TMP_WINDOW_NAME
                        TheFunction,           // TMP_FUNCTION_NAME
                        o_tokens);

                    if (DEBUG) DEBUG_MESSAGE(" {Syntax@} ", o_tokens);  // DEBUG

                    // *** C++ ***
                    CPP_CODE.append(" " + SYNTAX_BUFFER + " ");
                    // *** *** *** *** *** ***

                    // @ End.
                } else if (SYNTAX[z] != "") {
                    CPP_CODE.append(SYNTAX[z]);
                    if (DEBUG)
                        DEBUG_MESSAGE(" {" + SYNTAX[z] + "} ",
                                      o_tokens);  // DEBUG
                }
            }

            if (C_FOUND)
                continue;  // Point إلى next بعد _س_
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
                    ErrorCode("لا يمكن إضافة عملية هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);
            }

            if (p >= SYNTAX_LONG) {
                if (DEBUG)
                    DEBUG_MESSAGE("######################", o_tokens);  // DEBUG
                for (int p = 1; p <= SYNTAX_LONG; p++)
                    if (DEBUG)
                        DEBUG_MESSAGE("[" + SYNTAX[p] + "] ",
                                      o_tokens);  // DEBUG
                ErrorCode("لا يمكن الإنتهاء بعملية ' " + SYNTAX[p] + " ' ",
                          o_tokens);
            }

            if (OBJECTIF_TYPE ==
                "نص")  // only acceptable operation in نص is '+'
            {
                if (SYNTAX[p] != "+")
                    ErrorCode(
                        "لا يمكن إضافة عملية داخل نص ' " + SYNTAX[p] + " ' ",
                        o_tokens);
            }

            if (OBJECTIF_TYPE == "منطق")
                ErrorCode(
                    "لا يمكن أجراء عملية في متغير منطقي ' " + SYNTAX[p] + " ' ",
                    o_tokens);

            if (substr_utf8(SYNTAX[p - 1], 0, 1) == "\"" &&
                substr_utf8(SYNTAX[p + 1], 0, 1) == "\"")  // "test" + "hello"
                ErrorCode("لا يمكن إضافة نص إلى نص ' " + SYNTAX[p - 1] + " " +
                              SYNTAX[p] + " " + SYNTAX[p + 1] + " ' ",
                          o_tokens);

            if (DEBUG)
                DEBUG_MESSAGE("[OPERATION " + SYNTAX[p] + "] ",
                              o_tokens);  // DEBUG

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
                    ErrorCode("لا يمكن إضافة قوس مفتوح هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            ALIF_PARENTHESIS_STATUS++;

            if (DEBUG)
                DEBUG_MESSAGE("[PARENTHESIS Open (] ", o_tokens);  // DEBUG

            // *** Generate Code ***
            CPP_CODE.append(" ( ");
            // *** *** *** *** *** ***
        }

        // ----------------------
        // قوس CLOSE
        // ----------------------

        else if (SYNTAX[p] == ")") {
            if (ALIF_PARENTHESIS_STATUS < 1)
                ErrorCode("غلق قوس ليس مفتوحا ' " + SYNTAX[p] + " ' ",
                          o_tokens);

            if (p > 0)
                if (!CAN_ADD_PARENTHESIS_CLOSE_HERE(SYNTAX[p - 1]))
                    ErrorCode("لا يمكن إضافة غلق قوس هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            ALIF_PARENTHESIS_STATUS--;

            if (DEBUG)
                DEBUG_MESSAGE("[PARENTHESIS CLOSE )] ", o_tokens);  // DEBUG

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
                    ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            if (OBJECTIF_TYPE == "عدد") {
                if (DEBUG)
                    DEBUG_MESSAGE("[LOCAL-INT (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
                // *** *** *** *** *** ***
            } else if (OBJECTIF_TYPE == "نص") {
                if (ACCEPT_INT_TO_STRING) {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[LOCAL-INT (" + SYNTAX[p] + ").ToString] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" alifcore_IntToString(" + ID[SYNTAX[p]] +
                                    ") ");
                    // *** *** *** *** *** ***
                } else {
                    ErrorCode(
                        "لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                        o_tokens);
                }
            } else if (OBJECTIF_TYPE == "C++") {
                if (DEBUG)
                    DEBUG_MESSAGE("[C++ Local INT (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
                // *** *** *** *** *** ***
            } else {
                ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                              " -> " + SYNTAX[p] + " ' ",
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
                    ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            if (OBJECTIF_TYPE == "عدد") {
                // if(DEBUG)DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] +
                // ").ToInt] ", o_tokens); // DEBUG
                ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ",
                          o_tokens);
            } else if (OBJECTIF_TYPE == "نص") {
                if (DEBUG)
                    DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
                // *** *** *** *** *** ***
            } else if (OBJECTIF_TYPE == "C++") {
                if (DEBUG)
                    DEBUG_MESSAGE("[C++ Local STRING (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
                // *** *** *** *** *** ***
            } else {
                ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                              " -> " + SYNTAX[p] + " ' ",
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
                              " '، على العموم لا يمكن أجراء أية عملية على "
                              "متغير منطقي ",
                          o_tokens);
            } else if (OBJECTIF_TYPE == "C++") {
                if (DEBUG)
                    DEBUG_MESSAGE("[C++ Local Bool (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

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
                DEBUG_MESSAGE("[LOCAL-BOOL ' " + SYNTAX[p] + " '] ",
                              o_tokens);  // DEBUG

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
                    ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            if (OBJECTIF_TYPE == "عدد") {
                ErrorCode(
                    "لا يمكن تحويل مؤشر_دالة إلى عدد ' " + SYNTAX[p] + " ' ",
                    o_tokens);
            } else if (OBJECTIF_TYPE == "نص") {
                ErrorCode(
                    "لا يمكن تحويل مؤشر_دالة إلى نص ' " + SYNTAX[p] + " ' ",
                    o_tokens);
            } else if (OBJECTIF_TYPE == "مؤشر_دالة") {
                if (DEBUG)
                    DEBUG_MESSAGE("[LOCAL-POINTER-FUNC (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
                // *** *** *** *** *** ***
            } else if (OBJECTIF_TYPE == "C++") {
                if (DEBUG)
                    DEBUG_MESSAGE(
                        "[C++ Local POINTER-FUNC (" + SYNTAX[p] + ")] ",
                        o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
                // *** *** *** *** *** ***
            } else {
                ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                              " -> " + SYNTAX[p] + " ' ",
                          o_tokens);
            }
        }

        // ----------------------
        // Global VARIABLE (all type)
        // Global VARIABLE without _
        // ----------------------

        else if (substr_utf8(SYNTAX[p], 0, 1) == "_" ||
                 G_VAR_IS_SET[(SYNTAX[p])]) {
            if (p > 0)
                if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
                    ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            std::string G_VAR_WITHOUT_;

            if (G_VAR_IS_SET[(SYNTAX[p])])
                G_VAR_WITHOUT_ = SYNTAX[p];
            else
                G_VAR_WITHOUT_ = substr_utf8(
                    SYNTAX[p], 1, CharCount_utf8(SYNTAX[p], o_tokens));

            if (!G_VAR_IS_SET[(G_VAR_WITHOUT_)])
                ErrorCode("ليس متغير عام ' " + SYNTAX[p] + " ' ", o_tokens);

            if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "عدد") {
                if (OBJECTIF_TYPE == "عدد") {
                    if (DEBUG)
                        DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ + ")] ",
                                      o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
                    // *** *** *** *** *** ***
                } else if (OBJECTIF_TYPE == "نص") {
                    if (ACCEPT_INT_TO_STRING) {
                        if (DEBUG)
                            DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ +
                                              ").ToString] ",
                                          o_tokens);  // DEBUG

                        // *** Generate Code ***
                        CPP_CODE.append(" alifcore_IntToString(" +
                                        Global_ID[G_VAR_WITHOUT_] + ") ");
                        // *** *** *** *** *** ***
                    } else {
                        ErrorCode(
                            "لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                            o_tokens);
                    }
                } else if (OBJECTIF_TYPE == "C++") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[C++ Global عدد (" + G_VAR_WITHOUT_ + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
                    // *** *** *** *** *** ***
                } else {
                    ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                  OBJECTIF_TYPE + " -> " + SYNTAX[p] + " ' ",
                              o_tokens);
                }
            } else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "نص") {
                if (OBJECTIF_TYPE == "عدد") {
                    if (ACCEPT_INT_TO_STRING)  // or ACCEPT_STR_TO_INT !!
                    {
                        // if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" +
                        // G_VAR_WITHOUT_ +
                        // ").ToInt] ", o_tokens); // DEBUG
                        ErrorCode("لا يمكن تحويل نص إلى عدد ' " +
                                      G_VAR_WITHOUT_ + " ' ",
                                  o_tokens);
                    } else {
                        ErrorCode(
                            "لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                            o_tokens);
                    }
                } else if (OBJECTIF_TYPE == "نص") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[GLOBAL-STRING (" + G_VAR_WITHOUT_ + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
                    // *** *** *** *** *** ***
                } else if (OBJECTIF_TYPE == "C++") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[C++ Global نص (" + G_VAR_WITHOUT_ + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
                    // *** *** *** *** *** ***
                } else {
                    ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                  OBJECTIF_TYPE + " -> " + SYNTAX[p] + " ' ",
                              o_tokens);
                }
            } else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "منطق") {
                if (OBJECTIF_TYPE != "منطق" && OBJECTIF_TYPE != "C++") {
                    ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE +
                                  " ' " + G_VAR_WITHOUT_ + " ' ",
                              o_tokens);
                }

                if (DEBUG)
                    DEBUG_MESSAGE("[GLOBAL-BOOL ' " + G_VAR_WITHOUT_ + " '] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
                // *** *** *** *** *** ***
            } else {
                ErrorCode("علة : نوع المتغير العام غير معروف ' " +
                              G_VAR_TYPE[(G_VAR_WITHOUT_)] + " -> " +
                              G_VAR_WITHOUT_ + " ' ",
                          o_tokens);
            }
        }

        // -----------------------------------
        // True / False
        // -----------------------------------

        else if (SYNTAX[p] == "صحيح") {
            if (p != 1)
                ErrorCode("لا يمكن وضع ( منطق ) هنا ' " + SYNTAX[p] + " ' ",
                          o_tokens);

            if (SYNTAX[p + 1] != "") {
                // bool A = TRUE + wrong
                ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] +
                              " '، على العموم لا يمكن أجراء أية عملية على "
                              "متغير منطقي ",
                          o_tokens);
            }

            if (OBJECTIF_TYPE == "منطق") {
                if (DEBUG) DEBUG_MESSAGE("[TRUE] ", o_tokens);  // DEBUG

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
                ErrorCode("لا يمكن وضع ( منطق ) هنا ' " + SYNTAX[p] + " ' ",
                          o_tokens);

            if (SYNTAX[p + 1] != "") {
                // bool A = FALSE + wrong
                ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] +
                              " '، على العموم لا يمكن أجراء أية عملية على "
                              "متغير منطقي ",
                          o_tokens);
            }

            if (OBJECTIF_TYPE == "منطق") {
                if (DEBUG) DEBUG_MESSAGE("[FALSE] ", o_tokens);  // DEBUG

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
                    ErrorCode("لا يمكن إضافة سطر جديد هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            if (OBJECTIF_TYPE == "عدد") {
                // if(DEBUG)DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] +
                // ").ToInt] ", o_tokens); // DEBUG
                ErrorCode(
                    "لا يمكن تحويل سطر جديد إلى عدد ' " + SYNTAX[p] + " ' ",
                    o_tokens);
            } else if (OBJECTIF_TYPE == "نص") {
                if (DEBUG) DEBUG_MESSAGE("[NewLine] ", o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" ALIFCORE_NEW_LINE ");
                // *** *** *** *** *** ***
            } else if (OBJECTIF_TYPE == "منطق") {
                ErrorCode(
                    "لا يمكن تحويل سطر جديد إلى منطق ' " + SYNTAX[p] + " ' ",
                    o_tokens);
            } else if (OBJECTIF_TYPE == "C++") {
                if (DEBUG) DEBUG_MESSAGE("[C++_NewLine] ", o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" (\"\n\") ");
                // *** *** *** *** *** ***
            } else {
                ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                              " -> " + SYNTAX[p] + " ' ",
                          o_tokens);
            }
        }

        // ----------------------
        // Class Global/Local Obj
        // ----------------------

        else if (OBJ_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS + TmpFunction,
                                           SYNTAX[p])] ||
                 OBJ_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                           SYNTAX[p])] ||
                 OBJ_IS_SET[std::make_pair("", SYNTAX[p])]) {
            // a = obj:mem + c
            // b = obj:mem_func(a, b) + c

            if (p > 0)
                if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
                    ErrorCode("لا يمكن إضافة كائن هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            // if (SYNTAX[p + 1] != ":")
            // ErrorCode("يجب إضافة ':' بعد ' " + SYNTAX[p] + " ' ", o_tokens);
            // if (SYNTAX[p + 2] == "")
            // ErrorCode("يجب إضافة إسم منتمي للصنف بعد ' " + SYNTAX[p] + " : '
            // ", o_tokens);

            if (SYNTAX[p + 1] != ":")
                ErrorCode("يجب وضع نقطتين ':' بين الكائن و المنتمي، أي بعد ' " +
                              SYNTAX[p] + " ' ",
                          o_tokens);
            if (SYNTAX[p + 2] == "بناء")
                ErrorCode(
                    "لا يمكن استدعاء دالة ' بناء() '، هته الدالة تستدعى بشكل "
                    "آلي "
                    "عند إنشاء الكائن",
                    o_tokens);
            if (SYNTAX[p + 2] == "هدم")
                ErrorCode(
                    "لا يمكن استدعاء دالة ' هدم() '، هته الدالة تستدعى بشكل "
                    "آلي "
                    "عند الحاجة إلى تدمير هذا الكائن",
                    o_tokens);
            if (SYNTAX[p + 2] == "")
                ErrorCode("يجب وضع اسم المنتمي بعد ' " + SYNTAX[p] + ":' ",
                          o_tokens);

            // C++, allow create Obj on global, global-class, local, but using
            // it only on func.
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
                ErrorCode("الصنف ' " + TK_CLASS +
                              " ' ليس فيه أي منتمي معرف باسم ' " +
                              SYNTAX[p + 2] + " ' ",
                          o_tokens);

            std::string MEMBER_TYPE;

            if (CLASS_G_VAR_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])]) {
                // متغير member

                // a = obj : mem + c

                if (CLASS_G_VAR_PRIVATE[std::make_pair(TK_CLASS,
                                                       SYNTAX[p + 2])])
                    ErrorCode("لا يمكن استدعاء المتغير ' " + SYNTAX[p + 2] +
                                  " ' المنتمي للصنف ' " + TK_CLASS +
                                  " ' لأنه من نوع ' خاص ' ",
                              o_tokens);

                MEMBER_TYPE =
                    CLASS_G_VAR_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])];

                if (MEMBER_TYPE == "عادم")
                    ErrorCode("لا يمكن إضافة منتمي عادم ' " + SYNTAX[p] +
                                  " : " + SYNTAX[p + 2] + " ' ",
                              o_tokens);

                if (MEMBER_TYPE == "عدد") {
                    if (OBJECTIF_TYPE == "عدد") {
                        if (IS_GLOBAL_OBJ) {
                            if (DEBUG)
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(INT)] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                                            "." + Global_ID[SYNTAX[p + 2]] +
                                            " ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(INT)] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." +
                                            Global_ID[SYNTAX[p + 2]] + " ");
                            // *** *** *** *** *** ***
                        }
                    } else if (OBJECTIF_TYPE == "نص") {
                        if (IS_GLOBAL_OBJ) {
                            if (DEBUG)
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(INT).ToString] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" alifcore_IntToString(" +
                                            GlobalObj_ID[SYNTAX[p]] + "." +
                                            Global_ID[SYNTAX[p + 2]] + ") ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(INT).ToString] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" alifcore_IntToString(" +
                                            Obj_ID[SYNTAX[p]] + "." +
                                            Global_ID[SYNTAX[p + 2]] + ") ");
                            // *** *** *** *** *** ***
                        }
                    } else if (OBJECTIF_TYPE == "منطق") {
                        ErrorCode("لا يمكن تحويل عدد إلى منطق ' " + SYNTAX[p] +
                                      " ':'" + SYNTAX[p + 2] + " ' ",
                                  o_tokens);
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      " ' ",
                                  o_tokens);
                    }
                } else if (MEMBER_TYPE == "نص") {
                    if (OBJECTIF_TYPE == "عدد") {
                        ErrorCode(
                            "لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ",
                            o_tokens);
                    } else if (OBJECTIF_TYPE == "نص") {
                        if (IS_GLOBAL_OBJ) {
                            if (DEBUG)
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(STRING)] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                                            "." + Global_ID[SYNTAX[p + 2]] +
                                            " ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(STRING)] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." +
                                            Global_ID[SYNTAX[p + 2]] + " ");
                            // *** *** *** *** *** ***
                        }
                    } else if (OBJECTIF_TYPE == "منطق") {
                        ErrorCode("لا يمكن تحويل نص إلى منطق ' " + SYNTAX[p] +
                                      " ':'" + SYNTAX[p + 2] + " ' ",
                                  o_tokens);
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      " ' ",
                                  o_tokens);
                    }
                } else if (MEMBER_TYPE == "منطق") {
                    if (OBJECTIF_TYPE != "منطق") {
                        ErrorCode("لا يمكن تحويل منتمي منطق إلى " +
                                      OBJECTIF_TYPE + " ' " + SYNTAX[p] + " ' ",
                                  o_tokens);
                    } else if (OBJECTIF_TYPE == "منطق") {
                        if (IS_GLOBAL_OBJ) {
                            if (DEBUG)
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(BOOL)] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                                            "." + Global_ID[SYNTAX[p + 2]] +
                                            " ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(BOOL)] ",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." +
                                            Global_ID[SYNTAX[p + 2]] + " ");
                            // *** *** *** *** *** ***
                        }
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      " ' ",
                                  o_tokens);
                    }
                } else {
                    ErrorCode("علة : نوع المنتمي غير معروف ' " + MEMBER_TYPE +
                                  " -> " + SYNTAX[p] + " ' ",
                              o_tokens);
                }

                p += 2;  // point إلى next token بعد obj:mem
            } else if (CLASS_FUN_IS_SET[std::make_pair(TK_CLASS,
                                                       SYNTAX[p + 2])]) {
                // Function member

                // b = obj : memf (a, b)

                if (CLASS_FUN_PRIVATE[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
                    ErrorCode("الدالة منتمي ' " + SYNTAX[p + 2] +
                                  " ' داخل الصنف ' " + TK_CLASS + " ' خاص ",
                              o_tokens);

                if (CLASS_FUN_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])] ==
                    "عادم")
                    ErrorCode("لا يمكن إضافة منتمي عادم ' " + SYNTAX[p] +
                                  " : " + SYNTAX[p + 2] + " ' داخل الصنف ' " +
                                  TK_CLASS + " ' ",
                              o_tokens);

                if (SYNTAX[p + 3] != "(")
                    ErrorCode("يجب إضافة '(' بعد ' " + SYNTAX[p] + ":" +
                                  SYNTAX[p + 2] + " ' ",
                              o_tokens);

                MEMBER_TYPE =
                    CLASS_FUN_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])];

                int TMP_FUN_LONG = p + 4;  // a + b + 'p'obj : memf (a, b) + c
                int OPEN_PARENTIZE = 0;

                while (TMP_FUN_LONG <= SYNTAX_LONG) {
                    if (SYNTAX[TMP_FUN_LONG] ==
                        "(")  // مفتوح inside الدالة args : fun( a + (b))
                        OPEN_PARENTIZE++;
                    else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                             OPEN_PARENTIZE > 0)  // Close inside الدالة args
                        OPEN_PARENTIZE--;
                    else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                             OPEN_PARENTIZE < 1)  // Close final الدالة call
                    {
                        if (TMP_FUN_LONG < SYNTAX_LONG) {
                            // a = fun( a + (b)) + 123
                            // str / عدد = متغير + (fun(var)) * (fun(var) /
                            // fun(var, fun(var), var) - var)

                            if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "-") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "*") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "/") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "،") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != ",") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != ")")) {
                                ErrorCode("نص غير معروف بعد نداء ' " +
                                              SYNTAX[p] + " : " +
                                              SYNTAX[p + 2] + "()' : ' " +
                                              SYNTAX[TMP_FUN_LONG + 1] + " ' ",
                                          o_tokens);
                            }
                        } else if (TMP_FUN_LONG == SYNTAX_LONG) {
                            // a = obj:fun( a + (b))
                            if (SYNTAX[TMP_FUN_LONG] != ")" ||
                                SYNTAX[SYNTAX_LONG] != ")")  // double check!
                                ErrorCode("يجب إنهاء نداء الدالة ' " +
                                              SYNTAX[p] + " : " +
                                              SYNTAX[p + 2] +
                                              "()' بالإشارة ')' ",
                                          o_tokens);
                        }

                        break;
                    }

                    TMP_FUN_LONG++;
                }

                if (SYNTAX[TMP_FUN_LONG] != ")")  // Double check!
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
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-INT) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                                            ".ClassFUNCTION_" +
                                            ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-INT) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] +
                                            ".ClassFUNCTION_" +
                                            ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        }

                        CPP_CODE.append(CHECK_CALL_FUN_ARG(
                            false, TK_CLASS, SYNTAX[p + 2],
                            2,  // 2 = الدالة member
                            TheNamespace, TheFunction, TempToken,
                            (TempTokenCount - 1), o_tokens));

                        if (DEBUG) DEBUG_MESSAGE(")] ", o_tokens);  // DEBUG

                        // *** Generate Code ***
                        CPP_CODE.append(" ) ");
                        // *** *** *** *** *** ***
                    } else if (OBJECTIF_TYPE == "نص") {
                        if (IS_GLOBAL_OBJ) {
                            if (DEBUG)
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-INT) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" alifcore_IntToString(" +
                                            GlobalObj_ID[SYNTAX[p]] +
                                            ".ClassFUNCTION_" +
                                            ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-INT) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(
                                " alifcore_IntToString(" + Obj_ID[SYNTAX[p]] +
                                ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        }

                        CPP_CODE.append(CHECK_CALL_FUN_ARG(
                            false, TK_CLASS, SYNTAX[p + 2],
                            2,  // 2 = الدالة member
                            TheNamespace, TheFunction, TempToken,
                            (TempTokenCount - 1), o_tokens));

                        if (DEBUG)
                            DEBUG_MESSAGE(").IntFunToString] ",
                                          o_tokens);  // DEBUG

                        // *** Generate Code ***
                        CPP_CODE.append(" )) ");
                        // *** *** *** *** *** ***
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      " : " + SYNTAX[p + 2] + "()' ",
                                  o_tokens);
                    }
                } else if (MEMBER_TYPE == "نص") {
                    if (OBJECTIF_TYPE == "عدد") {
                        ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] +
                                      " : " + SYNTAX[p + 2] + "()' ",
                                  o_tokens);
                    } else if (OBJECTIF_TYPE == "نص") {
                        if (IS_GLOBAL_OBJ) {
                            if (DEBUG)
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-STRING) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                                            ".ClassFUNCTION_" +
                                            ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-STRING) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] +
                                            ".ClassFUNCTION_" +
                                            ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        }

                        CPP_CODE.append(CHECK_CALL_FUN_ARG(
                            false, TK_CLASS, SYNTAX[p + 2],
                            2,  // 2 = الدالة member
                            TheNamespace, TheFunction, TempToken,
                            (TempTokenCount - 1), o_tokens));

                        if (DEBUG) DEBUG_MESSAGE(")] ", o_tokens);  // DEBUG

                        // *** Generate Code ***
                        CPP_CODE.append(" ) ");
                        // *** *** *** *** *** ***
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      " : " + SYNTAX[p + 2] + "()' ",
                                  o_tokens);
                    }
                } else if (MEMBER_TYPE == "منطق") {
                    if (OBJECTIF_TYPE != "منطق") {
                        ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE +
                                      " ' " + SYNTAX[p] + " : " +
                                      SYNTAX[p + 2] + "()' ",
                                  o_tokens);
                    } else if (OBJECTIF_TYPE == "منطق") {
                        if (IS_GLOBAL_OBJ) {
                            if (DEBUG)
                                DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-BOOL) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] +
                                            ".ClassFUNCTION_" +
                                            ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        } else {
                            if (DEBUG)
                                DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] +
                                                  " ':'" + SYNTAX[p + 2] +
                                                  " '(Func-BOOL) (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] +
                                            ".ClassFUNCTION_" +
                                            ID[SYNTAX[p + 2]] + "( ");
                            // *** *** *** *** *** ***
                        }

                        CPP_CODE.append(CHECK_CALL_FUN_ARG(
                            false, TK_CLASS, SYNTAX[p + 2],
                            2,  // 2 = الدالة member
                            TheNamespace, TheFunction, TempToken,
                            (TempTokenCount - 1), o_tokens));

                        if (DEBUG) DEBUG_MESSAGE(")] ", o_tokens);  // DEBUG

                        // *** Generate Code ***
                        CPP_CODE.append(" ) ");
                        // *** *** *** *** *** ***
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      " : " + SYNTAX[p + 2] + "()' ",
                                  o_tokens);
                    }
                } else {
                    ErrorCode("علة : نوع المنتمي غير معروف ' " + MEMBER_TYPE +
                                  " -> " + SYNTAX[p] + " : " + SYNTAX[p + 2] +
                                  "()' ",
                              o_tokens);
                }

                p = TMP_FUN_LONG;  // point إلى next token بعد fun(...)
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
                    ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] +
                                  " " + SYNTAX[p] + " ' ",
                              o_tokens);

            if (CLASS_G_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                                SYNTAX[p])] == "عدد") {
                if (OBJECTIF_TYPE == "عدد") {
                    if (DEBUG)
                        DEBUG_MESSAGE("[CLASS-GLOBAL-INT (" + SYNTAX[p] + ")] ",
                                      o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                } else if (OBJECTIF_TYPE == "نص") {
                    if (ACCEPT_INT_TO_STRING) {
                        if (DEBUG)
                            DEBUG_MESSAGE("[CLASS-GLOBAL-INT (" + SYNTAX[p] +
                                              ").ToString] ",
                                          o_tokens);  // DEBUG

                        // *** Generate Code ***
                        CPP_CODE.append(" alifcore_IntToString(" +
                                        Global_ID[SYNTAX[p]] + ") ");
                        // *** *** *** *** *** ***
                    } else {
                        ErrorCode(
                            "لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ",
                            o_tokens);
                    }
                } else if (OBJECTIF_TYPE == "منطق") {
                    ErrorCode(
                        "لا يمكن تحويل عدد إلى منطق : ' " + SYNTAX[p] + " ' ",
                        o_tokens);
                } else if (OBJECTIF_TYPE == "C++") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[C++ CLASS-GLOBAL-INT (" + SYNTAX[p] + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                } else {
                    ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                  OBJECTIF_TYPE + " -> " + SYNTAX[p] + " ' ",
                              o_tokens);
                }
            } else if (CLASS_G_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                                       SYNTAX[p])] == "نص") {
                if (OBJECTIF_TYPE == "عدد") {
                    // if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" + SYNTAX[p] +
                    // ").ToInt]
                    // ", o_tokens); // DEBUG
                    ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ",
                              o_tokens);
                } else if (OBJECTIF_TYPE == "نص") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[CLASS-GLOBAL-STRING (" + SYNTAX[p] + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                } else if (OBJECTIF_TYPE == "منطق") {
                    ErrorCode(
                        "لا يمكن تحويل نص إلى منطق : ' " + SYNTAX[p] + " ' ",
                        o_tokens);
                } else if (OBJECTIF_TYPE == "C++") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[C++ CLASS-GLOBAL-STRING (" + SYNTAX[p] + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                } else {
                    ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                  OBJECTIF_TYPE + " -> " + SYNTAX[p] + " ' ",
                              o_tokens);
                }
            } else if (CLASS_G_VAR_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                                       SYNTAX[p])] == "منطق") {
                if (OBJECTIF_TYPE == "C++") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[C++ CLASS-GLOBAL-BOOL (" + SYNTAX[p] + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                } else if (OBJECTIF_TYPE != "منطق") {
                    ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE +
                                  " ' " + SYNTAX[p] + " ' ",
                              o_tokens);
                } else if (OBJECTIF_TYPE == "منطق") {
                    if (DEBUG)
                        DEBUG_MESSAGE(
                            "[CLASS-GLOBAL-BOOL (" + SYNTAX[p] + ")] ",
                            o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                } else {
                    ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                  OBJECTIF_TYPE + " -> " + SYNTAX[p] + " ' ",
                              o_tokens);
                }
            } else {
                ErrorCode(
                    "علة : نوع المتغير العام غير معروف ' " + SYNTAX[p] + " ' ",
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
                // if(DEBUG)DEBUG_MESSAGE("[STRING_MSG (" + SYNTAX[p] +
                // ").ToInt] ", o_tokens); // DEBUG
                ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ",
                          o_tokens);
            } else if (OBJECTIF_TYPE == "نص") {
                if (DEBUG)
                    DEBUG_MESSAGE("[STRING_MSG (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" (" + SYNTAX[p] + ") ");
                // *** *** *** *** *** ***
            } else if (OBJECTIF_TYPE == "منطق") {
                ErrorCode("لا يمكن تحويل نص إلى منطق ' " + SYNTAX[p] + " ' ",
                          o_tokens);
            } else if (OBJECTIF_TYPE == "C++") {
                if (DEBUG)
                    DEBUG_MESSAGE("[C++ نص (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" (" + SYNTAX[p] + ") ");
                // *** *** *** *** *** ***
            } else {
                ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                              " -> " + SYNTAX[p] + " ' ",
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
                    DEBUG_MESSAGE("[DIGIT " + SYNTAX[p] + "] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + SYNTAX[p] + " ");
                // *** *** *** *** *** ***
            } else if (OBJECTIF_TYPE == "نص") {
                if (ACCEPT_INT_TO_STRING) {
                    if (DEBUG)
                        DEBUG_MESSAGE("[DIGIT (" + SYNTAX[p] + ").ToString] ",
                                      o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" alifcore_IntToString(" + SYNTAX[p] +
                                    ") ");
                    // *** *** *** *** *** ***
                } else {
                    ErrorCode(
                        "لا يمكن تحويل رقم إلى نص : ' " + SYNTAX[p] + " '",
                        o_tokens);
                }
            } else if (OBJECTIF_TYPE == "منطق") {
                ErrorCode("لا يمكن تحويل رقم إلى منطق ' " + SYNTAX[p] + " ' ",
                          o_tokens);
            } else if (OBJECTIF_TYPE == "C++") {
                if (DEBUG)
                    DEBUG_MESSAGE("[C++ رقم (" + SYNTAX[p] + ")] ",
                                  o_tokens);  // DEBUG

                // *** Generate Code ***
                CPP_CODE.append(" " + SYNTAX[p] + " ");
                // *** *** *** *** *** ***
            } else {
                ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE +
                              " -> " + SYNTAX[p] + " ' ",
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
                                             SYNTAX[p])])  // call الدالة (a, b)
        {
            if (p > 0)
                if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
                    ErrorCode("لا يمكن وضع دالة هنا ' " + SYNTAX[p - 1] + " " +
                                  SYNTAX[p] + " ' ",
                              o_tokens);

            if (!IsInsideFunction)
                ErrorCode("يجب مناداة على الدالة من داخل دالة ' " + SYNTAX[p] +
                              "()' ",
                          o_tokens);

            if (OBJECTIF_TYPE == "مؤشر_دالة") {
                // ... function_name -> to an pointer-func

                std::string FUN_TYPE;
                bool IS_LOCAL_FUN = false;
                IS_LOCAL_FUN = L_FUN_IS_SET[std::make_pair(
                    TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];

                if (IS_LOCAL_FUN) {
                    FUN_TYPE = L_FUN_TYPE[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                                                         SYNTAX[p])];
                    if (DEBUG)
                        DEBUG_MESSAGE("[REFERENCE-OF-LOCAL-FUNCTION-" +
                                          FUN_TYPE + " ' " + SYNTAX[p] + " ']",
                                      o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" NS_" + ID[TheNamespace] + "::FUNCTION_" +
                                    ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                } else {
                    FUN_TYPE = G_FUN_TYPE[(SYNTAX[p])];
                    if (DEBUG)
                        DEBUG_MESSAGE("[REFERENCE-OF-GLOBAL-FUNCTION-" +
                                          FUN_TYPE + " ' " + SYNTAX[p] + " ']",
                                      o_tokens);  // DEBUG

                    // *** Generate Code ***
                    CPP_CODE.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + " ");
                    // *** *** *** *** *** ***
                }
            } else {
                // ... function_name(...) -> to an anythings except pointers

                if (SYNTAX[p + 1] == "")
                    ErrorCode("يجب اضافه '(' بعد ' " + SYNTAX[p] + " ' ",
                              o_tokens);

                if (SYNTAX[p + 1] != "(")
                    ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " ' ",
                              o_tokens);

                // bool ThisIsClassFunction = false;
                // if (CLASS_FUN_IS_SET[std::make_pair(TMP_NAMESPACE_OR_CLASS,
                // SYNTAX[p])])
                //   ThisIsClassFunction = true;

                // if(ThisIsClassFunction)
                // 	ErrorCode("_TEST_TEST_TEST_' " + SYNTAX[p + 1] + " ' ",
                // o_tokens);

                int TMP_FUN_LONG = p + 2;  // a + b + 'p'fun ( c + (1 * 2) ) + c
                int OPEN_PARENTIZE = 0;

                while (TMP_FUN_LONG <= SYNTAX_LONG) {
                    if (SYNTAX[TMP_FUN_LONG] ==
                        "(")  // مفتوح inside الدالة args : fun( a + (b))
                        OPEN_PARENTIZE++;
                    else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                             OPEN_PARENTIZE > 0)  // Close inside الدالة args
                        OPEN_PARENTIZE--;
                    else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                             OPEN_PARENTIZE < 1)  // Close final الدالة call
                    {
                        if (TMP_FUN_LONG < SYNTAX_LONG) {
                            // a = fun( a + (b)) + 123
                            // str / عدد = متغير + (fun(var)) * (fun(var) /
                            // fun(var, fun(var), var) - var)

                            if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "-") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "*") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "/") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != "،") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != ",") &&
                                (SYNTAX[TMP_FUN_LONG + 1] != ")")) {
                                ErrorCode("نص غير معروف بعد نداء ' " +
                                              SYNTAX[p] + "()' : ' " +
                                              SYNTAX[TMP_FUN_LONG + 1] + " ' ",
                                          o_tokens);
                            }
                        } else if (TMP_FUN_LONG == SYNTAX_LONG) {
                            // a = fun( a + (b))
                            if (SYNTAX[TMP_FUN_LONG] != ")")  // double check!
                                ErrorCode("يجب إنهاء نداء الدالة ' " +
                                              SYNTAX[p] + "()' بالإشارة ')' ",
                                          o_tokens);
                        }

                        break;
                    }

                    TMP_FUN_LONG++;
                }

                if (SYNTAX[TMP_FUN_LONG] != ")")  // Double check!
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
                if (IsInsideClass) {
                    IS_LOCAL_FUN = CLASS_FUN_IS_SET[std::make_pair(
                        TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];
                } else {
                    IS_LOCAL_FUN = L_FUN_IS_SET[std::make_pair(
                        TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];
                }

                std::string FUN_TYPE;

                if (IS_LOCAL_FUN) {
                    if (IsInsideClass) {
                        FUN_TYPE = CLASS_FUN_TYPE[std::make_pair(
                            TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];
                    } else {
                        FUN_TYPE = L_FUN_TYPE[std::make_pair(
                            TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];
                    }

                    if (FUN_TYPE == "عادم")
                        ErrorCode(
                            "الدالة المحلية ' " + SYNTAX[p] +
                                "()' من نوع عادم, لدى لا يمكن تحويلها إلى " +
                                OBJECTIF_TYPE,
                            o_tokens);
                } else {
                    FUN_TYPE = G_FUN_TYPE[(SYNTAX[p])];

                    if (FUN_TYPE == "عادم")
                        ErrorCode(
                            "الدالة العامة ' " + SYNTAX[p] +
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
                                DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-INT ' " +
                                                  SYNTAX[p] + " '] (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            if (IsInsideClass) {
                                CG_BUFFER.append(" ClassFUNCTION_" +
                                                 ID[SYNTAX[p]] + "( ");
                            } else {
                                CG_BUFFER.append(" NS_" + ID[TheNamespace] +
                                                 "::FUNCTION_" + ID[SYNTAX[p]] +
                                                 "( ");
                            }
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
                                DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-INT ' " +
                                                  SYNTAX[p] + " '] (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" FUNCTION_" +
                                             Global_ID[SYNTAX[p]] + "( ");
                            // *** *** *** *** *** ***

                            CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                                true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS,
                                TheFunction, TempToken, (TempTokenCount - 1),
                                o_tokens));

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" ) ");
                            // *** *** *** *** *** ***
                        }
                    } else if (FUN_TYPE == "نص") {
                        if (IS_LOCAL_FUN)
                            ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                          SYNTAX[p] + "()' من نص إلى عدد ",
                                      o_tokens);
                        else
                            ErrorCode("لا يمكن تحويل الدالة العامة ' " +
                                          SYNTAX[p] + "()' من نص إلى عدد ",
                                      o_tokens);
                    } else if (FUN_TYPE == "منطق") {
                        if (IS_LOCAL_FUN)
                            ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                          SYNTAX[p] + "()' من منطق إلى عدد ",
                                      o_tokens);
                        else
                            ErrorCode("لا يمكن تحويل الدالة العامة ' " +
                                          SYNTAX[p] + "()' من منطق إلى عدد ",
                                      o_tokens);
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " [" + FUN_TYPE + "] " +
                                      " [" + TMP_NAMESPACE_OR_CLASS + "] " +
                                      SYNTAX[p] + "()' ",
                                  o_tokens);
                    }
                } else if (OBJECTIF_TYPE == "نص") {
                    if (FUN_TYPE == "عدد") {
                        if (IS_LOCAL_FUN) {
                            // Call a local fun int.ToString

                            if (DEBUG)
                                DEBUG_MESSAGE(
                                    "[CALL-LOCAL-FUNCTION-INT.ToString ' " +
                                        SYNTAX[p] + " '] (",
                                    o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(
                                " alifcore_IntToString( OBJ_CLASS_WINDOW_" +
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
                                DEBUG_MESSAGE(
                                    "[CALL-GLOBAL-FUNCTION-INT.ToString ' " +
                                        SYNTAX[p] + " '] (",
                                    o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(
                                " alifcore_IntToString( FUNCTION_" +
                                Global_ID[SYNTAX[p]] + "( ");
                            // *** *** *** *** *** ***

                            CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                                true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS,
                                TheFunction, TempToken, (TempTokenCount - 1),
                                o_tokens));

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" ) ) ");
                            // *** *** *** *** *** ***
                        }
                    } else if (FUN_TYPE == "نص") {
                        if (IS_LOCAL_FUN) {
                            // Call a local fun std::string

                            if (DEBUG)
                                DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-STRING ' " +
                                                  SYNTAX[p] + " '] (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" NS_" + ID[TheNamespace] +
                                             "::FUNCTION_" + ID[SYNTAX[p]] +
                                             "( ");
                            // *** *** *** *** *** ***

                            CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                                false, TMP_NAMESPACE_OR_CLASS, SYNTAX[p], 0,
                                TMP_NAMESPACE_OR_CLASS, TheFunction, TempToken,
                                (TempTokenCount - 1), o_tokens));

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" ) ");
                            // *** *** *** *** *** ***

                            if (DEBUG) DEBUG_MESSAGE(") ", o_tokens);  // DEBUG
                        } else {
                            // Call a Global fun int

                            if (DEBUG)
                                DEBUG_MESSAGE(
                                    "[CALL-GLOBAL-FUNCTION-STRING ' " +
                                        SYNTAX[p] + " '] (",
                                    o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" FUNCTION_" +
                                             Global_ID[SYNTAX[p]] + "( ");
                            // *** *** *** *** *** ***

                            CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                                true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS,
                                TheFunction, TempToken, (TempTokenCount - 1),
                                o_tokens));

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" ) ");
                            // *** *** *** *** *** ***
                        }
                    } else if (FUN_TYPE == "منطق") {
                        if (IS_LOCAL_FUN)
                            ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                          SYNTAX[p] + "()' من منطق إلى نص ",
                                      o_tokens);
                        else
                            ErrorCode("لا يمكن تحويل الدالة العامة ' " +
                                          SYNTAX[p] + "()' من منطق إلى نص ",
                                      o_tokens);
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      "()' ",
                                  o_tokens);
                    }
                } else if (OBJECTIF_TYPE == "منطق") {
                    if (FUN_TYPE == "عدد") {
                        if (IS_LOCAL_FUN)
                            ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                          SYNTAX[p] + "()' من عدد إلى منطق ",
                                      o_tokens);
                        else
                            ErrorCode("لا يمكن تحويل الدالة العامة ' " +
                                          SYNTAX[p] + "()' من عدد إلى منطق ",
                                      o_tokens);
                    } else if (FUN_TYPE == "نص") {
                        if (IS_LOCAL_FUN)
                            ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                          SYNTAX[p] + "()' من نص إلى منطق ",
                                      o_tokens);
                        else
                            ErrorCode("لا يمكن تحويل الدالة العامة ' " +
                                          SYNTAX[p] + "()' من نص إلى منطق ",
                                      o_tokens);
                    } else if (FUN_TYPE == "منطق") {
                        if (IS_LOCAL_FUN) {
                            // Call a local fun bool

                            if (DEBUG)
                                DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-BOOL ' " +
                                                  SYNTAX[p] + " '] (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" NS_" + ID[TheNamespace] +
                                             "::FUNCTION_" + ID[SYNTAX[p]] +
                                             "( ");
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
                                DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-BOOL ' " +
                                                  SYNTAX[p] + " '] (",
                                              o_tokens);  // DEBUG

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" FUNCTION_" +
                                             Global_ID[SYNTAX[p]] + "( ");
                            // *** *** *** *** *** ***

                            CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                                true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS,
                                TheFunction, TempToken, (TempTokenCount - 1),
                                o_tokens));

                            // *** Generate Code ***
                            // Buffer
                            CG_BUFFER.append(" ) ");
                            // *** *** *** *** *** ***
                        }
                    } else {
                        ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                      OBJECTIF_TYPE + " -> " + SYNTAX[p] +
                                      "()' ",
                                  o_tokens);
                    }
                } else if (OBJECTIF_TYPE == "C++") {
                    // xType: We igiore type of function.. this is C++ targed
                    // user must take care of this!.

                    if (IS_LOCAL_FUN) {
                        // Call a local fun xType

                        if (DEBUG)
                            DEBUG_MESSAGE("[C++ CALL-LOCAL-FUNCTION-xType ' " +
                                              SYNTAX[p] + " '] (",
                                          o_tokens);  // DEBUG

                        // *** Generate Code ***
                        // Buffer
                        CG_BUFFER.append(" NS_" + ID[TheNamespace] +
                                         "::FUNCTION_" + ID[SYNTAX[p]] + "( ");
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
                        // Call a Global fun xType

                        if (DEBUG)
                            DEBUG_MESSAGE("[C++ CALL-GLOBAL-FUNCTION-xType ' " +
                                              SYNTAX[p] + " '] (",
                                          o_tokens);  // DEBUG

                        // *** Generate Code ***
                        // Buffer
                        CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] +
                                         "( ");
                        // *** *** *** *** *** ***

                        CG_BUFFER.append(CHECK_CALL_FUN_ARG(
                            true, "", SYNTAX[p], 0, TMP_NAMESPACE_OR_CLASS,
                            TheFunction, TempToken, (TempTokenCount - 1),
                            o_tokens));

                        // *** Generate Code ***
                        // Buffer
                        CG_BUFFER.append(" ) ");
                        // *** *** *** *** *** ***
                    }
                }

                // *** Generate Code ***
                CPP_CODE.append(" " + CG_BUFFER + " ");
                // *** *** *** *** *** ***

                p = TMP_FUN_LONG;  // point to next token after fun(...)
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
                // abc = namespace:local_func(a, b)	| namespace:my_func(a,
                // b) abc = namespace:local_var		| namespace:my_var

                //	namespace_name	:		local_func (
                //... ) 	p				+1		+2
                //+3

                if (SYNTAX[p + 1] != ":")
                    ErrorCode("يجب اضافه ' : ' بعد ' " + SYNTAX[p] + " ' ",
                              o_tokens);

                if (SYNTAX[p + 2] == "")
                    ErrorCode(
                        "يجب اضافه عضو تابع ل ' " + SYNTAX[p] + " ' بعد ':' ",
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
                    if (L_FUN_IS_SET[std::make_pair(SYNTAX[p],
                                                    SYNTAX[p + 2])]) {
                        // ---------------------------------------------------------------------------------
                        // namespace : member_function ()
                        // ---------------------------------------------------------------------------------

                        // abc = namespace:local_func(a, b) |
                        // namespace:my_func(a, b)

                        if (SYNTAX[p + 3] != "(")
                            ErrorCode("من اجل نداء الدالة ' " + SYNTAX[p + 2] +
                                          " ' يجب اضافه '()' بعد ' " +
                                          SYNTAX[p] + " " + SYNTAX[p + 1] +
                                          " " + SYNTAX[p + 2] + " ' ",
                                      o_tokens);

                        std::string FUN_TYPE;
                        std::string CPP_END;

                        FUN_TYPE = L_FUN_TYPE[std::make_pair(
                            TMP_NAMESPACE_OR_CLASS, SYNTAX[p])];

                        if (FUN_TYPE == "عادم")
                            ErrorCode("الدالة المحلية ' " + SYNTAX[p] +
                                          "()' من نوع عادم, لدى لا يمكن "
                                          "تحويلها إلى " +
                                          OBJECTIF_TYPE,
                                      o_tokens);

                        if (OBJECTIF_TYPE == "عدد") {
                            if (FUN_TYPE == "عدد") {
                                if (DEBUG)
                                    DEBUG_MESSAGE("[NS'" + SYNTAX[p] +
                                                      " ':LOCAL_INT_FUNC'" +
                                                      SYNTAX[p + 2] + " '( ",
                                                  o_tokens);  // DEBUG

                                // *** Generate Code ***
                                // Buffer
                                CPP_END = " ";
                                CPP_CODE.append(" NS_" + ID[SYNTAX[p]] +
                                                "::FUNCTION_" +
                                                ID[SYNTAX[p + 2]] + "( ");
                                // *** *** *** *** *** ***
                            } else {
                                ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                              SYNTAX[p + 2] + "()' من " +
                                              FUN_TYPE + " إلى عدد ",
                                          o_tokens);
                            }
                        } else if (OBJECTIF_TYPE == "نص") {
                            if (FUN_TYPE == "نص") {
                                if (DEBUG)
                                    DEBUG_MESSAGE("[NS'" + SYNTAX[p] +
                                                      " ':LOCAL_STRING_FUNC'" +
                                                      SYNTAX[p + 2] + " '( ",
                                                  o_tokens);  // DEBUG

                                // *** Generate Code ***
                                // Buffer
                                CPP_END = " ";
                                CPP_CODE.append(" NS_" + ID[SYNTAX[p]] +
                                                "::FUNCTION_" +
                                                ID[SYNTAX[p + 2]] + "( ");
                                // *** *** *** *** *** ***
                            } else if (FUN_TYPE == "عدد") {
                                if (DEBUG)
                                    DEBUG_MESSAGE("[NS'" + SYNTAX[p] +
                                                      " ':LOCAL_INT_FUNC'" +
                                                      SYNTAX[p + 2] +
                                                      " '().ToString( ",
                                                  o_tokens);  // DEBUG

                                // *** Generate Code ***
                                // Buffer
                                CPP_END = " ) ";
                                CPP_CODE.append(
                                    " alifcore_IntToString( OBJ_CLASS_WINDOW_" +
                                    ID[SYNTAX[p]] + "::FUNCTION_" +
                                    ID[SYNTAX[p + 2]] + "( ");
                                // *** *** *** *** *** ***
                            } else {
                                ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                              SYNTAX[p + 2] + "()' من " +
                                              FUN_TYPE + " إلى نص ",
                                          o_tokens);
                            }
                        } else if (OBJECTIF_TYPE == "منطق") {
                            if (FUN_TYPE == "منطق") {
                                if (DEBUG)
                                    DEBUG_MESSAGE("[NS'" + SYNTAX[p] +
                                                      " ':LOCAL_BOOL_FUNC'" +
                                                      SYNTAX[p + 2] + " '( ",
                                                  o_tokens);  // DEBUG

                                // *** Generate Code ***
                                // Buffer
                                CPP_END = " ";
                                CPP_CODE.append(" NS_" + ID[SYNTAX[p]] +
                                                "::FUNCTION_" +
                                                ID[SYNTAX[p + 2]] + "( ");
                                // *** *** *** *** *** ***
                            } else {
                                ErrorCode("لا يمكن تحويل الدالة المحلية ' " +
                                              SYNTAX[p + 2] + "()' من " +
                                              FUN_TYPE + " إلى منطق ",
                                          o_tokens);
                            }
                        } else
                            ErrorCode("علة : نوع المستهدف غير معروف ' " +
                                          OBJECTIF_TYPE + " -> " +
                                          SYNTAX[p + 2] + " ' ",
                                      o_tokens);

                        // abc = (p)namespace:local_func(a, b) + x + y

                        int TMP_FUN_LONG = p + 4;
                        int OPEN_PARENTIZE = 0;

                        // Get Local Function Args
                        while (TMP_FUN_LONG <= SYNTAX_LONG) {
                            if (SYNTAX[TMP_FUN_LONG] ==
                                "(")  // مفتوح inside الدالة args : fun( a +
                                      // (b))
                                OPEN_PARENTIZE++;
                            else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                                     OPEN_PARENTIZE >
                                         0)  // Close inside الدالة args
                                OPEN_PARENTIZE--;
                            else if (SYNTAX[TMP_FUN_LONG] == ")" &&
                                     OPEN_PARENTIZE <
                                         1)  // Close final الدالة call
                            {
                                if (TMP_FUN_LONG < SYNTAX_LONG) {
                                    // abc = fun( a + (b)) + 123
                                    // abc = x + (fun(var)) * (fun(var) /
                                    // fun(var, fun(var), var)
                                    // - var)

                                    if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
                                        (SYNTAX[TMP_FUN_LONG + 1] != "-") &&
                                        (SYNTAX[TMP_FUN_LONG + 1] != "*") &&
                                        (SYNTAX[TMP_FUN_LONG + 1] != "/") &&
                                        (SYNTAX[TMP_FUN_LONG + 1] != "،") &&
                                        (SYNTAX[TMP_FUN_LONG + 1] != ",") &&
                                        (SYNTAX[TMP_FUN_LONG + 1] != ")")) {
                                        ErrorCode("نص غير معروف بعد نداء ' " +
                                                      SYNTAX[p + 2] +
                                                      "()' : ' " +
                                                      SYNTAX[TMP_FUN_LONG + 1] +
                                                      " ' ",
                                                  o_tokens);
                                    }
                                } else if (TMP_FUN_LONG == SYNTAX_LONG) {
                                    // a = fun( a + (b))
                                    if (SYNTAX[TMP_FUN_LONG] != ")" ||
                                        SYNTAX[SYNTAX_LONG] !=
                                            ")")  // double check!
                                        ErrorCode("يجب إنهاء نداء الدالة ' " +
                                                      SYNTAX[p + 2] +
                                                      "()' بالإشارة ')' ",
                                                  o_tokens);
                                }

                                break;
                            }

                            TMP_FUN_LONG++;
                        }

                        if (SYNTAX[TMP_FUN_LONG] != ")")  // Double check!
                            ErrorCode("يجب إنهاء نداء الدالة ' " +
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

                        // Check local fun Args : fun (a + c, 2 * (b - 1))
                        CPP_CODE.append(CHECK_CALL_FUN_ARG(
                            false, SYNTAX[p], SYNTAX[p + 2], 0, TheNamespace,
                            TheFunction, TempToken, (TempTokenCount - 1),
                            o_tokens));

                        if (DEBUG) DEBUG_MESSAGE(")] \n\n", o_tokens);  // DEBUG

                        // *** Generate Code ***
                        CPP_CODE.append(" ) " + CPP_END + " ");
                        // *** *** *** *** *** ***

                        // Point to last token of full func call
                        p = TMP_FUN_LONG;

                        continue;
                    }

                    // TODO: Add support for local var
                    // if (L_VAR_IS_SET[...]) <-- abc[namespace][var] = local
                    // var already set ?

                    else
                        ErrorCode("النافذة ' " + SYNTAX[p] +
                                      " ' لا تحتوي على دالة محليه بإسم ' " +
                                      SYNTAX[p + 2] + " ' ",
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
        // ERROR
        // -----------------------------------

        else {
            // أو
            if (SYNTAX[p] == "او" || SYNTAX[p] == "ٱو" || SYNTAX[p] == "آو" ||
                SYNTAX[p] == "والا" || SYNTAX[p] == "وإلا" ||
                SYNTAX[p] == "وألا") {
                ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] +
                              " '، هل تقصد ' أو ' ؟ ",
                          o_tokens);
            } else {
                if (DEBUG)
                    DEBUG_MESSAGE("\nTMP_NAMESPACE_OR_CLASS --> '" +
                                      TMP_NAMESPACE_OR_CLASS + "'\n ",
                                  o_tokens);  // DEBUG
                if (DEBUG)
                    DEBUG_MESSAGE("TmpFunction --> '" + TmpFunction + "'\n ",
                                  o_tokens);  // DEBUG
                if (DEBUG)
                    DEBUG_MESSAGE("SYNTAX[p] --> '" + SYNTAX[p] + "'\n ",
                                  o_tokens);  // DEBUG

                ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] + " ' ",
                          o_tokens);
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
    std::string CALL_WIN_OR_CLASS,  // win1/class1 { fun1(int a) } | win2 {
                                    // fun2{ عدد b; fun1(b) } } ==> win1
    std::string CALL_FUN,  // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
                           // } } ==> fun1
    int CALL_IS_CLASS,  // 0 = non class, 1 constructor, 2 = الدالة member, ل
                        // Message when new obj
    std::string FROM_WIN_OR_CLASS,  // win1 { fun1(int a) } | win2 { fun2{ عدد
                                    // b; fun1(b) } } ==> win2
    std::string FROM_FUN,  // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b)
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

    if (CALL_FUN_GLOBAL) {
        // Call Global Fun

        CALL_ARG_TOTAL = G_FUN_ARG_TOTAL[CALL_FUN];

        // check args
        if (CALL_ARG_TOTAL > 0 && SYNTAX_LONG < 1)
            ErrorCode("الدالة العامة ' " + CALL_FUN + "()' تأخد " +
                          IntToString(CALL_ARG_TOTAL) + " خاصية",
                      o_tokens);

        // الدالة العامة () without args
        if (CALL_ARG_TOTAL == 0 && SYNTAX_LONG == 0) return "";
    } else {
        // Call Local Fun

        CALL_ARG_TOTAL =
            L_FUN_ARG_TOTAL[std::make_pair(CALL_WIN_OR_CLASS, CALL_FUN)];

        // check args
        if (CALL_ARG_TOTAL > 0 && SYNTAX_LONG < 1)
            ErrorCode("الدالة المحلية ' " + CALL_FUN + "()' تأخد " +
                          IntToString(CALL_ARG_TOTAL) + " خاصية",
                      o_tokens);

        // الدالة المحلية () without args
        if (CALL_ARG_TOTAL == 0 && SYNTAX_LONG == 0) return "";
    }

    // الدالة (int a, عدد b) with args
    while (p <= SYNTAX_LONG)  // using while is bcs need 'if p <
                              // G_FUN_ARG_TOTAL' بعد while finish
    {
        // if(DEBUG)DEBUG_MESSAGE("\n --[" + SYNTAX[p] + "|" +
        // G_FUN_ARG_TYPE[std::make_pair(CALL_FUN, p)] + "]-- \n", o_tokens); //
        // DEBUG

        if (SYNTAX[p] != "," && SYNTAX[p] != "،" && p < SYNTAX_LONG) {
            if (DEBUG)
                DEBUG_MESSAGE("-[ New ARG: " + SYNTAX[p] + " ]-",
                              o_tokens);  // DEBUG

            ARG[ARG_LONG] = SYNTAX[p];
            ARG_LONG++;
        } else {
            // if(DEBUG)DEBUG_MESSAGE("-[" +
            // CONVERT_STRING_ARRAY_TO_STRING(ARG,ARG_LONG) + "]-", o_tokens);
            // // DEBUG ErrorCode("|G_FUN_ARG_TYPE TmpFunction: " + TmpFunction
            // + " p : "
            // + IntToString(p) + "|", o_tokens );

            if (ARG_LONG < 2)  // الدالة ( , ...) ['0'] =, [1] 'user arg', [2]
                               // null ل incrumentation
                ErrorCode("إحدى الخصائص فارغة", o_tokens);

            if (CALL_FUN_GLOBAL) {
                // نداء a الدالة العامة ( ... check ARG n ...)

                // الدالة (a,a,a) ل (a,a)
                if ((CURRENT_ARG_NUMBER + 1) > CALL_ARG_TOTAL)
                    ErrorCode("خصائص أكثر من الازم، الدالة العامة ' " +
                                  CALL_FUN + "()' تأخد فقط " +
                                  IntToString(CALL_ARG_TOTAL) + " خاصية ",
                              o_tokens);

                // Current خاصية OBJECTIF_TYPE
                CPP_CODE.append(CheckForSyntax(
                    G_FUN_ARG_TYPE[std::make_pair(CALL_FUN,
                                                  CURRENT_ARG_NUMBER)],
                    true,   // Accept Using Reference إلى namespace:Controls
                    true,   // Accept Using Reference إلى namespace:Function
                    true,   // Accept Using Reference إلى Global Functions
                    true,   // Accept Using Reference إلى Local Functions
                    true,   // Accept Using Reference إلى Global VAR
                    true,   // Accept Using Reference إلى Local VAR
                    false,  // Accept Convertion من نص إلى Int
                    true,   // Accept Convertion من عدد إلى String
                    ARG,    // SYNTAX[] std::string
                    (ARG_LONG - 1),     // SYNTAX_LONG int
                    FROM_WIN_OR_CLASS,  // TMP_WINDOW_NAME
                    FROM_FUN,           // TMP_FUNCTION_NAME
                    o_tokens));
            } else {
                // نداء a الدالة المحلية ( ... check ARG n ...)

                // الدالة (a,a,a) ل (a,a)
                if ((CURRENT_ARG_NUMBER + 1) > CALL_ARG_TOTAL) {
                    if (CALL_IS_CLASS == 1)  // constructor
                        ErrorCode("خصائص أكثر من ألازم، دالة بناء الصنف ' " +
                                      CALL_WIN_OR_CLASS + " ' تأخد فقط " +
                                      IntToString(CALL_ARG_TOTAL) + " خاصية ",
                                  o_tokens);
                    else if (CALL_IS_CLASS == 2)  // الدالة member
                        ErrorCode("خصائص أكثر من ألازم، الدالة ' " + CALL_FUN +
                                      " ' المنتمية للصنف ' " +
                                      CALL_WIN_OR_CLASS + " ' تأخد فقط " +
                                      IntToString(CALL_ARG_TOTAL) + " خاصية ",
                                  o_tokens);
                    else
                        ErrorCode("خصائص أكثر من ألازم، الدالة ' " + CALL_FUN +
                                      " ' تأخد فقط " +
                                      IntToString(CALL_ARG_TOTAL) + " خاصية ",
                                  o_tokens);
                }

                // if(DEBUG)DEBUG_MESSAGE("-[ " +
                // CONVERT_STRING_ARRAY_TO_STRING(ARG, ARG_LONG) + " ]-",
                // o_tokens); // DEBUG ErrorCode(" Yep! ", o_tokens);

                // Current خاصية OBJECTIF_TYPE
                CPP_CODE.append(CheckForSyntax(
                    L_FUN_ARG_TYPE[std::make_pair(CALL_WIN_OR_CLASS + CALL_FUN,
                                                  CURRENT_ARG_NUMBER)],
                    true,   // Accept Using Reference إلى namespace:Controls
                    true,   // Accept Using Reference إلى namespace:Function
                    true,   // Accept Using Reference إلى Global Functions
                    true,   // Accept Using Reference إلى Local Functions
                    true,   // Accept Using Reference إلى Global VAR
                    true,   // Accept Using Reference إلى Local VAR
                    false,  // Accept Convertion من نص إلى Int
                    true,   // Accept Convertion من عدد إلى String
                    ARG,    // SYNTAX[] std::string
                    (ARG_LONG - 1),     // SYNTAX_LONG int
                    FROM_WIN_OR_CLASS,  // TMP_WINDOW_NAME
                    FROM_FUN,           // TMP_FUNCTION_NAME
                    o_tokens));
            }

            ARG[0] = "=";
            ARG_LONG = 1;  // Point إلى next arg writed بالإشارة user
            CURRENT_ARG_NUMBER++;  // Point إلى next يدعى الدالة arg

            if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL) {
                if (DEBUG) DEBUG_MESSAGE(", ", o_tokens);  // DEBUG
                CPP_CODE.append(" , ");
            }
        }

        p++;
    }

    if (CALL_FUN_GLOBAL) {
        // الدالة (a,a,a) ل (a,a)
        if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL)
            ErrorCode("خصائص قليلة, الدالة العامة ' " + CALL_FUN + "()' تأخد " +
                          IntToString(CALL_ARG_TOTAL) +
                          " خاصية، ثم إنشاء فقط " +
                          IntToString(CURRENT_ARG_NUMBER) + " خاصية ",
                      o_tokens);
    } else {
        // الدالة (a,a,a) ل (a,a)
        if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL) {
            if (CALL_IS_CLASS == 1)  // constructor
                ErrorCode("خصائص قليلة, دالة بناء الصنف ' " +
                              CALL_WIN_OR_CLASS + "()' تأخد " +
                              IntToString(CALL_ARG_TOTAL) +
                              " خاصية، ثم إنشاء فقط " +
                              IntToString(CURRENT_ARG_NUMBER) + " خاصية ",
                          o_tokens);
            else if (CALL_IS_CLASS == 2)  // الدالة member
                ErrorCode("خصائص قليلة, الدالة منتمي ' " + CALL_FUN +
                              " ' داخل الصنف ' " + CALL_WIN_OR_CLASS +
                              "()' تأخد " + IntToString(CALL_ARG_TOTAL) +
                              " خاصية، ثم إنشاء فقط " +
                              IntToString(CURRENT_ARG_NUMBER) + " خاصية ",
                          o_tokens);
            else
                ErrorCode("خصائص قليلة, الدالة المحلية ' " + CALL_FUN +
                              "()' تأخد " + IntToString(CALL_ARG_TOTAL) +
                              " خاصية، ثم إنشاء فقط " +
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
    if (IsInsideClass) ErrorCode("يجب إغلاق الصنف : " + TheClass, o_tokens);

    // IF
    if (ALIF_IF_STATUS > 0)
        ErrorCode(
            "مازال هناك " + IntToString(ALIF_IF_STATUS) + " شروط مازالت مفتوحه",
            o_tokens);

    // WHILE
    if (ALIF_LOOP_STATUS > 0)
        ErrorCode("مازال هناك " + IntToString(ALIF_LOOP_STATUS) +
                      " كلما مازالت مفتوحه",
                  o_tokens);

    // #Alif
    if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
        ErrorCode(
            "يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى",
            o_tokens);
}

// ====================================================

void FINAL_APPLICATION_CODE_CHECKING(CLASS_TOKEN *o_tokens, bool FIRST_FILE) {
    if (FIRST_FILE) {
        if (DEBUG)
            DEBUG_MESSAGE("\n ----------- FINAL CHECK ---------- \n",
                          o_tokens);  // DEBUG

        // #Alif
        if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
            ErrorCode(
                "يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى",
                o_tokens);

        // Main
        if (!script.main.is_set)
            ErrorCode(
                "يجب إنشاء الدالة الرئيسية، لأنها تعتبر المدخل الرئيسي لهذا "
                "التطبيق.",
                o_tokens);
    }
}

// Setup ********************************************************

#ifdef _WIN32

std::string GET_WORKING_PATH_WIN32() {
    char working_directory[MAX_PATH + 1];
    GetCurrentDirectoryA(sizeof(working_directory),
                         working_directory);  // **** win32 specific ****
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

    if (POSITION < 1)  // To avoid ABC/DEF/ or Relative Path
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

    std::string Last_Char = PATH.substr(PATH.length() - 1);  // PATH.back()

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

bool CHECK_SETUP()  // std::string ARGV_0, std::string OUTPUT)
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
            "alif [source.alif] [output.exe] [logfile.txt] "
            "-workpath=C:\\Alif ");
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
    // /Library/Application
    // Support/Aliflang/Alif_Compiler/linux_alif_version.inf
    // /Library/Application
    // Support/Aliflang/Alif_Compiler/Alif_Arabic_Programming_Language
    //		/Library/Application Support/Aliflang/Alif_Compiler/alif.icns
    //		/Library/Application Support/Aliflang/Alif_Compiler/Info.plist

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
        "/Library/Application "
        "Support/Aliflang/Alif_Compiler/aliflib/aliflib.inf";

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
        ALIF_ERROR(
            "ERROR: Alif can't run from a path contain the word 'Windows', "
            "because of a GNU LD Bug. \nPath: " +
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
    AlifExitFailure();
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
    if (type == STACK) code_stack.append(c);
    if (type == CODE) code.append(c);
    if (type == ENTRY_POINT) code_entry_point.append(c);
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
std::map<std::pair<std::string, std::string>, std::string>CPP_WINDOW;  // [window][option] = value

// Platformes

// PC Console
std::string CODE_GENERATOR_GET_PC_CONSOLE() { return ""; }

// PC GUI

// ----------------------------------
// C++ Add Code to the current Function
// ----------------------------------

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
            // CPP_WINDOW[std::make_pair(TheNamespace, "LOAD")] = CBUFER +
            // NEW_CODE;

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
                 CPP_WINDOW[std::make_pair(Namespace_Total_Names[i], "LOAD")] +
                 R"(
	}
	// ========================================================================

			)";

        CODE.append(BUFFER);
    }

    return CODE;
}

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
        "25";  // Because the macOS task-bar is on top screen.
#else
    CPP_WINDOW[std::make_pair("رئيسية", "افصول")] = "0";
    CPP_WINDOW[std::make_pair("رئيسية", "ارتوب")] = "0";
#endif
    CPP_WINDOW[std::make_pair("رئيسية", "عرض")] = "400";
    CPP_WINDOW[std::make_pair("رئيسية", "ارتفاع")] = "400";

    CPP_WINDOW[std::make_pair("رئيسية", "شكل")] =
        "wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | "
        "wxCLIP_CHILDREN";
    // wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX |
    // wxCLIP_CHILDREN) wxMAXIMIZE_BOX

    Namespace_Total = 0;
    Namespace_Total_Names[0] = "رئيسية";
    L_FUN_TYPE[std::make_pair("رئيسية", "رئيسية")] = "عادم";
}

// ----------------------------------
// C++ CODE INITIALIZATION
// ----------------------------------



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

void set_input_file(std::string file) {
    boost::filesystem::path p(file);

    std::string p_buf = p.parent_path().string();
    if (p_buf.empty()) p_buf = boost::filesystem::current_path().string();

    argument.input.path = p_buf + settings.os.path_sep;
    argument.input.filename = p.stem().string();
    argument.input.extension = p.extension().string();

    // Check root path
    if (argument.input.path == settings.os.path_sep) argument.input.path = "";

    // Check input extention
    if (argument.input.extension != ".alif")
        err("Unsupported file extention.\n"
            "Extention: " +
            argument.input.extension +
            "\n"
            "File: " +
            argument.input.path + argument.input.filename +
            argument.input.extension);

    argument.input.fullpath = argument.input.path + argument.input.filename +
                              argument.input.extension;
}

void set_output_file(std::string file) {
    boost::filesystem::path p(file);

    argument.output.path = p.parent_path().string() + settings.os.path_sep;
    argument.output.filename = p.stem().string();
    argument.output.extension = p.extension().string();

    // Check root path
    if (argument.output.path == settings.os.path_sep) argument.output.path = "";

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
    if (argument.log.path == settings.os.path_sep) argument.log.path = "";

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
    if (argument.log.filename == "") err("Log file name is empty.");

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

    olen = len * 4 / 3 + 4;      /* 3-byte blocks to 4-byte */
    olen += olen / 72;           /* line feeds */
    olen++;                      /* nul termination */
    if (olen < len) return NULL; /* integer overflow */
    out = (unsigned char *)malloc(olen);
    if (out == NULL) return NULL;

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

    if (line_len) *pos++ = '\n';

    *pos = '\0';
    if (out_len) *out_len = pos - out;
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
        if (dtable[src[i]] != 0x80) count++;
    }

    if (count == 0 || count % 4) return NULL;

    olen = count / 4 * 3;
    pos = out = (unsigned char *)malloc(olen);
    if (out == NULL) return NULL;

    count = 0;
    for (i = 0; i < len; i++) {
        tmp = dtable[src[i]];
        if (tmp == 0x80) continue;

        if (src[i] == '=') pad++;
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
                    if (out) free(out);
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

    pbuf = _base64_encode((const unsigned char *)data.c_str(), data.length(),
                          &len);
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

    pbuf = _base64_decode((const unsigned char *)data.c_str(), data.length(),
                          &len);
    buf = reinterpret_cast<char *>(pbuf);
    // alifcore_trim(buf);

    // alifcore_free(pbuf);

    // std::cout << "Decode: [" << data << "] -> [" << buf << "] (" << len << "
    // Bytes) \n";

    return buf;
}
