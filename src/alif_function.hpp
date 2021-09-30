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

void parser_Function(std::string Token[2048], CLASS_TOKEN *o_tokens) {

  //دالة

  int FUN_POS = 1;

  if (IsInsideFunction)
    ErrorCode("لا يمكن انشاء دالة داخل دالة، الدالة الحالية : " + TheFunction,
              o_tokens);

  if (Token[FUN_POS + 1] == "")
    ErrorCode("يجب اعطاء اسم لهته الدالة الجديدة", o_tokens);

  if (Token[FUN_POS + 1] == "رئيسية") // دالة رئيسية
  {
    if (IsInsideNamespace)
      ErrorCode("لا يمكن انشاء دالة رئيسية داخل مجال، بل يجب انشائها في المجال "
                "العام أو داخل صنف",
                o_tokens);

    if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
      ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);

    if (Token[FUN_POS + 2] == "(") {
      if (Token[FUN_POS + 3] == "")
        ErrorCode("يجب اغلاق القوس ')' ", o_tokens);

      if (Token[FUN_POS + 3] != ")")
        ErrorCode("الدالة الرئيسية لا تقبل الخصائص", o_tokens);

      if (Token[FUN_POS + 4] != "")
        ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 4] + " ' ", o_tokens);
    }

    if (!o_tokens->TOKENS_PREDEFINED) {
      if (MAIN_FUN_IS_SET[TheNamespace])
        ErrorCode("الدالة الرئيسية تم انشاؤها مسبقا في السطر : " +
                      MAIN_FUN_AT_LINE[TheNamespace],
                  o_tokens);

      MAIN_FUN_IS_SET[TheNamespace] = true;
      MAIN_FUN_AT_LINE[TheNamespace] = IntToString(o_tokens->Line);

      IsInsideFunction = true; // Need by Tokens Predefined
      TheFunction = "رئيسية";  // Need by Tokens Predefined

      script.main.is_set = true;
      script.main.set_at_line = IntToString(o_tokens->Line);

      return; // continue;
    }

    IsInsideFunction = true;
    TheFunction = "رئيسية";
    TheFunction_TYPE = "عادم";
    RETURN_FUN[std::make_pair(TheNamespace, "رئيسية")] = "OK";

    IsInsideNamespace = true; // Alif 3

    Win_FunctionNames[std::make_pair(TheNamespace, Win_CurrentTotalFucntion)] =
        TheFunction;
    Win_CurrentTotalFucntion++;
    Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

    if (DEBUG)
      DEBUG_MESSAGE("[FUNCTION] [MAIN] \n\n", o_tokens); // DEBUG

    return; // continue;
  } else if (Token[FUN_POS + 1] == "عدد" || Token[FUN_POS + 1] == "نص" ||
             Token[FUN_POS + 1] == "منطق") // دالة TYPE MyFunctionName (...)
  {
    // --------------------------
    // Type Function
    // --------------------------

    if (Token[FUN_POS + 2] == "")
      ErrorCode("يجب تحديد اسم الداله", o_tokens);

    if (Token[FUN_POS + 3] != "" && Token[FUN_POS + 3] != "(")
      ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 3] + " ' ", o_tokens);

    if (Token[FUN_POS + 3] == "") // دالة TYPE MyFunctionName
    {
      if (Token[FUN_POS + 1] == "عدد") {
        if (!IsInsideNamespace) {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Global function
            ADD_FUN(true, "", Token[FUN_POS + 2], "عدد", o_tokens->Line,
                    o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_INT] [" +
                              Token[FUN_POS + 2] + "] \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Global INT Func(void)
          CPP_GLOBAL.append(" double FUNCTION_" +
                            Global_ID[Token[FUN_POS + 2]] + "(); \n");
          CPP_GLOBAL_FUN.append(" double FUNCTION_" +
                                Global_ID[Token[FUN_POS + 2]] + "(){ \n");
          // *** *** *** *** *** ***
        } else {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Local function
            ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "عدد",
                    o_tokens->Line, o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] +
                              "] \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Local INT Func(void)
          CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
          CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
              CBUFER + " double FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
          CPP_GLOBAL_FUN.append(" double FUNCTION_" + ID[Token[FUN_POS + 2]] +
                                "(){ \n");
          // *** *** *** *** *** ***
        }

        IsInsideFunction = true;
        TheFunction = Token[FUN_POS + 2];
        TheFunction_TYPE = "عدد";

        Win_FunctionNames[std::make_pair(
            TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
        Win_CurrentTotalFucntion++;
        Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

        return; // continue;
      } else if (Token[FUN_POS + 1] == "نص") {
        if (!IsInsideNamespace) {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Global function
            ADD_FUN(true, "", Token[FUN_POS + 2], "نص", o_tokens->Line,
                    o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_STRING] [" +
                              Token[FUN_POS + 2] + "] \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Global String Func(void)
          CPP_GLOBAL.append(" std::string FUNCTION_" +
                            Global_ID[Token[FUN_POS + 2]] + "(); \n");
          CPP_GLOBAL_FUN.append(" std::string FUNCTION_" +
                                Global_ID[Token[FUN_POS + 2]] + "(){ \n");
          // *** *** *** *** *** ***
        } else {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Local function
            ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "نص",
                    o_tokens->Line, o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_STRING] [" +
                              Token[FUN_POS + 2] + "] \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Local String Func(void)
          CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
          CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
              CBUFER + " std::string FUNCTION_" + ID[Token[FUN_POS + 2]] +
              "(); \n";
          CPP_GLOBAL_FUN.append(" std::string FUNCTION_" +
                                ID[Token[FUN_POS + 2]] + "(){ \n");
          // *** *** *** *** *** ***
        }

        IsInsideFunction = true;
        TheFunction = Token[FUN_POS + 2];
        TheFunction_TYPE = "نص";

        Win_FunctionNames[std::make_pair(
            TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
        Win_CurrentTotalFucntion++;
        Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

        return; // continue;
      } else if (Token[FUN_POS + 1] == "منطق") {
        if (!IsInsideNamespace) {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Global function
            ADD_FUN(true, "", Token[FUN_POS + 2], "منطق", o_tokens->Line,
                    o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_BOOL] [" +
                              Token[FUN_POS + 2] + "] \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Global BOOL Func(void)
          CPP_GLOBAL.append(" bool FUNCTION_" + Global_ID[Token[FUN_POS + 2]] +
                            "(); \n");
          CPP_GLOBAL_FUN.append(" bool FUNCTION_" +
                                Global_ID[Token[FUN_POS + 2]] + "(){ \n");
          // *** *** *** *** *** ***
        } else {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Local function
            ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "منطق",
                    o_tokens->Line, o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_BOOL] [" + Token[FUN_POS + 2] +
                              "] \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Local BOOL Func(void)
          CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
          CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
              CBUFER + " bool FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
          CPP_GLOBAL_FUN.append(" bool FUNCTION_" + ID[Token[FUN_POS + 2]] +
                                "(){ \n");
          // *** *** *** *** *** ***
        }

        IsInsideFunction = true;
        TheFunction = Token[FUN_POS + 2];
        TheFunction_TYPE = "منطق";

        Win_FunctionNames[std::make_pair(
            TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
        Win_CurrentTotalFucntion++;
        Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

        return; // continue;
      }
    } else if (Token[FUN_POS + 3] == "(") // دالة TYPE MyFunctionName ( ???
    {
      if (Token[FUN_POS + 4] == "")
        ErrorCode("سطر غير كامل المرجو اضافة ')' ", o_tokens);

      if (Token[FUN_POS + 4] == ")") // دالة TYPE MyFunctionName ()
      {
        if (Token[FUN_POS + 5] != "")
          ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 5] + " ' ",
                    o_tokens);

        if (Token[FUN_POS + 1] == "عدد") {
          if (!IsInsideNamespace) {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Global function
              ADD_FUN(true, "", Token[FUN_POS + 2], "عدد", o_tokens->Line,
                      o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_INT] [" +
                                Token[FUN_POS + 2] + "] () \n\n",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Global INT Func(void)
            CPP_GLOBAL.append(" double FUNCTION_" +
                              Global_ID[Token[FUN_POS + 2]] + "(); \n");
            CPP_GLOBAL_FUN.append(" double FUNCTION_" +
                                  Global_ID[Token[FUN_POS + 2]] + "(){ \n");
            // *** *** *** *** *** ***
          } else {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Local function
              ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "عدد",
                      o_tokens->Line, o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] +
                                "] () \n\n",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Local INT Func(void)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + " double FUNCTION_" + ID[Token[FUN_POS + 2]] +
                "(); \n";
            CPP_GLOBAL_FUN.append(" double FUNCTION_" + ID[Token[FUN_POS + 2]] +
                                  "(){ \n");
            // *** *** *** *** *** ***
          }

          IsInsideFunction = true;
          TheFunction = Token[FUN_POS + 2];
          TheFunction_TYPE = "عدد";

          Win_FunctionNames[std::make_pair(
              TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
          Win_CurrentTotalFucntion++;
          Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

          return; // continue;
        } else if (Token[FUN_POS + 1] == "نص") {
          if (!IsInsideNamespace) {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Global function
              ADD_FUN(true, "", Token[FUN_POS + 2], "نص", o_tokens->Line,
                      o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_STRING] [" +
                                Token[FUN_POS + 2] + "] () \n\n",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Global String Func(void)
            CPP_GLOBAL.append(" std::string FUNCTION_" +
                              Global_ID[Token[FUN_POS + 2]] + "(); \n");
            CPP_GLOBAL_FUN.append(" std::string FUNCTION_" +
                                  Global_ID[Token[FUN_POS + 2]] + "(){ \n");
            // *** *** *** *** *** ***
          } else {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Local function
              ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "نص",
                      o_tokens->Line, o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_STRING] [" +
                                Token[FUN_POS + 2] + "] () \n\n",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Local String Func(void)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + " std::string FUNCTION_" + ID[Token[FUN_POS + 2]] +
                "(); \n";
            CPP_GLOBAL_FUN.append(" std::string FUNCTION_" +
                                  ID[Token[FUN_POS + 2]] + "(){ \n");
            // *** *** *** *** *** ***
          }

          IsInsideFunction = true;
          TheFunction = Token[FUN_POS + 2];
          TheFunction_TYPE = "نص";

          Win_FunctionNames[std::make_pair(
              TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
          Win_CurrentTotalFucntion++;
          Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

          return; // continue;
        } else if (Token[FUN_POS + 1] == "منطق") {
          if (!IsInsideNamespace) {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Global function
              ADD_FUN(true, "", Token[FUN_POS + 2], "منطق", o_tokens->Line,
                      o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_BOOL] [" +
                                Token[FUN_POS + 2] + "] () \n\n",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Global bool Func(void)
            CPP_GLOBAL.append(" bool FUNCTION_" +
                              Global_ID[Token[FUN_POS + 2]] + "(); \n");
            CPP_GLOBAL_FUN.append(" bool FUNCTION_" +
                                  Global_ID[Token[FUN_POS + 2]] + "(){ \n");
            // *** *** *** *** *** ***
          } else {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Local function
              ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "منطق",
                      o_tokens->Line, o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_BOOL] [" +
                                Token[FUN_POS + 2] + "] () \n\n",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Local INT Func(void)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + " bool FUNCTION_" + ID[Token[FUN_POS + 2]] + "(); \n";
            CPP_GLOBAL_FUN.append(" bool FUNCTION_" + ID[Token[FUN_POS + 2]] +
                                  "(){ \n");
            // *** *** *** *** *** ***
          }

          IsInsideFunction = true;
          TheFunction = Token[FUN_POS + 2];
          TheFunction_TYPE = "منطق";

          Win_FunctionNames[std::make_pair(
              TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
          Win_CurrentTotalFucntion++;
          Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

          return; // continue;
        }
      } else // دالة TYPE MyFunctionName (...)
      {
        if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
          ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);

        TempTokenCount = 0;
        for (int p = 5; p <= o_tokens->TOTAL[o_tokens->Line]; p++) {
          if (Token[p] != "") {
            TempToken[TempTokenCount] = Token[p];
            TempTokenCount++;
          }
        }

        if (Token[FUN_POS + 1] == "عدد") {
          if (!IsInsideNamespace) {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Global function
              ADD_FUN(true, "", Token[FUN_POS + 2], "عدد", o_tokens->Line,
                      o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              // Function int Syntax (..., ...)
              // To Set ARG counter, and ARG as Local Var
              ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                  true, TempToken, (TempTokenCount - 1), TheNamespace,
                  Token[FUN_POS + 2], o_tokens);

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_INT] [" +
                                Token[FUN_POS + 2] + "] ( ",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Global INT Func(...)
            CPP_GLOBAL.append(" double FUNCTION_" +
                              Global_ID[Token[FUN_POS + 2]] + "( ");
            CPP_GLOBAL_FUN.append(" double FUNCTION_" +
                                  Global_ID[Token[FUN_POS + 2]] + "( ");
            // *** *** *** *** *** ***

            // Function int Syntax (..., ...)
            // To Get Log, and C++ Code
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                true, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 2], o_tokens);
          } else {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Local function
              ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "عدد",
                      o_tokens->Line, o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              // Function int Syntax (..., ...)
              // To Set ARG counter, and ARG as Local Var
              ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                  false, TempToken, (TempTokenCount - 1), TheNamespace,
                  Token[FUN_POS + 2], o_tokens);

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_INT] [" + Token[FUN_POS + 2] +
                                "] ( ",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Local INT Func(...)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + " double FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ";
            CPP_GLOBAL_FUN.append(" double FUNCTION_" + ID[Token[FUN_POS + 2]] +
                                  "( ");
            // *** *** *** *** *** ***

            // Function int Syntax (..., ...)
            // To Get Log, and C++ Code
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                false, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 2], o_tokens);
          }

          if (DEBUG)
            DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

          // *** Generate Code ***
          if (!IsInsideNamespace) {
            // Global INT Func(...)
            CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
            CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
          } else {
            // Local INT Func(...)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + ScriptSyntaxBuffer + " ); \n";
            CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
          }
          // *** *** *** *** *** ***

          IsInsideFunction = true;
          TheFunction = Token[FUN_POS + 2];
          TheFunction_TYPE = "عدد";

          Win_FunctionNames[std::make_pair(
              TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
          Win_CurrentTotalFucntion++;
          Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

          return; // continue;
        } else if (Token[FUN_POS + 1] == "نص") {
          if (!IsInsideNamespace) {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Global function
              ADD_FUN(true, "", Token[FUN_POS + 2], "نص", o_tokens->Line,
                      o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              // Function std::string Syntax (..., ...)
              // To Set ARG counter, and ARG as Local Var
              ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                  true, TempToken, (TempTokenCount - 1), TheNamespace,
                  Token[FUN_POS + 2], o_tokens);

              ////SET_C_NAME(Token[FUN_POS + 2]);
              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_STRING] [" +
                                Token[FUN_POS + 2] + "] (",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Global String Func(...)
            ////SET_C_NAME(Token[FUN_POS + 2]);
            CPP_GLOBAL.append(" std::string FUNCTION_" +
                              Global_ID[Token[FUN_POS + 2]] + "( ");
            CPP_GLOBAL_FUN.append(" std::string FUNCTION_" +
                                  Global_ID[Token[FUN_POS + 2]] + "( ");
            // *** *** *** *** *** ***

            // Function std::string Syntax (..., ...)
            // To Get Log, and C++ Code
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                true, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 2], o_tokens);
          } else {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Local function
              ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "نص",
                      o_tokens->Line, o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              // Function std::string Syntax (..., ...)
              // To Set ARG counter, and ARG as Local Var
              ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                  false, TempToken, (TempTokenCount - 1), TheNamespace,
                  Token[FUN_POS + 2], o_tokens);

              ////SET_C_NAME(Token[FUN_POS + 2]);
              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_STRING] [" +
                                Token[FUN_POS + 2] + "] (",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Local String Func(...)
            ////SET_C_NAME(Token[FUN_POS + 2]);
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + " std::string FUNCTION_" + ID[Token[FUN_POS + 2]] +
                "( ";
            CPP_GLOBAL_FUN.append(" std::string FUNCTION_" +
                                  ID[Token[FUN_POS + 2]] + "( ");
            // *** *** *** *** *** ***

            // Function std::string Syntax (..., ...)
            // To Get Log, and C++ Code
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                false, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 2], o_tokens);
          }

          if (DEBUG)
            DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

          // *** Generate Code ***
          if (!IsInsideNamespace) {
            // Global String Func(...)
            CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
            CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
          } else {
            // Local String Func(...)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + ScriptSyntaxBuffer + " ); \n";
            CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
            // *** *** *** *** *** ***
          }

          IsInsideFunction = true;
          TheFunction = Token[FUN_POS + 2];
          TheFunction_TYPE = "نص";

          Win_FunctionNames[std::make_pair(
              TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
          Win_CurrentTotalFucntion++;
          Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

          return; // continue;
        } else if (Token[FUN_POS + 1] == "منطق") {
          if (!IsInsideNamespace) {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Global function
              ADD_FUN(true, "", Token[FUN_POS + 2], "منطق", o_tokens->Line,
                      o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              // Function int Syntax (..., ...)
              // To Set ARG counter, and ARG as Local Var
              ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                  true, TempToken, (TempTokenCount - 1), TheNamespace,
                  Token[FUN_POS + 2], o_tokens);

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[GLOBAL-FUNCTION] [FUN_TYPE_BOOL] [" +
                                Token[FUN_POS + 2] + "] ( ",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Global BOOL Func(...)
            CPP_GLOBAL.append(" bool FUNCTION_" +
                              Global_ID[Token[FUN_POS + 2]] + "( ");
            CPP_GLOBAL_FUN.append(" bool FUNCTION_" +
                                  Global_ID[Token[FUN_POS + 2]] + "( ");
            // *** *** *** *** *** ***

            // Function int Syntax (..., ...)
            // To Get Log, and C++ Code
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                true, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 2], o_tokens);
          } else {
            if (!o_tokens->TOKENS_PREDEFINED) {
              // Local function
              ADD_FUN(false, TheNamespace, Token[FUN_POS + 2], "منطق",
                      o_tokens->Line, o_tokens);

              IsInsideFunction = true;          // Need by Tokens Predefined
              TheFunction = Token[FUN_POS + 2]; // Need by Tokens Predefined

              // Function int Syntax (..., ...)
              // To Set ARG counter, and ARG as Local Var
              ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                  false, TempToken, (TempTokenCount - 1), TheNamespace,
                  Token[FUN_POS + 2], o_tokens);

              return; // continue;
            }

            if (DEBUG)
              DEBUG_MESSAGE("[FUNCTION] [FUN_TYPE_BOOL] [" +
                                Token[FUN_POS + 2] + "] ( ",
                            o_tokens); // DEBUG

            // *** Generate Code ***
            // Local BOOL Func(...)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + " bool FUNCTION_" + ID[Token[FUN_POS + 2]] + "( ";
            CPP_GLOBAL_FUN.append(" bool FUNCTION_" + ID[Token[FUN_POS + 2]] +
                                  "( ");
            // *** *** *** *** *** ***

            // Function int Syntax (..., ...)
            // To Get Log, and C++ Code
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                false, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 2], o_tokens);
          }

          if (DEBUG)
            DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

          // *** Generate Code ***
          if (!IsInsideNamespace) {
            // Global INT Func(...)
            CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
            CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
          } else {
            // Local INT Func(...)
            CBUFER =
                CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
            CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
                CBUFER + ScriptSyntaxBuffer + " ); \n";
            CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
          }
          // *** *** *** *** *** ***

          IsInsideFunction = true;
          TheFunction = Token[FUN_POS + 2];
          TheFunction_TYPE = "منطق";

          Win_FunctionNames[std::make_pair(
              TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
          Win_CurrentTotalFucntion++;
          Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

          return; // continue;
        }
      }
    }
  } else // void : دالة MyFunctionName (???) // void
  {
    if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
      ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);

    if (Token[FUN_POS + 2] != "" && Token[FUN_POS + 2] != "(")
      ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 2] + " ' ", o_tokens);

    if (Token[FUN_POS + 2] == "") // دالة MyFunctionName
    {
      if (!IsInsideNamespace) {
        if (!o_tokens->TOKENS_PREDEFINED) {
          // Global function
          ADD_FUN(true, "", Token[FUN_POS + 1], "عادم", o_tokens->Line,
                  o_tokens);

          IsInsideFunction = true;          // Need by Tokens Predefined
          TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

          ////SET_C_NAME(Token[FUN_POS + 2]);
          return; // continue;
        }

        if (DEBUG)
          DEBUG_MESSAGE("[GLOBAL-FUNCTION] [" + Token[FUN_POS + 1] + "] \n\n",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        // Global VOID Func(void)
        ////SET_C_NAME(Token[FUN_POS + 2]);
        CPP_GLOBAL.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] +
                          "(); \n");
        CPP_GLOBAL_FUN.append(" void FUNCTION_" +
                              Global_ID[Token[FUN_POS + 1]] + "(){ \n");
        // *** *** *** *** *** ***
      } else {
        if (!o_tokens->TOKENS_PREDEFINED) {
          // Local function
          ADD_FUN(false, TheNamespace, Token[FUN_POS + 1], "عادم",
                  o_tokens->Line, o_tokens);

          IsInsideFunction = true;          // Need by Tokens Predefined
          TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

          ////SET_C_NAME(Token[FUN_POS + 2]);
          return; // continue;
        }

        if (DEBUG)
          DEBUG_MESSAGE("[FUNCTION] [" + Token[FUN_POS + 1] + "] \n\n",
                        o_tokens); // DEBUG

        // *** Generate Code ***
        // Local VOID Func(void)
        ////SET_C_NAME(Token[FUN_POS + 2]);
        CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
        CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
            CBUFER + " void FUNCTION_" + ID[Token[FUN_POS + 1]] + "(); \n";
        CPP_GLOBAL_FUN.append(" void FUNCTION_" + ID[Token[FUN_POS + 1]] +
                              "(){ \n");
        // *** *** *** *** *** ***
      }

      IsInsideFunction = true;
      TheFunction = Token[FUN_POS + 1];
      TheFunction_TYPE = "عادم";
      RETURN_FUN[std::make_pair(TheNamespace, TheFunction)] = "OK";

      Win_FunctionNames[std::make_pair(TheNamespace,
                                       Win_CurrentTotalFucntion)] = TheFunction;
      Win_CurrentTotalFucntion++;
      Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

      return;                             // continue;
    } else if (Token[FUN_POS + 2] == "(") // دالة MyFunctionName ( ???
    {
      if (Token[FUN_POS + 3] == ")") // دالة MyFunctionName ()
      {
        if (Token[FUN_POS + 4] != "")
          ErrorCode("أمر غير معروف : ' " + Token[FUN_POS + 4] + " ' ",
                    o_tokens);

        if (!IsInsideNamespace) {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Global function
            ADD_FUN(true, "", Token[FUN_POS + 1], "عادم", o_tokens->Line,
                    o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

            ////SET_C_NAME(Token[FUN_POS + 2]);
            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[GLOBAL-FUNCTION] [" + Token[FUN_POS + 1] +
                              "] () \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Global VOID Func(void)
          ////SET_C_NAME(Token[FUN_POS + 2]);
          CPP_GLOBAL.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] +
                            "(); \n");
          CPP_GLOBAL_FUN.append(" void FUNCTION_" +
                                Global_ID[Token[FUN_POS + 1]] + "(){ \n");
          // *** *** *** *** *** ***
        } else {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Local function
            ADD_FUN(false, TheNamespace, Token[FUN_POS + 1], "عادم",
                    o_tokens->Line, o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

            ////SET_C_NAME(Token[FUN_POS + 2]);
            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[FUNCTION] [" + Token[FUN_POS + 1] + "] () \n\n",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Local VOID Func(void)
          ////SET_C_NAME(Token[FUN_POS + 2]);
          CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
          CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
              CBUFER + " void FUNCTION_" + ID[Token[FUN_POS + 1]] + "(); \n";
          CPP_GLOBAL_FUN.append(" void FUNCTION_" + ID[Token[FUN_POS + 1]] +
                                "(){ \n");
          // *** *** *** *** *** ***
        }

        IsInsideFunction = true;
        TheFunction = Token[FUN_POS + 1];
        TheFunction_TYPE = "عادم";
        RETURN_FUN[std::make_pair(TheNamespace, TheFunction)] = "OK";

        Win_FunctionNames[std::make_pair(
            TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
        Win_CurrentTotalFucntion++;
        Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

        return; // continue;
      } else    // دالة MyFunctionName (...)
      {
        if (Token[o_tokens->TOTAL[o_tokens->Line] - 1] != ")")
          ErrorCode("يجب انهاء السطر بالإشارة ')' ", o_tokens);

        TempTokenCount = 0;
        for (int p = 4; p <= o_tokens->TOTAL[o_tokens->Line]; p++) {
          if (Token[p] != "") {
            TempToken[TempTokenCount] = Token[p];
            TempTokenCount++;
          }
        }

        if (!IsInsideNamespace) {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Global function
            ADD_FUN(true, "", Token[FUN_POS + 1], "عادم", o_tokens->Line,
                    o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

            // Function std::string Syntax (..., ...)
            // To Set ARG counter, and ARG as Local Var
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                true, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 1], o_tokens);

            ////SET_C_NAME(Token[FUN_POS + 2]);
            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[GLOBAL-FUNCTION] [" + Token[FUN_POS + 1] + "] ( ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Global VOID Func(...)
          ////SET_C_NAME(Token[FUN_POS + 2]);
          CPP_GLOBAL.append(" void FUNCTION_" + Global_ID[Token[FUN_POS + 1]] +
                            "( ");
          CPP_GLOBAL_FUN.append(" void FUNCTION_" +
                                Global_ID[Token[FUN_POS + 1]] + "( ");
          // *** *** *** *** *** ***

          // Function std::string Syntax (..., ...)
          // To Get Log, and C++ Code
          ScriptSyntaxBuffer =
              CHECK_NEW_FUN_SYNTAX(true, TempToken, (TempTokenCount - 1),
                                   TheNamespace, Token[FUN_POS + 1], o_tokens);
        } else {
          if (!o_tokens->TOKENS_PREDEFINED) {
            // Local function
            ADD_FUN(false, TheNamespace, Token[FUN_POS + 1], "عادم",
                    o_tokens->Line, o_tokens);

            IsInsideFunction = true;          // Need by Tokens Predefined
            TheFunction = Token[FUN_POS + 1]; // Need by Tokens Predefined

            // Function std::string Syntax (..., ...)
            // To Set ARG counter, and ARG as Local Var
            ScriptSyntaxBuffer = CHECK_NEW_FUN_SYNTAX(
                false, TempToken, (TempTokenCount - 1), TheNamespace,
                Token[FUN_POS + 1], o_tokens);

            ////SET_C_NAME(Token[FUN_POS + 2]);
            return; // continue;
          }

          if (DEBUG)
            DEBUG_MESSAGE("[FUNCTION] [" + Token[FUN_POS + 1] + "] ( ",
                          o_tokens); // DEBUG

          // *** Generate Code ***
          // Local VOID Func(...)
          ////SET_C_NAME(Token[FUN_POS + 2]);
          CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
          CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
              CBUFER + " void FUNCTION_" + ID[Token[FUN_POS + 1]] + "( ";
          CPP_GLOBAL_FUN.append(" void FUNCTION_" + ID[Token[FUN_POS + 1]] +
                                "( ");
          // *** *** *** *** *** ***

          // Function std::string Syntax (..., ...)
          // To Get Log, and C++ Code
          ScriptSyntaxBuffer =
              CHECK_NEW_FUN_SYNTAX(false, TempToken, (TempTokenCount - 1),
                                   TheNamespace, Token[FUN_POS + 1], o_tokens);
        }

        if (DEBUG)
          DEBUG_MESSAGE(") \n\n", o_tokens); // DEBUG

        // *** Generate Code ***
        if (!IsInsideNamespace) {
          // Global VOID Func(...)
          CPP_GLOBAL.append(NEW_FUNCTION_ARG + " ); \n");
          CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
        } else {
          // Local VOID Func(...)
          CBUFER = CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")];
          CPP_WINDOW[std::make_pair(TheNamespace, "FUN_DECLARATION")] =
              CBUFER + ScriptSyntaxBuffer + " ); \n";
          CPP_GLOBAL_FUN.append(ScriptSyntaxBuffer + " ){ \n");
          // *** *** *** *** *** ***
        }

        IsInsideFunction = true;
        TheFunction = Token[FUN_POS + 1];
        TheFunction_TYPE = "عادم";
        RETURN_FUN[std::make_pair(TheNamespace, TheFunction)] = "OK";

        Win_FunctionNames[std::make_pair(
            TheNamespace, Win_CurrentTotalFucntion)] = TheFunction;
        Win_CurrentTotalFucntion++;
        Win_TotalFucntion[TheNamespace] = Win_CurrentTotalFucntion;

        return; // continue;
      }
    }
  }
}
