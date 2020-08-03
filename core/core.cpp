// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

#pragma once

int Generated_ID = 10000;
int ID_GENERATOR()
{
	//return rand() % 10 + 10000;
	return Generated_ID++;
}

bool replace(std::string& str, const std::string& from, const std::string& to){

    size_t start_pos = str.find(from);

    if(start_pos == std::string::npos)
        return false;

    str.replace(start_pos, from.length(), to);

    return true;
}

bool replace_end(std::string& str, const std::string& from, const std::string& to){

    size_t start_pos = str.rfind(from);

    if(start_pos == std::string::npos)
        return false;

    str.replace(start_pos, from.length(), to);

    return true;
}

string substr_utf8(string originalString, int Position, int MaxLen){

	string resultString_start = originalString;
	const char* aStr_start = originalString.c_str();
	
    int len = 0;
    int byteCount = 0;

	if (Position > 0)
	{
		while(*aStr_start)
		{

			if( (*aStr_start & 0xc0) != 0x80 )
			{
				len += 1;
			}
		
			if(len > Position)
			{
				resultString_start = resultString_start.substr(byteCount);
				break;
			}
		
			byteCount++;

			aStr_start++;
		}
	}
	
    if (MaxLen == 0){

		return resultString_start;

	} else {

		// Initialization
		len = 0;
		byteCount = 0;
	
		string resultString = resultString_start;

    	const char* aStr = resultString_start.c_str();

		while(*aStr){

			if( (*aStr & 0xc0) != 0x80 ){

				len += 1;
			}

			if(len>MaxLen){

				resultString = resultString.substr(0, byteCount);
				break;
			}

			byteCount++;
			aStr++;
		}

		return resultString;
    }
}

string IntToString(int INT_VAL){

	// Int --> String

	stringstream STRING_STREAM_BUFFER;
	STRING_STREAM_BUFFER << INT_VAL;
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}

string CONVERT_STRING_ARRAY_TO_STRING(string STRING_ARRAY_VAL[1024], int LONG)
{
	// string[123] --> String
	stringstream STRING_STREAM_BUFFER;
	
	if (LONG < 1)
		return "";
	
	for (int p = 0; p <= LONG; p++)
	{
		STRING_STREAM_BUFFER << STRING_ARRAY_VAL[p];
	}
	
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}

string CONVERT_WCHAT_T_TO_STRING(wchar_t* WCHART_T_VAL)
{
	// wchar_t --> String
	wstring W_STRING_BUFFER (WCHART_T_VAL);
	string STRING_BUFFER (W_STRING_BUFFER.begin(), W_STRING_BUFFER.end());
	return STRING_BUFFER;
}

string CONVERT_CHAR_TO_STRING(char* CHART_VAL)
{
	// Char --> String
	string STRING_BUFFER(CHART_VAL);
	return STRING_BUFFER;
}

// -----------------------------------------------------------
// Bin2C
// -----------------------------------------------------------
/*
int useconst = 0;
int zeroterminated = 0;

int BinaryToC_myfgetc(FILE *f){

	int c = fgetc(f);
	if (c == EOF && zeroterminated){
		zeroterminated = 0;
		return 0;
	}
	return c;
}

char* BinaryToC_str2upr(char *s){

	char * r = (char *) calloc(strlen(s)+1, sizeof(char));

	int i = 0;
	while(*s){

		r[i] = toupper(*s);
		++i;
		++s;
	}
	return r;
} 

void BinaryToC_process(const char *ifname, const char *ofname){

	FILE *ifile, *ofile;

	ifile = fopen(ifname, "rb");
	if (ifile == NULL) {
		//fprintf(stderr, "cannot open %s for reading\n", ifname);
		//exit(1);
		ALIF_ERROR("ERROR : cannot open file for reading -> " + string(ifname));
	}

	ofile = fopen(ofname, "wb");
	if (ofile == NULL) {
		//fprintf(stderr, "cannot open %s for writing\n", ofname);
		//exit(1);
		ALIF_ERROR("ERROR : cannot open file for reading -> " + string(ofname));
	}

	char buf[PATH_MAX], *p;
	const char *cp;

	if ((cp = strrchr(ifname, '/')) != NULL)
		++cp;
	else {
		if ((cp = strrchr(ifname, '\\')) != NULL)
		++cp;
		else
		cp = ifname;
	}

	strcpy(buf, cp);

	for (p = buf; *p != '\0'; ++p)
	
	if (!isalnum(*p))
		*p = '_';
	
	fprintf(ofile, 
			"#ifndef %s_H\n#define %s_H\nstatic %sunsigned char %s[] = {\n",
			BinaryToC_str2upr(buf), 
			BinaryToC_str2upr(buf), 
			useconst ? "const " : "", 
			buf);
	
	int c, col = 1;
	while ((c = BinaryToC_myfgetc(ifile)) != EOF){

		if (col >= 78 - 6){
			fputc('\n', ofile);
			col = 1;
		}

		fprintf(ofile, "0x%.2x, ", c);
		col += 6;
	}

	fprintf(ofile, "\n};\n#endif\n");
	fclose(ifile);
	fclose(ofile);
}

void BinaryToC(string FileIn, string FileOut){

	string FileType = FileIn.substr(FileIn.find_last_of(".") + 1);

	if (FileType == "CSS" || FileType == "css" ||
		FileType == "JS" || FileType == "js" ||
		FileType == "TXT" || FileType == "txt"){
		// String data structure (zero terminated byte)
		zeroterminated = 1;
		useconst = 1;
	} else {
		// Raw data structure
		zeroterminated = 0;
		useconst = 1;
	}

	// bin2c -c myimage.png myimage_png.cpp
	// bin2c -z sometext.txt sometext_txt.cpp
	BinaryToC_process(FileIn.c_str(), FileOut.c_str());
}
*/

// -----------------------------------------------------------
// Bse64
// -----------------------------------------------------------

//https://github.com/ReneNyffenegger/cpp-base64/blob/master/base64.cpp

static const std::string base64_chars = 
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";

static inline bool is_base64(unsigned char c) {
  return (isalnum(c) || (c == '+') || (c == '/'));
}

// const std::string s = "...";
// std::string encoded = base64_encode(reinterpret_cast<const unsigned char*>(s.c_str()), s.length());
// std::string decoded = base64_decode(encoded);

std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
  std::string ret;
  int i = 0;
  int j = 0;
  unsigned char char_array_3[3];
  unsigned char char_array_4[4];

  while (in_len--) {
    char_array_3[i++] = *(bytes_to_encode++);
    if (i == 3) {
      char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
      char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
      char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
      char_array_4[3] = char_array_3[2] & 0x3f;

      for(i = 0; (i <4) ; i++)
        ret += base64_chars[char_array_4[i]];
      i = 0;
    }
  }

  if (i)
  {
    for(j = i; j < 3; j++)
      char_array_3[j] = '\0';

    char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

    for (j = 0; (j < i + 1); j++)
      ret += base64_chars[char_array_4[j]];

    while((i++ < 3))
      ret += '=';

  }

  return ret;

}
/*
std::string base64_decode(std::string const& encoded_string) {
  size_t in_len = encoded_string.size();
  int i = 0;
  int j = 0;
  int in_ = 0;
  unsigned char char_array_4[4], char_array_3[3];
  std::string ret;

  while (in_len-- && ( encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
    char_array_4[i++] = encoded_string[in_]; in_++;
    if (i ==4) {
      for (i = 0; i <4; i++)
        char_array_4[i] = base64_chars.find(char_array_4[i]) & 0xff;

      char_array_3[0] = ( char_array_4[0] << 2       ) + ((char_array_4[1] & 0x30) >> 4);
      char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
      char_array_3[2] = ((char_array_4[2] & 0x3) << 6) +   char_array_4[3];

      for (i = 0; (i < 3); i++)
        ret += char_array_3[i];
      i = 0;
    }
  }

  if (i) {
    for (j = 0; j < i; j++)
      char_array_4[j] = base64_chars.find(char_array_4[j]) & 0xff;

    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);

    for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
  }

  return ret;
}
*/

string BinaryToBase64(string FileIn){

	return "";
}

// -----------------------------------------------------------
// CPP Set Variables Name
// -----------------------------------------------------------

static std::map<string, string> ID;  // abc[var] = V1000
static std::map<string, string> Global_ID; // abc[var] = G_V1000
static std::map<string, string> Control_ID; // abc[var] = C_V1000
static std::map<string, string> Obj_ID; // abc[var] = OBJ_V1000
static std::map<string, string> GlobalObj_ID; // abc[var] = G_OBJ_V1000

void SET_OBJ_C_NAME(string VAR)
{
	if (Obj_ID[VAR] == "")
		Obj_ID[VAR] = "OBJ_" + IntToString(ID_GENERATOR());
}

void SET_GLOBAL_OBJ_C_NAME(string VAR)
{
	if (GlobalObj_ID[VAR] == "")
		GlobalObj_ID[VAR] = "G_OBJ_" + IntToString(ID_GENERATOR());
}

void SET_C_NAME(string VAR)
{
	if (ID[VAR] == "")
		ID[VAR] = "V_" + IntToString(ID_GENERATOR());
}

void SET_GLOBAL_C_NAME(string VAR)
{
	if (Global_ID[VAR] == "")
		Global_ID[VAR] = "G_V_" + IntToString(ID_GENERATOR());
}

void SET_CONTROL_C_NAME(string VAR)
{
	if (Control_ID[VAR] == "")
		Control_ID[VAR] = "C_V_" + IntToString(ID_GENERATOR());
}

bool IsInArray_v(const std::string &value, const std::vector<string> &array){
	
    return std::find(array.begin(), array.end(), value) != array.end();
}

bool IsDataType(const std::string &value){
	
    return IsInArray_v(value, DataType_v);
}

// -----------------------------------------------------------
// Get UTF8 Total Leng
// -----------------------------------------------------------

int CharCount_utf8(string LINE8, CLASS_TOKEN *o_tokens){

	// ------------------------------------------------------
	// check ل invalid utf-8
	// ل a simple yes/no check,
	// there is also utf8::is_valid function
	// ------------------------------------------------------

	string::iterator LINE_END_IT = utf8::find_invalid(LINE8.begin(), LINE8.end());

	if (LINE_END_IT != LINE8.end())
	{
		ALIF_ERROR("ERROR : Invalid UTF-8 encoding at line " + IntToString(o_tokens->TOTAL_LINES));
	}

	// ------------------------------------------------------
	// تحويل line UTF8 إلى UTF32 ل fix character size
	//is needed إلى count chars in line
	// ------------------------------------------------------

	//vector<int short> LINE32;
	string LINE32;
	utf8::utf8to32(LINE8.begin(), LINE_END_IT, back_inserter(LINE32));

	// ------------------------------------------------------
	// Count chars
	// ------------------------------------------------------

	int LINE_LEN = LINE32.length();
	int LINE_UNIT = 0;
	int CHAR_COUNT32 = 0;
	const char *LINE_C_STR = LINE32.c_str();

	while (LINE_UNIT < LINE_LEN)
	{
		LINE_UNIT += mblen(&LINE_C_STR[LINE_UNIT], LINE_LEN - LINE_UNIT);
		CHAR_COUNT32 += 1;
	}

	return CHAR_COUNT32;
}

// ====================================================

string GET_REAL_LINE_MID (int START, int TOKEN_POSITION, CLASS_TOKEN *o_tokens)
{
	if(DEBUG)DEBUG_MESSAGE("REAL_LINE : |" + o_tokens->REAL_LINE[o_tokens->Line] + "| \n", o_tokens);
	if(DEBUG)DEBUG_MESSAGE("START : " + IntToString(START) + " \n", o_tokens);
	if(DEBUG)DEBUG_MESSAGE("TOKEN_POSITION : " + IntToString(TOKEN_POSITION) + " \n", o_tokens);
	if(DEBUG)DEBUG_MESSAGE("REAL_TOKEN_POSITION : " + IntToString(o_tokens->REAL_TOKEN_POSITION[std::make_pair(o_tokens->Line, TOKEN_POSITION)]) + " \n", o_tokens);

	if (START < 0 || TOKEN_POSITION < 1) // Exception !
		return o_tokens->REAL_LINE[o_tokens->Line];
	
	string BUFFER;

	BUFFER = substr_utf8(o_tokens->REAL_LINE[o_tokens->Line], 
						START, 
						o_tokens->REAL_TOKEN_POSITION[std::make_pair(o_tokens->Line, TOKEN_POSITION)]);
	
	if (BUFFER.empty())
		return o_tokens->REAL_LINE[o_tokens->Line]; // Empty, So tray to return full real line !
	else
		return BUFFER; // Return the SubStr requested.
}

// ====================================================

bool IsValidStringFormat(string STRING, CLASS_TOKEN *o_tokens)
{
	if (substr_utf8(STRING, 0, 1) != "\"")
		return false;

	if (substr_utf8(STRING, (CharCount_utf8(STRING, o_tokens) - 1), 1) != "\"")
		return false;

	return true;
}

/*
bool IsValidStringFormatOrStringVar(string STRING, CLASS_TOKEN *o_tokens){

	// TODO : We need add support for global class var, global var.

	if (L_VAR_IS_SET[std::make_pair(TheWindow + TheFunction, STRING)] || 
		L_VAR_IS_SET[std::make_pair(TheClass + TheFunction, STRING)]){

		string CLASS_OR_WIN;
		if (IsInsideClass)
			CLASS_OR_WIN = TheClass;
		else
			CLASS_OR_WIN = TheWindow;

		if(L_VAR_TYPE[std::make_pair(CLASS_OR_WIN + TheFunction, STRING)] != "نص")
			return false;
	}
	else {

		if (substr_utf8(STRING, 0, 1) != "\"")
			return false;

		if (substr_utf8(STRING, (CharCount_utf8(STRING, o_tokens) - 1), 1) != "\"")
			return false;		
	}

	return true;
}
*/

// ====================================================

string REMOVE_DOUBLE_SPACE(string LINE_BUFFER, CLASS_TOKEN *o_tokens)
{
	string Char;
	string BUFFER;

	bool INSIDE = false;
	bool VALID_CHAR = false;
	bool LAST_IS_SPACE = false;

	// Remove Extra Spaces
	// and Trim line end.
	for (int I = 0; I < CharCount_utf8(LINE_BUFFER, o_tokens); I++)
	{
		Char = substr_utf8(LINE_BUFFER, I, 1);

		if (Char == "\"")
		{
			INSIDE = !INSIDE;
			BUFFER.append("\"");
		}
		else if (Char == "\xEF\xBB\xBF") // BOM Signe in first char of file
		{
			if(DEBUG)
				DEBUG_MESSAGE("[BOM]", o_tokens); // LOG
		}
		else if (Char == "\u202E") // RIGHT-TO-LEFT OVERRIDE signe, إلى force RTL
		{
			if(DEBUG)
				DEBUG_MESSAGE("[RTL]", o_tokens); // LOG
		}
		else if (Char == " " || Char == "\t")
		{
			if (INSIDE)
			{
				BUFFER.append(Char);
			}
			else
			{
				if (VALID_CHAR)
				{
					if (!LAST_IS_SPACE)
					{
						BUFFER.append(" ");
						LAST_IS_SPACE = true;
					}
				}
			}
		}
		else
		{
			VALID_CHAR = true;
			LAST_IS_SPACE = false;
			BUFFER.append(Char);
		}
	}

	// Trim line end.
	for (int D = CharCount_utf8(BUFFER, o_tokens); D > 0; D--)
	{
		Char = substr_utf8(BUFFER, (D - 1), 1);

		if (Char != " " && Char != "\t")
		{
			BUFFER = substr_utf8(BUFFER, 0, D);
			break;
		}
	}

	return BUFFER;
}

// ====================================================

bool IsValidDigit(string DIGIT, bool FLOAT, CLASS_TOKEN *o_tokens)
{
	int I = 0;
	int TOTAL_CHAR = CharCount_utf8(DIGIT, o_tokens);
	string Char;
	bool First = true;
	bool DECIMAL = false;

	//  100 --> TRUE
	// -100 --> TRUE
	// -10.25 --> TRUE
	// -10,25 --> TRUE
	// -10،25 --> TRUE

	if (CharCount_utf8(DIGIT, o_tokens) > 1 && (substr_utf8(DIGIT, 0, 1) == "-" || substr_utf8(DIGIT, 0, 1) == "+"))
	{
		I = 1;
	}

	while (I < TOTAL_CHAR)
	{
		Char = substr_utf8(DIGIT, I, 1);

		if (Char == "." || Char == "," || Char == "،")
		{
			if (First)
			{
				return false; // .123
			}

			if (I == (TOTAL_CHAR - 1))
			{
				return false; // 123.
			}

			if (!FLOAT)
			{
				return false; // 12.3, but need integer not float.
			}

			if (DECIMAL)
			{
				return false; // 1.2.3
			}

			DECIMAL = true;
		}
		else if (Char != "0" &&
				 Char != "1" &&
				 Char != "2" &&
				 Char != "3" &&
				 Char != "4" &&
				 Char != "5" &&
				 Char != "6" &&
				 Char != "7" &&
				 Char != "8" &&
				 Char != "9")
		{
			return false;
		}

		I++;
		First = false;
	}

	return true;
}

