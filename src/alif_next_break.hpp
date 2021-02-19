/* 
	The Alif Programming Language
	Version 3.x Series
	(C)2021 Hassan DRAGA
	www.aliflang.org

	This file is part of Alif compiler.

	Alif compiler is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 3, or (at your option) any later
	version.

	Alif compiler is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
	for more details.

	You should have received a copy of the GNU General Public License
	along with Alif compiler; see the file COPYING3. If not see
	<http://www.gnu.org/licenses/>.
*/

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
        if (!IsInsideNamespace)
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
        if (!IsInsideNamespace)
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
