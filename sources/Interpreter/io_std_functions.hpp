/**
 * @file:io_std_functions.hpp
 * An interface for registration standard scheme IO functions, 
 * working with ports
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
    void registerIOFunctions( Context* ctx, parser::StringTable* strs);
}
