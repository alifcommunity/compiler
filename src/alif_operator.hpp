/*
  The Alif Programming Language
  Version 4.x Series
  (C)2022 Hassan DRAGA
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

void parser_TwoPointOperator(std::string Token[2048], CLASS_TOKEN *o_tokens)
{

  // ---------------------------------------------------------------------------------
  // : (Operator between members)
  // ---------------------------------------------------------------------------------

  // namespace:local_func(a, b)	| namespace:my_func(a, b)
  // namespace:local_var			| namespace:my_var

  // namespace_name	:		local_func		( 	... )
  // 1              2		3				      4 	5		6

  if (!o_tokens->TOKENS_PREDEFINED)
    return; // continue;

  if (!IsInsideFunction)
    ErrorCode("يجب استعمال المجال داخل دالة", o_tokens);

  if (namespace_is_set[Token[1]])
  {
    // namespace:local_func(a, b)		| win:my_func(a, b)

    if (Token[2] != ":")
      ErrorCode("يجب اضافه ' : ' بعد ' " + Token[1] + " ' ", o_tokens);

    if (Token[3] == "")
      ErrorCode("يجب اضافه عضو تابع ل ' " + Token[1] + " ' بعد ':' ", o_tokens);

    if (Token[4] == "")
      ErrorCode("يجب اضافه ':' أو '()' أو '=' بعد ' " + Token[1] + " " +
                    Token[2] + " " + Token[3] + " ' ",
                o_tokens);

    if (Token[4] != "(" && Token[4] != ":" && Token[4] != "=")
      ErrorCode("أمر غير معروف ' " + Token[4] +
                    " ', يجب اضافه ':' أو '()' أو '=' ",
                o_tokens);

    if (L_FUN_IS_SET[std::make_pair(Token[1], Token[3])] && Token[4] != "(")
      ErrorCode("من اجل نداء الدالة ' " + Token[3] +
                    " ' يجب اضافه '()' بعد ' " + Token[1] + " " + Token[2] +
                    " " + Token[3] + " ' ",
                o_tokens);

    if (Token[4] == "(")
    {
      // -----------------------
      // namespace:win-local_func()
      // -----------------------

      if (L_FUN_IS_SET[std::make_pair(Token[1], Token[3])])
      {
        if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
          ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);

        if (DEBUG)
          DEBUG_MESSAGE("[NS'" + Token[1] + " ':LOCAL_FUNC'" + Token[3] +
                            " ' ( ",
                        o_tokens); // DEBUG

        // // *** Generate Code ***
        // if (!IsInsideNamespace)
        // 	// Call other namespace local_func from global func
        // 	CPP_GLOBAL_FUN.append(CG_WIN_MEMBER(Token[1], ID[Token[3]] + "
        // (")); else
        // 	// Call other namespace local_func from local func
        // 	cpp_AddScript(TheFunction, CG_WIN_MEMBER(Token[1], ID[Token[3]]
        // + " ("));
        // // *** *** *** *** *** ***

        TempTokenCount = 0;
        for (int p = 5; p <= o_tokens->TOTAL[o_tokens->Line];
             p++) // | a, b + 2, c) |
        {
          if (Token[p] != "")
          {
            TempToken[TempTokenCount] = Token[p];
            TempTokenCount++;
          }
        }

        // Check local fun Args and Syntax : fun (a + c, 2 * (b - 1))
        ScriptSyntaxBuffer = CHECK_CALL_FUN_ARG(
            false, Token[1], Token[3], 0, TheNamespace, TheFunction, TempToken,
            (TempTokenCount - 1), o_tokens);

        if (DEBUG)
          DEBUG_MESSAGE(")] \n\n", o_tokens); // DEBUG

        // // *** Generate Code ***
        // if (!IsInsideNamespace)
        // 	// Call other win-local_func from global func
        // 	CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ); \n } \n");
        // else
        // 	// Call other win-local_func from local func
        // 	cpp_AddScript(TheFunction, ScriptSyntaxBuffer + " ); \n } \n");
        // // *** *** *** *** *** ***

        // *** Generate Code ***
        ScriptBuffer = " NS_" + ID[Token[1]] + "::FUNCTION_" + ID[Token[3]] +
                       "(" + ScriptSyntaxBuffer + "); \n";

        if (!IsInsideNamespace)
          // Call other win-local_func from global func
          CPP_GLOBAL_FUN.append(ScriptBuffer + " \n");
        else
          // Call other win-local_func from local func
          cpp_AddScript(TheFunction, ScriptBuffer + " \n");
        // *** *** *** *** *** ***

        return; // continue;
      }
      // ---------------------------------------------------------------------------------
      else
        ErrorCode("المجال ' " + Token[1] +
                      " ' لا تحتوي على دالة محليه بإسم ' " + Token[3] + " ' ",
                  o_tokens);

      // Exception!
      return; // continue;
    }
    else
    {
      // Exception !
      ErrorCode("يجب اضافه ':' أو '()' أو '=' بعد ' " + Token[1] + " " +
                    Token[2] + " " + Token[3] + " ' ",
                o_tokens);
    }
  }
  else
  {
    // Exception !
    ErrorCode("أمر غير معروف ' " + Token[1] + " ', يجب أن تكون فقط مجال",
              o_tokens);
  }
}
