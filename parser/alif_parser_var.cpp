// ==================================
// (C)2019 DRAGA Hassan.
// 	www.aliflang.org
// ----------------------------------
// Alif Programming Language
// 
// ==================================
#pragma once
#define UNICODE
#define _UNICODE
using namespace std;
// ----------------------------------
#include <map> // vector
// ----------------------------------

void parser_Var(string Token[2048], CLASS_TOKEN *o_tokens){

	// Local var
	// a = 1 + 2

	// Note: Parsing Local var must be first, and global after.
	
	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;
		
	if (!IsInsideFunction)
		ErrorCode("يجب استعمال المتغير داخل دالة ' " + Token[1] + " ' ", o_tokens);
	
	if (Token[2] == ":")
		ErrorCode("ليس كائن ' " + Token[1] + " ' ", o_tokens);
	
	if (Token[2] != "=")
		ErrorCode("يجب اضافة ' = ' بعد اسم المتغير", o_tokens);
	
	if (Token[3] == "")
		ErrorCode("يجب اضافه قيمة بعد ' = ' ", o_tokens);
	
	string CLASS_OR_WIN;
	if (IsInsideClass)
		CLASS_OR_WIN = TheClass;
	else
		CLASS_OR_WIN = TheWindow;
	
	if (L_VAR_TYPE[std::make_pair(CLASS_OR_WIN + TheFunction, Token[1])] == "عدد")
	{
		//if (L_VAR_IS_CONST[std::make_pair(CLASS_OR_WIN + TheFunction, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + Token[1] + " ' لأنه من نوع ثابت ", o_tokens);

		if(DEBUG)DEBUG_MESSAGE("		[LOCAL-INT (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		// Local INT = ...
		if (IsInsideClass)
		{
			CPP_CLASS.append(" " + ID[Token[1]] + " = ");
		}
		else if (!IsInsideWindow)
		{
			// global func
			CPP_GLOBAL_FUN.append(" " + ID[Token[1]] + " = ");
		}
		else
		{
			// local func
			cpp_AddScript(TheFunction, " " + ID[Token[1]] + " = ");
		}
		// *** *** *** *** *** ***
	}
	else if (L_VAR_TYPE[std::make_pair(CLASS_OR_WIN + TheFunction, Token[1])] == "نص")
	{
		//if (L_VAR_IS_CONST[std::make_pair(CLASS_OR_WIN + TheFunction, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + Token[1] + " ' لأنه من نوع ثابت ", o_tokens);

		if(DEBUG)DEBUG_MESSAGE("		[LOCAL-STRING (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		// Local STRING = ...
		if (IsInsideClass)
		{
			CPP_CLASS.append(" " + ID[Token[1]] + " = ");
		}
		else if (!IsInsideWindow)
		{
			// global func
			CPP_GLOBAL_FUN.append(" " + ID[Token[1]] + " = ");
		}
		else
		{
			// local func
			cpp_AddScript(TheFunction, " " + ID[Token[1]] + " = ");
		}
		// *** *** *** *** *** ***
	}
	else if (L_VAR_TYPE[std::make_pair(CLASS_OR_WIN + TheFunction, Token[1])] == "منطق")
	{
		//if (L_VAR_IS_CONST[std::make_pair(CLASS_OR_WIN + TheFunction, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + Token[1] + " ' لأنه من نوع ثابت ", o_tokens);
			
		if(DEBUG)DEBUG_MESSAGE("		[LOCAL-BOOL (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		// Local BOOL = ...
		if (IsInsideClass)
		{
			CPP_CLASS.append(" " + ID[Token[1]] + " = ");
		}
		else if (!IsInsideWindow)
		{
			CPP_GLOBAL_FUN.append(" " + ID[Token[1]] + " = ");
		}
		else
		{
			cpp_AddScript(TheFunction, " " + ID[Token[1]] + " = ");
		}
		// *** *** *** *** *** ***
	}
	
	/*
	if (!IsInsideWindow && IsInsideFunction)
	{
		// *** Generate Code ***
		// Global Function
		CPP_GLOBAL_FUN.append(" " + ID[Token[1]] + " = ");
		// *** *** *** *** *** ***
	}
	else if (IsInsideWindow && IsInsideFunction)
	{
		// *** Generate Code ***
		// Local Function
		cpp_AddScript(TheFunction, " " + ID[Token[1]] + " = ");
		// *** *** *** *** *** ***
	}
	*/

	TempTokenCount = 0;

	for (int p = 2; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
	{
		if (Token[p] != "")
		{
			TempToken[TempTokenCount] = Token[p];
			TempTokenCount++;
		}
	}
	
	// Local var Syntax
	// a = 1 + a * (_g - 3) ...
	
	ScriptSyntaxBuffer = CheckForSyntax(L_VAR_TYPE[std::make_pair(CLASS_OR_WIN + TheFunction, Token[1])], // OBJECTIF_TYPE
									true, // Accept Using Reference to Window:Controls
									true, // Accept Using Reference to Window:Function
									true, // Accept Using Reference to Global Functions
									true, // Accept Using Reference to Local Functions
									true, // Accept Using Reference to Global VAR
									true, // Accept Using Reference to Local VAR
									false, // Accept Convertion from String To Int
									true, // Accept Convertion from Int To String
									TempToken, 			// SYNTAX[] string
									(TempTokenCount - 1), // SYNTAX_LONG int
									CLASS_OR_WIN, 		// TMP_WINDOW_NAME
									TheFunction, 		// TMP_FUNCTION_NAME
									o_tokens);

	if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG

	// *** Generate Code ***
	// Local A = ...
	if (IsInsideClass)
	{
		CPP_CLASS.append(ScriptSyntaxBuffer + " ; \n");
	}
	else if (!IsInsideWindow)
	{
		// from global func
		CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ; \n");
	}
	else
	{
		// from local func
		cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ; \n");
	}
	// *** *** *** *** *** ***

	return; // continue;
}

