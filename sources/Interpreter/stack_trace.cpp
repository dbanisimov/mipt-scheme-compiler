/**
 * @file:evaluator.cpp
 * An implementation of class, responding for evaluation any S-expression, in 
 * given context. It follows for Visitor pattern, visiting every node in AST.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <iostream>
#include "stack_trace.hpp"
#include "interpreter_exception.hpp"
#include "pretty_printer.hpp"

namespace interpreter
{

using namespace est;

StackEntry::StackEntry( callType::t type_, SExprp args_, std::string name_)
    :type( type_), args( args_), name( name_)
{
}

StackEntry& StackEntry::operator= ( const StackEntry& other)
{
    name = other.name;
    type = other.type;
    args = other.args;
}

void StackEntry::print( std::ostream& out, parser::StringTable* strs) const
{
    out <<"in ";
    switch (type)
    {
    case callType::fun:
        out << "execution of function ";
        break;
    case callType::mac:
        out << "expansion of macros ";
        break;
    case callType::spec:
        out << "processing special-form ";
        break;
    };
    out <<name;
    out <<" with arguments: ";
    PrettyPrinter p( out, strs, false);
    p.print( args);
}

void StackTrace::enterCall( callType::t type, SExprp args, std::string name)
{
    push_back( StackEntry( type, args, name));
}

void StackTrace::leaveCall()
{
    pop_back();
}

void StackTrace::print( std::ostream& out, parser::StringTable* strs) const
{
    for (int i = 0; i < size(); ++i)
    {
        at( i).print( out, strs);
        out <<std::endl;
    }
}

InCallAuto::InCallAuto( callType::t type, SExprp args,
                        StackTrace* trace_, std::string name)
    :trace( trace_)
{
    trace->enterCall( type, args, name);
}

InCallAuto::~InCallAuto()
{
    trace->leaveCall();
}

}//namespace interpreter
