/**
 * @file:interpreter_exception.hpp
 * An exception representation of interpreter especial situations.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>
#include "frontend_exception.hpp"
#include "stack_trace.hpp"
#include "est.hpp"

namespace interpreter
{

class Exception :public FrontendException
{
public:
    Exception( std::string msg, est::SExprp problem, const StackTrace& bt = StackTrace())
        :FrontendException( msg), problem_( problem), trace( bt) {}
    ~Exception() throw() {}

    est::SExprp problem() const { return problem_; }
    const StackTrace& backtrace() const { return trace; }

private:
    est::SExprp problem_;
    StackTrace trace;
};

}
