// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

// ----------------------------------
// INITIALIZATION
// ----------------------------------

void ALIF_VAR_INITIALIZATION_FOR_NEW_SOURCE_FILE(bool FIRST_FILE)
{
	// ----------------------------------
	// نافدة
	// ----------------------------------
	
	if (FIRST_FILE)
	{
		MAIN_WIN_IS_SET = false;
		MAIN_WIN_AT_LINE = "0";
	}
	
	IsInsideWindow = false;
	TheWindow = "";
	
	// ----------------------------------
	// دالة
	// ----------------------------------
	
	IsInsideFunction = false;
	TheFunction = "";
	
	// ----------------------------------
	// PARENTHESIS / IF
	// ----------------------------------
	
	ALIF_PARENTHESIS_STATUS = 0;
	
	ALIF_IF_STATUS = 0;

	// ----------------------------------
	// Code
	// ----------------------------------

	ScriptSyntaxBuffer = "";
	ScriptBuffer = "";
}


// ------------------------------------------------------
// Read ALif Lib Setting file
// ------------------------------------------------------

void ALIF_LIB_SETTING()
{
	ifstream FILE_STREAM(PATH_FULL_LIB_SETTING.c_str());

	if (!FILE_STREAM.is_open())
	{
		ALIF_ERROR("ERROR [F001]: Could not open " + PATH_FULL_LIB_SETTING);
		exit(EXIT_FAILURE);
	}

	string LINE8;

	while (getline(FILE_STREAM, LINE8))
	{
		// ------------------------------------------------------
		// Ignore blank lines
		// ------------------------------------------------------

		if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") || (LINE8 == "") || (LINE8 == " "))
			continue;
		
		// ------------------------------------------------------
		// Remove bad line-break character
		// Windows: \r\n
		// Linux: \n
		// MacOS: \r
		// ------------------------------------------------------

		size_t LINE_FIX_LAST_POS = LINE8.find_last_not_of("\r\n");

		if (LINE_FIX_LAST_POS != string::npos)
		{
			LINE8.substr(0, LINE_FIX_LAST_POS + 1).swap(LINE8);
		}

		// ------------------------------------------------------
		// Remove extra spaces
		// ------------------------------------------------------

		//LINE8 = REMOVE_DOUBLE_SPACE(LINE8, &OBJ_CLASS_TOKEN);

		//if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") || (LINE8 == "") || (LINE8 == " "))
			//continue;

		// ------------------------------------------------------
		// Split
		// ------------------------------------------------------

		// رسالة|risalah

		LIB_FILE_NAME[LINE8.substr(0, LINE8.find("|"))] = LINE8.substr(LINE8.find("|") + 1);
	}

	FILE_STREAM.close();
}