// ====================================================

bool CAN_ADD_OPERATION_HERE(string TOKEN_LAST)
{
	// = 1 + 2 * (3 / 4 - 5) + 6
	// IF 

	/*
	if (IS_IF_SYNTAX)
	{
		if ((TOKEN_LAST == "=") ||
			(TOKEN_LAST == "+") ||
			(TOKEN_LAST == "-") ||
			(TOKEN_LAST == "*") ||
			(TOKEN_LAST == "/") ||

			(TOKEN_LAST != "<") ||
			(TOKEN_LAST != ">") ||
			(TOKEN_LAST != "و") ||
			(TOKEN_LAST != "أو") ||

			(TOKEN_LAST == "("))
			return false;
	}
	else
	{
	*/
		if ((TOKEN_LAST == "=") ||
			(TOKEN_LAST == "+") ||
			(TOKEN_LAST == "-") ||
			(TOKEN_LAST == "*") ||
			(TOKEN_LAST == "/") ||
			(TOKEN_LAST == "("))
			return false;

	//}

	return true;
}

bool CAN_ADD_VAR_HERE(string TOKEN_LAST)
{
	// str / عدد = متغير + متغير * (var / متغير - var)

	/*
	if (IS_IF_SYNTAX)
	{
		if ((TOKEN_LAST != "+") &&
			(TOKEN_LAST != "-") &&
			(TOKEN_LAST != "*") &&
			(TOKEN_LAST != "/") &&
			(TOKEN_LAST != "=") &&

			(TOKEN_LAST != "<") &&
			(TOKEN_LAST != ">") &&
			(TOKEN_LAST != "و") &&
			(TOKEN_LAST != "أو") &&

			(TOKEN_LAST != "("))
			return false;
	}
	else
	{
	*/
		if ((TOKEN_LAST != "+") &&
			(TOKEN_LAST != "-") &&
			(TOKEN_LAST != "*") &&
			(TOKEN_LAST != "/") &&
			(TOKEN_LAST != "=") &&
			(TOKEN_LAST != "("))
			return false;

	//}

	return true;
}

// ====================================================

bool CAN_ADD_FUN_HERE(string TOKEN_LAST)
{
	// str / عدد = متغير + fun(var) * (fun(var) / fun(var, fun(var), var) - var)

	/*
	if (IS_IF_SYNTAX)
	{
		if ((TOKEN_LAST != "+") &&
			(TOKEN_LAST != "-") &&
			(TOKEN_LAST != "*") &&
			(TOKEN_LAST != "/") &&
			(TOKEN_LAST != "=") &&
			(TOKEN_LAST != "،") &&
			(TOKEN_LAST != ",") &&

			(TOKEN_LAST != "<") &&
			(TOKEN_LAST != ">") &&
			(TOKEN_LAST != "و") &&
			(TOKEN_LAST != "أو") &&

			(TOKEN_LAST != "("))
			return false;
	}
	*/

	if ((TOKEN_LAST != "+") &&
		(TOKEN_LAST != "-") &&
		(TOKEN_LAST != "*") &&
		(TOKEN_LAST != "/") &&
		(TOKEN_LAST != "=") &&
		(TOKEN_LAST != "،") &&
		(TOKEN_LAST != ",") &&
		(TOKEN_LAST != "("))
		return false;

	return true;
}

// ====================================================

bool CAN_ADD_PARENTHESIS_OPEN_HERE(string TOKEN_LAST)
{
	// = ((1)) + (2 * (3 / (4) - 5) +( 6))
	// IF = (a + s < b * h) او (z + 2) != (x - 7) و (z = x)

	
	if (IS_IF_SYNTAX)
	{
		if ((TOKEN_LAST != "+") &&
			(TOKEN_LAST != "-") &&
			(TOKEN_LAST != "*") &&
			(TOKEN_LAST != "/") &&
			(TOKEN_LAST != "=") &&

			(TOKEN_LAST != "<") &&
			(TOKEN_LAST != ">") &&
			(TOKEN_LAST != "و") &&
			(TOKEN_LAST != "أو") &&

			(TOKEN_LAST != "("))
			return true;
	}
	else
	{
		if ((TOKEN_LAST != "+") &&
			(TOKEN_LAST != "-") &&
			(TOKEN_LAST != "*") &&
			(TOKEN_LAST != "/") &&
			(TOKEN_LAST != "=") &&
			(TOKEN_LAST != "("))
			return false;

	}

	return true;
}

// ====================================================

string IsValidVar_Type = "عادم";

bool IsValidVar(string Var, CLASS_TOKEN *o_tokens)
{
	IsValidVar_Type = "عادم";

	if (Var == ""){

		return false;
	}
	else if (Var == "صحيح" || Var == "خطأ"){
		
		IsValidVar_Type = "منطق";
		return true;
	}
	else if (IsValidStringFormat(Var, o_tokens)){
		
		// STRING

		IsValidVar_Type = "نص";

		return true;
	}
	else if (IsValidDigit(Var, true, o_tokens)){
		
		// DIGIT

		IsValidVar_Type = "عدد";

		return true;
	}
	else if ((substr_utf8(Var, 0, 1) == "_" &&
			  G_VAR_IS_SET[(substr_utf8(Var, 1, CharCount_utf8(Var, o_tokens)))])){
		
		// Global متغير _

		IsValidVar_Type = G_VAR_TYPE[substr_utf8(Var, 1, CharCount_utf8(Var, o_tokens))];

		return true;
	}
	else if (L_VAR_IS_SET[std::make_pair(TheWindow + TheFunction, Var)]){
		
		// Local Var

		IsValidVar_Type = L_VAR_TYPE[std::make_pair(TheWindow + TheFunction, Var)];

		return true;
	}
	else if (L_VAR_IS_SET[std::make_pair(TheClass + TheFunction, Var)]){
		
		// Class Local Var

		IsValidVar_Type = L_VAR_TYPE[std::make_pair(TheClass + TheFunction, Var)];

		return true;
	}
	else if (G_VAR_IS_SET[(Var)]){
		
		// Global Var

		IsValidVar_Type = G_VAR_TYPE[Var];

		return true;
	}
	else if (CLASS_G_VAR_IS_SET[std::make_pair(TheClass, Var)] && IsInsideClass){

		// Class Global Var

		IsValidVar_Type = CLASS_G_VAR_TYPE[std::make_pair(TheClass, Var)];

		return true;
	}
	else if (G_FUN_IS_SET[(Var)]){
		
		// Global Function

		IsValidVar_Type = G_FUN_TYPE[Var];

		return true;
	}
	else if (L_FUN_IS_SET[std::make_pair(TheWindow, Var)]){
		
		// Local Function

		IsValidVar_Type = L_FUN_TYPE[std::make_pair(TheWindow, Var)];

		return true;
	}
	else if (L_FUN_IS_SET[std::make_pair(TheClass, Var)]){
		
		// Class Local Function

		IsValidVar_Type = L_FUN_TYPE[std::make_pair(TheClass, Var)];

		return true;
	}
	else if (CONTROL_WIN_IS_SET[Var]){
		
		// Window: (By UI)

		IsValidVar_Type = "عادم";

		return true;
	}
	else if (WIN_IS_SET[Var]){
		
		// Window: (By Code)

		IsValidVar_Type = "عادم";

		return true;
	}
	else if (CONTROL_IS_SET[std::make_pair(TheWindow, Var)]){
		
		// Control:

		IsValidVar_Type = "عادم";

		return true;
	}
	else
		return false;
}

// ====================================================

bool CAN_ADD_PARENTHESIS_CLOSE_HERE(string TOKEN_LAST)
{
	// = ((1)) + (2 * (3 / (4) - 5) + ( 6)) + ()

	if (TOKEN_LAST == "(") // إلى avoid ... () ...
		return false;

	if ((TOKEN_LAST == "=") ||
		(TOKEN_LAST == "+") ||
		(TOKEN_LAST == "-") ||
		(TOKEN_LAST == "*") ||
		(TOKEN_LAST == "/"))
		return false;

	return true;
}

// ====================================================

bool CAN_ADD_DIGIT_HERE(string TOKEN_LAST)
{
	// = 1 + 2 * (3 / 4 - 5) + 6

	/*
	if (IS_IF_SYNTAX)
	{
		if ((TOKEN_LAST != "+") &&
			(TOKEN_LAST != "-") &&
			(TOKEN_LAST != "*") &&
			(TOKEN_LAST != "/") &&
			(TOKEN_LAST != "=") &&

			(TOKEN_LAST != "<") &&
			(TOKEN_LAST != ">") &&
			(TOKEN_LAST != "و") &&
			(TOKEN_LAST != "أو") &&

			(TOKEN_LAST != "("))
			return false;
	}
	else
	{
	*/
		if ((TOKEN_LAST != "+") &&
			(TOKEN_LAST != "-") &&
			(TOKEN_LAST != "*") &&
			(TOKEN_LAST != "/") &&
			(TOKEN_LAST != "=") &&
			(TOKEN_LAST != "("))
			return false;
	//}

	return true;
}

// ====================================================

string GET_TXT_FROM_STRING(string STRING, CLASS_TOKEN *o_tokens)
{
	string MESSAGE_BUFFER;

	if (CharCount_utf8(STRING, o_tokens) < 3)	// "" Emty string
		return "";									// 3 is the minimum msg, "a"

	MESSAGE_BUFFER = substr_utf8(STRING, 1, CharCount_utf8(STRING, o_tokens));
	MESSAGE_BUFFER = substr_utf8(MESSAGE_BUFFER, 0, (CharCount_utf8(MESSAGE_BUFFER, o_tokens) - 1));

	return MESSAGE_BUFFER;
}

// ====================================================

void CheckForSameGlobalID(string Name, CLASS_TOKEN *o_tokens)
{
	// check if same name with any already global declaration

	if (OBJ_IS_SET[std::make_pair("", Name)])
	{
		// same name with Global Obj
		ErrorCode("تشابه في الاسم مع كائن عام ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " + Name + " ' ", o_tokens);
	}
	else if (CLASS_IS_SET[Name])
	{
		// same name with Class
		ErrorCode("تشابه في الاسم مع صنف ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " + Name + " ' ", o_tokens);
	}
	else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "عدد")
	{
		// same name with Global INT
		ErrorCode("تشابه في الاسم مع متغير عددي ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " + Name + " ' ", o_tokens);
	}
	else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "نص")
	{
		// same name with Global STRING
		ErrorCode("تشابه في الاسم مع متغير نصي ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " + Name + " ' ", o_tokens);
	}
	else if (G_VAR_IS_SET[(Name)] && G_VAR_TYPE[(Name)] == "منطق")
	{
		// same name with Global BOOL
		ErrorCode("تشابه في الاسم مع متغير منطقي ثم إنشائه مسبقا، المرجو تغيير الاسم : ' " + Name + " ' ", o_tokens);
	}
	else if (G_FUN_IS_SET[(Name)])
	{
		// same name with Global Function
		ErrorCode("تشابه في الاسم مع دالة عامة ثم إنشائها مسبقا، المرجو تغيير الاسم : ' " + Name + " ' ", o_tokens);
	}
	else if (WIN_IS_SET[Name] || CONTROL_WIN_IS_SET[Name])
	{
		// same name with Window
		ErrorCode("تشابه في الاسم مع نافذة ثم إنشائها مسبقا، المرجو تغيير الاسم : ' " + Name + " ' ", o_tokens);
	}
}

bool IsValidName(string Name, CLASS_TOKEN *o_tokens){

	string Char;
	bool First = true;

	for (int I = 0; I <= CharCount_utf8(Name, o_tokens); I++){

		Char = substr_utf8(Name, I, 1);

		if (Char == "_" || // Allow, Example : ABC_XZ1
			Char == "0" ||
			Char == "1" ||
			Char == "2" ||
			Char == "3" ||
			Char == "4" ||
			Char == "5" ||
			Char == "5" ||
			Char == "6" ||
			Char == "8" ||
			Char == "9" ||

			Char == "۰" || // 0, Zero In Arabic, ITS NOT A DOT!
			Char == "۱" || // 1
			Char == "۲" || // 2
			Char == "۳" || // 3
			Char == "۴" || // 4
			Char == "۵" || // 5
			Char == "۶" || // 6
			Char == "۷" || // 7
			Char == "۸" || // 8
			Char == "۹")   // 9
		{
			if (First)
			{
				ErrorCode("أول حرف في الإسم يجب أن يكون حرفا، و ليس رقم أو إشارة : ' " + Char + " ' ", o_tokens);
				return false;
			}
		}
		else if (Char == "." || // Allow DOT in متغير names ABC.DEF
				 Char == "," ||
				 Char == "،" || // اشارة ،
				 Char == "{" ||
				 Char == "}" ||
				 Char == "|" ||
				 Char == "~" ||
				 //Char == "_" || Allow AB_C
				 Char == "$" ||
				 Char == "#" ||
				 Char == ":" ||
				 Char == "!" ||
				 Char == ";" ||
				 Char == "?" ||
				 Char == "'" ||
				 Char == "-" ||
				 Char == "/" ||
				 Char == "=" ||
				 Char == "\\" ||
				 Char == "^" ||
				 Char == "\"" ||
				 Char == "%" ||
				 Char == ")" ||
				 Char == "(" ||
				 Char == "*" ||
				 Char == "+" ||
				 Char == "<" ||
				 Char == ">" ||
				 Char == "@" ||
				 Char == "[" ||
				 Char == "]" ||
				 Char == "&" ||
				 Char == " " ||		// Space
				 Char == "	" ||	// Tab
				 Char == "")
		{
			if (Char == " ")
			{
				if (!IS_CONTROL_FUNCTION_NAME)
				{
					ErrorCode("حرف غير مقبول : ' " + Char + " ' داخل ' " + Name + " ' ", o_tokens);
					return false; // Exception !
				}

				IS_CONTROL_FUNCTION_NAME = false;
				return true;
			}

			ErrorCode("حرف غير مقبول : ' " + Char + " ' داخل ' " + Name + " ' ", o_tokens);
			return false;
		}

		First = false;
	} // End char loop.

	for (int i = 0; i <= ALIF_RESERVED_TOTAL; i++)
	{
		if (ALIF_RESERVED[i] == Name)
		{
			//if (Name == "رئيسية" && IsInsideClass)
			//continue;

			ErrorCode("عفوا، هذا الاسم محجوز في لغة ألف : ' " + Name + " ' ", o_tokens);
		}
	}

	IS_CONTROL_FUNCTION_NAME = false;
	return true;
}

// ====================================================

void ADD_FUN(bool GLOBAL, string WIN_NAME, string FUN_NAME, string TYPE, int Line, CLASS_TOKEN *o_tokens)
{
	if (!IsValidName(FUN_NAME, o_tokens))
		ErrorCode("اسم غير مقبول : ' " + FUN_NAME + " ' ", o_tokens);

	// if same name as Window !
	if (WIN_NAME == FUN_NAME)
		ErrorCode("تشابه في الاسم بين الدالة و النافدة ' " + FUN_NAME + " ' ", o_tokens);

	if (GLOBAL)
	{
		// if already exist global fun
		if (G_FUN_IS_SET[(FUN_NAME)])
		{
			ErrorCode("الدالة العامه ' " + FUN_NAME + " ' تم انشاؤها مسبقا في السطر : " + G_FUN_AT_LINE[(FUN_NAME)], o_tokens);
		}

		// if already any other type of global var exist
		CheckForSameGlobalID(FUN_NAME, o_tokens);

		G_FUN_TYPE[(FUN_NAME)] = TYPE;
		G_FUN_IS_SET[(FUN_NAME)] = true;
		G_FUN_AT_LINE[(FUN_NAME)] = IntToString(Line);

		Global_FunctionNames[Global_TotalFucntion] = FUN_NAME;
		Global_TotalFucntion++;

		if (TYPE == "عادم")
			RETURN_FUN[std::make_pair("", FUN_NAME)] = "OK";

		// *** Generate Code ***
		SET_GLOBAL_C_NAME(FUN_NAME);
		// *** *** *** *** *** ***
	}
	else
	{
		// if already exist local fun
		if (L_FUN_IS_SET[std::make_pair(WIN_NAME, FUN_NAME)])
		{
			ErrorCode("الدالة ' " + FUN_NAME + " ' تم انشاؤها مسبقا في السطر : " +
							  L_FUN_AT_LINE[std::make_pair(WIN_NAME, FUN_NAME)],
						  o_tokens);
		}

		L_FUN_TYPE[std::make_pair(WIN_NAME, FUN_NAME)] = TYPE;
		L_FUN_IS_SET[std::make_pair(WIN_NAME, FUN_NAME)] = true;
		L_FUN_AT_LINE[std::make_pair(WIN_NAME, FUN_NAME)] = IntToString(Line);

		if (TYPE == "عادم")
			RETURN_FUN[std::make_pair(WIN_NAME, FUN_NAME)] = "OK";

		// *** Generate Code ***
		SET_C_NAME(FUN_NAME);
		// *** *** *** *** *** ***
	}
}

