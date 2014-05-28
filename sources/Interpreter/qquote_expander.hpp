/**
 * @file:qquote_expander.hpp
 * A declaration of class, expanding quasiquote entries, like
 * `(a b c ,d . (,@(list k e) i))  -> (a b c (eval d) . ((eval k) (eval e) i))
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include "interpreter_dep.hpp"

namespace parser
{
    class StringTable;
}

namespace interpreter
{

const int MaximulQuasiQuoteNesting = 100;

namespace ast = parser::ast;
using ast::SExprp;
using parser::StringTable;

class Evaluator;

class QQuoteExpander : public ast::Visitor
{
public:
    QQuoteExpander( StringTable* strs, Evaluator* eva = 0);
    SExprp operator()( SExprp args, Evaluator* eva);

    SExprp expand( SExprp subj);
    virtual SExprp onNil( SExprp subj);
    virtual SExprp onCons( SExprp subj);
    virtual SExprp onSymbol( SExprp subj);
    virtual SExprp onVector( SExprp subj);
    virtual SExprp onNumber( SExprp subj);
    virtual SExprp onStrConst( SExprp subj);
    virtual SExprp onCharacter( SExprp subj);
    virtual SExprp onBoolean( SExprp subj);

private:
    SExprp processUnquote( SExprp body);
    SExprp processNestedQuasiQuote( SExprp body);
    SExprp processUnquoteSplicing( SExprp arg, SExprp tail);

    Evaluator* eva;
    int q_depth;

    int unquote;
    int quasiquote;   /* Uniq ids for the corresponding symbols. */
    int unquote_splicing;
};

} //namespace interpreter
