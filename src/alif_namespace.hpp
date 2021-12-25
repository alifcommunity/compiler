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

void parser_new_namespace(std::string Token[2048], CLASS_TOKEN *o_tokens)
{

  // namespace abc

  if (IsInsideNamespace)
    ErrorCode("لا يمكن انشاء مجال داخل مجال، المجال الحالية : " + TheNamespace,
              o_tokens);

  if (IsInsideClass)
    ErrorCode("لا يمكن انشاء مجال داخل صنف : " + TheClass, o_tokens);

  if (IsInsideFunction)
    ErrorCode("لا يمكن انشاء مجال داخل دالة : " + TheFunction, o_tokens);

  if (Token[2] == "")
    ErrorCode("يجب تحديد اسم المجال", o_tokens);

  if (Token[3] != "")
    ErrorCode("أمر غير معروف : ' " + Token[3] + " ' ", o_tokens);

  // temporary set because this namespace
  // did not get created yet, but we need
  // his Generated ID now.
  // TheNamespace = Token[2];

  if (!IsValidName(Token[2], o_tokens))
    ErrorCode("اسم غير مقبول : ' " + Token[2] + " ' ", o_tokens);

  CheckForSameGlobalID(Token[2], o_tokens);

  if (!o_tokens->TOKENS_PREDEFINED)
  {

    if (namespace_is_set[Token[2]])
      ErrorCode("المجال ' " + Token[2] + " ' تم انشاؤها مسبقا في السطر : " +
                    namespace_at_line[Token[2]],
                o_tokens);

    namespace_is_set[Token[2]] = true;
    namespace_at_line[Token[2]] = IntToString(o_tokens->Line);
    SET_GLOBAL_C_NAME(
        Token[2]); // TODO: is okay (SET_C_NAME -> SET_GLOBAL_C_NAME) ?

    IsInsideNamespace = true; // Need by Tokens Predefined
    TheNamespace = Token[2];  // Need by Tokens Predefined

    // temporary end
    // TheNamespace = "";

    return; // continue;
  }

  IsInsideNamespace = true;
  TheNamespace = Token[2];

  if (DEBUG)
    DEBUG_MESSAGE("[NAMESPACE] [" + Token[2] + "] \n\n", o_tokens); // DEBUG

  // CPP_GLOBAL.append(" bool WINDOW_IS_SHOW_" + ID[Token[2]] + " = false; \n");
  CPP_GLOBAL_FUN.append(" namespace NS_" + ID[Token[2]] + " { \n");
}