void SetNewVar(	bool IsGlobal, 
				string TmpWindow, 
				string TmpFunction, 
				string VarName, 
				string VarDataType, 
				bool IsConstant, 
				bool IsArray,
				int Line, 
				CLASS_TOKEN *o_tokens){
	
	if (!IsValidName(VarName, o_tokens))
		ErrorCode("اسم غير مقبول : ' " + VarName + " ' ", o_tokens);
	
	if (TmpFunction == VarName)
		ErrorCode("تشابه في الاسم بين المتغير و الدالة ' " + VarName + " ' ", o_tokens);

	if (IsGlobal)
	{
		if (G_VAR_IS_SET[(VarName)])
			ErrorCode("المتغير العام ' " + VarName + " ' تم انشاؤه مسبقا في السطر : " + G_VAR_AT_LINE[(VarName)], o_tokens);

		CheckForSameGlobalID(VarName, o_tokens);

		G_VAR_TYPE[(VarName)] = VarDataType;
		G_VAR_IS_SET[(VarName)] = true;
		G_VAR_AT_LINE[(VarName)] = IntToString(Line);
		G_VAR_IS_CONST[(VarName)] = IsConstant;
		G_VAR_IS_ARRAY[(VarName)] = IsArray;
		G_VAR_NAMES[G_VAR_TOTAL] = VarName;
		G_VAR_TOTAL++;

		// *** Generate Code ***
		SET_GLOBAL_C_NAME(VarName);
		// *** *** *** *** *** ***
	}
	else
	{
		if (L_VAR_IS_SET[std::make_pair(TmpWindow + TmpFunction, VarName)])
		{
			ErrorCode(	"المتغير ' " + VarName + " ' تم انشاؤه مسبقا في السطر : " + 
						L_VAR_AT_LINE[std::make_pair(TmpWindow + TmpFunction, VarName)], o_tokens);
		}

		L_VAR_TYPE[std::make_pair(TmpWindow + TmpFunction, VarName)] = VarDataType;
		L_VAR_IS_SET[std::make_pair(TmpWindow + TmpFunction, VarName)] = true;
		L_VAR_AT_LINE[std::make_pair(TmpWindow + TmpFunction, VarName)] = IntToString(Line);
		L_VAR_IS_CONST[std::make_pair(TmpWindow + TmpFunction, VarName)] = IsConstant;
		L_VAR_IS_ARRAY[std::make_pair(TmpWindow + TmpFunction, VarName)] = IsArray;

		// *** Generate Code ***
		SET_C_NAME(VarName);
		// *** *** *** *** *** ***
	}
}

void SetNewVarClass( bool IsGlobal, 
					 bool IsPrivate, 
					 string ClassName, 
					 string FunctionName, 
					 string VarName, 
					 string VarDataType, 
					 bool IsConstant, 
					 bool IsArray,
					 int Line,
					 CLASS_TOKEN *o_tokens){

	if (!IsValidName(VarName, o_tokens))
		ErrorCode("اسم غير مقبول : ' " + VarName + " ' ", o_tokens);

	if (!IsGlobal && IsPrivate)
		ErrorCode("يجب استخدام ' خاص ' خارج الدالة", o_tokens);
	
	if (ClassName == VarName)
		ErrorCode("تشابه في الاسم بين المتغير و الصنف ' " + VarName + " ' ", o_tokens);

	if (IsGlobal)
	{
		if (CLASS_G_VAR_IS_SET[std::make_pair(ClassName, VarName)])
		{
			ErrorCode(	"المتغير العام ' " + VarName + " ' تم انشاؤه مسبقا في السطر : " +
						CLASS_G_VAR_AT_LINE[std::make_pair(ClassName, VarName)], o_tokens);
		}

		/*
		if (G_VAR_IS_SET[(VarName)])
		{
			ErrorCode("المتغير العام ' " + VarName + " ' تم انشاؤه مسبقا في السطر : " +
					  CLASS_G_VAR_AT_LINE[std::make_pair(ClassName, VarName)], o_tokens);
		}
		*/

		CLASS_G_VAR_TYPE[std::make_pair(ClassName, VarName)] = VarDataType;
		CLASS_G_VAR_IS_SET[std::make_pair(ClassName, VarName)] = true;
		CLASS_G_VAR_AT_LINE[std::make_pair(ClassName, VarName)] = IntToString(Line);
		CLASS_G_VAR_PRIVATE[std::make_pair(ClassName, VarName)] = IsPrivate;
		CLASS_G_VAR_IS_CONST[std::make_pair(ClassName, VarName)] = IsConstant;
		CLASS_G_VAR_IS_ARRAY[std::make_pair(ClassName, VarName)] = IsArray;

		// *** Generate Code ***
		SET_GLOBAL_C_NAME(VarName);
		// *** *** *** *** *** ***
	}
	else
	{
		if (L_VAR_IS_SET[std::make_pair(ClassName + FunctionName, VarName)])
		{
			ErrorCode(	"المتغير ' " + VarName + " ' تم انشاؤه مسبقا في السطر : " +
						L_VAR_AT_LINE[std::make_pair(ClassName + FunctionName, VarName)], o_tokens);
		}

		L_VAR_TYPE[std::make_pair(ClassName + FunctionName, VarName)] = VarDataType;
		L_VAR_IS_SET[std::make_pair(ClassName + FunctionName, VarName)] = true;
		L_VAR_AT_LINE[std::make_pair(ClassName + FunctionName, VarName)] = IntToString(Line);
		L_VAR_IS_CONST[std::make_pair(ClassName + FunctionName, VarName)] = IsConstant;
		L_VAR_IS_ARRAY[std::make_pair(ClassName + FunctionName, VarName)] = IsArray;

		// *** Generate Code ***
		SET_C_NAME(VarName);
		// *** *** *** *** *** ***
	}
}

string GetSyntaxDataType(string Token[1024], int Position, CLASS_TOKEN *o_tokens){

	// Token[Position] -> '='

	if (IsValidVar(Token[Position + 1], o_tokens))
		return IsValidVar_Type;

	return {};
}

void ADD_FUN_CLASS(bool PRIVATE, string CLASS_NAME, string FUN_NAME, string TYPE, int Line, CLASS_TOKEN *o_tokens)
{
	if (!IsValidName(FUN_NAME, o_tokens))
		ErrorCode("اسم غير مقبول : ' " + FUN_NAME + " ' ", o_tokens);

	// if already exist class fun
	if (CLASS_FUN_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)])
	{
		ErrorCode("الصنف الدالة ' " + FUN_NAME + " ' تم انشاؤها مسبقا في السطر : " +
						CLASS_FUN_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)], o_tokens);
	}

	// if already same Generated_ID with global class var.
	if (CLASS_G_VAR_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)])
	{
		ErrorCode("تشابه في الاسم مع متغير ' " + FUN_NAME + " ' في السطر : " + 
						CLASS_G_VAR_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)] + " ", o_tokens);
	}
	//else
		//if(DEBUG)DEBUG_MESSAGE("---C " + CLASS_NAME + " :F " + FUN_NAME + " E---", o_tokens); // DEBUG

	// if same name as class !
	if (CLASS_NAME == FUN_NAME)
		ErrorCode("تشابه في الاسم بين الدالة و الصنف ' " + FUN_NAME + 
					  " '، على العموم إن كنت تقصد دالة بناء الصنف استخدم التعريف ' بناء '", o_tokens);

	CLASS_FUN_TYPE[std::make_pair(CLASS_NAME, FUN_NAME)] = TYPE;
	CLASS_FUN_IS_SET[std::make_pair(CLASS_NAME, FUN_NAME)] = true;
	CLASS_FUN_AT_LINE[std::make_pair(CLASS_NAME, FUN_NAME)] = IntToString(Line);
	CLASS_FUN_PRIVATE[std::make_pair(CLASS_NAME, FUN_NAME)] = PRIVATE;

	if (TYPE == "عادم")
		RETURN_FUN[std::make_pair(CLASS_NAME, FUN_NAME)] = "OK";

	// *** Generate Code ***
	SET_C_NAME(FUN_NAME);
	// *** *** *** *** *** ***
}

// ====================================================

// Here, was void ADD_VAR() / void ADD_VAR_CLASS()
// Now, we use NewVar.cpp

// ====================================================

string C_LAST_ARG;

string CHECK_NEW_FUN_SYNTAX(bool GLOBAL,
							string SYNTAX[1024],
							int SYNTAX_LONG,
							string TmpWindow, // fun1 { a = b + win:fun2(x) + z }
							string TmpFunction, // fun1 { a = b + win:fun2(x) + z }
							CLASS_TOKEN *o_tokens)
{
	// function (int a, نص b)

	// SYNTAX : عدد 	| SYNTAX[3] : string
	// SYNTAX[1] : a	| SYNTAX[4] : b
	// SYNTAX[2] : ,	| SYNTAX[5] : )

	int TYPE = 0, VAR = 1, COMMA = 2;

	string CPP_CODE;
	NEW_FUNCTION_ARG = "";

	for (int p = 0; p <= SYNTAX_LONG; p += 3)
	{
		if (!o_tokens->TOKENS_PREDEFINED)
		{
			// Tokens not predifined, so we need only to set ARGs

			if (SYNTAX[TYPE] != "عدد" && SYNTAX[TYPE] != "نص" && SYNTAX[TYPE] != "منطق")
			{
				ErrorCode("نوع غير مقبول ' " + SYNTAX[TYPE] + " ' ", o_tokens);
			}
			
			if (!IsValidName(SYNTAX[VAR], o_tokens))
				ErrorCode("اسم غير مقبول ' " + SYNTAX[VAR] + " ' ", o_tokens);

			if (L_VAR_IS_SET[std::make_pair(TmpWindow + TmpFunction, SYNTAX[VAR])]) // الدالة (int a, نص a)
				ErrorCode("متغير محلي موجود مسبقا ' " + SYNTAX[VAR] + " ' ", o_tokens);

			if (SYNTAX[COMMA] != "،" && SYNTAX[COMMA] != "," && SYNTAX[COMMA] != ")")
				ErrorCode("فاصله غير صحيحه ' " + SYNTAX[COMMA] + " ' ", o_tokens);

			if (SYNTAX[COMMA] == ")" && COMMA < SYNTAX_LONG)
				ErrorCode("غير مكتمل ' " + IntToString(SYNTAX_LONG) + " ' ", o_tokens);

			//if(DEBUG)DEBUG_MESSAGE("[" + SYNTAX[TYPE] + "|" + SYNTAX[VAR] + "]", o_tokens); // DEBUG

			// add always-local (bcs this var is in arg..) VAR to fun
			SetNewVar(false, TmpWindow, TmpFunction, SYNTAX[VAR], SYNTAX[TYPE], false, false, o_tokens->Line, o_tokens);

			// add local ARG-VAR as ARGUMENT to fun
			if (GLOBAL)
			{
				G_FUN_ARG_TYPE[std::make_pair(TmpFunction, G_FUN_ARG_TOTAL[TmpFunction])] = SYNTAX[TYPE];
				G_FUN_ARG_TOTAL[TmpFunction]++;
			}
			else
			{
				L_FUN_ARG_TYPE[std::make_pair(TmpWindow + TmpFunction, L_FUN_ARG_TOTAL[std::make_pair(TmpWindow, TmpFunction)])] = SYNTAX[TYPE];
				L_FUN_ARG_TOTAL[std::make_pair(TmpWindow, TmpFunction)]++;
			}

			TYPE += 3, VAR += 3, COMMA += 3; // For ARG non-predifined loop
		}
		else
		{
			// Tokens already predifined, so we need show log, and return C++ code
			
			if (SYNTAX[TYPE] == "عدد")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[INT " + SYNTAX[VAR] + "]", o_tokens); // DEBUG

				// *** Generate Code ***
				// always need pointer to one elem array
				CPP_CODE.append(" double " + ID[SYNTAX[VAR]] + " ");
				NEW_FUNCTION_ARG.append(" double " + ID[SYNTAX[VAR]] + " ");
				// *** *** *** *** *** ***
			}
			else if (SYNTAX[TYPE] == "نص")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[STRING " + SYNTAX[VAR] + "]", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" wxString " + ID[SYNTAX[VAR]] + " ");
				NEW_FUNCTION_ARG.append(" wxString " + ID[SYNTAX[VAR]] + " ");
				// *** *** *** *** *** ***

			}
			else if (SYNTAX[TYPE] == "منطق")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[BOOL " + SYNTAX[VAR] + "]", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" bool " + ID[SYNTAX[VAR]] + " ");
				NEW_FUNCTION_ARG.append(" bool " + ID[SYNTAX[VAR]] + " ");
				// *** *** *** *** *** ***
			}
			else
			{
				// Exception !
				ErrorCode("نوع غير مقبول ' " + SYNTAX[TYPE] + " ' ", o_tokens);
			}

			TYPE += 3, VAR += 3, COMMA += 3; // For ARG predifined / C++ loop, and need by comma

			// *** Generate Code ***
			if (COMMA <= SYNTAX_LONG)
			{
				CPP_CODE.append(" , ");
				NEW_FUNCTION_ARG.append(" , ");				
			}
			// *** *** *** *** *** ***
		}
	}

	if (o_tokens->TOKENS_PREDEFINED)
		return CPP_CODE;
	else
		return "";
}

// ====================================================

