/**
 * @file:ast_printer.hpp
 * A declaration of class, printing S-expressions through std::ostream.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <iostream>
#include "ast.hpp"

namespace parser
{

using std::ostream;
using ast::SExprp;

class AstPrinter : public ast::Visitor
{
public:
    AstPrinter( ostream& out_):out(out_){}
    void print( SExprp subj);
    virtual SExprp onNil( SExprp subj);
    virtual SExprp onCons( SExprp subj);
    virtual SExprp onSymbol( SExprp subj);
    virtual SExprp onVector( SExprp subj);
    virtual SExprp onNumber( SExprp subj);
    virtual SExprp onStrConst( SExprp subj);
    virtual SExprp onCharacter( SExprp subj);
    virtual SExprp onBoolean( SExprp subj);

private:
    ostream& out;
};

} //namespace parser
