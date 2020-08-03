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
// C++ Add Code to the current Function
// ----------------------------------

/*
void Script(string Code){

	// CPP_ID_DECLARATION
	// CPP_GLOBAL
	// CPP_OBJ_DECLARATION
	// Windows const..
	// Event..
	// 		CPP_CLASS
	// 		CPP_FUN
	// CPP_GLOBAL_FUN

	if (IsInsideClass){

		// Local Class Function
		CPP_CLASS.append(NEW_CODE);
	}
	else
	{
		// Local Window Function
		// Local Window Main Function

		if (FUN != "رئيسية")
		{
			// Normal Function
			CPP_FUN.append(NEW_CODE);
		}
		else
		{
			// Main Function
			CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "LOAD")];
			CPP_WINDOW[std::make_pair(TheWindow, "LOAD")] = CBUFER + NEW_CODE;
		}
	}
}
*/

void cpp_AddScript(string FUN, string NEW_CODE)
{
	if (IsInsideClass)
	{
		// Local Class Function
		CPP_CLASS.append(NEW_CODE);
	}
	else
	{
		// Local Window Function
		// Local Window Main Function

		if (FUN != "رئيسية")
		{
			// Normal Function
			CPP_FUN.append(NEW_CODE);
		}
		else
		{
			// Main Function
			CBUFER = CPP_WINDOW[std::make_pair(TheWindow, "LOAD")];
			CPP_WINDOW[std::make_pair(TheWindow, "LOAD")] = CBUFER + NEW_CODE;
		}
	}
}

// ----------------------------------
// WinX Load()
// ----------------------------------

string CG_WINX_LOAD()
{
	string CODE;
	string BUFFER;

	for (int i = 1; i <= WIN_TOTAL; i++) 
	{
		BUFFER = R"( 

// ========================================================================
// ------------------------------------------------
// Window X )" + ID[WIN_TOTAL_NAMES[i]] + R"( Load()
// ------------------------------------------------

void WINDOW_LOAD_)" + ID[WIN_TOTAL_NAMES[i]] + R"(()
{
	)" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "LOAD")] + R"(
}
// ========================================================================

		)"; 

		CODE.append(BUFFER);
	}

	return CODE;
}

// ----------------------------------
// Application Destroy()
// ----------------------------------

string CG_APP_DESTROY()
{
	string CODE;
	string BUFFER;

	BUFFER = R"( 
	// ------------------------------------------------
	// Application Destroy
	// ------------------------------------------------
	)";

	CODE.append(BUFFER);

	for (int i = 1; i <= WIN_TOTAL; i++) 
	{
		BUFFER = R"( 
		// Window X )" + ID[WIN_TOTAL_NAMES[i]] + R"( Destroy
		// ---------
		//#ifdef __APPLE__
			//if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN_TOTAL_NAMES[i]] + R"() OBJ_CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(->EndModal(0);
		//#else
			if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN_TOTAL_NAMES[i]] + R"() OBJ_CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(->Destroy();
		//#endif
		)";

		CODE.append(BUFFER);
	}

	BUFFER = R"( 
	// ---------
	// Win Main Destroy
	// ---------
	//#ifdef __APPLE__
		//if (WINDOW_IS_CONSTRUCTION_)" + ID["رئيسية"] + R"() OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(->EndModal(0);
	//#else
		if (WINDOW_IS_CONSTRUCTION_)" + ID["رئيسية"] + R"() OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(->Destroy();
	//#endif
	// ------------------------------------------------
	)";

	CODE.append(BUFFER);

	return CODE;
}

// ----------------------------------
// Window Show ()
// ----------------------------------

string CG_WIN_SHOW(string WIN)
{
	string CODE = R"(
	// -------------------------------
	// Win Show ()" + WIN + R"()
	// -------------------------------
	if (!WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"()
	{
		OBJ_CLASS_WINDOW_)" + ID[WIN] + R"( = new CLASS_WINDOW_)" + ID[WIN] + R"(();
		OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->SetLayoutDirection(wxLayout_RightToLeft);
		WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"( = true;
	}
	OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->Show(true);
	OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->SetFocus();
	WINDOW_IS_SHOW_)" + ID[WIN] + R"( = true;
	WINDOW_LOAD_)" + ID[WIN] + R"(();
	// -------------------------------		
				)";

	return CODE;
}

// ----------------------------------
// Window Center ()
// ----------------------------------

string CG_WIN_CENTER(string WIN)
{
	string CODE = R"(
	// -------------------------------
	// Win Center ()" + WIN + R"()
	// -------------------------------
	if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"()
	{
		OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->Center();
	}
	// -------------------------------		
	)";

	return CODE;
}

