/*
 This is the core used in every app
 compiled by Alif 3.x Series, which
 conain all functions and tools needed
 by the app in the runtimes.
 File: [Alif folder]/alifcore/alifcore.cc
 Version: Alif Core 1.0.2
*/

#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE
#endif

#define _HAS_STD_BYTE 0 // Fixing 'byte': ambiguous symbol

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>
#elif __APPLE__
// ...
#else
// ...
#endif

// Standard
#include <algorithm> // Standard replace()
#include <chrono>    // Wait..
#include <codecvt>   // conversion from wide 'wchar_t' to 'string' bytes
#include <cstdio>    // Remove files
#include <ctime>     // Initializ time, for rand()
#include <ctype.h>   //
#include <fstream>   // O/I files
#include <iomanip>   // std::setprecision
#include <iostream>  // O/I
#include <iterator>  //
#include <locale>    //
#include <map>       // vectors
#include <sstream>   // stringstream
#include <stdlib.h>  // mblen, mbtowc, wchar_t(C)
#include <streambuf> //
#include <string>    // strings
#include <thread>    // Treading
#include <vector>    // UTF8, Vectors

// Boost
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/locale.hpp>
#include <boost/nowide/args.hpp>
#include <boost/nowide/fstream.hpp>
#include <boost/nowide/iostream.hpp>
#include <boost/program_options.hpp>
#include <boost/thread/thread.hpp>

// UTF-8
#include "utf8.h"

using namespace std;

// -- Alif v2 -------------------------------------------------------------

const static std::string ALIFCORE_NEW_LINE = "\n";

// IDs
int Generated_ID = 10000;
int ALIFCORE_ID_GENERATOR() { return Generated_ID++; }

// int to string
std::string alifcore_IntToString(double INT_VAL) {

  std::stringstream STRING_STREAM_BUFFER;
  STRING_STREAM_BUFFER << std::setprecision(40) << INT_VAL;
  std::string STRING_BUFFER = STRING_STREAM_BUFFER.str();
  return STRING_BUFFER;
}

double alifcore_StringToInt(std::string s) {

  std::string::size_type sz;
  return std::stod(s, &sz);
}

bool alifcore_StringToBool(std::string Value) {

  if (Value == "true" || Value == "TRUE" || Value == "1" || Value == "نعم" ||
      Value == "صحيح" || Value == "True")
    return true;

  else
    return false;
}

std::string alifcore_BoolToString(bool Value) {

  if (Value)
    return "true";
  else
    return "false";
}

// -- Alif v3 -------------------------------------------------------------

struct _alifcore_inf {

#ifdef _WIN32
  std::string exe_ext = ".exe";
  std::string path_sep = "\\";
#elif __APPLE__
  std::string exe_ext = ".app";
  std::string path_sep = "/";
#else
  std::string exe_ext = "";
  std::string path_sep = "/";
#endif

} alifcore_inf;

unsigned char *alifcore_malloc(size_t size) {

  // return NULL;
  if (size > 0)
    return (unsigned char *)malloc(size);
  else
    return NULL;
}

void *alifcore_memset(void *s, int c, size_t n) {

  // return s;
  if (s != NULL)
    return memset(s, c, n);
  else
    return NULL;
}

void alifcore_free(void *ptr) {

  if (ptr != NULL)
    free(ptr);
}

std::string &alifcore_ltrim(std::string &s) {

  auto it = std::find_if(s.begin(), s.end(), [](char c) {
    return !std::isspace<char>(c, std::locale::classic());
  });

  s.erase(s.begin(), it);

  return s;
}

std::string &alifcore_rtrim(std::string &s) {

  auto it = std::find_if(s.rbegin(), s.rend(), [](char c) {
    return !std::isspace<char>(c, std::locale::classic());
  });

  s.erase(it.base(), s.end());

  return s;
}

std::string &alifcore_trim(std::string &s) {

  return alifcore_ltrim(alifcore_rtrim(s));
}

void alifcore_initialization() {

#ifdef _WIN32
  // Create and install global locale
  std::locale::global(boost::locale::generator().generate(""));
#endif

  // Make boost.filesystem use it
  boost::filesystem::path::imbue(std::locale());
}

std::string alifcore_wchar_to_str(wchar_t wstr) {

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

std::string alifcore_wchar_to_str(wchar_t wstr[]) {

  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  return converter.to_bytes(wstr);
}

