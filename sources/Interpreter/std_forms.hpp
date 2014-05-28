/**
 * @file:std_forms.hpp
 * An interface file for function, registering standard scheme functions.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>

namespace parser
{
    class StringTable;
}

namespace interpreter
{
    class Context;
    void registerStdForms( Context* ctx, parser::StringTable* strs);

    class Gensym
    {
        StringTable* strs_;
    public:
        Gensym( StringTable* strs) :strs_( strs) {}
        
        SExprp operator()( SExprp args, Evaluator* eva);
    };

}
