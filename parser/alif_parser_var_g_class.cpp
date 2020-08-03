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

void parser_VarGlobalClass(string Token[2048], CLASS_TOKEN *o_tokens){
	
	// Global Class var
	// g1 = 1 + 2

	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;
	
	if (!IsInsideFunction)
		ErrorCode("يجب استعمال المتغير داخل دالة ' " + Token[1] + " ' ", o_tokens);
	
	if (Token[2] != "=")
		ErrorCode("يجب اضافة ' = ' بعد اسم المتغير", o_tokens);
	
	if (Token[3] == "")
		ErrorCode("يجب اضافه قيمة بعد ' = ' ", o_tokens);
	
	if (CLASS_G_VAR_TYPE[std::make_pair(TheClass, Token[1])] == "عدد")
	{
		//if (CLASS_G_VAR_IS_CONST[std::make_pair(TheClass, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

		if(DEBUG)DEBUG_MESSAGE("		[CLASS-GLOBAL-INT (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		CPP_CLASS.append(" " + Global_ID[Token[1]] + " = ");
		// *** *** *** *** *** ***
	}
	else if (CLASS_G_VAR_TYPE[std::make_pair(TheClass, Token[1])] == "نص")
	{
		//if (CLASS_G_VAR_IS_CONST[std::make_pair(TheClass, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

		if(DEBUG)DEBUG_MESSAGE("		[CLASS-GLOBAL-STRING (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		CPP_CLASS.append(" " + Global_ID[Token[1]] + " = ");
		// *** *** *** *** *** ***
	}
	else if (CLASS_G_VAR_TYPE[std::make_pair(TheClass, Token[1])] == "منطق")
	{
		//if (CLASS_G_VAR_IS_CONST[std::make_pair(TheClass, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);
			
		if(DEBUG)DEBUG_MESSAGE("		[CLASS-GLOBAL-BOOL (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		CPP_CLASS.append(" " + Global_ID[Token[1]] + " = ");
		// *** *** *** *** *** ***
	}
	
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
	
	ScriptSyntaxBuffer = CheckForSyntax(CLASS_G_VAR_TYPE[std::make_pair(TheClass, Token[1])], // OBJECTIF_TYPE
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
									TheClass, 		// TMP_WINDOW_NAME
									TheFunction, 		// TMP_FUNCTION_NAME
									o_tokens);
	
	if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG

	// *** Generate Code ***
	CPP_CLASS.append(ScriptSyntaxBuffer + " ; \n");
	// *** *** *** *** *** ***
	
	return; // continue;
}