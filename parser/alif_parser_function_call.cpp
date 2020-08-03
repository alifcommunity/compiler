// ==================================
// (C)2020 DRAGA Hassan.
// 	 www.aliflang.org
// ----------------------------------
// The Alif Programming Language
// ==================================
#pragma once
#define UNICODE
#define _UNICODE
using namespace std;
// ----------------------------------

void parser_FunctionCall(string Token[2048], CLASS_TOKEN *o_tokens){

	// *myfunction(a, b, c..)

	if (!o_tokens->TOKENS_PREDEFINED)
		return;
		
	// --- Check for errors ---------------------------------------
	
	if (!IsInsideFunction)
		ErrorCode("لايمكن مناداة على دالة من خارج دالة '" + Token[1] + "()' ", o_tokens);
	
	if (Token[2] == "")
		ErrorCode("يجب اضافه '(' بعد اسم الدالة ' " + Token[1] + " ' ", o_tokens);
	
	if (Token[2] != "(")
		ErrorCode("من أجل نداء الدالة العامة ' " + Token[1] + "() ' يجب إضافة الإشارة ' ( ' قبل  ' " + Token[2] + " ' ", o_tokens); // TODO: Whats is this?
	
	if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
		ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);

	// --- Set current parent ---------------------------------------

	string CurrentParent = "";

	if(IsInsideClass)
		CurrentParent = TheClass;
	else if(IsInsideWindow)
		CurrentParent = TheWindow;
	
	// --- Initializing ---------------------------------------------

	bool CallClassFun = false;
	bool CallGlobalFun = false;
	string FuncParent = "";
	string FunName = "";
	string FunType = "";

	// --- Identify -------------------------------------------------

	if(L_FUN_IS_SET[std::make_pair(CurrentParent, Token[1])]){

		// Local
		
		FuncParent = CurrentParent;
		FunName = Token[1];
		FunType = L_FUN_TYPE[std::make_pair(CurrentParent, Token[1])];

		// Debug
		if(DEBUG)
			DEBUG_MESSAGE("Call Local-function [" + Token[1] + "] (", o_tokens);

	} else if(CLASS_FUN_IS_SET[std::make_pair(CurrentParent, Token[1])]){

		// Local Class

		CallClassFun = true;
		FuncParent = CurrentParent;
		FunName = Token[1];
		FunType = CLASS_FUN_TYPE[std::make_pair(CurrentParent, Token[1])];

		// Debug
		if(DEBUG)
			DEBUG_MESSAGE("Call Class-function [" + Token[1] + "] (", o_tokens);

	} else if (G_FUN_IS_SET[(Token[1])]){

		// Global

		CallGlobalFun = true;
		FunName = Token[1];
		FunType = G_FUN_TYPE[(Token[1])];

		// Debug
		if(DEBUG)
			DEBUG_MESSAGE("Call Global-function [" + Token[1] + "] (", o_tokens);

	} else {

		// Error
		ErrorCode("علة: نوع الدالة غير معروف'" + Token[1] + "' ", o_tokens);
	}

	// --- Check -------------------------------------------------

	if (IsDataType(FunType))
		ErrorCode("الدالة المحلية ' " + FunName + "()' من نوع " + FunType + ", لذلك أنت بحاجة إلى متغير للحصول على قيمة الرجوع", o_tokens);
	
	// --- ARGV -------------------------------------------------

	if(o_tokens->TOTAL[o_tokens->Line] > 3){

		TempTokenCount = 0;
		for (int p = 3; p <= o_tokens->TOTAL[o_tokens->Line]; p++){

			// ... [a, b + 2, c(...), d*x)]

			if (Token[p] != ""){

				TempToken[TempTokenCount] = Token[p];
				TempTokenCount++;
			}
		}

		ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG(CallGlobalFun, FuncParent, FunName, CallClassFun, CurrentParent, TheFunction, TempToken, (TempTokenCount - 1),  o_tokens);	
	}

	// DEBUG
	if(DEBUG)
		DEBUG_MESSAGE(") ", o_tokens);

	// --- Gen. Code -------------------------------------------------

	if(CallGlobalFun){

		// Call a Global func
		ScriptBuffer = " FUNCTION_" + Global_ID[FunName] + "( " + ScriptSyntaxBuffer + "); \n";

	} else if (CallClassFun){

		// Call a Local-Class func
		ScriptBuffer = " ClassFUNCTION_" + ID[FunName] + "(" + ScriptSyntaxBuffer + "); \n";

	} else {

		// Call a Local func
		ScriptBuffer = " OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->FUNCTION_" + ID[FunName] + "(" + ScriptSyntaxBuffer + "); \n";
	}

	// --- Add. Code -------------------------------------------------

	if(IsInsideClass){

		// Class
		CPP_CLASS.append(ScriptBuffer);

	} else if (IsInsideWindow){

		// Local func
		cpp_AddScript(TheFunction, ScriptBuffer);

	} else if (IsInsideFunction){

		// Global func
		CPP_GLOBAL_FUN.append(ScriptBuffer);

	} else {

		// Global
		//CPP_GLOBAL.append(ScriptBuffer);

		ErrorCode("علة: لايمكن مناداة على دالة في مكان عام", o_tokens);
	}

	// ------------------------------------------------------------------------------------------
	/*
	// NOTE: Searching for local func first, if no one go to global func
	if (L_FUN_IS_SET[std::make_pair(TheWindow, Token[1])])
	{
		// Call a local fun

		if (L_FUN_TYPE[std::make_pair(TheWindow, Token[1])] == "عدد")
			ErrorCode("الدالة المحلية ' " + Token[1] + "()' من نوع عدد, لذلك أنت بحاجة إلى متغير للحصول على قيمة الرجوع", o_tokens);
		
		if (L_FUN_TYPE[std::make_pair(TheWindow, Token[1])] == "نص")
			ErrorCode("الدالة المحلية ' " + Token[1] + "()' من نوع كلمة, لذلك أنت بحاجة إلى متغير للحصول على قيمة الرجوع", o_tokens);
		
		if(DEBUG)DEBUG_MESSAGE("		[CALL-LOCAL-FUNCTION ' " + Token[1] + " '] (", o_tokens); // DEBUG

		// *** Generate Code ***
		// Call Local Func from inside Local Func
		cpp_AddScript(TheFunction, " OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->FUNCTION_" + ID[Token[1]] + " (");
		// *** *** *** *** *** ***

		// Check local fun Args and Syntax : fun (a + c, 2 * (b - 1))
		ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG(false, TheWindow, Token[1], 0, TheWindow, TheFunction, TempToken, (TempTokenCount - 1),  o_tokens);

		if(DEBUG)DEBUG_MESSAGE(" ) ", o_tokens); // DEBUG
		
		// *** Generate Code ***
		cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ); \n");
		// *** *** *** *** *** ***
	}
	else if (G_FUN_IS_SET[(Token[1])])
	{
		// Call a global fun

		if (G_FUN_TYPE[(Token[1])] == "عدد")
			ErrorCode("الدالة العامه ' " + Token[1] + "()' من نوع عدد, لذلك أنت بحاجة إلى متغير للحصول على قيمة الرجوع", o_tokens);
		
		if (G_FUN_TYPE[(Token[1])] == "نص")
			ErrorCode("الدالة العامه ' " + Token[1] + "()' من نوع كلمة, لذلك أنت بحاجة إلى متغير للحصول على قيمة الرجوع", o_tokens);
		
		if (IsInsideClass)
		{
			if(DEBUG)DEBUG_MESSAGE("		[CALL-GLOBAL-FUNCTION ' " + Token[1] + " '] ( ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Call Global Func from inside Class Func
			CPP_CLASS.append("FUNCTION_" + Global_ID[Token[1]] + " (");
			// *** *** *** *** *** ***
		}
		else if (!IsInsideWindow)
		{
			if(DEBUG)DEBUG_MESSAGE("	[CALL-GLOBAL-FUNCTION ' " + Token[1] + " '] ( ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Call Global Func from inside Global Func
			CPP_GLOBAL_FUN.append("FUNCTION_" + Global_ID[Token[1]] + " (");
			// *** *** *** *** *** ***
		}
		else
		{
			if(DEBUG)DEBUG_MESSAGE("		[CALL-GLOBAL-FUNCTION ' " + Token[1] + " '] ( ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Call Global Func from inside Local Func
			cpp_AddScript(TheFunction, "FUNCTION_" + Global_ID[Token[1]] + " (");
			// *** *** *** *** *** ***
		}

		// Check Global fun Args and Syntax : gfun (a + c, 2 * (b - 1))
		ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG(true, "", Token[1], 0, WIN_OR_CLASS, TheFunction, TempToken, (TempTokenCount - 1),  o_tokens);

		if(DEBUG)DEBUG_MESSAGE(" ) ", o_tokens); // DEBUG

		if (IsInsideClass)
		{
			// *** Generate Code ***
			CPP_CLASS.append(ScriptSyntaxBuffer + " ); \n");
			// *** *** *** *** *** ***
		}
		else if (!IsInsideWindow)
		{
			// *** Generate Code ***
			CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ); \n");
			// *** *** *** *** *** ***			
		}
		else
		{
			// *** Generate Code ***
			cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ); \n");
			// *** *** *** *** *** ***			
		}
	}
	else
		ErrorCode("علة: نوع الدالة غير معروف'" + Token[1] + "' ", o_tokens);
	*/
}
