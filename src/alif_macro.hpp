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

        check_macro_alif(o_tokens);

        if (Token[3] == "")
			ErrorCode("مسار الملف غير محدد" + Token[2], o_tokens);
        
        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("مسار الملف غير صائب : ' " + Token[3] + " ' ", o_tokens);
		
        if (Token[4] != "")
            ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);
        
        if(remove_quote(Token[3], o_tokens) == "البايثون")
			PythonSetEnvirenment();
        
        AlifLexerParser(remove_quote(Token[3], o_tokens), "ALIFLIB", false, o_tokens->TOKENS_PREDEFINED);

        if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF LIB] ... \n\n", o_tokens);
    }
    
    // --[ Macro - Include ] --------------------------------------------------

    else if (Token[2] == "أضف") {

        check_macro_alif(o_tokens);

       if (Token[3] == "")
			ErrorCode("مسار الملف غير محدد" + Token[2], o_tokens);
        
        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("مسار الملف غير صائب : ' " + Token[3] + " ' ", o_tokens);
		
        if (Token[4] != "")
            ErrorCode("أمر غير معروف : ' " + Token[4] + " ' ", o_tokens);
        
		AlifLexerParser(remove_quote(Token[3], o_tokens), "ALIF", false, o_tokens->TOKENS_PREDEFINED);

        if(DEBUG)DEBUG_MESSAGE("[#INCLUDE " + Token[3] + " . ALIF] ... \n\n", o_tokens);
    }

   // --[ Macro - UI (WebUI) ] ------------------------------------------------

    else if (Token[2] == "نص") {

        // This macro basically read a file
		// and save it into a const std string.
        // Struct:
        // # نص my_str "my_file"
        
        check_macro_alif(o_tokens);

        if (Token[3] == "")
			ErrorCode("يجب تحديد اسم المتغير", o_tokens);
        
        if (Token[4] == "")
			ErrorCode("يجب تحديد اسم الملف", o_tokens);
        
        if(!IsValidStringFormat(Token[4], o_tokens))
			ErrorCode("خطأ في كتابة إسم الملف: "+ Token[4], o_tokens);
        
        if (Token[5] != "")
			ErrorCode("أمر غير معروف : ' " + Token[5] + " ' ", o_tokens);
        
        if (!o_tokens->TOKENS_PREDEFINED) {

			SET_GLOBAL_C_NAME(Token[3]);
            SetNewVar(true, "", "", Token[3], "نص", false, false, o_tokens->Line, o_tokens);
			return;
		}

        std::string path = remove_quote(Token[4], o_tokens);

        if (!is_file_exists(path)) 
			ErrorCode("ملف غير موجود : ' " + Token[4] + " ' ", o_tokens);
        
        std::string buf;

        file_embed(path, buf, o_tokens);
		
        CPP_GLOBAL.append(" const std::string " + Global_ID[Token[3]] + " = R\"V0G0N(\n" + buf + "\n)V0G0N\"; \n");

        if(DEBUG)DEBUG_MESSAGE("[#نص] [" + Token[3] + "] [" + Token[4] + "] ", o_tokens); // DEBUG
    }

    // --[ Macro - Extra Compile / Link ] -------------------------------------

    // else if (Token[2] == "أظف_ترجمة" || Token[2] == "أظف_ربط") {

    //     // #أظف_ترجمة "-O3 -z"
    //     // #أظف_ربط "-l"        

    //     if(!IsValidStringFormat(Token[3], o_tokens))
    //         ErrorCode("خطأ في كتابة إسم الأمر: "+ Token[3], o_tokens);
        
    //     if (Token[2] == "أظف_ترجمة")
    //         add_extra_arg_to_compiler(remove_quote(Token[3], o_tokens));
    //     else
    //         add_extra_arg_to_linker(remove_quote(Token[3], o_tokens));
    // }

    // --[ Macro - Python ] ---------------------------------------------------

    else if(Token[2] == "البايثون_مسار_عناوين") {

        check_macro_alif(o_tokens);

        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("الأمر غير صائب : ' " + Token[3] + " ' ", o_tokens);

        PythonInclude_path = remove_quote(Token[3], o_tokens);

        if(DEBUG)DEBUG_MESSAGE("[Setting Python Include '" + remove_quote(Token[3], o_tokens) + "' ] \n\n", o_tokens);
    }
    else if(Token[2] == "البايثون_مسار_مكتبات") {

        check_macro_alif(o_tokens);

        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("الأمر غير صائب : ' " + Token[3] + " ' ", o_tokens);

        PythonLib_path = remove_quote(Token[3], o_tokens);

        if(DEBUG)DEBUG_MESSAGE("[Setting Python Lib '" + remove_quote(Token[3], o_tokens) + "' ] \n\n", o_tokens);
    }
    else if(Token[2] == "البايثون_مكتبات") {

        check_macro_alif(o_tokens);

        if (!IsValidStringFormat(Token[3], o_tokens))
			ErrorCode("الأمر غير صائب : ' " + Token[3] + " ' ", o_tokens);

        PythonLibName = remove_quote(Token[3], o_tokens);

        if(DEBUG)DEBUG_MESSAGE("[Setting Python LibName '" + remove_quote(Token[3], o_tokens) + "' ] \n\n", o_tokens);
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

