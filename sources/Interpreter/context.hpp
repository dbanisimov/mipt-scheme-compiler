/**
 * @file:context.hpp
 * A declaration of Context class, storing all circumstances,
 * the interpretation running in.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <boost/function.hpp>
#include <map>
#include "est.hpp"
#include "activation.hpp"
#include "interpreter_dep.hpp"

namespace parser
{
    class StringTable;
}

namespace interpreter
{

using boost::function;
using std::map;
using interpreter::est::SExprp;
using parser::StringTable;

class Evaluator;

typedef function<SExprp (SExprp args, Evaluator* eva)> fun;
typedef function<SExprp (SExprp args, Evaluator* eva)> spec_form;

class Context
{
public:
    Context( StringTable* strs);
    ~Context();

    void enterScope();
    void exitScope();

    Activationp activation();
    void setActivation( Activationp act);

    StringTable* getStrings() { return strs;}

    void setVariable( int name, SExprp val)
        { vars->setSymbol( name, val); }
    void defVariable( int name, SExprp val)
        { vars->defSymbol( name, val); }
    SExprp getVariable( int name);
    SExprp funcall( int id, SExprp args, Evaluator* eva);
    int regFunction( fun val);
    void regFunction( int name, fun val);
    SExprp macroexpand( int id, SExprp args, Evaluator* eva);
    int regMacro( fun val);
    void regMacro( int name, fun val);

    SExprp applySpec( int id, SExprp args, Evaluator* eva);
    int regSpec( spec_form spec);
    void regSpec( int name, spec_form spec);

    bool knownMacro( int id)
        { return macroses.find( id) != macroses.end(); }
    bool knownFun( int id)
        { return functions.find( id) != functions.end(); }
    bool knownVar( int name)
        { return vars->exists( name); }
    bool knownSpec( int id)
        { return specs.find( id) != specs.end(); }

    int generateUniqId()
        { return nextUniqId++; }

    SExprp current_input;
    SExprp current_output;
private:
    Activationp vars;
    map<int, fun> functions;
    map<int, fun> macroses;
    map<int, spec_form> specs;

    StringTable* strs;

    int nextUniqId;
};

}
