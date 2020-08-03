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

void parser_IfElse(string Token[2048], CLASS_TOKEN *o_tokens){
	// وإلا

	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;

	if (!IsInsideFunction)
		ErrorCode("يجب استعمال الشرط داخل دالة", o_tokens);
	
	if (ALIF_IF_STATUS < 1)
		ErrorCode("لايمكن إستعمال ' وإلا ' من دون فتح شرط، ربمى تقصد ' إذا ' ", o_tokens);

	if (Token[2] == "إذا")
		ErrorCode("شرط غير صحيح، هل تقصد ' أو إذا ' ؟ ", o_tokens);
	
	if (Token[2] != "")
		ErrorCode("أمر غير معروف : ' " + Token[2] + " '، على العموم إذا كنت تقصد شرط جديد المرجو إستعمال ' إذا ' ", o_tokens);

	if(DEBUG)DEBUG_MESSAGE("	[ELSE " + IntToString(ALIF_IF_STATUS) + "] \n\n", o_tokens); // DEBUG

	// TODO: show error when double Else
	// if ...
	// else ...
	// else ...
	// end if

	// *** Generate Code ***
	if (!IsInsideWindow)
	{
		// Global Fun IF
		CPP_GLOBAL_FUN.append("\n } else { \n ");
	}
	else
	{
		// Local Fun IF
		cpp_AddScript(TheFunction, "\n } else { \n ");
	}
	// *** *** *** *** *** ***

	return; // continue;
}
