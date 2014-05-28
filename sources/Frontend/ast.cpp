/**
 * @file:ast.cpp
 * A definition of some support methods of a set of classes
 * reprodusing Abstract Symbol Tree of Scheme language.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "ast.hpp"
#include "string_table.hpp"

namespace parser
{
namespace ast
{

SET_CLASS_RUNTIME_TAG_N_NAME( Nil, SExpr::nil, "nil");
SET_CLASS_RUNTIME_TAG_N_NAME( Cons, SExpr::cons, "cons");
SET_CLASS_RUNTIME_TAG_N_NAME( Symbol, SExpr::symbol, "symbol");
SET_CLASS_RUNTIME_TAG_N_NAME( Vector, SExpr::vector, "vector");
SET_CLASS_RUNTIME_TAG_N_NAME( StrConst, SExpr::string_const, "string");
SET_CLASS_RUNTIME_TAG_N_NAME( Boolean, SExpr::boolean_const, "boolean");
SET_CLASS_RUNTIME_TAG_N_NAME( Character, SExpr::character_const, "character");
SET_CLASS_RUNTIME_TAG_N_NAME( Number, SExpr::number_const, "number");

SExprp SExprp::visit (Visitor* v)
{
    return get()->visit( v, *this);
}

SExprp Nil::make()
{
    return SExprp( new Nil);
}

SExprp Cons::make( SExprp head, SExprp tail)
{
    return SExprp( new Cons( head, tail));
}

SExprp Vector::make( const std::vector<SExprp>& cont)
{
    return SExprp( new Vector( cont));
}
SExprp Number::make( long double num)
{
    return SExprp( new Number( num));
}
SExprp Character::make( char ch)
{
    return SExprp( new Character( ch));
}
SExprp StrConst::make( const string& s)
{
    return SExprp( new StrConst( s));
}
SExprp Symbol::make( const string& name, StringTable* table)
{
    return SExprp( new Symbol( table->id( name)));
}
SExprp Symbol::make( int name)
{
    return SExprp( new Symbol( name));
}
SExprp Boolean::make( bool b)
{
    return SExprp( new Boolean( b));
}

} //namespace ast
} //namespace parser
