/**
 * @file:parser_exception.hpp
 * An exceptions holder for parser.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>
#include "frontend_exception.hpp"

namespace parser
{

class Exception :public FrontendException
{
public:
    Exception( std::string msg, int lineno)
        :FrontendException( msg), line( lineno) {}
    ~Exception() throw() {}

    int lineno() const { return line; }
private:
    int line;
};

}
