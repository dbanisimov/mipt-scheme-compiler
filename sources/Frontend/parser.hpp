/**
 * @file:parser.hpp 
 * Interface of scheme parser relying on an appropriate lexer, interfacing
 * through tokens.hpp and lexer_data.h.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <FlexLexer.h>
#include "tokens.hpp"

namespace parser
{

class StringTable;

class Parser
{
public:
    Parser (StringTable* strs, std::istream *in);
    Parser (StringTable* strs, std::string filename);
    virtual ~Parser();
    ast::SExprp read( bool* is_end = 0);
    std::vector<ast::SExprp> readAll();
    bool eof() const;
    bool fileOpened() const;

    StringTable* getStrings();
private:
    ast::SExprp scanToken( tokens::Tokens token);
    ast::SExprp scanVector();
    ast::SExprp expandShortcut( std::string name);
    ast::SExprp scanList();
    tokens::Tokens getToken();

    yyFlexLexer lex;
    std::ifstream *mine_istream;
    bool end;

    StringTable* strings;
};

} //namespace parser