string CheckForSyntax(string OBJECTIF_TYPE,		// OBJECTIF_TYPE
					bool ACCEPT_REF_WIN_WIDGET, // Accept Using Reference إلى Window:Controls
					bool ACCEPT_REF_WIN_FUN,	// Accept Using Reference إلى Window:Function
					bool ACCEPT_REF_GLOBAL_FUN, // Accept Using Reference إلى Global Functions
					bool ACCEPT_REF_LOCAL_FUN,  // Accept Using Reference إلى Local Functions
					bool ACCEPT_REF_GLOBAL_VAR, // Accept Using Reference إلى Global VAR
					bool ACCEPT_REF_LOCAL_VAR,  // Accept Using Reference إلى Local VAR
					bool ACCEPT_STR_TO_INT,		// Accept Convertion من نص إلى Int
					bool ACCEPT_INT_TO_STRING,  // Accept Convertion من عدد إلى String
					string SYNTAX[1024],		// SYNTAX[] string
					int SYNTAX_LONG,			// SYNTAX_LONG int
					string TMP_WIN_OR_CLASS,	// a = b + win:fun(2+2) + class:fun(x)
					string TmpFunction,				// a = b + win/class:fun(2+2)
					CLASS_TOKEN *o_tokens)
{
	// Note : WX Crash if On creating using reference إلى Controls, BUT OK ل Functions.

	// SYNTAX = '='
	// SYNTAX[1] = ...

	// TmpFunction				: = a + b + TMP_FUN_NAME(X, Y, Z) + ...
	// TMP_WIN_OR_CLASS		: إلى get Generated_ID of tmp الدالة name
	// OBJECTIF_TYPE		: INT.ToString or STRING.ToInt

	string CPP_CODE;

	if (SYNTAX[0] != "=")
	{
		ErrorCode("بناء غير صحيح : علة : " + CONVERT_STRING_ARRAY_TO_STRING(SYNTAX, SYNTAX_LONG), o_tokens);
	}

	// Clear Syntax Array, needfor C++ Code _س_
	//for (int p = 0; p <= SYNTAX_LONG; p++)
	//TMP_SYNTAX[p].clear(); // SYNTAX[p] = ""; SYNTAX[p] = 0;

	for (int p = 1; p <= SYNTAX_LONG; p++)
	{
		// ----------------------
		// _س_ / _ج_
		// ----------------------

		if (SYNTAX[p] == "_ج_"){
			ErrorCode("لا يمكن بأي شكل من الأشكال معرفة نوع البيانات الناتجة من شفرة جافا سكريبت عشوائية", o_tokens);
		}
		else if (SYNTAX[p] == "_س_")
		{
			// xyz = _س_ ...C++...@ Alif @...C++... _س_

			if(DEBUG)
				DEBUG_MESSAGE(" {_س_ Syntax START} ", o_tokens); // DEBUG

			bool C_FOUND = false;

			for (int z = p + 1; z <= SYNTAX_LONG; z++)
			{
				if (SYNTAX[z] == "_س_") // End C++ Code
				{
					if(DEBUG)
						DEBUG_MESSAGE(" {_س_ Syntax END} ", o_tokens); // DEBUG
					C_FOUND = true;
					p = z;
					break;
				}
				else if (SYNTAX[z] == "@") // @ Start ...
				{
					// xyz = ...C++...@ Alif @...C++...

					if(DEBUG)
						DEBUG_MESSAGE(" {Syntax@} ", o_tokens); // DEBUG

					string TempToken[1024];
					TempToken[0] = "=";	  // CheckForSyntax() Need this.
					int TempTokenCount = 1; // CheckForSyntax() Need this.

					bool AT_FOUND = false;

					for (int c = z + 1; c < SYNTAX_LONG; c++)
					{
						if (SYNTAX[c] == "@")
						{
							z = c;
							AT_FOUND = true;
							break;
						}

						TempToken[TempTokenCount] = SYNTAX[c];
						TempTokenCount++;
					}

					if (!AT_FOUND)
						ErrorCode("نهايه شفرة غير موجود داخل البناء ' @ '", o_tokens);

					string SYNTAX_BUFFER = CheckForSyntax("C++",				// OBJECTIF_TYPE
														true,				// Accept Using Reference إلى Window:Controls
														true,				// Accept Using Reference إلى Window:Function
														true,				// Accept Using Reference إلى Global Functions
														true,				// Accept Using Reference إلى Local Functions
														true,				// Accept Using Reference إلى Global VAR
														true,				// Accept Using Reference إلى Local VAR
														true,				// Accept Convertion من نص إلى Int
														true,				// Accept Convertion من عدد إلى String
														TempToken,				// SYNTAX[] string
														(TempTokenCount - 1), // SYNTAX_LONG int
														TheWindow,		// TMP_WINDOW_NAME
														TheFunction,		// TMP_FUNCTION_NAME
														o_tokens);

					if(DEBUG)
						DEBUG_MESSAGE(" {Syntax@} ", o_tokens); // DEBUG

					// *** C++ ***
					CPP_CODE.append(" " + SYNTAX_BUFFER + " ");
					// *** *** *** *** *** ***

					// @ End.
				}
				else if (SYNTAX[z] != "")
				{
					CPP_CODE.append(SYNTAX[z]);
					if(DEBUG)
						DEBUG_MESSAGE(" {" + SYNTAX[z] + "} ", o_tokens); // DEBUG
				}
			}

			if (C_FOUND)
				continue; // Point إلى next بعد _س_
			else
				ErrorCode("نهايه شفرة غير موجود داخل البناء ' _س_ '", o_tokens);
		}

		// ----------------------
		// OPERATION
		// ----------------------

		if (SYNTAX[p] == "+" ||
			SYNTAX[p] == "-" ||
			SYNTAX[p] == "*" ||
			SYNTAX[p] == "/")
		{
			if (p > 0)
			{
				if (!CAN_ADD_OPERATION_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة عملية هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);				
			}

			if (p >= SYNTAX_LONG){
				if(DEBUG)DEBUG_MESSAGE("######################", o_tokens); // DEBUG
				for (int p = 1; p <= SYNTAX_LONG; p++)
					if(DEBUG)DEBUG_MESSAGE("[" + SYNTAX[p] + "] ", o_tokens); // DEBUG
				ErrorCode("لا يمكن الإنتهاء بعملية ' " + SYNTAX[p] + " ' ", o_tokens);
			}

			if (OBJECTIF_TYPE == "نص") // only acceptable operation in نص is '+'
			{
				if (SYNTAX[p] != "+")
					ErrorCode("لا يمكن إضافة عملية داخل نص ' " + SYNTAX[p] + " ' ", o_tokens);				
			}

			if (OBJECTIF_TYPE == "منطق")
				ErrorCode("لا يمكن أجراء عملية في متغير منطقي ' " + SYNTAX[p] + " ' ", o_tokens);

			if (substr_utf8(SYNTAX[p - 1], 0, 1) == "\"" && substr_utf8(SYNTAX[p + 1], 0, 1) == "\"") // "test" + "hello"
				ErrorCode("لا يمكن إضافة نص إلى نص ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " " + SYNTAX[p + 1] + " ' ", o_tokens);

			if(DEBUG)DEBUG_MESSAGE("[OPERATION " + SYNTAX[p] + "] ", o_tokens); // DEBUG

			// *** Generate Code ***
			CPP_CODE.append(" " + SYNTAX[p] + " ");
			// *** *** *** *** *** ***
		}

		// ----------------------
		// قوس OPEN
		// ----------------------

		else if (SYNTAX[p] == "(")
		{
			if (p > 0)
				if (!CAN_ADD_PARENTHESIS_OPEN_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة قوس مفتوح هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			ALIF_PARENTHESIS_STATUS++;

			if(DEBUG)
				DEBUG_MESSAGE("[PARENTHESIS Open (] ", o_tokens); // DEBUG

			// *** Generate Code ***
			CPP_CODE.append(" ( ");
			// *** *** *** *** *** ***
		}

		// ----------------------
		// قوس CLOSE
		// ----------------------

		else if (SYNTAX[p] == ")")
		{
			if (ALIF_PARENTHESIS_STATUS < 1)
				ErrorCode("غلق قوس ليس مفتوحا ' " + SYNTAX[p] + " ' ", o_tokens);

			if (p > 0)
				if (!CAN_ADD_PARENTHESIS_CLOSE_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة غلق قوس هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			ALIF_PARENTHESIS_STATUS--;

			if(DEBUG)
				DEBUG_MESSAGE("[PARENTHESIS CLOSE )] ", o_tokens); // DEBUG

			// *** Generate Code ***
			CPP_CODE.append(" ) ");
			// *** *** *** *** *** ***
		}

		// ----------------------
		// Global VARIABLE (all type)
		// Global VARIABLE without _
		// ----------------------

		else if (substr_utf8(SYNTAX[p], 0, 1) == "_" || G_VAR_IS_SET[(SYNTAX[p])])
		{
			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			string G_VAR_WITHOUT_ ;

			if (G_VAR_IS_SET[(SYNTAX[p])])
				G_VAR_WITHOUT_ = SYNTAX[p];
			else
				G_VAR_WITHOUT_ = substr_utf8(SYNTAX[p], 1, CharCount_utf8(SYNTAX[p], o_tokens));

			if (!G_VAR_IS_SET[(G_VAR_WITHOUT_)])
				ErrorCode("ليس متغير عام ' " + SYNTAX[p] + " ' ", o_tokens);

			if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "عدد")
			{
				if (OBJECTIF_TYPE == "عدد")
				{
					if(DEBUG)
						DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
					// *** *** *** *** *** ***
				}
				else if (OBJECTIF_TYPE == "نص")
				{
					if (ACCEPT_INT_TO_STRING)
					{
						if(DEBUG)
							DEBUG_MESSAGE("[GLOBAL-INT (" + G_VAR_WITHOUT_ + ").ToString] ", o_tokens); // DEBUG

						// *** Generate Code ***
						CPP_CODE.append(" alifcore_IntToString(" + Global_ID[G_VAR_WITHOUT_] + ") ");
						// *** *** *** *** *** ***
					}
					else
					{
						ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ", o_tokens);
					}
				}
				else if (OBJECTIF_TYPE == "C++")
				{
					if(DEBUG)
						DEBUG_MESSAGE("[C++ Global عدد (" + G_VAR_WITHOUT_ + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
				}
			}
			else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "نص")
			{
				if (OBJECTIF_TYPE == "عدد")
				{
					if (ACCEPT_INT_TO_STRING) // or ACCEPT_STR_TO_INT !!
					{
						//if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" + G_VAR_WITHOUT_ + ").ToInt] ", o_tokens); // DEBUG
						ErrorCode("لا يمكن تحويل نص إلى عدد ' " + G_VAR_WITHOUT_ + " ' ", o_tokens);
					}
					else
					{
						ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ", o_tokens);
					}
				}
				else if (OBJECTIF_TYPE == "نص")
				{
					if(DEBUG)
						DEBUG_MESSAGE("[GLOBAL-STRING (" + G_VAR_WITHOUT_ + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
					// *** *** *** *** *** ***
				}
				else if (OBJECTIF_TYPE == "C++")
				{
					if(DEBUG)
						DEBUG_MESSAGE("[C++ Global نص (" + G_VAR_WITHOUT_ + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
				}
			}
			else if (G_VAR_TYPE[(G_VAR_WITHOUT_)] == "منطق")
			{
				if (OBJECTIF_TYPE != "منطق")
				{
					ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " + G_VAR_WITHOUT_ + " ' ", o_tokens);
				}

				if(DEBUG)DEBUG_MESSAGE("[GLOBAL-BOOL ' " + G_VAR_WITHOUT_ + " '] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + Global_ID[G_VAR_WITHOUT_] + " ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("علة : نوع المتغير العام غير معروف ' " + G_VAR_TYPE[(G_VAR_WITHOUT_)] + " ' ل ' " + G_VAR_WITHOUT_ + " ' ",o_tokens);
			}
		}

		// ----------------------
		// Local VARIABLE INT
		// ----------------------

		else if (L_VAR_TYPE[std::make_pair(TMP_WIN_OR_CLASS + TmpFunction, SYNTAX[p])] == "عدد")
		{
			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			if (OBJECTIF_TYPE == "عدد")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[LOCAL-INT (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
				// *** *** *** *** *** ***
			}
			else if (OBJECTIF_TYPE == "نص")
			{
				if (ACCEPT_INT_TO_STRING)
				{
					if(DEBUG)
						DEBUG_MESSAGE("[LOCAL-INT (" + SYNTAX[p] + ").ToString] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" alifcore_IntToString(" + ID[SYNTAX[p]] + ") ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ", o_tokens);
				}
			}
			else if (OBJECTIF_TYPE == "C++")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[C++ Local INT (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}

		// -----------------------------------
		// Local VARIABLE STRING
		// -----------------------------------

		else if (L_VAR_TYPE[std::make_pair(TMP_WIN_OR_CLASS + TmpFunction, SYNTAX[p])] == "نص")
		{
			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			if (OBJECTIF_TYPE == "عدد")
			{
				//if(DEBUG)DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] + ").ToInt] ", o_tokens); // DEBUG
				ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ", o_tokens);
			}
			else if (OBJECTIF_TYPE == "نص")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
				// *** *** *** *** *** ***
			}
			else if (OBJECTIF_TYPE == "C++")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[C++ Local STRING (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}

		// -----------------------------------
		// Local VARIABLE Bool
		// -----------------------------------

		else if (L_VAR_TYPE[std::make_pair(TMP_WIN_OR_CLASS + TmpFunction, SYNTAX[p])] == "منطق")
		{
			//if (p != 1)
			//{
				//ErrorCode("أمر غير معروف ' " + SYNTAX[p] + " ' ", o_tokens);
			//}
			
			if (SYNTAX[p + 1] != "")
			{
				// bool A = B + wrong
				ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " '، على العموم لا يمكن أجراء أية عملية على متغير منطقي ", o_tokens);
			}
			else if (OBJECTIF_TYPE == "C++")
			{
				if(DEBUG)DEBUG_MESSAGE("[C++ Local Bool (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
				// *** *** *** *** *** ***

				continue;
			}
			else if (OBJECTIF_TYPE != "منطق")
			{
				ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " + SYNTAX[p] + " ' ", o_tokens);
			}
			//else
			//{
				//ErrorCode("علة : استثناء في متغير منطقي ' " + SYNTAX[p] + " ' ", o_tokens);
			//}

			if(DEBUG)DEBUG_MESSAGE("[LOCAL-BOOL ' " + SYNTAX[p] + " '] ", o_tokens); // DEBUG

			// *** Generate Code ***
			CPP_CODE.append(" " + ID[SYNTAX[p]] + " ");
			// *** *** *** *** *** ***
		}

		// -----------------------------------
		// True / False
		// -----------------------------------

		else if (SYNTAX[p] == "صحيح")
		{
			if (p != 1)
				ErrorCode("لا يمكن وضع ( منطق ) هنا ' " + SYNTAX[p] + " ' ", o_tokens);

			if (SYNTAX[p + 1] != "")
			{
				// bool A = TRUE + wrong
				ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " '، على العموم لا يمكن أجراء أية عملية على متغير منطقي ", o_tokens);
			}

			if (OBJECTIF_TYPE == "منطق")
			{
				if(DEBUG)DEBUG_MESSAGE("[TRUE] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" true ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}
		else if (SYNTAX[p] == "خطأ")
		{
			if (p != 1)
				ErrorCode("لا يمكن وضع ( منطق ) هنا ' " + SYNTAX[p] + " ' ", o_tokens);
			
			if (SYNTAX[p + 1] != "")
			{
				// bool A = FALSE + wrong
				ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " '، على العموم لا يمكن أجراء أية عملية على متغير منطقي ", o_tokens);
			}

			if (OBJECTIF_TYPE == "منطق")
			{
				if(DEBUG)DEBUG_MESSAGE("[FALSE] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" false ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}

		// -----------------------------------
		// New Line سطر
		// -----------------------------------

		else if (SYNTAX[p] == "سطر")
		{
			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة سطر جديد هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			if (OBJECTIF_TYPE == "عدد")
			{
				//if(DEBUG)DEBUG_MESSAGE("[LOCAL-STRING (" + SYNTAX[p] + ").ToInt] ", o_tokens); // DEBUG
				ErrorCode("لا يمكن تحويل سطر جديد إلى عدد ' " + SYNTAX[p] + " ' ", o_tokens);
			}
			else if (OBJECTIF_TYPE == "نص")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[NewLine] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" ALIFCORE_NEW_LINE ");
				// *** *** *** *** *** ***
			}
			else if (OBJECTIF_TYPE == "منطق")
			{
				ErrorCode("لا يمكن تحويل سطر جديد إلى منطق ' " + SYNTAX[p] + " ' ", o_tokens);
			}
			else if (OBJECTIF_TYPE == "C++")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[C++_NewLine] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" wxT(\"\n\") ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}

		// ----------------------
		// Class Global/Local Obj
		// ----------------------

		else if (OBJ_IS_SET[std::make_pair(TMP_WIN_OR_CLASS + TmpFunction, SYNTAX[p])] ||
				 OBJ_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])] ||
				 OBJ_IS_SET[std::make_pair("", SYNTAX[p])])
		{
			// a = obj:mem + c
			// b = obj:mem_func(a, b) + c

			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة كائن هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			//if (SYNTAX[p + 1] != ":")
				//ErrorCode("يجب إضافة ':' بعد ' " + SYNTAX[p] + " ' ", o_tokens);
			//if (SYNTAX[p + 2] == "")
				//ErrorCode("يجب إضافة إسم منتمي للصنف بعد ' " + SYNTAX[p] + " : ' ", o_tokens);
			
			if (SYNTAX[p + 1] != ":")
				ErrorCode("يجب وضع نقطتين ':' بين الكائن و المنتمي، أي بعد ' " + SYNTAX[p] + " ' ", o_tokens);
			if (SYNTAX[p + 2] == "بناء")
				ErrorCode("لا يمكن استدعاء دالة ' بناء() '، هته الدالة تستدعى بشكل آلي عند إنشاء الكائن", o_tokens);
			if (SYNTAX[p + 2] == "هدم")
				ErrorCode("لا يمكن استدعاء دالة ' هدم() '، هته الدالة تستدعى بشكل آلي عند الحاجة إلى تدمير هذا الكائن", o_tokens);
			if (SYNTAX[p + 2] == "")
				ErrorCode("يجب وضع اسم المنتمي بعد ' " + SYNTAX[p] + ":' ", o_tokens);

			// C++, allow create Obj on global, global-class, local, but using it only on func.
			string OBJ_ID;
			bool IS_GLOBAL_OBJ = false;
			if (OBJ_IS_SET[std::make_pair(TMP_WIN_OR_CLASS + TmpFunction, SYNTAX[p])])
			{
				// Local Obj.
				// Window -> Func -> Obj. Or
				// Class  -> Func -> Obj.
				OBJ_ID = TMP_WIN_OR_CLASS + TmpFunction;
			}
			else if (OBJ_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])])
			{
				// Global Class Obj.
				// Class -> Obj.
				OBJ_ID = TMP_WIN_OR_CLASS;
			}
			else
			{
				// Global Obj.
				// Global-Area -> Obj.
				OBJ_ID = "";
				IS_GLOBAL_OBJ = true;
			}

			string TK_CLASS = OBJ_CLASS[std::make_pair(OBJ_ID, SYNTAX[p])];

			if (!CLASS_G_VAR_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])] &&
				!CLASS_FUN_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
				ErrorCode("الصنف ' " + TK_CLASS + " ' ليس فيه أي منتمي معرف باسم ' " + SYNTAX[p + 2] + " ' ", o_tokens);

			string MEMBER_TYPE;

			if (CLASS_G_VAR_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
			{
				// متغير member

				// a = obj : mem + c

				if (CLASS_G_VAR_PRIVATE[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
					ErrorCode("لا يمكن استدعاء المتغير ' " + SYNTAX[p + 2] + " ' المنتمي للصنف ' " + TK_CLASS + " ' لأنه من نوع ' خاص ' ", o_tokens);

				MEMBER_TYPE = CLASS_G_VAR_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])];

				if (MEMBER_TYPE == "عادم")
					ErrorCode("لا يمكن إضافة منتمي عادم ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + " ' ", o_tokens);

				if (MEMBER_TYPE == "عدد")
				{
					if (OBJECTIF_TYPE == "عدد")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(INT)] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + " ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(INT)] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + " ");
							// *** *** *** *** *** ***
						}
					}
					else if (OBJECTIF_TYPE == "نص")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(INT).ToString] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" alifcore_IntToString(" + GlobalObj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + ") ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(INT).ToString] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" alifcore_IntToString(" + Obj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + ") ");
							// *** *** *** *** *** ***
						}
					}
					else if (OBJECTIF_TYPE == "منطق")
					{
						ErrorCode("لا يمكن تحويل عدد إلى منطق ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " ' ", o_tokens);
					}
					else
					{
						ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
					}
				}
				else if (MEMBER_TYPE == "نص")
				{
					if (OBJECTIF_TYPE == "عدد")
					{
						ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ", o_tokens);
					}
					else if (OBJECTIF_TYPE == "نص")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(STRING)] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + " ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(STRING)] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + " ");
							// *** *** *** *** *** ***
						}
					}
					else if (OBJECTIF_TYPE == "منطق")
					{
						ErrorCode("لا يمكن تحويل نص إلى منطق ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " ' ", o_tokens);
					}
					else
					{
						ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
					}
				}
				else if (MEMBER_TYPE == "منطق")
				{
					if (OBJECTIF_TYPE != "منطق")
					{
						ErrorCode("لا يمكن تحويل منتمي منطق إلى " + OBJECTIF_TYPE + " ' " + SYNTAX[p] + " ' ", o_tokens);
					}
					else if (OBJECTIF_TYPE == "منطق")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(BOOL)] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + " ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(BOOL)] ", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + "." + Global_ID[SYNTAX[p + 2]] + " ");
							// *** *** *** *** *** ***
						}
					}
					else
					{
						ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
					}
				}
				else
				{
					ErrorCode("علة : نوع المنتمي غير معروف ' " + MEMBER_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
				}

				p += 2; // point إلى next token بعد obj:mem
			}
			else if (CLASS_FUN_IS_SET[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
			{
				// Function member

				// b = obj : memf (a, b)

				if (CLASS_FUN_PRIVATE[std::make_pair(TK_CLASS, SYNTAX[p + 2])])
					ErrorCode("الدالة منتمي ' " + SYNTAX[p + 2] + " ' داخل الصنف ' " + TK_CLASS + " ' خاص ", o_tokens);

				if (CLASS_FUN_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])] == "عادم")
					ErrorCode("لا يمكن إضافة منتمي عادم ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + " ' داخل الصنف ' " + TK_CLASS + " ' ", o_tokens);

				if (SYNTAX[p + 3] != "(")
					ErrorCode("يجب إضافة '(' بعد ' " + SYNTAX[p] + ":" + SYNTAX[p + 2] + " ' ", o_tokens);

				MEMBER_TYPE = CLASS_FUN_TYPE[std::make_pair(TK_CLASS, SYNTAX[p + 2])];

				int TMP_FUN_LONG = p + 4; // a + b + 'p'obj : memf (a, b) + c
				int OPEN_PARENTIZE = 0;

				while (TMP_FUN_LONG <= SYNTAX_LONG)
				{
					if (SYNTAX[TMP_FUN_LONG] == "(") // مفتوح inside الدالة args : fun( a + (b))
						OPEN_PARENTIZE++;
					else if (SYNTAX[TMP_FUN_LONG] == ")" && OPEN_PARENTIZE > 0) // Close inside الدالة args
						OPEN_PARENTIZE--;
					else if (SYNTAX[TMP_FUN_LONG] == ")" && OPEN_PARENTIZE < 1) // Close final الدالة call
					{
						if (TMP_FUN_LONG < SYNTAX_LONG)
						{
							// a = fun( a + (b)) + 123
							// str / عدد = متغير + (fun(var)) * (fun(var) / fun(var, fun(var), var) - var)

							if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
								(SYNTAX[TMP_FUN_LONG + 1] != "-") &&
								(SYNTAX[TMP_FUN_LONG + 1] != "*") &&
								(SYNTAX[TMP_FUN_LONG + 1] != "/") &&
								(SYNTAX[TMP_FUN_LONG + 1] != "،") &&
								(SYNTAX[TMP_FUN_LONG + 1] != ",") &&
								(SYNTAX[TMP_FUN_LONG + 1] != ")"))
							{
								ErrorCode("نص غير معروف بعد نداء ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' : ' " + SYNTAX[TMP_FUN_LONG + 1] + " ' ", o_tokens);
							}
						}
						else if (TMP_FUN_LONG == SYNTAX_LONG)
						{
							// a = obj:fun( a + (b))
							if (SYNTAX[TMP_FUN_LONG] != ")" || SYNTAX[SYNTAX_LONG] != ")") // double check!
								ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' بالإشارة ')' ", o_tokens);
						}

						break;
					}

					TMP_FUN_LONG++;
				}

				if (SYNTAX[TMP_FUN_LONG] != ")") // Double check!
					ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' بالإشارة ')' ", o_tokens);

				string TempToken[1024];
				int TempTokenCount = 0;
				for (int i = p + 4; i <= TMP_FUN_LONG; i++)
				{
					if (SYNTAX[i] != "")
					{
						TempToken[TempTokenCount] = SYNTAX[i];
						TempTokenCount++;
					}
				}

				if (MEMBER_TYPE == "عدد")
				{
					if (OBJECTIF_TYPE == "عدد")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-INT) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-INT) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}

						CPP_CODE.append( CHECK_CALL_FUN_ARG(false, 
															TK_CLASS, 
															SYNTAX[p + 2], 
															2, // 2 = الدالة member
															TheWindow, 
															TheFunction, 
															TempToken, 
															(TempTokenCount - 1), 
															o_tokens));

						if(DEBUG)DEBUG_MESSAGE(")] ", o_tokens); // DEBUG

						// *** Generate Code ***
						CPP_CODE.append(" ) ");
						// *** *** *** *** *** ***

					}
					else if (OBJECTIF_TYPE == "نص")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-INT) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" alifcore_IntToString(" + GlobalObj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-INT) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" alifcore_IntToString(" + Obj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}

						CPP_CODE.append( CHECK_CALL_FUN_ARG(false, 
															TK_CLASS, 
															SYNTAX[p + 2], 
															2, // 2 = الدالة member
															TheWindow, 
															TheFunction, 
															TempToken, 
															(TempTokenCount - 1), 
															o_tokens));

						if(DEBUG)DEBUG_MESSAGE(").IntFunToString] ", o_tokens); // DEBUG

						// *** Generate Code ***
						CPP_CODE.append(" )) ");
						// *** *** *** *** *** ***
					}
					else
					{
						ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ", o_tokens);
					}
				}
				else if (MEMBER_TYPE == "نص")
				{
					if (OBJECTIF_TYPE == "عدد")
					{
						ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ", o_tokens);
					}
					else if (OBJECTIF_TYPE == "نص")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-STRING) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-STRING) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}

						CPP_CODE.append( CHECK_CALL_FUN_ARG(false, 
															TK_CLASS, 
															SYNTAX[p + 2], 
															2, // 2 = الدالة member
															TheWindow, 
															TheFunction, 
															TempToken, 
															(TempTokenCount - 1), 
															o_tokens));

						if(DEBUG)DEBUG_MESSAGE(")] ", o_tokens); // DEBUG

						// *** Generate Code ***
						CPP_CODE.append(" ) ");
						// *** *** *** *** *** ***
					}
					else
					{
						ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ", o_tokens);
					}
				}
				else if (MEMBER_TYPE == "منطق")
				{
					if (OBJECTIF_TYPE != "منطق")
					{
						ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ", o_tokens);
					}
					else if (OBJECTIF_TYPE == "منطق")
					{
						if (IS_GLOBAL_OBJ)
						{
							if(DEBUG)DEBUG_MESSAGE("[GLOBAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-BOOL) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + GlobalObj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}
						else
						{
							if(DEBUG)DEBUG_MESSAGE("[LOCAL-OBJ ' " + SYNTAX[p] + " ':'" + SYNTAX[p + 2] + " '(Func-BOOL) (", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" " + Obj_ID[SYNTAX[p]] + ".ClassFUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
							// *** *** *** *** *** ***
						}

						CPP_CODE.append( CHECK_CALL_FUN_ARG(false, 
															TK_CLASS, 
															SYNTAX[p + 2], 
															2, // 2 = الدالة member
															TheWindow, 
															TheFunction, 
															TempToken, 
															(TempTokenCount - 1), 
															o_tokens));

						if(DEBUG)DEBUG_MESSAGE(")] ", o_tokens); // DEBUG

						// *** Generate Code ***
						CPP_CODE.append(" ) ");
						// *** *** *** *** *** ***
					}
					else
					{
						ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ", o_tokens);
					}
				}
				else
				{
					ErrorCode("علة : نوع المنتمي غير معروف ' " + MEMBER_TYPE + " ' ل ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + "()' ", o_tokens);
				}

				p = TMP_FUN_LONG; // point إلى next token بعد fun(...)
			}
		}

		// ----------------------
		// Class Global VARIABLE
		// ----------------------

		// class
		//   g1 = 123
		//   g2 = g1<--

		else if (CLASS_G_VAR_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])] && IsInsideClass)
		{
			// Class Global area

			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة متغير هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			if (CLASS_G_VAR_TYPE[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])] == "عدد")
			{
				if (OBJECTIF_TYPE == "عدد")
				{
					if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-INT (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
					// *** *** *** *** *** ***
				}
				else if (OBJECTIF_TYPE == "نص")
				{
					if (ACCEPT_INT_TO_STRING)
					{
						if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-INT (" + SYNTAX[p] + ").ToString] ", o_tokens); // DEBUG

						// *** Generate Code ***
						CPP_CODE.append(" alifcore_IntToString(" + Global_ID[SYNTAX[p]] + ") ");
						// *** *** *** *** *** ***
					}
					else
					{
						ErrorCode("لا يمكن تحويل عدد إلى نص : ' " + SYNTAX[p] + " ' ", o_tokens);
					}
				}
				else if (OBJECTIF_TYPE == "منطق")
				{
					ErrorCode("لا يمكن تحويل عدد إلى منطق : ' " + SYNTAX[p] + " ' ", o_tokens);
				}
				else if (OBJECTIF_TYPE == "C++")
				{
					if(DEBUG)
						DEBUG_MESSAGE("[C++ CLASS-GLOBAL-INT (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
				}
			}
			else if (CLASS_G_VAR_TYPE[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])] == "نص")
			{
				if (OBJECTIF_TYPE == "عدد")
				{
					//if(DEBUG)DEBUG_MESSAGE("[GLOBAL-STRING (" + SYNTAX[p] + ").ToInt] ", o_tokens); // DEBUG
					ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ", o_tokens);
				}
				else if (OBJECTIF_TYPE == "نص")
				{
					if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-STRING (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
					// *** *** *** *** *** ***
				}
				else if (OBJECTIF_TYPE == "منطق")
				{
					ErrorCode("لا يمكن تحويل نص إلى منطق : ' " + SYNTAX[p] + " ' ", o_tokens);
				}
				else if (OBJECTIF_TYPE == "C++")
				{
					if(DEBUG)
						DEBUG_MESSAGE("[C++ CLASS-GLOBAL-STRING (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
				}
			}
			else if (CLASS_G_VAR_TYPE[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])] == "منطق")
			{
				if (OBJECTIF_TYPE == "C++")
				{
					if(DEBUG)
						DEBUG_MESSAGE("[C++ CLASS-GLOBAL-BOOL (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
					// *** *** *** *** *** ***
				}
				else if (OBJECTIF_TYPE != "منطق")
				{
					ErrorCode("لا يمكن تحويل منطق إلى " + OBJECTIF_TYPE + " ' " + SYNTAX[p] + " ' ", o_tokens);
				}
				else if (OBJECTIF_TYPE == "منطق")
				{
					if(DEBUG)DEBUG_MESSAGE("[CLASS-GLOBAL-BOOL (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" " + Global_ID[SYNTAX[p]] + " ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
				}
			}
			else
			{
				ErrorCode("علة : نوع المتغير العام غير معروف ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}

		// ----------------------
		// Global VARIABLE without _
		// ----------------------

		// g1 = 123
		// g2 = _g1 + 'g1' <--

		/*
		else if (G_VAR_IS_SET[(SYNTAX[p])])
		{
			...
		}
		*/

		// -----------------------------------
		// TXT
		// -----------------------------------

		else if (substr_utf8(SYNTAX[p], 0, 1) == "\"")
		{
			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة نص هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			if (!IsValidStringFormat(SYNTAX[p], o_tokens))
				ErrorCode("ليس بنص صحيح ' " + SYNTAX[p] + " ' ", o_tokens);

			if (OBJECTIF_TYPE == "عدد")
			{
				//if(DEBUG)DEBUG_MESSAGE("[STRING_MSG (" + SYNTAX[p] + ").ToInt] ", o_tokens); // DEBUG
				ErrorCode("لا يمكن تحويل نص إلى عدد ' " + SYNTAX[p] + " ' ", o_tokens);
			}
			else if (OBJECTIF_TYPE == "نص")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[STRING_MSG (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" wxT(" + SYNTAX[p] + ") ");
				// *** *** *** *** *** ***
			}
			else if (OBJECTIF_TYPE == "منطق")
			{
				ErrorCode("لا يمكن تحويل نص إلى منطق ' " + SYNTAX[p] + " ' ", o_tokens);
			}
			else if (OBJECTIF_TYPE == "C++")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[C++ نص (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" wxT(" + SYNTAX[p] + ") ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}

		// -----------------------------------
		// DIGIT
		// -----------------------------------

		else if (IsValidDigit(SYNTAX[p], true, o_tokens))
		{
			if (p > 0)
				if (!CAN_ADD_DIGIT_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن إضافة رقم هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			if (OBJECTIF_TYPE == "عدد")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[DIGIT " + SYNTAX[p] + "] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + SYNTAX[p] + " ");
				// *** *** *** *** *** ***
			}
			else if (OBJECTIF_TYPE == "نص")
			{
				if (ACCEPT_INT_TO_STRING)
				{
					if(DEBUG)
						DEBUG_MESSAGE("[DIGIT (" + SYNTAX[p] + ").ToString] ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" alifcore_IntToString(" + SYNTAX[p] + ") ");
					// *** *** *** *** *** ***
				}
				else
				{
					ErrorCode("لا يمكن تحويل رقم إلى نص : ' " + SYNTAX[p] + " '", o_tokens);
				}
			}
			else if (OBJECTIF_TYPE == "منطق")
			{
				ErrorCode("لا يمكن تحويل رقم إلى منطق ' " + SYNTAX[p] + " ' ", o_tokens);
			}
			else if (OBJECTIF_TYPE == "C++")
			{
				if(DEBUG)
					DEBUG_MESSAGE("[C++ رقم (" + SYNTAX[p] + ")] ", o_tokens); // DEBUG

				// *** Generate Code ***
				CPP_CODE.append(" " + SYNTAX[p] + " ");
				// *** *** *** *** *** ***
			}
			else
			{
				ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + " ' ", o_tokens);
			}
		}

		// -----------------------------------
		// Function / Class-Function
		// -----------------------------------

		else if (	CLASS_FUN_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])] || 
					G_FUN_IS_SET[(SYNTAX[p])] || 
					L_FUN_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])]) // call الدالة (a, b)
		{
			if (p > 0)
				if (!CAN_ADD_VAR_HERE(SYNTAX[p - 1]))
					ErrorCode("لا يمكن وضع دالة هنا ' " + SYNTAX[p - 1] + " " + SYNTAX[p] + " ' ", o_tokens);

			if (!IsInsideFunction)
				ErrorCode("يجب مناداة على الدالة من داخل دالة ' " + SYNTAX[p] + "()' ", o_tokens);

			if (SYNTAX[p + 1] == "")
				ErrorCode("يجب اضافه '(' بعد ' " + SYNTAX[p] + " ' ", o_tokens);

			if (SYNTAX[p + 1] != "(")
				ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " ' ", o_tokens);

			bool ThisIsClassFunction = false;
			if(CLASS_FUN_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])])
				ThisIsClassFunction = true;
			
			if(ThisIsClassFunction)
				ErrorCode("ggggggggggggggg' " + SYNTAX[p + 1] + " ' ", o_tokens);
			
			int TMP_FUN_LONG = p + 2; // a + b + 'p'fun ( c + (1 * 2) ) + c
			int OPEN_PARENTIZE = 0;

			while (TMP_FUN_LONG <= SYNTAX_LONG)
			{
				if (SYNTAX[TMP_FUN_LONG] == "(") // مفتوح inside الدالة args : fun( a + (b))
					OPEN_PARENTIZE++;
				else if (SYNTAX[TMP_FUN_LONG] == ")" && OPEN_PARENTIZE > 0) // Close inside الدالة args
					OPEN_PARENTIZE--;
				else if (SYNTAX[TMP_FUN_LONG] == ")" && OPEN_PARENTIZE < 1) // Close final الدالة call
				{
					if (TMP_FUN_LONG < SYNTAX_LONG)
					{
						// a = fun( a + (b)) + 123
						// str / عدد = متغير + (fun(var)) * (fun(var) / fun(var, fun(var), var) - var)

						if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
							(SYNTAX[TMP_FUN_LONG + 1] != "-") &&
							(SYNTAX[TMP_FUN_LONG + 1] != "*") &&
							(SYNTAX[TMP_FUN_LONG + 1] != "/") &&
							(SYNTAX[TMP_FUN_LONG + 1] != "،") &&
							(SYNTAX[TMP_FUN_LONG + 1] != ",") &&
							(SYNTAX[TMP_FUN_LONG + 1] != ")"))
						{
							ErrorCode("نص غير معروف بعد نداء ' " + SYNTAX[p] + "()' : ' " +
										SYNTAX[TMP_FUN_LONG + 1] + " ' ",
										o_tokens);
						}
					}
					else if (TMP_FUN_LONG == SYNTAX_LONG)
					{
						// a = fun( a + (b))
						if (SYNTAX[TMP_FUN_LONG] != ")") // double check!
							ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] + "()' بالإشارة ')' ", o_tokens);
					}

					break;
				}

				TMP_FUN_LONG++;
			}

			if (SYNTAX[TMP_FUN_LONG] != ")") // Double check!
				ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p] + "()' بالإشارة ')' ", o_tokens);
				//ErrorCode("===== |" + SYNTAX[TMP_FUN_LONG - 1] + "| =====", o_tokens);

			string TempToken[1024];
			int TempTokenCount = 0;
			for (int i = p + 2; i <= TMP_FUN_LONG; i++)
			{
				if (SYNTAX[i] != "")
				{
					TempToken[TempTokenCount] = SYNTAX[i];
					TempTokenCount++;
				}
			}

			bool IS_LOCAL_FUN = false;
			IS_LOCAL_FUN = L_FUN_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])];

			string FUN_TYPE;

			if (IS_LOCAL_FUN)
			{
				FUN_TYPE = L_FUN_TYPE[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])];

				if (FUN_TYPE == "عادم")
					ErrorCode("الدالة المحلية ' " + SYNTAX[p] + "()' من نوع عادم, لدى لا يمكن تحويلها إلى " + OBJECTIF_TYPE, o_tokens);
			}
			else
			{
				FUN_TYPE = G_FUN_TYPE[(SYNTAX[p])];

				if (FUN_TYPE == "عادم")
					ErrorCode("الدالة العامة ' " + SYNTAX[p] + "()' من نوع عادم, لدى لا يمكن تحويل إلى " + OBJECTIF_TYPE, o_tokens);
			}

			string CG_BUFFER;
			
			if (OBJECTIF_TYPE == "عدد")
			{
				if (FUN_TYPE == "عدد")
				{
					if (IS_LOCAL_FUN)
					{
						// Call a local fun int

						if(DEBUG)DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-INT ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->FUNCTION_" + ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( 	false, 
																TMP_WIN_OR_CLASS, 
																SYNTAX[p], 
																0, 
																TMP_WIN_OR_CLASS, 
																TheFunction, 
																TempToken, 
																(TempTokenCount - 1), 
																o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ");
						// *** *** *** *** *** ***
					}
					else
					{
						// Call a Global fun int

						if(DEBUG)DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-INT ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( true, 
																	"", 
																	SYNTAX[p], 
																	0, 
																	TMP_WIN_OR_CLASS, 
																	TheFunction, 
																	TempToken, 
																	(TempTokenCount - 1), 
																	o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ");
						// *** *** *** *** *** ***
					}
				}
				else if (FUN_TYPE == "نص")
				{
					if (IS_LOCAL_FUN)
						ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] + "()' من نص إلى عدد ", o_tokens);
					else
						ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] + "()' من نص إلى عدد ", o_tokens);
				}
				else if (FUN_TYPE == "منطق")
				{
					if (IS_LOCAL_FUN)
						ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] + "()' من منطق إلى عدد ", o_tokens);
					else
						ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] + "()' من منطق إلى عدد ", o_tokens);
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + "()' ", o_tokens);
				}
			}
			else if (OBJECTIF_TYPE == "نص")
			{
				if (FUN_TYPE == "عدد")
				{
					if (IS_LOCAL_FUN)
					{
						// Call a local fun int.ToString

						if(DEBUG)DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-INT.ToString ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" alifcore_IntToString( OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->FUNCTION_" + ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( false, 
																	TMP_WIN_OR_CLASS, 
																	SYNTAX[p], 
																	0, 
																	TMP_WIN_OR_CLASS, 
																	TheFunction, 
																	TempToken, 
																	(TempTokenCount - 1), 
																	o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ) ");
						// *** *** *** *** *** ***
					}
					else
					{
						// Call a Global fun int.ToString

						if(DEBUG)DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-INT.ToString ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" alifcore_IntToString( FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( true, 
																	"", 
																	SYNTAX[p], 
																	0, 
																	TMP_WIN_OR_CLASS, 
																	TheFunction, 
																	TempToken, 
																	(TempTokenCount - 1), 
																	o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ) ");
						// *** *** *** *** *** ***
					}
				}
				else if (FUN_TYPE == "نص")
				{
					if (IS_LOCAL_FUN)
					{
						// Call a local fun string

						if(DEBUG)DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-STRING ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->FUNCTION_" + ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( false, 
																TMP_WIN_OR_CLASS, 
																SYNTAX[p], 
																0, 
																TMP_WIN_OR_CLASS, 
																TheFunction, 
																TempToken, 
																(TempTokenCount - 1), 
																o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ");
						// *** *** *** *** *** ***

						if(DEBUG)DEBUG_MESSAGE(" ) ", o_tokens); // DEBUG
					}
					else
					{
						// Call a Global fun int

						if(DEBUG)DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-STRING ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( true, 
																	"", 
																	SYNTAX[p], 
																	0, 
																	TMP_WIN_OR_CLASS, 
																	TheFunction, 
																	TempToken, 
																	(TempTokenCount - 1), 
																	o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ");
						// *** *** *** *** *** ***
					}
				}
				else if (FUN_TYPE == "منطق")
				{
					if (IS_LOCAL_FUN)
						ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] + "()' من منطق إلى نص ", o_tokens);
					else
						ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] + "()' من منطق إلى نص ", o_tokens);
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + "()' ", o_tokens);
				}
			}
			else if (OBJECTIF_TYPE == "منطق")
			{
				if (FUN_TYPE == "عدد")
				{
					if (IS_LOCAL_FUN)
						ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] + "()' من عدد إلى منطق ", o_tokens);
					else
						ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] + "()' من عدد إلى منطق ", o_tokens);
				}
				else if (FUN_TYPE == "نص")
				{
					if (IS_LOCAL_FUN)
						ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p] + "()' من نص إلى منطق ", o_tokens);
					else
						ErrorCode("لا يمكن تحويل الدالة العامة ' " + SYNTAX[p] + "()' من نص إلى منطق ", o_tokens);
				}
				else if (FUN_TYPE == "منطق")
				{
					if (IS_LOCAL_FUN)
					{
						// Call a local fun bool

						if(DEBUG)DEBUG_MESSAGE("[CALL-LOCAL-FUNCTION-BOOL ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->FUNCTION_" + ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( false, 
																	TMP_WIN_OR_CLASS, 
																	SYNTAX[p], 
																	0, 
																	TMP_WIN_OR_CLASS, 
																	TheFunction, 
																	TempToken, 
																	(TempTokenCount - 1), 
																	o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ");
						// *** *** *** *** *** ***
					}
					else
					{
						// Call a Global fun bool

						if(DEBUG)DEBUG_MESSAGE("[CALL-GLOBAL-FUNCTION-BOOL ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
						// *** *** *** *** *** ***

						CG_BUFFER.append( CHECK_CALL_FUN_ARG( true, 
																	"", 
																	SYNTAX[p], 
																	0, 
																	TMP_WIN_OR_CLASS, 
																	TheFunction, 
																	TempToken, 
																	(TempTokenCount - 1), 
																	o_tokens));

						// *** Generate Code ***
						// Buffer
						CG_BUFFER.append(" ) ");
						// *** *** *** *** *** ***
					}
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p] + "()' ", o_tokens);
				}
			}
			else if (OBJECTIF_TYPE == "C++"){

				// xType: We igiore type of function.. this is C++ targed
				// user must take care of this!.

				if (IS_LOCAL_FUN){

					// Call a local fun xType

					if(DEBUG)DEBUG_MESSAGE("[C++ CALL-LOCAL-FUNCTION-xType ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

					// *** Generate Code ***
					// Buffer
					CG_BUFFER.append(" OBJ_CLASS_WINDOW_" + ID[TheWindow] + "->FUNCTION_" + ID[SYNTAX[p]] + "( ");
					// *** *** *** *** *** ***

					CG_BUFFER.append( CHECK_CALL_FUN_ARG(	false, 
															TMP_WIN_OR_CLASS, 
															SYNTAX[p], 
															0, 
															TMP_WIN_OR_CLASS, 
															TheFunction, 
															TempToken, 
															(TempTokenCount - 1), 
															o_tokens));

					// *** Generate Code ***
					// Buffer
					CG_BUFFER.append(" ) ");
					// *** *** *** *** *** ***
				}
				else
				{
					// Call a Global fun xType

					if(DEBUG)DEBUG_MESSAGE("[C++ CALL-GLOBAL-FUNCTION-xType ' " + SYNTAX[p] + " '] (", o_tokens); // DEBUG

					// *** Generate Code ***
					// Buffer
					CG_BUFFER.append(" FUNCTION_" + Global_ID[SYNTAX[p]] + "( ");
					// *** *** *** *** *** ***

					CG_BUFFER.append( CHECK_CALL_FUN_ARG( true, 
																"", 
																SYNTAX[p], 
																0, 
																TMP_WIN_OR_CLASS, 
																TheFunction, 
																TempToken, 
																(TempTokenCount - 1), 
																o_tokens));

					// *** Generate Code ***
					// Buffer
					CG_BUFFER.append(" ) ");
					// *** *** *** *** *** ***
				}
			}

			// *** Generate Code ***
			CPP_CODE.append(" " + CG_BUFFER + " ");
			// *** *** *** *** *** ***

			p = TMP_FUN_LONG; // point to next token after fun(...)
		}

		// -----------------------------------
		// : (Operator between members)
		// -----------------------------------

		else if (CONTROL_WIN_IS_SET[SYNTAX[p]] || CONTROL_IS_SET[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])]) 
		{
			// abc = Window:option() 				| win:show()
			// abc = Window:Option					| win:with
			// abc = Window:local_func(a, b)		| win:my_func(a, b)
			// abc = Window:Control:Option()		| win:ctr:disable()
			// abc = Window:Control:Option			| win:ctr:text

			// abc = Control:Option()				| ctr:disable()
			// abc = Control:Option					| ctr:text

			string CTR_WIN;
			string CTR_CONTROL;
			string CTR_OPTION;
			string CTR_OPTION_TYPE;
			string CTR_OPTION_CPP_END;
			//int CTR_ARG;
			int CTR_LAST_TOKEN_NUMBER;

			if (CONTROL_WIN_IS_SET[SYNTAX[p]] || WIN_IS_SET [SYNTAX[p]])
			{
				// abc = Window:option() 				| win:show()
				// abc = Window:Option					| win:with
				// abc = Window:local_func(a, b)		| win:my_func(a, b)
				// abc = Window:Control:Option()		| win:ctr:disable()
				// abc = Window:Control:Option			| win:ctr:text
				
					if (SYNTAX[p + 1] != ":")
						ErrorCode("يجب اضافه ' : ' بعد ' " + SYNTAX[p] + " ' ", o_tokens);
					
					if (SYNTAX[p + 2] == "")
						ErrorCode("يجب اضافه عضو تابع ل ' " + SYNTAX[p] + " ' بعد ':' ", o_tokens);
					
					if (SYNTAX[p + 3] == "")
						ErrorCode("يجب اضافه ':' أو '()' بعد ' " + SYNTAX[p] + " " + SYNTAX[p + 1] + " " + SYNTAX[p + 2] + " ' ", o_tokens);
					
					if (SYNTAX[p + 3] != "(" && SYNTAX[p + 3] != ":")
						ErrorCode("أمر غير معروف ' " + SYNTAX[p + 3] + " ', يجب اضافه ':' أو '()' ", o_tokens);
					
					if (SYNTAX[p + 3] == "(")
					{
						// ---------------------------------------------------------------------------------
						// Window : Options ()
						// ---------------------------------------------------------------------------------

						// abc = Window:option() 				| win:show()
						// abc = Window:local_func(a, b)		| win:my_func(a, b)

						// -----------------------
						// abc = Window:win-option() 
						// -----------------------

						if (SYNTAX[p + 2] == "إظهار" ||
							SYNTAX[p + 2] == "إخفاء" ||
							SYNTAX[p + 2] == "تدمير")
						{
							ErrorCode("لا يمكن إستعمال خاصية ' " + SYNTAX[p + 2] + " ' هنا، لأنها من نوع عادم" + OBJECTIF_TYPE, o_tokens);
						}

						// -----------------------
						// abc = Window:win-local_func() 
						// -----------------------

						else if (L_FUN_IS_SET[std::make_pair(SYNTAX[p], SYNTAX[p + 2])])
						{
							if (SYNTAX[p + 3] != "(")
								ErrorCode("من اجل نداء الدالة ' " + SYNTAX[p + 2] + " ' يجب اضافه '()' بعد ' " + SYNTAX[p] + " " + 
												SYNTAX[p + 1] + " " + SYNTAX[p + 2] + " ' ", o_tokens);
							
							string FUN_TYPE;
							string CPP_END;

							FUN_TYPE = L_FUN_TYPE[std::make_pair(TMP_WIN_OR_CLASS, SYNTAX[p])];

							if (FUN_TYPE == "عادم")
								ErrorCode("الدالة المحلية ' " + SYNTAX[p] + "()' من نوع عادم, لدى لا يمكن تحويلها إلى " + OBJECTIF_TYPE, o_tokens);

							if (OBJECTIF_TYPE == "عدد")
							{
								if (FUN_TYPE == "عدد")
								{
									if(DEBUG)DEBUG_MESSAGE(" [WIN'" + SYNTAX[p] + " ':LOCAL_INT_FUNC'" + SYNTAX[p + 2] + " '( ", o_tokens); // DEBUG

									// *** Generate Code ***
									// Buffer
									CPP_END = " ";
									CPP_CODE.append(" OBJ_CLASS_WINDOW_" + ID[SYNTAX[p]] + "->FUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
									// *** *** *** *** *** ***
								}
								else
								{
									ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p + 2] + "()' من " + FUN_TYPE + " إلى عدد ", o_tokens);
								}
							}
							else if (OBJECTIF_TYPE == "نص")
							{
								if (FUN_TYPE == "نص")
								{
									if(DEBUG)DEBUG_MESSAGE(" [WIN'" + SYNTAX[p] + " ':LOCAL_STRING_FUNC'" + SYNTAX[p + 2] + " '( ", o_tokens); // DEBUG

									// *** Generate Code ***
									// Buffer
									CPP_END = " ";
									CPP_CODE.append(" OBJ_CLASS_WINDOW_" + ID[SYNTAX[p]] + "->FUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
									// *** *** *** *** *** ***
								}
								else if (FUN_TYPE == "عدد")
								{
									if(DEBUG)DEBUG_MESSAGE(" [WIN'" + SYNTAX[p] + " ':LOCAL_INT_FUNC'" + SYNTAX[p + 2] + " '().ToString( ", o_tokens); // DEBUG

									// *** Generate Code ***
									// Buffer
									CPP_END = " ) ";
									CPP_CODE.append(" alifcore_IntToString( OBJ_CLASS_WINDOW_" + ID[SYNTAX[p]] + "->FUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
									// *** *** *** *** *** ***
								}
								else
								{
									ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p + 2] + "()' من " + FUN_TYPE + " إلى نص ", o_tokens);
								}
							}
							else if (OBJECTIF_TYPE == "منطق")
							{
								if (FUN_TYPE == "منطق")
								{
									if(DEBUG)DEBUG_MESSAGE(" [WIN'" + SYNTAX[p] + " ':LOCAL_BOOL_FUNC'" + SYNTAX[p + 2] + " '( ", o_tokens); // DEBUG

									// *** Generate Code ***
									// Buffer
									CPP_END = " ";
									CPP_CODE.append(" OBJ_CLASS_WINDOW_" + ID[SYNTAX[p]] + "->FUNCTION_" + ID[SYNTAX[p + 2]] + "( ");
									// *** *** *** *** *** ***
								}
								else
								{
									ErrorCode("لا يمكن تحويل الدالة المحلية ' " + SYNTAX[p + 2] + "()' من " + FUN_TYPE + " إلى منطق ", o_tokens);
								}
							}
							else
								ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + SYNTAX[p + 2] + " ' ", o_tokens);

							// abc = (p)Window:local_func(a, b) + x + y

							int TMP_FUN_LONG = p + 4;
							int OPEN_PARENTIZE = 0;

							// Get Local Function Args
							while (TMP_FUN_LONG <= SYNTAX_LONG)
							{
								if (SYNTAX[TMP_FUN_LONG] == "(") // مفتوح inside الدالة args : fun( a + (b))
									OPEN_PARENTIZE++;
								else if (SYNTAX[TMP_FUN_LONG] == ")" && OPEN_PARENTIZE > 0) // Close inside الدالة args
									OPEN_PARENTIZE--;
								else if (SYNTAX[TMP_FUN_LONG] == ")" && OPEN_PARENTIZE < 1) // Close final الدالة call
								{
									if (TMP_FUN_LONG < SYNTAX_LONG)
									{
										// abc = fun( a + (b)) + 123
										// abc = x + (fun(var)) * (fun(var) / fun(var, fun(var), var) - var)

										if ((SYNTAX[TMP_FUN_LONG + 1] != "+") &&
											(SYNTAX[TMP_FUN_LONG + 1] != "-") &&
											(SYNTAX[TMP_FUN_LONG + 1] != "*") &&
											(SYNTAX[TMP_FUN_LONG + 1] != "/") &&
											(SYNTAX[TMP_FUN_LONG + 1] != "،") &&
											(SYNTAX[TMP_FUN_LONG + 1] != ",") &&
											(SYNTAX[TMP_FUN_LONG + 1] != ")"))
										{
											ErrorCode("نص غير معروف بعد نداء ' " + SYNTAX[p + 2] + "()' : ' " +
															SYNTAX[TMP_FUN_LONG + 1] + " ' ",
															o_tokens);
										}
									}
									else if (TMP_FUN_LONG == SYNTAX_LONG)
									{
										// a = fun( a + (b))
										if (SYNTAX[TMP_FUN_LONG] != ")" || SYNTAX[SYNTAX_LONG] != ")") // double check!
											ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p + 2] + "()' بالإشارة ')' ", o_tokens);
									}

									break;
								}

								TMP_FUN_LONG++;
							}

							if (SYNTAX[TMP_FUN_LONG] != ")") // Double check!
								ErrorCode("يجب إنهاء نداء الدالة ' " + SYNTAX[p + 2] + "()' بالإشارة ')' ", o_tokens);

							string TempToken[1024];
							int TempTokenCount = 0;
							for (int i = p + 4; i <= TMP_FUN_LONG; i++)
							{
								if (SYNTAX[i] != "")
								{
									TempToken[TempTokenCount] = SYNTAX[i];
									TempTokenCount++;
								}
							}

							// Check local fun Args : fun (a + c, 2 * (b - 1))
							CPP_CODE.append( CHECK_CALL_FUN_ARG( false, 
																 SYNTAX[p], 
																 SYNTAX[p + 2], 
																 0, 
																 TheWindow, 
																 TheFunction, 
																 TempToken, 
																 (TempTokenCount - 1),  
																 o_tokens));

							if(DEBUG)DEBUG_MESSAGE(" )] \n\n", o_tokens); // DEBUG

							// *** Generate Code ***
							CPP_CODE.append(" ) " + CPP_END + " ");
							// *** *** *** *** *** ***

							// Point to last token of full func call
							p = TMP_FUN_LONG;
							
							continue;
						}
						// ---------------------------------------------------------------------------------
						else
							ErrorCode("النافذة ' " + SYNTAX[p] + " ' لا تحتوي على دالة محليه بإسم ' " + SYNTAX[p + 2] + " ' ", o_tokens);
						
						// Exception!
						continue;
					}
					/*
					else if (SYNTAX[p + 3] == "=")
					{
						// ---------------------------------------------------------------------------------
						// abc = Window : Value
						// ---------------------------------------------------------------------------------

						// abc = Window:Title
						// abc = Window:With
						// abc = Window:Heint
						// abc = Window:X
						// abc = Window:Y
						// abc = Window:Color
						// abc = Window:Background

						// bool A = Window:Is_Show

						if (SYNTAX[p + 4] == "")
								ErrorCode("يجب اضافه قيمة بعد '=' ", o_tokens);

						string VALUE_TYPE;
						string VALUE_CPP_END;

						if (SYNTAX[p + 2] == "عنوان")
						{
							// Window:Title = 

							if(DEBUG)DEBUG_MESSAGE("		[WIN'" + SYNTAX[p] + " ':VALUE'" + SYNTAX[p + 2] + "(SetWindowTitle)' = ", o_tokens); // DEBUG

							// *** Generate Code ***
							if (!IsInsideWindow)
								// Window : Title =
								CPP_GLOBAL_FUN.append(CG_WIN_MEMBER(SYNTAX[p], "SetLabel (wxT( "));
							else
								// Window : Title =
								cpp_AddScript(TheFunction, CG_WIN_MEMBER(SYNTAX[p], "SetLabel (wxT( "));
							// *** *** *** *** *** ***
							VALUE_CPP_END = " )); \n } \n";
							// *** *** *** *** *** ***
						}
						else
						{
							ErrorCode("النافذة ' " + SYNTAX[p] + " ' لا تحتوي على خاصيه باسم ' " + SYNTAX[p + 2] + " ' ", o_tokens);
						}

						// Window : Option = ...

						TempTokenCount = 0;
						for (int p = 3; p <= o_tokens->TOTAL[o_tokens->Line]; p++) // | = a * b + 2 / (c) + 1 |
						{
							if (SYNTAX[p] != "")
							{
								TempToken[TempTokenCount] = SYNTAX[p];
								TempTokenCount++;
							}
						}

						CheckForSyntax(VALUE_TYPE,	// OBJECTIF_TYPE
										true, 		// Accept Using Reference to Window:Controls
										true, 		// Accept Using Reference to Window:Function
										true, 		// Accept Using Reference to Global Functions
										true, 		// Accept Using Reference to Local Functions
										true, 		// Accept Using Reference to Global VAR
										true, 		// Accept Using Reference to Local VAR
										false, 		// Accept Convertion from String To Int
										true, 		// Accept Convertion from Int To String
										TempToken,				// SYNTAX[] string
										(TempTokenCount - 1),	// SYNTAX_LONG int
										TheWindow,		// TMP_WINDOW_NAME
										TheFunction,		// TMP_FUNCTION_NAME
										o_tokens);

						// *** Generate Code ***
						if (!IsInsideWindow)
							// Window : Option =
							CPP_GLOBAL_FUN.append(VALUE_CPP_END);
						else
							// Window : Option =
							cpp_AddScript(TheFunction, VALUE_CPP_END);
						// *** *** *** *** *** ***

						continue;
					}
					*/
					else if (SYNTAX[p + 3] == ":")
					{
						// ---------------------------------------------------------------------------------
						// abc = Window : Control : Options
						// ---------------------------------------------------------------------------------

						// abc = Window:Control:Option()	| win:ctr:disable()
						// abc = Window:Control:Option		| win:ctr:text
						
						if (!CONTROL_IS_SET[std::make_pair(SYNTAX[p], SYNTAX[p + 2])])
							ErrorCode("النافذة ' " + SYNTAX[p] + " ' لا تحتوي على اداه باسم ' " + SYNTAX[p + 2] + " ' ", o_tokens);
					
						//if (SYNTAX[p + 5] == "")
							//ErrorCode("يجب اضافه '=' أو '()' بعد ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + " : " + SYNTAX[p + 4] + " ' ", o_tokens);
					
						//if (SYNTAX[p + 5] != "=" && SYNTAX[p + 5] != "(")
							//ErrorCode("أمر غير معروف ' " + SYNTAX[p + 5] + " ' ", o_tokens);
					
						//if (SYNTAX[p + 6] == "")
							//ErrorCode("يجب اضافه قيمة بعد ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + " : " + SYNTAX[p + 4] + " " + SYNTAX[p + 6] + " ' ", o_tokens);

						CTR_WIN = SYNTAX[p];
						CTR_CONTROL = SYNTAX[p + 2];
						CTR_OPTION = SYNTAX[p + 4];
						//CTR_ARG = 6;
						CTR_LAST_TOKEN_NUMBER = p + 4;
					}
					else
					{
						// Exception !
						ErrorCode("يجب اضافه ':' أو '()' بعد ' " + SYNTAX[p] + " " + SYNTAX[p + 1] + " " + SYNTAX[p + 2] + " ' ", o_tokens);
					}
			}
			else if (CONTROL_IS_SET[std::make_pair(TheWindow, SYNTAX[p])])
			{
					// ---------------------------------------------------------------------------------
					// Control : Options
					// ---------------------------------------------------------------------------------

					// abc = Control:Option()		| ctr:disable()
					// abc = Control:Option			| ctr:text
					
					if (SYNTAX[p + 1] == "")
						ErrorCode("يجب اضافة ':' بعد ' " + SYNTAX[p] + " ' ", o_tokens);

					if (SYNTAX[p + 1] != ":")
						ErrorCode("أمر غير معروف ' " + SYNTAX[p + 1] + " ', يجب اضافة ':' بعد ' " + SYNTAX[p] + " ' ", o_tokens);
					
					if (SYNTAX[p + 2] == "")
						ErrorCode("يجب اضافة خاصية بعد ' " + SYNTAX[p] + " : ' ", o_tokens);
					
					//if (SYNTAX[p + 3] == "")
						//ErrorCode("يجب اضافة '=' أو '()' بعد ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + " ' ", o_tokens);
					
					//if (SYNTAX[p + 3] != "=" && SYNTAX[p + 3] != "(")
						//ErrorCode("أمر غير معروف ' " + SYNTAX[p + 5] + " ' ", o_tokens);
					
					//if (SYNTAX[p + 4] == "")
						//ErrorCode("يجب اضافة قيمة بعد ' " + SYNTAX[p] + " : " + SYNTAX[p + 2] + " " + SYNTAX[p + 3] + " ' ", o_tokens);
					
					CTR_WIN = TheWindow;
					CTR_CONTROL = SYNTAX[p];
					CTR_OPTION = SYNTAX[p + 2];
					//CTR_ARG = 4;
					CTR_LAST_TOKEN_NUMBER = p + 2;
			}
			else
			{
					// Exception !
					ErrorCode("أمر غير معروف ' " + SYNTAX[p] + " ', يجب اضافة نافذه او اداة", o_tokens);
			}
			
			// ---------------------------------------------------------------------------------
			// Check Control Options if valid
			// For Control:Option AND Window:Control:Option
			// ---------------------------------------------------------------------------------

			if (CTR_OPTION == "نص") // abc = Control:نص
			{
				// GetValue not working Label, need 
				string GetValueFix = "GetValue";
				if (CONTROL_TYPE[std::make_pair(CTR_WIN, CTR_CONTROL)] == "ملصق")
					GetValueFix = "GetLabelText";

				CTR_OPTION_TYPE = "نص";

				if (OBJECTIF_TYPE == "عدد")
				{
					if(DEBUG)DEBUG_MESSAGE(" [WIN'" + CTR_WIN + " ':CTR'" + CTR_CONTROL + " ':OPTION'" + CTR_OPTION + "(Text).ToInt()]' ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" (alifcore_StringToInt(OBJ_CTR_" + ID[CTR_WIN] + "_" + Control_ID[CTR_CONTROL] + "->" + GetValueFix + "())) ");
					// *** *** *** *** *** ***
				}
				else if (OBJECTIF_TYPE == "نص")
				{
					if(DEBUG)DEBUG_MESSAGE(" [WIN'" + CTR_WIN + " ':CTR'" + CTR_CONTROL + " ':OPTION'" + CTR_OPTION + "(Text)]' ", o_tokens); // DEBUG

					// *** Generate Code ***
					CPP_CODE.append(" (OBJ_CTR_" + ID[CTR_WIN] + "_" + Control_ID[CTR_CONTROL] + "->" + GetValueFix + "()) ");
					// *** *** *** *** *** ***
				}
				else if (OBJECTIF_TYPE == "منطق")
				{
					ErrorCode("لا يمكن تحويل نص إلى منطق ", o_tokens);
				}
				else
				{
					ErrorCode("علة : نوع المستهدف غير معروف ' " + OBJECTIF_TYPE + " ' ل ' " + CTR_CONTROL + " ' ", o_tokens);
				}
			}
			else
			{
				ErrorCode("الأداة ' " + CTR_CONTROL + " ' لا تحتوي على خاصيه باسم ' " + CTR_OPTION + " ' ", o_tokens);
			}

			// Point to last token of full win:ctrl:option
			p = CTR_LAST_TOKEN_NUMBER;
		}

		// -----------------------------------
		// ERROR
		// -----------------------------------

		else
		{
			//if(DEBUG)DEBUG_MESSAGE(" \n \n *** \n TMP_WIN_OR_CLASS : " + TMP_WIN_OR_CLASS + " \n", o_tokens);
			//if(DEBUG)DEBUG_MESSAGE("TmpFunction : " + TmpFunction + " \n *** \n", o_tokens);

			if (Control_ID[SYNTAX[p]] != "")
			{
				// show error description
				// when, (no-win) CTR:OPTION
				// in global area.

				ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] + " '، على العموم، إن كنت تقصد أداة، لا تنسى أن تحدد إسم النافذة قبل إسم الأداة  ", o_tokens);
			}
			// أو
			else if (SYNTAX[p] == "او" || 
					 SYNTAX[p] == "ٱو" || 
					 SYNTAX[p] == "آو" ||
					 SYNTAX[p] == "والا" || 
					 SYNTAX[p] == "وإلا" || 
					 SYNTAX[p] == "وألا")
			{
				ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] + " '، هل تقصد ' أو ' ؟ ", o_tokens);
			}
			else{

				//L_VAR_TYPE[std::make_pair(TMP_WIN_OR_CLASS + TmpFunction, SYNTAX[p])]
				if(DEBUG)DEBUG_MESSAGE("TMP_WIN_OR_CLASS '" + TMP_WIN_OR_CLASS + "'\n ", o_tokens); // DEBUG
				if(DEBUG)DEBUG_MESSAGE("TmpFunction '" + TmpFunction + "'\n ", o_tokens); // DEBUG
				if(DEBUG)DEBUG_MESSAGE("SYNTAX[p] '" + SYNTAX[p] + "'\n ", o_tokens); // DEBUG
				ErrorCode("بناء الجملة غير مفهوم : ' " + SYNTAX[p] + " ' ", o_tokens);
			}	
		}
	}

	if (ALIF_PARENTHESIS_STATUS > 0 && !IS_IF_SYNTAX)
		ErrorCode("غلق قوس مفقود ')' ", o_tokens);

	IS_IF_SYNTAX = false;

	return CPP_CODE;
}

