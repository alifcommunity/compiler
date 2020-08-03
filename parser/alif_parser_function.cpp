// ==================================
// (C)2019 DRAGA Hassan.
// 	www.aliflang.org
// ----------------------------------
// Alif Programming Language
// ==================================
#pragma once
#define UNICODE
#define _UNICODE
using namespace std;
// ----------------------------------

void parser_Function(string Token[2048], CLASS_TOKEN *o_tokens){
	
	//دالة

	int FUN_POS = 1;
	
	if (IsInsideFunction)
		ErrorCode("لا يمكن انشاء دالة داخل دالة، الدالة الحالية : " + TheFunction, o_tokens);
	
	if (Token[FUN_POS + 1] == "")
		ErrorCode("يجب اعطاء اسم لهته الدالة الجديدة", o_tokens);
	
	if (Token[FUN_POS + 1] == "رئيسية") // دالة رئيسية
	{
		if (!IsInsideWindow)
			ErrorCode("لا يمكن انشاء دالة رئيسية في المجال العام، بل يجب انشائها داخل نافذة أو داخل صنف", o_tokens);
		
		if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);
		
		if (Token[FUN_POS + 2] == "(")
		{
			if (Token[FUN_POS + 3] == "")
				ErrorCode("يجب اغلاق القوس ')' ", o_tokens);
			
			if (Token[FUN_POS + 3] != ")")
				ErrorCode("الدالة الرئيسية لا تقبل الخصائص", o_tokens);
			
			if (Token[FUN_POS + 4] != "")
				ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 4] + " ' ", o_tokens);
		}

		if (!o_tokens->TOKENS_PREDEFINED)
		{
			if (MAIN_FUN_IS_SET[TheWindow])
				ErrorCode("الدالة الرئيسية تم انشاؤها مسبقا في السطر : " + MAIN_FUN_AT_LINE[TheWindow], o_tokens);

			MAIN_FUN_IS_SET[TheWindow] = true;
			MAIN_FUN_AT_LINE[TheWindow] = IntToString(o_tokens->Line);

			IsInsideFunction = true; // Need by Tokens Predefined
			TheFunction = "رئيسية"; // Need by Tokens Predefined

			return; // continue;
		}

		IsInsideFunction = true;
		TheFunction = "رئيسية";
		TheFunction_TYPE = "عادم";
		RETURN_FUN[std::make_pair(TheWindow, "رئيسية")] = "OK";

		Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
		Win_CurrentTotalFucntion++;
		Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;
		
		if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [MAIN] \n\n", o_tokens); // DEBUG
		
		return; // continue;
	}
	else if (Token[FUN_POS + 1] == "نقر") // || Token[FUN_POS + 1] == "فارة_نقر") // دالة Mouse_Click MyFunctionName ()
	{
		// --------------------------
		// Control Function
		// --------------------------

		if (!IsInsideWindow)
			ErrorCode("يجب استعمال هذه الخاصيه داخل النافذه ' " + Token[FUN_POS + 1] + " ' ", o_tokens);

		if (Token[FUN_POS + 2] == "")
			ErrorCode("يجب تحديد اسم الاداة", o_tokens);

		if (!CONTROL_IS_SET[std::make_pair(TheWindow, Token[FUN_POS + 2])])
			ErrorCode("النافذة ' " + TheWindow + " ' لا تحتوي على أداة باسم ' " +  Token[FUN_POS + 2] + " ' ", o_tokens);

		if (Token[FUN_POS + 3] != "" && Token[FUN_POS + 3] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 3] + " ' ", o_tokens);

		if (Token[FUN_POS + 3] == "(")
		{
			if (Token[FUN_POS + 4] != ")")
				ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 4] + " ', However Control Function didnt need args! ", o_tokens);
			
			if (Token[FUN_POS + 5] != "")
				ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 5] + " ' ", o_tokens);
		}

		if (!o_tokens->TOKENS_PREDEFINED)
		{
			// Control function
			IS_CONTROL_FUNCTION_NAME = true;
			ADD_FUN(false, TheWindow, (Token[FUN_POS + 1] + " " + Token[FUN_POS + 2]), "عادم", o_tokens->Line, o_tokens);

			IsInsideFunction = true; // Need by Tokens Predefined
			TheFunction = (Token[FUN_POS + 1] + " " + Token[FUN_POS + 2]); // Need by Tokens Predefined

			return; // continue;
		}

		if (Token[FUN_POS + 1] == "نقر") // || Token[FUN_POS + 1] == "فارة_نقر")
		{
			if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [Mouse_Click] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			// Control Function ()
			CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
			CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " void FUNCTION_MouseClick_" + Control_ID[Token[FUN_POS + 2]] + "(wxCommandEvent &event); \n";
			
			CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "EVENT")];
			CPP_WINDOW[std::make_pair(TheWindow, "EVENT")] = CBUFER + " EVT_BUTTON(ID_CTR_" + ID[TheWindow] + "_" + Control_ID[Token[FUN_POS + 2]] + ", CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_MouseClick_" + Control_ID[Token[FUN_POS + 2]] + ") \n";
			
			CPP_FUN.append(" void CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_MouseClick_" + Control_ID[Token[FUN_POS + 2]] + "(wxCommandEvent &event){event.Skip(); \n");
			// *** *** *** *** *** ***
		}
		else
		{
			ErrorCode("علة: نوع الحدث غير معروف ' " + Token[FUN_POS + 1] + " ' ", o_tokens);
		}

		IsInsideFunction = true;
		TheFunction = (Token[FUN_POS + 1] + " " + Token[FUN_POS + 2]);
		TheFunction_TYPE = "عادم";

		Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
		Win_CurrentTotalFucntion++;
		Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

		return; // continue;
	}
	else if (Token[FUN_POS + 1] == "عدد" || Token[FUN_POS + 1] == "نص" || Token[FUN_POS + 1] == "منطق") // دالة TYPE MyFunctionName (...)
	{
		// --------------------------
		// Type Function
		// --------------------------

		if (Token[FUN_POS + 2] == "")
			ErrorCode("يجب تحديد اسم الداله", o_tokens);
		
		if (Token[FUN_POS + 3] != "" && Token[FUN_POS + 3] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 3] + " ' ", o_tokens);
		
		if (Token[FUN_POS + 3] == "") // دالة TYPE MyFunctionName
		{
			if (Token[FUN_POS + 1] == "عدد")
			{
				if (!IsInsideWindow)
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Global function
						ADD_FUN(true, "", Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

						IsInsideFunction = true;			// Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Global INT Func(void)
					CPP_GLOBAL.append(" double FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(); \n");
					CPP_GLOBAL_FUN.append(" double FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(){ \n");
					// *** *** *** *** *** ***
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Local function
						ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

						IsInsideFunction = true; // Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Local INT Func(void)
					CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
					CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " double FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
					CPP_FUN.append(" double CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "(){ \n");
					// *** *** *** *** *** ***
				}

				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 2];
				TheFunction_TYPE = "عدد";

				Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
				Win_CurrentTotalFucntion++;
				Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

				return; // continue;
			}
			else if (Token[FUN_POS + 1] == "نص")
			{
				if (!IsInsideWindow)
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Global function
						ADD_FUN(true, "", Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);

						IsInsideFunction = true;			// Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Global String Func(void)
					CPP_GLOBAL.append(" wxString FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(); \n");
					CPP_GLOBAL_FUN.append(" wxString FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(){ \n");
					// *** *** *** *** *** ***
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Local function
						ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);

						IsInsideFunction = true;			// Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Local String Func(void)
					CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
					CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " wxString FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
					CPP_FUN.append(" wxString CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "(){ \n");
					// *** *** *** *** *** ***
				}

				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 2];
				TheFunction_TYPE = "نص";

				Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
				Win_CurrentTotalFucntion++;
				Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

				return; // continue;
			}
			else if (Token[FUN_POS + 1] == "منطق")
			{
				if (!IsInsideWindow)
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Global function
						ADD_FUN(true, "", Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

						IsInsideFunction = true;			// Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Global BOOL Func(void)
					CPP_GLOBAL.append(" bool FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(); \n");
					CPP_GLOBAL_FUN.append(" bool FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(){ \n");
					// *** *** *** *** *** ***
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Local function
						ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

						IsInsideFunction = true; // Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Local BOOL Func(void)
					CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
					CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " bool FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
					CPP_FUN.append(" bool CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "(){ \n");
					// *** *** *** *** *** ***
				}

				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 2];
				TheFunction_TYPE = "منطق";

				Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
				Win_CurrentTotalFucntion++;
				Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

				return; // continue;
			}
		}
		else if (Token[FUN_POS + 3] == "(") // دالة TYPE MyFunctionName ( ???
		{
			if (Token[FUN_POS + 4] == "")
				ErrorCode("سطر غير كامل المرجو اضافة ')' ", o_tokens);
			
			if (Token[FUN_POS + 4] == ")") // دالة TYPE MyFunctionName ()
			{
				if (Token[FUN_POS + 5] != "")
					ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 5] + " ' ", o_tokens);
				
				if (Token[FUN_POS + 1] == "عدد")
				{
					if (!IsInsideWindow)
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Global function
							ADD_FUN(true, "", Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Global INT Func(void)
						CPP_GLOBAL.append(" double FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(); \n");
						CPP_GLOBAL_FUN.append(" double FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(){ \n");
						// *** *** *** *** *** ***
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Local function
							ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Local INT Func(void)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " double FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
						CPP_FUN.append(" double CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "(){ \n");
						// *** *** *** *** *** ***
					}

					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "عدد";

					Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
					Win_CurrentTotalFucntion++;
					Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "نص")
				{
					if (!IsInsideWindow)
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Global function
							ADD_FUN(true, "", Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Global String Func(void)
						CPP_GLOBAL.append(" wxString FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(); \n");
						CPP_GLOBAL_FUN.append(" wxString FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(){ \n");
						// *** *** *** *** *** ***
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Local function
							ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Local String Func(void)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " wxString FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
						CPP_FUN.append(" wxString CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "(){ \n");
						// *** *** *** *** *** ***
					}

					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "نص";

					Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
					Win_CurrentTotalFucntion++;
					Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "منطق")
				{
					if (!IsInsideWindow)
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Global function
							ADD_FUN(true, "", Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Global bool Func(void)
						CPP_GLOBAL.append(" bool FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(); \n");
						CPP_GLOBAL_FUN.append(" bool FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "(){ \n");
						// *** *** *** *** *** ***
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Local function
							ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Local INT Func(void)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " bool FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
						CPP_FUN.append(" bool CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "(){ \n");
						// *** *** *** *** *** ***
					}

					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "منطق";

					Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
					Win_CurrentTotalFucntion++;
					Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

					return; // continue;
				}
			}
			else // دالة TYPE MyFunctionName (...)
			{
				if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
					ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);
				
				TempTokenCount = 0;
				for (int p = 5; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
				{
					if (Token[p] != "")
					{
						TempToken[TempTokenCount] = Token[p];
						TempTokenCount++;
					}
				}
				
				if (Token[FUN_POS + 1] == "عدد")
				{
					if (!IsInsideWindow)
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Global function
							ADD_FUN(true, "", Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// Function int Syntax (..., ...)
							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] ( ", o_tokens); // DEBUG

						// *** Generate Code ***
						// Global INT Func(...)
						CPP_GLOBAL.append(" double FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "( ");
						CPP_GLOBAL_FUN.append(" double FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "( ");
						// *** *** *** *** *** ***

						// Function int Syntax (..., ...)
						// To Get Log, and C++ Code
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Local function
							ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// Function int Syntax (..., ...)
							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] ( ", o_tokens); // DEBUG

						// *** Generate Code ***
						// Local INT Func(...)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " double FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ";
						CPP_FUN.append(" double CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ");
						// *** *** *** *** *** ***

						// Function int Syntax (..., ...)
						// To Get Log, and C++ Code
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,
															TempToken, 
															(TempTokenCount - 1), 
															TheWindow, 
															Token[FUN_POS + 2], 
															o_tokens);
					}
					
					if(DEBUG)DEBUG_MESSAGE(" ) \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					if (!IsInsideWindow)
					{
						// Global INT Func(...)
						CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
						CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
					}
					else
					{
						// Local INT Func(...)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + ScriptSyntaxBuffer + " ); \n";
						CPP_FUN.append(ScriptSyntaxBuffer + " ){ \n");
					}
					// *** *** *** *** *** ***

					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "عدد";

					Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
					Win_CurrentTotalFucntion++;
					Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "نص")
				{
					if (!IsInsideWindow)
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Global function
							ADD_FUN(true, "", Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);

							IsInsideFunction = true; // Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

							// Function string Syntax (..., ...)
							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);

							////SET_C_NAME(Token[FUN_POS + 2]);
							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Global String Func(...)
						////SET_C_NAME(Token[FUN_POS + 2]);
						CPP_GLOBAL.append(" wxString FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "( ");
						CPP_GLOBAL_FUN.append(" wxString FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "( ");
						// *** *** *** *** *** ***

						// Function string Syntax (..., ...)
						// To Get Log, and C++ Code
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Local function
							ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);
						
							IsInsideFunction = true; // Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

							// Function string Syntax (..., ...)
							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);

							////SET_C_NAME(Token[FUN_POS + 2]);
							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Local String Func(...)
						////SET_C_NAME(Token[FUN_POS + 2]);
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " wxString FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ";
						CPP_FUN.append(" wxString CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ");
						// *** *** *** *** *** ***

						// Function string Syntax (..., ...)
						// To Get Log, and C++ Code
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);
					}

					if(DEBUG)DEBUG_MESSAGE(" ) \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					if (!IsInsideWindow)
					{
						// Global String Func(...)
						CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
						CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
					}
					else
					{
						// Local String Func(...)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + ScriptSyntaxBuffer + " ); \n";
						CPP_FUN.append(ScriptSyntaxBuffer + " ){ \n");
						// *** *** *** *** *** ***
					}

					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "نص";

					Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
					Win_CurrentTotalFucntion++;
					Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "منطق")
				{
					if (!IsInsideWindow)
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Global function
							ADD_FUN(true, "", Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// Function int Syntax (..., ...)
							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] ( ", o_tokens); // DEBUG

						// *** Generate Code ***
						// Global BOOL Func(...)
						CPP_GLOBAL.append(" bool FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "( ");
						CPP_GLOBAL_FUN.append(" bool FUNCTION_" + Global_ID[Token[FUN_POS + 2]] + "( ");
						// *** *** *** *** *** ***

						// Function int Syntax (..., ...)
						// To Get Log, and C++ Code
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							// Local function
							ADD_FUN(false, TheWindow, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

							IsInsideFunction = true;			// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// Function int Syntax (..., ...)
							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] ( ", o_tokens); // DEBUG

						// *** Generate Code ***
						// Local BOOL Func(...)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " bool FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ";
						CPP_FUN.append(" bool CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ");
						// *** *** *** *** *** ***

						// Function int Syntax (..., ...)
						// To Get Log, and C++ Code
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 2], o_tokens);
					}
					
					if(DEBUG)DEBUG_MESSAGE(" ) \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					if (!IsInsideWindow)
					{
						// Global INT Func(...)
						CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
						CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
					}
					else
					{
						// Local INT Func(...)
						CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
						CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + ScriptSyntaxBuffer + " ); \n";
						CPP_FUN.append(ScriptSyntaxBuffer + " ){ \n");
					}
					// *** *** *** *** *** ***

					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "منطق";

					Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
					Win_CurrentTotalFucntion++;
					Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

					return; // continue;
				}
			}
		}
	}
	else // void : دالة MyFunctionName (???) // void
	{
		if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);
		
		if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);
		
		if (Token[FUN_POS + 2] == "") // دالة MyFunctionName
		{
			if (!IsInsideWindow)
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					// Global function
					ADD_FUN(true, "", Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
				
					IsInsideFunction = true; // Need by Tokens Predefined
					TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

					////SET_C_NAME(Token[FUN_POS + 2]);
					return; // continue;
				}
				
				if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [" + Token[FUN_POS + 1] + "] \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Global VOID Func(void)
				////SET_C_NAME(Token[FUN_POS + 2]);
				CPP_GLOBAL.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] + "(); \n");
				CPP_GLOBAL_FUN.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] + "(){ \n");
				// *** *** *** *** *** ***
			}
			else
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					// Local function
					ADD_FUN(false, TheWindow, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
					
					IsInsideFunction = true; // Need by Tokens Predefined
					TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

					////SET_C_NAME(Token[FUN_POS + 2]);
					return; // continue;
				}

				if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [" + Token[FUN_POS + 1] + "] \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Local VOID Func(void)
				////SET_C_NAME(Token[FUN_POS + 2]);
				CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
				CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " void FUNCTION_" + ID[Token[FUN_POS + 1]] + "(); \n";
				CPP_FUN.append(" void CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 1]] + "(){ \n");
				// *** *** *** *** *** ***
			}

			IsInsideFunction = true;
			TheFunction = Token[FUN_POS + 1];
			TheFunction_TYPE = "عادم";
			RETURN_FUN[std::make_pair(TheWindow, TheFunction)] = "OK";

			Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
			Win_CurrentTotalFucntion++;
			Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

			return; // continue;
		}
		else if (Token[FUN_POS + 2] == "(") // دالة MyFunctionName ( ???
		{
			if (Token[FUN_POS + 3] == ")") // دالة MyFunctionName ()
			{
				if (Token[FUN_POS + 4] != "")
					ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 4] + " ' ", o_tokens);
				
				if (!IsInsideWindow)
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Global function
						ADD_FUN(true, "", Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
					
						IsInsideFunction = true; // Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

						////SET_C_NAME(Token[FUN_POS + 2]);
						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [" + Token[FUN_POS + 1] + "] () \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Global VOID Func(void)
					////SET_C_NAME(Token[FUN_POS + 2]);
					CPP_GLOBAL.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] + "(); \n");
					CPP_GLOBAL_FUN.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] + "(){ \n");
					// *** *** *** *** *** ***
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Local function
						ADD_FUN(false, TheWindow, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
					
						IsInsideFunction = true; // Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

						////SET_C_NAME(Token[FUN_POS + 2]);
						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [" + Token[FUN_POS + 1] + "] () \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Local VOID Func(void)
					////SET_C_NAME(Token[FUN_POS + 2]);
					CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
					CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " void FUNCTION_" + ID[Token[FUN_POS + 1]] + "(); \n";
					CPP_FUN.append(" void CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 1]] + "(){ \n");
					// *** *** *** *** *** ***
				}

				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 1];
				TheFunction_TYPE = "عادم";
				RETURN_FUN[std::make_pair(TheWindow, TheFunction)] = "OK";

				Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
				Win_CurrentTotalFucntion++;
				Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

				return; // continue;
			}
			else // دالة MyFunctionName (...)
			{
				if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
					ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);
				
				TempTokenCount = 0;
				for (int p = 4; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
				{
					if (Token[p] != "")
					{
						TempToken[TempTokenCount] = Token[p];
						TempTokenCount++;
					}
				}
				
				if (!IsInsideWindow)
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Global function
						ADD_FUN(true, "", Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
					
						IsInsideFunction = true; // Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

						// Function string Syntax (..., ...)
						// To Set ARG counter, and ARG as Local Var
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 1], o_tokens);

						////SET_C_NAME(Token[FUN_POS + 2]);
						return; // continue;
					}
					
					if(DEBUG)DEBUG_MESSAGE("[GLOBAL-FUNCTION] [" + Token[FUN_POS + 1] + "] ( ", o_tokens); // DEBUG

					// *** Generate Code ***
					// Global VOID Func(...)
					////SET_C_NAME(Token[FUN_POS + 2]);
					CPP_GLOBAL.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] + "( ");
					CPP_GLOBAL_FUN.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] + "( ");
					// *** *** *** *** *** ***

					// Function string Syntax (..., ...)
					// To Get Log, and C++ Code
					ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 1], o_tokens);
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						// Local function
						ADD_FUN(false, TheWindow, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
					
						IsInsideFunction = true; // Need by Tokens Predefined
						TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

						// Function string Syntax (..., ...)
						// To Set ARG counter, and ARG as Local Var
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 1], o_tokens);

						////SET_C_NAME(Token[FUN_POS + 2]);
						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("	[FUNCTION] [" + Token[FUN_POS + 1] + "] ( ", o_tokens); // DEBUG

					// *** Generate Code ***
					// Local VOID Func(...)
					////SET_C_NAME(Token[FUN_POS + 2]);
					CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
					CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " void FUNCTION_" + ID[Token[FUN_POS + 1]] + "( ";
					CPP_FUN.append(" void CLASS_WINDOW_" + ID[TheWindow] + "::FUNCTION_" + ID[Token[FUN_POS + 1]] + "( ");
					// *** *** *** *** *** ***

					// Function string Syntax (..., ...)
					// To Get Log, and C++ Code
					ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheWindow, Token[FUN_POS + 1], o_tokens);
				}
				
				if(DEBUG)DEBUG_MESSAGE(" ) \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				if (!IsInsideWindow)
				{
					// Global VOID Func(...)
					CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
					CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
				}
				else
				{
					// Local VOID Func(...)
					CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
					CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + ScriptSyntaxBuffer + " ); \n";
					CPP_FUN.append(ScriptSyntaxBuffer + " ){ \n");
					// *** *** *** *** *** ***
				}

				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 1];
				TheFunction_TYPE = "عادم";
				RETURN_FUN[std::make_pair(TheWindow, TheFunction)] = "OK";

				Win_FunctionNames[std::make_pair(TheWindow, Win_CurrentTotalFucntion)] = TheFunction;
				Win_CurrentTotalFucntion++;
				Win_TotalFucntion[TheWindow] = Win_CurrentTotalFucntion;

				return; // continue;
			}
		}
	}
}
