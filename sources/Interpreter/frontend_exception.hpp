/**
 * @file:frontend_exception.hpp
 * A base exception for all lexer, parser and interpreter situations.
 * @author: Arseniy Zaostovnykh
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>

class FrontendException :public std::exception
{
public:
    FrontendException( std::string msg)
        :message( msg) {}
    ~FrontendException() throw() {}
    
    virtual const char* what() const throw()
        { return message.c_str(); }
private:
    std::string message;
};
