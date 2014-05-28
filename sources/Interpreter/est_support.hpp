/**
 * @file:ast_support.hpp
 * A declaration of a variety of convinient functions for easy AST manipulating
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once


#include "est.hpp"

namespace interpreter
{
namespace est
{

template< int n>
inline Cons* nth( SExprp list)
{
    return nth<n-1>(as<Cons>(list)->cdr);
}

template<>
inline Cons* nth<0>( SExprp list)
{
    return as<Cons>(list);
}

SExprp copyList( SExprp list);
SExprp append( SExprp list, SExprp tail);
SExprp reverse( SExprp list, SExprp tail = Nil::make());
int listLength( SExprp list);

bool areEq( SExprp one, SExprp another);
bool areEqv( SExprp one, SExprp another);

}//namespace est
}//namespace interpreter