// ----------------------------------
// Window Hide ()
// ----------------------------------

string CG_WIN_HIDE(string WIN)
{
	string CODE = R"(
	// -----------------------------------------------
	// Win Hide
	// -----------------------------------------------
	if (WINDOW_IS_SHOW_)" + ID[WIN] + R"()
	{
		OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->Show(false);
		WINDOW_IS_SHOW_)" + ID[WIN] + R"( = false;
	}
	// -----------------------------------------------		
				)";

	return CODE;
}

// ----------------------------------
// Window Destroy ()
// ----------------------------------

string CG_WIN_DESTROY(string WIN)
{
	string CODE = R"(
	// -----------------------------------------------
	// Win Destroy
	// -----------------------------------------------
	if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"()
	{
		//#ifdef __APPLE__
			//OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->EndModal(0); // Hum, maybe this is only for wxDialog, and not wxFrame !
		//#else
			OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->Destroy();
		//#endif
		WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"( = false;
	}
	// -----------------------------------------------	
	)";

	return CODE;
}

// ----------------------------------
// Window : members
// ----------------------------------

string CG_WIN_MEMBER(string WIN, string MEMBER_CODE)
{
	// Used by Operator:xx:yy

	string CODE = R"(
	// -----------------------------------------------
	// Win Members
	// -----------------------------------------------
	if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"()
	{
		OBJ_CLASS_WINDOW_)" + ID[WIN] + R"(->)" + MEMBER_CODE + R"( )";

	return CODE;
}

// ----------------------------------
// Window : members
// ----------------------------------
/*

// We will need this, if we set a way to delete a control by alif code.

string CG_CONTROL_MEMBER(string WIN, string CONTROL, string MEMBER_CODE)
{
	string CODE = R"(
	// -----------------------------------------------
	// Control Members
	// -----------------------------------------------
	//if (WINDOW_IS_CONSTRUCTION_)" + ID[WIN] + R"()
	//{
		OBJ_)" + ID[WIN] + R"(_)" + ID[CONTROL] + R"(->)" + MEMBER_CODE + R"( )";

	return CODE;
}
*/

// ----------------------------------
// AlifJavaScript Generator
// ----------------------------------

string AlifJavaScript_Bridge_SendRequest(){

	#ifdef _WIN32
		return " window.open(link); ";
	#elif  __APPLE__
		return " window.open(link); ";
	#else
		return " document.title = link; window.open('http://AlifJSBridge'); document.title = ''; ";
	#endif
}

string AlifJavaScript_Bridge_OnNewWindowSendRequest(string WVObjName){

	#ifdef _WIN32
		return " this->AlifJavaScriptBridge(evt.GetURL()); ";
	#elif  __APPLE__
		return " this->AlifJavaScriptBridge(evt.GetURL()); ";
	#else
		return " this->AlifJavaScriptBridge(" + WVObjName + "->GetCurrentTitle()); ";
	#endif
}

string AlifJavaScript_Bridge = R"V0G0N( 
<script>
	// *** AlifJS Bridge ***

	var AlifJSBridg_Res = '';
	var AlifJSBridg_Status = false;

	function Alif(/**/){
		
		var args = arguments;
		var link = '';

		for(var i=0; i < args.length; i++){
			if(typeof args[i] === 'object'){
				alert('[جسر ألف]\n\n' + 
					'معطيات رقم ' + i + ' من نوع كائن');
				return '';
			}
			link = link + '~@' + args[i];
		}

		)V0G0N" + AlifJavaScript_Bridge_SendRequest() + R"V0G0N(
		
		if(!AlifJSBridg_Status && AlifJSBridg_Res != ''){

			alert('[جسر ألف]\n\n' + AlifJSBridg_Res);
			return '';

		} else {

			var Res = AlifJSBridg_Res;
			AlifJSBridg_Res = '';
			AlifJSBridg_Status = false;
			return Res;
		}
	}
</script>
</html>
)V0G0N";

