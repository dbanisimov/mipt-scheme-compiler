/**
 * @file:activation.hpp
 * Declaration of the class, storing activation record for function.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <map>
#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include "interpreter_dep.hpp"

namespace interpreter
{

using std::map;
using parser::ast::SExprp;

class Activation;

typedef boost::shared_ptr< Activation> Activationp;

class Activation :public map<int, SExprp>
{
public:
    Activation( Activationp parent = Activationp());
    void setSymbol( int name, SExprp val);
    void defSymbol( int name, SExprp val);
    SExprp getSymbol( int name) const;
    bool exists( int name) const;
    const Activation* getYongestContaining( int name) const;

    Activationp parent() const;
private:
    Activationp parent_;
};

}
