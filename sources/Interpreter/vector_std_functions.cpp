/**
 * @file:vector_std_functions.cpp
 * A definition of the most basic functions accessing vectors.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <vector>
#include "vector_std_functions.hpp"
#include "exp_checker.hpp"
#include "interpreter_dep.hpp"
#include "context.hpp"
#include "evaluator.hpp"
#include "est_support.hpp"

namespace interpreter
{

using std::vector;

SExprp makeVector( SExprp args, Evaluator* eva)
{
    ensureOr ( ensureList2<Number, SExpr>,
               ensureList1<Number>, args, eva);
    int length = as<Number>( as<Cons>( args)->car)->val();
    SExprp cell = Nil::make();
    if ( as<Cons>( args)->cdr->type != SExpr::nil)
        cell = nth<1>( args)->car;

    return Vector::make( vector<SExprp>( length, cell));
}

SExprp vectorFun( SExprp args, Evaluator* eva)
{
    vector<SExprp> result;
    while ( args->type != SExpr::nil)
    {
        ensureCons< SExpr, SExpr>( args, eva);
        result.push_back( as<Cons>( args)->car);
        args = as<Cons>( args)->cdr;
    }
    return Vector::make( result);
}

SExprp vectorLength( SExprp args, Evaluator* eva)
{
    ensureList1<Vector>( args, eva);
    return Number::make( as<Vector>( as<Cons>( args)->car)->val().size());
}

SExprp vectorRef( SExprp args, Evaluator* eva)
{
    ensureList2<Vector, Number>( args, eva);
    const vector<SExprp>& vec = as<Vector>( as<Cons>( args)->car)->val();
    int pos = as<Number>( nth<1>( args)->car)->val();
    if ( pos >= vec.size() || 0 > pos)
        throw Exception( "index must be inside the vector", args);
    return vec[pos];
}

SExprp vectorSet( SExprp args, Evaluator* eva)
{
    ensureList3<Vector, Number, SExpr>( args, eva);
    vector<SExprp>& vec = as<Vector>( as<Cons>( args)->car)->val();
    int pos = as<Number>( nth<1>( args)->car)->val();
    if ( pos >= vec.size() || 0 > pos)
        throw Exception( "index must be inside the vector", args);
    SExprp val = nth<2>( args)->car;
    vec[pos] = val;
    return as<Cons>( args)->car;
}


void registerVectorFunctions( Context* ctx, StringTable* strs)
{
    ctx->regFunction( strs->id( "make-vector"), &makeVector);
    ctx->regFunction( strs->id( "vector"), &vectorFun);
    ctx->regFunction( strs->id( "vector-length"), &vectorLength);
    ctx->regFunction( strs->id( "vector-ref"), &vectorRef);
    ctx->regFunction( strs->id( "vector-set!"), &vectorSet);
}

}//namespace interpreter
