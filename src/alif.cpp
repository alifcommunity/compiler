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

#define ALIF_VERSION "3.0.40 (Beta)"

// Stack ********************************************************

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#define _HAS_STD_BYTE 0  // Fixing 'byte': ambiguous symbol
#define XSTR(a) STR(a)
#define STR(a) #a

// OS Include ***************************************************

#ifdef _WIN32
#define _WINSOCKAPI_
#include <direct.h>  // getcwd
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
#include <ctype.h>   //
#include <stdlib.h>  // mblen, mbtowc, wchar_t(C)
#include <algorithm>  // Standard replace()
#include <chrono>     // Wait..
#include <codecvt>  // warning C4244: 'argument': conversion from 'wchar_t' to 'const _Elem', possible loss of data
#include <cstdio>   // Remove files
#include <ctime>    // Initializ time, for rand()
#include <fstream>  // O/I files
#include <iostream>  // O/I
#include <iterator>  //
#include <locale>  // warning C4244: 'argument': conversion from 'wchar_t' to 'const _Elem', possible loss of data
#include <map>     // vectors
#include <sstream>  // stringstream
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
#include <boost/nowide/args.hpp>
#include <boost/nowide/fstream.hpp>
#include <boost/nowide/iostream.hpp>
#include <boost/program_options.hpp>
// UTF-8
#include "utf8.h"

// Core *********************************************************
#include "alif_core.hpp"

// Parser *******************************************************
//#include "alif_bool.hpp"
#include "alif_class.hpp"
#include "alif_end.hpp"
#include "alif_function.hpp"
#include "alif_function_call.hpp"
#include "alif_function_class.hpp"
#include "alif_if.hpp"
#include "alif_if_else.hpp"
//#include "alif_int.hpp"
#include "alif_loop.hpp"
#include "alif_macro.hpp"
#include "alif_namespace.hpp"
#include "alif_next_break.hpp"
#include "alif_obj.hpp"
#include "alif_obj_new.hpp"
#include "alif_operator.hpp"
#include "alif_return.hpp"
//#include "alif_string.hpp"
#include "alif_var.hpp"
#include "alif_var_g.hpp"
#include "alif_var_g_class.hpp"

