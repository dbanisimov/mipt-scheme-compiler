/**
 * @file:stack_trace.hpp
 * A support class, being used for storing execution stack trace,
 * and print it.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>
#include <vector>
#include "est.hpp"

namespace parser
{
    class StringTable;
};

namespace interpreter
{

using est::SExprp;

namespace callType
{
    enum t
    {
        fun,
        mac,
        spec
    };
}


class StackEntry
{
    std::string name;
    callType::t type;
    SExprp args;

public:
    StackEntry( callType::t type, SExprp args, std::string name);
    StackEntry( const StackEntry& other)
        :name( other.name), type( other.type), args( other.args) {}
    
    StackEntry& operator= ( const StackEntry& other);

    void print( std::ostream& out, parser::StringTable* strs) const;
};

class StackTrace :private std::vector<StackEntry>
{
public:
    void enterCall( callType::t type, SExprp args, std::string name = "<?>");
    void leaveCall();
    
    void print( std::ostream& out, parser::StringTable* strs) const;
};

class InCallAuto
{
    StackTrace* trace;
public:
    InCallAuto( callType::t type, SExprp args,
                StackTrace* trace, std::string name = "<?>");
    ~InCallAuto();
};

}// namespace interpreter
