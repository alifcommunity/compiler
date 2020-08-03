// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

#pragma once

// -----------------------------------------------------------
// Reserved Words
// -----------------------------------------------------------

// ألف أضف مكتبة رئيسية _س_ واجهة خاص
// نهاية كلما نافدة دالة عدد نص كائن إذا و أو سطر إرجاع صنف أداة نقر زر نص
// ملصق إظهار إخفاء تدمير عنوان نص تجميد عرض محتوى ارتفاع أفصول أرتوب 

static const string ALIF_RESERVED[] = {	"ألف", "أضف", "مكتبة", "واجهة", "_س_", "نافذة", "إرجاع", "نهاية", 
										"صنف", "خاص", "عدد", "نص", "كائن", "دالة", "هدم", "بناء",
										"كلما", "إذا", "أو", "و", "وإلا", "سطر", "أداة", "نقر", "زر", "ملصق", 
										"صحيح", "خطأ", "كسر", "متغير", "ثابت", "منطق", "طرفية"
										 }; // "رئيسية"

										// "عرض", "محتوى", "ارتفاع", "أفصول", "أرتوب", "تدمير", "عنوان", "تجميد", "إظهار", "إخفاء"
										
static const int ALIF_RESERVED_TOTAL = 33;

static const std::vector<std::string> DataType_v { "منطق", "نص", "عدد" };

// -----------------------------------------------------------
// Lexic Tokens Class
// Create Obj for every Alif Source files
// -----------------------------------------------------------

string TempToken [1024];
int TempTokenCount = 1;

class CLASS_TOKEN
{
	public:

		// ----------------------------------
		// Script Type
		// ----------------------------------
		
		// to block setting Alif-Window inside librarys
		string 	ALIF_SCRIPT_TYPE;
		
		// ----------------------------------
		// File Path
		// ----------------------------------
		
		// usr/username/Desktop/myproject/src/mysource.alif
		// Used only by Lexer()
		string PATH_FULL_SOURCE;
		
		// ----------------------------------
		// Tokens
		// ----------------------------------

		string REAL_LINE[2048];										// The real Alif script line, without toknonisation !
		std::map<std::pair<int, int>, int> REAL_TOKEN_POSITION;		// The real Alif script token position in real line !
																	// [Line Number, Token Number] = "real char number"
		
		bool TOKENS_PREDEFINED = false;

		std::map<std::pair<int, int>, string> TOKEN; // TOKEN [Line Number, Token Number] = "token"
		
		int TOTAL [2048];
		int TOTAL_LINES;
		int NUMBER;
		int Line;
		
		// ----------------------------------
		// Constructor
		// ----------------------------------
		
