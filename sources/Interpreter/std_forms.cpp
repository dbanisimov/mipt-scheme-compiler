/**
 * @file:std_forms.cpp
 * A sort of minimal set of standard froms, required to be implemented
 * in c in order to have fully functional scheme interpreter.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <string>
#include <vector>
#include <boost/format.hpp>
#include "interpreter_dep.hpp"
#include "context.hpp"
#include "evaluator.hpp"
#include "est_support.hpp"
#include "interpreter_exception.hpp"
#include "exp_checker.hpp"
#include "io_std_functions.hpp"
#include "string_std_functions.hpp"
#include "vector_std_functions.hpp"
#include "numeric_std_functions.hpp"
#include "std_forms.hpp"

namespace interpreter
{

using std::vector;
using std::string;
using parser::StringTable;

using namespace est;

SExprp ifSpec( SExprp body, Evaluator* eva)
{
    ensureOr( ensureList2<SExpr, SExpr>,//one or two alternatives
              ensureList3<SExpr, SExpr, SExpr>, body, eva);
    SExprp cond = eva->eval( as<Cons>(body)->car, false);
    SExprp if_true = nth<1>(body)->car;
    SExprp if_false = Boolean::make( false);
    if ( nth<1>(body)->cdr->type == SExpr::cons)
        if_false = nth<2>(body)->car;
    if ( (cond->type != SExpr::boolean_const)
         || as<Boolean>(cond)->val() )
        return eva->eval( if_true, true);
    return eva->eval( if_false, true);
}

SExprp set( SExprp args, Evaluator* eva)
{
    ensureList2<Symbol, SExpr>( args, eva);
    SExprp var = as<Cons>(args)->car;
    if ( !eva->context()->knownVar( as<Symbol>(var)->getName()))
        throw Exception( "var unbound", var);
    SExprp val = eva->eval( nth<1>(args)->car, false);
    eva->context()->setVariable( as<Symbol>(var)->getName(), val);
    return val;
}

SExprp begin( SExprp exps, Evaluator* eva)
{
    SExprp rez = Nil::make();
    while ( exps->type != SExpr::nil)
    {
        ensure<Cons>( exps, eva);
        bool tail = as<Cons>(exps)->cdr->type == SExpr::nil;
        rez = eva->eval( as<Cons>(exps)->car, tail);
        exps = as<Cons>(exps)->cdr;
    }
    return rez;
}

SExprp def( SExprp body, Evaluator* eva)
{
    ensureList2<Symbol, SExpr>( body, eva);
    Context* ctx = eva->context();
    SExprp var = as<Cons>(body)->car;
    SExprp val = eva->eval( nth<1>(body)->car, false);

    ctx->defVariable( as<Symbol>(var)->getName(), val);
    return val;
}

SExprp apply( SExprp args, Evaluator* eva)
{
    ensureList2<FunId, SExpr>( args, eva);
    SExprp fun = as<Cons>(args)->car;
    SExprp list = nth<1>(args)->car;
    ensureOr( ensure<Cons>, ensure<Nil>, list, eva);
    return eva->funcall( as<FunId>(fun)->id(), list,
                         eva->tryGetName( as<Cons>(args)->car));
}

SExprp quote( SExprp args, Evaluator* eva)
{
    ensureList1<SExpr>( args, eva);
    return as<Cons>(args)->car;
}

SExprp unQuote( SExprp arg, Evaluator* eva)
{
    ensureList1<SExpr>( arg, eva);
    throw Exception( "unquote outside the quasi-quote", arg);
}

SExprp unQuoteSplicing( SExprp arg, Evaluator* eva)
{
    ensureList1<SExpr>( arg, eva);
    throw Exception( "unquote-splicing outside the quasi-quote is forbidden", arg);
}

SExprp cons( SExprp args, Evaluator* eva)
{
    ensureList2<SExpr, SExpr>( args, eva);
    SExprp car = as<Cons>(args)->car;
    SExprp cdr = as<Cons>(as<Cons>(args)->cdr)->car;
    return Cons::make( car, cdr);
}

SExprp car( SExprp args, Evaluator* eva)
{
    ensureList1<Cons>( args, eva);
    SExprp arg = as<Cons>(args)->car;
    return as<Cons>(arg)->car;
}

SExprp cdr( SExprp args, Evaluator* eva)
{
    ensureList1<Cons>( args, eva);
    SExprp arg = as<Cons>(args)->car;
    return as<Cons>(arg)->cdr;
}

SExprp setCar( SExprp args, Evaluator* eva)
{
    ensureList2<Cons, SExpr>( args, eva);
    SExprp var = as<Cons>(args)->car;
    SExprp val = nth<1>(args)->car;
    as<Cons>(var)->car = val;
    return val;
}

SExprp setCdr( SExprp args, Evaluator* eva)
{
    ensureList2<Cons, SExpr>( args, eva);
    SExprp var = as<Cons>(args)->car;
    SExprp val = nth<1>(args)->car;
    as<Cons>(var)->cdr = val;
    return val;
}
/* <-- wrong. function list? check whole list not to conatin a dot.
SExprp isList( SExprp args, Evaluator* eva)
{
    assert( listLength( args) == 1);
    SExprp l = as<Cons>(args)->car;
    return Boolean::make( l->isList());
}
*/

