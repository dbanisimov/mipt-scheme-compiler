/**
 * File: lowering_exception.hpp
 * Copyright (C) 2012 Denis Anisimov
 */

#pragma once

#include <string>

class LoweringException :public std::exception
{
public:
    LoweringException( std::string msg)
        :message( msg) {}
    ~LoweringException() throw() {}

    virtual const char* what() const throw()
    { return message.c_str(); }
private:
    std::string message;
};
