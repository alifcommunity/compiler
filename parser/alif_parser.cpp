// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

// #include "NewVar.cpp" // New Var
#include "alif_parser_window.cpp"
#include "alif_parser_control.cpp"
#include "alif_parser_int.cpp"
#include "alif_parser_string.cpp"
#include "alif_parser_bool.cpp"
#include "alif_parser_class.cpp"
#include "alif_parser_function_class.cpp"
#include "alif_parser_function.cpp"
#include "alif_parser_if.cpp"
#include "alif_parser_if_else.cpp"
#include "alif_parser_loop.cpp"
#include "alif_parser_next_break.cpp"
#include "alif_parser_return.cpp"
#include "alif_parser_obj_new.cpp"
#include "alif_parser_obj.cpp"
#include "alif_parser_var.cpp"
#include "alif_parser_var_g_class.cpp"
#include "alif_parser_var_g.cpp"
#include "alif_parser_function_call.cpp"
#include "alif_parser_operator.cpp"
#include "alif_parser_end.cpp"

void ALIF_PARSER (CLASS_TOKEN *o_tokens)
{
	// ------------------------------------------------------
	// Already Parsed files
	// -----------------------------------------------------

	if (!o_tokens->TOKENS_PREDEFINED)
	{
		// Check for tokens not predifined

		if (ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED[o_tokens->PATH_FULL_SOURCE])
		{
			if(DEBUG)DEBUG_MESSAGE("\n Already Parsed(Not-Predifined) [" + o_tokens->PATH_FULL_SOURCE + "] \n", o_tokens);
			return;
		}
		else
			ALREADY_PARSED_FILE_TOKENS_NOT_PREDEFINED[o_tokens->PATH_FULL_SOURCE] = true;
	}
	else
	{
		// Check for tokens already predifined

		if (ALREADY_PARSED_FILE_TOKENS_PREDEFINED[o_tokens->PATH_FULL_SOURCE])
		{
			if(DEBUG)DEBUG_MESSAGE("\n Already Parsed(Predifined) [" + o_tokens->PATH_FULL_SOURCE + "] \n", o_tokens);
			return;
		}
		else
			ALREADY_PARSED_FILE_TOKENS_PREDEFINED[o_tokens->PATH_FULL_SOURCE] = true;
	}

	// ------------------------------------------------------

	// For every line
	for(o_tokens->Line = 1; o_tokens->Line <= o_tokens->TOTAL_LINES; o_tokens->Line++)
	{
		if (o_tokens->TOTAL[o_tokens->Line] < 1){continue;}
		//printf("\n");
		
		// 'unsigned' alway comparison false if '-1', 
		// so Crash in convert loop fucntion like "CONVERT_STRING_ARRAY_TO_STRING()"
		// Move from 'unsigned' to 'int' !
		
		string Token[2048];

		// Clear TempToken[1024] on every line
		// this is for fixing TempToken[p + 1] --> last token from last line!
		for(int clear_p = 0; clear_p <= 1023; clear_p++){
			TempToken[clear_p] = "";
		}
		
		for(o_tokens->NUMBER = 1; 
			o_tokens->NUMBER <= o_tokens->TOTAL[o_tokens->Line]; 
			o_tokens->NUMBER++ )
		{
			// -----------------------------------------------------------------
			// Line Number 		: o_tokens->Line
			// Token Number 	: o_tokens->NUMBER
			// Token 			: o_tokens->TOKEN[std::make_pair(o_tokens->Line, o_tokens->NUMBER)]
			// -----------------------------------------------------------------
			if (o_tokens->TOKEN[std::make_pair(o_tokens->Line, o_tokens->NUMBER)] != "")
			{
				Token[o_tokens->NUMBER] = o_tokens->TOKEN[std::make_pair(o_tokens->Line, o_tokens->NUMBER)];
				//printf("%s | %d -> %d -> %s", &o_tokens->PATH_FULL_SOURCE, o_tokens->Line, o_tokens->NUMBER, Token[o_tokens->NUMBER]);

				/*
				if (Token[o_tokens->NUMBER] == "ادا" || 
					Token[o_tokens->NUMBER] == "إدا" || 
					Token[o_tokens->NUMBER] == "أدا" ||
					Token[o_tokens->NUMBER] == "اذا" ||
					Token[o_tokens->NUMBER] == "أذا")
						Token[o_tokens->NUMBER] = "إذا";

				else if (Token[o_tokens->NUMBER] == "او" || 
						 Token[o_tokens->NUMBER] == "ٱو" || 
						 Token[o_tokens->NUMBER] == "آو" ||
						 Token[o_tokens->NUMBER] == "والا" || 
						 Token[o_tokens->NUMBER] == "وإلا" || 
						 Token[o_tokens->NUMBER] == "وألا")
						 	Token[o_tokens->NUMBER] = "أو";
				*/
			}
		} // End Tokens Loop Log way..
		//for (int i = 1; i <= o_tokens->NUMBER; i++){
		if (Token[1] == "") continue;
// #######################################################

		if(DEBUG)DEBUG_MESSAGE(IntToString(o_tokens->Line) + ": ", o_tokens); // DEBUG

if (Token[1] == "#") // #
{
	if (Token[2] == "")
		ErrorCode("هاش غير محدد ' # '", o_tokens);
	
	if (IsInsideWindow || IsInsideFunction)
		ErrorCode("يجب استعمال هاش ' # ' في مكان عام", o_tokens);
	
	// --------------------------------------------
	
	if (Token[2] == "واجهة_ويب"){

		// Create new Window
		// Set Web control
		// #واجهة_ويب رئيسية "UI_WEB_1"

		if (Token[3] == "")
		ErrorCode("يجب تحديد اسم النافذة", o_tokens);
	
		if (!IsValidName(Token[3], o_tokens))
			ErrorCode("اسم غير مقبول : ' " + Token[3] + " ' ", o_tokens);

		if (Token[4] == "")
			ErrorCode("يجب تحديد اسم الملف", o_tokens);

		if(!IsValidStringFormat(Token[4], o_tokens))
			ErrorCode("خطأ في كتابة إسم الملف: "+ Token[4], o_tokens);

		// Get PATH_FULL_WINDOW_WEB
		AlifLexerParser(GET_TXT_FROM_STRING(Token[4], o_tokens), "ALIFUIW", false, o_tokens->TOKENS_PREDEFINED);

		parser_NewWindowWeb(Token, o_tokens);
	}
	else if (Token[2] == "أضف"		||	// #اضف "test.ALIF"
			 Token[2] == "واجهة"	||	// #واجهة "test.ALIFUI"
			 Token[2] == "مكتبة" )		// #مكتبة "test.ALIFLIB"
	{
		if (Token[3] == "")
			ErrorCode("مسار الملف غير محدد" + Token[2], o_tokens);
		
		if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("مسار الملف غير صائب : ' " + Token[3] + " ' ", o_tokens);
		
		if (Token[4] != "")
			ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);
		
		if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
			ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى", o_tokens);
		
		if (Token[2] == "أضف")
		{
			// Include Local File for aditional user code.

			//if (Token[4] != "")
			//	ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);

			if(!IsValidStringFormat(Token[3], o_tokens))
				ErrorCode("خطأ في كتابة إسم الملف: "+ Token[3], o_tokens);
			
			if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF] ... \n\n", o_tokens);
			AlifLexerParser(GET_TXT_FROM_STRING(Token[3], o_tokens), "ALIF", false, o_tokens->TOKENS_PREDEFINED);
		}
		else if (Token[2] == "واجهة")
		{
			// Include local file, for user UI code.

			//if (Token[4] != "")
			//	ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);

			if(!IsValidStringFormat(Token[3], o_tokens))
				ErrorCode("خطأ في كتابة إسم الملف: "+ Token[3], o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF UI] ... \n\n", o_tokens);
			AlifLexerParser(GET_TXT_FROM_STRING(Token[3], o_tokens), "ALIFUI", false, o_tokens->TOKENS_PREDEFINED);
		}
		else if (Token[2] == "مكتبة")
		{
			// Include Library from Lib folder, or local folder.

			if (GET_TXT_FROM_STRING(Token[3], o_tokens) == "رسالة" || 
				GET_TXT_FROM_STRING(Token[3], o_tokens) == "الرسالة")
				ErrorCode("ثم دمج مكتبة ' رسالة ' مع مكتبات ألف القياسية الرئيسية، لدى يقوم المترجم باستعمالها بشكل آلي، المرجو إزالة هذا السطر ", o_tokens);
			
			// Python lib need 3 other var to setup
			/* if(GET_TXT_FROM_STRING(Token[3], o_tokens) == "البايثون"){

				//if (Token[7] != "")
				//	ErrorCode("أمر غير معروف : ' " + Token[7] + " ' ", o_tokens);

				// #مكتبة "البايثون" "/usr/include/python3.5"4 "/usr/lib/python3.5/config-3.5m-x86_64-linux-gnu"6 "python3.5"6 ...7

				if (Token[4] == "")
					ErrorCode("يجب تحديد مسار عناوين البايثون", o_tokens);
				if(!IsValidStringFormat(Token[4], o_tokens))
					ErrorCode("خطأ في كتابة مسار عناوين البايثون: "+ Token[4], o_tokens);

				if (Token[5] == "")
					ErrorCode("يجب تحديد مسار مكتبات البايثون", o_tokens);
				if(!IsValidStringFormat(Token[5], o_tokens))
					ErrorCode("خطأ في كتابة مسار مكتبات البايثون: "+ Token[5], o_tokens);

				if (Token[6] == "")
					ErrorCode("يجب تحديد إسم مكتبة البايثون", o_tokens);
				if(!IsValidStringFormat(Token[6], o_tokens))
					ErrorCode("خطأ في كتابة إسم مكتبة البايثون: "+ Token[6], o_tokens);
				
				// Setup Python lib env.
				PythonSetEnvirenment(	GET_TXT_FROM_STRING(Token[4], o_tokens),	// /usr/include/python3.5
										GET_TXT_FROM_STRING(Token[5], o_tokens),	// /usr/lib/python3.5/config-3.5m-x86_64-linux-gnu
										GET_TXT_FROM_STRING(Token[6], o_tokens));	// python3.5
			} */

			else {

				if(GET_TXT_FROM_STRING(Token[3], o_tokens) == "البايثون")
					PythonSetEnvirenment();
				
				// Other librarys check..
				// Example: #مكتبة "الوقت"

				if (Token[4] != "")
					ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);
			}
			
			if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF LIB] ... \n\n", o_tokens);
			AlifLexerParser(GET_TXT_FROM_STRING(Token[3], o_tokens), "ALIFLIB", false, o_tokens->TOKENS_PREDEFINED);
		}
		else if (Token[2] == "أظف_أمر_ترجمة" || Token[2] == "أظف_أمر_ربط"){

			// #أظف_أمر_ترجمة "..."
			// #أظف_أمر_ربط "..."

			if(!IsValidStringFormat(Token[3], o_tokens))
				ErrorCode("خطأ في كتابة إسم الأمر: "+ Token[3], o_tokens);
			
			if (Token[2] == "أظف_أمر_ترجمة"){

				CompileAddExtra_Compile(GET_TXT_FROM_STRING(Token[3], o_tokens));

			} else {

				CompileAddExtra_Link(GET_TXT_FROM_STRING(Token[3], o_tokens));
			}
		}
		
		continue;
	}

	// --------------------------------------------
	// البايثون
	// --------------------------------------------

	else if(Token[2] == "البايثون_مسار_عناوين"){

		PythonInclude_path = GET_TXT_FROM_STRING(Token[3], o_tokens);
		if(DEBUG)DEBUG_MESSAGE("[Setting Python Include '" + GET_TXT_FROM_STRING(Token[3], o_tokens) + "' ] \n\n", o_tokens);
	}
	else if(Token[2] == "البايثون_مسار_مكتبات"){

		PythonLib_path = GET_TXT_FROM_STRING(Token[3], o_tokens);
		if(DEBUG)DEBUG_MESSAGE("[Setting Python Lib '" + GET_TXT_FROM_STRING(Token[3], o_tokens) + "' ] \n\n", o_tokens);
	}
	else if(Token[2] == "البايثون_مكتبات"){

		PythonLibName = GET_TXT_FROM_STRING(Token[3], o_tokens);
		if(DEBUG)DEBUG_MESSAGE("[Setting Python LibName '" + GET_TXT_FROM_STRING(Token[3], o_tokens) + "' ] \n\n", o_tokens);
	}
	
	// --------------------------------------------
	// #ألف
	// --------------------------------------------

	else if (Token[2] == "ألف") // || Token[2] == "الف") // || Token[2] == "الف_اندرويد" || Token[2] == "الف_ايفون")
	{
		if (Token[3] != "")
			ErrorCode("أمر غير معروف : ' " + Token[3] + " ' ", o_tokens);
		
		if (!o_tokens->TOKENS_PREDEFINED)
		{
			if (ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
				ErrorCode("تم الاعلان عن علم ألف مسبقا في السطر : " + ALIF_FLAG_AT_LINE_FILE[o_tokens->PATH_FULL_SOURCE], o_tokens);

			ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE] = true;
			ALIF_FLAG_AT_LINE_FILE[o_tokens->PATH_FULL_SOURCE] = IntToString(o_tokens->Line);

			continue;
		}
		
		if(DEBUG)DEBUG_MESSAGE("[#ALIF] \n\n", o_tokens);

		continue;
		
		/*
		if (o_tokens->ALIF_FLAG)
		{
			if (ALIF_PROGRAM == "PC_BIN_32_64")
				ErrorCode("Alif flag already set at line : " + o_tokens->ALIF_FLAG_AT_LINE + " (For PC) ", o_tokens);
			
			else if (ALIF_PROGRAM == "ANDROID")
				ErrorCode("Alif flag already set at line : " + o_tokens->ALIF_FLAG_AT_LINE + " (For Android) ", o_tokens);
			
			else if (ALIF_PROGRAM == "IPHONE")
				ErrorCode("Alif flag already set at line : " + o_tokens->ALIF_FLAG_AT_LINE + " (For iPhone) ", o_tokens);
			
			else
				ErrorCode("علة: Unknow program type : ' " + ALIF_PROGRAM + " ' ", o_tokens);
		}
		
		
		if (ALIF_PROGRAM != "")
			ErrorCode("علة: Unknow program : ' " + ALIF_PROGRAM + " ' ", o_tokens);
		
		if (Token[2] == "الف")
		{
			ALIF_PROGRAM = "PC_BIN_32_64";
			if(DEBUG)DEBUG_MESSAGE("[#ALIF] \n\n", o_tokens);
		}
		else if (Token[2] == "الف_اندرويد")
		{
			ALIF_PROGRAM = "ANDROID";
			if(DEBUG)DEBUG_MESSAGE("[#ALIF_ANDROID] \n\n", o_tokens);
		}
		else if (Token[2] == "الف_ايفون")
		{
			ALIF_PROGRAM = "IPHONE";
			if(DEBUG)DEBUG_MESSAGE("[#ALIF-IPHONE] \n\n", o_tokens);
		}
		else
			ErrorCode("علة: Unknow flag type : ' " + Token[2] + " ' ", o_tokens);
		*/
	}
	// --------------------------------------------
	else
	{
		if (Token[2] == "الف" ||
			Token[2] == "ا" ||
			Token[2] == "أ" ||
			Token[2] == "الألف" ||
			Token[2] == "الالف")
		{
			ErrorCode("أمر غير معروف : ' " + Token[2] + " '، هل تقصد ' ألف ' ؟ ", o_tokens);
		}
		else
			ErrorCode("أمر غير معروف : ' " + Token[2] + " ' ", o_tokens);
	}	
}
// ---------------------------------------------------------------------------------
// General Erros
// ---------------------------------------------------------------------------------

