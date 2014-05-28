/**
 * @file:frontend_exception.hpp
 * A base exception for all parser situations.
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
