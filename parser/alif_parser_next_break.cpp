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

void parser_NextBreak(string Token[2048], CLASS_TOKEN *o_tokens){
    // التالي - كسر

	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;

	if (!IsInsideFunction)
		ErrorCode("يجب استعمال ' " + Token[1] + " ' داخل دالة", o_tokens);
	
	if (ALIF_LOOP_STATUS < 1)
		ErrorCode("يجب استعمال ' " + Token[1] + " ' داخل جملة تكرارية \"كلما\" ", o_tokens);

	if (Token[2] != "")
		ErrorCode("أمر غير معروف : ' " + Token[2] + " '، على العموم لايجب وضع أي شيئ بعد ' " + Token[1] + " ' ' ", o_tokens);

    if (Token[1] == "التالي")
    {
        if(DEBUG)DEBUG_MESSAGE("	[Continue] \n\n", o_tokens); // DEBUG

        // *** Generate Code ***
        if (!IsInsideWindow)
        {
            // Global Fun IF
            CPP_GLOBAL_FUN.append("\n continue; \n ");
        }
        else
        {
            // Local Fun IF
            cpp_AddScript(TheFunction, "\n continue; \n ");
        }
        // *** *** *** *** *** ***
    }
    else
    {
        if(DEBUG)DEBUG_MESSAGE("	[Break] \n\n", o_tokens); // DEBUG

        // *** Generate Code ***
        if (!IsInsideWindow)
        {
            // Global Fun IF
            CPP_GLOBAL_FUN.append("\n break; \n ");
        }
        else
        {
            // Local Fun IF
            cpp_AddScript(TheFunction, "\n break; \n ");
        }
        // *** *** *** *** *** ***
    }
}
