/* 
	The Alif Programming Language
	Version 3.x Series
	(C)2021 Hassan DRAGA
	www.aliflang.org

	This file is part of Alif compiler.

	Alif compiler is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 3, or (at your option) any later
	version.

	Alif compiler is distributed in the hope that it will be useful, but WITHOUT ANY
	WARRANTY; without even the implied warranty of MERCHANTABILITY or
	FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
	for more details.

	You should have received a copy of the GNU General Public License
	along with Alif compiler; see the file COPYING3. If not see
	<http://www.gnu.org/licenses/>.
*/

void check_macro_alif(CLASS_TOKEN *o_tokens) {

    if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
        ErrorCode("يجب الإعلان عن ماكرو ألف اولا، المرجو اضافة ' #ألف ' في الأعلى", o_tokens);  
}

void parser_macro_ui(std::string Token[2048], CLASS_TOKEN *o_tokens){
	
    if (Token[2] == "")
		ErrorCode("الماكرو غير محدد ' # '", o_tokens);
    
    if (IsInsideNamespace || IsInsideFunction)
		ErrorCode("يجب استعمال الماكرو ' # ' في مكان عام", o_tokens);
    
    check_macro_alif(o_tokens);
    
    // --[ Macro - Alif ] -----------------------------------------------------

    if (Token[2] == "ألف") {

        if (Token[3] != "")
            ErrorCode("أمر غير معروف : ' " + Token[3] + " ' ", o_tokens);
        
        if (!o_tokens->TOKENS_PREDEFINED) {

            if (ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
                ErrorCode("تم الاعلان عن ماكرو ألف مسبقا في السطر : " + ALIF_FLAG_AT_LINE_FILE[o_tokens->PATH_FULL_SOURCE], o_tokens);

            ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE] = true;
            ALIF_FLAG_AT_LINE_FILE[o_tokens->PATH_FULL_SOURCE] = IntToString(o_tokens->Line);
        }
        
        if(DEBUG)DEBUG_MESSAGE("[#ALIF]\n", o_tokens);
    }

    // --[ Macro - Library ] --------------------------------------------------

    else if (Token[2] == "مكتبة") { 

        if (Token[3] == "")
			ErrorCode("مسار الملف غير محدد" + Token[2], o_tokens);
        
        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("مسار الملف غير صائب : ' " + Token[3] + " ' ", o_tokens);
		
        if (Token[4] != "")
            ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);
        
        if(GET_TXT_FROM_STRING(Token[3], o_tokens) == "البايثون")
			PythonSetEnvirenment();
        
        AlifLexerParser(GET_TXT_FROM_STRING(Token[3], o_tokens), "ALIFLIB", false, o_tokens->TOKENS_PREDEFINED);

        if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF LIB] ... \n\n", o_tokens);
    }
    
    // --[ Macro - Include ] --------------------------------------------------

    else if (Token[2] == "أضف") {

       if (Token[3] == "")
			ErrorCode("مسار الملف غير محدد" + Token[2], o_tokens);
        
        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("مسار الملف غير صائب : ' " + Token[3] + " ' ", o_tokens);
		
        if (Token[4] != "")
            ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);
        
		AlifLexerParser(GET_TXT_FROM_STRING(Token[3], o_tokens), "ALIF", false, o_tokens->TOKENS_PREDEFINED);

        if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF] ... \n\n", o_tokens);
    }

   // --[ Macro - UI (WebUI) ] ------------------------------------------------

    else if (Token[2] == "واجهة") {

        // This macro basicaly read html file
		// and save it into a const std string.


    }

    // --[ Macro - Extra Compile / Link ] -------------------------------------

    // else if (Token[2] == "أظف_ترجمة" || Token[2] == "أظف_ربط") {

    //     // #أظف_ترجمة "-O3 -z"
    //     // #أظف_ربط "-l"        

    //     if(!IsValidStringFormat(Token[3], o_tokens))
    //         ErrorCode("خطأ في كتابة إسم الأمر: "+ Token[3], o_tokens);
        
    //     if (Token[2] == "أظف_ترجمة")
    //         add_extra_arg_to_compiler(GET_TXT_FROM_STRING(Token[3], o_tokens));
    //     else
    //         add_extra_arg_to_linker(GET_TXT_FROM_STRING(Token[3], o_tokens));
    // }

    // --[ Macro - Python ] ---------------------------------------------------

    else if(Token[2] == "البايثون_مسار_عناوين") {

        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("الأمر غير صائب : ' " + Token[3] + " ' ", o_tokens);

        PythonInclude_path = GET_TXT_FROM_STRING(Token[3], o_tokens);

        if(DEBUG)DEBUG_MESSAGE("[Setting Python Include '" + GET_TXT_FROM_STRING(Token[3], o_tokens) + "' ] \n\n", o_tokens);
    }
    else if(Token[2] == "البايثون_مسار_مكتبات") {

        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("الأمر غير صائب : ' " + Token[3] + " ' ", o_tokens);

        PythonLib_path = GET_TXT_FROM_STRING(Token[3], o_tokens);

        if(DEBUG)DEBUG_MESSAGE("[Setting Python Lib '" + GET_TXT_FROM_STRING(Token[3], o_tokens) + "' ] \n\n", o_tokens);
    }
    else if(Token[2] == "البايثون_مكتبات") {

        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("الأمر غير صائب : ' " + Token[3] + " ' ", o_tokens);

        PythonLibName = GET_TXT_FROM_STRING(Token[3], o_tokens);

        if(DEBUG)DEBUG_MESSAGE("[Setting Python LibName '" + GET_TXT_FROM_STRING(Token[3], o_tokens) + "' ] \n\n", o_tokens);
    }

    // --[ Macro - Unknow ] ---------------------------------------------------

    else {

        if (Token[2] == "الف" ||
            Token[2] == "ا" ||
            Token[2] == "أ" ||
            Token[2] == "الألف" ||
            Token[2] == "الالف") {
            
            ErrorCode("ماكرو غير معروف : ' " + Token[2] + " '، هل تقصد ' ألف ' ؟ ", o_tokens);
        }
        else
            ErrorCode("ماكرو غير معروف : ' " + Token[2] + " ' ", o_tokens);
    }
}

