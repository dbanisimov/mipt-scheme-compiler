/**
 * @file:activation.cpp
 * Defenition of methods of the class, storing activation record for function.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "activation.hpp"
#include "interpreter_dep.hpp"
#include "interpreter_exception.hpp"

namespace interpreter
{

using namespace parser::ast;

Activation::Activation( Activationp parent_arg)
    :parent_( parent_arg)
{
}

void Activation::setSymbol( int name, SExprp val)
{
    if ( find( name) == end() )
    {
        assert( parent_.get() != 0);
        parent_->setSymbol( name, val);
    }
    else
        (*this)[name] = val;
}

void Activation::defSymbol( int name, SExprp val)
{
    (*this)[name] = val;
}

SExprp Activation::getSymbol( int name) const
{
    if ( find( name) == end() )
        if ( parent_.get() != 0 )
            return parent_->getSymbol( name);
        else
            throw Exception( "var is unbound", Symbol::make( name));
    return find( name)->second;
}

const Activation* Activation::getYongestContaining( int name) const
{
    if ( find( name) == end() )
        if ( parent_.get() != 0 )
            return parent_->getYongestContaining( name);
        else
            return 0;
    return this;
}

bool Activation::exists( int name) const
{
    if ( find( name) == end() )
        if ( parent_.get() != 0 )
            return parent_->exists( name);
        else
            return false;
    return true;
}

Activationp Activation::parent() const
{
    return parent_;
}

} //namespace interpreter
