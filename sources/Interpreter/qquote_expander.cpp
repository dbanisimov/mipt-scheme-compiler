/**
 * @file:qquote_expander.hpp
 * A definition of the class, expanding quasiquote expressions, like
 * `(a b c ,d . (,@(list k e) i))  -> (a b c (eval d) . ((eval k) (eval e) i))
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "qquote_expander.hpp"
#include "evaluator.hpp"
#include "est_support.hpp"
#include "exp_checker.hpp"

namespace interpreter
{

using namespace est;
using parser::StringTable;

QQuoteExpander::QQuoteExpander( StringTable* strs, Evaluator* eva_) :eva( eva_)
{
    unquote = strs->id( "unquote");
    quasiquote = strs->id( "quasiquote");
    unquote_splicing = strs->id( "unquote-splicing");
}
SExprp QQuoteExpander::operator()( SExprp args, Evaluator* eva_)
{
    eva = eva_;
    assert( args->type == SExpr::cons);
    assert( as<Cons>(args)->cdr->type == SExpr::nil);
    q_depth = 0;
    return expand( as<Cons>(args)->car);
}

SExprp QQuoteExpander::expand( SExprp subj)
{
    return subj.visit( this);
}

SExprp QQuoteExpander::onNil( SExprp subj)
{
    return subj;
}

SExprp QQuoteExpander::onCons( SExprp subj)
{
    SExprp head = as<Cons>(subj)->car;
    SExprp body = as<Cons>(subj)->cdr;
    if ( head->type == SExpr::symbol)
    {
        int name = as<Symbol>(head)->getName();
        if ( name == unquote)
        {
            return processUnquote( body);
        }
        else if ( name == quasiquote)
        {
            return processNestedQuasiQuote( body);
        }
        assert( name != unquote_splicing); //this case must be caught upper
    }
    
    if ( head->type == SExpr::cons 
         && as<Cons>(head)->car->type == SExpr::symbol
         && as<Symbol>(as<Cons>(head)->car)->getName() == unquote_splicing )
    {
        return processUnquoteSplicing( as<Cons>(head)->cdr, body);
    }
    return Cons::make( expand( head), expand( body));
}

SExprp QQuoteExpander::processUnquote( SExprp body)
{
    ensureList1<SExpr>( body, eva);
    if ( q_depth == 0)
    {
        return eva->eval( as<Cons>(body)->car, false);
    }
    q_depth--;
    SExprp rez = Cons::make( Symbol::make( unquote),
                             expand( body));
    q_depth++;
    return rez;
}

SExprp QQuoteExpander::processNestedQuasiQuote( SExprp body)
{
    q_depth++;
    if ( q_depth > MaximulQuasiQuoteNesting)
        throw Exception( "maximul nesting level of quasi quotation",
                         body, eva->backtrace());
    SExprp rez = Cons::make( Symbol::make( quasiquote),
                             expand( body));
    q_depth--;
    return rez;
}

SExprp QQuoteExpander::processUnquoteSplicing( SExprp arg, SExprp tail)
{
    if ( q_depth == 0)
    {
        ensureList1<SExpr>( arg, eva);
        SExprp list = eva->eval( as<Cons>(arg)->car, false);
        ensureList( list, eva);
        return append( copyList( list), expand( tail));
    }
    q_depth--;
    SExprp rez = Cons::make( Cons::make( Symbol::make( unquote_splicing),
                                         Nil::make()),
                             expand( tail));
    q_depth++;
    return rez;
}

SExprp QQuoteExpander::onSymbol( SExprp subj)
{
    return subj;
}

SExprp QQuoteExpander::onVector( SExprp subj)
{
    return subj;
}

SExprp QQuoteExpander::onNumber( SExprp subj)
{
    return subj;
}

SExprp QQuoteExpander::onStrConst( SExprp subj)
{
    return subj;
}

SExprp QQuoteExpander::onCharacter( SExprp subj)
{
    return subj;
}

SExprp QQuoteExpander::onBoolean( SExprp subj)
{
    return subj;
}

}//namespace interpreter
