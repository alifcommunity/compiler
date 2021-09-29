/*
        The Alif Programming Language
        Version 3.x Series
        (C)2021 Hassan DRAGA
        www.aliflang.org

        This file is part of Alif compiler.

        Alif compiler is free software; you can redistribute it and/or modify it
   under the terms of the GNU General Public License as published by the Free
        Software Foundation; either version 3, or (at your option) any later
        version.

        Alif compiler is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
        FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
        for more details.

        You should have received a copy of the GNU General Public License
        along with Alif compiler; see the file COPYING3. If not see
        <http://www.gnu.org/licenses/>.
*/

void parser_IfElse(std::string Token[2048], CLASS_TOKEN *o_tokens) {
  // وإلا

  if (!o_tokens->TOKENS_PREDEFINED)
    return; // continue;

  if (!IsInsideFunction)
    ErrorCode("يجب استعمال الشرط داخل دالة", o_tokens);

  if (ALIF_IF_STATUS < 1)
    ErrorCode("لايمكن إستعمال ' وإلا ' من دون فتح شرط، ربمى تقصد ' إذا ' ",
              o_tokens);

  if (Token[2] == "إذا")
    ErrorCode("شرط غير صحيح، هل تقصد ' أو إذا ' ؟ ", o_tokens);

  if (Token[2] != "")
    ErrorCode(
        "أمر غير معروف : ' " + Token[2] +
            " '، على العموم إذا كنت تقصد شرط جديد المرجو إستعمال ' إذا ' ",
        o_tokens);

  if (DEBUG)
    DEBUG_MESSAGE("[ELSE " + IntToString(ALIF_IF_STATUS) + "] \n\n",
                  o_tokens); // DEBUG

  // TODO: show error when double Else
  // if ...
  // else ...
  // else ...
  // end if

  /*
  // *** Generate Code ***
  if (!IsInsideNamespace)
  {
          // Global Fun IF
          CPP_GLOBAL_FUN.append("\n } else { \n ");
  }
  else
  {
          // Local Fun IF
          cpp_AddScript(TheFunction, "\n } else { \n ");
  }
  // *** *** *** *** *** ***
  */

  if (IsInsideClass) {
    // just for fixing this ...
    // *** Generate Code ***
    CPP_CLASS.append("\n } else { ");
    // *** *** *** *** *** ***
  } else if (!IsInsideNamespace) {
    // Global Fun IF
    CPP_GLOBAL_FUN.append("\n } else { ");
  } else {
    // Local Fun IF
    cpp_AddScript(TheFunction, "\n } else { ");
  }
  // *** *** *** *** *** ***

  return; // continue;
}
