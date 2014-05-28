/**
 * @file:vector_std_functions.hpp
 * A set of functions for working with vectors in scheme.
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
    void registerVectorFunctions( Context* ctx, parser::StringTable* strs);
}