		CLASS_TOKEN()
		{
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

static string APP_TYPE = "PC_CONSOLE";

// ----------------------------------
// Flag
// ----------------------------------

static std::map<string, bool>	ALIF_FLAG_FILE;			// alif_flag[full_file_path] = true or false
static std::map<string, string>	ALIF_FLAG_AT_LINE_FILE;	// alif_flag[full_file_path] = at_line

// ----------------------------------
// صنف
// ----------------------------------

static std::map<string, bool>							CLASS_CONSTRICTOR_FUN_IS_SET;		// abc[class] = main_func true or false
static std::map<string, string>							CLASS_CONSTRICTOR_FUN_AT_LINE;		// abc[class] = main_func_at_line
static std::map<string, string>							CLASS_CONSTRICTOR_FUN_ARG_TOTAL;	// abc[class] = total args
static std::map<std::pair<string, int>, string>			CLASS_CONSTRICTOR_FUN_ARG_TYPE;		// abc[class][arg number] = arg type

static bool												IsInsideClass = false;
static string											TheClass;
static std::map<string, bool>							CLASS_IS_SET;				// abc['class name'] = already set ?
static std::map<string, string>							CLASS_AT_LINE;				// abc['class name'] = class at line

static std::map<std::pair<string, string>, bool>		CLASS_FUN_IS_SET; 			// abc[class][fun] = class fun already set ?
static std::map<std::pair<string, string>, string>		CLASS_FUN_AT_LINE; 			// abc[class][fun] = class fun line number
static std::map<std::pair<string, string>, string>		CLASS_FUN_TYPE;				// abc[class][fun] = class fun type
static std::map<std::pair<string, string>, bool>		CLASS_FUN_PRIVATE; 			// abc[class][fun] = class fun private ?
static std::map<std::pair<string, string>, int>			CLASS_FUN_ARG_TOTAL;		// abc[class][fun] = global total args
static std::map<std::pair<string, int>, string>			CLASS_FUN_ARG_TYPE;			// abc[class + fun][arg number] = arg type

static std::map<std::pair<string, string>, bool>		CLASS_G_VAR_IS_SET; 	    // abc[class][var] = global var already set ?
static std::map<std::pair<string, string>, string>		CLASS_G_VAR_AT_LINE; 	    // abc[class][var] = global var line number
static std::map<std::pair<string, string>, string>		CLASS_G_VAR_TYPE; 		    // abc[class][var] = global var type
static std::map<std::pair<string, string>, bool>		CLASS_G_VAR_PRIVATE; 	    // abc[class][var] = global private ?
static std::map<std::pair<string, string>, bool>		CLASS_G_VAR_IS_CONST;		// abc[class][var] = global var Constant
static std::map<std::pair<string, string>, bool>		CLASS_G_VAR_IS_ARRAY;

// ----------------------------------
// كائن
// ----------------------------------

static std::map<std::pair<string, string>, bool>	OBJ_IS_SET;			        // abc[win + fun][obj] = already set ?
static std::map<std::pair<string, string>, string>	OBJ_AT_LINE;		        // abc[win + fun][obj] = line number
static std::map<std::pair<string, string>, string>	OBJ_CLASS;			        // abc[win + fun][obj] = base class
static std::map<string, string>						OBJ_GLOBAL_DECLARATION;     // abc[class] = Obj1; Obj2;...

// ----------------------------------
// نافدة
// ----------------------------------

static bool 	IsInsideWindow = false;
static string 	TheWindow;

int WIN_TOTAL = 0;
string WIN_TOTAL_NAMES[64];

// ----------------------------------
// نافدة رئيسية
// ----------------------------------

static bool 	MAIN_WIN_IS_SET = false;
static string 	MAIN_WIN_AT_LINE;

// ----------------------------------
// نافدة
// ----------------------------------

static std::map<string, bool>		WIN_IS_SET;		// abc['window_name'] = already set ?
static std::map<string, string> 	WIN_AT_LINE;	// abc['window_name'] = func_at_line
static std::map<string, bool>		WIN_IS_WEB; 	// abc['window_name'] = Web UI ?

// ----------------------------------
// دالة 
// ----------------------------------

static bool											IsInsideFunction = false;
static string										TheFunction;
static string 										TheFunction_TYPE;
static std::map<std::pair<string, string>, string>	RETURN_FUN;	// abc[win][fun] = return is ok ? "OK", "IF"
static bool 										IS_CONTROL_FUNCTION_NAME = false;

// ----------------------------------
// دالة رئيسية
// ----------------------------------

static std::map<string, bool> 	MAIN_FUN_IS_SET;	// abc['window_name'] = main_func true or false
static std::map<string, string> MAIN_FUN_AT_LINE; 	// abc['window_name'] = main_func_at_line

// ----------------------------------
// دالة
// ----------------------------------

static std::map<string, bool>							G_FUN_IS_SET; 			// abc[gfun] = global fun already set ?
static std::map<string, string>							G_FUN_AT_LINE; 			// abc[gfun] = global fun line number
static std::map<string, string>							G_FUN_TYPE; 			// abc[gfun] = global fun type
static std::map<string, int>							G_FUN_ARG_TOTAL;		// abc[gfun] = global total args
static std::map<std::pair<string, int>, string>			G_FUN_ARG_TYPE;			// abc[gfun][arg number] = arg type
int														Global_TotalFucntion = 0;
static std::map<int, string>							Global_FunctionNames;	// abc[global fun number] = func name

static std::map<std::pair<string, string>, bool>		L_FUN_IS_SET; 	    // abc[window][fun] = local fun already set ?
static std::map<std::pair<string, string>, string>		L_FUN_AT_LINE; 		// abc[window][fun] = local fun line number
static std::map<std::pair<string, string>, string>		L_FUN_TYPE; 		// abc[window][fun] = local fun type
static std::map<std::pair<string, string>, int>			L_FUN_ARG_TOTAL;	// abc[window][fun] = global total args
static std::map<std::pair<string, int>, string>			L_FUN_ARG_TYPE;		// abc[window + fun][arg number] = arg type

int														Win_CurrentTotalFucntion = 0;
static std::map<string, int>							Win_TotalFucntion;	// abc[window] = total functions
static std::map<std::pair<string, int>, string>			Win_FunctionNames;	// abc[window][fun number] = func name

static std::string										NEW_FUNCTION_ARG = "";

// -----------------------------------------------------------
// VARs
// -----------------------------------------------------------

// Local var

static std::map<std::pair<string, string>, bool>	L_VAR_IS_SET; 		// abc[window + func][var] = local var already set ?
static std::map<std::pair<string, string>, string>	L_VAR_AT_LINE; 		// abc[window + func][var] = local var line number
static std::map<std::pair<string, string>, string>	L_VAR_TYPE; 		// abc[window + func][var] = local var type
static std::map<std::pair<string, string>, bool>	L_VAR_IS_CONST; 	// abc[window + func][var] = local var Constant
static std::map<std::pair<string, string>, bool>	L_VAR_IS_ARRAY;

// Global var

static std::map<string, bool>	G_VAR_IS_SET; 		// abc[var] = global var already set ?
static std::map<string, string>	G_VAR_AT_LINE; 		// abc[var] = global var line number
static std::map<string, string>	G_VAR_TYPE; 		// abc[var] = global var type
static std::map<string, bool>	G_VAR_IS_CONST; 	// abc[var] = global var Constant
static std::map<string, bool>	G_VAR_IS_ARRAY;
static int						G_VAR_TOTAL = 0;	// Total of global vars
static std::map<int, string>	G_VAR_NAMES; 		// abc[var] = var_name

// -----------------------------------------------------------
// CONTROLS TABLE
// -----------------------------------------------------------

static std::map<string, bool>						CONTROL_WIN_IS_SET; 	// abc[win-control] = already set ?
static std::map<string, string>						CONTROL_WIN_AT_LINE;	// abc[win-control] = line number
static std::map<std::pair<string, string>, bool>	CONTROL_IS_SET;			// abc[window][control] = already set ?
static std::map<std::pair<string, string>, string>	CONTROL_AT_LINE;		// abc[window][control] = line number
static std::map<std::pair<string, string>, string>	CONTROL_TYPE;			// abc[window][control] = type

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

static const bool DEBUG_PRINT_ON_SCREEN = false;
bool THIS_IS_ALIF_C_FILE = false;
bool SHOW_FILE_AND_LINE = true;

// Error

bool ERROR_PRINT_ON_SCREEN = false;

// Log
string LOG_MESSAGE_FULL = "";

// -----------------------------------------------------------
// CPP BUFERs
// -----------------------------------------------------------

string CBUFER;
string CBUFER_ID;
string CBUFER_OBJ;
string ScriptSyntaxBuffer;
string ScriptBuffer;

// -----------------------------------------------------------
// C++ Code Inside Alif Code
// -----------------------------------------------------------

static bool LIB_INSIDE_CPP_CODE = false;
string LIB_LEXER_CG_BUFER;
string LIB_PARSER_CG_BUFER;
bool ThisIsJavaScript = false;

// -----------------------------------------------------------
// ALIF COMPILER VARs
// -----------------------------------------------------------

static std::map<string, bool> ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED;
static std::map<string, bool> ALREADY_PARSED_FILE_TOKENS_PREDEFINED;

string PATH_FULL_ALIF;				// Desktop/myapp.alif
string PATH_FULL_BIN;				// Desktop/mybin.bin
string PATH_FULL_LOG;				// Desktop/myapp.log

string PATH_FULL_GCC;				// Linux : 'gcc' | Win : c:\Alif\bin\gcc.exe
string PATH_FULL_CPP;				// tmp/123456789.cpp
string PATH_FULL_OBJ;				// tmp/123456789.o
string PATH_FULL_RC;				// tmp/123456789.rc
string PATH_FULL_ICO;				// Mac: /usr/share/aliflang/alif.ics | Win: c:\Alif\alif.ico
#if  __APPLE__
	string PATH_FULL_PLIST;				// Mac only
	string PATH_FULL_BIN_TMP;			// Tmp/mybin.bin
#endif

string PATH_ABSOLUTE;
string PATH_WORKING;

string PATH_TEMP;					// /usr/tmp
string RANDOM;						// 1234

string PATH_FULL_LIB_SETTING;		// 'gcc' | c:\Alif\aliflib\aliflib.inf
std::map<string, string> LIB_FILE_NAME;

// Complet path of Web UI window (HTML5) file
// Used by parser #window_web
string PATH_FULL_WINDOW_WEB;

#ifdef _WIN32
	string SEPARATION = "\\";
#else
	string SEPARATION = "/";
#endif

// -----------------------------------------------------------
// Compile Extra commands
// -----------------------------------------------------------

string Compile_ExtraCompile = " ";	// -I/...
string Compile_ExtraLink = " ";		// -L\"/...\" -lmylib...

void CompileAddExtra_Compile (string cmd){

	Compile_ExtraCompile.append(cmd);
}

void CompileAddExtra_Link (string cmd){

	Compile_ExtraLink.append(cmd);
}

// -----------------------------------------------------------
// Python
// -----------------------------------------------------------

// -I/usr/include/python3.5
// -L\"/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu\" -lpython3.5

string PythonInclude_path = "";
string PythonLib_path = "";
string PythonLibName = "";

void PythonSetEnvirenment(){

	// -------------------------------
	// Python cmd: python3 -c "import sys; print(sys.path)"
	// Python includes	-> /usr/include/python3.5
	// Python Libs		-> /usr/lib/python3.5/config-3.5m-x86_64-linux-gnu
	// python lib name	-> python3.5
	// -------------------------------

	// TODO: if thos var are empty, then get python paths by cmd.

	string buffer = " -I\"" + PythonInclude_path + "\" ";

	CompileAddExtra_Compile(buffer);

	buffer = " -L\"" + PythonLib_path + "\" -l" + PythonLibName + " ";

	CompileAddExtra_Link(buffer);
}


