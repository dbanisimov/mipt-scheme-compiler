/**
 * @file:environment.cpp
 * Implementation of procedures and operators of a set of 
 * classes, supporting scheme interpretation
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "environment.hpp"
#include "context.hpp"

namespace interpreter
{
using namespace est;

SExprp FunctionManager::operator()( SExprp args, Evaluator* eva)
{
    Lambda* l = makeLambda( args, eva->context()->activation(), eva, true);
    return SExprp( new FunId( eva->context()->regFunction( *l)));
}

SExprp MacroManager::operator()( SExprp args, Evaluator* eva)
{
    Lambda* l = makeLambda( args, eva->context()->activation(), eva, false);
    return SExprp( new MacId( eva->context()->regMacro( *l)));
}

}//namespace interpreter
