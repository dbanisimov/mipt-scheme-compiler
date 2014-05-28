/**
 * @file:string_std_functions.cpp
 * Implementation of the most basic string and character functions
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "string_std_functions.hpp"
#include "interpreter_dep.hpp"
#include "exp_checker.hpp"
#include "context.hpp"
#include "evaluator.hpp"
#include "est_support.hpp"

namespace interpreter
{

SExprp charEq( SExprp args, Evaluator* eva)
{
    ensureList2<Character, Character>( args, eva);
    return Boolean::make( as<Character>( as<Cons>(args)->car)->val()
                          == as<Character>( nth<1>(args)->car)->val());
}

SExprp makeString( SExprp args, Evaluator* eva)
{
    ensureOr ( ensureList2<Number, Character>,
               ensureList1<Number>, args, eva);
    int length = as<Number>( as<Cons>( args)->car)->val();
    char c = 0;
    if ( as<Cons>( args)->cdr->type != SExpr::nil)
        c = as<Character>( nth<1>( args)->car)->val();
    return StrConst::make( string( length, c));
}

SExprp stringFun( SExprp args, Evaluator* eva)
{
    string result;
    while ( args->type != SExpr::nil)
    {
        ensureCons< Character, SExpr>( args, eva);
        result += as<Character>( as<Cons>( args)->car)->val();
        args = as<Cons>( args)->cdr;
    }
    return StrConst::make( result);
}

SExprp stringLength( SExprp args, Evaluator* eva)
{
    ensureList1<StrConst>( args, eva);
    return Number::make( as<StrConst>( as<Cons>( args)->car)->val().length());
}

SExprp stringRef( SExprp args, Evaluator* eva)
{
    ensureList2<StrConst, Number>( args, eva);
    string& str = as<StrConst>( as<Cons>( args)->car)->val();
    int pos = as<Number>( nth<1>( args)->car)->val();
    if ( pos >= str.length() || 0 > pos)
        throw Exception( "index must be inside the string", args);
    return Character::make( str[pos]);
}

SExprp stringSet( SExprp args, Evaluator* eva)
{
    ensureList3<StrConst, Number, Character>( args, eva);
    StrConst* str = as<StrConst>( as<Cons>( args)->car);
    int pos = as<Number>( nth<1>( args)->car)->val();
    if ( pos >= str->val().length() || 0 > pos)
        throw Exception( "index must be inside the string", args);
    char c = as<Character>( nth<2>( args)->car)->val();
    str->val()[pos] = c;
    return  as<Cons>( args)->car;
}

SExprp stringAppend( SExprp strs, Evaluator* eva)
{
    string rez = "";
    while ( strs->type != SExpr::nil)
    {
        ensureCons<StrConst, SExpr>( strs, eva);
        rez += as<StrConst>( as<Cons>( strs)->car)->val();
        strs = as<Cons>( strs)->cdr;
    }
    return StrConst::make( rez);
}

SExprp stringFill( SExprp args, Evaluator* eva)
{
    ensureList2<StrConst, Character>( args, eva);
    char c = as<Character>( nth<1>( args)->car)->val();
    string& str = as<StrConst>( as<Cons>( args)->car)->val();
    str = string( str.length(), c);
    return as<Cons>( args)->car;
}

void registerStringFunctions( Context* ctx, StringTable* strs)
{
    ctx->regFunction( strs->id( "char=?"), &charEq);
    ctx->regFunction( strs->id( "make-string"), &makeString);
    ctx->regFunction( strs->id( "string"), &stringFun);
    ctx->regFunction( strs->id( "string-ref"), &stringRef);
    ctx->regFunction( strs->id( "string-set!"), &stringSet);
    ctx->regFunction( strs->id( "string-length"), &stringLength);
    ctx->regFunction( strs->id( "string-append"), &stringAppend);
    ctx->regFunction( strs->id( "string-fill!"), &stringFill);
}

}//namespace interpreter
