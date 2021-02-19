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

void parser_Class(string Token[2048], CLASS_TOKEN *o_tokens){
	//صنف

	if (IsInsideClass)
		ErrorCode("لا يمكن إنشاء صنف داخل صنف، الصنف الحالي  : " + TheClass, o_tokens);
	
	if (IsInsideFunction)
		ErrorCode("لا يمكن أنشاء صنف داخل دالة، يجب إنشائها في المجال عام فقط", o_tokens);

	if (IsInsideNamespace)
		ErrorCode("لا يمكن أنشاء صنف داخل مجال، يجب إنشائها في المجال عام فقط", o_tokens);
	
	if (Token[2] == "")
		ErrorCode("يجب اعطاء اسم لهدا الصنف الجديدة", o_tokens);
	
	if (Token[3] != "" && Token[3] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[3] + " ' ", o_tokens);
	
	if (Token[3] == "(")
		ErrorCode("الصنف لا يقبل خصائص ' ( ' ", o_tokens);
	
	if (!IsValidName(Token[2], o_tokens))
		ErrorCode("تعريف غير مقبول : ' " + Token[2] + " ' ", o_tokens);
	
	// TODO: Check if extended from other class

	if (!o_tokens->TOKENS_PREDEFINED)
	{
		if (CLASS_IS_SET[Token[2]])
			ErrorCode("الصنف ' " + Token[2] + " ' تم انشاؤه مسبقا في السطر رقم : " + CLASS_AT_LINE[Token[2]], o_tokens);

		// if already any other type of global var exist
		CheckForSameGlobalID(Token[2], o_tokens);
		
		// Add New Class
		CLASS_IS_SET[Token[2]] = true;
		CLASS_AT_LINE[Token[2]] = IntToString(o_tokens->Line);

		IsInsideClass = true;
		TheClass = Token[2];

		SET_GLOBAL_C_NAME(Token[2]);

		return; // continue;
	}
	
	IsInsideClass = true;
	TheClass = Token[2];




	//TheNamespace = TheClass; // < ======================================= TODO: TO BE CHECK IF THIS WORKAROUND WORK FINE !!!!!!



	
	if(DEBUG)DEBUG_MESSAGE("[NEW CLASS] [" + Token[2] + "] \n\n", o_tokens); // DEBUG

	// *** Generate Code ***
	// New Class
	CPP_CLASS.append(" class CLASS_" + Global_ID[Token[2]] + " { \n");
	// *** *** *** *** *** ***

	return; // continue;
}
