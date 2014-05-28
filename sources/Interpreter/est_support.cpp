/**
 * @file:ast_support.cpp
 * A definition of a variety of convinient functions for easy AST manipulating
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "est_support.hpp"
#include "interpreter_exception.hpp"

namespace interpreter
{
namespace est
{

SExprp copyList( SExprp list)
{
    if ( list->type != SExpr::cons)
        return Nil::make();
    return Cons::make( as<Cons>(list)->car, //TODO: rewrite it in an
                       copyList( as<Cons>(list)->cdr)); // iterative fashion
}

SExprp append( SExprp list, SExprp tail)
{
    SExprp prev = list;
    if ( prev->type == SExpr::nil)
        return tail;
    if ( prev->type != SExpr::cons)
        throw Exception( "trying to append to a non list", prev);
    SExprp current = as<Cons>(prev)->cdr;
    while( current->type != SExpr::nil)
    {
        if ( prev->type != SExpr::cons)
            throw Exception( "trying to append to a non list", prev);
        prev = current;
        current = as<Cons>(current)->cdr;
    }
    as<Cons>(prev)->cdr = tail;
    return list;
}

SExprp reverse( SExprp list, SExprp tail)
{
    if (list->type == SExpr::nil) return tail;

    assert( list->type== SExpr::cons);
    return reverse( as<Cons>(list)->cdr, Cons::make( as<Cons>(list)->car, tail));
}

int listLength( SExprp list)
{
    int len = 0;
    SExprp current = list;
    while( current->type == SExpr::cons)
    {
        current = as<Cons>(current)->cdr;
//        assert( current != list);//TODO: provide acyclic list
        ++len;
    }
    assert( current->type == SExpr::nil);
    return len;
}

/* Many parts, which are implementation specified, implemented
 * as they should be implemented in eqv for simplicity*/
bool areEq( SExprp one, SExprp another)
{
    if ( one.get() == another.get())
        return true;

    if (one->type != another->type)
        return false;

    switch( one->type)
    {
    case SExpr::nil:
        return true;
    case SExpr::cons:
        return false;
    case SExpr::symbol:
        return as<Symbol>(one)->getName() == as<Symbol>(another)->getName();
    case SExpr::vector:
        return as<Vector>(one)->val().size() == 0
            && as<Vector>(another)->val().size() == 0;
    case SExpr::string_const:
        return as<StrConst>(one)->val().size() == 0
            && as<StrConst>(another)->val().size() == 0;
    case SExpr::number_const:
        return as<Number>(one)->val() == as<Number>(another)->val();
    case SExpr::character_const:
        return as<Character>(one)->val() == as<Character>(another)->val();
    case SExpr::boolean_const:
        return as<Boolean>(one)->val() == as<Boolean>(another)->val();
    case SystemObj::funid:
        return as<FunId>(one)->id() == as<FunId>(another)->id();
    case SystemObj::macid:
        return as<MacId>(one)->id() == as<MacId>(another)->id();
    case SystemObj::specid:
        return as<SpecId>(one)->id() == as<SpecId>(another)->id();
    case SystemObj::port:
        return as<Port>(one)->in() == as<Port>(another)->in()
            && as<Port>(one)->out() == as<Port>(another)->out();
    case SystemObj::eof_val:
        return true;
    default:
        return false;
    };
}

bool areEqv( SExprp one, SExprp another)
{
    if ( areEq( one, another))
        return true;
    return false;//here can be some additional comparings
}

}//namespace est
}//namespace interpreter