void ADD_TOKEN(string TOKEN_CHAR, bool NEW_TOKEN, bool NEW_TOKEN_AFTER, int REAL_CHAR_NUMBER, CLASS_TOKEN *o_tokens)
{
	if (NEW_TOKEN)
	{
		// New Token
		if (o_tokens->TOKEN[std::make_pair(o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] != "")
		{
			o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
		}

		if (TOKEN_CHAR != "")
		{
			if (o_tokens->TOTAL[o_tokens->TOTAL_LINES] < 1)
			{
				o_tokens->TOTAL[o_tokens->TOTAL_LINES] = 1;
			}

			o_tokens->TOKEN[std::make_pair(o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] = TOKEN_CHAR;

			// Set Real Token position in the real line
			o_tokens->REAL_TOKEN_POSITION[std::make_pair(o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] 
			= REAL_CHAR_NUMBER + CharCount_utf8(TOKEN_CHAR, o_tokens);

			if (NEW_TOKEN_AFTER && o_tokens->TOKEN[std::make_pair(o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] != "")
			{
				o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
			}
		}
	}
	else if (TOKEN_CHAR != "")
	{
		// New Char
		if (o_tokens->TOTAL[o_tokens->TOTAL_LINES] < 1)
		{
			o_tokens->TOTAL[o_tokens->TOTAL_LINES] = 1;
		}

		(o_tokens->TOKEN[std::make_pair(o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])]).append(TOKEN_CHAR);

		// Set Real Token position in the real line
		o_tokens->REAL_TOKEN_POSITION[std::make_pair(o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] 
		= REAL_CHAR_NUMBER + CharCount_utf8(TOKEN_CHAR, o_tokens);

		if (NEW_TOKEN_AFTER && o_tokens->TOKEN[std::make_pair(o_tokens->TOTAL_LINES, o_tokens->TOTAL[o_tokens->TOTAL_LINES])] != "")
		{
			o_tokens->TOTAL[o_tokens->TOTAL_LINES]++;
		}
	}
}

void AlifLexerParser(string FILE_NAME, string FILE_TYPE, bool FIRST_FILE, bool TOKENS_ARE_PREDININED)
{
	// ------------------------------------------------------
	// Create new object of tokens class
	// referenced here in this func by ref &
	// referenced in other func by pointer *
	// ------------------------------------------------------

	CLASS_TOKEN OBJ_CLASS_TOKEN; // Create Obj of Tokens Class, Also INITIALIZATION of Tokens
	//OBJ_CLASS_TOKEN.TOTAL_LINES = 1;

	// ------------------------------------------------------
	// ALIF FILE EXTENTION
	// ------------------------------------------------------

	// TODO: check file_name format
	// check if file existe
	// if(!IsValidStringFormat(Token[4], o_tokens))
	// ErrorCode("خطأ في كتابة إسم الملف: "+ Token[4], o_tokens);

	int POS = FILE_NAME.find_last_of(".");
	string EXTENTION;

	// Get extention
	if (POS > 0)
		EXTENTION = FILE_NAME.substr(POS + 1);
	else
		EXTENTION = "";

	// check extention
	if (EXTENTION == "") // #include "myfile" OR #include "/abc/folder/myfile"
	{
		if (FILE_TYPE == "ALIF")
		{
			if (IS_PATH(FILE_NAME))
				OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME + ".alif";
			else
				OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_WORKING + SEPARATION + FILE_NAME + ".alif";
		}
		else if (FILE_TYPE == "ALIFUI")
		{
			if (IS_PATH(FILE_NAME))
				OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME + ".alifui";
			else
				OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_WORKING + SEPARATION + FILE_NAME + ".alifui";
		}
		else if (FILE_TYPE == "ALIFUIW"){

			if (IS_PATH(FILE_NAME)){
				OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME + ".alifuiw";
				PATH_FULL_WINDOW_WEB = FILE_NAME + ".alifuiw";
			}
			else {
				OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_WORKING + SEPARATION + FILE_NAME + ".alifuiw";
				PATH_FULL_WINDOW_WEB = PATH_WORKING + SEPARATION + FILE_NAME + ".alifuiw";
			}
		}
		else if (FILE_TYPE == "ALIFLIB")
		{
			if (!LIB_FILE_NAME[FILE_NAME].empty())
				FILE_NAME = LIB_FILE_NAME[FILE_NAME];

			// #include "MyLib" OR #include "/abc/folder/MyLib"
			// SO, need to Include Library file from Alif Lib folder.

			#ifdef _WIN32
				if (IS_PATH(FILE_NAME))
					OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME + ".aliflib";
				else
					OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_ABSOLUTE + "\\aliflib\\" + FILE_NAME + ".aliflib";
			#elif  __APPLE__
				if (IS_PATH(FILE_NAME))
					OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME + ".aliflib";
				else
					OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = "/Library/Application Support/Aliflang/Alif_Compiler/aliflib/" + FILE_NAME + ".aliflib";
			#else
				if (IS_PATH(FILE_NAME))
					OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME + ".aliflib";
				else
					OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = "/usr/local/lib/aliflib/" + FILE_NAME + ".aliflib";
			#endif
		}
		else
			ErrorCode("علة: نوع ملف غير معروف : ' " + FILE_TYPE + " ' ", &OBJ_CLASS_TOKEN);
	}
	else if (EXTENTION == "alif" || EXTENTION == "ALIF") // #include "myfile.alif" OR "/abc/folder/myfile.alif" OR [alif myfile.alif ...]
	{
		if (FILE_TYPE != "ALIF")
			ErrorCode("يجب إستعمال #اضف لترجمة هدا الملف : ' " + FILE_NAME + " ' ", &OBJ_CLASS_TOKEN);

		if (IS_PATH(FILE_NAME))
			OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME;
		else
			OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_WORKING + SEPARATION + FILE_NAME;
	}
	else if (EXTENTION == "alifui" || EXTENTION == "ALIFUI")
	{
		if (FILE_TYPE != "ALIFUI")
			ErrorCode("جب إستعمال #واجهة لترجمة هدا الملف : ' " + FILE_NAME + " ' ", &OBJ_CLASS_TOKEN);

		if (IS_PATH(FILE_NAME))
			OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME;
		else
			OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_WORKING + SEPARATION + FILE_NAME;
	}
	else if (EXTENTION == "alifuiw" || EXTENTION == "ALIFUIW"){

		if (FILE_TYPE != "ALIFUIW")
			ErrorCode("جب إستعمال #واجهة_ويب لترجمة هدا الملف : ' " + FILE_NAME + " ' ", &OBJ_CLASS_TOKEN);

		if (IS_PATH(FILE_NAME)){
			OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = FILE_NAME;
			PATH_FULL_WINDOW_WEB = FILE_NAME;
		}
		else {
			OBJ_CLASS_TOKEN.PATH_FULL_SOURCE = PATH_WORKING + SEPARATION + FILE_NAME;
			PATH_FULL_WINDOW_WEB = PATH_WORKING + SEPARATION + FILE_NAME;
		}
	}
	else if (EXTENTION == "aliflib" || EXTENTION == "ALIFLIB")
	{
		if (FILE_TYPE != "ALIFLIB")
			ErrorCode("يجب إستعمال #مكتبة لترجمة هدا الملف : ' " + FILE_NAME + " ' ", &OBJ_CLASS_TOKEN);

		// #include "MyLib.aliflib" OR #include "/abc/folder/MyLib.aliflib"

		// This is custom user library file.
		
		ErrorCode("نأسف، حاليا مترجم ألف لا يقبل المكتبات الشخصية، أو مكتبات مسبقة الترجمة، المرجو إزالة إمتداد الملف من أجل إستعمال المكتبات المدمجة مع مترجم ألف : ' " + FILE_NAME + " ' ", &OBJ_CLASS_TOKEN);
	}
	else
	{
		ErrorCode("امتداد الملف غير مقبول : ' " + FILE_NAME + " ' ", &OBJ_CLASS_TOKEN);
	}

	// check file existe
	if (!CHECK_FILE_EXISTE(OBJ_CLASS_TOKEN.PATH_FULL_SOURCE))
	{
		SHOW_FILE_AND_LINE = false;
		ErrorCode("ملف غير موجود : ' " + OBJ_CLASS_TOKEN.PATH_FULL_SOURCE + " ' ", &OBJ_CLASS_TOKEN);
	}

	// ------------------------------------------------------
	// Lex/Pars for WebUI
	// ------------------------------------------------------

	if (FILE_TYPE == "ALIFUIW"){

		// We don't need to do anything with Alif WindowWeb
		// just back to privious parser to continue
		// now, we have the full path of Alif WindowWeb file
		// PATH_FULL_WINDOW_WEB
		return;
	}
	
	// ------------------------------------------------------
	// Current File Type
	// ------------------------------------------------------

	OBJ_CLASS_TOKEN.ALIF_SCRIPT_TYPE = FILE_TYPE;

	// ------------------------------------------------------
	// ALIF VARIABLES INITIALIZATION
	// ------------------------------------------------------

	// Set Initialisation of general variables..
	// Main window set, flag, inside fun or win..
	ALIF_VAR_INITIALIZATION_FOR_NEW_SOURCE_FILE(FIRST_FILE); // true = first file to Lex, false = seconde file..

	// ------------------------------------------------------
	// Log File Start
	// ------------------------------------------------------

	//if(DEBUG)DEBUG_MESSAGE(" ----------- DEBUGING START ----------- \n", &OBJ_CLASS_TOKEN);

	// ------------------------------------------------------
	// Read Source file (UTF8 File name)
	// ------------------------------------------------------

	//Get it from Save folder...

	//map < int , vector<string> > ZZZ;
	//map < int , int, string > ZZZ;

	//map<string, string> SSS;
	//strMap["Monday"]    = "1";

	//std::vector<std::string> VVV;
	//VVV [1] = "xxx";
	//VVV [2] = "xxx";

	//std::map<std::pair<int,int>, string> ZZZ;
	//myMap[std::make_pair(10,20)] = 25;

	//ZZZ[std::make_pair(1,1)] = "aaa";
	//ZZZ[std::make_pair(1,2)] = "الف";

	//ZZZ[std::make_pair(2,1)] = "ccc";
	//ZZZ[std::make_pair(2,2)] = "ddd";

	//ZZZ[3] = {"xxx","SSS"};
	//ZZZ[1][1] = "aaa";
	//ZZZ[1][2] = "bbb";

	//ZZZ[2][1] = "ccc";
	//ZZZ[2][2] = "ddd";

	////cout << "1.1 : " << ZZZ[std::make_pair(1,1)] << endl;
	////cout << "2.1 : " << ZZZ[std::make_pair(2,1)] << endl;

	/*
	if (ZZZ[std::make_pair(1,2)] == "الف")
	{
		//cout << "1.2 = ALIF YES" << endl;
	}
	else
	{
		//cout << "1.2 = ALIF NO." << endl;
	}
	*/

	//OBJ_CLASS_TOKEN.TOTAL [1] = 10;

	//exit(EXIT_FAILURE);

	// ------------------------------------------------------
	// Read Source file (ANSI File name)
	// ------------------------------------------------------

	ifstream FILE_STREAM(OBJ_CLASS_TOKEN.PATH_FULL_SOURCE.c_str());

	if (!FILE_STREAM.is_open())
	{
		ALIF_ERROR("ERROR [F001]: Could not open " + OBJ_CLASS_TOKEN.PATH_FULL_SOURCE);
		exit(EXIT_FAILURE);
	}

	// ------------------------------------------------------

	string LINE8;

	bool INSIDE_STRING_CPP = false;

	while (getline(FILE_STREAM, LINE8))
	{
		// ------------------------------------------------------
		// Ignore blank lines
		// ------------------------------------------------------

		if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") || (LINE8 == "") || (LINE8 == " "))
		{
			OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES] = 0;
			OBJ_CLASS_TOKEN.TOTAL_LINES++;
			continue;
		}

		// ------------------------------------------------------
		// Remove bad line-break character
		// Windows: \r\n
		// Linux: \n
		// MacOS: \r
		// ------------------------------------------------------

		size_t LINE_FIX_LAST_POS = LINE8.find_last_not_of("\r\n");

		if (LINE_FIX_LAST_POS != string::npos)
		{
			LINE8.substr(0, LINE_FIX_LAST_POS + 1).swap(LINE8);
		}

		// ------------------------------------------------------
		// Remove extra spaces
		// ------------------------------------------------------

		LINE8 = REMOVE_DOUBLE_SPACE(LINE8, &OBJ_CLASS_TOKEN);

		if ((LINE8 == "\n") || (LINE8 == "\r") || (LINE8 == "\r\n") || (LINE8 == "") || (LINE8 == " "))
		{
			OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES] = 0;
			OBJ_CLASS_TOKEN.TOTAL_LINES++;
			continue;
		}

		// Set Real Line
		OBJ_CLASS_TOKEN.REAL_LINE[OBJ_CLASS_TOKEN.TOTAL_LINES] = LINE8;

		// ------------------------------------------------------
		// Char loop
		// ------------------------------------------------------

		int CHAR_NUMBER = 0;
		int LINE_CHAR_TOTAL = 0;
		string Char;
		bool INSIDE_STRING = false;

		OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES] = 0;

		LINE_CHAR_TOTAL = CharCount_utf8(LINE8, &OBJ_CLASS_TOKEN);

		while (CHAR_NUMBER < LINE_CHAR_TOTAL) // '<=' is wrong!
		{
			Char = substr_utf8(LINE8, CHAR_NUMBER, 1);
			// ---------------------------------------------

			if (Char == "۰")
				Char = "0";
			else if (Char == "۱")
				Char = "1";
			else if (Char == "۲")
				Char = "2";
			else if (Char == "۳")
				Char = "3";
			else if (Char == "۴")
				Char = "4";
			else if (Char == "۵")
				Char = "5";
			else if (Char == "۶")
				Char = "6";
			else if (Char == "۷")
				Char = "7";
			else if (Char == "۸")
				Char = "8";
			else if (Char == "۹")
				Char = "9";

			else if (Char == "“")
				Char = "\"";
			else if (Char == "”")
				Char = "\"";

			else if (Char == "‘")
				Char = "'";

			// Comments
			//if ( (Char == "\\") && (substr_utf8(LINE8, (CHAR_NUMBER + 1), 1) == "\\") && !INSIDE_STRING)
			if ((CHAR_NUMBER == 0) && (Char == "'") && !INSIDE_STRING) // '
			{
				goto NEXT_LINE;
			}

			// -------------------------------------------------------------

			// _س_ / _ج_

			if ((Char == "_" && (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "س") && (substr_utf8(LINE8, CHAR_NUMBER + 2, 1) == "_")) ||
				(Char == "_" && (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "ج") && (substr_utf8(LINE8, CHAR_NUMBER + 2, 1) == "_")) ||
				(Char == "@" && (substr_utf8(LINE8, CHAR_NUMBER + 1, 1) != "@"))) // Skip '@@'
			{
				string CompletChar = "";

				if(substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "س")
					CompletChar = "_س_";
				else if(substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "ج")
					CompletChar = "_ج_";

				if (Char != "@"){
					CHAR_NUMBER = CHAR_NUMBER + 2; // Point to after : _س_
				}

				if (!INSIDE_STRING_CPP)
				{
					if (Char != "@")
					{
						ADD_TOKEN(CompletChar, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

						//if(DEBUG)DEBUG_MESSAGE(" <NEW start:_س_> " , &OBJ_CLASS_TOKEN); // DEBUG
					}
					else
					{
						ADD_TOKEN("@", true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

						//if(DEBUG)DEBUG_MESSAGE(" <NEW start:@> " , &OBJ_CLASS_TOKEN); // DEBUG
					}

					INSIDE_STRING_CPP = true;
					LIB_LEXER_CG_BUFER = "";
				}
				else
				{
					INSIDE_STRING_CPP = false;
					ADD_TOKEN(LIB_LEXER_CG_BUFER, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

					if (Char != "@")
					{
						ADD_TOKEN(CompletChar, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

						//if(DEBUG)DEBUG_MESSAGE(" <NEW END:_س_>|" + LIB_LEXER_CG_BUFER + "| " , &OBJ_CLASS_TOKEN); // DEBUG
					}
					else
					{
						ADD_TOKEN("@", true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);

						//if(DEBUG)DEBUG_MESSAGE(" <NEW END:@>|" + LIB_LEXER_CG_BUFER + "| " , &OBJ_CLASS_TOKEN); // DEBUG
					}

					LIB_LEXER_CG_BUFER = "";
				}
			}
			else if (INSIDE_STRING_CPP)
			{
				// This char is inside quote _س_

				LIB_LEXER_CG_BUFER.append(Char);
			}
			else if ((Char == "'" && !INSIDE_STRING && !INSIDE_STRING_CPP) || 
					 (Char == "-" && substr_utf8(LINE8, CHAR_NUMBER + 1, 1) == "-" && !INSIDE_STRING && !INSIDE_STRING_CPP))
			{
				goto NEXT_LINE;
			}
			else if (Char == "\"")
			{
				/*
				if (INSIDE_STRING_CPP)
				{
					// This char is inside quote _س_ ..... _س_
					// Add this char to this current token
					ADD_TOKEN("\"", false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
				}
				*/
				if (INSIDE_STRING)
				{
					// String End
					// Add char, and new token after
					ADD_TOKEN("\"", false, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
					INSIDE_STRING = false;
				}
				else
				{
					// String Start
					// New token, add char
					ADD_TOKEN("\"", true, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
					INSIDE_STRING = true;
				}
			}
			/*
			else if (Char == "@")
			{
				if (INSIDE_STRING)
				{
					// Inside String
					// Add char, continue
					ADD_TOKEN("@", false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
				}
				else
				{
					// New token, add char
					ADD_TOKEN("@", true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
				}
			}
			*/
			else if (INSIDE_STRING)
			{
				// This char is inside quote " " OR _س_
				// Add this char to this current token
				ADD_TOKEN(Char, false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
			}
			else if (Char == " ") // Space
			{
				// ignore space!
				// Point to next token
				ADD_TOKEN("", true, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
			}
			else if (Char == "-") // || Char == "+")
			{
				// If is '-123', must be in the same token
				// else, '-', and '123', in different token

				if (IsValidDigit(substr_utf8(LINE8, CHAR_NUMBER + 1, 1), false, &OBJ_CLASS_TOKEN))
				{
					// FORCED Point to next token
					//if(OBJ_CLASS_TOKEN.TOKEN[std::make_pair(OBJ_CLASS_TOKEN.TOTAL_LINES, OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES])] != "")
					//{
					//OBJ_CLASS_TOKEN.TOTAL[OBJ_CLASS_TOKEN.TOTAL_LINES]++;
					//}

					// Add char '-', next is digit, so no new token
					ADD_TOKEN(Char, true, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
				}
				else
				{
					// Add char
					// Point to next token
					ADD_TOKEN(Char, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
				}
			}
			else if (Char == "@" || // C++ Code
					 Char == "#" || // Hash
					 Char == ":" || // Operator
					 Char == "=" ||
					 Char == "+" ||
					 //Char == "-" ||
					 Char == "*" ||
					 Char == "&" || // New line
					 Char == "(" ||
					 Char == ")" ||
					 Char == "[" ||
					 Char == "]" ||
					 Char == "{" ||
					 Char == "}" ||
					 Char == "!" ||
					 Char == "<" ||
					 Char == ">" ||
					 Char == ";" || // To show error !
					 Char == "،" || // اشارة ،
					 Char == ",")
			{
				// Add char
				// Point to next token
				ADD_TOKEN(Char, true, true, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
			}
			else
			{
				// Add char
				ADD_TOKEN(Char, false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
			}
			// ======================================================
			CHAR_NUMBER++;
		} // End char loop.
	NEXT_LINE:

		if (INSIDE_STRING_CPP)
			//ADD_TOKEN("\n", false, false, CHAR_NUMBER, &OBJ_CLASS_TOKEN);
			LIB_LEXER_CG_BUFER.append("\n");

		OBJ_CLASS_TOKEN.TOTAL_LINES++;
	} // End Line loop.
	// ------------------------------------------------------

	if (FIRST_FILE)
	{
		// This is the first file (main.alif)
		// this file, need Tokens Predefinetion

		// ------------------------------------------------------
		// Parser - Tokens Predefinetion
		// ------------------------------------------------------
		if(DEBUG)
			DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION START --- \n", &OBJ_CLASS_TOKEN);
		OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = false;
		// Read list of tokens
		ALIF_PARSER(&OBJ_CLASS_TOKEN);
		if(DEBUG)
			DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION END --- \n\n", &OBJ_CLASS_TOKEN);

		// ------------------------------------------------------
		// Parser - Normal way
		// ------------------------------------------------------
		if(DEBUG)
			DEBUG_MESSAGE(" ----------- DEBUGING START ----------- \n", &OBJ_CLASS_TOKEN);
		OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = true;
		// Read list of tokens
		ALIF_PARSER(&OBJ_CLASS_TOKEN);
		// Check final application code
		FINAL_APPLICATION_CODE_CHECKING(&OBJ_CLASS_TOKEN, FIRST_FILE);
		if(DEBUG)
			DEBUG_MESSAGE("\n ----------- DEBUGING FINISH ------------- \n", &OBJ_CLASS_TOKEN);
	}
	else
	{
		// This is a seconde file (mylib.alif)
		// so, check if this file need Tokens Predefinetion

		if (!TOKENS_ARE_PREDININED)
		{
			// This file did not have any Tokens Predefinetion
			// so, lets start one..

			// ------------------------------------------------------
			// Parser - Tokens Predefinetion
			// ------------------------------------------------------
			if(DEBUG)
				DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION START --- \n", &OBJ_CLASS_TOKEN);
			OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = false;
			// Read list of tokens
			ALIF_PARSER(&OBJ_CLASS_TOKEN);
			if(DEBUG)
				DEBUG_MESSAGE("\n --- TOKENS PREDEFINITION END --- \n\n", &OBJ_CLASS_TOKEN);
		}
		else
		{
			// This file have already Tokens Predefinetion
			// so, lets parse in normal way..

			// ------------------------------------------------------
			// Parser - Normal way
			// ------------------------------------------------------
			if(DEBUG)
				DEBUG_MESSAGE(" ----------- DEBUGING START ----------- \n", &OBJ_CLASS_TOKEN);
			OBJ_CLASS_TOKEN.TOKENS_PREDEFINED = true;
			// Read list of tokens
			ALIF_PARSER(&OBJ_CLASS_TOKEN);
			// Check final application code
			FINAL_APPLICATION_CODE_CHECKING(&OBJ_CLASS_TOKEN, FIRST_FILE);
			if(DEBUG)
				DEBUG_MESSAGE("\n ----------- DEBUGING FINISH ------------- \n", &OBJ_CLASS_TOKEN);
		}
	}
}