string Generate_AlifJavaScript(){

	// TODO: So, instead of generate JS code of global Var/Func
	// for eache window..  we can create JS bool CheckGlobal()
	// this is for minimize generated JS code.

	// Generate all AlifJavaScript functions

	string ScriptFunctions = "";
	string WindowName = "";
	string FunctionName = "";
	string WebUI_obj_name = "";

	// --- Global Vars loop ----------------------------------------------------

	string ScriptBuffer_GlobalVarsCallCondition = "";

	for (int g = 0; g < G_VAR_TOTAL; g++){

		//FunctionName = "";
		string GlobalVarName = G_VAR_NAMES[g];
		string GlobalVarType = G_VAR_TYPE[GlobalVarName];

		string Return = "";
		string Return_end = " ; ";

		// Global Var return type
		if(GlobalVarType == "منطق"){
			Return = "FunctionReturn = alifcore_BoolToString( ";
			Return_end = " ); ";
		} else if(GlobalVarType == "نص"){
			Return = " FunctionReturn = '\\\'' + ";
			Return_end = " + '\\\'' ; ";
		} else if(GlobalVarType == "عدد"){
			Return = "FunctionReturn = alifcore_IntToString( ";
			Return_end = " ); ";
		} else {
			Return = "this->AlifJavaScript_Error(wxT(\"علة. نوع المتغير غير معروف\"));return; // ";
		}

		ScriptBuffer_GlobalVarsCallCondition.append(R"V0G0N(  
		else if (Tokens[0] == wxT(")V0G0N" + GlobalVarName + R"V0G0N(")){

			if(PramsCount != 0 ){this->AlifJavaScript_Error(wxT("لا يمكن تمرير أي معطيات لمتغير عام، المرجو إزالتها كلها، أو نداء على دالة تنتمي لنفس النافذة"));return;}

			)V0G0N" + Return + R"V0G0N( )V0G0N" + Global_ID[GlobalVarName] + Return_end + R"V0G0N(

		}  
		)V0G0N");
	}

	// --- Global Functions loop ----------------------------------------------------

	string ScriptBuffer_GlobalFuncCallCondition = "";
	
	for (int f = 0; f < Global_TotalFucntion; f++){

		FunctionName = Global_FunctionNames[f];

		string FunctionType = G_FUN_TYPE[FunctionName];
		string FunctionARGS = IntToString(G_FUN_ARG_TOTAL[FunctionName]);

		string Return = "";
		string Return_end = " ; ";

		// Function return type
		if(FunctionType == "عادم"){
			// No return.
		}
		else if(FunctionType == "منطق"){
			Return = "FunctionReturn = alifcore_BoolToString( ";
			Return_end = " ); ";
		} else if(FunctionType == "نص"){
			Return = " FunctionReturn = '\\\'' + ";
			Return_end = " + '\\\'' ; ";
		} else if(FunctionType == "عدد"){
			Return = "FunctionReturn = alifcore_IntToString( ";
			Return_end = " ); ";
		} else {
			Return = "this->AlifJavaScript_Error(wxT(\"علة. نوع الدالة غير معروف\"));return; // ";
		}

		string ARG_Call = "";
		int ARG_Total = G_FUN_ARG_TOTAL[FunctionName];

		// ARG loop
		for (int a = 0; a < ARG_Total; a++){ // arg loop

			string Type = G_FUN_ARG_TYPE[std::make_pair(FunctionName, a)];
			
			if(Type == "عدد"){
				ARG_Call.append(" alifcore_StringToInt(Tokens[" + IntToString(a + 1) + "]) ");
			} else if(Type == "نص"){
				ARG_Call.append(" Tokens[" + IntToString(a + 1) + "] ");
			} else if(Type == "منطق"){
				ARG_Call.append(" alifcore_StringToBool(Tokens[" + IntToString(a + 1) + "]) ");
			} else {
				Return = "this->AlifJavaScript_Error(wxT(\"علة. نوع معطيات غير معروف\"));return; // ";
			}

			if((a + 1) < ARG_Total)
				ARG_Call.append(" , ");
		}

		ScriptBuffer_GlobalFuncCallCondition.append(R"V0G0N(  
		else if (Tokens[0] == wxT(")V0G0N" + FunctionName + R"V0G0N(")){

			if(PramsCount != )V0G0N" + FunctionARGS + R"V0G0N( ){this->AlifJavaScript_Error(wxT("الدالة العامة تقبل فقط )V0G0N" + FunctionARGS + R"V0G0N( معطيات"));return;}

			)V0G0N" + Return + R"V0G0N( FUNCTION_)V0G0N" + Global_ID[FunctionName] + R"V0G0N( ( )V0G0N" + ARG_Call + R"V0G0N( ) )V0G0N" + Return_end + R"V0G0N(

		}  
		)V0G0N");
	}

	// --- All windows loop ----------------------------------------------------

	for (int i = 0; i <= WIN_TOTAL; i++){

		WindowName = WIN_TOTAL_NAMES[i]; //ID[WIN_TOTAL_NAMES[i]];
		//ALIF_ERROR(" {" + WindowName + "} ");

		if(!WIN_IS_WEB[WindowName]){ // Skip non-web type window
			//ALIF_ERROR("nonweb -> " + WindowName);
			//exit(EXIT_FAILURE);
			continue;
		}

		//ALIF_ERROR("continue -> " + WindowName);
		//exit(EXIT_FAILURE);
		
		string ScriptBuffer = "";
		string ScriptBuffer_FunctionCallCondition = "";

		FunctionName = "";
		WebUI_obj_name = "OBJ_CTR_" + ID[WindowName] + "_" + Control_ID["AlifUIWeb"];

		// OnNavigationRequest()
		ScriptBuffer.append(R"V0G0N(  
			void CLASS_WINDOW_)V0G0N" + ID[WindowName] + R"V0G0N(::AlifJavaScript_OnNavigationRequest(wxWebViewEvent& evt){
				evt.Veto();
				if(evt.GetURL().Mid(0,6) != wxT("memory"))
					wxLaunchDefaultBrowser(evt.GetURL());
			}
		)V0G0N");

		// OnNewWindow()
		ScriptBuffer.append(R"V0G0N(  
			void CLASS_WINDOW_)V0G0N" + ID[WindowName] + R"V0G0N(::AlifJavaScript_OnNewWindow(wxWebViewEvent& evt){
				)V0G0N" + AlifJavaScript_Bridge_OnNewWindowSendRequest(WebUI_obj_name) + R"V0G0N(
			}  
		)V0G0N");

		// AlifJavaScript_Error()
		ScriptBuffer.append(R"V0G0N(  
			void CLASS_WINDOW_)V0G0N" + ID[WindowName] + R"V0G0N(::AlifJavaScript_Error(wxString sError){
				)V0G0N" + WebUI_obj_name + R"V0G0N(->RunScript(wxT("function fJSTempBuffer(){AlifJSBridg_Res='" + sError +"';AlifJSBridg_Status=false;}fJSTempBuffer();"));
			}  
		)V0G0N");

		// AlifJavaScript_Run()
		ScriptBuffer.append(R"V0G0N(  
			void CLASS_WINDOW_)V0G0N" + ID[WindowName] + R"V0G0N(::AlifJavaScript_Run(wxString JSCode){
				)V0G0N" + WebUI_obj_name + R"V0G0N(->RunScript(wxT("function fAlifJSRun(){" + JSCode + "}fAlifJSRun();"));
			}  
		)V0G0N");

		// --- Local Functions loop ----------------------------------------------------

		for (int f = 0; f < Win_TotalFucntion[WindowName]; f++){

			FunctionName = Win_FunctionNames[std::make_pair(WindowName, f)];

			if (FunctionName == "رئيسية")
				continue;

			string FunctionType = L_FUN_TYPE[std::make_pair(WindowName, FunctionName)];
			string FunctionARGS = IntToString(L_FUN_ARG_TOTAL[std::make_pair(WindowName, FunctionName)]);

			//ALIF_ERROR("FunctionARGS -> " + FunctionARGS);
			//exit(EXIT_FAILURE);

			string Return = "";
			string Return_end = " ; ";

			// Function return type
			if(FunctionType == "عادم"){
				// No return.
			}
			else if(FunctionType == "منطق"){
				Return = "FunctionReturn = alifcore_BoolToString( ";
				Return_end = " ); ";
			} else if(FunctionType == "نص"){
				Return = " FunctionReturn = '\\\'' + ";
				Return_end = " + '\\\'' ; ";
			} else if(FunctionType == "عدد"){
				Return = "FunctionReturn = alifcore_IntToString( ";
				Return_end = " ); ";
			} else {
				Return = "this->AlifJavaScript_Error(wxT(\"علة. نوع الدالة غير معروف\"));return; // ";
			}

			string ARG_Call = "";
			int ARG_Total = L_FUN_ARG_TOTAL[std::make_pair(WindowName, FunctionName)];

			// ARG loop
			for (int a = 0; a < ARG_Total; a++){ // arg loop

				string Type = L_FUN_ARG_TYPE[std::make_pair(WindowName + FunctionName, a)];
				
				if(Type == "عدد"){
					ARG_Call.append(" alifcore_StringToInt(Tokens[" + IntToString(a + 1) + "]) ");
				} else if(Type == "نص"){
					ARG_Call.append(" Tokens[" + IntToString(a + 1) + "] ");
				} else if(Type == "منطق"){
					ARG_Call.append(" alifcore_StringToBool(Tokens[" + IntToString(a + 1) + "]) ");
				} else {
					Return = "this->AlifJavaScript_Error(wxT(\"علة. نوع معطيات غير معروف\"));return; // ";
				}

				if((a + 1) < ARG_Total)
					ARG_Call.append(" , ");
			}

			ScriptBuffer_FunctionCallCondition.append(R"V0G0N(  
			else if ((Tokens[0] == wxT(")V0G0N" + FunctionName + R"V0G0N(")) || (Tokens[0] == wxT(")V0G0N" + WindowName + R"V0G0N(:)V0G0N" + FunctionName + R"V0G0N("))){

				if(PramsCount != )V0G0N" + FunctionARGS + R"V0G0N( ){this->AlifJavaScript_Error(wxT("الدالة تقبل فقط )V0G0N" + FunctionARGS + R"V0G0N( معطيات"));return;}

				)V0G0N" + Return + R"V0G0N( FUNCTION_)V0G0N" + ID[FunctionName] + R"V0G0N( ( )V0G0N" + ARG_Call + R"V0G0N( ) )V0G0N" + Return_end + R"V0G0N(

			}  
			)V0G0N");
		}

		// AlifJavaScriptBridge()
		ScriptBuffer.append(R"V0G0N(  
			void CLASS_WINDOW_)V0G0N" + ID[WindowName] + R"V0G0N(::AlifJavaScriptBridge(wxString sLink){

				wxString Tokens[64];
				int TokensCount = 0;

				#ifdef _WIN32
					sLink.Replace(wxString("memory:~"), "");
				#elif  __APPLE__
					sLink.Replace(wxString("memory:~"), "");
				#else
					sLink.erase(0,1); // ~
					//sLink.Replace(wxString("memory:"), "");
					//sLink.Replace(wxString("http://AlifJSBridg/~"), "");
				#endif

				wxStringTokenizer Tokenizer(sLink, wxT("~"), wxTOKEN_RET_EMPTY);

				while (Tokenizer.HasMoreTokens()){

					wxString TokenBuffer = Tokenizer.GetNextToken();
					Tokens[TokensCount] = TokenBuffer.erase(0,1);
					// Skip JS objects
					if (Tokens[TokensCount].Find("[object ") > -1){
						this->AlifJavaScript_Error(wxT("معطيات رقم ")
						+ alifcore_IntToString(TokensCount) +
						wxT(" من نوع كائن"));
						return;
					}
					
					TokensCount++;
				}
				int PramsCount = TokensCount - 1;
				wxString FunctionReturn = "''";
				// Function call condition
				if (Tokens[0] == wxT("")){
					this->AlifJavaScript_Error(wxT("إسم الدالة أو متغير عام غير محدد"));
					return;
				}
				)V0G0N" + ScriptBuffer_FunctionCallCondition + ScriptBuffer_GlobalFuncCallCondition + ScriptBuffer_GlobalVarsCallCondition + R"V0G0N(
				else {
					this->AlifJavaScript_Error(wxT("دالة، دالة عامة، متغير عام. غير موجود"));
					return;
				}
				)V0G0N" + WebUI_obj_name + R"V0G0N(->RunScript(wxT("function fJSBuffer(){AlifJSBridg_Res=" + FunctionReturn +";AlifJSBridg_Status=true;}fJSBuffer();"));
			}  
		)V0G0N");

		ScriptFunctions.append(ScriptBuffer);
	}

	return ScriptFunctions;
}

