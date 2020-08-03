// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

// General
string CPP_ID_DECLARATION;
string CPP_GLOBAL;
string CPP_OBJ_DECLARATION;
string CPP_FUN;
string CPP_GLOBAL_FUN;
string CPP_CLASS;
//string CPP_AlifJavaScript_Functions;

// Window
std::map<std::pair<string,string>, string>	CPP_WINDOW;	// [window][option] = value

// Platforme
#include "pc_console.cpp"       // PC Console
#include "pc_gui.cpp"           // PC GUI
#include "phone_android.cpp"    // Phone Android
