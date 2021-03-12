/*
 This is the core used in every app
 compiled by Alif 3.x Series, which 
 conain all functions and tools needed 
 by the app in the runtimes.

 Version: Alif Runtimes Core 1.0.1
*/

#ifndef UNICODE
    #define UNICODE
#endif

#ifndef _UNICODE
    #define _UNICODE
#endif

#ifdef _WIN32
    #include <windows.h>
#endif

#include <iostream>
#include <sstream>
#include <string>
using namespace std;

const static string ALIFCORE_NEW_LINE = "\n";

// IDs
int Generated_ID = 10000;
int ALIFCORE_ID_GENERATOR() {

    return Generated_ID++;
}

// int to string
string alifcore_IntToString(double INT_VAL) {

    stringstream STRING_STREAM_BUFFER;
	STRING_STREAM_BUFFER << INT_VAL;
	string STRING_BUFFER = STRING_STREAM_BUFFER.str();
	return STRING_BUFFER;
}

double alifcore_StringToInt(string s) {

    std::string::size_type sz;
    return std::stod (s, &sz);
}

bool alifcore_StringToBool(string Value) {

	if (Value == "true" || 
		Value == "TRUE" || 
		Value == "1" || 
		Value == "نعم" || 
		Value == "صحيح" ||
		Value == "True")
		return true;

	else return false;
}

string alifcore_BoolToString(bool Value) {

	if(Value) return "true";
	else return "false";
}

// Bind functions
/*
std::array<void(*)(), 256> key_actions;
unsigned int key_actions_c = 0;

void alifcore_bind_func_shutdown(void(*function_ref)()) {
	
	key_actions[key_actions_c] = function_ref;
	key_actions_c++;
}

void alifcore_call_all_shutdown() {
	
	if(key_actions_c < 1)
		return;
	
	for (int i = 0; i < key_actions_c; i++)
		key_actions[i];
}
*/