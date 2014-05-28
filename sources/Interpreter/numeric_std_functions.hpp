/**
 * @file:numeric_std_functions.hpp
 * A set of various numeric functions and operations declaration.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace parser
{
    class StringTable;
}

namespace interpreter
{
    class Context;
    void registerNumericFunctions( Context* ctx, parser::StringTable* strs);
}
