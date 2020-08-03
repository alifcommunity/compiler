// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

// Core

string CHECK_CALL_FUN_ARG(bool CALL_FUN_GLOBAL,
						  string CALL_WIN_OR_CLASS, // win1/class1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> win1
						  string CALL_FUN,			// win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> fun1
						  int CALL_IS_CLASS,		// 0 = non class, 1 constructor, 2 = الدالة member, ل Message when new obj
						  string FROM_WIN_OR_CLASS, // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> win2
						  string FROM_FUN,			// win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> fun2
						  string SYNTAX[2048],
						  int SYNTAX_LONG,
						  CLASS_TOKEN *o_tokens);

// Parser

void SetNewVar(	bool IsGlobal, 
				string TmpWindow, 
				string TmpFunction, 
				string VarName, 
				string VarDataType, 
				bool IsConstant, 
				bool IsArray,
				int Line, 
				CLASS_TOKEN *o_tokens);

void parser_NewWindowWeb(string Token[2048], CLASS_TOKEN *o_tokens);

// Lexer

void AlifLexerParser(string FILE_NAME, string FILE_TYPE, bool FIRST_FILE, bool TOKENS_ARE_PREDININED);
