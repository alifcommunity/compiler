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

void parser_FunctionClass(string Token[2048], CLASS_TOKEN *o_tokens){
	// Class -> دالة

	if (IsInsideFunction)
		ErrorCode("لا يمكن انشاء دالة داخل دالة، الدالة الحالية : " + TheFunction, o_tokens);
	
	int FUN_POS = 1;
	
	if (Token[1] == "خاص")
		FUN_POS = 2;
	
	if (Token[FUN_POS + 1] == "")
		ErrorCode("يجب اعطاء اسم لهته الدالة الجديدة", o_tokens);
	
	if (Token[FUN_POS + 1] == "رئيسية")
		ErrorCode("لايمكن إنشاء دالة رئيسية داخل صنف، هل تقصد ' بناء ' ؟", o_tokens);
	
	if (Token[FUN_POS + 1] == "نقر")
		ErrorCode("لايمكن إنشاء دالة نقر داخل صنف", o_tokens);
	
	if (Token[FUN_POS + 1] == "بناء") // Class -> Constrictor
	{
		if (Token[1] == "خاص")
			ErrorCode(" ' دالة بناء الصنف لا يمكن ان تكون من نوع ' خاص", o_tokens);
		
		if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);
		
		if (Token[FUN_POS + 2] == "")
		{
			// Class -> Constrictor.

			if (!o_tokens->TOKENS_PREDEFINED)
			{
				if (CLASS_CONSTRICTOR_FUN_IS_SET[TheClass])
					ErrorCode("دالة بناء الصنف تم انشاؤها مسبقا في السطر : " + CLASS_CONSTRICTOR_FUN_AT_LINE[TheClass], o_tokens);

				CLASS_CONSTRICTOR_FUN_IS_SET[TheClass] = true;
				CLASS_CONSTRICTOR_FUN_AT_LINE[TheClass] = IntToString(o_tokens->Line);

				IsInsideFunction = true; // Need by Tokens Predefined
				TheFunction = "بناء"; // Need by Tokens Predefined

				return; // continue;
			}

			IsInsideFunction = true;
			TheFunction = "بناء";

			TheFunction_TYPE = "عادم";
			RETURN_FUN[std::make_pair(TheClass, "بناء")] = "OK";

			if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION-CONSTRICTOR] [" + Token[FUN_POS + 1] + "] \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			// Class -> CONSTRICTOR-Func.
			CPP_CLASS.append(" public: CLASS_" + Global_ID[TheClass] + "() { \n");
			// *** *** *** *** *** ***
			
			return; // continue;
		}
		else if (Token[FUN_POS + 2] == "(") // دالة بناء Class ( ???
		{
			if (Token[FUN_POS + 3] == ")") // دالة بناء Class ().
			{
				if (Token[FUN_POS + 4] != "")
					ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 4] + " ' ", o_tokens);

				if (!o_tokens->TOKENS_PREDEFINED)
				{
					if (CLASS_CONSTRICTOR_FUN_IS_SET[TheClass])
						ErrorCode("دالة بناء الصنف تم انشاؤها مسبقا في السطر : " + CLASS_CONSTRICTOR_FUN_AT_LINE[TheClass], o_tokens);
					
					CLASS_CONSTRICTOR_FUN_IS_SET[TheClass] = true;
					MAIN_FUN_AT_LINE[TheClass] = IntToString(o_tokens->Line);

					IsInsideFunction = true; // Need by Tokens Predefined
					TheFunction = "بناء"; // Need by Tokens Predefined

					return; // continue;
				}

				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 1];

				TheFunction_TYPE = "عادم";
				RETURN_FUN[std::make_pair(TheClass, "بناء")] = "OK";

				if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION-CONSTRICTOR] [" + Token[FUN_POS + 1] + "] () \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class -> CONSTRICTOR-Func ()
				CPP_CLASS.append(" public: CLASS_" + Global_ID[TheClass] + " () { \n");
				// *** *** *** *** *** ***

				return; // continue;
			}
			else // دالة بناء Class (...)
			{
				if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
					ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);
				
				TempTokenCount = 0;
				for (int p = (FUN_POS + 3); p <= o_tokens->TOTAL[o_tokens->Line]; p++)
				{
					if (Token[p] != "")
					{
						TempToken[TempTokenCount] = Token[p];
						TempTokenCount++;
					}
				}

				if (!o_tokens->TOKENS_PREDEFINED)
				{
					if (CLASS_CONSTRICTOR_FUN_IS_SET[TheClass])
						ErrorCode("دالة بناء الصنف تم انشاؤها مسبقا في السطر : " + CLASS_CONSTRICTOR_FUN_AT_LINE[TheClass], o_tokens);
					
					CLASS_CONSTRICTOR_FUN_IS_SET[TheClass] = true;
					MAIN_FUN_AT_LINE[TheClass] = IntToString(o_tokens->Line);

					IsInsideFunction = true; // Need by Tokens Predefined
					TheFunction = "بناء"; // Need by Tokens Predefined

					// To Set ARG counter, and ARG as Local Var
					ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 1], o_tokens);
				
					return; // continue;
				}
				
				if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION-CONSTRICTOR] [" + Token[FUN_POS + 1] + "] (", o_tokens); // DEBUG
				
				// *** Generate Code ***
				// Class -> CONSTRICTOR-Func (...
				CPP_CLASS.append(" public: CLASS_" + Global_ID[TheClass] + " ( ");
				// *** *** *** *** *** ***

				// To Get Log, and C++ Code
				ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 1], o_tokens);
				
				if(DEBUG)DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class -> CONSTRICTOR-Func ...)
				CPP_CLASS.append(ScriptSyntaxBuffer + " ) { \n");
				// *** *** *** *** *** ***
				
				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 1];

				TheFunction_TYPE = "عادم";
				RETURN_FUN[std::make_pair(TheClass, "بناء")] = "OK";

				return; // continue;
			}
		}
	}
	else if (Token[FUN_POS + 1] == "عدد" || Token[FUN_POS + 1] == "نص" || Token[FUN_POS + 1] == "منطق") // Class دالة TYPE MyFunctionName (...)
	{
		if (Token[FUN_POS + 2] == "")
			ErrorCode("يجب اعطاء اسم لهته الدالة الجديدة", o_tokens);
		
		if (Token[FUN_POS + 3] != "" && Token[FUN_POS + 3] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 3] + " ' ", o_tokens);
		
		if (Token[FUN_POS + 3] == "") // Class دالة TYPE MyFunctionName
		{
			if (Token[FUN_POS + 1] == "عدد")
			{
				if (Token[1] == "خاص")
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Class -> Private INT-Func
						CPP_CLASS.append(" private: double ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + "() { \n");
						// *** *** *** *** *** ***
					}
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> INT-Func
						CPP_CLASS.append(" public: double ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + "() { \n");
						// *** *** *** *** *** ***
					}
				}
				
				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 2];
				TheFunction_TYPE = "عدد";
				return; // continue;
			}
			else if (Token[FUN_POS + 1] == "نص")
			{
				if (Token[1] == "خاص")
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Class -> Private STRING-Func
						CPP_CLASS.append(" private: wxString ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + "() { \n");
						// *** *** *** *** *** ***
					}
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Class -> STRING-Func
						CPP_CLASS.append(" public: wxString ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + "() { \n");
						// *** *** *** *** *** ***
					}
				}
				
				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 2];
				TheFunction_TYPE = "نص";
				return; // continue;
			}
			else if (Token[FUN_POS + 1] == "منطق")
			{
				if (Token[1] == "خاص")
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Class -> Private BOOL-Func
						CPP_CLASS.append(" private: bool ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + "() { \n");
						// *** *** *** *** *** ***
					}
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] \n\n", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> BOOL-Func
						CPP_CLASS.append(" public: bool ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + "() { \n");
						// *** *** *** *** *** ***
					}
				}
				
				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 2];
				TheFunction_TYPE = "منطق";
				return; // continue;
			}
		}
		else if (Token[FUN_POS + 3] == "(") // Class دالة TYPE MyFunctionName ( ???
		{
			if (Token[FUN_POS + 4] == "")
				ErrorCode("سطر غير كامل المرجو اضافة ')' ", o_tokens);
			
			if (Token[FUN_POS + 4] == ")") // Class دالة TYPE MyFunctionName ()
			{
				if (Token[FUN_POS + 5] != "")
					ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 5] + " ' ", o_tokens);
				
				if (Token[FUN_POS + 1] == "عدد")
				{
					if (Token[1] == "خاص")
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

							// *** Generate Code ***
							// Class -> Private INT-Func ()
							CPP_CLASS.append(" private: double ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " () { \n");
							// *** *** *** *** *** ***
						}
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG
							
							// *** Generate Code ***
							// Class -> INT-Func ()
							CPP_CLASS.append(" public: double ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " () { \n");
							// *** *** *** *** *** ***
						}
					}
					
					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "عدد";
					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "نص")
				{
					if (Token[1] == "خاص")
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

							// *** Generate Code ***
							// Class -> Private STRING-Func ()
							CPP_CLASS.append(" private: wxString ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " () { \n");
							// *** *** *** *** *** ***
						}
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

							// *** Generate Code ***
							// Class -> STRING-Func ()
							CPP_CLASS.append(" public: wxString ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " () { \n");
							// *** *** *** *** *** ***
						}
					}
					
					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "نص";
					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "منطق")
				{
					if (Token[1] == "خاص")
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG

							// *** Generate Code ***
							// Class -> Private BOOL-Func ()
							CPP_CLASS.append(" private: bool ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " () { \n");
							// *** *** *** *** *** ***
						}
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] () \n\n", o_tokens); // DEBUG
							
							// *** Generate Code ***
							// Class -> BOOL-Func ()
							CPP_CLASS.append(" public: bool ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " () { \n");
							// *** *** *** *** *** ***
						}
					}
					
					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "منطق";
					return; // continue;
				}
			}
			else // Class دالة TYPE MyFunctionName (...)
			{
				if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
					ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);
				
				TempTokenCount = 0;
				for (int p = (FUN_POS + 4); p <= o_tokens->TOTAL[o_tokens->Line]; p++)
				{
					if (Token[p] != "")
					{
						TempToken[TempTokenCount] = Token[p];
						TempTokenCount++;
					}
				}
				
				if (Token[FUN_POS + 1] == "عدد")
				{
					if (Token[1] == "خاص")
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

							IsInsideFunction = true;	// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> Private INT-Func (...
						CPP_CLASS.append(" private: double ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " ( ");
						// *** *** *** *** *** ***
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "عدد", o_tokens->Line, o_tokens);

							IsInsideFunction = true;	// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> Public INT-Func (...
						CPP_CLASS.append(" public: double ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " ( ");
						// *** *** *** *** *** ***
					}

					ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);
					
					if(DEBUG)DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Class -> Pub/Priv INT-Func ...)
					CPP_CLASS.append(ScriptSyntaxBuffer + " ) { \n");
					// *** *** *** *** *** ***
					
					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "عدد";
					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "نص")
				{
					if (Token[1] == "خاص")
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);

							IsInsideFunction = true;	// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> Private STRING-Func (...
						CPP_CLASS.append(" private: wxString ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " ( ");
						// *** *** *** *** *** ***
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "نص", o_tokens->Line, o_tokens);

							IsInsideFunction = true;	// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_STRING] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> Public STRING-Func (...
						CPP_CLASS.append(" public: wxString ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " ( ");
						// *** *** *** *** *** ***
					}

					ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);
					
					if(DEBUG)DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Class -> Pub/Priv INT-Func ...)
					CPP_CLASS.append(ScriptSyntaxBuffer + " ) { \n");
					// *** *** *** *** *** ***
					
					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "نص";
					return; // continue;
				}
				else if (Token[FUN_POS + 1] == "منطق")
				{
					if (Token[1] == "خاص")
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

							IsInsideFunction = true;	// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> Private BOOL-Func (...
						CPP_CLASS.append(" private: bool ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " ( ");
						// *** *** *** *** *** ***
					}
					else
					{
						if (!o_tokens->TOKENS_PREDEFINED)
						{
							ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 2], "منطق", o_tokens->Line, o_tokens);

							IsInsideFunction = true;	// Need by Tokens Predefined
							TheFunction = Token[FUN_POS + 2];	// Need by Tokens Predefined

							// To Set ARG counter, and ARG as Local Var
							ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);

							return; // continue;
						}

						if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] + "] (", o_tokens); // DEBUG
						
						// *** Generate Code ***
						// Class -> Public BOOL-Func (...
						CPP_CLASS.append(" public: bool ClassFUNCTION_" + ID[Token[FUN_POS + 2]] + " ( ");
						// *** *** *** *** *** ***
					}

					ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false,TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 2], o_tokens);
					
					if(DEBUG)DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Class -> Pub/Priv BOOL-Func ...)
					CPP_CLASS.append(ScriptSyntaxBuffer + " ) { \n");
					// *** *** *** *** *** ***
					
					IsInsideFunction = true;
					TheFunction = Token[FUN_POS + 2];
					TheFunction_TYPE = "منطق";
					return; // continue;
				}
			}
		}
	}
	else // void : Class دالة MyFunctionName (???) // void
	{
		if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);
		
		if (Token[FUN_POS + 2] == "") // دالة MyFunctionName
		{
			if (Token[1] == "خاص")
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
				}
				else
				{
					if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [" + Token[FUN_POS + 1] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Class -> Private Func
					CPP_CLASS.append(" private: void ClassFUNCTION_" + ID[Token[FUN_POS + 1]] + "() { \n");
					// *** *** *** *** *** ***
				}
			}
			else
			{
				if (!o_tokens->TOKENS_PREDEFINED)
				{
					ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
				}
				else
				{
					if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [" + Token[FUN_POS + 1] + "] \n\n", o_tokens); // DEBUG

					// *** Generate Code ***
					// Class -> Func
					CPP_CLASS.append(" public: void ClassFUNCTION_" + ID[Token[FUN_POS + 1]] + "() { \n");
					// *** *** *** *** *** ***
				}
			}
			
			IsInsideFunction = true;
			TheFunction = Token[FUN_POS + 1];
			TheFunction_TYPE = "عادم";
			return; // continue;
		}
		else if (Token[FUN_POS + 2] == "(") // Class دالة MyFunctionName ( ???
		{
			if (Token[FUN_POS + 3] == ")") // Class دالة MyFunctionName ()
			{
				if (Token[FUN_POS + 4] != "")
					ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 4] + " ' ", o_tokens);
				
				if (Token[1] == "خاص")
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [" + Token[FUN_POS + 1] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Class -> Private Func ()
						CPP_CLASS.append(" private: void ClassFUNCTION_" + ID[Token[FUN_POS + 1]] + " () { \n");
						// *** *** *** *** *** ***
					}
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
					}
					else
					{
						if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [" + Token[FUN_POS + 1] + "] () \n\n", o_tokens); // DEBUG

						// *** Generate Code ***
						// Class -> Func ()
						CPP_CLASS.append(" public: void ClassFUNCTION_" + ID[Token[FUN_POS + 1]] + " () { \n");
						// *** *** *** *** *** ***
					}
				}
				
				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 1];
				TheFunction_TYPE = "عادم";
				return; // continue;
			}
			else // Class دالة MyFunctionName (...)
			{
				if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
					ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);
				
				TempTokenCount = 0;
				for (int p = (FUN_POS + 3); p <= o_tokens->TOTAL[o_tokens->Line]; p++)
				{
					if (Token[p] != "")
					{
						TempToken[TempTokenCount] = Token[p];
						TempTokenCount++;
					}
				}
				
				if (Token[1] == "خاص")
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(true, TheClass, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);

						IsInsideFunction = true;
						TheFunction = Token[FUN_POS + 1];

						// To Set ARG counter, and ARG as Local Var
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 1], o_tokens);

						return; // continue;
					}
					
					if(DEBUG)DEBUG_MESSAGE("	[PRIVATE] [CLASS-FUNCTION] [" + Token[FUN_POS + 1] + "] ( ", o_tokens); // DEBUG

					// *** Generate Code ***
					// Class -> Private Func (...
					CPP_CLASS.append(" private: void ClassFUNCTION_" + ID[Token[FUN_POS + 1]] + " ( ");
					// *** *** *** *** *** ***
				
				}
				else
				{
					if (!o_tokens->TOKENS_PREDEFINED)
					{
						ADD_FUN_CLASS(false, TheClass, Token[FUN_POS + 1], "عادم", o_tokens->Line, o_tokens);
						
						IsInsideFunction = true;
						TheFunction = Token[FUN_POS + 1];

						// To Set ARG counter, and ARG as Local Var
						ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 1], o_tokens);

						return; // continue;
					}

					if(DEBUG)DEBUG_MESSAGE("	[CLASS-FUNCTION] [" + Token[FUN_POS + 1] + "] ( ", o_tokens); // DEBUG
				
					// *** Generate Code ***
					// Class -> Public Func (...
					CPP_CLASS.append(" public: void ClassFUNCTION_" + ID[Token[FUN_POS + 1]] + " ( ");
					// *** *** *** *** *** ***

				}

				ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1), TheClass, Token[FUN_POS + 1], o_tokens);
				
				if(DEBUG)DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				// Class -> Pub/Priv Func ...)
				CPP_CLASS.append(ScriptSyntaxBuffer + " ) { \n");
				// *** *** *** *** *** ***
				
				IsInsideFunction = true;
				TheFunction = Token[FUN_POS + 1];
				TheFunction_TYPE = "عادم";
				return; // continue;
			}
		}
	}
}