// ====================================================

string CHECK_CALL_FUN_ARG(bool CALL_FUN_GLOBAL,
						  string CALL_WIN_OR_CLASS, // win1/class1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> win1
						  string CALL_FUN,			// win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> fun1
						  int CALL_IS_CLASS,		// 0 = non class, 1 constructor, 2 = الدالة member, ل Message when new obj
						  string FROM_WIN_OR_CLASS, // win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> win2
						  string FROM_FUN,			// win1 { fun1(int a) } | win2 { fun2{ عدد b; fun1(b) } } ==> fun2
						  string SYNTAX[1024],
						  int SYNTAX_LONG,
						  CLASS_TOKEN *o_tokens)
{
	// SYNTAX[]			: 	1+1, b*2, 	("test" + s)
	// G_FUN_ARG_TYPE 	: 	INT, 	INT, 	STRING

	int CURRENT_ARG_NUMBER = 0;

	int p = 0;
	string ARG[1024];
	int ARG_LONG = 1;

	ARG[0] = "=";

	int CALL_ARG_TOTAL = 0;

	string CPP_CODE;

	//if(DEBUG)DEBUG_MESSAGE("_---_|" + IntToString(G_FUN_ARG_TOTAL[CALL_FUN]) + "\n" + IntToString(SYNTAX_LONG) + "|_---_", o_tokens); // DEBUG
	//if(DEBUG)DEBUG_MESSAGE("=__=|" + SYNTAX + "|_---_", o_tokens); // DEBUG

	if (CALL_FUN_GLOBAL)
	{
		// Call Global Fun

		CALL_ARG_TOTAL = G_FUN_ARG_TOTAL[CALL_FUN];

		// check args
		if (CALL_ARG_TOTAL > 0 && SYNTAX_LONG < 1)
			ErrorCode("الدالة العامة ' " + CALL_FUN + "()' تأخد " + IntToString(CALL_ARG_TOTAL) + " خاصية", o_tokens);

		// الدالة العامة () without args
		if (CALL_ARG_TOTAL == 0 && SYNTAX_LONG == 0)
			return "";
	}
	else
	{
		// Call Local Fun

		CALL_ARG_TOTAL = L_FUN_ARG_TOTAL[std::make_pair(CALL_WIN_OR_CLASS, CALL_FUN)];

		// check args
		if (CALL_ARG_TOTAL > 0 && SYNTAX_LONG < 1)
			ErrorCode("الدالة المحلية ' " + CALL_FUN + "()' تأخد " + IntToString(CALL_ARG_TOTAL) + " خاصية", o_tokens);

		// الدالة المحلية () without args
		if (CALL_ARG_TOTAL == 0 && SYNTAX_LONG == 0)
			return "";
	}

	// الدالة (int a, عدد b) with args
	while (p <= SYNTAX_LONG) // using while is bcs need 'if p < G_FUN_ARG_TOTAL' بعد while finish
	{
		//if(DEBUG)DEBUG_MESSAGE("\n --[" + SYNTAX[p] + "|" + G_FUN_ARG_TYPE[std::make_pair(CALL_FUN, p)] + "]-- \n", o_tokens); // DEBUG

		if (SYNTAX[p] != "," && SYNTAX[p] != "،" && p < SYNTAX_LONG)
		{
			ARG[ARG_LONG] = SYNTAX[p];
			ARG_LONG++;
		}
		else
		{
			//if(DEBUG)DEBUG_MESSAGE("-[" + CONVERT_STRING_ARRAY_TO_STRING(ARG,ARG_LONG) + "]-", o_tokens); // DEBUG
			//ErrorCode("|G_FUN_ARG_TYPE TmpFunction: " + TmpFunction + " p : " + IntToString(p) + "|", o_tokens );

			if (ARG_LONG < 2) // الدالة ( , ...) ['0'] =, [1] 'user arg', [2] null ل incrumentation
				ErrorCode("إحدى الخصائص فارغة", o_tokens);

			if (CALL_FUN_GLOBAL)
			{
				// نداء a الدالة العامة ( ... check ARG n ...)

				// الدالة (a,a,a) ل (a,a)
				if ((CURRENT_ARG_NUMBER + 1) > CALL_ARG_TOTAL)
					ErrorCode("خصائص أكثر من الازم، الدالة العامة ' " + CALL_FUN + "()' تأخد فقط " + IntToString(CALL_ARG_TOTAL) + " خاصية ", o_tokens);

				// Current خاصية OBJECTIF_TYPE
				CPP_CODE.append(CheckForSyntax(G_FUN_ARG_TYPE[std::make_pair(CALL_FUN, CURRENT_ARG_NUMBER)],
											 true,				// Accept Using Reference إلى Window:Controls
											 true,				// Accept Using Reference إلى Window:Function
											 true,				// Accept Using Reference إلى Global Functions
											 true,				// Accept Using Reference إلى Local Functions
											 true,				// Accept Using Reference إلى Global VAR
											 true,				// Accept Using Reference إلى Local VAR
											 false,				// Accept Convertion من نص إلى Int
											 true,				// Accept Convertion من عدد إلى String
											 ARG,				// SYNTAX[] string
											 (ARG_LONG - 1),	// SYNTAX_LONG int
											 FROM_WIN_OR_CLASS, // TMP_WINDOW_NAME
											 FROM_FUN,			// TMP_FUNCTION_NAME
											 o_tokens));
			}
			else
			{
				// نداء a الدالة المحلية ( ... check ARG n ...)

				// الدالة (a,a,a) ل (a,a)
				if ((CURRENT_ARG_NUMBER + 1) > CALL_ARG_TOTAL)
				{
					if (CALL_IS_CLASS == 1) // constructor
						ErrorCode("خصائص أكثر من ألازم، دالة بناء الصنف ' " + CALL_WIN_OR_CLASS + " ' تأخد فقط " + IntToString(CALL_ARG_TOTAL) + " خاصية ", o_tokens);
					else if (CALL_IS_CLASS == 2) // الدالة member
						ErrorCode("خصائص أكثر من ألازم، الدالة ' " + CALL_FUN + " ' المنتمية للصنف ' " + CALL_WIN_OR_CLASS + " ' تأخد فقط " + IntToString(CALL_ARG_TOTAL) + " خاصية ", o_tokens);
					else
						ErrorCode("خصائص أكثر من ألازم، الدالة ' " + CALL_FUN + " ' تأخد فقط " + IntToString(CALL_ARG_TOTAL) + " خاصية ", o_tokens);
				}

				// Current خاصية OBJECTIF_TYPE
				CPP_CODE.append(CheckForSyntax(L_FUN_ARG_TYPE[std::make_pair(CALL_WIN_OR_CLASS + CALL_FUN, CURRENT_ARG_NUMBER)],
											 true,				// Accept Using Reference إلى Window:Controls
											 true,				// Accept Using Reference إلى Window:Function
											 true,				// Accept Using Reference إلى Global Functions
											 true,				// Accept Using Reference إلى Local Functions
											 true,				// Accept Using Reference إلى Global VAR
											 true,				// Accept Using Reference إلى Local VAR
											 false,				// Accept Convertion من نص إلى Int
											 true,				// Accept Convertion من عدد إلى String
											 ARG,				// SYNTAX[] string
											 (ARG_LONG - 1),	// SYNTAX_LONG int
											 FROM_WIN_OR_CLASS, // TMP_WINDOW_NAME
											 FROM_FUN,			// TMP_FUNCTION_NAME
											 o_tokens));
			}

			//ARG[] = ""; // bug!
			//delete[] ARG; // delete is فقط used when paired with new()

			ARG[0] = "=";
			ARG_LONG = 1;		  // Point إلى next arg writed بالإشارة user
			CURRENT_ARG_NUMBER++; // Point إلى next يدعى الدالة arg

			if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL)
			{
				if(DEBUG)
					DEBUG_MESSAGE(", ", o_tokens); // DEBUG
				CPP_CODE.append(" , ");
			}
		}

		p++;
	}

	if (CALL_FUN_GLOBAL)
	{
		// الدالة (a,a,a) ل (a,a)
		if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL)
			ErrorCode("خصائص قليلة, الدالة العامة ' " + CALL_FUN + "()' تأخد " + IntToString(CALL_ARG_TOTAL) + " خاصية، ثم إنشاء فقط " + IntToString(CURRENT_ARG_NUMBER) + " خاصية ", o_tokens);
	}
	else
	{
		// الدالة (a,a,a) ل (a,a)
		if (CURRENT_ARG_NUMBER < CALL_ARG_TOTAL)
		{
			if (CALL_IS_CLASS == 1) // constructor
				ErrorCode("خصائص قليلة, دالة بناء الصنف ' " + CALL_WIN_OR_CLASS + "()' تأخد " + IntToString(CALL_ARG_TOTAL) + " خاصية، ثم إنشاء فقط " + IntToString(CURRENT_ARG_NUMBER) + " خاصية ", o_tokens);
			else if (CALL_IS_CLASS == 2) // الدالة member
				ErrorCode("خصائص قليلة, الدالة منتمي ' " + CALL_FUN + " ' داخل الصنف ' " + CALL_WIN_OR_CLASS + "()' تأخد " + IntToString(CALL_ARG_TOTAL) + " خاصية، ثم إنشاء فقط " + IntToString(CURRENT_ARG_NUMBER) + " خاصية ", o_tokens);
			else
				ErrorCode("خصائص قليلة, الدالة المحلية ' " + CALL_FUN + "()' تأخد " + IntToString(CALL_ARG_TOTAL) + " خاصية، ثم إنشاء فقط " + IntToString(CURRENT_ARG_NUMBER) + " خاصية ", o_tokens);
		}
	}

	return CPP_CODE;
}

