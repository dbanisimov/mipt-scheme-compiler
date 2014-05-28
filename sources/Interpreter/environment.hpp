/**
 * @file:environment.hpp
 * Declaration of a set of 
 * classes, supporting scheme interpretation
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include "lambda.hpp"
#include "interpreter_dep.hpp"

namespace interpreter
{

class Context;
class Evaluator;

using lambda::Lambda;
using lambda::LambdaManager;
using parser::ast::SExprp;

class FunctionManager :private LambdaManager
{
public:
    SExprp operator()( SExprp args, Evaluator* eva);
};

class MacroManager :private LambdaManager
{
public:
    SExprp operator()( SExprp args, Evaluator* eva);
};

}//namespace interpreter
