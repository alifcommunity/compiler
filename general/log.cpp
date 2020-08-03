// ==================================
// (C)2019 DRAGA Hassan.
// (C)2019 Alif Community.
// 	 www.aliflang.org
// ----------------------------------
// Licence : GPLv3.
// ----------------------------------
// Alif Programming Language
// ==================================

void ALIF_ERROR(string ERROR_DESCRIPTION) 
{
	cout << endl << ERROR_DESCRIPTION << endl << endl;
	
	ofstream file;
	
	file.open (PATH_FULL_LOG, ios::app);
	
		file << ERROR_DESCRIPTION;
		
	file.close();
	
	exit(EXIT_FAILURE);
}

void DEBUG_MESSAGE(string MESSAGE, CLASS_TOKEN *o_tokens)
{
	LOG_MESSAGE_FULL.append(MESSAGE);
	
	if (DEBUG_PRINT_ON_SCREEN)
	{
		cout << MESSAGE;
	}
}

void LogMessage(string MESSAGE)
{
	LOG_MESSAGE_FULL.append(MESSAGE);
	
	if (DEBUG_PRINT_ON_SCREEN)
	{
		cout << MESSAGE;
	}
}

void PATH_FULL_LOG_SAVE()
{
	ofstream file;

	file.open (PATH_FULL_LOG);
		file << LOG_MESSAGE_FULL;
	file.close();
}

void ErrorCode(string ERROR_DESCRIPTION, CLASS_TOKEN *o_tokens) 
{

	if(DEBUG)DEBUG_MESSAGE("\n ----------- DEBUGING ERROR ------------- \n", o_tokens);
	
	if(DEBUG)DEBUG_MESSAGE("Class : " + TheClass + " \n", o_tokens);
	if(DEBUG)DEBUG_MESSAGE("Win : " + TheWindow + " \n", o_tokens);
	if(DEBUG)DEBUG_MESSAGE("Fun : " + TheFunction + " \n\n", o_tokens);
	
	string ERR_MSG;
	
	if (SHOW_FILE_AND_LINE)
	{
		ERR_MSG = " الملف		: " + o_tokens->PATH_FULL_SOURCE + "\n";
		
		if (o_tokens->Line > 0)
			ERR_MSG.append(" السطر		: " + (boost::lexical_cast<std::string>(o_tokens->Line)) + "\n");
	}
	
	ERR_MSG.append("\n الخطأ		: " + ERROR_DESCRIPTION);
	
	if (ERROR_PRINT_ON_SCREEN)
		cout << "---| DEBUG_MESSAGE |--------------" << endl << ERR_MSG << endl << "------------------------" << endl;
	
	LOG_MESSAGE_FULL.append(ERR_MSG);
	
	PATH_FULL_LOG_SAVE();
	
	// Exit code must be 'success', to let IDE read from DEBUG_MESSAGE file
	// if exit_failur, so IDE read data from process output
	exit(EXIT_SUCCESS);
}
