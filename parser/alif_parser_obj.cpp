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

void parser_Obj(string Token[2048], CLASS_TOKEN *o_tokens){
	// كائن

	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;
	
	// ك:م = 2

	if (!IsInsideFunction)
		ErrorCode("يجب استعمال الكائن داخل دالة ' " + Token[1] + " ' ", o_tokens);
	
	if (Token[2] != ":")
		ErrorCode("يجب وضع نقطتين ':' بين الكائن و المنتمي، أي بعد ' " + Token[1] + " ' ", o_tokens);
	
	if (Token[3] == "بناء")
		ErrorCode("لا يمكن استدعاء دالة ' بناء() '، هته الدالة تستدعى بشكل آلي عند إنشاء الكائن", o_tokens);
	
	if (Token[3] == "هدم")
		ErrorCode("لا يمكن استدعاء دالة ' هدم() '، هته الدالة تستدعى بشكل آلي عند الحاجة إلى تدمير هذا الكائن", o_tokens);

	if (Token[3] == "")
		ErrorCode("يجب وضع اسم المنتمي بعد ' " + Token[1] + ":' ", o_tokens);
	
	// C++, allow create Obj on global, global-class, local, but using it only on func.
	string OBJ_ID;
	bool IS_GLOBAL_OBJ = false;
	string WIN_OR_CLASS; // Needed by CheckForSyntax()

	if (IsInsideClass)
	{
		if (OBJ_IS_SET[std::make_pair(TheClass + TheFunction, Token[1])])
		{
			// Class -> Function.
			// Local Class Obj.

			OBJ_ID = TheClass + TheFunction;
			WIN_OR_CLASS = TheClass;
		}
		else if (OBJ_IS_SET[std::make_pair(TheClass, Token[1])])
		{
			// Global Class.

			OBJ_ID = TheClass;
			WIN_OR_CLASS = TheClass;
			IS_GLOBAL_OBJ = true;
		}
		else if (OBJ_IS_SET[std::make_pair("", Token[1])])
		{
			// Global Area Obj.

			OBJ_ID = "";
			WIN_OR_CLASS = "";
			IS_GLOBAL_OBJ = true;
		}
		else
			ErrorCode("علة 1: انتماء الكائن غير معروف ' " + Token[1] + " ' ", o_tokens);
	}
	else if (IsInsideWindow)
	{
		if (OBJ_IS_SET[std::make_pair(TheWindow + TheFunction, Token[1])])
		{
			// Window -> Function.
			// Local Obj.

			OBJ_ID = TheWindow + TheFunction;
			WIN_OR_CLASS = TheWindow;
		}
		else if (OBJ_IS_SET[std::make_pair("", Token[1])])
		{
			// Global Area Obj.

			OBJ_ID = "";
			WIN_OR_CLASS = "";
			IS_GLOBAL_OBJ = true;
		}
		else
			ErrorCode("علة 2: انتماء الكائن غير معروف ' " + Token[1] + " ' ", o_tokens);
	}
	else
	{
		if (OBJ_IS_SET[std::make_pair(TheFunction, Token[1])])
		{
			// Global Function.
			// Local Obj.

			OBJ_ID = TheFunction;
			WIN_OR_CLASS = "";
		}
		else if (OBJ_IS_SET[std::make_pair("", Token[1])])
		{
			// Global Area Obj.

			OBJ_ID = "";
			WIN_OR_CLASS = "";
			IS_GLOBAL_OBJ = true;
		}
		else
			ErrorCode("علة 3: انتماء الكائن غير معروف ' " + Token[1] + " ' ", o_tokens);
	}

	/*
	if (OBJ_IS_SET[std::make_pair(TheWindow + TheFunction, Token[1])])
	{
		// Window -> Function.
		// Local Obj.
		OBJ_ID = TheWindow + TheFunction;
		WIN_OR_CLASS = TheWindow;
		if(DEBUG)DEBUG_MESSAGE("{{WF}}", o_tokens); // DEBUG
	}
	else if (OBJ_IS_SET[std::make_pair(TheClass + TheFunction, Token[1])])
	{
		// Class -> Function.
		// Local Class Obj.
		OBJ_ID = TheClass + TheFunction;
		WIN_OR_CLASS = TheClass;
		if(DEBUG)DEBUG_MESSAGE("{{CF}}", o_tokens); // DEBUG
	}
	else if (OBJ_IS_SET[std::make_pair(TheClass, Token[1])])
	{
		// Class.
		// Global Class Obj.
		OBJ_ID = TheClass;
		WIN_OR_CLASS = TheClass;
		if(DEBUG)DEBUG_MESSAGE("{{C}"+ TheClass + "}", o_tokens); // DEBUG
	}
	else
	{
		// Global Area.
		// Global Obj.
		OBJ_ID = "";
		WIN_OR_CLASS = "";
		IS_GLOBAL_OBJ = true;
	}
	*/
	
	string TK_CLASS = OBJ_CLASS[std::make_pair(OBJ_ID, Token[1])];
	
	if (!CLASS_G_VAR_IS_SET[std::make_pair(TK_CLASS, Token[3])] &&
		!CLASS_FUN_IS_SET[std::make_pair(TK_CLASS, Token[3])])
		ErrorCode("الصنف ' " + TK_CLASS + " ' ليس فيه أي منتمي معرف باسم ' " + Token[3] + " ' ", o_tokens);
	
	string MEMBER_TYPE;
	
	if (CLASS_G_VAR_IS_SET[std::make_pair(TK_CLASS, Token[3])])
	{
		// Var member
		
		if (CLASS_G_VAR_PRIVATE[std::make_pair(TK_CLASS, Token[3])])
			ErrorCode("لا يمكن استدعاء المتغير ' " + Token[3] + " ' المنتمي للصنف ' " + TK_CLASS + " ' لأنه من نوع ' خاص ' ", o_tokens);
		
		if (Token[4] != "=")
			ErrorCode("يجب وضع '=' بعد ' " + Token[1] + " : " + Token[3] + " ' ", o_tokens);
		
		if (Token[5] == "")
			ErrorCode("يجب وضع قيمة بعد '=' ", o_tokens);
		
		MEMBER_TYPE = CLASS_G_VAR_TYPE[std::make_pair(TK_CLASS, Token[3])];

		if (MEMBER_TYPE == "عادم")
			ErrorCode("لا يمكن إضافة منتمي عادم ' " + Token[1] + " : " +Token[3] + " ' ", o_tokens);
		
		TempTokenCount = 0;
		for (int p = 4; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // | = a + b |
		{
			if (Token[p] != "")
			{
				TempToken[TempTokenCount] = Token[p];
				TempTokenCount++;
			}
		}

		if (IS_GLOBAL_OBJ)
		{
			if(DEBUG)DEBUG_MESSAGE("		[GLOBAL-OBJ ' " + Token[1] + " ' : ' " + Token[3] + " '] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Obj:Mem = ...
			if (IsInsideClass)
			{
				CPP_CLASS.append(" " + GlobalObj_ID[Token[1]] + "." + Global_ID[Token[3]] + " = ");
			}
			else if (!IsInsideWindow)
			{
				// global func
				CPP_GLOBAL_FUN.append(" " + GlobalObj_ID[Token[1]] + "." + Global_ID[Token[3]] + " = ");
			}
			else
			{
				// local func
				cpp_AddScript(TheFunction, " " + GlobalObj_ID[Token[1]] + "." + Global_ID[Token[3]] + " = ");
			}
			// *** *** *** *** *** ***
		}
		else
		{
			if(DEBUG)DEBUG_MESSAGE("		[LOCAL-OBJ ' " + Token[1] + " ' : ' " + Token[3] + " '] = ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Obj:Mem = ...
			if (IsInsideClass)
			{
				CPP_CLASS.append(" " + Obj_ID[Token[1]] + "." + Global_ID[Token[3]] + " = ");
			}
			else if (!IsInsideWindow)
			{
				// global func
				//CPP_GLOBAL_FUN.append(" " + Obj_ID[Token[1]] + "." + Global_ID[Token[3]] + " = ");
				ErrorCode("علة : كائن محلي داخل دالة عامة ' " + Token[1] + " : " + Token[3] + " ' ", o_tokens);
			}
			else
			{
				// local func
				cpp_AddScript(TheFunction, " " + Obj_ID[Token[1]] + "." + Global_ID[Token[3]] + " = ");
			}
			// *** *** *** *** *** ***
		}

		ScriptSyntaxBuffer = CheckForSyntax(MEMBER_TYPE, // OBJECTIF_TYPE
											true, // Accept Using Reference to Window:Controls
											true, // Accept Using Reference to Window:Function
											true, // Accept Using Reference to Global Functions
											true, // Accept Using Reference to Local Functions
											true, // Accept Using Reference to Global VAR
											true, // Accept Using Reference to Local VAR
											false, // Accept Convertion from String To Int
											true, // Accept Convertion from Int To String
											TempToken, 				// SYNTAX[] string
											(TempTokenCount - 1), 	// SYNTAX_LONG int
											WIN_OR_CLASS, 			// TMP_WINDOW_NAME or CLASS_NAME
											TheFunction, 			// TMP_FUNCTION_NAME
											o_tokens);
	
		if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG

		// *** Generate Code ***
		// Obj:Mem = ... ;
		if (IsInsideClass)
		{
			CPP_CLASS.append(ScriptSyntaxBuffer + " ; \n");
		}
		else if (!IsInsideWindow)
		{
			// global func
			CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ; \n");
		}
		else
		{
			// local func
			cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ; \n");
		}
		// *** *** *** *** *** ***

		return; // continue;
	}
	else if (CLASS_FUN_IS_SET[std::make_pair(TK_CLASS, Token[3])])
	{
		// Function member
		
		if (CLASS_FUN_PRIVATE[std::make_pair(TK_CLASS, Token[3])])
			ErrorCode("لا يمكن استدعاء الدالة ' " + Token[3] + " ' المنتمية للصنف ' " + TK_CLASS + " ' لأنها من نوع ' خاص ' ", o_tokens);
		
		if (CLASS_FUN_TYPE[std::make_pair(TK_CLASS, Token[3])] != "عادم")
			ErrorCode("الدالة ' " + Token[3] + " ' المنتمية للصنف ' " + TK_CLASS + " ' ليست من نوع 'عادم'، لذى لابد من استعمال متغير ليلتقط قيمة الإرجاع ", o_tokens);
		
		if (Token[4] != "(")
			ErrorCode("يجب وضع '(' بعد ' " + Token[1] + " : " + Token[3] + " ' ", o_tokens);
		
		if (Token[5] == "")
			ErrorCode("يجب غلق القوس بعد ' " + Token[1] + " : " + Token[3] + " (' ", o_tokens);
		
		if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
			ErrorCode("يجب غلق القوس", o_tokens);
		
		MEMBER_TYPE = CLASS_FUN_TYPE[std::make_pair(TK_CLASS, Token[3])];
		
		TempTokenCount = 0;
		for (int p = 5; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // | a + b, c, 5) |
		{
			if (Token[p] != "")
			{
				TempToken[TempTokenCount] = Token[p];
				TempTokenCount++;
			}
		}

		if (IS_GLOBAL_OBJ)
		{
			if(DEBUG)DEBUG_MESSAGE("		[GLOBAL-OBJ ' " + Token[1] + " ':'" + Token[3] + " '(Func)( ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Obj:MemFunc ( ...
			if (IsInsideClass)
			{
				CPP_CLASS.append(" " + GlobalObj_ID[Token[1]] + ".ClassFUNCTION_" + ID[Token[3]] + "( ");
			}
			else if (!IsInsideWindow)
			{
				// global func
				CPP_GLOBAL_FUN.append(" " + GlobalObj_ID[Token[1]] + ".ClassFUNCTION_" + ID[Token[3]] + "( ");
			}
			else
			{
				// local func
				cpp_AddScript(TheFunction, " " + GlobalObj_ID[Token[1]] + ".ClassFUNCTION_" + ID[Token[3]] + "( ");
			}
			// *** *** *** *** *** ***
		}
		else
		{
			if(DEBUG)DEBUG_MESSAGE("		[LOCAL-OBJ ' " + Token[1] + " ':'" + Token[3] + " '(Func)( ", o_tokens); // DEBUG

			// *** Generate Code ***
			// Obj:MemFunc ( ...
			if (IsInsideClass)
			{
				CPP_CLASS.append(" " + Obj_ID[Token[1]] + ".ClassFUNCTION_" + ID[Token[3]] + "( ");
			}
			else if (!IsInsideWindow)
			{
				// global func
				//CPP_GLOBAL_FUN.append();
				ErrorCode("علة : كائن محلي داخل دالة عامة ' " + Token[1] + " : " + Token[3] + " ' ", o_tokens);
			}
			else
			{
				// local func
				cpp_AddScript(TheFunction, " " + Obj_ID[Token[1]] + ".ClassFUNCTION_" + ID[Token[3]] + "( ");
			}
			// *** *** *** *** *** ***
		}
		
		ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG(	false, 
												TK_CLASS, 
												Token[3], 
												2, // 2 = fun member
												WIN_OR_CLASS, 
												TheFunction, 
												TempToken, 
												(TempTokenCount - 1), 
												o_tokens);

		if(DEBUG)DEBUG_MESSAGE(")] \n\n", o_tokens); // DEBUG

		// *** Generate Code ***
		// Obj:MemFunc ( ... ) ;
		if (IsInsideClass)
		{
			CPP_CLASS.append(ScriptSyntaxBuffer + " ); \n");
		}
		else if (!IsInsideWindow)
		{
			// global func
			CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ); \n");
		}
		else
		{
			// local func
			cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ); \n");
		}
		// *** *** *** *** *** ***

		return; // continue;
	}
}