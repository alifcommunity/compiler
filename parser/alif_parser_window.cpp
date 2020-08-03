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

std::string GetFormatedHTML(std::string HTML, std::string WindowName, CLASS_TOKEN *o_tokens){

	std::string FormatedHTML;
	
	/*
	std::istringstream isCode(Code);
    std::string Line;
    while (std::getline(isCode, Line)){

		std::size_t p = Line.find("ألف:");
		if (p != std::string::npos){

			std::string AlifScript;

			// ... ألف:رئيسية:الدالة_test(10, 20, 'test_1') ...
			// 6 = 'ألف'
			// 1 = ':'
			//AlifScript = Line.substr(p + 7);
			AlifScript = substr_utf8(Line, p + 4, 0);

			int LINE_CHAR_TOTAL = 0;
			int CHAR_NUMBER = 0;
			string Char;
			LINE_CHAR_TOTAL = CharCount_utf8(AlifScript, o_tokens);
			bool INSIDE_STRING = false;

			int PramCount = 0;
			string Pram;

			while (CHAR_NUMBER < LINE_CHAR_TOTAL){
				Char = substr_utf8(AlifScript, CHAR_NUMBER, 1);

				if (Char == "'")
					Char = "\"";

				if (Char == "\""){
					if (INSIDE_STRING){
						// String End
						// Add char, and pram is ready
						Pram.append(Char);
						PramCount++;
					}
				}
			}

			//ErrorCode(" ####### Script : ||" + AlifScript + "||", o_tokens);
		} else {
			FormatedHTML.append(Line);
		}
    }
	*/

	// --- Reformating HTML script ------------------

	std::istringstream ScriptStream(HTML);
    std::string Line;
    while (std::getline(ScriptStream, Line)){

		// <img src="..." ...>
		// <img src="data:image/png;base64,..." ...>
		// Note: src=\" -> 6

		std::size_t p_1;
		std::size_t p_2;
		std::string LineBuffer;
		std::string LineLower;

		(void)p_2; // Just to skip --> warning: variable ‘p_2’ set but not used [-Wunused-but-set-variable]

		LineLower = boost::algorithm::to_lower_copy(Line);
		
		p_1 = LineLower.find(" src=\"");
		if (p_1 != std::string::npos){
			
			p_1 = p_1 + 6;

			/*
			#ifdef __linux__
				// src="file:///myfolder/test.jpg"
				if(substr_utf8(Line, p_1, 8) == ""){

				}
			#endif
			*/
			
			LineBuffer = substr_utf8(Line, p_1, 0);

			//if(DEBUG)DEBUG_MESSAGE("LineBuffer 000 |" + LineBuffer + "| \n\n", o_tokens); // DEBUG

			std::size_t p_2 = Line.find('\"', p_1);
			if (p_2 != std::string::npos){

				LineBuffer = boost::algorithm::trim_copy(substr_utf8(Line, p_1, (p_2 - p_1)));
				//LineBuffer = substr_utf8(Line, p_1, (p_2 - p_1));

				// Fix: src="file://..."
				if(substr_utf8(LineBuffer, 0, 7) == "file://"){
					
					boost::algorithm::replace_first(LineBuffer, "file://", "");
					if(DEBUG)DEBUG_MESSAGE("file and fixed |" + LineBuffer + "| \n\n", o_tokens); // DEBUG
				}

				const std::string c_URI = LineBuffer;

				//if(DEBUG)DEBUG_MESSAGE("LineBuffer 111111 |" + LineBuffer + "| \n\n", o_tokens); // DEBUG

				// Fix: src="http://..."
				//std::size_t p_h = LineBuffer.find("http://");
				//std::size_t p_hs = LineBuffer.find("https://");
				//if (p_h != std::string::npos || p_hs != std::string::npos){
				if(substr_utf8(LineBuffer, 0, 7) == "http://" || substr_utf8(LineBuffer, 0, 8) == "https://"){

					// Allow external link..
					FormatedHTML.append(Line);
					//if(DEBUG)DEBUG_MESSAGE("http 0 |" + LineBuffer + "| next \n\n", o_tokens); // DEBUG
					continue;
				}
				
				std::string FileType = LineBuffer.substr(LineBuffer.find_last_of(".") + 1);

				// .apng								-> image/apng
				// .bmp									-> image/bmp
				// .gif									-> image/gif
				// .ico / .cur							-> image/x-icon
				// .jpg / .jpeg / .jfif / .pjpeg / .pjp	-> image/jpeg
				// .png									-> image/png
				// .svg									-> image/svg+xml
				// .tif / .tiff							-> image/tiff
				// .webp								-> image/webp

				if (FileType == "jpg" || FileType == "jpeg" || FileType == "jfif" || FileType == "pjpeg" || FileType == "pjp"){
					FileType = "image/jpeg";
				} else if (FileType == "png"){
					FileType = "image/png";
				} else if (FileType == "gif"){
					FileType = "image/gif";
				} else if (FileType == "apng"){
					FileType = "image/apng";
				} else if (FileType == "bmp"){
					FileType = "image/bmp";
				} else if (FileType == "ico" || FileType == "cur"){
					FileType = "image/x-icon";
				} else if (FileType == "tif" || FileType == "tiff"){
					FileType = "image/tiff";
				} else if (FileType == "svg"){
					FileType = "image/svg+xml";
				} else if (FileType == "webp"){
					FileType = "image/webp";
				} else {
					
					//if(DEBUG)DEBUG_MESSAGE("Unknow 0 |" + FileType + "| next \n\n", o_tokens); // DEBUG
					// Unknow file type
					// so, skip embeding it.
					FormatedHTML.append(Line);
					continue;
				}

				// --- BinToBase64 Data ------------------------
				std::ifstream FileStream(c_URI, std::ios::binary);
				std::string FileData((std::istreambuf_iterator<char>(FileStream)), std::istreambuf_iterator<char>());
				std::string DataBase64 = base64_encode(reinterpret_cast<const unsigned char*>(FileData.c_str()), FileData.length());
				// ---------------------------------------------

				boost::algorithm::replace_first(Line, c_URI, ("data:" + FileType + ";base64," + DataBase64));

				//if(DEBUG)DEBUG_MESSAGE("=======> c_URI |" + c_URI + "| \n\n", o_tokens); // DEBUG
				//if(DEBUG)DEBUG_MESSAGE("=======> FileType |" + FileType + "| \n\n", o_tokens); // DEBUG
				//if(DEBUG)DEBUG_MESSAGE("=======> Line |" + Line + "| \n\n", o_tokens); // DEBUG
			}
		}

		FormatedHTML.append(Line + '\n');
		
		p_1 = 0;
		p_2 = 0;
		LineBuffer = "";
		LineLower = "";
		Line = "";
	}

	//ErrorCode("END -> ", o_tokens);

	if(!replace_end(FormatedHTML, "</html>", AlifJavaScript_Bridge)){
		ErrorCode("شيفرة واجهة ويب، لا تحتوي على '</html>' -> " + WindowName, o_tokens);
	}
	
	// --- Bin2C Data ------------------------------
	// BinaryToC() work.. but result need extra work.
	// BinaryToC("D:\\Code\\Alif_App\\Alif_2\\test.jpg", "D:\\Code\\Alif_App\\Alif_2\\test.jpg.cpp");
	// BinaryToC("D:\\Code\\Alif_App\\Alif_2\\test2.png", "D:\\Code\\Alif_App\\Alif_2\\test2.png.cpp");
	// ---------------------------------------------

	return FormatedHTML;
}

