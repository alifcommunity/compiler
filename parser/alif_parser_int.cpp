// ==================================
// (C)2018 DRAGA Hassan.
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

void parser_Int(string Token[2048], CLASS_TOKEN *o_tokens){
	// عدد

	if (!o_tokens->TOKENS_PREDEFINED && IsInsideFunction)
		// Igiore Local func var predefinition, 
		// focus only on Global var predefinition, and Class global var predefinition.
		return; // continue;

	if (IsInsideWindow && !IsInsideFunction)
		ErrorCode("انشاء متغير جديد يجب ان يكون خارج النافذة ليكون عام، او داخل دالة ليكون محلي", o_tokens);
	
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
					SetNewVarClass(true, true, TheClass, TheFunction, Token[INT_POS + 1], "عدد", false,false, o_tokens->Line, o_tokens);
					return; // continue;
				}

				if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-GLOBAL-INT] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class : New Private INT
				CPP_CLASS.append(" private: double " + Global_ID[Token[INT_POS + 1]] + " ");
				if (Token[INT_POS + 2] == "")
				{
					CPP_CLASS.append(" = 0; \n");

					if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
			else
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					SetNewVarClass(true, false, TheClass, TheFunction, Token[INT_POS + 1], "عدد", false,false, o_tokens->Line, o_tokens);
					return; // continue;
				}

				if(DEBUG)DEBUG_MESSAGE("	[CLASS-GLOBAL-INT] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG
				
				// *** Generate Code ***
				// Class : New Public INT
				CPP_CLASS.append(" public: double " + Global_ID[Token[INT_POS + 1]] + " ");
				if (Token[INT_POS + 2] == "")
				{
					CPP_CLASS.append(" = 0; \n");

					if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
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
					SetNewVarClass(false, false, TheClass, TheFunction, Token[INT_POS + 1], "عدد", false,false, o_tokens->Line, o_tokens);
					//return; // continue;
				//}

				if(DEBUG)DEBUG_MESSAGE("		[CLASS-INT] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class : New Local Class INT
				CPP_CLASS.append(" double " + ID[Token[INT_POS + 1]] + " ");
				if (Token[INT_POS + 2] == "")
				{
					CPP_CLASS.append(" = 0; \n");

					if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
		}
	}
	// Global INT
	else if (!IsInsideFunction)
	{
		// Igiore Local var predefinition, 
		// focus only on Global var predefinition

		if (!o_tokens->TOKENS_PREDEFINED)
		{
			SetNewVar(true, "", "", Token[INT_POS + 1], "عدد", false,false, o_tokens->Line, o_tokens);

			return; // continue;
		}

		if(DEBUG)DEBUG_MESSAGE("[GLOBAL-INT] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG

		// *** Generate Code ***
		// Global Area
		// INT a
		CPP_GLOBAL.append(" double " + Global_ID[Token[INT_POS + 1]] + " ");
		if (Token[INT_POS + 2] == "")
		{
			CPP_GLOBAL.append(" ; \n");

			if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
			return; // continue;
		}
		// *** *** *** *** *** ***
	}
	else
	{
		// Local INT

		// Igiore Local var predefinition, 
		// focus only on Global var predefinition

		SetNewVar(false, TheWindow, TheFunction, Token[INT_POS + 1], "عدد", false, false, o_tokens->Line, o_tokens);
		
		if(DEBUG)DEBUG_MESSAGE("		[LOCAL-INT] [" + Token[INT_POS + 1] + "] ", o_tokens); // DEBUG
		
		// *** Generate Code ***
		// Local Area
		// INT a
		if (!IsInsideWindow)
		{
			// Global Function
			CPP_GLOBAL_FUN.append(" double " + ID[Token[INT_POS + 1]] + " ");
			if (Token[INT_POS + 2] == "")
			{
				CPP_GLOBAL_FUN.append(" ; \n");

				if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
				return; // continue;
			}
		}
		else
		{
			// Local Function
			cpp_AddScript(TheFunction, " double " + ID[Token[INT_POS + 1]] + " ");
			if (Token[INT_POS + 2] == "")
			{
				cpp_AddScript(TheFunction, " = 0; \n");

				if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
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
				// INT a =
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
				// INT a =
				if (!IsInsideWindow)
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
		
		string WIN_OR_CLASS;
		if (IsInsideClass)
			WIN_OR_CLASS = TheClass;
		else
			WIN_OR_CLASS = TheWindow;
		
		// INT Syntax
		ScriptSyntaxBuffer = CheckForSyntax("عدد", // OBJECTIF_TYPE
								true, // Accept Using Reference to Window:Controls
								true, // Accept Using Reference to Window:Function
								true, // Accept Using Reference to Global Functions
								true, // Accept Using Reference to Local Functions
								true, // Accept Using Reference to Global VAR
								true, // Accept Using Reference to Local VAR
								false, // Accept Convertion from INT To Int
								true, // Accept Convertion from Int To INT
								TempToken, 						// SYNTAX[] INT
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
			// INT a = ... ;
			CPP_GLOBAL.append(ScriptSyntaxBuffer + " ; \n ");
			// *** *** *** *** *** ***
		}
		else
		{
			// *** Generate Code ***
			// Local Area
			// INT a = ... ;
			if (!IsInsideWindow)
				// Global Function
				CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ; \n ");
			else
				// Local Function
				cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ; \n ");
			// *** *** *** *** *** ***
		}

		if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
		return; // continue;
	}
	else if (Token[INT_POS + 2] != "")
		ErrorCode("أمر غير معروف '" + Token[INT_POS + 2] + "', ربما تقصد '=' ", o_tokens);

	if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
}
