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

void parser_Return(string Token[2048], CLASS_TOKEN *o_tokens){
	// إرجاع
	// func int ()
	// return 1 + 2

	// Note: Parsing Local var must be first, and global after.

	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;
		
	if (!IsInsideFunction)
		ErrorCode("يجب استعمال ' إرجاع ' داخل دالة", o_tokens);
	
	if (TheFunction_TYPE == "عادم")
	{
		//ErrorCode("لا يمكن استعمال ' إرجاع ' في دالة من نوع عادم", o_tokens);

		if (Token[2] != "")
			ErrorCode("لا يمكن إرجاع قيمة، لأن الدالة ' " + TheFunction + " ' من نوع عادم، المرجو عدم كتابة أي شيئ بعد ' إرجاع '، أو تغيير نوع الدالة", o_tokens);
		
		if (IsInsideClass)
		{
			// void return in class func

			if(DEBUG)DEBUG_MESSAGE("		[VOID-RETURN-CLASS-FUN] \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			CPP_CLASS.append("\n return; \n");
			// *** *** *** *** *** ***

			return; // continue;
		}
		else if (!IsInsideWindow)
		{
			// void return in global func

			if(DEBUG)DEBUG_MESSAGE("	[VOID-RETURN-GLOBAL-FUN] \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			CPP_GLOBAL_FUN.append("\n  return; \n");
			// *** *** *** *** *** ***

			return; // continue;
		}
		else
		{
			// void return in local func

			if(DEBUG)DEBUG_MESSAGE("		[VOID-RETURN-LOCAL-FUN] \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			cpp_AddScript(TheFunction, "\n  return; \n");
			// *** *** *** *** *** ***

			return; // continue;
		}
	}
	else
	{
		// return int
		// return string
		// return bool

		if (Token[2] == "")
			ErrorCode("يجب اضافه قيمة بعد ' إرجاع ' لأن الدالة ' " + TheFunction + " ' من نوع " + TheFunction_TYPE, o_tokens);
	}
	
	//string FUN_TYPE;
	
	if (IsInsideClass)
	{
		// return in class func
		
		if (TheFunction_TYPE == "عدد")
		{
			if(DEBUG)DEBUG_MESSAGE("		[RETURN-CLASS-INT] ", o_tokens); // DEBUG
		}
		else if (TheFunction_TYPE == "نص")
		{
			if(DEBUG)DEBUG_MESSAGE("		[RETURN-CLASS-STRING] ", o_tokens); // DEBUG
		}
		else if (TheFunction_TYPE == "منطق")
		{
			if(DEBUG)DEBUG_MESSAGE("		[RETURN-CLASS-BOOL] ", o_tokens); // DEBUG
		}

		// *** Generate Code ***
		CPP_CLASS.append("\n return ( ");
		// *** *** *** *** *** ***
		
		if (ALIF_IF_STATUS > 0 && RETURN_FUN[std::make_pair(TheClass, TheFunction)] == "")
			RETURN_FUN[std::make_pair(TheClass, TheFunction)] = "IF";
		else
			RETURN_FUN[std::make_pair(TheClass, TheFunction)] = "OK";
	}
	else if (!IsInsideWindow)
	{
		// return in global func
		
		if (TheFunction_TYPE == "عدد")
		{
			if(DEBUG)DEBUG_MESSAGE("	[RETURN-GLOBAL-INT] ", o_tokens); // DEBUG
		}
		else if (TheFunction_TYPE == "نص")
		{
			if(DEBUG)DEBUG_MESSAGE("	[RETURN-GLOBAL-STRING] ", o_tokens); // DEBUG
		}
		else if (TheFunction_TYPE == "منطق")
		{
			if(DEBUG)DEBUG_MESSAGE("	[RETURN-GLOABL-BOOL] ", o_tokens); // DEBUG
		}

		// *** Generate Code ***
		// Global Func Return
		CPP_GLOBAL_FUN.append("\n  return ( ");
		// *** *** *** *** *** ***
		
		if (ALIF_IF_STATUS > 0 && RETURN_FUN[std::make_pair("", TheFunction)] == "")
			RETURN_FUN[std::make_pair("", TheFunction)] = "IF";
		else
			RETURN_FUN[std::make_pair("", TheFunction)] = "OK";
	}
	else
	{
		// return in local func
		
		if (TheFunction_TYPE == "عدد")
		{
			if(DEBUG)DEBUG_MESSAGE("		[RETURN-INT] ", o_tokens); // DEBUG
		}
		else if (TheFunction_TYPE == "نص")
		{
			if(DEBUG)DEBUG_MESSAGE("		[RETURN-STRING] ", o_tokens); // DEBUG
		}
		else if (TheFunction_TYPE == "منطق")
		{
			if(DEBUG)DEBUG_MESSAGE("		[RETURN-BOOL] ", o_tokens); // DEBUG
		}

		// *** Generate Code ***
		// Local Func Return
		cpp_AddScript(TheFunction, "\n  return ( ");
		// *** *** *** *** *** ***
		
		if (ALIF_IF_STATUS > 0 && RETURN_FUN[std::make_pair(TheWindow, TheFunction)] == "")
			RETURN_FUN[std::make_pair(TheWindow, TheFunction)] = "IF";
		else
			RETURN_FUN[std::make_pair(TheWindow, TheFunction)] = "OK";
	}
	
	if(Token[2] == "صحيح"){
		// Return Syntax
		// return true.

		ScriptSyntaxBuffer = " true ";
		if(DEBUG)DEBUG_MESSAGE(" [true] ", o_tokens); // DEBUG

	} else if(Token[2] == "خطأ"){
		// Return Syntax
		// return false.

		ScriptSyntaxBuffer = " false ";
		if(DEBUG)DEBUG_MESSAGE(" [false] ", o_tokens); // DEBUG
	} else {

		// Return Syntax
		// return a + 1 + a * (_g - 3) ...

		TempToken[0] = "=";
		TempTokenCount = 1;
		
		for (int p = 2; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
		{
			if (Token[p] != "")
			{
				TempToken[TempTokenCount] = Token[p];
				TempTokenCount++;
			}
		}
		
		string WIN_OR_CLASS;
		if (IsInsideClass)
			WIN_OR_CLASS = TheClass;
		else
			WIN_OR_CLASS = TheWindow;
		
		ScriptSyntaxBuffer = CheckForSyntax(TheFunction_TYPE,		// OBJECTIF_TYPE
											true,					// Accept Using Reference to Window:Controls
											true,					// Accept Using Reference to Window:Function
											true,					// Accept Using Reference to Global Functions
											true,					// Accept Using Reference to Local Functions
											true,					// Accept Using Reference to Global VAR
											true,					// Accept Using Reference to Local VAR
											false,					// Accept Convertion from String To Int
											true,					// Accept Convertion from Int To String
											TempToken,				// SYNTAX[] string
											(TempTokenCount - 1),	// SYNTAX_LONG int
											WIN_OR_CLASS,			// TMP_WINDOW_NAME
											TheFunction,			// TMP_FUNCTION_NAME
											o_tokens);
	}
	

	
	// return
	// if (condition)
	//   return
	// else 
	//   return

	// *** Generate Code ***
	if (IsInsideClass)
	{
		// Class Func return
		CPP_CLASS.append(ScriptSyntaxBuffer + " ); \n");
	}
	else if (!IsInsideWindow)
	{
		// Global Func Return
		CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ); \n");
	}
	else
	{
		// Local Func Return
		cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ); \n");
	}
	// *** *** *** *** *** ***

	if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG

	return; // continue;
}