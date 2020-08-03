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

void parser_End(string Token[2048], CLASS_TOKEN *o_tokens){
	// نهاية

	if (Token[2] == "")
		ErrorCode("نهايه من دون تحديد المستهدف", o_tokens);
	
	if (Token[3] != "")
		ErrorCode("أمر غير معروف : ' " + Token[3] + " ' ", o_tokens);
	
	if (Token[2] == "دالة")
	{
		if (!IsInsideFunction)
			ErrorCode("يجب استعمال نهايه داخل داله", o_tokens);
		
		if (ALIF_IF_STATUS > 0)
			ErrorCode("مازال هناك " + IntToString(ALIF_IF_STATUS) + " شروط مازالت مفتوحه", o_tokens);

		if (ALIF_LOOP_STATUS > 0)
			ErrorCode("مازال هناك " + IntToString(ALIF_LOOP_STATUS) + " كلما مازالت مفتوحه", o_tokens);
		
		if (IsInsideClass)
		{
			if (!o_tokens->TOKENS_PREDEFINED)
			{
				IsInsideFunction = false;	// Need by Tokens Predefined
				TheFunction = "";		// Need by Tokens Predefined
				return; // continue;
			}

			if (RETURN_FUN[std::make_pair(TheClass, TheFunction)] == "")
				ErrorCode("يجب تعيين إرجاع للدالة ' " + TheFunction + " ' ", o_tokens);
				
			if (RETURN_FUN[std::make_pair(TheClass, TheFunction)] == "IF")
				ErrorCode("لقد عينت إرجاع مشروطه للدالة ' " + TheFunction + " ' ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("	[END CLASS-FUNCTION] {" + TheFunction + "} \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			// End Class Func()
			CPP_CLASS.append(" \n } \n");
			// *** *** *** *** *** ***
		}
		else if (!IsInsideWindow)
		{
			if (!o_tokens->TOKENS_PREDEFINED)
			{
				IsInsideFunction = false; // Need by Tokens Predefined
				TheFunction = ""; // Need by Tokens Predefined
				return; // continue;
			}

			if (RETURN_FUN[std::make_pair("", TheFunction)] == "")
				ErrorCode("يجب تعيين إرجاع للدالة ' " + TheFunction + " ' ", o_tokens);

			if (RETURN_FUN[std::make_pair("", TheFunction)] == "IF")
				ErrorCode("لقد عينت إرجاع مشروطه للدالة ' " + TheFunction + " ' ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[END GLOBAL-FUNCTION] {" + TheFunction + "} \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			// End Global Func()
			CPP_GLOBAL_FUN.append(" \n } \n");
			// *** *** *** *** *** ***
		}
		else
		{
			if (!o_tokens->TOKENS_PREDEFINED)
			{
				IsInsideFunction = false; // Need by Tokens Predefined
				TheFunction = ""; // Need by Tokens Predefined
				return; // continue;
			}

			if (RETURN_FUN[std::make_pair(TheWindow, TheFunction)] == "")
				ErrorCode("يجب تعيين إرجاع للدالة ' " + TheFunction + " ' ", o_tokens);
			if (RETURN_FUN[std::make_pair(TheWindow, TheFunction)] == "IF")
				ErrorCode("لقد عينت إرجاع مشروطه للدالة ' " + TheFunction + " ' ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("	[END FUNCTION] {" + TheFunction + "} \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			// End Local Func()
			if (TheFunction != "رئيسية")
				CPP_FUN.append(" \n } \n");
			// *** *** *** *** *** ***
		}
		
		IsInsideFunction = false;
		TheFunction = "";
		return; // continue;
	}
	else if (Token[2] == "إذا")
	{
		if (!o_tokens->TOKENS_PREDEFINED)
			return; // continue;

		if (ALIF_IF_STATUS < 1)
			ErrorCode("بدايه الشرط غير موجود", o_tokens);
		
		if(DEBUG)DEBUG_MESSAGE("	[END IF " + IntToString(ALIF_IF_STATUS) + "] \n\n", o_tokens); // DEBUG

		if (IsInsideClass)
		{
			// just for fixing this ...
			// *** Generate Code ***
			CPP_CLASS.append("\n } \n ");
			// *** *** *** *** *** ***
		}
		else if (!IsInsideWindow)
			// global func
			CPP_GLOBAL_FUN.append("\n } \n ");
		else
			// local func
			cpp_AddScript(TheFunction, "\n } \n ");		
		// *** *** *** *** *** ***
		
		ALIF_IF_STATUS--;
		return; // continue;
	}
	else if (Token[2] == "كلما")
	{
		if (!o_tokens->TOKENS_PREDEFINED)
			return; // continue;

		if (ALIF_LOOP_STATUS < 1)
			ErrorCode("بدايه كلما غير موجود", o_tokens);
		
		if(DEBUG)DEBUG_MESSAGE("	[END LOOP " + IntToString(ALIF_LOOP_STATUS) + "] \n\n", o_tokens); // DEBUG

		if (IsInsideClass)
		{
			// just for fixing this ...
			// *** Generate Code ***
			CPP_CLASS.append("\n } \n ");
			// *** *** *** *** *** ***
		}
		else if (!IsInsideWindow)
			// global func
			CPP_GLOBAL_FUN.append("\n } \n ");
		else
			// local func
			cpp_AddScript(TheFunction, "\n } \n ");		
		// *** *** *** *** *** ***
		
		ALIF_LOOP_STATUS--;
		return; // continue;
	}
	else if (Token[2] == "نافذة")
	{
		if (!IsInsideWindow)
			ErrorCode("يجب ان تكون داخل داله", o_tokens);
		
		if (IsInsideFunction)
			ErrorCode("يجب اغلاق الدالة : " + TheFunction, o_tokens);

		if (!o_tokens->TOKENS_PREDEFINED)
		{
			IsInsideWindow = false; // Need by Tokens Predefined
			TheWindow = ""; // Need by Tokens Predefined
			return; // continue;
		}
		
		if(DEBUG)DEBUG_MESSAGE("[END WINDOW] {" + TheWindow + "} \n\n", o_tokens); // DEBUG
		
		IsInsideWindow = false;
		TheWindow = "";
		return; // continue;
	}
	else if (Token[2] == "صنف")
	{
		if (!IsInsideClass)
			ErrorCode("يجب أن تكون داخل صنف", o_tokens);
		
		if (IsInsideFunction)
			ErrorCode("يجب اغلاق الدالة : " + TheFunction, o_tokens);
		
		if (!o_tokens->TOKENS_PREDEFINED)
		{
			IsInsideClass = false; // Need by Tokens Predefined
			TheClass = ""; // Need by Tokens Predefined
			return; // continue;
		}

		if(DEBUG)DEBUG_MESSAGE("[END CLASS] {" + TheClass + "} \n\n", o_tokens); // DEBUG

		// *** Generate Code ***
		/*
		if (!IsInsideFunction)
			CPP_GLOBAL.append(" }; \n ");
		else
			cpp_AddScript(TheFunction, " }; \n ");
		*/
		CPP_CLASS.append("\n }; \n "); // Close the Class.
		CPP_CLASS.append(OBJ_GLOBAL_DECLARATION[TheClass] + " \n "); // Add all global objs related to this class.
		// *** *** *** *** *** ***
		
		IsInsideClass = false;
		OBJ_GLOBAL_DECLARATION[TheClass] = "";
		TheClass = "";
		return; // continue;
	}
	else
	{
		if (Token[2] == "ادا" || 
			Token[2] == "إدا" || 
			Token[2] == "أدا" ||
			Token[2] == "اذا" ||
			Token[2] == "أذا")
		{
			ErrorCode("أمر غير معروف : ' " + Token[2] + " '، هل تقصد ' نهاية إذا ' ؟ ", o_tokens);
		}
		else
			ErrorCode("أمر غير معروف : ' " + Token[2] + " ' ", o_tokens);
		
		// TODO: More doyo mean ?!
	}

}