void HTML_to_c(string sHTMLPath, string sCPath, string VarName, string WindowName, CLASS_TOKEN *o_tokens){
	// This function can be completly replace in c++20 by <embed>
	// In: test.html
	// Out: test.c -> const test_content = "..test.html..";

	// --- Read --------------------------
	std::ifstream rBuffer(sHTMLPath);
	std::stringstream sHTML;
	sHTML << rBuffer.rdbuf();

	// --- HTML to C++ -------------------
	string sHTMLCode = "<!-- Alif compiler " + VERSION + " - HTML Start --> \n" 
						+ 
						// --- Get formated AlifJavaScript ---
						GetFormatedHTML(sHTML.str(), WindowName, o_tokens)
						//sHTML.str() 
						+ 
						" \n<!-- Alif compiler " + VERSION + " - HTML End -->";

	//string::size_type pos = 0;
    //while ((pos = sHTMLCode.find('\"', pos)) != string::npos){
    //    sHTMLCode.replace(pos, 1, "\\\"");
    //    pos += 2;
    //}

	string cppCode_start = "static const wxString " + VarName + " = wxT( R\"V0G0N( \n\n";
	string cppCode_end = "\n\n )V0G0N\" );";
	string cppCode = R"( 
		// --- Const HTML script --------------------------
		)" + cppCode_start + sHTMLCode + cppCode_end + R"(
		// ------------------------------------------------
		wxCharBuffer )" + VarName  + R"(_CharBuffer = )" + VarName  + R"(.ToUTF8();
		wxMemoryFSHandler::AddFile(")" + VarName + R"(.htm", )" + VarName + R"(_CharBuffer, strlen()" + VarName + R"(_CharBuffer ) );
	)";

	// --- Write -----------------------
	std::ofstream cFile(sCPath);
	cFile << cppCode;
}

