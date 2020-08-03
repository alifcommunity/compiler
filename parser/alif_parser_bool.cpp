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

void parser_bool(string Token[2048], CLASS_TOKEN *o_tokens){
	//منطق

    // bool A = true

	if (!o_tokens->TOKENS_PREDEFINED && IsInsideFunction)
		// Igiore Local func bool-var predefinition, 
		// focus only on Global bool-var predefinition, and Class global bool-var predefinition.
		return; // continue;

    if (IsInsideWindow && !IsInsideFunction)
		ErrorCode("انشاء متغير منطقي جديد يجب ان يكون خارج النافذة ليكون عام، او داخل دالة ليكون محلي", o_tokens);

    if (Token[1] == "خاص" && !IsInsideClass)
		ErrorCode("يجب استعمال خاصيه ' خاص ' داخل صنف", o_tokens);
	
	if (Token[1] == "خاص" && IsInsideFunction)
		ErrorCode("يجب استعمال خاصيه ' خاص ' خارج الدالة", o_tokens);

	int BOOL_POS = 1;
	
	if (Token[1] == "خاص")
		BOOL_POS = 2;
	
	if (Token[BOOL_POS + 1] == "")
		ErrorCode("يجب تحديد اسم المتغير المنطقي", o_tokens);
	
	if (Token[BOOL_POS + 2] != "" && Token[BOOL_POS + 2] != "=")
		ErrorCode("يجب استعمال ' = ' بعد اسم المتغير المنطقي", o_tokens);
	
	if (Token[BOOL_POS + 2] == "=" && Token[BOOL_POS + 3] == "")
		ErrorCode("يجب اعطاء قيمة بعد ' = '", o_tokens);
	
	// Class
	if (IsInsideClass)
	{
		if (!IsInsideFunction)
		{
			// global class bool-var Bool
			
			if (Token[1] == "خاص")
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					SetNewVarClass(true, true, TheClass, TheFunction, Token[BOOL_POS + 1], "منطق", false,false, o_tokens->Line, o_tokens);
					return; // continue;
				}

				if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-GLOBAL-BOOL] [" + Token[BOOL_POS + 1] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class : New Private Bool
				CPP_CLASS.append(" private: bool " + Global_ID[Token[BOOL_POS + 1]] + " ");
				if (Token[BOOL_POS + 2] == "")
				{
					CPP_CLASS.append(" = false; \n");

					if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
			else
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					SetNewVarClass(true, false, TheClass, TheFunction, Token[BOOL_POS + 1], "منطق", false,false, o_tokens->Line, o_tokens);
					return; // continue;
				}

				if(DEBUG)DEBUG_MESSAGE("	[CLASS-GLOBAL-BOOL] [" + Token[BOOL_POS + 1] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class : New Public Bool
				CPP_CLASS.append(" public: bool " + Global_ID[Token[BOOL_POS + 1]] + " ");
				if (Token[BOOL_POS + 2] == "")
				{
					CPP_CLASS.append(" = false; \n");

					if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
		}
		else
		{
			// local Bool class
			
			if (Token[1] == "خاص")
			{
				ErrorCode("يجب استعمال خاصيه ' خاص ' خارج الدالة", o_tokens);
			}
			else
			{
				//if (!o_tokens->TOKENS_PREDEFINED)
				//{
					SetNewVarClass(false, false, TheClass, TheFunction, Token[BOOL_POS + 1], "منطق", false,false, o_tokens->Line, o_tokens);
					//return; // continue;
				//}

				if(DEBUG)DEBUG_MESSAGE("		[CLASS-BOOL] [" + Token[BOOL_POS + 1] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class : New Local Class Bool
				CPP_CLASS.append(" bool " + ID[Token[BOOL_POS + 1]] + " ");
				if (Token[BOOL_POS + 2] == "")
				{
					CPP_CLASS.append(" = false; \n");

					if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
					return; // continue;
				}
				// *** *** *** *** *** ***
			}
		}
	}
	else if (!IsInsideFunction)
	{
		// Global Area Bool

		// Igiore Local bool-var predefinition, 
		// focus only on Global bool-var predefinition

		if (!o_tokens->TOKENS_PREDEFINED)
		{
			SetNewVar(true, "", "", Token[BOOL_POS + 1], "منطق", false,false, o_tokens->Line, o_tokens);
			
			return; // continue;
		}
		
		if(DEBUG)DEBUG_MESSAGE("[GLOBAL-BOOL] [" + Token[BOOL_POS + 1] + "] ", o_tokens); // DEBUG

		// *** Generate Code ***
		// Global Area
		// Bool a
		CPP_GLOBAL.append(" bool " + Global_ID[Token[BOOL_POS + 1]] + " ");
		if (Token[BOOL_POS + 2] == "")
		{
			CPP_GLOBAL.append(" = false; \n");

			if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
			return; // continue;
		}
		// *** *** *** *** *** ***
	}
	else
	{
		// Local Bool

		SetNewVar(false, TheWindow, TheFunction, Token[BOOL_POS + 1], "منطق", false,false, o_tokens->Line, o_tokens);
		
		if(DEBUG)DEBUG_MESSAGE("		[LOCAL-BOOL ] [" + Token[BOOL_POS + 1] + "] ", o_tokens); // DEBUG

		// *** Generate Code ***
		// Local Area
		// Bool a
		if (!IsInsideWindow)
		{
			// Global Function
			CPP_GLOBAL_FUN.append(" bool " + ID[Token[BOOL_POS + 1]] + " ");
			if (Token[BOOL_POS + 2] == "")
			{
				CPP_GLOBAL_FUN.append(" = false; \n");

				if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
				return; // continue;
			}
		}
		else
		{
			// Local Function
			cpp_AddScript(TheFunction, " bool " + ID[Token[BOOL_POS + 1]] + " ");
			if (Token[BOOL_POS + 2] == "")
			{
				cpp_AddScript(TheFunction, " = false; \n");

				if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
				return; // continue;
			}
		}
		// *** *** *** *** *** ***
	}
	
	if (Token[BOOL_POS + 2] == "=")
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
				// Bool a =
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
				// Bool a =
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
		
		for (int p = (BOOL_POS + 2); p <= o_tokens->TOTAL[o_tokens->Line]; p++)
		{
			if (Token[p] != "")
			{
				if (substr_utf8(Token[p],0,1) == "_" && !IsInsideFunction) // to avoid : a = b + _a
				{
					if (Token[BOOL_POS + 1] == substr_utf8(Token[p], 1, CharCount_utf8(Token[p], o_tokens)))
						ErrorCode("لا يمكن استعمال المتغير المنطقي العام في نفس السطر الذي تم الانشاء فيه '" + Token[p] + "' ", o_tokens);
				}
				else if (Token[BOOL_POS + 1] == Token[p] && (Token[p - 1] != ":")) // to avoid a = b + a, but ok for a = b + x:y:a
				{
					if (G_VAR_IS_SET[(Token[BOOL_POS + 1])] && IntToString(o_tokens->Line) != G_VAR_AT_LINE[(Token[BOOL_POS + 1])])
						ErrorCode("لا يمكن استعمال المتغير المنطقي في نفس السطر الذي تم الانشاء فيه '" + Token[p] + "', كذلك اذا كنت تود استعمال المتغير العام '" + Token[BOOL_POS + 1] + "', يجب اضافه خط قبل اسم المتغير على سبيل المثال : _" + Token[BOOL_POS + 1], o_tokens);
					
					ErrorCode("لا يمكن استعمال المتغير المنطقي في نفس السطر الذي تم الانشاء فيه '" + Token[p] + "' ", o_tokens);
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
		
		// Bool Syntax
		ScriptSyntaxBuffer = CheckForSyntax("منطق",	// OBJECTIF_TYPE
											true,	// Accept Using Reference to Window:Controls
											true,	// Accept Using Reference to Window:Function
											true,	// Accept Using Reference to Global Functions
											true,	// Accept Using Reference to Local Functions
											true,	// Accept Using Reference to Global VAR
											true,	// Accept Using Reference to Local VAR
											false,	// Accept Convertion from String To Bool
											true,	// Accept Convertion from Bool To String
											TempToken,				// SYNTAX[] string
											(TempTokenCount - 1),	// SYNTAX_LONG
											WIN_OR_CLASS,			// TMP_WINDOW_NAME
											TheFunction,			// TMP_FUNCTION_NAME
											o_tokens);
		
		if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG

		if (IsInsideClass)
		{
			// just for fixing this ...
			// *** Generate Code ***
			CPP_CLASS.append(ScriptSyntaxBuffer + " ; \n ");
			// *** *** *** *** *** ***
		}
		else if (!IsInsideFunction)
		{
			// Global Area
			// Bool a = ... ;
			CPP_GLOBAL.append(ScriptSyntaxBuffer + " ; \n ");
		}
		else
		{
			// Local Area
			// Bool a = ... ;
			if (!IsInsideWindow)
				// Global Function
				CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ; \n ");
			else
				// Local Function
				cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ; \n ");
		}
		// *** *** *** *** *** ***

		if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
		return; // continue;
	}
	else if (Token[BOOL_POS + 2] != "")
		ErrorCode("أمر غير معروف '" + Token[BOOL_POS + 2] + "', ربما تقصد '=' ", o_tokens);
	
	if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
}
