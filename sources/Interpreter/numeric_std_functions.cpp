/**
 * @file:numeric_std_functions.cpp
 * A basic set of numeric operators and functions definition.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "interpreter_dep.hpp"
#include "numeric_std_functions.hpp"
#include "exp_checker.hpp"
#include "context.hpp"
#include "evaluator.hpp"
#include "est_support.hpp"

namespace interpreter
{

#define BINARY_BOOL_OPERATION( name, sign)                              \
    SExprp name( SExprp args, Evaluator* eva)                           \
    {                                                                   \
        ensureList2<Number, Number>( args, eva);                        \
        return Boolean::make( as<Number>( as<Cons>( args)->car)->val()  \
                              sign as<Number>( nth<1>( args)->car)->val()); \
    }                                                                       

#define BINARY_OPERATION( name, sign)                                   \
    SExprp name( SExprp args, Evaluator* eva)                           \
    {                                                                   \
        ensureList2<Number, Number>( args, eva);                        \
        return Number::make( as<Number>( as<Cons>( args)->car)->val()   \
                             sign as<Number>( nth<1>( args)->car)->val()); \
    }                                                                       

BINARY_BOOL_OPERATION( less, <);
BINARY_BOOL_OPERATION( numEqu, ==);
BINARY_BOOL_OPERATION( lessEqu, <=);
BINARY_BOOL_OPERATION( greather, >);
BINARY_BOOL_OPERATION( greatherEqu, >=);

BINARY_OPERATION( add, +);
BINARY_OPERATION( sub, -);
BINARY_OPERATION( div, /);
BINARY_OPERATION( mul, *);

SExprp truncate( SExprp args, Evaluator* eva)
{
    ensureList1<Number>( args, eva);
    return Number::make( (long long)as<Number>( as<Cons>( args)->car)->val());
}

void registerNumericFunctions( Context* ctx, StringTable* strs)
{
    ctx->regFunction( strs->id( "="), &numEqu);
    ctx->regFunction( strs->id( "<"), &less);
    ctx->regFunction( strs->id( "<="), &lessEqu);
    ctx->regFunction( strs->id( ">"), &greather);
    ctx->regFunction( strs->id( ">="), &greatherEqu);
    ctx->regFunction( strs->id( "+"), &add);
    ctx->regFunction( strs->id( "-"), &sub);
    ctx->regFunction( strs->id( "/"), &div);
    ctx->regFunction( strs->id( "*"), &mul);
    ctx->regFunction( strs->id( "truncate"), &truncate);
}

}//namespace interpreter
