/**
 * @file:context.cpp
 * A defenition of context-managing class routines.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "context.hpp"
#include "interpreter_exception.hpp"

namespace interpreter
{

using namespace est;

void noDelete( void* smthp) { /* do not delete it */}

Context::Context( StringTable* strs_)
    :nextUniqId( 0), vars(), strs( strs_),
     current_input( Port::make( Istreamp( &std::cin, &noDelete), Ostreamp())),
     current_output( Port::make( Istreamp(), Ostreamp(&std::cout, &noDelete)))
{
    enterScope();
}


Context::~Context()
{
}

void Context::enterScope()
{
    vars = Activationp( new Activation( vars));
}

void Context::exitScope()
{
    vars = vars->parent();
}

Activationp Context::activation()
{
    return vars;
}

void Context::setActivation( Activationp act)
{
    vars = act;
}

SExprp Context::getVariable( int name)
{
    if ( !knownVar (name))
    {
        throw Exception( "var unbound", StrConst::make( strs->str( name)));
    }
    return vars->getSymbol( name);
}

SExprp Context::funcall( int id, SExprp args, Evaluator* eva)
{
    if ( !knownFun (id))
    {
        throw Exception( "The function doesn't exist",
                         Number::make( id));
    }
    return functions[id]( args, eva);
}
int Context::regFunction( fun val)
{
    int id = generateUniqId();
    functions[id] = val;
    return id;
}
void Context::regFunction( int name, fun val)
{
    int id = regFunction( val);
    defVariable( name, FunId::make( id));
}

SExprp Context::macroexpand( int id, SExprp args, Evaluator* eva)
{
    if ( !knownMacro( id) )
    {
        throw Exception( "The macros doesn't exist",
                         Number::make( id));
    }
    return macroses[id]( args, eva);
}
int Context::regMacro( fun val)
{
    int id = generateUniqId();
    macroses[id] = val;
    return id;
}
void Context::regMacro( int name, fun val)
{
    int id = regMacro( val);
    defVariable( name, MacId::make( id));
}
SExprp Context::applySpec( int id, SExprp args, Evaluator* eva)
{
    if ( !knownSpec( id) )
    {
        throw Exception( "The special form doesn't exist",
                         Number::make( id));
    }
    return specs[id]( args, eva);
}

int Context::regSpec( spec_form spec)
{
    int id = generateUniqId();
    specs[id] = spec;
    return id;
}
void Context::regSpec( int name, spec_form spec)
{
    int id = regSpec( spec);
    defVariable( name, SpecId::make( id));
}

}//namespace interpreter
