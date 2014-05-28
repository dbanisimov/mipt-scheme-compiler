/**
 * @file:evaluator.hpp
 * A declaration of class, responding for evaluation any S-expression, in 
 * given context.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>
#include "est.hpp"
#include "context.hpp"
#include "stack_trace.hpp"

namespace interpreter
{

using est::SExprp;

class Evaluator :public parser::ast::Visitor
{
public:
    Evaluator( Context* c) :ctx(c) {}
    SExprp eval( SExprp root, bool tail);
    virtual SExprp onNil( SExprp subj);
    virtual SExprp onCons( SExprp subj);
    virtual SExprp onSymbol( SExprp subj);
    virtual SExprp onVector( SExprp subj);
    virtual SExprp onNumber( SExprp subj);
    virtual SExprp onStrConst( SExprp subj);
    virtual SExprp onCharacter( SExprp subj);
    virtual SExprp onBoolean( SExprp subj);

    std::string tryGetName( SExprp exp);
    SExprp funcall( int id, SExprp args, std::string name);

    Context* context() const;
    const StackTrace& backtrace() const;
private:
    Context* ctx;
    StackTrace trace;

    SExprp delayCall( int id, SExprp args, std::string name);
    SExprp macroexpand( int id, SExprp args, std::string name);
    SExprp specprocess( int id, SExprp args, std::string name);

    SExprp evalDelay( SExprp delay);
};

}// namespace interpreter