// ----------------------------------
// C++ CODE INITIALIZATION
// ----------------------------------

void CG_INITIALIZATION()
{
	//رئيسية
	
	SET_C_NAME("رئيسية");
	SET_CONTROL_C_NAME("AlifUIWeb");

	// x = افصول
	// y = ارتوب
	CPP_WINDOW[std::make_pair("رئيسية", "نص")] 		= " (رئيسية) نافدة بدون عنوان";
	#ifdef __APPLE__
		CPP_WINDOW[std::make_pair("رئيسية", "افصول")] 	= "10";
		CPP_WINDOW[std::make_pair("رئيسية", "ارتوب")] 	= "25"; // Because the macOS task-bar is on top screen.
	#else
		CPP_WINDOW[std::make_pair("رئيسية", "افصول")] 	= "0";
		CPP_WINDOW[std::make_pair("رئيسية", "ارتوب")] 	= "0";
	#endif
	CPP_WINDOW[std::make_pair("رئيسية", "عرض")]		= "400";
	CPP_WINDOW[std::make_pair("رئيسية", "ارتفاع")] 	= "400";

	CPP_WINDOW[std::make_pair("رئيسية", "شكل")] = "wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN";
	// wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
	// wxMAXIMIZE_BOX

	WIN_TOTAL = 0;
	WIN_TOTAL_NAMES[0] = "رئيسية";
	L_FUN_TYPE[std::make_pair("رئيسية", "رئيسية")] = "عادم";
}

