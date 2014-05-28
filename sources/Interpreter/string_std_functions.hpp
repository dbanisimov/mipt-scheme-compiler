/**
 * @file:string_std_functions.hpp
 * A set of functions for working with strings and characters in scheme.
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
    void registerStringFunctions( Context* ctx, parser::StringTable* strs);
}