// ====================================================

void FINAL_CURRENT_FILE_CODE_CHECKING(CLASS_TOKEN *o_tokens)
{
	//if(DEBUG)DEBUG_MESSAGE("FINAL_CURRENT_FILE_CODE_CHECKING()... \n", o_tokens); // DEBUG

	// Function()
	if (IsInsideFunction)
		ErrorCode("يجب إغلاق الدالة : " + TheFunction, o_tokens);

	// Window()
	if (IsInsideWindow)
		ErrorCode("يجب إغلاق النافذة : " + TheWindow, o_tokens);

	// Class()
	if (IsInsideClass)
		ErrorCode("يجب إغلاق الصنف : " + TheClass, o_tokens);

	// IF
	if (ALIF_IF_STATUS > 0)
		ErrorCode("مازال هناك " + IntToString(ALIF_IF_STATUS) + " شروط مازالت مفتوحه", o_tokens);

	// WHILE
	if (ALIF_LOOP_STATUS > 0)
		ErrorCode("مازال هناك " + IntToString(ALIF_LOOP_STATUS) + " كلما مازالت مفتوحه", o_tokens);

	// #Alif
	if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
		ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى", o_tokens);
}

// ====================================================

void FINAL_APPLICATION_CODE_CHECKING(CLASS_TOKEN *o_tokens, bool FIRST_FILE)
{
	if (FIRST_FILE)
	{
		if(DEBUG)DEBUG_MESSAGE("FINAL_APPLICATION_CODE_CHECKING()... \n", o_tokens); // DEBUG

		// Main Window()
		//if (!MAIN_WIN_IS_SET && !CONTROL_WIN_IS_SET["رئيسية"])
			//ErrorCode("النافذة الرئيسية غير موجودة", o_tokens);

		// TODO: IDE dont catch the error log.

		if (APP_TYPE == "PC_GUI")
		{
			if (!MAIN_WIN_IS_SET)
				ErrorCode("هذا التطبيق ذي واجهة رسومية، لكن النافذة الرئيسية غير محددة، هل نسيت ' نافذة رئيسية ' ؟", o_tokens);
		}
		
		// #Alif
		if (!ALIF_FLAG_FILE[o_tokens->PATH_FULL_SOURCE])
			ErrorCode("يجب الإعلان عن علم ألف اولا، المرجو اضافة ' #ألف ' في الأعلى", o_tokens);
	}
}

