/**
 * @file:vocab.lex
 * Implementation of scheme lexer, speaks via tokens.hpp, and transmits
 * the attached data via lexer_data.h global variables.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

%top{
#include <stdarg.h>
#include "lexer_data.h"
#include "tokens.hpp"

}
%{
#define USER_STRING_SIZE 32768

void yyerror(char const *format, ...);

int comment_depth = 0;

%}

%Start STRST COMMENT
%option noyywrap yylineno c++

DIGIT [0-9]
LETTER [[:alpha:]]
EXALPHA [][!$%&*+./:<=>?@^_~{}-]|"|"
CHAR {DIGIT}|{LETTER}|{EXALPHA}
WS [ \t]
%%

<INITIAL>{
"("                      { return tokens::LPAREN; }
")"                      { return tokens::RPAREN; }
"."                      { return tokens::DOT; }
"'"                      { return tokens::QUOTE; }
"`"                      { return tokens::QUASIQUOTE; }
","                      { return tokens::UNQUOTE; }
",@"                     { return tokens::UNQUOTESPLICING; }
"#("                     { return tokens::VECTORLPAREN; }
"#vu8("                  { return tokens::BYTEVECTORLPAREN; }
"#|"                     { ++comment_depth; BEGIN(COMMENT); }
;.*\n                    { /* skip on-lined comment */ }
-?{DIGIT}+               { lex_int_read = atoi( YYText());
                                    return tokens::INTEGER; }
-?{DIGIT}*\.{DIGIT}+     { lex_fl_read = atof( YYText());
                                    return tokens::FLOATING; }
{CHAR}({CHAR}|"#")*      { lex_str_read = YYText();
                                    return tokens::SYMBOL; }
#t|#T                    { lex_bool_read = true;
                                    return tokens::BOOLEAN; }
#f|#F                    { lex_bool_read = false;
                                    return tokens::BOOLEAN; }
#\\({CHAR}|" "|")"|"("|"\""|"'"|"\\") {
                           lex_ch_read = YYText()[2];
                                    return tokens::CHARACTER; }

({WS}|\n)+                    { /* omit blanks */}


\"                       { lex_str_read =""; BEGIN(STRST);}
<<EOF>>                  { return tokens::EXIT; }
}

<COMMENT>{
    "#|"                       { ++comment_depth;}
    "|#"                       { if ( --comment_depth == 0)
                                   BEGIN( INITIAL); }
    .|\n                       { /* skip every character */}
    <<EOF>>                    { yyerror( "unmatched #|"); }
 }

<STRST>{
    \"                         { BEGIN( INITIAL);
                                 return tokens::STRING; }
    .|\n                       { lex_str_read += YYText();
                                 if ( lex_str_read.length() >= USER_STRING_SIZE )
                                     yyerror( "string is too long");}
    <<EOF>>                    { yyerror( "unmatched \" sybol ");
                                 BEGIN( INITIAL);}
}

.                              { yyerror( "unrecognized token %s", YYText()); }
%%

void
yyerror( char const *format, ...)
{
  va_list args;
  va_start(args, format);
  vfprintf( stderr, format, args);
  va_end(args);
}

//TODO: support cammentaries
/* Defenition of data holders, used for
   transfering token contents to the parser */
std::string lex_str_read;
int lex_int_read;
long double lex_fl_read;
bool lex_bool_read;
char lex_ch_read;

