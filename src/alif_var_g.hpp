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

void parser_VarGlobal(std::string Token[2048], CLASS_TOKEN *o_tokens){

	// Global var
	// _g1 = 1 + 2
	//  g1 = 1 + 2
	
	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;

	if (!IsInsideFunction)
		ErrorCode("يجب استعمال المتغير العام داخل دالة ' " + Token[1] + " ' ", o_tokens);
		
	//if (IsInsideNamespace && !IsInsideFunction) // C++ cant do this !
		//ErrorCode("Using vars must set outside window for global, or inside function for local.", o_tokens);
	
	std::string G_VAR_WITHOUT_;
	
	if (substr_utf8(Token[1], 0, 1) == "_")
		G_VAR_WITHOUT_ = substr_utf8(Token[1], 1, CharCount_utf8(Token[1], o_tokens));
	else
		G_VAR_WITHOUT_ = Token[1];
	
	if (!G_VAR_IS_SET[(G_VAR_WITHOUT_)])
		ErrorCode("ليس متغير عام ' " + Token[1] + " ' ", o_tokens);
	
	if (Token[2] != "=")
		ErrorCode("يجب اضافة ' = ' بعد اسم المتغير", o_tokens);
	
	if (Token[3] == "")
		ErrorCode("يجب اضافه قيمة بعد ' = ' ", o_tokens);
	
	//if (!IsInsideFunction)
		//ErrorCode("Your are in global area, so you dont need to use '_' for the global var : " + Token[1], o_tokens);
	
	if (IsInsideFunction)
	{
		//if (substr_utf8(Token[1], 0, 1) != "_")
			//ErrorCode("متغير محلي غير موجود, من اجل استعمال المتغير العام ' " + Token[1] + " ' يجب اضافه خط قبل اسم المتغير على سبيل المثال : _" + Token[1], o_tokens);
		
		if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "عدد")
		{
			//if (G_VAR_IS_CONST[(G_VAR_WITHOUT_)] == "ثابت")
				//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ + ")] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			if (!IsInsideNamespace)
				// Global Function
				// global-int = ...
				CPP_GLOBAL_FUN.append(Global_ID[G_VAR_WITHOUT_] + " = ");
			else
				// Local Function
				// global-int = ...
				cpp_AddScript(TheFunction, Global_ID[G_VAR_WITHOUT_] + " = ");
			// *** *** *** *** *** ***
		}
		else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "نص")
		{
			//if (G_VAR_IS_CONST[(G_VAR_WITHOUT_)] == "ثابت")
				//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" + G_VAR_WITHOUT_ + ")] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			if (!IsInsideNamespace)
				// Global Function
				// global-std::string = ...
				CPP_GLOBAL_FUN.append(Global_ID[G_VAR_WITHOUT_] + " = ");
			else
				// Local Function
				// global-std::string = ...
				cpp_AddScript(TheFunction, Global_ID[G_VAR_WITHOUT_] + " = ");
			// *** *** *** *** *** ***
		}
		else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "منطق")
		{
			//if (G_VAR_IS_CONST[(G_VAR_WITHOUT_)] == "ثابت")
				//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[GLOBAL-BOOL (" + G_VAR_WITHOUT_ + ")] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			if (!IsInsideNamespace)
				// Global Function
				// global-BOOL = ...
				CPP_GLOBAL_FUN.append(Global_ID[G_VAR_WITHOUT_] + " = ");
			else
				// Local Function
				// global-BOOL = ...
				cpp_AddScript(TheFunction, Global_ID[G_VAR_WITHOUT_] + " = ");
			// *** *** *** *** *** ***
		}
	}
	else
	{
		if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "عدد")
		{
			//if (G_VAR_IS_CONST[(G_VAR_WITHOUT_)] == "ثابت")
				//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ + ")] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Global Area
			// global-int = ...
			CPP_GLOBAL.append(Global_ID[G_VAR_WITHOUT_] + " = ");
			// *** *** *** *** *** ***
		}
		else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "نص")
		{
			//if (G_VAR_IS_CONST[(G_VAR_WITHOUT_)] == "ثابت")
				//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" + G_VAR_WITHOUT_ + ")] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Global Area
			// global-std::string = ...
			CPP_GLOBAL.append(Global_ID[G_VAR_WITHOUT_] + " = ");
			// *** *** *** *** *** ***
		}
		else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "منطق")
		{
			//if (G_VAR_IS_CONST[(G_VAR_WITHOUT_)] == "ثابت")
				//ErrorCode("لا يمكن تغيير قيمة المتغير ' " + G_VAR_WITHOUT_ + " ' لأنه من نوع ثابت ", o_tokens);
				
			if(DEBUG)DEBUG_MESSAGE("[GLOBAL-BOOL (" + G_VAR_WITHOUT_ + ")] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Global Area
			// global-BOOL = ...
			CPP_GLOBAL.append(Global_ID[G_VAR_WITHOUT_] + " = ");
			// *** *** *** *** *** ***
		}
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
	
	// Global var Syntax
	// _a = 1 + (_a * 3) ...
	
	ScriptSyntaxBuffer = CheckForSyntax(G_VAR_TYPE[(G_VAR_WITHOUT_)], // OBJECTIF_TYPE
									true, // Accept Using Reference to Namespace:Controls
									true, // Accept Using Reference to Namespace:Function
									true, // Accept Using Reference to Global Functions
									true, // Accept Using Reference to Local Functions
									true, // Accept Using Reference to Global VAR
									true, // Accept Using Reference to Local VAR
									false, // Accept Convertion from String To Int
									true, // Accept Convertion from Int To String
									TempToken, 			// SYNTAX[] std::string
									(TempTokenCount - 1), 	// SYNTAX_LONG int
									TheNamespace, 			// TMP_WINDOW_NAME
									TheFunction, 				// TMP_FUNCTION_NAME
									o_tokens);
	
	if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG

	// *** Generate Code ***
	if (IsInsideFunction)
	{
		if (!IsInsideNamespace)
			// Global Function
			CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ; \n");
		else
			// Local Function
			cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ; \n");
	}
	else
	{
		// Global Area
		CPP_GLOBAL.append(ScriptSyntaxBuffer + " ; \n");
	}
	// *** *** *** *** *** ***

	return; // continue;
}