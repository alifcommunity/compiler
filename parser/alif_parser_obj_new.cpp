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

void parser_ObjNew(string Token[2048], CLASS_TOKEN *o_tokens){
	// كائن

	if (!o_tokens->TOKENS_PREDEFINED && IsInsideFunction)
		// Igiore Local func var predefinition, 
		// focus only on Global var predefinition, and Class global var predefinition.
		return; // continue;
	
	// كائن  الصنف_الثاني  ن  (ع, 2
	// كائن ن = الصنف_الثاني (3, ح

	// TODO: Set Obj Private or Public "خاص"
	//int OBJ_POS = 1;
	//if (Token[1] == "خاص")
		//OBJ_POS = 2;

	// C++, can create obj global/class/func, but use it on func.
	if (IsInsideWindow && !IsInsideFunction)
		ErrorCode("يجب إنشاء الكائن الجديد داخل دالة ", o_tokens);

	//if (Token[2] == "")
		//ErrorCode("يجب وضع إسم الصنف بعد ' كائن '", o_tokens);
	if (Token[2] == "")
		ErrorCode("يجب وضع إسم معرف الكائن بعد ' كائن '", o_tokens);

	//if (Token[3] == "")
		//ErrorCode("يجب وضع إسم معرف الكائن بعد ' كائن " + Token[2] + " ' ", o_tokens);
	if (Token[3] != "=")
		ErrorCode("يجب وضع إشارة '=' بعد ' كائن " + Token[2] + " ' ", o_tokens);

	//if (Token[4] != "" && Token[4] != "(")
		//ErrorCode("أمر غير معروف ' " + Token[4] + " ', هل تقصد ' () ' ؟ ", o_tokens);
	if (Token[4] == "")
		ErrorCode("يجب وضع إسم الصنف بعد ' كائن " + Token[2] + " = '", o_tokens);

	//if (Token[4] == "(" && Token[5] == "")
		//ErrorCode("قوس مازال مفتوح، ربما تقصد ' () ' ", o_tokens);
	if (Token[5] == "(" && Token[6] == "")
		ErrorCode("قوس مازال مفتوح، ربما تقصد ' () ' ", o_tokens);

	if (Token[5] == "(")
	{
		if (Token[o_tokens->TOTAL[o_tokens->Line]] != ")" &&
			Token[o_tokens->TOTAL[o_tokens->Line]] != "")
			ErrorCode("يجب انهاء السطر بالإشارة ')' '... " + Token[o_tokens->TOTAL[o_tokens->Line]] + " ' ", o_tokens);
	}

	if (!CLASS_IS_SET[Token[4]])
		ErrorCode("صنف غير معروف ' " + Token[4] + " ' ", o_tokens);

	if (!IsValidName(Token[2], o_tokens))
		ErrorCode("اسم غير مقبول : ' " + Token[2] + " ' ", o_tokens);

	// if same name as class !
	if (Token[2] == Token[4])
		ErrorCode("تشابه في الاسم بين الكائن و الصنف ' " + Token[2] + " ' ", o_tokens);

	// TODO: Is C++ Allow create Obj inside the same class ? class X { X o; }

	// C++, allow create Obj on global, global-class, local, but using it only on func.
	string OBJ_ID;
	if (IsInsideWindow)
	{
		if (IsInsideFunction)
		{
			// Window -> Function.
			// Local Obj.

			OBJ_ID = TheWindow + TheFunction;

			if (!o_tokens->TOKENS_PREDEFINED){

				if (OBJ_IS_SET[std::make_pair(OBJ_ID, Token[2])])
					ErrorCode("الكائن ' " + Token[2] + " ' تم انشاؤه مسبقا في السطر : " + OBJ_AT_LINE[std::make_pair(OBJ_ID, Token[2])], o_tokens);				
			}
		}
		else
		{
			// Window.
			// Error Obj.
			ErrorCode("يجب إنشاء الكائن الجديد داخل دالة ", o_tokens);
		}
	}
	else if (IsInsideClass)
	{
		if (IsInsideFunction)
		{
			// Class -> Function.
			// Local Class Obj.
			OBJ_ID = TheClass + TheFunction;

			if (!o_tokens->TOKENS_PREDEFINED){

				if (OBJ_IS_SET[std::make_pair(OBJ_ID, Token[2])])
					ErrorCode("الكائن ' " + Token[2] + " ' تم انشاؤه مسبقا في السطر : " + OBJ_AT_LINE[std::make_pair(OBJ_ID, Token[2])], o_tokens);				
			}
		}
		else
		{
			// Class.
			// Global Class Obj.

			//if (o_tokens->TOKENS_PREDEFINED)
				// This Global Obj has already Predefined.

			OBJ_ID = TheClass;

			if (!o_tokens->TOKENS_PREDEFINED){

				if (OBJ_IS_SET[std::make_pair(OBJ_ID, Token[2])])
					ErrorCode("الكائن ' " + Token[2] + " ' تم انشاؤه مسبقا في السطر : " + OBJ_AT_LINE[std::make_pair(OBJ_ID, Token[2])], o_tokens);				
			}
		}
	}
	else
	{
		// Global Area.
		// Global Obj.

		//if (o_tokens->TOKENS_PREDEFINED)
			// This Global Obj has already Predefined.
		
		OBJ_ID = "";

		if (!o_tokens->TOKENS_PREDEFINED){

			if (OBJ_IS_SET[std::make_pair(OBJ_ID, Token[2])])
				ErrorCode("الكائن العام ' " + Token[2] + " ' تم انشاؤه مسبقا في السطر : " + OBJ_AT_LINE[std::make_pair(OBJ_ID, Token[2])], o_tokens);
			
			// if already any other type of global var exist
			CheckForSameGlobalID(Token[2], o_tokens);
		}
	}
	
	// Add new Obj
	//OBJ_IS_SET[std::make_pair(TheWindow + TheFunction, Token[3])] = true;
	//OBJ_AT_LINE[std::make_pair(TheWindow + TheFunction, Token[3])] = IntToString(o_tokens->Line);
	//OBJ_CLASS[std::make_pair(TheWindow + TheFunction, Token[3])] = Token[2];
	
	//if(DEBUG)DEBUG_MESSAGE("		[NEW-OBJ] [CLASS ' " + Token[2] + " '] [Generated_ID ' " + Token[3] + " '] (", o_tokens); // DEBUG
	//if(DEBUG)DEBUG_MESSAGE("		[NEW-OBJ] [Generated_ID ' " + Token[2] + " '] = [CLASS ' " + Token[4] + " '] (", o_tokens); // DEBUG

	// NOTE: Dont add '(' now, 
	// bcs if no Arg then error: 
	// ... which is of non-class type.

	if (IsInsideWindow)
	{
		// Window -> Function.

		if(DEBUG)DEBUG_MESSAGE("		[NEW-LOCAL-OBJ] [Generated_ID ' " + Token[2] + " '] = [CLASS ' " + Token[4] + " '] (", o_tokens); // DEBUG

		SET_OBJ_C_NAME(Token[2]);
		OBJ_IS_SET[std::make_pair(OBJ_ID, Token[2])] = true;
		OBJ_AT_LINE[std::make_pair(OBJ_ID, Token[2])] = IntToString(o_tokens->Line);
		OBJ_CLASS[std::make_pair(OBJ_ID, Token[2])] = Token[4];

		// *** Generate Code ***
		cpp_AddScript(TheFunction, " CLASS_" + Global_ID[Token[4]] + " " + Obj_ID[Token[2]] + " ");
		// *** *** *** *** *** ***
	}
	else if (IsInsideClass)
	{
		if (IsInsideFunction)
		{
			// Class -> Function.

			if(DEBUG)DEBUG_MESSAGE("		[NEW-LOCAL-CLASS-OBJ] [Generated_ID ' " + Token[2] + " '] = [CLASS ' " + Token[4] + " '] (", o_tokens); // DEBUG

			SET_OBJ_C_NAME(Token[2]);
			OBJ_IS_SET[std::make_pair(OBJ_ID, Token[2])] = true;
			OBJ_AT_LINE[std::make_pair(OBJ_ID, Token[2])] = IntToString(o_tokens->Line);
			OBJ_CLASS[std::make_pair(OBJ_ID, Token[2])] = Token[4];

			// *** Generate Code ***
			CPP_CLASS.append(" CLASS_" + Global_ID[Token[4]] + " " + Obj_ID[Token[2]] + " ");
			// *** *** *** *** *** ***
		}
		else
		{
			// Global Class.

			if (!o_tokens->TOKENS_PREDEFINED)
			{
				SET_GLOBAL_OBJ_C_NAME(Token[2]);
				OBJ_IS_SET[std::make_pair(OBJ_ID, Token[2])] = true;
				OBJ_AT_LINE[std::make_pair(OBJ_ID, Token[2])] = IntToString(o_tokens->Line);
				OBJ_CLASS[std::make_pair(OBJ_ID, Token[2])] = Token[4];

				return; // continue;
			}

			if(DEBUG)DEBUG_MESSAGE("	[NEW-GLOBAL-CLASS-OBJ] [Generated_ID ' " + Token[2] + " '] = [CLASS ' " + Token[4] + " '] (", o_tokens); // DEBUG

			// *** Generate Code ***
			CPP_CLASS.append("public: CLASS_" + Global_ID[Token[4]] + " " + GlobalObj_ID[Token[2]] + " ");
			// *** *** *** *** *** ***
		}
	}
	else
	{
		// Global Area.

		if (!o_tokens->TOKENS_PREDEFINED)
		{
			SET_GLOBAL_OBJ_C_NAME(Token[2]);
			OBJ_IS_SET[std::make_pair("", Token[2])] = true;
			OBJ_AT_LINE[std::make_pair("", Token[2])] = IntToString(o_tokens->Line);
			OBJ_CLASS[std::make_pair("", Token[2])] = Token[4];

			// *** Generate Code ***
			// Add this Global Obj to the list.
			// when closing this class, the list will be added at the buttom of class.

			//CPP_CLASS_GLOBAL_OBJ.append(" CLASS_" + Global_ID[Token[4]] + " " + GlobalObj_ID[Token[2]] + " ");
			OBJ_GLOBAL_DECLARATION[Token[4]].append("\n CLASS_" + Global_ID[Token[4]] + " " + GlobalObj_ID[Token[2]] + " ");
			// *** *** *** *** *** ***

			// Searching for ARGs of this global Obj.

			TempTokenCount = 0;
			for (int p = 6; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
			{
				if (Token[p] != "")
				{
					TempToken[TempTokenCount] = Token[p];
					TempTokenCount++;
				}
			}

			ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG( false, 
													//Token[2], 
													Token[4],
													"بناء", 
													1, // 1 = constructor
													TheWindow, 
													TheFunction, 
													TempToken, 
													(TempTokenCount - 1),  
													o_tokens);

			if (ScriptSyntaxBuffer == "")
			{
				//CPP_CLASS_GLOBAL_OBJ.append(" ; \n");
				OBJ_GLOBAL_DECLARATION[Token[4]].append(" ; \n");
			}
			else
			{
				//CPP_CLASS_GLOBAL_OBJ.append(" ( " + ScriptSyntaxBuffer + " ); \n");
				OBJ_GLOBAL_DECLARATION[Token[4]].append(" ( " + ScriptSyntaxBuffer + " ); \n");
			}

			return; // continue;
		}

		if(DEBUG)DEBUG_MESSAGE("[NEW-GLOBAL-OBJ] [Generated_ID ' " + Token[2] + " '] = [CLASS ' " + Token[4] + " '] (", o_tokens); // DEBUG
	}
	
	TempTokenCount = 0;
	//for (int p = 5; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
	for (int p = 6; p <= o_tokens->TOTAL[o_tokens->Line]; p++)
	{
		if (Token[p] != "")
		{
			TempToken[TempTokenCount] = Token[p];
			TempTokenCount++;
		}
	}

	ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG( false, 
											//Token[2], 
											Token[4],
											"بناء", 
											1, // 1 = constructor
											TheWindow, 
											TheFunction, 
											TempToken, 
											(TempTokenCount - 1),  
											o_tokens);
	
	if(DEBUG)DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

	// To fix GCC Error "which is of non-class type"
	// MyClass MyObj();  -->  MyClass MyObj;

	bool IS_EMPTY_ARG = true;
	if (ScriptSyntaxBuffer != "")
		IS_EMPTY_ARG = false;

	// *** Generate Code ***
	if (IsInsideClass)
	{
		// Class.
		// Class -> Function.
		if (IS_EMPTY_ARG)
		{
			CPP_CLASS.append(" ; \n");
		}
		else
		{
			CPP_CLASS.append(" ( " + ScriptSyntaxBuffer + " ); \n");
		}
	}
	else if (IsInsideWindow)
	{
		// Window -> Function.
		if (IS_EMPTY_ARG)
		{
			cpp_AddScript(TheFunction, " ; \n");
		}
		else
		{
			cpp_AddScript(TheFunction, " ( " + ScriptSyntaxBuffer + " ); \n");
		}
	}
	else
	{
		// Global Area.
		// Already processed in OBJ_GLOBAL_DECLARATION[]
	}
	// *** *** *** *** *** ***
	
	return; // continue;
}
