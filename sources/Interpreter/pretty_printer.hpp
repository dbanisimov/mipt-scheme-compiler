/**
 * @file:pretty_prtinter.hpp
 * A declaration of class, printing S-expressions through std::ostream
 * in human readable, and beautiful form.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <iostream>
#include "est.hpp"

namespace parser
{
    class StringTable;
}

namespace interpreter
{

using std::ostream;
using est::SExprp;
using parser::StringTable;

class PrettyPrinter : public est::Visitor
{
public:
    PrettyPrinter( ostream& out_, StringTable* strs_, bool print_inside_info_ = false)
        :out(out_), strs(strs_), print_inside_info( print_inside_info_) {}
    void print( SExprp subj);
    virtual SExprp onNil( SExprp subj);
    virtual SExprp onCons( SExprp subj);
    virtual SExprp onSymbol( SExprp subj);
    virtual SExprp onVector( SExprp subj);
    virtual SExprp onNumber( SExprp subj);
    virtual SExprp onStrConst( SExprp subj);
    virtual SExprp onCharacter( SExprp subj);
    virtual SExprp onBoolean( SExprp subj);
    virtual SExprp onFunId( SExprp subj);
    virtual SExprp onMacId( SExprp subj);
    virtual SExprp onSpecId( SExprp subj);
    virtual SExprp onPort( SExprp subj);
    virtual SExprp onDelayedCall( SExprp subj);
    virtual SExprp onEofVal( SExprp subj);
    virtual SExprp onSyntax( SExprp subj);

private:
    ostream& out;
    StringTable* strs;

    bool print_inside_info;
};

} //namespace parser