// ====================================================

/*
bool IsValidName(string Name) 
{
	int I = 0;
	string Char;
	bool First = true;
	
	while (I < CharCount_utf8(Name))
	{
		Char = substr_utf8(Name, I, 1);
		
		if (Char == "0" && 
			Char == "1" && 
			Char == "2" && 
			Char == "3" && 
			Char == "4" && 
			Char == "5" && 
			Char == "5" && 
			Char == "6" && 
			Char == "8" && 
			Char == "9" && 
			
			Char == "۰" && 
			Char == "۱" && 
			Char == "۲" && 
			Char == "۳" && 
			Char == "۴" && 
			Char == "۵" && 
			Char == "۶" && 
			Char == "۷" && 
			Char == "۸" && 
			Char == "۹")
		{
			if (First) 
			{
				////cout << "VALID_NAME - false.First : |" << Char << "|" << endl;
				return false;
			}
		}
		
		else if (Char !=  "ٱ" && 
					Char !=  "ٲ" && 
					Char !=  "ٳ" && 
					Char !=  "ٵ" && 
					Char !=  "ٶ" && 
					Char !=  "ٷ" && 
					Char !=  "ٸ" && 
					Char !=  "ٹ" && 
					Char !=  "ٺ" && 
					Char !=  "ٻ" && 
					Char !=  "ټ" && 
					Char !=  "ٽ" && 
					Char !=  "پ" && 
					Char !=  "ٿ" && 
					Char !=  "ڀ" && 
					Char !=  "ځ" && 
					Char !=  "ڂ" && 
					Char !=  "ڃ" && 
					Char !=  "ڄ" && 
					Char !=  "څ" && 
					Char !=  "چ" && 
					Char !=  "ڇ" && 
					Char !=  "ڈ" && 
					Char !=  "ډ" && 
					Char !=  "ڊ" && 
					Char !=  "ڋ" && 
					Char !=  "ڌ" && 
					Char !=  "ڍ" && 
					Char !=  "ڎ" && 
					Char !=  "ڏ" && 
					Char !=  "ڐ" && 
					Char !=  "ڑ" && 
					Char !=  "ڒ" && 
					Char !=  "ړ" && 
					Char !=  "ڔ" && 
					Char !=  "ڕ" && 
					Char !=  "ږ" && 
					Char !=  "ڗ" && 
					Char !=  "ژ" && 
					Char !=  "ڙ" && 
					Char !=  "ښ" && 
					Char !=  "ڛ" && 
					Char !=  "ڜ" && 
					Char !=  "ڝ" && 
					Char !=  "ڞ" && 
					Char !=  "ڟ" && 
					Char !=  "ڠ" && 
					Char !=  "ڡ" && 
					Char !=  "ڢ" && 
					Char !=  "ڣ" && 
					Char !=  "ڤ" && 
					Char !=  "ڥ" && 
					Char !=  "ڦ" && 
					Char !=  "ڧ" && 
					Char !=  "ڨ" && 
					Char !=  "ک" && 
					Char !=  "ڪ" && 
					Char !=  "ګ" && 
					Char !=  "ڬ" && 
					Char !=  "ڭ" && 
					Char !=  "ڮ" && 
					Char !=  "گ" && 
					Char !=  "ڰ" && 
					Char !=  "ڱ" && 
					Char !=  "ڲ" && 
					Char !=  "ڳ" && 
					Char !=  "ڴ" && 
					Char !=  "ڵ" && 
					Char !=  "ڶ" && 
					Char !=  "ڷ" && 
					Char !=  "ڸ" && 
					Char !=  "ڹ" && 
					Char !=  "ں" && 
					Char !=  "ڻ" && 
					Char !=  "ڼ" && 
					Char !=  "ڽ" && 
					Char !=  "ھ" && 
					Char !=  "ڿ" && 
					Char !=  "ۀ" && 
					Char !=  "ہ" && 
					Char !=  "ۂ" && 
					Char !=  "ۃ" && 
					Char !=  "ۄ" && 
					Char !=  "ۅ" && 
					Char !=  "ۆ" && 
					Char !=  "ۇ" && 
					Char !=  "ۈ" && 
					Char !=  "ۉ" && 
					Char !=  "ۊ" && 
					Char !=  "ۋ" && 
					Char !=  "ی" && 
					Char !=  "ۍ" && 
					Char !=  "ێ" && 
					Char !=  "ۏ" && 
					Char !=  "ې" && 
					Char !=  "ۑ" && 
					Char !=  "ٮ" && 
					Char !=  "ٯ" && 
					Char !=  "ف" && 
					Char !=  "ق" && 
					Char !=  "ك" && 
					Char !=  "ل" && 
					Char !=  "م" && 
					Char !=  "ن" && 
					Char !=  "ه" && 
					Char !=  "و" && 
					Char !=  "ى" && 
					Char !=  "ي" && 
					Char !=  "ء" && 
					Char !=  "آ" && 
					Char !=  "أ" && 
					Char !=  "ؤ" && 
					Char !=  "إ" && 
					Char !=  "ئ" && 
					Char !=  "ا" && 
					Char !=  "ب" && 
					Char !=  "ة" && 
					Char !=  "ت" && 
					Char !=  "ث" && 
					Char !=  "ج" && 
					Char !=  "ح" && 
					Char !=  "خ" && 
					Char !=  "د" && 
					Char !=  "ذ" && 
					Char !=  "ر" && 
					Char !=  "ز" && 
					Char !=  "س" && 
					Char !=  "ش" && 
					Char !=  "ص" && 
					Char !=  "ض" && 
					Char !=  "ط" && 
					Char !=  "ظ" && 
					Char !=  "ع" && 
					Char !=  "غ" && 
					Char !=  "؋" &&
					
					Char !=  "a" &&
					Char !=  "b" &&
					Char !=  "c" &&
					Char !=  "d" &&
					Char !=  "e" &&
					Char !=  "f" &&
					Char !=  "g" &&
					Char !=  "h" &&
					Char !=  "i" &&
					Char !=  "j" &&
					Char !=  "k" &&
					Char !=  "l" &&
					Char !=  "m" &&
					Char !=  "n" &&
					Char !=  "o" &&
					Char !=  "p" &&
					Char !=  "q" &&
					Char !=  "r" &&
					Char !=  "s" &&
					Char !=  "t" &&
					Char !=  "u" &&
					Char !=  "v" &&
					Char !=  "w" &&
					Char !=  "x" &&
					Char !=  "y" &&
					Char !=  "z")
		{
			////cout << "VALID_NAME - false : |" << Char << "|" << endl;
			return false;
		}
		
		First = false;
		I++;
	} // End char loop.
	
	return true;
}
*/
