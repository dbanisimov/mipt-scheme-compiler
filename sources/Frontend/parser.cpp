/**
 * @file:parser.cpp 
 * Implementation of scheme parser relying on an appropriate lexer, interfacing
 * through tokens.hpp and lexer_data.h.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <FlexLexer.h>
#include <string>
#include <fstream>
#include <boost/format.hpp>
#include <vector>
#include "tokens.hpp"
#include "lexer_data.h"
#include "ast.hpp"
#include "parser.hpp"
#include "parser_exception.hpp"
#include "string_table.hpp"

namespace parser
{

using std::string;
using std::vector;
using std::ifstream;
using boost::format;
using boost::str;
using namespace ast;

Parser::Parser( StringTable* strs, std::istream* in):lex(in), mine_istream(0),
                                                     end(false), strings( strs)
{
}

Parser::Parser( StringTable* strs, string fname) : end(false), strings( strs)
{
    mine_istream = new ifstream( fname.c_str());
    lex.switch_streams( mine_istream);
}

Parser::~Parser()
{
    if (mine_istream)
    {
        if (mine_istream->is_open())
            mine_istream->close();
        delete mine_istream;
    }
}

SExprp Parser::read( bool* is_end)
{
    tokens::Tokens token = getToken();
    if (token == tokens::EXIT)
    {
        if ( is_end) *is_end = true;
        else throw Exception( "unexpected EOF", lex.lineno() );
        end = true;
        return Nil::make();
    }
    if ( is_end) *is_end = false;
    return scanToken( token);
}

vector<SExprp> Parser::readAll()
{
    vector<SExprp> ret;
    SExprp next = read();
    while (!eof())
    {
        ret.push_back(next);
        next = read();
    }
    return ret;
}

bool Parser::eof() const
{
    return end;
}

bool Parser::fileOpened() const
{
    return mine_istream != 0 &&
        mine_istream->good();
}

StringTable* Parser::getStrings()
{
    return strings;
}

tokens::Tokens Parser::getToken()
{
    return (tokens::Tokens)lex.yylex();
}

SExprp Parser::scanList()
{
    tokens::Tokens token = getToken();
    if ( token == tokens::RPAREN)
        return Nil::make();
    if ( token == tokens::DOT )
    {
        token = getToken();
        SExprp cdr = scanToken( token);
        token = getToken();
        if ( token != tokens::RPAREN)
            throw Exception( "wrong dotted list", lex.lineno());
        return cdr;
    }
    SExprp car = scanToken( token);
    return Cons::make( car, scanList());
}

SExprp Parser::scanVector()
{
    vector<SExprp> content;
    tokens::Tokens token = getToken();
    while ( token != tokens::RPAREN)
    {
        if ( token == tokens::DOT )
            throw Exception( "dot can not be met inside a first level of a vector",
                             lex.lineno());
        if ( token == tokens::EXIT )
            throw Exception( "unexpected end of input inside an array",
                             lex.lineno());
        content.push_back( scanToken( token));
        token = getToken();
    }
    return Vector::make( content);
}

SExprp Parser::expandShortcut( string name)
{
    tokens::Tokens token = getToken();
    SExprp tail = Cons::make( scanToken( token), Nil::make());
    return Cons::make( Symbol::make( name, strings), tail);
}

SExprp Parser::scanToken( tokens::Tokens token)
{
    switch ( token)
    {
    case tokens::INTEGER:
        return Number::make( lex_int_read);
    case tokens::FLOATING:
        return Number::make( lex_fl_read);
    case tokens::SYMBOL:
        return Symbol::make( lex_str_read, strings);
    case tokens::STRING:
        return StrConst::make( lex_str_read);
    case tokens::BOOLEAN:
        return Boolean::make( lex_bool_read);
    case tokens::CHARACTER:
        return Character::make( lex_ch_read);
    case tokens::RPAREN:
        throw Exception( "unmatched right parenthesis", lex.lineno());
    case tokens::LPAREN:
        return scanList();
    case tokens::VECTORLPAREN:
        return scanVector();
    case tokens::QUOTE:
        return expandShortcut( "quote");
    case tokens::QUASIQUOTE:
        return expandShortcut( "quasiquote");
    case tokens::UNQUOTE:
        return expandShortcut( "unquote");
    case tokens::UNQUOTESPLICING:
        return expandShortcut( "unquote-splicing");
    case tokens::DOT:
        throw Exception( "misplaced dot", lex.lineno());
    default:
        throw Exception( str( format( "unrecognized token: %1%") % token),
                         lex.lineno());
    }
}

} //namespace parser
