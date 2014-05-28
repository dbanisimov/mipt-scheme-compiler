/**
 * @file:tokens.hpp
 * A enumiration defining constants, naming all tokens, for communication
 * between lexer, and parser.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace tokens
{
    enum Tokens
    {
        EXIT,
        INTEGER,
        FLOATING,
        STRING,
        SYMBOL,
        VECTORLPAREN,
        BYTEVECTORLPAREN,
        LPAREN,
        RPAREN,
        BOOLEAN,
        CHARACTER,
        DOT,
        QUOTE,
        QUASIQUOTE,
        UNQUOTE,
        UNQUOTESPLICING
    };
}
