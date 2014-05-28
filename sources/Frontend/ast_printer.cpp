/**
 * @file:ast_printer.cpp
 * An implementation of AST-printing class.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <vector>
#include "ast_printer.hpp"

namespace parser
{

using namespace ast;

void AstPrinter::print( SExprp subj)
{
    subj.visit( this);
}
SExprp AstPrinter::onNil( SExprp subj)
{
    out <<"()";
    return subj;
}
SExprp AstPrinter::onCons( SExprp subj)
{
    out <<"( ";
    print( as<Cons>(subj)->car);
    out <<". ";
    print( as<Cons>(subj)->cdr);
    out <<")";
    return subj;
}
SExprp AstPrinter::onSymbol( SExprp subj)
{
    out << "sym[" << as<Symbol>(subj)->getName() <<"]";
    return subj;
}
SExprp AstPrinter::onVector( SExprp subj)
{
    out <<"#(";
    const std::vector<SExprp>& items = as<Vector>(subj)->val();
    for (std::vector<SExprp>::const_iterator i = items.begin(); i != items.end(); ++i)
    {
        print( *i);
        out <<" ";
    }
    out << ")";
    return subj;
}

SExprp AstPrinter::onNumber( SExprp subj)
{
    out <<"n[" << as<Number>(subj)->val() <<"]";
    return subj;
}
SExprp AstPrinter::onStrConst( SExprp subj)
{
    out <<'"' <<as<StrConst>(subj)->val() <<'"';
    return subj;
}
SExprp AstPrinter::onCharacter( SExprp subj)
{
    out <<"#\\" <<as<Character>(subj)->val();
    return subj;
}
SExprp AstPrinter::onBoolean( SExprp subj)
{
    if (as<Boolean>(subj)->val())
        out <<"#t";
    else
        out <<"#f";
    return subj;
}/*
SExprp AstPrinter::onFunId( SExprp subj)
{
    out <<"fun[" << as<FunId>(subj)->id() <<"]";
    return subj;
}
SExprp AstPrinter::onMacId( SExprp subj)
{
    out <<"mac[" << as<MacId>(subj)->id() <<"]";
    return subj;
}
SExprp AstPrinter::onSpecId( SExprp subj)
{
    out <<"spec[" << as<SpecId>(subj)->id() <<"]";
    return subj;
}
 */
} //namespace parser