// ----------------------------------
// C++ CODE INITIALIZATION
// ----------------------------------

string CG_WINX_CODE()
{
	string CODE;
	string BUFFER;

	for (int i = 1; i <= WIN_TOTAL; i++) 
	{
		BUFFER = R"( 
// ========================================================================
// Window X )" + ID[WIN_TOTAL_NAMES[i]] + R"( 
// ========================================================================

// --------------------
// Window )" + ID[WIN_TOTAL_NAMES[i]] + R"( Definition
// --------------------

class CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"( : public wxFrame
{
    public:
		void OnClose(wxCloseEvent& event);
        CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"( ();
        virtual ~CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(();
		
		// --------------------
		// Window )" + ID[WIN_TOTAL_NAMES[i]] + R"( Functions Declaration
		// --------------------
		
		// +++++++++ >
			)" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "FUN_DECLARATION")] + R"(
		// +++++++++ >

    private:
		DECLARE_NO_COPY_CLASS(CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"()
        DECLARE_EVENT_TABLE()
};

CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"( *OBJ_CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(;
CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(::~CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"((){}

// --------------------
// Window )" + ID[WIN_TOTAL_NAMES[i]] + R"( Constructor
// --------------------

CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"( :: CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(() : 
	wxFrame(NULL, ID_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(, 
	wxT(")" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "نص")] + R"("),
	wxPoint()" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "افصول")] + R"(, )" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "ارتوب")] + R"(),
	wxSize()" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "عرض")] + R"(, )" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "ارتفاع")] + R"(), )" + 
	CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "شكل")] + R"( ) 
	// wxMAXIMIZE_BOX | wxFRAME_NO_TASKBAR | wxFRAME_FLOAT_ON_PARENT | wxRESIZE_BORDER
{
    wxPanel *P = new wxPanel(this, wxID_ANY);
	
	// --------------------
	// )" + ID[WIN_TOTAL_NAMES[i]] + R"( Controls Constructor
	// --------------------
	
	// +++++++++ >
		)" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "CTR_CONSTRUCTOR")] + R"(
	// +++++++++ >
	
	// ------------------------------------------------
}

// ------------------------------------------------
// Window Winx Close
// ------------------------------------------------

void CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(::OnClose(wxCloseEvent& event)
{
	event.Skip();

	WINDOW_IS_SHOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"( = false;
	WINDOW_IS_CONSTRUCTION_)" + ID[WIN_TOTAL_NAMES[i]] + R"( = false;
	
	// +++++++++ >
		)" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "CLOSE")] + R"(
	// +++++++++ >
}

// ------------------------------------------------
// Event Table
// ------------------------------------------------

BEGIN_EVENT_TABLE(CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(, wxFrame)
	EVT_CLOSE(CLASS_WINDOW_)" + ID[WIN_TOTAL_NAMES[i]] + R"(::OnClose)
	
	// +++++++++ >
		)" + CPP_WINDOW[std::make_pair(WIN_TOTAL_NAMES[i], "EVENT")] + R"(
	// +++++++++ >
	
END_EVENT_TABLE()

// ========================================================================
)";

		CODE.append(BUFFER);
	}

	return CODE;
}

// -----------------------------------------------------------
// C++ CODE
// -----------------------------------------------------------

// TODO: So, when tray GetValue() of control of destroyed Win
// the app not continue the fucntion... so, solution is be sure
// the windows is constructed before control:GetValue() [ MyControl:نص ]

string CODE_GENERATOR_GET_PC_GUI()
{
	return R"( 
// ------------------------------------------------
// Generated by Alif Compiler )" + VERSION + R"(
// www.aliflang.org
// ------------------------------------------------

#define UNICODE
#define _UNICODE
#define wxDEBUG_LEVEL 0

#if  __APPLE__
	// Fix : /include/wx/wxcrt.h:576:14: error: call to 'wcsstr' is ambiguous
	#define _WCHAR_H_CPLUSPLUS_98_CONFORMANCE_
#endif

#include <sstream> // stringstream (int To string)
#include <wx/wx.h>

#include "wx/webview.h"
#if wxUSE_WEBVIEW_IE
	#include "wx/msw/webview_ie.h"
#endif
#include "wx/webviewarchivehandler.h"
#include "wx/webviewfshandler.h"
#include "wx/infobar.h"
#include "wx/filesys.h"
#include "wx/fs_arc.h"
#include "wx/fs_mem.h"
#include <wx/tokenzr.h>

using namespace std;

class MyApp : public wxApp
{
    public:
        MyApp();
		~MyApp();
		virtual bool OnInit();
        virtual int OnExit();
	private:
		DECLARE_NO_COPY_CLASS(MyApp)
};
DECLARE_APP(MyApp);
IMPLEMENT_APP(MyApp);
MyApp::MyApp(){}
MyApp::~MyApp(){}
int MyApp::OnExit()
{exit(EXIT_FAILURE);}

int Generated_ID = 10000;
int ALIFCORE_ID_GENERATOR(){return Generated_ID++;}

string alifcore_IntToString(double INT_VAL)
{
	stringstream STRING_STREAM_BUFFER;
	STRING_STREAM_BUFFER << INT_VAL;
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}

double alifcore_StringToInt(wxString STR_VAL)
{
	double value;

	if(STR_VAL.ToDouble(&value))
	{
		return value;
	}
	else
	{
		return 0;
	}
}

bool alifcore_StringToBool(wxString Value){
	if (Value == "true" || 
		Value == "TRUE" || 
		Value == "1" || 
		Value == "نعم" || 
		Value == "صحيح" ||
		Value == "True")
		return true;
	else return false;
}

string alifcore_BoolToString(bool Value){
	if(Value) return "true";
	else return "false";
}

// --------------------
// Generated_ID GENERATOR
// Generated_ID for every Win and Ctrs
// --------------------

int ID_WINDOW_)" + ID["رئيسية"] + R"( = ALIFCORE_ID_GENERATOR(); // FIX

// +++++++++ >
	)" + CPP_ID_DECLARATION + R"(
// +++++++++ >

// --------------------
// GLOBAL Declaration
// --------------------

const static wxString ALIFCORE_NEW_LINE  =  wxT("\n"); // FIX
bool WINDOW_IS_SHOW_)" + ID["رئيسية"] + R"( = false; // FIX
bool WINDOW_IS_CONSTRUCTION_)" + ID["رئيسية"] + R"( = false; // FIX
void WINDOW_LOAD_)" + ID["رئيسية"] + R"((); // FIX

void AlifCore_AppExit();

// +++++++++ >
	)" + CPP_GLOBAL + R"(
// +++++++++ >

// --------------------
// OBJ Declaration 
// --------------------

// +++++++++ >
	)" + CPP_OBJ_DECLARATION + R"(
// +++++++++ >

// ========================================================================
// Window Main )" + ID["رئيسية"] + R"( Fixed Code
// ========================================================================

// ------------------------------------------------
// Window )" + ID["رئيسية"] + R"( Definition
// ------------------------------------------------

class CLASS_WINDOW_)" + ID["رئيسية"] + R"( : public wxFrame
{
    public:
		void OnClose(wxCloseEvent& event);
        CLASS_WINDOW_)" + ID["رئيسية"] + R"( ();
        virtual ~CLASS_WINDOW_)" + ID["رئيسية"] + R"(();
		
		// --------------------
		// Window )" + ID["رئيسية"] + R"( Functions Declaration
		// --------------------
		
		// +++++++++ >
			)" + CPP_WINDOW[std::make_pair("رئيسية", "FUN_DECLARATION")] + R"(
		// +++++++++ >

    private:
		DECLARE_NO_COPY_CLASS(CLASS_WINDOW_)" + ID["رئيسية"] + R"()
        DECLARE_EVENT_TABLE()
};

CLASS_WINDOW_)" + ID["رئيسية"] + R"( *OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(;
CLASS_WINDOW_)" + ID["رئيسية"] + R"(::~CLASS_WINDOW_)" + ID["رئيسية"] + R"((){}

// --------------------
// Window )" + ID["رئيسية"] + R"( Constructor
// --------------------

CLASS_WINDOW_)" + ID["رئيسية"] + R"( :: CLASS_WINDOW_)" + ID["رئيسية"] + R"(() : 
	wxFrame(NULL, ID_WINDOW_)" + ID["رئيسية"] + R"(, 
	wxT(")" + CPP_WINDOW[std::make_pair("رئيسية", "نص")] + R"("),
	wxPoint()" + CPP_WINDOW[std::make_pair("رئيسية", "افصول")] + R"(, )" + CPP_WINDOW[std::make_pair("رئيسية", "ارتوب")] + R"(),
	wxSize()" + CPP_WINDOW[std::make_pair("رئيسية", "عرض")] + R"(, )" + CPP_WINDOW[std::make_pair("رئيسية", "ارتفاع")] + R"(), )" + 
	CPP_WINDOW[std::make_pair("رئيسية", "شكل")] + R"( )
{
    wxPanel *P = new wxPanel(this, wxID_ANY);
	
	// --------------------
	// Window )" + ID["رئيسية"] + R"( Controls Constructor
	// --------------------
	
	// +++++++++ >
		)" + CPP_WINDOW[std::make_pair("رئيسية", "CTR_CONSTRUCTOR")] + R"(
	// +++++++++ >
	
	// ------------------------------------------------
}

// --------------------
// Application Class Initialization
// --------------------

bool MyApp::OnInit()
{
	if (!wxApp::OnInit())
		return false;
	
	//WebUI FS
    //wxFileSystem::AddHandler(new wxArchiveFSHandler); // Accessing files inside Zip Archive
    wxFileSystem::AddHandler(new wxMemoryFSHandler);
	
	// --- Bin2C Data ------------------------------

	// need extra work..
	//wxImage::AddHandler(new wxPNGHandler);
	//#include "D:\\Code\\Alif_App\\Alif_2\\test.jpg.cpp"
	//wxMemoryFSHandler::AddFile("test.jpg", test_jpg, wxBITMAP_TYPE_JPEG);
	//#include "D:\\Code\\Alif_App\\Alif_2\\test2.png.cpp"
	//wxMemoryFSHandler::AddFile("test2.png", wxBitmap(test2_png), wxBITMAP_TYPE_PNG);

	wxInitAllImageHandlers();

	// ---------------------------------------------

	OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"( = new CLASS_WINDOW_)" + ID["رئيسية"] + R"(();
	OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(->SetLayoutDirection(wxLayout_RightToLeft);
    OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(->Show();
	SetTopWindow(OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"();
	OBJ_CLASS_WINDOW_)" + ID["رئيسية"] + R"(->Refresh();
	WINDOW_IS_SHOW_)" + ID["رئيسية"] + R"( = true;
	WINDOW_IS_CONSTRUCTION_)" + ID["رئيسية"] + R"( = true;

	WINDOW_LOAD_)" + ID["رئيسية"] + R"(();

    return true;
}

// ------------------------------------------------
// Event Table
// ------------------------------------------------

BEGIN_EVENT_TABLE(CLASS_WINDOW_)" + ID["رئيسية"] + R"(, wxFrame)
	EVT_CLOSE(CLASS_WINDOW_)" + ID["رئيسية"] + R"(::OnClose)
	
	// +++++++++ >
		)" + CPP_WINDOW[std::make_pair("رئيسية", "EVENT")] + R"(
	// +++++++++ >
	
END_EVENT_TABLE()

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Class's
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
)" + CPP_CLASS + R"(
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// WinX Loop Code
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
)" + CG_WINX_CODE() + R"(
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ========================================================================
// Function / Controls Functions 
// ========================================================================

// +++++++++ >
	)" + CPP_FUN + R"(
// +++++++++ >

// ========================================================================
// Global Functions 
// ========================================================================

// +++++++++ >
	)" + CPP_GLOBAL_FUN + R"(
// +++++++++ >

// ========================================================================
// Window Main )" + ID["رئيسية"] + R"( Close
// ========================================================================

void CLASS_WINDOW_)" + ID["رئيسية"] + R"(::OnClose(wxCloseEvent& event)
{
	event.Skip();

	WINDOW_IS_SHOW_)" + ID["رئيسية"] + R"( = false;
	WINDOW_IS_CONSTRUCTION_)" + ID["رئيسية"] + R"( = false;
	
	// +++++++++ >
	)" + CPP_WINDOW[std::make_pair(ID["رئيسية"], "CLOSE")] + R"(
	// +++++++++ >
}

// ========================================================================
// Window Main Load
// ========================================================================

void WINDOW_LOAD_)" + ID["رئيسية"] + R"(()
{
	)" + CPP_WINDOW[std::make_pair("رئيسية", "LOAD")] + R"(
}

// ========================================================================
// Windows X Loads
// ========================================================================

)" + CG_WINX_LOAD() + R"(

// ========================================================================
// AlifJavaScript
// ========================================================================

)" + Generate_AlifJavaScript() + R"(

// ========================================================================

void AlifCore_AppExit(){

	)" + CG_APP_DESTROY() + R"(
}

// ------------------------------------------------
// Generated by Alif Compiler )" + VERSION + R"(
// www.aliflang.org
// ------------------------------------------------
 )";

}

// ========================================================================================
