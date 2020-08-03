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

void parser_TwoPointOperator(string Token[2048], CLASS_TOKEN *o_tokens){
	// :

	// ---------------------------------------------------------------------------------
	// : (Operator between members)
	// ---------------------------------------------------------------------------------

	// Window:option() 				| win:show()
	// Window:Option = a 			| win:with = 100
	// Window:local_func(a, b)		| win:my_func(a, b)
	// Window:Control:Option()		| win:ctr:disable()
	// Window:Control:Option = a	| win:ctr:text = "abc"	

	// Control:Option()				| ctr:disable()
	// Control:Option = a			| ctr:text = "abc"

	if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;
		
	if (!IsInsideFunction)
		ErrorCode("يجب استعمال الاداة داخل دالة", o_tokens);
	
	//int ARG;
	//string WIN, CONTROL, OPTION, OPTION_TYPE;
	//WIN = "";
	//CONTROL = "";
	//OPTION = "";
	//ARG = 0;

	string CTR_WIN;
	string CTR_CONTROL;
	string CTR_OPTION;
	string CTR_OPTION_TYPE;
	string CTR_OPTION_CPP_END;
	int CTR_ARG;
	//int CTR_OPERATOR_POSITION;

	// TODO: main:center() not work if main set only by code !

	// TODO: devision, must allow '/' and '\'
	
	if (CONTROL_WIN_IS_SET[Token[1]] || 
		WIN_IS_SET [Token[1]] ||
		(Token[1] == "رئيسية" && MAIN_WIN_IS_SET))
	{
		// Window:option() 				| win:show()
		// Window:Option = a 			| win:with = 100
		// Window:local_func(a, b)		| win:my_func(a, b)
		// Window:Control:Option()		| win:ctr:disable()
		// Window:Control:Option = a	| win:ctr:text = "abc"	
		
		if (Token[2] != ":")
			ErrorCode("يجب اضافه ' : ' بعد ' " + Token[1] + " ' ", o_tokens);
		
		if (Token[3] == "")
			ErrorCode("يجب اضافه عضو تابع ل ' " + Token[1] + " ' بعد ':' ", o_tokens);
		
		if (Token[4] == "")
			ErrorCode("يجب اضافه ':' أو '()' أو '=' بعد ' " + Token[1] + " " + Token[2] + " " + Token[3] + " ' ", o_tokens);
		
		if (Token[4] != "(" && Token[4] != ":" && Token[4] != "=")
			ErrorCode("أمر غير معروف ' " + Token[4] + " ', يجب اضافه ':' أو '()' أو '=' ", o_tokens);
		
		if (Token[4] == "(")
		{
			// ---------------------------------------------------------------------------------
			// Window : Options ()
			// ---------------------------------------------------------------------------------

			// Window:win-option() 				| win:show()
			// Window:win-local_func(a, b)		| win:my_func(a, b)

			// -----------------------
			// Window:win-option() 
			// -----------------------

			if (Token[3] == "إظهار")
			{
				// Window:Show()
				
				if (Token[5] == "")
					ErrorCode("يجب اضافه ')' في النهاية", o_tokens);

				if (Token[5] != ")")
					ErrorCode("أمر غير معروف ' " + Token[5] + " ', على كل حال اظهار لا تحتاج ايت خاصيه", o_tokens);

				if (Token[6] != "")
					ErrorCode("أمر غير معروف ' " + Token[6] + " ' ", o_tokens);

				if(DEBUG)DEBUG_MESSAGE("		[WIN'" + Token[1] + " ':OPT'" + Token[3] + " '(CPP_WIN_SHOW)] \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Window:Show()
				if (!IsInsideWindow)
					CPP_GLOBAL_FUN.append(CG_WIN_SHOW(Token[1]));
				else
					cpp_AddScript(TheFunction, CG_WIN_SHOW(Token[1]));
				// *** *** *** *** *** ***

				return; // continue;
			}
			else if (Token[3] == "إخفاء")
			{
				// Window:Hide()
				
				if (Token[5] == "")
					ErrorCode("يجب اضافه ')' في النهاية", o_tokens);

				if (Token[5] != ")")
					ErrorCode("أمر غير معروف ' " + Token[5] + " ', على كل حال اخفاء لا تحتاج ايت خاصيه", o_tokens);
				
				if (Token[6] != "")
					ErrorCode("أمر غير معروف ' " + Token[6] + " ' ", o_tokens);

				if(DEBUG)DEBUG_MESSAGE("		[WIN'" + Token[1] + " ':OPT'" + Token[3] + " '(CPP_WIN_HIDE)] \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Window:Hide()
				if (!IsInsideWindow)
					CPP_GLOBAL_FUN.append(CG_WIN_HIDE(Token[1]));
				else
					cpp_AddScript(TheFunction, CG_WIN_HIDE(Token[1]));
				// *** *** *** *** *** ***

				return; // continue;
			}
			else if (Token[3] == "تدمير")
			{
				// Window:Destroy()
				
				if (Token[5] == "")
					ErrorCode("يجب اضافه ')' في النهاية", o_tokens);

				if (Token[5] != ")")
					ErrorCode("أمر غير معروف ' " + Token[5] + " ', على كل حال تدمير لا تحتاج ايت خاصيه", o_tokens);
				
				if (Token[6] != "")
					ErrorCode("أمر غير معروف ' " + Token[6] + " ' ", o_tokens);

				if(DEBUG)DEBUG_MESSAGE("		[WIN'" + Token[1] + " ':OPT'" + Token[3] + " '(CPP_WIN_DESTROY)] \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Window:Destroy()
				if (!IsInsideWindow)
					CPP_GLOBAL_FUN.append(CG_WIN_DESTROY(Token[1]));
				else
					cpp_AddScript(TheFunction, CG_WIN_DESTROY(Token[1]));
				// *** *** *** *** *** ***

				return; // continue;
			}
			else if (Token[3] == "منتصف")
			{
				// Window:Center()
				
				if (Token[5] == "")
					ErrorCode("يجب اضافه ')' في النهاية", o_tokens);

				if (Token[5] != ")")
					ErrorCode("أمر غير معروف ' " + Token[5] + " ', على كل حال منتصف لا تحتاج ايت خاصيه", o_tokens);
				
				if (Token[6] != "")
					ErrorCode("أمر غير معروف ' " + Token[6] + " ' ", o_tokens);

				if(DEBUG)DEBUG_MESSAGE("		[WIN'" + Token[1] + " ':OPT'" + Token[3] + " '(Center)] \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Window:Center()
				if (!IsInsideWindow)
					CPP_GLOBAL_FUN.append(CG_WIN_CENTER(Token[1]));
				else
					cpp_AddScript(TheFunction, CG_WIN_CENTER(Token[1]));
				// *** *** *** *** *** ***

				return; // continue;
			}

			// -----------------------
			// Window:win-local_func() 
			// -----------------------

			else if (L_FUN_IS_SET[std::make_pair(Token[1], Token[3])])
			{
				if (Token[4] != "(")
					ErrorCode("من اجل نداء الدالة ' " + Token[3] + " ' يجب اضافه '()' بعد ' " + Token[1] + " " + 
									Token[2] + " " + Token[3] + " ' ", o_tokens);

				if(DEBUG)DEBUG_MESSAGE("		[WIN'" + Token[1] + " ':LOCAL_FUNC'" + Token[3] + " '( ", o_tokens); // DEBUG

				// *** Generate Code ***
				if (!IsInsideWindow)
					// Call other win local_func from global func
					CPP_GLOBAL_FUN.append(CG_WIN_MEMBER(Token[1], ID[Token[3]] + " ("));
				else
					// Call other win local_func from local func
					cpp_AddScript(TheFunction, CG_WIN_MEMBER(Token[1], ID[Token[3]] + " ("));
				// *** *** *** *** *** ***

				TempTokenCount = 0;
				for (int p = 3; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // | a, b + 2, c) |
				{
					if (Token[p] != "")
					{
						TempToken[TempTokenCount] = Token[p];
						TempTokenCount++;
					}
				}

				// Check local fun Args and Syntax : fun (a + c, 2 * (b - 1))
				ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG(false, TheWindow, Token[1], 0, TheWindow, TheFunction, TempToken, 
				(TempTokenCount - 1),  o_tokens);

				if(DEBUG)DEBUG_MESSAGE(" )] \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				if (!IsInsideWindow)
					// Call other win-local_func from global func
					CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ); \n } \n");
				else
					// Call other win-local_func from local func
					cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ); \n } \n");
				// *** *** *** *** *** ***

				return; // continue;
			}
			// ---------------------------------------------------------------------------------
			else
				ErrorCode("النافذة ' " + Token[1] + " ' لا تحتوي على دالة محليه بإسم ' " + Token[3] + " ' ", o_tokens);
			
			// Exception!
			return; // continue;
		}
		else if (Token[4] == "=")
		{
			// ---------------------------------------------------------------------------------
			// Window : Value =
			// ---------------------------------------------------------------------------------

			// Window:Title = 
			// Window:With =
			// Window:Heint =
			// Window:X =
			// Window:Y =
			// Window:Color = 
			// Window:Background =

			// bool A = Window:Is_Show

			if (Token[5] == "")
					ErrorCode("يجب اضافه قيمة بعد '=' ", o_tokens);

			string OBJECTIF_TYPE;
			string VALUE_CPP_END;

			if (Token[3] == "عنوان")
			{
				// Window:Title = 

				OBJECTIF_TYPE = "نص";

				if(DEBUG)DEBUG_MESSAGE("		[WIN'" + Token[1] + " ':VALUE'" + Token[3] + "(SetWindowTitle)' = ", o_tokens); // DEBUG

				// *** Generate Code ***
				if (!IsInsideWindow)
					// Window : Title =
					CPP_GLOBAL_FUN.append(CG_WIN_MEMBER(Token[1], "SetLabel( "));
				else
					// Window : Title =
					cpp_AddScript(TheFunction, CG_WIN_MEMBER(Token[1], "SetLabel( "));
				// *** *** *** *** *** ***
				VALUE_CPP_END = " ); } \n"; // Need close [if (WINDOW_IS_CONSTRUCTION_...]
				// *** *** *** *** *** ***
			}
			/*

			// TODO: so, need Draw on wxPanel P !
			// http://zetcode.com/gui/wxwidgets/gdi/

			else if (Token[3] == "نص")
			{
				// Window:Text = 

				OBJECTIF_TYPE = "نص";

				if(DEBUG)DEBUG_MESSAGE("		[WIN'" + Token[1] + " ':VALUE'" + Token[3] + "(SetWindowLabel)' = ", o_tokens); // DEBUG

				// *** Generate Code ***
				if (!IsInsideWindow)
					// Window : Text =
					CPP_GLOBAL_FUN.append(CG_WIN_MEMBER(Token[1], "SetLabel( "));
				else
					// Window : Text =
					cpp_AddScript(TheFunction, CG_WIN_MEMBER(Token[1], "SetLabel( "));
				// *** *** *** *** *** ***
				VALUE_CPP_END = " ); \n";
				// *** *** *** *** *** ***
			}
			*/
			else
			{
				ErrorCode("النافذة ' " + Token[1] + " ' لا تحتوي على خاصيه باسم ' " + Token[3] + " ' ", o_tokens);
			}

			// Window : Option = ...

			TempTokenCount = 0;
			for (int p = 4; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // | = a * b + 2 / (c) + 1 |
			{
				if (Token[p] != "")
				{
					TempToken[TempTokenCount] = Token[p];
					TempTokenCount++;
				}
			}

			ScriptSyntaxBuffer = CheckForSyntax(OBJECTIF_TYPE,	// OBJECTIF_TYPE
											true, 			// Accept Using Reference to Window:Controls
											true, 			// Accept Using Reference to Window:Function
											true, 			// Accept Using Reference to Global Functions
											true, 			// Accept Using Reference to Local Functions
											true, 			// Accept Using Reference to Global VAR
											true, 			// Accept Using Reference to Local VAR
											false, 			// Accept Convertion from String To Int
											true, 			// Accept Convertion from Int To String
											TempToken,				// SYNTAX[] string
											(TempTokenCount - 1),	// SYNTAX_LONG int
											TheWindow,		// TMP_WINDOW_NAME
											TheFunction,		// TMP_FUNCTION_NAME
											o_tokens);

			// *** Generate Code ***
			if (!IsInsideWindow)
				// Window : Option =
				CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + VALUE_CPP_END);
			else
				// Window : Option =
				cpp_AddScript(TheFunction, ScriptSyntaxBuffer + VALUE_CPP_END);
			// *** *** *** *** *** ***

			return; // continue;
		}
		else if (Token[4] == ":")
		{
			// ---------------------------------------------------------------------------------
			// Window : Control : Options
			// ---------------------------------------------------------------------------------

			// Window:Control:Option()		| win:ctr:disable()
			// Window:Control:Option = a	| win:ctr:text = "abc"	
			
			if (!CONTROL_IS_SET[std::make_pair(Token[1], Token[3])])
				ErrorCode("النافذة ' " + Token[1] + " ' لا تحتوي على اداه باسم ' " + Token[3] + " ' ", o_tokens);
		
			if (Token[6] == "")
				ErrorCode("يجب اضافه '=' أو '()' بعد ' " + Token[1] + " : " + Token[3] + " : " + Token[5] + " ' ", o_tokens);
		
			if (Token[6] != "=" && Token[6] != "(")
				ErrorCode("أمر غير معروف ' " + Token[6] + " ' ", o_tokens);
		
			if (Token[7] == "")
				ErrorCode("يجب اضافه قيمة بعد ' " + Token[1] + " : " + Token[3] + " : " + Token[5] + " " + Token[6] + " ' ", o_tokens);

			CTR_WIN = Token[1];
			CTR_CONTROL = Token[3];
			CTR_OPTION = Token[5];
			CTR_ARG = 6;
		}
		else
		{
			// Exception !
			ErrorCode("يجب اضافه ':' أو '()' أو '=' بعد ' " + Token[1] + " " + Token[2] + " " + Token[3] + " ' ", o_tokens);
		}
	}
	else if (CONTROL_IS_SET[std::make_pair(TheWindow, Token[1])])
	{
		// ---------------------------------------------------------------------------------
		// Control : Options
		// ---------------------------------------------------------------------------------

		// Control:Option()				| ctr:disable()
		// Control:Option = a			| ctr:text = "abc"
		
		if (Token[2] == "")
			ErrorCode("يجب اضافة ':' بعد ' " + Token[1] + " ' ", o_tokens);

		if (Token[2] != ":")
			ErrorCode("أمر غير معروف ' " + Token[2] + " ', يجب اضافة ':' بعد ' " + Token[1] + " ' ", o_tokens);
		
		if (Token[3] == "")
			ErrorCode("يجب اضافة خاصية بعد ' " + Token[1] + " : ' ", o_tokens);
		
		if (Token[4] == "")
			ErrorCode("يجب اضافة '=' أو '()' بعد ' " + Token[1] + " : " + Token[3] + " ' ", o_tokens);
		
		if (Token[4] != "=" && Token[4] != "(")
			ErrorCode("أمر غير معروف ' " + Token[6] + " ' ", o_tokens);
		
		if (Token[5] == "")
			ErrorCode("يجب اضافة قيمة بعد ' " + Token[1] + " : " + Token[3] + " " + Token[4] + " ' ", o_tokens);
		
		CTR_WIN = TheWindow;
		CTR_CONTROL = Token[1];
		CTR_OPTION = Token[3];
		CTR_ARG = 4;
	}
	else
	{
		// Exception !
		ErrorCode("أمر غير معروف ' " + Token[1] + " ', يجب أن تكون فقط نافذه او اداة", o_tokens);
	}
	
	// ---------------------------------------------------------------------------------
	// Check Control Options if valid
	// For Control:Option AND Window:Control:Option
	// ---------------------------------------------------------------------------------

	if (Token[CTR_ARG] == "=")
	{
		// -----------------------
		// Window : Control : Option = ...
		// 			Control : Option = ...
		// -----------------------

		if (CTR_OPTION == "نص") // Control:نص = ...
		{
			CTR_OPTION_TYPE = "نص";

			if(DEBUG)DEBUG_MESSAGE("		[WIN'" + CTR_WIN + " ':CTR'" + CTR_CONTROL + " ':VALUE'" + CTR_OPTION + "(Text)(String)' = ", o_tokens); // DEBUG

			// *** Generate Code ***
			// CTR : Label =

			// SetLabel not working on Linux.. tray SetValue
			string SetValueFix = "SetValue";
			if (CONTROL_TYPE[std::make_pair(CTR_WIN, CTR_CONTROL)] == "ملصق")
				SetValueFix = "SetLabel";

			if (!IsInsideWindow)
				CPP_GLOBAL_FUN.append(" if (WINDOW_IS_CONSTRUCTION_" + ID[CTR_WIN] + ") { OBJ_CTR_" + ID[CTR_WIN] + "_" + Control_ID[CTR_CONTROL] + "->" + SetValueFix + "( "); // SetLabel
			else
				cpp_AddScript(TheFunction, " if (WINDOW_IS_CONSTRUCTION_" + ID[CTR_WIN] + ") { OBJ_CTR_" + ID[CTR_WIN] + "_" + Control_ID[CTR_CONTROL] + "->" + SetValueFix + "( ");
			// *** *** *** *** *** ***
			CTR_OPTION_CPP_END = " ); } \n";
			// *** *** *** *** *** ***
		}
		else
		{
			ErrorCode("الأداة ' " + CTR_CONTROL + " ' لا تحتوي على خاصيه باسم ' " + CTR_OPTION + " ' ", o_tokens);
		}
	}
	else if (Token[CTR_ARG] == "(")
	{
		// -----------------------
		// Window : Control : Option ( ... )
		// 			Control : Option ( ... )
		// -----------------------

		if (CTR_OPTION == "تجميد")
		{
			CTR_OPTION_TYPE = "عادم"; // Exception!

			if (Token[CTR_ARG + 1] != ")")
				ErrorCode("أمر غير معروف ' " + Token[CTR_ARG + 1] + " ', على كل حال تجميد لا تحتاج ايت خاصيه", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("		[WIN'" + CTR_WIN + " ':CTR'" + CTR_CONTROL + " ':OPTION'" + CTR_OPTION + "(FREEZ)(VOID)']", o_tokens); // DEBUG

			// *** Generate Code ***

			// *** *** *** *** *** ***

			return; // continue;
		}
		else
		{
			ErrorCode("الأداة ' " + CTR_CONTROL + " ' لا تحتوي على خاصيه باسم ' " + CTR_OPTION + " ' ", o_tokens);
		}
	}
	else
	{
		// Exception !
		ErrorCode("أمر غير معروف ' " + Token[CTR_ARG] + " ' ", o_tokens);
	}

	// Check Control arguments only for = ...

	TempTokenCount = 0;
	for (int p = CTR_ARG; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
	{
		if (Token[p] != "")
		{
			TempToken[TempTokenCount] = Token[p];
			TempTokenCount++;
		}
	}

	ScriptSyntaxBuffer = CheckForSyntax(CTR_OPTION_TYPE,	// OBJECTIF_TYPE
					true, 			// Accept Using Reference to Window:Controls
					true, 			// Accept Using Reference to Window:Function
					true, 			// Accept Using Reference to Global Functions
					true, 			// Accept Using Reference to Local Functions
					true, 			// Accept Using Reference to Global VAR
					true, 			// Accept Using Reference to Local VAR
					false, 			// Accept Convertion from String To Int
					true, 			// Accept Convertion from Int To String
					TempToken,				// SYNTAX[] string
					(TempTokenCount - 1),	// SYNTAX_LONG int
					TheWindow,		// TMP_WINDOW_NAME
					TheFunction,		// TMP_FUNCTION_NAME
					o_tokens);

	if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG

	// *** Generate Code ***
	if (!IsInsideWindow)
		// Control : Option '=' أو '(' ...
		CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + CTR_OPTION_CPP_END);
	else
		// Control : Option '=' أو '(' ...
		cpp_AddScript(TheFunction, ScriptSyntaxBuffer + CTR_OPTION_CPP_END);
	// *** *** *** *** *** ***

	return; // continue;
}