template<SExpr::Type t>
SExprp isType( SExprp args, Evaluator* eva)
{
    ensureList1<SExpr> ( args, eva);
    SExprp val = as<Cons>(args)->car;
    return Boolean::make( val->type == t);
}

SExprp ext( SExprp args, Evaluator* eva)
{
    ensure<Nil>( args, eva);
    exit(0);
    return SExprp( new Nil);
}

SExprp eq( SExprp args, Evaluator* eva)
{
    ensureList2<SExpr, SExpr>( args, eva);
    return Boolean::make( areEq( as<Cons>(args)->car, nth<1>(args)->car));
}

SExprp eqv( SExprp args, Evaluator* eva)
{
    ensureList2<SExpr, SExpr>( args, eva);
    return Boolean::make( areEqv( as<Cons>(args)->car, nth<1>(args)->car));
}//TODO: should it work like: (eqv? #'a #'a) -> #t ?

SExprp symbolToString( SExprp args, Evaluator* eva)
{
    ensureList1<Symbol>( args, eva);
    Symbol* s = as<Symbol>( as<Cons>( args)->car);
    string name = eva->context()->getStrings()->str( s->getName());
    return StrConst::make( name);
}

SExprp syntax( SExprp args, Evaluator* eva)
{
    ensureList1<SExpr>( args, eva);
    return Syntax::make( as<Cons>( args)->car, eva->context()->activation());
}

SExprp isIdentifier( SExprp args, Evaluator* eva)
{
    ensureList1<SExpr>( args, eva);
    return Boolean::make( as<Cons>( args)->car->type == SystemObj::syntax 
                          && as<Syntax>( as<Cons>( args)->car)->obj()->type
                          == SExpr::symbol);
}

SExprp unwrapSyntax( SExprp args, Evaluator* eva)
{
    ensureList1<Syntax>( args, eva);
    SExprp arg =  as<Cons>( args)->car;
    Syntax* s = as<Syntax>( arg);
    if ( s->obj()->type == SExpr::symbol)
        return arg;
    if ( s->obj()->type == SExpr::cons)
    {
        const Cons* c = as<Cons>( s->obj());
        return Cons::make( Syntax::make( c->car, s->ctx()),
                           Syntax::make( c->cdr, s->ctx()));
    }
    if ( s->obj()->type == SExpr::vector)
    {
        const Vector* v = as<Vector>( s->obj());
        std::vector<SExprp> syntaxes;
        for ( std::vector<SExprp>::const_iterator i = v->val().begin();
              i != v->val().end(); ++i)
        {
            syntaxes.push_back( Syntax::make( *i, s->ctx()));
        }
        return Vector::make( syntaxes);
    }
    return s->obj();
}

