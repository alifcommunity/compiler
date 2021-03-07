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

void parser_String(std::string Token[2048], CLASS_TOKEN *o_tokens){
	// نص

	if (!o_tokens->TOKENS_PREDEFINED && IsInsideFunction)
		// Igiore Local func var predefinition, 
		// focus only on Global var predefinition, and Class global var predefinition.
		return; // continue;

	if (IsInsideNamespace && !IsInsideFunction)
		ErrorCode("انشاء متغير جديد يجب ان يكون خارج المجال ليكون عام، او داخل دالة ليكون محلي", o_tokens);
	
	if (Token[1] == "خاص" && !IsInsideClass)
		ErrorCode("يجب استعمال خاصيه ' خاص ' داخل صنف", o_tokens);
	
	if (Token[1] == "خاص" && IsInsideFunction)
		ErrorCode("يجب استعمال خاصيه ' خاص ' خارج الدالة", o_tokens);
	
	int INT_POS = 1;
	
	if (Token[1] == "خاص")
		INT_POS = 2;
	
	if (Token[INT_POS + 1] == "")
		ErrorCode("يجب تحديد اسم المتغير", o_tokens);
	
	if (Token[INT_POS + 2] != "" && Token[INT_POS + 2] != "=")
		ErrorCode("يجب استعمال ' = ' بعد اسم المتغير", o_tokens);
	
	if (Token[INT_POS + 2] == "=" && Token[INT_POS + 3] == "")
		ErrorCode("يجب اعطاء قيمة بعد ' = '", o_tokens);
	
	// Class
	if (IsInsideClass)
	{
		if (!IsInsideFunction)
		{
			// global var int class
			
			if (Token[1] == "خاص")
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					SetNewVarClass(true, true, TheClass, TheFunction, Token[INT_POS + 1], "نص", false,false, o_tokens->Line, o_tokens);
					return; // continue;
				}

				if(DEBUG)DEBUG_MESSAGE("[PRIVATE] [CLASS-GLOBAL-STRING] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class : New Private STRING
				CPP_CLASS.append(" private: std::string " + Global_ID[Token[INT_POS + 1]] + " ");
				if (Token[INT_POS + 2] == "")
				{
					CPP_CLASS.append(" = \"\"; \n");

					if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
			else
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					SetNewVarClass(true, false, TheClass, TheFunction, Token[INT_POS + 1], "نص", false,false, o_tokens->Line, o_tokens);
					return; // continue;
				}

				if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-STRING] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG
				
				// *** Generate Code ***
				// Class : New Public STRING
				CPP_CLASS.append(" public: std::string " + Global_ID[Token[INT_POS + 1]] + " ");
				if (Token[INT_POS + 2] == "")
				{
					CPP_CLASS.append(" = \"\"; \n");

					if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
		}
		else
		{
			// local int class
			
			if (Token[1] == "خاص")
			{
				ErrorCode("يجب استعمال خاصيه ' خاص ' خارج الدالة", o_tokens);
			}
			else
			{
				//if (!o_tokens->TOKENS_PREDEFINED)
				//{
					SetNewVarClass(false, false, TheClass, TheFunction, Token[INT_POS + 1], "نص", false,false, o_tokens->Line, o_tokens);
					//return; // continue;
				//}

				if(DEBUG)DEBUG_MESSAGE("[CLASS-STRING] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class : New Local Class STRING
				CPP_CLASS.append(" std::string " + ID[Token[INT_POS + 1]] + " ");
				if (Token[INT_POS + 2] == "")
				{
					CPP_CLASS.append(" = \"\"; \n");

					if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
		}
	}
	// Global String
	else if (!IsInsideFunction)
	{
		// Igiore Local var predefinition, 
		// focus only on Global var predefinition

		if (!o_tokens->TOKENS_PREDEFINED)
		{
			SetNewVar(true, "", "", Token[INT_POS + 1], "نص", false,false, o_tokens->Line, o_tokens);

			return; // continue;
		}

		if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG

		// *** Generate Code ***
		// Global Area
		// String a
		CPP_GLOBAL.append(" std::string " + Global_ID[Token[INT_POS + 1]] + " ");
		if (Token[INT_POS + 2] == "")
		{
			CPP_GLOBAL.append(" ; \n");

			if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
			return; // continue;
		}
		// *** *** *** *** *** ***
	}
	else
	{
		// Local String

		// Igiore Local var predefinition, 
		// focus only on Global var predefinition

		SetNewVar(false, TheNamespace, TheFunction, Token[INT_POS + 1], "نص", false, false, o_tokens->Line, o_tokens);
		
		if(DEBUG)DEBUG_MESSAGE("[LOCAL-STRING] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG
		
		// *** Generate Code ***
		// Local Area
		// STRING a
		if (!IsInsideNamespace)
		{
			// Global Function
			CPP_GLOBAL_FUN.append(" std::string " + ID[Token[INT_POS + 1]] + " ");
			if (Token[INT_POS + 2] == "")
			{
				CPP_GLOBAL_FUN.append(" ; \n");

				if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
				return; // continue;
			}
		}
		else
		{
			// Local Function
			cpp_AddScript(TheFunction, " std::string " + ID[Token[INT_POS + 1]] + " ");
			if (Token[INT_POS + 2] == "")
			{
				cpp_AddScript(TheFunction, " = \"\"; \n");

				if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
				return; // continue;
			}
		}
		// *** *** *** *** *** ***
	}
	
	if (Token[INT_POS + 2] == "=")
	{
		if(DEBUG)DEBUG_MESSAGE("= ", o_tokens); // DEBUG

		if (!IsInsideFunction)
		{
			if (IsInsideClass)
			{
				// Class Global-Area
				// *** Generate Code ***
				CPP_CLASS.append(" = ");
				// *** *** *** *** *** ***
			}
			else
			{
				// *** Generate Code ***
				// Global Area
				// STRING a =
				CPP_GLOBAL.append(" = ");
				// *** *** *** *** *** ***
			}
		}
		else
		{
			if (IsInsideClass)
			{
				// Local Function Class
				// *** Generate Code ***
				CPP_CLASS.append(" = ");
				// *** *** *** *** *** ***
			}
			else
			{
				// *** Generate Code ***
				// Local Area
				// STRING a =
				if (!IsInsideNamespace)
					// Global Function
					CPP_GLOBAL_FUN.append(" = ");
				else
					// Local Function
					cpp_AddScript(TheFunction, " = ");
				// *** *** *** *** *** ***
			}
		}
		
		TempTokenCount = 0;
		
		for (int p = (INT_POS + 2); p <= o_tokens->TOTAL[o_tokens->Line]; p++)
		{
			if (Token[p] != "")
			{
				if (substr_utf8(Token[p],0,1) == "_" && !IsInsideFunction) // to avoid : a = b + _a
				{
					if (Token[INT_POS + 1] == substr_utf8(Token[p], 1, CharCount_utf8(Token[p], o_tokens)))
						ErrorCode("لا يمكن استعمال المتغير العام في نفس السطر الذي تم الانشاء فيه '" + Token[p] + "' ", o_tokens);
				}
				else if (Token[INT_POS + 1] == Token[p] && (Token[p - 1] != ":")) // to avoid a = b + a, but ok for a = b + x:y:a
				{
					if (G_VAR_IS_SET[(Token[INT_POS + 1])] && IntToString(o_tokens->Line) != G_VAR_AT_LINE[(Token[INT_POS + 1])])
						ErrorCode("لا يمكن استعمال المتغير في نفس السطر الذي تم الانشاء فيه '" + Token[p] + "', كذلك اذا كنت تود استعمال المتغير العام '" + Token[INT_POS + 1] + "', يجب اضافه خط قبل اسم المتغير على سبيل المثال : _" + Token[INT_POS + 1], o_tokens);
					
					ErrorCode("لا يمكن استعمال المتغير في نفس السطر الذي تم الانشاء فيه '" + Token[p] + "' ", o_tokens);
				}
				
				TempToken[TempTokenCount] = Token[p];
				TempTokenCount++;
			}
		}
		
		std::string WIN_OR_CLASS;
		if (IsInsideClass)
			WIN_OR_CLASS = TheClass;
		else
			WIN_OR_CLASS = TheNamespace;
		
		// STRING Syntax
		ScriptSyntaxBuffer = CheckForSyntax("نص", // OBJECTIF_TYPE
								true, // Accept Using Reference to Namespace:Controls
								true, // Accept Using Reference to Namespace:Function
								true, // Accept Using Reference to Global Functions
								true, // Accept Using Reference to Local Functions
								true, // Accept Using Reference to Global VAR
								true, // Accept Using Reference to Local VAR
								false, // Accept Convertion from String To Int
								true, // Accept Convertion from Int To String
								TempToken, 						// SYNTAX[] std::string
								(TempTokenCount - 1), 	// SYNTAX_LONG int
								WIN_OR_CLASS, 			// TMP_WINDOW_NAME
								TheFunction, 				// TMP_FUNCTION_NAME
								o_tokens);
		
		if (IsInsideClass)
		{
			// just for fixing this ...
			// *** Generate Code ***
			CPP_CLASS.append(ScriptSyntaxBuffer + " ; \n ");
			// *** *** *** *** *** ***
		}
		else if (!IsInsideFunction)
		{
			// *** Generate Code ***
			// Global Area
			// STRING a = ... ;
			CPP_GLOBAL.append(ScriptSyntaxBuffer + " ; \n "); // TODO: did we need 'CPP_CLASS' ??????
			// *** *** *** *** *** ***
		}
		else
		{
			// *** Generate Code ***
			// Local Area
			// STRING a = ... ;
			if (!IsInsideNamespace)
				// Global Function
				CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ; \n ");
			else
				// Local Function
				cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ; \n ");
			// *** *** *** *** *** ***
		}

		if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
		return; // continue;
	}
	else if (Token[INT_POS + 2] != "")
		ErrorCode("أمر غير معروف '" + Token[INT_POS + 2] + "', ربما تقصد '=' ", o_tokens);

	if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens); // DEBUG
}
