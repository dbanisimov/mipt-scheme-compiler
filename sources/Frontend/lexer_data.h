/**
 * @file:lexer_data.h
 * Declaration of globals, used in transmission of tokens data
 * from lexer to parser of scheme.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>

extern std::string lex_str_read;
extern int lex_int_read;
extern long double lex_fl_read;
extern bool lex_bool_read;
extern char lex_ch_read;