SExprp areFreeIdentifiersEqual( SExprp args, Evaluator* eva)
{
    ensureList2<Syntax, Syntax>( args, eva);
    Syntax* a = as<Syntax>( nth<0>( args)->car);
    Syntax* b = as<Syntax>( nth<1>( args)->car);

    if ( a->obj()->type != SExpr::symbol)
        throw Exception( "It must be an identifier", a->obj(), eva->backtrace());
    if ( b->obj()->type != SExpr::symbol)
        throw Exception( "It must be an identifier", nth<1>( args)->car, eva->backtrace());

    const Activation* act_a = a->ctx()->getYongestContaining( as<Symbol>(a->obj())->getName());
    const Activation* act_b = b->ctx()->getYongestContaining( as<Symbol>(b->obj())->getName());

    return Boolean::make( act_a == act_b);
}
/*
SExprp areBoundIdentifiersEqual( SExprp args, Evaluator* eva)
{
    if ( !as<Boolean>( areFreeIdentifiersEqual( args, eva))->
}
*/

SExprp Gensym::operator()( SExprp args, Evaluator* eva)
{
    if ( args->type == SExpr::nil )
        return Symbol::make( strs_->gensym( ""));
    ensureOr( ensureList1<Symbol>,
              ensureList1<StrConst>, args, eva);
    if ( nth<0>( args)->car->type == SExpr::symbol )
        return Symbol::make( strs_->gensym( strs_->str
                           ( as<Symbol>( nth<0>( args)->car)->getName())));
    return Symbol::make( strs_->gensym
                         ( as<StrConst>( nth<0>( args)->car)->val()));
}

void registerStdForms( Context* ctx, StringTable* strs)
{
    ctx->regSpec( strs->id( "if"), &ifSpec);
    ctx->regSpec( strs->id( "begin"), &begin);
    ctx->regSpec( strs->id( "set!"), &set);
    ctx->regSpec( strs->id( "define"), &def);
    ctx->regSpec( strs->id( "quote"), &quote);
    ctx->regSpec( strs->id( "unquote"), &unQuote);
    ctx->regSpec( strs->id( "unquote-splicing"), &unQuoteSplicing);
    //quasiquote must be registered with QQuoteExpander
    ctx->regFunction( strs->id( "apply"), &apply);
    ctx->regFunction( strs->id( "cons"), &cons);
    ctx->regFunction( strs->id( "car"), &car);
    ctx->regFunction( strs->id( "cdr"), &cdr);
    ctx->regFunction( strs->id( "set-car!"), &setCar);
    ctx->regFunction( strs->id( "set-cdr!"), &setCdr);

    ctx->regFunction( strs->id( "null?"), &isType<SExpr::nil>);
    ctx->regFunction( strs->id( "pair?"), &isType<SExpr::cons>);
    ctx->regFunction( strs->id( "string?"), &isType<SExpr::string_const>);
    ctx->regFunction( strs->id( "vector?"), &isType<SExpr::vector>);
    ctx->regFunction( strs->id( "char?"), &isType<SExpr::character_const>);
    ctx->regFunction( strs->id( "symbol?"), &isType<SExpr::symbol>);
    ctx->regFunction( strs->id( "boolean?"), &isType<SExpr::boolean_const>);
    ctx->regFunction( strs->id( "number?"), &isType<SExpr::number_const>);
    ctx->regFunction( strs->id( "procedure?"), &isType<SystemObj::funid>);
    ctx->regFunction( strs->id( "port?"), &isType<SystemObj::port>);
    ctx->regFunction( strs->id( "eof-object?"), &isType<SystemObj::eof_val>);

    ctx->regFunction( strs->id( "eq?"), &eq);
    ctx->regFunction( strs->id( "eqv?"), &eq);
    //equal? can be implemented in scheme, using basic functions.

    ctx->regFunction( strs->id( "symbol->string"), &symbolToString);

    ctx->regSpec( strs->id( "syntax"), &syntax);
    ctx->regFunction( strs->id( "identifier?"), &isIdentifier);
    ctx->regFunction( strs->id( "free-identifier=?"), &areFreeIdentifiersEqual);
//    ctx->regFunction( strs->id( "bound-identifier=?"), &areBoundIdentifiersEqual);
    ctx->regFunction( strs->id( "unwrap-syntax"), &unwrapSyntax);

    registerIOFunctions( ctx, strs);
    registerStringFunctions( ctx, strs);
    registerVectorFunctions( ctx, strs);
    registerNumericFunctions( ctx, strs);

    ctx->regFunction( strs->id( "exit"), &ext);
    ctx->defVariable( strs->id( "quit"), ctx->getVariable( strs->id( "exit")));
}

}//namespace interpreter
