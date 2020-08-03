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

void parser_While(string Token[2048], CLASS_TOKEN *o_tokens){
	// كلما

    if (!o_tokens->TOKENS_PREDEFINED)
		return; // continue;

    if (!IsInsideFunction)
		ErrorCode("يجب استعمال كلما داخل دالة", o_tokens);

    string PART[1024];
	int PART_TOTAL;

    PART[0] = "=";
	PART_TOTAL = 0;

    int IF_PARENTISE_POSITION;
	IF_PARENTISE_POSITION = 0;

    int Start = 2; // from IF code source !

    ALIF_LOOP_STATUS++;

    if(DEBUG)DEBUG_MESSAGE("	[LOOP " + IntToString(ALIF_LOOP_STATUS) + "] ( ", o_tokens); // DEBUG

    /*
        while(condition)
        {
            statement(s);
        }
    */

    // *** Generate Code ***
    if (!IsInsideWindow)
    {
        // Global Fun Loop
        CPP_GLOBAL_FUN.append(" while ( ");
    }
    else
    {
        // Local Fun Loop
        cpp_AddScript(TheFunction, " while ( ");
    }
	// *** *** *** *** *** ***

    string OBJECTIF_TYPE = "عادم";

    bool PART_A;
	bool PART_B;

    PART_A = false;
	PART_B = false;

	string PART_A_OBJECTIF_TYPE;
	PART_A_OBJECTIF_TYPE = "";

	string IF_SYNTAX_BUFFER;
	IF_SYNTAX_BUFFER = "";

	for (int p = Start; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // Line loop after كلما
	{
		if (Token[p] != "") // Token
		{
			// Condition-part is ready for syntax checking

			if (Token[p] == ">"		||
				Token[p] == "<"		||
				Token[p] == "="		||
				Token[p] == "و"		||
				Token[p] == "!"		||
				Token[p] == "أو")
			{
				// Check if this current condition-part is not empty
				if ( (PART_TOTAL < 1) )
				{
					ErrorCode("جزء غير كامل : ' " + GET_REAL_LINE_MID(0, p, o_tokens) + " ' <-- ", o_tokens);
				}
				
				// Check this condition-part for syntaxt
				IS_IF_SYNTAX = true;
				IF_SYNTAX_BUFFER.append(CheckForSyntax(OBJECTIF_TYPE,			// OBJECTIF_TYPE
									 				true,					// Accept Using Reference to Window:Controls
									  				true,					// Accept Using Reference to Window:Function
									  				true,					// Accept Using Reference to Global Functions
									  				true,					// Accept Using Reference to Local Functions
									  				true,					// Accept Using Reference to Global VAR
									  				true,					// Accept Using Reference to Local VAR
									  				false,					// Accept Convertion from String To Int
									  				false,					// Accept Convertion from Int To String
									  				PART, 					// SYNTAX[] string
									  				(PART_TOTAL),			// SYNTAX_LONG int
									  				TheWindow,			// TMP_WINDOW_NAME
									  				TheFunction,			// TMP_FUNCTION_NAME
									  				o_tokens));

				// IF X1 < X2 or X3 < X4
				if (!PART_A)
				{
					PART_A = true;
					PART_B = false;
				}
				else if (!PART_B)
				{
					PART_A = true;
					PART_B = true;
				}
				else
				{
					PART_A = true;
					PART_B = false;
				}

				// After checking the last part,
				// now process the if operator
				if (Token[p] == ">" && Token[p + 1] == "=")
				{
					// أكبر أو يساوي

					if(DEBUG)DEBUG_MESSAGE("[أكبر أو يساوي] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" >= ");
					// *** *** *** *** *** ***

					p++;
				}
				else if (Token[p] == ">")
				{
					// أكبر

					if(DEBUG)DEBUG_MESSAGE("[أكبر] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" > ");
					// *** *** *** *** *** ***
				}
				else if (Token[p] == "<" && Token[p + 1] == "=")
				{
					// أصغر أو يساوي

					if(DEBUG)DEBUG_MESSAGE("[أصغر أو يساوي] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" <= ");
					// *** *** *** *** *** ***

					p++;
				}
				else if (Token[p] == "<")
				{
					// أصغر

					if(DEBUG)DEBUG_MESSAGE("[أصغر] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" < ");
					// *** *** *** *** *** ***
				}
				else if ( (Token[p] == "!" && Token[p + 1] == "=") || (Token[p] == "=" && Token[p + 1] == "!") )
				{
					// يخالف

					if(DEBUG)DEBUG_MESSAGE("[يخالف] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" != ");
					// *** *** *** *** *** ***

					p++;
				}
				else if (Token[p] == "=")
				{
					// يساوي

					if(DEBUG)DEBUG_MESSAGE("[يساوي] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" == ");
					// *** *** *** *** *** ***
				}
				else if (Token[p] == "و")
				{
					// و

					if(DEBUG)DEBUG_MESSAGE("[و] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" && ");
					// *** *** *** *** *** ***
				}
				else if (Token[p] == "أو")
				{
					// او

					if(DEBUG)DEBUG_MESSAGE("[او] ", o_tokens); // DEBUG

					// *** Generate Code ***
					IF_SYNTAX_BUFFER.append(" || ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("علة : نوع الجزء غير معروف ' " + Token[p] + " ' ", o_tokens);
				}

				// Clear
				for (int c = 0; c <= PART_TOTAL; c++)
					PART[c] = "";

				PART[0] = "=";
				PART_TOTAL = 0;
				OBJECTIF_TYPE = "عادم";
			}

			// Condition-part is not ready for syntax chiking
			// so, add tokens.

			else if (Token[p] == "(") // Open
			{
				IS_IF_SYNTAX = true;

				if (p > 0)
					if (!CAN_ADD_PARENTHESIS_OPEN_HERE(Token[p - 1]))
						ErrorCode("لا يمكن إضافة قوس مفتوح هنا ' " + Token[p - 1] + " " + Token[p] + " ' ", o_tokens);

				IF_PARENTISE_POSITION++;

				PART_TOTAL++;
				PART[PART_TOTAL] = Token[p];
			}
			else if (Token[p] == ")") // Close
			{
				IS_IF_SYNTAX = true;

				if (IF_PARENTISE_POSITION < 1)
					ErrorCode("غلق قوس ليس مفتوحا ' " + Token[p] + " ' ", o_tokens);

				if (p > 0)
					if (!CAN_ADD_PARENTHESIS_CLOSE_HERE(Token[p - 1]))
						ErrorCode("لا يمكن إضافة غلق قوس هنا ' " + Token[p - 1] + " " + Token[p] + " ' ", o_tokens);

				IF_PARENTISE_POSITION--;

				PART_TOTAL++;
				PART[PART_TOTAL] = Token[p];
			}
			else
			{
				if (OBJECTIF_TYPE != "عدد" && OBJECTIF_TYPE != "نص")
				{
					if ( (Token[p] != "+") && (Token[p] != "-") && (Token[p] != "*") && (Token[p] != "\\") )
						if (!IsValidVar(Token[p], o_tokens))
							ErrorCode("نوع المتغير غير معروف ' " + Token[p] + " ' ", o_tokens);
					
					if (IsValidVar_Type == "عدد")
						OBJECTIF_TYPE = "عدد";
					else if (IsValidVar_Type == "نص")
						OBJECTIF_TYPE = "نص";
					else
						ErrorCode("لايمكن إستعمال هدا المتغير ' " + Token[p] + " ' لأنه من نوع " + IsValidVar_Type, o_tokens);

					// IF X1 < X2 or X3 < X4
					if (!PART_A)
					{
						// A
						PART_A_OBJECTIF_TYPE = OBJECTIF_TYPE;
					}
					else if (!PART_B)
					{
						// B
						if (PART_A_OBJECTIF_TYPE != OBJECTIF_TYPE)
							ErrorCode("لايمكن مقارنة نوعين مختلفين : ' " + GET_REAL_LINE_MID(0, p, o_tokens) + " ' <-- ", o_tokens);
					}
					else
					{
						// A
						PART_A_OBJECTIF_TYPE = OBJECTIF_TYPE;
					}				
				}

				PART_TOTAL++;
				PART[PART_TOTAL] = Token[p];
			}
		}
	}

	// Check if the last condition-part is not empty
	if ( (PART_TOTAL < 1) )
	{
		ErrorCode("جزء غير كامل في نهاية السطر : " + GET_REAL_LINE_MID(0, 0, o_tokens) + " <-- ", o_tokens);
	}

	// Part status
	if (!PART_A)
	{
		PART_A = true;
		PART_B = false;
	}
	else if (!PART_B)
	{
		PART_A = true;
		PART_B = true;
	}
	else
	{
		PART_A = true;
		PART_B = false;
	}

	// Check if Part A or B is empty
	if (!PART_A || !PART_B)
	{
		ErrorCode("أحد أطراف الجزء غير موجود : " + GET_REAL_LINE_MID(0, 0, o_tokens) + " <-- ", o_tokens);
	}

	// Check this condition-part for syntaxt
	IS_IF_SYNTAX = true;
	IF_SYNTAX_BUFFER.append(CheckForSyntax(OBJECTIF_TYPE,			// OBJECTIF_TYPE
									 	true,					// Accept Using Reference to Window:Controls
									  	true,					// Accept Using Reference to Window:Function
									  	true,					// Accept Using Reference to Global Functions
									  	true,					// Accept Using Reference to Local Functions
									  	true,					// Accept Using Reference to Global VAR
									  	true,					// Accept Using Reference to Local VAR
									  	false,					// Accept Convertion from String To Int
									  	false,					// Accept Convertion from Int To String
									  	PART, 					// SYNTAX[] string
									  	(PART_TOTAL),			// SYNTAX_LONG int
									  	TheWindow,			// TMP_WINDOW_NAME
									  	TheFunction,			// TMP_FUNCTION_NAME
									  	o_tokens));
	
	// Parentise
	if (IF_PARENTISE_POSITION < 0)
		ErrorCode("هناك " + IntToString(IF_PARENTISE_POSITION) + " أقواس مغلوقة من دون فتحها، المرجو إزالتها", o_tokens);
	else if (IF_PARENTISE_POSITION > 0)
		ErrorCode("بقيت " + IntToString(IF_PARENTISE_POSITION) + " أقواس مفتوحة، المرجو إغلاقها بالإشارة ' ) '", o_tokens);

	// *** Generate Code ***
	if (!IsInsideWindow)
	{
		// Global Fun Loop
		CPP_GLOBAL_FUN.append(IF_SYNTAX_BUFFER + " ) { \n");
	}
	else
	{
		// Local Fun Loop
		cpp_AddScript(TheFunction, IF_SYNTAX_BUFFER + " ) { \n");
	}
	// *** *** *** *** *** ***

	if(DEBUG)DEBUG_MESSAGE(" \n\n", o_tokens); // DEBUG
	return; // continue;
}