void ALIF_PARSER(CLASS_TOKEN *o_tokens) {
    if (!o_tokens->TOKENS_PREDEFINED) {
        // Check for tokens not predifined

        if (ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED[o_tokens
                                                          ->PATH_FULL_SOURCE]) {
            if (DEBUG)
                DEBUG_MESSAGE("\n Already Parsed(Not-Predifined) [" +
                                  o_tokens->PATH_FULL_SOURCE + "] \n",
                              o_tokens);
            return;
        } else
            ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED[o_tokens
                                                          ->PATH_FULL_SOURCE] =
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
            ALREADY_PARSED_FILE_TOKENS_PREDEFINED[o_tokens->PATH_FULL_SOURCE] =
                true;
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
            // Token 			:
            // o_tokens->TOKEN[std::make_pair(o_tokens->Line, o_tokens->NUMBER)]
            // -----------------------------------------------------------------
            if (o_tokens
                    ->TOKEN[std::make_pair(o_tokens->Line, o_tokens->NUMBER)] !=
                "")
                Token[o_tokens->NUMBER] = o_tokens->TOKEN[std::make_pair(
                    o_tokens->Line, o_tokens->NUMBER)];
        }

        if (Token[1] == "") continue;

        if (DEBUG)
            DEBUG_MESSAGE(IntToString(o_tokens->Line) + ": ",
                          o_tokens);  // DEBUG

        // ------------------------------------------------------------------------

        if (Token[1] == "#") {
            // Macros
            parser_macro_ui(Token, o_tokens);
        }

        // ---------------------------------------------------------------------------------
        // General Erros
        // ---------------------------------------------------------------------------------

        else if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
            ErrorCode(
                "يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى",
                o_tokens);

        else if (Token[1] == "خاص" && !IsInsideClass)  // ERROR
            ErrorCode("يجب استعمال ' خاص ' داخل صنف", o_tokens);
        else if (Token[1] == "خاص" && Token[2] == "")
            ErrorCode("يجب تحديد نوع المتغير بعد ' خاص ' ", o_tokens);
        else if (Token[1] == "خاص" &&
                 (Token[2] != "عدد" && Token[2] != "نص" && Token[2] != "منطق" &&
                  Token[2] != "دالة"))  // ERROR
            ErrorCode(
                "أمر غير معروف : ' " + Token[2] +
                    " ' ، على العموم أمر ' خاص ' يمكن استعماله فقط مع متغيرات "
                    "من نوع حرف، عدد، منطق، أو دالة",
                o_tokens);

        // ---------------------------------------------------------------------------------
        // _س_ ... C++ ... _س_
        // ---------------------------------------------------------------------------------

        else if (Token[1] == "_س_") {
            if (!o_tokens->TOKENS_PREDEFINED) continue;

            if (!LIB_INSIDE_CPP_CODE) {
                if (DEBUG) DEBUG_MESSAGE("{_س_ START} ", o_tokens);  // DEBUG

                LIB_INSIDE_CPP_CODE = true;
                LIB_PARSER_CG_BUFER = "";

                for (int p = 2; p <= o_tokens->TOTAL[o_tokens->Line];
                     p++)  // _س_ ...C++...@ Alif @...C++... _س_
                {
                    if (Token[p] == "_س_")  // End C++ Code
                    {
                        if (DEBUG)
                            DEBUG_MESSAGE("{_س_ END} ", o_tokens);  // DEBUG
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
                            CPP_GLOBAL_FUN.append(" " + LIB_PARSER_CG_BUFER +
                                                  " ");
                            // code_add(CODE, LIB_PARSER_CG_BUFER);
                        } else if (IsInsideNamespace && IsInsideFunction) {
                            // Local Function
                            cpp_AddScript(TheFunction,
                                          " " + LIB_PARSER_CG_BUFER + " ");
                            // code_add(CODE, LIB_PARSER_CG_BUFER);
                        } else {
                            ErrorCode("علة: لم تنجح عملية ايجاد مكان شفرة سي++",
                                      o_tokens);
                        }
                        // *** *** *** *** *** ***
                    } else if (Token[p] == "@")  // @ Start ...
                    {
                        // ...C++...@ Alif @...C++...

                        if (DEBUG) DEBUG_MESSAGE("{@} ", o_tokens);  // DEBUG

                        TempTokenCount = 1;  // CheckForSyntax() Need this.
                        TempToken[0] = "=";  // CheckForSyntax() Need this.

                        bool AT_FOUND = false;

                        for (int c = p + 1; c < o_tokens->TOTAL[o_tokens->Line];
                             c++) {
                            if (Token[c] == "@") {
                                p = c;
                                AT_FOUND = true;
                                break;
                            }

                            TempToken[TempTokenCount] = Token[c];
                            TempTokenCount++;
                        }

                        if (!AT_FOUND)
                            ErrorCode("نهايه شفرة سي++ غير موجوده ' @ '",
                                      o_tokens);

                        std::string CLASS_OR_WIN;
                        if (IsInsideClass)
                            CLASS_OR_WIN = TheClass;
                        else
                            CLASS_OR_WIN = TheNamespace;

                        ScriptSyntaxBuffer = CheckForSyntax(
                            "C++",  // OBJECTIF_TYPE
                            true,   // Accept Using Reference to
                                    // namespace:Controls
                            true,   // Accept Using Reference to
                                    // namespace:Function
                            true,  // Accept Using Reference to Global Functions
                            true,  // Accept Using Reference to Local Functions
                            true,  // Accept Using Reference to Global VAR
                            true,  // Accept Using Reference to Local VAR
                            true,  // Accept Convertion from String To Int
                            true,  // Accept Convertion from Int To String
                            TempToken,             // SYNTAX[] std::string
                            (TempTokenCount - 1),  // SYNTAX_LONG int
                            CLASS_OR_WIN,          // TMP_WINDOW_NAME
                            TheFunction,           // TMP_FUNCTION_NAME
                            o_tokens);

                        if (DEBUG) DEBUG_MESSAGE("{@} ", o_tokens);  // DEBUG

                        // *** C++ ***
                        LIB_PARSER_CG_BUFER.append(" " + ScriptSyntaxBuffer +
                                                   " ");
                        // *** *** *** *** *** ***

                        // @ End.
                    } else if (Token[p] != "") {
                        // Add C++ Code to the buffer
                        LIB_PARSER_CG_BUFER.append(Token[p]);
                        if (DEBUG)
                            DEBUG_MESSAGE("{" + Token[p] + "} ",
                                          o_tokens);  // DEBUG
                    }
                }
            } else {
                if (DEBUG) DEBUG_MESSAGE("{_س_ END} ", o_tokens);  // DEBUG
                LIB_INSIDE_CPP_CODE = false;
            }
        }

        else if (LIB_INSIDE_CPP_CODE) {
            // Continue to adding C++ Code to the Buffer

            for (int p = 1; p <= o_tokens->TOTAL[o_tokens->Line];
                 p++)  // _س_ ...C++...@ Alif @...C++... _س_
            {
                if (Token[p] == "_س_")  // End C++ Code
                {
                    if (DEBUG) DEBUG_MESSAGE("{_س_ END} ", o_tokens);  // DEBUG
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
                        cpp_AddScript(TheFunction,
                                      " " + LIB_PARSER_CG_BUFER + " ");
                        // code_add(CODE, LIB_PARSER_CG_BUFER);
                    } else {
                        ErrorCode("علة: لم تنجح عملية ايجاد مكان شفرة سي++",
                                  o_tokens);
                    }
                    // *** *** *** *** *** ***
                } else if (Token[p] == "@")  // @ Start ...
                {
                    // ...C++...@ Alif @...C++...

                    if (DEBUG) DEBUG_MESSAGE("{@} ", o_tokens);  // DEBUG

                    TempTokenCount = 1;  // CheckForSyntax() Need this.
                    TempToken[0] = "=";  // CheckForSyntax() Need this.

                    bool AT_FOUND = false;

                    for (int c = p + 1; c < o_tokens->TOTAL[o_tokens->Line];
                         c++) {
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
                        "C++",  // OBJECTIF_TYPE
                        true,   // Accept Using Reference to namespace:Controls
                        true,   // Accept Using Reference to namespace:Function
                        true,   // Accept Using Reference to Global Functions
                        true,   // Accept Using Reference to Local Functions
                        true,   // Accept Using Reference to Global VAR
                        true,   // Accept Using Reference to Local VAR
                        true,   // Accept Convertion from String To Int
                        true,   // Accept Convertion from Int To String
                        TempToken,             // SYNTAX[] std::string
                        (TempTokenCount - 1),  // SYNTAX_LONG int
                        CLASS_OR_WIN,          // TMP_WINDOW_NAME
                        TheFunction,           // TMP_FUNCTION_NAME
                        o_tokens);

                    if (DEBUG) DEBUG_MESSAGE("{@} ", o_tokens);  // DEBUG

                    // *** C++ ***
                    LIB_PARSER_CG_BUFER.append(" " + ScriptSyntaxBuffer + " ");
                    // *** *** *** *** *** ***

                    // @ End.
                } else if (Token[p] != "") {
                    // Add C++ Code to the buffer
                    LIB_PARSER_CG_BUFER.append(Token[p]);
                    if (DEBUG)
                        DEBUG_MESSAGE("{" + Token[p] + "} ",
                                      o_tokens);  // DEBUG
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

        else if (Token[1] == "عدد" ||
                 (Token[1] == "خاص" && Token[2] == "عدد")) {
            parser_Int(Token, o_tokens);
        }

        else if (Token[1] == "نص" || (Token[1] == "خاص" && Token[2] == "نص")) {
            parser_String(Token, o_tokens);
        }

        else if (Token[1] == "منطق" ||
                 (Token[1] == "خاص" && Token[2] == "منطق")) {
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

        else if (Token[1] == "كائن" || CLASS_IS_SET[Token[1]]) {
            parser_ObjNew(Token, o_tokens);
        }

        else if (OBJ_IS_SET[std::make_pair(
                     TheNamespace + TheFunction,
                     Token[1])] ||  // Namespace -> Func. 	Local Obj.
                 OBJ_IS_SET[std::make_pair(
                     TheClass + TheFunction,
                     Token[1])] ||  // Class -> Func. 	Local Obj.
                 OBJ_IS_SET[std::make_pair(TheClass,
                                           Token[1])] ||  // Class.
                                                          // Global Class Obj.
                 OBJ_IS_SET[std::make_pair(
                     "",
                     Token[1])]) {  // Global Area. Global Obj.
            parser_Obj(Token, o_tokens);
        }

        else if (L_VAR_IS_SET[std::make_pair(TheNamespace + TheFunction,
                                             Token[1])] ||
                 L_VAR_IS_SET[std::make_pair(TheClass + TheFunction,
                                             Token[1])]) {
            parser_Var(Token, o_tokens);
        }

        else if (CLASS_G_VAR_IS_SET[std::make_pair(TheClass, Token[1])] &&
                 IsInsideClass) {
            parser_VarGlobalClass(Token, o_tokens);
        }

        else if (substr_utf8(Token[1], 0, 1) == "_" ||
                 G_VAR_IS_SET[(Token[1])]) {
            parser_VarGlobal(Token, o_tokens);
        }

        // Call function (Global / Local / Class-fun)

        else if (G_FUN_IS_SET[(Token[1])] ||
                 L_FUN_IS_SET[std::make_pair(TheNamespace, Token[1])] ||
                 CLASS_FUN_IS_SET[std::make_pair(TheClass, Token[1])]) {
            parser_FunctionCall(Token, o_tokens);
        }

        else if (  // CONTROL_WIN_IS_SET[Token[1]] ||
            namespace_is_set
                [Token[1]]  // ||
                            // CONTROL_IS_SET[std::make_pair(TheNamespace,
                            // Token[1])] ||
                            //(Token[1] == "رئيسية" &&
                            // MAIN_WIN_IS_SET)
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
                                  " '، على العموم، إن كنت تقصد أداة، لا تنسى "
                                  "أن تحدد إسم "
                                  "النافذة قبل إسم الأداة  ",
                              o_tokens);
                }
                // مجال
                else if (Token[1] == "مجال" || Token[1] == "المجال" ||
                         Token[1] == "النافذة" || Token[1] == "نافدت" ||
                         Token[1] == "نافذت" || Token[1] == "نافذ" ||
                         Token[1] == "نافد") {
                    ErrorCode("أمر غير معروف : ' " + Token[1] +
                                  " '، هل تقصد ' مجال ' ؟ ",
                              o_tokens);
                }
                // نص
                else if (Token[1] == "كلمة" || Token[1] == "حروف" ||
                         Token[1] == "الحرف" || Token[1] == "نص" ||
                         Token[1] == "الحروف" || Token[1] == "جملة") {
                    ErrorCode("أمر غير معروف : ' " + Token[1] +
                                  " '، هل تقصد ' نص ' ؟ ",
                              o_tokens);
                }
                // إذا
                else if (Token[1] == "ادا" || Token[1] == "إدا" ||
                         Token[1] == "أدا" || Token[1] == "اذا" ||
                         Token[1] == "أذا") {
                    ErrorCode("أمر غير معروف : ' " + Token[1] +
                                  " '، هل تقصد ' إذا ' ؟ ",
                              o_tokens);
                }
                // أو
                else if (Token[1] == "او" || Token[1] == "ٱو" ||
                         Token[1] == "آو" || Token[1] == "والا" ||
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
                    ErrorCode("أمر غير معروف : ' " + Token[1] +
                                  " '، هل تقصد ' أداة ' ؟ ",
                              o_tokens);
                }
                // أضف
                else if (Token[1] == "اضف" || Token[1] == "ٱضف" ||
                         Token[1] == "آضف" || Token[1] == "أظف" ||
                         Token[1] == "اظف" || Token[1] == "ٱظف" ||
                         Token[1] == "آظف" || Token[1] == "إظف" ||
                         Token[1] == "إضف") {
                    ErrorCode("أمر غير معروف : ' " + Token[1] +
                                  " '، هل تقصد ' #أضف ' ؟ ",
                              o_tokens);
                }
                // خاص
                else if (Token[1] == "خص" || Token[1] == "حاص" ||
                         Token[1] == "الخاص" || Token[1] == "الحاص") {
                    ErrorCode("أمر غير معروف : ' " + Token[1] +
                                  " '، هل تقصد ' خاص ' ؟ ",
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
            if (DEBUG) DEBUG_MESSAGE("\n\n", o_tokens);

    }  // End Line Loop

    // -------------------------
    // Check current file
    // if (o_tokens->TOKENS_PREDEFINED)
    FINAL_CURRENT_FILE_CODE_CHECKING(o_tokens);
    // -------------------------
}

// Lexer ********************************************************

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

void ADD_TOKEN(std::string TOKEN_CHAR, bool NEW_TOKEN, bool NEW_TOKEN_AFTER,
               int REAL_CHAR_NUMBER, CLASS_TOKEN *o_tokens) {
    if (NEW_TOKEN) {
        // New Token
        if (o_tokens->TOKEN[std::make_pair(
                o_tokens->TOTAL_LINES,
                o_tokens->TOTAL[o_tokens->TOTAL_LINES])] != "") {
            o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
        }

        if (TOKEN_CHAR != "") {
            if (o_tokens->TOTAL[o_tokens->TOTAL_LINES] < 1) {
                o_tokens->TOTAL[o_tokens->TOTAL_LINES] = 1;
            }

            o_tokens->TOKEN[std::make_pair(
                o_tokens->TOTAL_LINES,
                o_tokens->TOTAL[o_tokens->TOTAL_LINES])] = TOKEN_CHAR;

            // Set Real Token position in the real line
            o_tokens->REAL_TOKEN_POSITION[std::make_pair(
                o_tokens->TOTAL_LINES,
                o_tokens->TOTAL[o_tokens->TOTAL_LINES])] =
                REAL_CHAR_NUMBER + CharCount_utf8(TOKEN_CHAR, o_tokens);

            if (NEW_TOKEN_AFTER &&
                o_tokens->TOKEN[std::make_pair(
                    o_tokens->TOTAL_LINES,
                    o_tokens->TOTAL[o_tokens->TOTAL_LINES])] != "") {
                o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
            }
        }
    } else if (TOKEN_CHAR != "") {
        // New Char
        if (o_tokens->TOTAL[o_tokens->TOTAL_LINES] < 1) {
            o_tokens->TOTAL[o_tokens->TOTAL_LINES] = 1;
        }

        (o_tokens->TOKEN[std::make_pair(
             o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])])
            .append(TOKEN_CHAR);

        // Set Real Token position in the real line
        o_tokens->REAL_TOKEN_POSITION[std::make_pair(
            o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] =
            REAL_CHAR_NUMBER + CharCount_utf8(TOKEN_CHAR, o_tokens);

        if (NEW_TOKEN_AFTER &&
            o_tokens->TOKEN[std::make_pair(
                o_tokens->TOTAL_LINES,
                o_tokens->TOTAL[o_tokens->TOTAL_LINES])] != "") {
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
    } else if (EXTENTION == "alif" || EXTENTION == "ALIF" ||
               EXTENTION == "ألف") {
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
        ErrorCode("امتداد الملف غير مقبول : ' " + file + " ' ",
                  &OBJ_CLASS_TOKEN);

    // check file existe
    if (!is_file_exists(OBJ_CLASS_TOKEN.PATH_FULL_SOURCE)) {
        // Try second path
        std::string second_path = argument.input.path;  // [/home/folder/]
        second_path.append(file);                       // [test]

        if (is_file_exists(second_path)) {  // [/home/folder/test]

            OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = second_path;
        } else if (is_file_exists(second_path +
                                  ".alif")) {  // [/home/folder/test.alif]

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
                    OBJ_CLASS_TOKEN.PATH_FULL_SOURCE =
                        inc_path + file + ".alif";
                    found = true;
                    break;
                }
            }

            if (!found)
                ErrorCode("ملف غير موجود : ' " + file + " ' ",
                          &OBJ_CLASS_TOKEN);
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
    // OBJ_CLASS_TOKEN.PATH_FULL_SOURCE); 	AlifExitFailure();
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

        while (CHAR_NUMBER < LINE_CHAR_TOTAL)  // '<=' is wrong!
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
            // if ( (Char == "\\") && (substr_utf8(LINE8, (CHAR_NUMBER + 1), 1)
            // ==
            // "\\") && !INSIDE_STRING)
            if ((CHAR_NUMBER == 0) && !INSIDE_STRING && (Char == "'"))  // '
            {
                goto NEXT_LINE;
            }

            // -------------------------------------------------------------

            // _س_

            if ((Char == "_" &&
                 (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "س") &&
                 (substr_utf8(LINE8, CHAR_NUMBER + 2, 1) == "_")) ||
                // (Char == "_" && (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) ==
                // "ج") && (substr_utf8(LINE8, CHAR_NUMBER + 2, 1) == "_")) ||
                (Char == "@" &&
                 (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) != "@")))  // Skip '@@'
            {
                std::string CompletChar = "";

                if (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "س")
                    CompletChar = "_س_";

                if (Char != "@") {
                    CHAR_NUMBER = CHAR_NUMBER + 2;  // Point to after : _س_
                }

                if (!INSIDE_STRING_CPP) {
                    if (Char != "@") {
                        ADD_TOKEN(CompletChar, true, true, CHAR_NUMBER,
                                  &OBJ_CLASS_TOKEN);

                        // if(DEBUG)DEBUG_MESSAGE("<NEW start:_س_> " ,
                        // &OBJ_CLASS_TOKEN); // DEBUG
                    } else {
                        ADD_TOKEN("@", true, true, CHAR_NUMBER,
                                  &OBJ_CLASS_TOKEN);

                        // if(DEBUG)DEBUG_MESSAGE("<NEW start:@> " ,
                        // &OBJ_CLASS_TOKEN); // DEBUG
                    }

                    INSIDE_STRING_CPP = true;
                    LIB_LEXER_CG_BUFER = "";
                } else {
                    INSIDE_STRING_CPP = false;
                    ADD_TOKEN(LIB_LEXER_CG_BUFER, true, true, CHAR_NUMBER,
                              &OBJ_CLASS_TOKEN);

                    if (Char != "@") {
                        ADD_TOKEN(CompletChar, true, true, CHAR_NUMBER,
                                  &OBJ_CLASS_TOKEN);

                        // if(DEBUG)DEBUG_MESSAGE("<NEW END:_س_>|" +
                        // LIB_LEXER_CG_BUFER + "| " , &OBJ_CLASS_TOKEN); //
                        // DEBUG
                    } else {
                        ADD_TOKEN("@", true, true, CHAR_NUMBER,
                                  &OBJ_CLASS_TOKEN);

                        // if(DEBUG)DEBUG_MESSAGE("<NEW END:@>|" +
                        // LIB_LEXER_CG_BUFER + "| " , &OBJ_CLASS_TOKEN); //
                        // DEBUG
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
            } else if ((Char == "\"" && substr_utf8(LINE8, (CHAR_NUMBER - 1),
                                                    1) != "\\") ||  // [salam"]
                       (Char == "\"" && substr_utf8(LINE8, (CHAR_NUMBER - 2),
                                                    2) == "\\\\")  // [salam\\"]
            ) {
                // [salam"]		-> Yes
                // [salam\"]	-> No
                // [salam\\"]	-> Yes

                /*
                if (INSIDE_STRING_CPP)
                {
                        // This char is inside quote _س_ ..... _س_
                        // Add this char to this current token
                        ADD_TOKEN("\"", false, false, CHAR_NUMBER,
                &OBJ_CLASS_TOKEN);
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
                            ADD_TOKEN("@", true, true, CHAR_NUMBER,
            &OBJ_CLASS_TOKEN);
                    }
            }
            */
            else if (INSIDE_STRING) {
                // This char is inside quote " " OR _س_
                // Add this char to this current token
                ADD_TOKEN(Char, false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
            } else if (Char == " ")  // Space
            {
                // ignore space!
                // Point to next token
                ADD_TOKEN("", true, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
            } else if (Char == "-")  // || Char == "+")
            {
                // If is '-123', must be in the same token
                // else, '-', and '123', in different token

                if (IsValidDigit(substr_utf8(LINE8, CHAR_NUMBER + 1, 1), false,
                                 &OBJ_CLASS_TOKEN)) {
                    // FORCED Point to next token
                    // if(OBJ_CLASS_TOKEN.TOKEN[std::make_pair(OBJ_CLASS_TOKEN.TOTAL_LINES,
                    // OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES])] !=
                    // "")
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
            } else if (Char == "@" ||  // C++ Code
                       Char == "#" ||  // Hash
                       Char == ":" ||  // Operator
                       Char == "=" || Char == "+" ||
                       // Char == "-" ||
                       Char == "*" || Char == "&" ||  // New line
                       Char == "(" || Char == ")" || Char == "[" ||
                       Char == "]" || Char == "{" || Char == "}" ||
                       Char == "!" || Char == "<" || Char == ">" ||
                       Char == ";" ||  // To show error !
                       Char == "،" ||  // اشارة ،
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
        }  // End char loop.

    NEXT_LINE:

        if (INSIDE_STRING_CPP)
            // ADD_TOKEN("\n", false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
            LIB_LEXER_CG_BUFER.append("\n");

        OBJ_CLASS_TOKEN.TOTAL_LINES++;
    }  // End Line loop.
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
            DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION END ----- \n\n",
                          &OBJ_CLASS_TOKEN);

        // ------------------------------------------------------
        // Parser - Normal way
        // ------------------------------------------------------
        if (DEBUG)
            DEBUG_MESSAGE(" ----------- DEBUGING START ------- \n",
                          &OBJ_CLASS_TOKEN);
        OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = true;
        // Read list of tokens
        ALIF_PARSER(&OBJ_CLASS_TOKEN);
        // Check final application code
        FINAL_APPLICATION_CODE_CHECKING(&OBJ_CLASS_TOKEN, is_first_file);
        if (DEBUG)
            DEBUG_MESSAGE("\n ----------- DEBUGING FINISH ------ \n",
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
                DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION END ----- \n\n",
                              &OBJ_CLASS_TOKEN);
        } else {
            // This file have already Tokens Predefinetion
            // so, lets parse in normal way..

            // ------------------------------------------------------
            // Parser - Normal way
            // ------------------------------------------------------
            if (DEBUG)
                DEBUG_MESSAGE(" ----------- DEBUGING START ------- \n",
                              &OBJ_CLASS_TOKEN);
            OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = true;
            // Read list of tokens
            ALIF_PARSER(&OBJ_CLASS_TOKEN);
            // Check final application code
            FINAL_APPLICATION_CODE_CHECKING(&OBJ_CLASS_TOKEN, is_first_file);
            if (DEBUG)
                DEBUG_MESSAGE("\n ----------- DEBUGING FINISH ------ \n",
                              &OBJ_CLASS_TOKEN);
        }
    }

    FILE_STREAM.close();
}

// Compiler *****************************************************
#include "alif_compile.hpp"

// Main *********************************************************

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
    // cout << "argument.output.extension: " << argument.output.extension <<
    // endl; cout << "argument.output.fullpath: " << argument.output.fullpath <<
    // endl;

    // Temporary Immigrate setting from Alif v3 style to Alif v2.
    PATH_FULL_ALIF = argument.input.fullpath;
    PATH_FULL_BIN = argument.output.fullpath;
    // PATH_FULL_LOG	= argument.input.fullpath + ".log";
    PATH_FULL_LOG = argument.log.fullpath;

    // Check Setup Installation
    if (!CHECK_SETUP()) {
        err("Please re-install Alif Compiler, or download the latest version "
            "from "
            "\n\nwww.aliflang.org");
        AlifExitFailure();
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
            "]\nPlease re-install Alif Compiler, or download the latest "
            "version "
            "from http://www.aliflang.org");
        AlifExitFailure();
    }

    // Alif Lib Setting (to fix arab file name problem)
    // ALIF_LIB_SETTING();

    // CPP CODE INITIALIZATION
    // Set basic value to variables
    // to run a minimal app (Title, With, Heint, WebUI..)
    CG_INITIALIZATION();

    // ALIF VARIABLES INITIALIZATION
    // Set Initialisation of general alif variables
    // like Alif flag, inside fun or namespace etc..
    ALIF_VAR_INITIALIZATION_FOR_NEW_SOURCE_FILE(true);

    // The Alif Standard Library
    if (DEBUG)
        LogMessage("\n ----------- The Alif Standard Library ------------- \n");
    AlifLexerParser("alifstandardlib", "ALIFLIB", false,
                    false);  // Global Tokens Predefinition.
    AlifLexerParser("alifstandardlib", "ALIFLIB", false,
                    true);  // Full Syntaxt Checking.
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
                << "Warning: Can't execut Windows remove EXE output command"
                << endl;
            // boost::nowide::remove(PATH_FULL_BIN);
            // std::remove(PATH_FULL_BIN);
    #elif __APPLE__
        std::string CLEAR_BIN_CMD = std::string("rm -rf \"") + PATH_FULL_BIN +
                                    std::string("\" 2> /dev/null");
        if (system(CLEAR_BIN_CMD.c_str()) != 0)
            cout << endl
                << "Warning: Can't execut macOS remove App output command" << endl;
    #else
        std::string CLEAR_BIN_CMD = std::string("rm -f \"") + PATH_FULL_BIN +
                                    std::string("\" 2> /dev/null");
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
        AlifExitFailure();
    }

    // Save debug message
    if (!THIS_IS_ALIF_C_FILE)
        if (DEBUG) PATH_FULL_LOG_SAVE();

    // ++++++++
    // Debug
    // PATH_FULL_LOG_SAVE();
    // ALIF_ERROR("Compiling... ");
    // exit(EXIT_SUCCESS);
    // ++++++++

    if (SyntaxOnly) return;  // This is syntax only checking no compile needed

    #ifdef _WIN32

    if (!THIS_IS_ALIF_C_FILE) {
        // Get GENERATED C++ CODE

        boost::nowide::ofstream FILE_CPP;
        FILE_CPP.open(PATH_FULL_CPP);  // .c_str());

        if (!FILE_CPP.is_open()) {
            ALIF_ERROR("ERROR: Could not create CPP temp file. " +
                       PATH_FULL_CPP);
            AlifExitFailure();
        }

        FILE_CPP << CODE_GENERATOR_GET_PC_GUI();
        FILE_CPP.close();
    }

    // app icon
    if (!PATH_FULL_ICO.empty()) boost::replace_all(PATH_FULL_ICO, "\\", "\\\\");

    // GENERATE Windows Resource CODE
    boost::nowide::ofstream FILE_RESOURCE;
    FILE_RESOURCE.open(PATH_FULL_RC);  // .c_str());
    if (!FILE_RESOURCE.is_open()) {
        ALIF_ERROR("ERROR: Could not create RC temp files. ");
        AlifExitFailure();
    }
    FILE_RESOURCE << "aaaa ICON \"" + PATH_FULL_ICO + "\"";
    FILE_RESOURCE.close();

    // Compile
    compile_win64();

    #elif __APPLE__
    // app icon
    if (!PATH_FULL_ICO.empty()) boost::replace_all(PATH_FULL_ICO, "\\", "\\\\");

    if (!THIS_IS_ALIF_C_FILE) {
        // GENERATE C++ CODE

        boost::nowide::ofstream FILE_CPP;
        FILE_CPP.open(PATH_FULL_CPP);  // .c_str());
        if (!FILE_CPP.is_open()) {
            ALIF_ERROR("ERROR [F004]: Could not create CPP temp files. ");
            AlifExitFailure();
        }
        FILE_CPP << CODE_GENERATOR_GET_PC_GUI();
        FILE_CPP.close();
    }

    // Compile
    COMPILE_MAC_64BIT();

    #else  // Linux

    if (!THIS_IS_ALIF_C_FILE) {
        // GENERATE C++ CODE

        boost::nowide::ofstream FILE_CPP;
        FILE_CPP.open(PATH_FULL_CPP);  // .c_str());
        if (!FILE_CPP.is_open()) {
            ALIF_ERROR("ERROR [F004]: Could not create CPP temp files. ");
            AlifExitFailure();
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
            "Add debugging info to the log file")("debug-screen",
            "Add debugging info to the log file, And print it on the screen")("syntax-only", "Check only the Alif code syntax (no compile)")("o", boost::program_options::value<std::string>(), "Set output file")("log", boost::program_options::value<std::string>(), "Set log file")("log-json", boost::program_options::value<std::string>(),
            "Set log file as JSON format")("log-json-base64", boost::program_options::value<std::string>(),
            "Like 'log-json' but data is base64 encoded")("input", boost::program_options::value<vector<std::string>>(),
            "Set input file (only one)")("include-path,I", boost::program_options::value<vector<std::string>>(),
            "Add include path")("clib,L", boost::program_options::value<vector<std::string>>(),
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
            cout << "This is free software; see the source for copying "
                    "conditions.  "
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
                cout << "Error: You need to set only one input source file."
                     << endl;
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

    alif();

    if(AlifFailure)
        return EXIT_FAILURE;

    // Al Hamdo li ALLAH =)
    exit(EXIT_SUCCESS);
}
