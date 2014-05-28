/**
 * @file:pretty_printer.cpp
 * An implementation of a human readable, beautiful AST-printing class.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <vector>
#include "pretty_printer.hpp"
#include "interpreter_dep.hpp"

namespace interpreter
{

using namespace est;

void PrettyPrinter::print( SExprp subj)
{
    subj.visit( this);
}
SExprp PrettyPrinter::onNil( SExprp subj)
{
    out <<"()";
    return subj;
}
SExprp PrettyPrinter::onCons( SExprp subj)
{
    out <<"(";
    while ( as<Cons>(subj)->cdr->type == SExpr::cons)
    {
        print( as<Cons>(subj)->car);
        subj = as<Cons>(subj)->cdr;
        out <<" ";
    }
    print( as<Cons>(subj)->car);
    if ( as<Cons>(subj)->cdr->type != SExpr::nil)
    {
        out <<" . ";
        print( as<Cons>(subj)->cdr);
    }
    out <<")";
    return subj;
}
SExprp PrettyPrinter::onSymbol( SExprp subj)
{
    out << strs->str( as<Symbol>(subj)->getName());
    return subj;
}
SExprp PrettyPrinter::onVector( SExprp subj)
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

SExprp PrettyPrinter::onNumber( SExprp subj)
{
    out << as<Number>(subj)->val();
    return subj;
}
SExprp PrettyPrinter::onStrConst( SExprp subj)
{
    out <<'"' <<as<StrConst>(subj)->val() <<'"';
    return subj;
}
SExprp PrettyPrinter::onCharacter( SExprp subj)
{
    out <<"#\\" <<as<Character>(subj)->val();
    return subj;
}
SExprp PrettyPrinter::onBoolean( SExprp subj)
{
    if (as<Boolean>(subj)->val())
        out <<"#t";
    else
        out <<"#f";
    return subj;
}
SExprp PrettyPrinter::onFunId( SExprp subj)
{
    out <<"#<function ";
    if (print_inside_info)
        out << as<FunId>(subj)->id();
    out <<">";
    return subj;
}
SExprp PrettyPrinter::onMacId( SExprp subj)
{
    out <<"#<macros ";
    if (print_inside_info)
        out << as<MacId>(subj)->id();
    out <<">";
    return subj;
}
SExprp PrettyPrinter::onSpecId( SExprp subj)
{
    out <<"#<special form ";
    if (print_inside_info)
        out << as<SpecId>(subj)->id();
    out <<">";
    return subj;
}
SExprp PrettyPrinter::onPort( SExprp subj)
{
    out <<"#<port>";
    return subj;
}
SExprp PrettyPrinter::onDelayedCall( SExprp subj)
{
    out <<"#<delayed call>";
    return subj;
}
SExprp PrettyPrinter::onEofVal( SExprp subj)
{
    out <<"#<EOF>";
    return subj;
}
SExprp PrettyPrinter::onSyntax( SExprp subj)
{
    out <<"#<Syntax ";
    print( as<Syntax>( subj)->obj());
    out <<">";
    return subj;
}

} //namespace interpreter
