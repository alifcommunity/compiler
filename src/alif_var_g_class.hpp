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

void parser_VarGlobalClass(std::string Token[2048], CLASS_TOKEN *o_tokens){
	
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

		if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-INT (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		CPP_CLASS.append(" " + Global_ID[Token[1]] + " = ");
		// *** *** *** *** *** ***
	}
	else if (CLASS_G_VAR_TYPE[std::make_pair(TheClass, Token[1])] == "نص")
	{
		//if (CLASS_G_VAR_IS_CONST[std::make_pair(TheClass, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

		if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-STRING (" + Token[1] + ")] = ", o_tokens); // DEBUG

		// *** Generate Code ***
		CPP_CLASS.append(" " + Global_ID[Token[1]] + " = ");
		// *** *** *** *** *** ***
	}
	else if (CLASS_G_VAR_TYPE[std::make_pair(TheClass, Token[1])] == "منطق")
	{
		//if (CLASS_G_VAR_IS_CONST[std::make_pair(TheClass, Token[1])] == "ثابت")
			//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);
			
		if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-BOOL (" + Token[1] + ")] = ", o_tokens); // DEBUG

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
									true, // Accept Using Reference to Namespace:Controls
									true, // Accept Using Reference to Namespace:Function
									true, // Accept Using Reference to Global Functions
									true, // Accept Using Reference to Local Functions
									true, // Accept Using Reference to Global VAR
									true, // Accept Using Reference to Local VAR
									false, // Accept Convertion from String To Int
									true, // Accept Convertion from Int To String
									TempToken, 			// SYNTAX[] std::string
									(TempTokenCount - 1), // SYNTAX_LONG int
									TheClass, 		// TMP_WINDOW_NAME
									TheFunction, 		// TMP_FUNCTION_NAME
									o_tokens);
	
	if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG

	// *** Generate Code ***
	CPP_CLASS.append(ScriptSyntaxBuffer + " ; \n");
	// *** *** *** *** *** ***
	
	return; // continue;
}