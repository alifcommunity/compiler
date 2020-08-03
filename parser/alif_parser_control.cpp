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

void parser_Control(string Token[2048], CLASS_TOKEN *o_tokens){
	//أداة

	if (IsInsideWindow || IsInsideFunction || IsInsideClass)
		ErrorCode("يجب انشاء الادوات في المنطقة العامة", o_tokens);

	if (Token[2] == "نافذة")
	{
		if (Token[3] != "(")
			ErrorCode("إشارة مفقودة '(' قبل ' " + Token[3] + " ' ", o_tokens);
		if (Token[5] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[5] + " ' ", o_tokens);
		if (Token[7] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[7] + " ' ", o_tokens);
		if (Token[9] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[9] + " ' ", o_tokens);
		if (Token[11] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[11] + " ' ", o_tokens);
		if (Token[13] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[13] + " ' ", o_tokens);
		if (Token[15] != ")")
			ErrorCode("إشارة مفقودة ')' بعد ' " + Token[14] + " ' ", o_tokens);
		
		if (!IsValidName(Token[4], o_tokens))
			ErrorCode("تعريف غير مقبول : ' " + Token[4] + " ' ", o_tokens);

		// if already any other type of global var exist
		CheckForSameGlobalID(Token[6], o_tokens);
		
		if (!o_tokens->TOKENS_PREDEFINED)
		{
			if (CONTROL_WIN_IS_SET[Token[4]])
				ErrorCode("النافذة ' " + Token[4] + " ' تم انشاؤها مسبقا في السطر رقم : " + CONTROL_WIN_AT_LINE[(Token[4])], o_tokens);
			
			CONTROL_WIN_IS_SET[Token[4]] = true;
			WIN_IS_WEB[Token[4]] = false;
			CONTROL_WIN_AT_LINE[(Token[4])] = IntToString(o_tokens->Line);
			CPP_WINDOW[std::make_pair(Token[4], "شكل")] = " wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN ";
			APP_TYPE = "PC_GUI";

			SET_C_NAME(Token[4]);
			return; // continue;
		}
		
		if(DEBUG)DEBUG_MESSAGE("[CONTROL] [WINDOW] ['" + Token[4] + " '] \n\n", o_tokens); // DEBUG
		
		// *** Generate Code ***
		// New Window
		CPP_WINDOW[std::make_pair(Token[4], "افصول")] = Token[6];
		CPP_WINDOW[std::make_pair(Token[4], "ارتوب")] = Token[8];
		CPP_WINDOW[std::make_pair(Token[4], "عرض")] = Token[10];
		CPP_WINDOW[std::make_pair(Token[4], "ارتفاع")] = Token[12];
		CPP_WINDOW[std::make_pair(Token[4], "نص")] = GET_TXT_FROM_STRING(Token[14], o_tokens);
		if (Token[4] != "رئيسية")
		{
			WIN_TOTAL++;
			WIN_TOTAL_NAMES[WIN_TOTAL] = Token[4];
			CPP_ID_DECLARATION.append(" int ID_WINDOW_" + ID[Token[4]] + " = ALIFCORE_ID_GENERATOR(); \n");
			CPP_GLOBAL.append(" bool WINDOW_IS_SHOW_" + ID[Token[4]] + " = false; \n");
			CPP_GLOBAL.append(" bool WINDOW_IS_CONSTRUCTION_" + ID[Token[4]] + " = false; \n");
			CPP_GLOBAL.append(" void WINDOW_LOAD_" + ID[Token[4]] + "(); \n");
		}
		// *** *** *** *** *** ***
		
		return; // continue;
	}
	else if (	Token[2] == "زر" || 
				Token[2] == "نص" ||
				Token[2] == "ملصق")
	{
		if (Token[3] != "(")
			ErrorCode("إشارة مفقودة '(' قبل ' " + Token[3] + " ' ", o_tokens);
		if (Token[5] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[5] + " ' ", o_tokens);
		if (Token[7] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[7] + " ' ", o_tokens);
		if (Token[9] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[9] + " ' ", o_tokens);
		if (Token[11] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[11] + " ' ", o_tokens);
		if (Token[13] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[13] + " ' ", o_tokens);
		if (Token[15] != ",")
			ErrorCode("إشارة مفقودة ',' قبل ' " + Token[15] + " ' ", o_tokens);
		if (Token[17] != ")")
			ErrorCode("إشارة مفقودة ')' بعد ' " + Token[16] + " ' ", o_tokens);
		
		if (!CONTROL_WIN_IS_SET[Token[4]] && Token[4] != "رئيسية")
			ErrorCode("نافذه غير موجوده : ' " + Token[4] + " ' ", o_tokens);
		
		if (!IsValidName(Token[6], o_tokens))
			ErrorCode("تعريف غير مقبول : ' " + Token[6] + " ' ", o_tokens);
		
		// if already any other type of global var exist
		CheckForSameGlobalID(Token[6], o_tokens);
		
		if (!o_tokens->TOKENS_PREDEFINED)
		{
			if (CONTROL_IS_SET[std::make_pair(Token[4], Token[6])])
				ErrorCode("الأداة ' " + Token[6] + " ' تم انشاؤها مسبقا في السطر رقم : " + 
								CONTROL_AT_LINE[std::make_pair(Token[4], Token[6])], o_tokens);
		
			CONTROL_IS_SET[std::make_pair(Token[4], Token[6])] = true;
			CONTROL_AT_LINE[std::make_pair(Token[4], Token[6])] = IntToString(o_tokens->Line);
			CONTROL_TYPE[std::make_pair(Token[4], Token[6])] = Token[2];

			SET_CONTROL_C_NAME(Token[6]);
			return; // continue;
		}
		
		if(DEBUG)DEBUG_MESSAGE("[CONTROL] ['" + Token[6] + " '] ['" + Token[2] + " '] \n\n", o_tokens); // DEBUG

		// *** Generate Code ***
		// New CTR
		CBUFER_ID = "ID_CTR_" + ID[Token[4]] + "_" + Control_ID[Token[6]];
		CBUFER_OBJ = "OBJ_CTR_" + ID[Token[4]] + "_" + Control_ID[Token[6]];
		CPP_ID_DECLARATION.append(" int " + CBUFER_ID + " = ALIFCORE_ID_GENERATOR(); \n");
		if (Token[2] == "زر")
		{
			CPP_OBJ_DECLARATION.append(" wxButton* " + CBUFER_OBJ + "; \n");
			CBUFER = CPP_WINDOW[std::make_pair(Token[4], "CTR_CONSTRUCTOR")];
			CPP_WINDOW[std::make_pair(Token[4], "CTR_CONSTRUCTOR")] = CBUFER + " \n" + 
			CBUFER_OBJ + " = new wxButton (P, " + CBUFER_ID + ", wxT(\"" + GET_TXT_FROM_STRING(Token[16],o_tokens) + "\"), wxPoint(" + Token[8] + "," + Token[10] + "), wxSize(" + Token[12] + ", " + Token[14] + ")); \n";
		}
		else if (Token[2] == "نص")
		{
			// TODO: AppendText -> أضف
			//       SetLabel -> نص
			// what about wxTE_MULTILINE ?

			CPP_OBJ_DECLARATION.append(" wxTextCtrl* " + CBUFER_OBJ + "; \n");
			CBUFER = CPP_WINDOW[std::make_pair(Token[4], "CTR_CONSTRUCTOR")];
			CPP_WINDOW[std::make_pair(Token[4], "CTR_CONSTRUCTOR")] = CBUFER + " \n" + 
			CBUFER_OBJ + " = new wxTextCtrl (P, " + CBUFER_ID + ", wxT(\"" + GET_TXT_FROM_STRING(Token[16],o_tokens) + "\"), wxPoint(" + Token[8] + "," + Token[10] + "), wxSize(" + Token[12] + ", " + Token[14] + "), wxTE_MULTILINE | wxTE_RIGHT); \n";
		}
		else if (Token[2] == "ملصق")
		{
			CPP_OBJ_DECLARATION.append(" wxStaticText* " + CBUFER_OBJ + "; \n");
			CBUFER = CPP_WINDOW[std::make_pair(Token[4], "CTR_CONSTRUCTOR")];
			CPP_WINDOW[std::make_pair(Token[4], "CTR_CONSTRUCTOR")] = CBUFER + " \n" + 
			CBUFER_OBJ + " = new wxStaticText (P, " + CBUFER_ID + ", wxT(\"" + GET_TXT_FROM_STRING(Token[16],o_tokens) + "\"), wxPoint(" + Token[8] + "," + Token[10] + "), wxSize(" + Token[12] + ", " + Token[14] + "), wxST_NO_AUTORESIZE | wxALIGN_RIGHT | wxST_ELLIPSIZE_END); \n";
		}
		else
		{
			ErrorCode("علة: نوع الأداة غير معروف ' " + Token[2] + " ' ", o_tokens);
		}
		// *** *** *** *** *** ***

		return; // continue;
	}
	else
	{
		if (Token[2] == "نافدة")
			ErrorCode("نوع الأداة غير معروف ' " + Token[2] + " '، هل تقصد ' نافذة ' ؟ ", o_tokens);
		else
			ErrorCode("نوع الأداة غير معروف ' " + Token[2] + " ' ", o_tokens);
	}
}