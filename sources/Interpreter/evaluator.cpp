/**
 * @file:evaluator.cpp
 * An implementation of class, responding for evaluation any S-expression, in 
 * given context. It follows for Visitor pattern, visiting every node in AST.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "evaluator.hpp"
#include "est_support.hpp"
#include "interpreter_exception.hpp"
#include "exp_checker.hpp"
#include <stdio.h>
namespace interpreter
{

using namespace est;

SExprp Evaluator::eval( SExprp root, bool tail)
{
    SExprp rez = root.visit( this);
    while ( rez->type == SystemObj::delayed_call
            && !tail)
    {
        rez = evalDelay( rez);
    }
    return rez;
}

string Evaluator::tryGetName( SExprp exp)
{
    if ( exp->type == SExpr::symbol)
        return ctx->getStrings()->str( as<Symbol>(exp)->getName());
    return "<?>";
}

SExprp Evaluator::onNil( SExprp subj)
{
    throw Exception( "evaluation of nil", subj, backtrace());
}

SExprp Evaluator::onCons( SExprp subj)
{
    SExprp fun = eval( as<Cons>(subj)->car, false);
    string name = tryGetName( as<Cons>(subj)->car);

    switch ( fun->type)
    {
    case SystemObj::funid:
        return delayCall( as<FunId>(fun)->id(), as<Cons>(subj)->cdr, name);
    case SystemObj::macid:
        return macroexpand( as<MacId>(fun)->id(), as<Cons>(subj)->cdr, name);
    case SystemObj::specid:
        return specprocess( as<SpecId>(fun)->id(), as<Cons>(subj)->cdr, name);
    default:
        throw Exception( "it must be a function or macro, or a special form ",
                         as<Cons>(subj)->car, backtrace());
    };
}
SExprp Evaluator::onSymbol( SExprp subj)
{
    int name =  as<Symbol>(subj)->getName();
    if ( !ctx->knownVar( name))
        throw Exception( "var unbound", subj, backtrace());
    return ctx->getVariable( name);
}
SExprp Evaluator::onVector( SExprp subj)
{
    return subj;
}
SExprp Evaluator::onNumber( SExprp subj)
{
    return subj;
}
SExprp Evaluator::onStrConst( SExprp subj)
{
    return subj;
}
SExprp Evaluator::onCharacter( SExprp subj)
{
    return subj;
}
SExprp Evaluator::onBoolean( SExprp subj)
{
    return subj;
}

SExprp Evaluator::evalDelay( SExprp delay)
{
    assert( delay->type == SystemObj::delayed_call);
    DelayedCall* call = as<DelayedCall>( delay);
    return funcall( call->id(), call->args(), call->name());
}
SExprp Evaluator::funcall( int id, SExprp args, std::string name)
{
    InCallAuto ica( callType::fun, args, &trace, name);
    return ctx->funcall( id, args, this);
}
SExprp Evaluator::delayCall( int id, SExprp rest, std::string name)
{
    ensureOr( ensureCons<SExpr, SExpr>,
              ensure<Nil>, rest, this);
    SExprp args( new Nil);
    for ( SExprp arg = rest;
          arg->type != SExpr::nil;
          arg = as<Cons>(arg)->cdr )
    {//check here for type == cons
        args = Cons::make( eval( as<Cons>(arg)->car, false), args);
    }
//    InCallAuto ica( callType::fun, reverse( args), &trace, name);
//    return ctx->funcall( id, reverse( args), this);
    return DelayedCall::make( id, reverse( args), name);
}

SExprp Evaluator::macroexpand( int id, SExprp rest, std::string name)
{
    InCallAuto ica( callType::mac, rest, &trace, name);
    return eval( ctx->macroexpand( id, rest, this), true);
}

SExprp Evaluator::specprocess( int id, SExprp rest, std::string name)
{
    InCallAuto ica ( callType::spec, rest, &trace, name);
    return ctx->applySpec( id, rest, this);
}

Context* Evaluator::context() const 
{
    return ctx;
}

const StackTrace& Evaluator::backtrace() const 
{
    return trace;
}

}//namespace interpreter