void parser_NewWindowWeb(string Token[2048], CLASS_TOKEN *o_tokens){

	// #واجهة_ويب رئيسية "UI_WEB_1"
	// #window_web MyWindow "MyFile.html"

	if (IsInsideWindow)
		ErrorCode("لا يمكن انشاء نافدة داخل نافدة، النافذة الحالية : " + TheWindow, o_tokens);
	
	if (IsInsideClass)
		ErrorCode("لا يمكن انشاء نافذة داخل صنف : " + TheClass, o_tokens);
	
	if (IsInsideFunction)
		ErrorCode("لا يمكن انشاء نافذة داخل دالة : " + TheFunction, o_tokens);
	
	if (Token[3] == "")
		ErrorCode("يجب تحديد اسم النافذة", o_tokens);
	
	if (!IsValidName(Token[3], o_tokens))
		ErrorCode("اسم غير مقبول : ' " + Token[3] + " ' ", o_tokens);

	if (Token[4] == "")
		ErrorCode("يجب تحديد اسم الملف", o_tokens);

	if(!IsValidStringFormat(Token[4], o_tokens))
		ErrorCode("خطأ في كتابة إسم الملف: "+ Token[4], o_tokens);
	
	// ------------------------

	if (Token[3] == "رئيسية"){

		// temporary set because this window
		// didnt created yet, but we need
		// this window Generated_ID now.
		TheWindow = "رئيسية";

		// --- WindowWeb Main --------------------

		if (!o_tokens->TOKENS_PREDEFINED){
			
			if (CONTROL_WIN_IS_SET["رئيسية"])
				ErrorCode("النافذة ' رئيسية ' تم انشاؤها مسبقا في السطر رقم : " + CONTROL_WIN_AT_LINE["رئيسية"], o_tokens);
			
			//MAIN_WIN_IS_SET = true;
			//MAIN_WIN_AT_LINE = IntToString(o_tokens->Line);
			CONTROL_WIN_IS_SET["رئيسية"] = true;
			WIN_IS_WEB["رئيسية"] = true;
			//Win_CurrentTotalFucntion = 0;
			CONTROL_WIN_AT_LINE["رئيسية"] = IntToString(o_tokens->Line);
			CPP_WINDOW[std::make_pair("رئيسية", "شكل")] = " wxDEFAULT_FRAME_STYLE ";
			APP_TYPE = "PC_GUI";

			// temporary end
			TheWindow = "";

			return; // continue;
		}

		if(DEBUG)DEBUG_MESSAGE("[WINDOW_WEB] [MAIN] {SET BASE CTR} \n\n", o_tokens); // DEBUG

		/*
		if (CONTROL_WIN_IS_SET["رئيسية"]){

			// Main Window already set by control (Alif lang UI)
			ErrorCode("النافذه الرئيسية تم انشاؤها مسبقا في السطر : " + CONTROL_WIN_AT_LINE[(Token[3])], o_tokens);
		}
		*/

		// Not need this because we are in: #win web "MyWindow.alifui"
		//IsInsideWindow = true;
		//TheWindow = "رئيسية";
		//Win_CurrentTotalFucntion = 0;
		//APP_TYPE = "PC_GUI";

		// *** Generate Code ***
		// New Web Window -> WebUI construction
		CBUFER_ID = "ID_CTR_" + ID["رئيسية"] + "_" + Control_ID["AlifUIWeb"];
		CBUFER_OBJ = "OBJ_CTR_" + ID["رئيسية"] + "_" + Control_ID["AlifUIWeb"];
		string CBUFER_VarName = ID["رئيسية"] + "_ConstHTML";
		CPP_ID_DECLARATION.append(" int " + CBUFER_ID + " = ALIFCORE_ID_GENERATOR(); \n");
		CPP_OBJ_DECLARATION.append(" wxWebView* " + CBUFER_OBJ + "; \n");
		// New Web Window -> WebUI HTML
		string PATH_FULL_HTML_CPP = PATH_TEMP + SEPARATION + "alifcompiler_html_" + ID["رئيسية"] + "_" + RANDOM + ".cpp";
		HTML_to_c(PATH_FULL_WINDOW_WEB, PATH_FULL_HTML_CPP, CBUFER_VarName, "رئيسية", o_tokens);
		CPP_WINDOW[std::make_pair("رئيسية", "CTR_CONSTRUCTOR")] = 
		"wxBoxSizer* o_TopSizer = new wxBoxSizer(wxVERTICAL); P->SetSizer(o_TopSizer); " + CBUFER_OBJ + " = wxWebView::New(P, wxID_ANY, \"\", wxPoint(10,10), wxSize(100, 100), wxWebViewBackendDefault, wxBORDER_NONE, wxT(\"Alif_Application\")); o_TopSizer->Add(" + CBUFER_OBJ + ", wxSizerFlags().Expand().Proportion(1)); " + CBUFER_OBJ + "->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewArchiveHandler(\"wxfs\"))); " + CBUFER_OBJ + "->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler(\"memory\")));\n #include \"" + PATH_FULL_HTML_CPP + "\" \n " + CBUFER_OBJ + "->LoadURL(\"memory:" + CBUFER_VarName + ".htm\"); \n Center(); \n Bind(wxEVT_WEBVIEW_NEWWINDOW, &CLASS_WINDOW_" + ID["رئيسية"] + "::AlifJavaScript_OnNewWindow, this, " + CBUFER_OBJ + "->GetId()); \n Bind(wxEVT_WEBVIEW_NAVIGATING, &CLASS_WINDOW_" + ID["رئيسية"] + "::AlifJavaScript_OnNavigationRequest, this, " + CBUFER_OBJ + "->GetId()); \n " + CBUFER_OBJ + "->EnableContextMenu(false); \n " + CBUFER_OBJ + "->EnableHistory(false); \n ";

		// " + CBUFER_OBJ + "->SetEditable(false); " 
		// + CBUFER_OBJ + "->EnableContextMenu(false); " 
		// + CBUFER_OBJ + "->EnableHistory(false); " 
		// + CBUFER_OBJ + "->Enable(true); \n 

		// New Web Window -> WebUI AlifJavaScript
		CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
		CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " \n void AlifJavaScript_OnNewWindow(wxWebViewEvent& evt); \n void AlifJavaScript_OnNavigationRequest(wxWebViewEvent& evt); \n void AlifJavaScript_Error(wxString sError); void AlifJavaScript_Run(wxString JSCode); \n void AlifJavaScriptBridge(wxString sLink); \n ";
		// New Web Window -> Basic Window settings
		//CPP_WINDOW[std::make_pair(TheWindow, "CTR_CONSTRUCTOR")] = " Center(); \n ";
		// x = افصول
		// y = ارتوب
		/*
		CPP_WINDOW[std::make_pair(TheWindow, "نص")] = " (" + TheWindow + ") نافدة بدون عنوان";
		#ifdef __APPLE__
			CPP_WINDOW[std::make_pair(TheWindow, "افصول")] 	= "10";
			CPP_WINDOW[std::make_pair(TheWindow, "ارتوب")] 	= "25"; // Because the macOS task-bar is on top screen.
		#else
			CPP_WINDOW[std::make_pair(TheWindow, "افصول")] = "0";
			CPP_WINDOW[std::make_pair(TheWindow, "ارتوب")] = "0";
		#endif
		CPP_WINDOW[std::make_pair(TheWindow, "عرض")] = "400";
		CPP_WINDOW[std::make_pair(TheWindow, "ارتفاع")] = "400";
		WIN_TOTAL++; // only on second windows, no main.
		WIN_TOTAL_NAMES[WIN_TOTAL] = TheWindow; // only on second windows, no main.
		CPP_ID_DECLARATION.append(" int ID_WINDOW_" + ID[TheWindow] + " = ALIFCORE_ID_GENERATOR(); \n");
		CPP_GLOBAL.append(" bool WINDOW_IS_SHOW_" + ID[TheWindow] + " = false; \n");
		CPP_GLOBAL.append(" bool WINDOW_IS_CONSTRUCTION_" + ID[TheWindow] + " = false; \n");
		CPP_GLOBAL.append(" void WINDOW_LOAD_" + ID[TheWindow] + "(); \n");
		*/
		// *** *** *** *** *** ***

		// temporary end
		TheWindow = "";
	}
	else {
		// --- WindowWeb MyWindowName --------------------

		// temporary set because this window
		// didnt created yet, but we need
		// this window Generated_ID now.
		TheWindow = Token[3];

		if (!IsValidName(Token[3], o_tokens))
			ErrorCode("اسم غير مقبول : ' " + Token[3] + " ' ", o_tokens);

		if (!o_tokens->TOKENS_PREDEFINED){
			
			if (CONTROL_WIN_IS_SET[Token[3]])
				ErrorCode("النافذة ' " + Token[3] + 
				" ' تم انشاؤها مسبقا في السطر : " + CONTROL_WIN_AT_LINE[Token[3]], o_tokens);
			
			//MAIN_WIN_IS_SET = true;
			//MAIN_WIN_AT_LINE = IntToString(o_tokens->Line);
			CONTROL_WIN_IS_SET[Token[3]] = true;
			WIN_IS_WEB[Token[3]] = true;
			//Win_CurrentTotalFucntion = 0;
			CONTROL_WIN_AT_LINE[Token[3]] = IntToString(o_tokens->Line);
			CPP_WINDOW[std::make_pair(Token[3], "شكل")] = " wxDEFAULT_FRAME_STYLE ";
			APP_TYPE = "PC_GUI";

			SET_C_NAME(Token[3]);

			// temporary end
			TheWindow = "";

			return; // continue;
		}

		if(DEBUG)DEBUG_MESSAGE("[WINDOW] [" + Token[3] + "] {SET BASE CTR} \n\n", o_tokens); // DEBUG

		// Not need this because we are in: #win web "MyWindow.alifui"
		//IsInsideWindow = true;
		//TheWindow = Token[3];
		//Win_CurrentTotalFucntion = 0;
		//APP_TYPE = "PC_GUI";

		// *** Generate Code ***
		// New Web Window -> WebUI construction
		CBUFER_ID = "ID_CTR_" + ID[Token[3]] + "_" + Control_ID["AlifUIWeb"];
		CBUFER_OBJ = "OBJ_CTR_" + ID[Token[3]] + "_" + Control_ID["AlifUIWeb"];
		string CBUFER_VarName = ID[Token[3]] + "_ConstHTML";
		CPP_ID_DECLARATION.append(" int " + CBUFER_ID + " = ALIFCORE_ID_GENERATOR(); \n");
		CPP_OBJ_DECLARATION.append(" wxWebView* " + CBUFER_OBJ + "; \n");
		// New Web Window -> WebUI HTML
		string PATH_FULL_HTML_CPP = PATH_TEMP + SEPARATION + "alifcompiler_html_" + ID[Token[3]] + "_" + RANDOM + ".cpp";
		HTML_to_c(PATH_FULL_WINDOW_WEB, PATH_FULL_HTML_CPP, CBUFER_VarName, Token[3], o_tokens);
		CPP_WINDOW[std::make_pair(Token[3], "CTR_CONSTRUCTOR")] = 
		"wxBoxSizer* o_TopSizer = new wxBoxSizer(wxVERTICAL); P->SetSizer(o_TopSizer); " + CBUFER_OBJ + " = wxWebView::New(P, wxID_ANY, \"\", wxPoint(10,10), wxSize(100, 100), wxWebViewBackendDefault, wxBORDER_NONE, wxT(\"Alif_Application\")); o_TopSizer->Add(" + CBUFER_OBJ + ", wxSizerFlags().Expand().Proportion(1)); " + CBUFER_OBJ + "->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewArchiveHandler(\"wxfs\"))); " + CBUFER_OBJ + "->RegisterHandler(wxSharedPtr<wxWebViewHandler>(new wxWebViewFSHandler(\"memory\")));\n #include \"" + PATH_FULL_HTML_CPP + "\" \n " + CBUFER_OBJ + "->LoadURL(\"memory:" + CBUFER_VarName + ".htm\"); \n Center(); \n Bind(wxEVT_WEBVIEW_NEWWINDOW, &CLASS_WINDOW_" + ID[Token[3]] + "::AlifJavaScript_OnNewWindow, this, " + CBUFER_OBJ + "->GetId()); \n Bind(wxEVT_WEBVIEW_NAVIGATING, &CLASS_WINDOW_" + ID[Token[3]] + "::AlifJavaScript_OnNavigationRequest, this, " + CBUFER_OBJ + "->GetId()); \n " + CBUFER_OBJ + "->EnableContextMenu(false); \n " + CBUFER_OBJ + "->EnableHistory(false); \n ";

		// " + CBUFER_OBJ + "->SetEditable(false); " 
		// + CBUFER_OBJ + "->EnableContextMenu(false); " 
		// + CBUFER_OBJ + "->EnableHistory(false); " 
		// + CBUFER_OBJ + "->Enable(true); \n 

		// New Web Window -> WebUI AlifJavaScript
		CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")];
		CPP_WINDOW[std::make_pair(TheWindow, "FUN_DECLARATION")] = CBUFER + " \n void AlifJavaScript_OnNewWindow(wxWebViewEvent& evt); \n void AlifJavaScript_OnNavigationRequest(wxWebViewEvent& evt); \n void AlifJavaScript_Error(wxString sError); void AlifJavaScript_Run(wxString JSCode); \n void AlifJavaScriptBridge(wxString sLink); \n ";
		// New Web Window -> Basic Window settings
		//CPP_WINDOW[std::make_pair(TheWindow, "CTR_CONSTRUCTOR")] = " Center(); \n ";
		// x = افصول
		// y = ارتوب
		
		CPP_WINDOW[std::make_pair(TheWindow, "نص")] = " (" + TheWindow + ") نافدة بدون عنوان";
		#ifdef __APPLE__
			CPP_WINDOW[std::make_pair(TheWindow, "افصول")] 	= "10";
			CPP_WINDOW[std::make_pair(TheWindow, "ارتوب")] 	= "25"; // Because the macOS task-bar is on top screen.
		#else
			CPP_WINDOW[std::make_pair(TheWindow, "افصول")] = "0";
			CPP_WINDOW[std::make_pair(TheWindow, "ارتوب")] = "0";
		#endif
		CPP_WINDOW[std::make_pair(TheWindow, "عرض")] = "400";
		CPP_WINDOW[std::make_pair(TheWindow, "ارتفاع")] = "400";
		WIN_TOTAL++; // only on second windows, no main.
		WIN_TOTAL_NAMES[WIN_TOTAL] = TheWindow; // only on second windows, no main.
		CPP_ID_DECLARATION.append(" int ID_WINDOW_" + ID[TheWindow] + " = ALIFCORE_ID_GENERATOR(); \n");
		CPP_GLOBAL.append(" bool WINDOW_IS_SHOW_" + ID[TheWindow] + " = false; \n");
		CPP_GLOBAL.append(" bool WINDOW_IS_CONSTRUCTION_" + ID[TheWindow] + " = false; \n");
		CPP_GLOBAL.append(" void WINDOW_LOAD_" + ID[TheWindow] + "(); \n");
		// *** *** *** *** *** ***

		// temporary end
		TheWindow = "";
	}

	if(DEBUG)DEBUG_MESSAGE("#NewWindow_Web (" + Token[3] + ") [HTML " + Token[4] + " . ALIF UIWeb] \n\n", o_tokens);

	// temporary end
	TheWindow = "";
}