else if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
	ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى", o_tokens);

else if (Token[1] == "خاص" && !IsInsideClass) // ERROR
	ErrorCode("يجب استعمال ' خاص ' داخل صنف", o_tokens);
else if (Token[1] == "خاص" && Token[2] == "")
	ErrorCode("يجب تحديد نوع المتغير بعد ' خاص ' ", o_tokens);
else if (Token[1] == "خاص" && (Token[2] != "عدد" && Token[2] != "نص" && Token[2] != "منطق" && Token[2] != "دالة") ) // ERROR
	ErrorCode("أمر غير معروف : ' " + Token[2] + " ' ، على العموم أمر ' خاص ' يمكن استعماله فقط مع متغيرات من نوع حرف، عدد، منطق، أو دالة", o_tokens);

// ---------------------------------------------------------------------------------
// _س_ ... C++ ... | _ج_ ... JavaScript ...
// ---------------------------------------------------------------------------------

else if (Token[1] == "_س_" || Token[1] == "_ج_"){

	if (!o_tokens->TOKENS_PREDEFINED)
		continue;
	
	//string DebugTag = Token[1];

	if(Token[1] == "_ج_"){
		// AlifJavaScript

		if (!IsInsideWindow)
			ErrorCode("لايمكن تنفيذ جافاسكريبت من خارج نافذة '" + Token[1] + "' ", o_tokens);

		if (!IsInsideFunction)
			ErrorCode("لايمكن تنفيذ جافاسكريبت من خارج دالة '" + Token[1] + "' ", o_tokens);
		
		if(!WIN_IS_WEB[TheWindow])
			ErrorCode("لايمكن تنفيذ جافاسكريبت في نافذة ليست من نوع ويب '" + TheWindow + "' -> '" + Token[1] + "' ", o_tokens);
		
		ThisIsJavaScript = true;
	}
	
	if (!LIB_INSIDE_CPP_CODE){

		if(DEBUG)DEBUG_MESSAGE(" {_س_ START} " , o_tokens); // DEBUG

		LIB_INSIDE_CPP_CODE = true;
		LIB_PARSER_CG_BUFER = "";

		if(ThisIsJavaScript)
			LIB_PARSER_CG_BUFER = "\n OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->AlifJavaScript_Run(wxT(R\"V0G0N( \n ";

		for (int p = 2; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // _س_ ...C++...@ Alif @...C++... _س_
		{
			if (Token[p] == "_س_" || Token[p] == "_ج_") // End C++ Code
			{
				if(DEBUG)DEBUG_MESSAGE(" {_س_ END} ", o_tokens); // DEBUG
				LIB_INSIDE_CPP_CODE = false;

				if(ThisIsJavaScript)
					LIB_PARSER_CG_BUFER.append(" \n )V0G0N\")); \n "); // JavaScript END
					
				ThisIsJavaScript = false;

				// *** Generate Code ***
				if (IsInsideClass)
				{
					// Class Area
					CPP_CLASS.append(" " + LIB_PARSER_CG_BUFER + " ");
				}
				else if (!IsInsideWindow && !IsInsideFunction)
				{
					// Global Area
					CPP_GLOBAL.append(" " + LIB_PARSER_CG_BUFER + " ");
				}
				else if (!IsInsideWindow && IsInsideFunction)
				{
					// Global Function
					CPP_GLOBAL_FUN.append(" " + LIB_PARSER_CG_BUFER + " ");
				}
				else if (IsInsideWindow && IsInsideFunction)
				{
					// Local Function
					cpp_AddScript(TheFunction, " " + LIB_PARSER_CG_BUFER + " ");
				}
				else
				{
					ErrorCode("علة: لم تنجح عملية ايجاد مكان شفرة سي++", o_tokens);
				}
				// *** *** *** *** *** ***
			}
			else if (Token[p] == "@") // @ Start ...
			{
				// ...C++...@ Alif @...C++...

				if(DEBUG)DEBUG_MESSAGE(" {@} " , o_tokens); // DEBUG

				TempTokenCount = 1; // CheckForSyntax() Need this.
				TempToken[0] = "="; // CheckForSyntax() Need this.

				bool AT_FOUND = false;
				
				for (int c = p + 1; c < o_tokens->TOTAL[o_tokens->Line]; c++)
				{
					if (Token[c] == "@")
					{
						p = c;
						AT_FOUND = true;
						break;
					}
					
					TempToken[TempTokenCount] = Token[c];
					TempTokenCount++;
				}

				if (!AT_FOUND)
					ErrorCode("نهايه شفرة سي++ غير موجوده ' @ '", o_tokens);

				string CLASS_OR_WIN;
				if (IsInsideClass)
					CLASS_OR_WIN = TheClass;
				else
					CLASS_OR_WIN = TheWindow;

				ScriptSyntaxBuffer = CheckForSyntax("C++",				// OBJECTIF_TYPE
												true,					// Accept Using Reference to Window:Controls
												true,					// Accept Using Reference to Window:Function
												true,					// Accept Using Reference to Global Functions
												true,					// Accept Using Reference to Local Functions
												true,					// Accept Using Reference to Global VAR
												true,					// Accept Using Reference to Local VAR
												true,					// Accept Convertion from String To Int
												true,					// Accept Convertion from Int To String
												TempToken, 				// SYNTAX[] string
												(TempTokenCount - 1),	// SYNTAX_LONG int
												CLASS_OR_WIN,			// TMP_WINDOW_NAME
												TheFunction,			// TMP_FUNCTION_NAME
												o_tokens);
			
				if(DEBUG)DEBUG_MESSAGE(" {@} " , o_tokens); // DEBUG

				// *** C++ ***
				LIB_PARSER_CG_BUFER.append(" " + ScriptSyntaxBuffer + " ");
				// *** *** *** *** *** ***

				// @ End.
			}
			else if (Token[p] != "")
			{
				// Add C++ Code to the buffer
				LIB_PARSER_CG_BUFER.append(Token[p]);
				if(DEBUG)DEBUG_MESSAGE(" {" + Token[p] + "} ", o_tokens); // DEBUG
			}
		}
	}
	else
	{
		if(DEBUG)DEBUG_MESSAGE(" {_س_ END} ", o_tokens); // DEBUG
		LIB_INSIDE_CPP_CODE = false;
		ThisIsJavaScript = false;
	}
}

else if (LIB_INSIDE_CPP_CODE)
{
	// Continue to adding C++ Code to the Buffer

		for (int p = 1; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // _س_ ...C++...@ Alif @...C++... _س_
		{
			if (Token[p] == "_س_" || Token[p] == "_ج_") // End C++ Code
			{
				if(DEBUG)DEBUG_MESSAGE(" {_س_ END} ", o_tokens); // DEBUG
				LIB_INSIDE_CPP_CODE = false;

				if(ThisIsJavaScript)
					LIB_PARSER_CG_BUFER.append(" \n )V0G0N\")); \n "); // JavaScript END

				ThisIsJavaScript = false;

				//if(DEBUG)DEBUG_MESSAGE(" \n\n GENERATOR -------> |" + LIB_PARSER_CG_BUFER + "| \n\n", o_tokens); // DEBUG

				// *** Generate Code ***
				if (IsInsideClass)
				{
					// Class Area
					CPP_CLASS.append(" " + LIB_PARSER_CG_BUFER + " ");
				}
				else if (!IsInsideWindow && !IsInsideFunction)
				{
					// Global Area
					CPP_GLOBAL.append(" " + LIB_PARSER_CG_BUFER + " ");
				}
				else if (!IsInsideWindow && IsInsideFunction)
				{
					// Global Function
					CPP_GLOBAL_FUN.append(" " + LIB_PARSER_CG_BUFER + " ");
				}
				else if (IsInsideWindow && IsInsideFunction)
				{
					// Local Function
					cpp_AddScript(TheFunction, " " + LIB_PARSER_CG_BUFER + " ");
				}
				else
				{
					ErrorCode("علة: لم تنجح عملية ايجاد مكان شفرة سي++", o_tokens);
				}
				// *** *** *** *** *** ***
			}
			else if (Token[p] == "@") // @ Start ...
			{
				// ...C++...@ Alif @...C++...

				if(DEBUG)DEBUG_MESSAGE(" {@} " , o_tokens); // DEBUG

				TempTokenCount = 1; // CheckForSyntax() Need this.
				TempToken[0] = "="; // CheckForSyntax() Need this.

				bool AT_FOUND = false;
				
				for (int c = p + 1; c < o_tokens->TOTAL[o_tokens->Line]; c++)
				{
					if (Token[c] == "@")
					{
						p = c;
						AT_FOUND = true;
						break;
					}
					
					TempToken[TempTokenCount] = Token[c];
					TempTokenCount++;
				}

				if (!AT_FOUND)
					ErrorCode("نهايه شفرة سي++ غير موجوده ' @ '", o_tokens);

				string CLASS_OR_WIN;
				if (IsInsideClass)
					CLASS_OR_WIN = TheClass;
				else
					CLASS_OR_WIN = TheWindow;
				
				ScriptSyntaxBuffer = CheckForSyntax("C++",				// OBJECTIF_TYPE
												true,					// Accept Using Reference to Window:Controls
												true,					// Accept Using Reference to Window:Function
												true,					// Accept Using Reference to Global Functions
												true,					// Accept Using Reference to Local Functions
												true,					// Accept Using Reference to Global VAR
												true,					// Accept Using Reference to Local VAR
												true,					// Accept Convertion from String To Int
												true,					// Accept Convertion from Int To String
												TempToken, 				// SYNTAX[] string
												(TempTokenCount - 1),	// SYNTAX_LONG int
												CLASS_OR_WIN,			// TMP_WINDOW_NAME
												TheFunction,			// TMP_FUNCTION_NAME
												o_tokens);
			
				if(DEBUG)DEBUG_MESSAGE(" {@} " , o_tokens); // DEBUG

				// *** C++ ***
				LIB_PARSER_CG_BUFER.append(" " + ScriptSyntaxBuffer + " ");
				// *** *** *** *** *** ***

				// @ End.
			}
			else if (Token[p] != "")
			{
				// Add C++ Code to the buffer
				LIB_PARSER_CG_BUFER.append(Token[p]);
				if(DEBUG)DEBUG_MESSAGE(" {" + Token[p] + "} ", o_tokens); // DEBUG
			}
		}
}

else if (Token[1] == "@")
{
	if (!LIB_INSIDE_CPP_CODE)
		ErrorCode("يجب تحديد ' _س_ ' قبل ' @ '", o_tokens);
	
	ErrorCode("لا يمكن بدء السطر بالإشارة ' @ '", o_tokens);
}

// ---------------------------------------------------------------------------------

else if (Token[1] == "أداة"){
	parser_Control(Token, o_tokens);
}

else if (Token[1] == "نافذة"){
	parser_NewWindow(Token, o_tokens);
}
/*
else if (Token[1] == "عدد"		|| 
		Token[1] == "نص"		|| 
		Token[1] == "منطق"		|| 
		Token[1] == "منطق"		|| 
		Token[1] == "خاص"	|| 
		Token[1] == "ثابت")
	CheckForNewVar(Token, o_tokens);
*/

else if (Token[1] == "عدد" || (Token[1] == "خاص" && Token[2] == "عدد") ){
	parser_Int(Token, o_tokens);
}

else if (Token[1] == "نص" || (Token[1] == "خاص" && Token[2] == "نص") ){
	parser_String(Token, o_tokens);
}

else if (Token[1] == "منطق" || (Token[1] == "خاص" && Token[2] == "منطق")){
	parser_bool(Token, o_tokens);
}

else if (Token[1] == "صنف"){
	parser_Class(Token, o_tokens);
}

else if ((Token[1] == "دالة" && IsInsideClass) || 
		(Token[1] == "خاص" && Token[2] == "دالة" && IsInsideClass)){
	parser_FunctionClass(Token, o_tokens);
}

else if (Token[1] == "دالة"){
	parser_Function(Token, o_tokens);
}

else if (Token[1] == "إذا" || Token[1] == "أو"){
	parser_IfOr(Token, o_tokens);
}

else if (Token[1] == "وإلا"){
	parser_IfElse(Token, o_tokens);
}

else if (Token[1] == "كلما"){
	parser_While(Token, o_tokens);
}

else if (Token[1] == "التالي" || Token[1] == "كسر"){
	parser_NextBreak(Token, o_tokens);
}

else if (Token[1] == "إرجاع"){
	parser_Return(Token, o_tokens);
}

else if (Token[1] == "كائن") {
	parser_ObjNew(Token, o_tokens);
}

else if (OBJ_IS_SET[std::make_pair(TheWindow + TheFunction, Token[1])] || 	// Window -> Func. 	Local Obj.
		 OBJ_IS_SET[std::make_pair(TheClass + TheFunction, Token[1])] || 	// Class -> Func. 	Local Obj.
		 OBJ_IS_SET[std::make_pair(TheClass, Token[1])] ||					// Class. 			Global Class Obj.
		 OBJ_IS_SET[std::make_pair("", Token[1])]){							// Global Area. 	Global Obj.
	parser_Obj(Token, o_tokens);
}

else if (L_VAR_IS_SET[std::make_pair(TheWindow + TheFunction, Token[1])] || 
		 L_VAR_IS_SET[std::make_pair(TheClass + TheFunction, Token[1])]){
	parser_Var(Token, o_tokens);
}

else if (CLASS_G_VAR_IS_SET[std::make_pair(TheClass, Token[1])] && IsInsideClass){
	parser_VarGlobalClass(Token, o_tokens);
}

else if (substr_utf8(Token[1], 0, 1) == "_" || G_VAR_IS_SET[(Token[1])]){
	parser_VarGlobal(Token, o_tokens);
}

		// Call function (Global / Local / Class-fun)

		else if (	G_FUN_IS_SET[(Token[1])] || 
					L_FUN_IS_SET[std::make_pair(TheWindow, Token[1])] || 
					CLASS_FUN_IS_SET[std::make_pair(TheClass, Token[1])]){
			
			parser_FunctionCall(Token, o_tokens);
		}

else if (	CONTROL_WIN_IS_SET[Token[1]] || 
			WIN_IS_SET [Token[1]] || 
			CONTROL_IS_SET[std::make_pair(TheWindow, Token[1])] ||
			(Token[1] == "رئيسية" && MAIN_WIN_IS_SET)){
	parser_TwoPointOperator(Token, o_tokens);
}

else if (Token[1] == "نهاية"){
	parser_End(Token, o_tokens);
}

// #######################################################

		else
		{
			if (!o_tokens->TOKENS_PREDEFINED)
			{
				if (Control_ID[Token[1]] != "")
				{
					// show error description
					// when, (no-win) CTR:OPTION
					// in global area.

					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، على العموم، إن كنت تقصد أداة، لا تنسى أن تحدد إسم النافذة قبل إسم الأداة  ", o_tokens);
				}
				// نافذة
				else if (Token[1] == "نافدة" || 
						Token[1] == "النافدة" || 
						Token[1] == "النافذة" ||
						Token[1] == "نافدت" ||
						Token[1] == "نافذت" ||
						Token[1] == "نافذ" ||
						Token[1] == "نافد")
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' نافذة ' ؟ ", o_tokens);
				}
				// نص
				else if (Token[1] == "كلمة" || 
						Token[1] == "حروف" || 
						Token[1] == "الحرف" ||
						Token[1] == "نص" ||
						Token[1] == "الحروف" ||
						Token[1] == "جملة")
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' نص ' ؟ ", o_tokens);
				}
				// إذا
				else if (Token[1] == "ادا" || 
						Token[1] == "إدا" || 
						Token[1] == "أدا" ||
						Token[1] == "اذا" ||
						Token[1] == "أذا")
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' إذا ' ؟ ", o_tokens);
				}
				// أو
				else if (Token[1] == "او" || 
						Token[1] == "ٱو" || 
						Token[1] == "آو" ||
						Token[1] == "والا" || 
						//Token[1] == "وإلا" || 
						Token[1] == "وألا")
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' وإلا ' أو تقصد ' أو ' ؟ ", o_tokens);
				}
				// أداة
				else if (Token[1] == "اداة" || Token[1] == "اداه" || Token[1] == "ادات" || 
						Token[1] == "آداة" || Token[1] == "آداه" || Token[1] == "آدات" || 
						Token[1] == "أداه" || Token[1] == "أدات" || 
						Token[1] == "ٱداة" || Token[1] == "ٱداه" || Token[1] == "ٱدات")
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' أداة ' ؟ ", o_tokens);
				}
				// أضف
				else if (Token[1] == "اضف" || Token[1] == "ٱضف" || Token[1] == "آضف" || 
						Token[1] == "أظف" || Token[1] == "اظف" || Token[1] == "ٱظف" || 
						Token[1] == "آظف" || Token[1] == "إظف" || Token[1] == "إضف") 
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' #أضف ' ؟ ", o_tokens);
				}
				// خاص
				else if (Token[1] == "خص" || 
						Token[1] == "حاص" || 
						Token[1] == "الخاص" ||
						Token[1] == "الحاص")
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' خاص ' ؟ ", o_tokens);
				}
				// إرجاع
				else if (Token[1] == "رجوع" || 
						Token[1] == "الرجوع" || 
						Token[1] == "return" ||
						Token[1] == "أرجاع" ||
						Token[1] == "ارجاع")
				{
					ErrorCode("أمر غير معروف : ' " + Token[1] + " '، هل تقصد ' إرجاع ' ؟ ", o_tokens);
				}

				// Yes, Unknow Token.. but meybe the definition 
				// of this unknown token is in the next lines
				// so lets Parse all Tokens first, then re-parse again
			}
			else
			{
				
				//else
					ErrorCode("أمر غير معروف : ' " + Token[1] + " ' ", o_tokens);
			}	
		}

		// #######################################################

		if (o_tokens->TOKENS_PREDEFINED)
			if(DEBUG)DEBUG_MESSAGE("\n\n", o_tokens);

	} // End Line Loop

	// -------------------------
	// Check current file
		//if (o_tokens->TOKENS_PREDEFINED)
		FINAL_CURRENT_FILE_CODE_CHECKING(o_tokens);
	// -------------------------
}