void parser_NewWindow(string Token[2048], CLASS_TOKEN *o_tokens){

	if (IsInsideWindow)
		ErrorCode("لا يمكن انشاء نافدة داخل نافدة، النافذة الحالية : " + TheWindow, o_tokens);
	
	if (IsInsideClass)
		ErrorCode("لا يمكن انشاء نافذة داخل صنف : " + TheClass, o_tokens);
	
	if (IsInsideFunction)
		ErrorCode("لا يمكن انشاء نافذة داخل دالة : " + TheFunction, o_tokens);
	
	if (Token[2] == "")
		ErrorCode("يجب تحديد اسم النافذة", o_tokens);
	
	if (Token[3] != "" && Token[3] != "(")
			ErrorCode("أمر غير معروف : ' " + Token[3] + " ' ", o_tokens);
	
	if (Token[3] == "(")
	{
		if (Token[4] == "")
			ErrorCode("يجب اغلاق ')' ", o_tokens);
			
		if (Token[4] != ")")
			ErrorCode("لا يمكن اعطاء خصائص لنافدة", o_tokens);
			
		if (Token[5] != "")
			ErrorCode("أمر غير معروف : ' " + Token[5] + " ' ", o_tokens);
	}
	
	if (Token[2] == "رئيسية") // نافدة رئيسية
	{
		if (!o_tokens->TOKENS_PREDEFINED)
		{
			if (MAIN_WIN_IS_SET)
				ErrorCode("النافذه الرئيسية تم انشاؤها مسبقا في السطر : " + MAIN_WIN_AT_LINE, o_tokens);
			
			MAIN_WIN_IS_SET = true;
			MAIN_WIN_AT_LINE = IntToString(o_tokens->Line);
			WIN_IS_WEB["رئيسية"] = (WIN_IS_WEB["رئيسية"] ? true : false);

			IsInsideWindow = true; // Need by Tokens Predefined
			TheWindow = "رئيسية"; // Need by Tokens Predefined
			Win_CurrentTotalFucntion = 0;
			APP_TYPE = "PC_GUI";

			return; // continue;
		}

		IsInsideWindow = true;
		TheWindow = "رئيسية";
		Win_CurrentTotalFucntion = 0;
		APP_TYPE = "PC_GUI";
		
		if (CONTROL_WIN_IS_SET["رئيسية"])
		{
			if(DEBUG)DEBUG_MESSAGE("[WINDOW] [MAIN] {CTR ALREADY SET} \n\n", o_tokens); // DEBUG
			
			return; // continue;
		}
		else
		{
			if(DEBUG)DEBUG_MESSAGE("[WINDOW] [MAIN] {SET BASE CTR} \n\n", o_tokens); // DEBUG
			
			// *** Generate Code ***
			// CG_INITIALIZATION() // already done by 'alif.cpp'
			CPP_WINDOW[std::make_pair("رئيسية", "CTR_CONSTRUCTOR")] = " Center(); \n ";
			// *** *** *** *** *** ***
			
			return; // continue;
		}
	}
	else // نافدة MyWindowName
	{
		if (!IsValidName(Token[2], o_tokens))
			ErrorCode("اسم غير مقبول : ' " + Token[2] + " ' ", o_tokens);
		
		if (!o_tokens->TOKENS_PREDEFINED)
		{
			if (WIN_IS_SET[Token[2]])
				ErrorCode("النافذة ' " + Token[2] + " ' تم انشاؤها مسبقا في السطر : " + WIN_AT_LINE[Token[2]], o_tokens);
			
			WIN_IS_SET[Token[2]] = true;
			WIN_IS_WEB[Token[2]] = (WIN_IS_WEB[Token[2]] ? true : false);
			WIN_AT_LINE[Token[2]] = IntToString(o_tokens->Line);

			IsInsideWindow = true; // Need by Tokens Predefined
			TheWindow = Token[2]; // Need by Tokens Predefined
			Win_CurrentTotalFucntion = 0;
			APP_TYPE = "PC_GUI";

			SET_C_NAME(Token[2]);
			return; // continue;
		}

		IsInsideWindow = true;
		TheWindow = Token[2];
		Win_CurrentTotalFucntion = 0;
		APP_TYPE = "PC_GUI";
		
		if (CONTROL_WIN_IS_SET[Token[2]])
		{
			if(DEBUG)DEBUG_MESSAGE("[WINDOW] [" + Token[2] + "] {CTR ALREADY SET} \n\n", o_tokens); // DEBUG
			return; // continue;
		}
		else
		{
			if(DEBUG)DEBUG_MESSAGE("[WINDOW] [" + Token[2] + "] {SET BASE CTR} \n\n", o_tokens); // DEBUG

			// *** Generate Code ***
			// New Window
			// x = افصول
			// y = ارتوب
			CPP_WINDOW[std::make_pair(Token[2], "نص")] = " (" + Token[2] + ") نافدة بدون عنوان";
			#ifdef __APPLE__
				CPP_WINDOW[std::make_pair(Token[2], "افصول")] 	= "10";
				CPP_WINDOW[std::make_pair(Token[2], "ارتوب")] 	= "25"; // Because the macOS task-bar is on top screen.
			#else
				CPP_WINDOW[std::make_pair(Token[2], "افصول")] = "0";
				CPP_WINDOW[std::make_pair(Token[2], "ارتوب")] = "0";
			#endif
			CPP_WINDOW[std::make_pair(Token[2], "عرض")] = "400";
			CPP_WINDOW[std::make_pair(Token[2], "ارتفاع")] = "400";
			WIN_TOTAL++;
			WIN_TOTAL_NAMES[WIN_TOTAL] = Token[2];
			CPP_ID_DECLARATION.append(" int ID_WINDOW_" + ID[Token[2]] + " = ALIFCORE_ID_GENERATOR(); \n");
			CPP_GLOBAL.append(" bool WINDOW_IS_SHOW_" + ID[Token[2]] + " = false; \n");
			CPP_GLOBAL.append(" bool WINDOW_IS_CONSTRUCTION_" + ID[Token[2]] + " = false; \n");
			CPP_GLOBAL.append(" void WINDOW_LOAD_" + ID[Token[2]] + "(); \n");
			// *** *** *** *** *** ***
			
			return; // continue;
		}
	}
}
