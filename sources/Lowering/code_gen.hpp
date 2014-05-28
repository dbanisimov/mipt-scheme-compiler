/**
 * File: code_gen.hpp
 * Copyright (C) 2012 Denis Anisimov
 */

#pragma once

#include <boost/format.hpp>
#include "lowering_impl.hpp"

namespace lowering
{

using namespace parser;
using ast::SExprp; 
using namespace IR;
using namespace Opt;

typedef Module< OptMdes> OptModule;
typedef Function< OptMdes> OptFunction;
typedef Operation< OptMdes> OptOper;
typedef Object< OptMdes> OptObj;
typedef enum OptOperName OperName_t;

/**
 * Intermediate representation code generator class
 */
class CodeGen: public ast::Visitor 
{
public:
    CodeGen( StringTable* strt);
    //~CodeGen();
    void gen( SExprp subj);
    void print();
    virtual SExprp onNil( SExprp subj);
    virtual SExprp onCons( SExprp subj);
    virtual SExprp onSymbol( SExprp subj);
    virtual SExprp onVector( SExprp subj);
    virtual SExprp onNumber( SExprp subj);
    virtual SExprp onStrConst( SExprp subj);
    virtual SExprp onCharacter( SExprp subj);
    virtual SExprp onBoolean( SExprp subj);
private: 
    StringTable* st;
    OptModule module;
    OptFunction* cur_func;
    OptOper* cur_oper;
    Envp outer_env;
    Envp cur_env;
    OptObj* return_reg;
    OptObj* cont_reg;
    OptObj* env_reg;

    void doMalloc();
    void doLambda( SExprp lambda);
    void doDefine( SExprp define);
    void doLet( SExprp let);
    void doFunctionCall( SExprp func);
    void doList( SExprp list);

    std::string getSymStr( SExprp subj);

    bool isLambda( SExprp subj);
    bool isDefine( SExprp subj);
    bool isLet( SExprp subj);
    bool isLetSeq( SExprp subj);
    bool isLetRec( SExprp subj);

    void doFuncNumericWithOper( OperName_t oper_name, string func_name);
    #include "std_func_mac_inc.hpp"
};

} //namespace lowering
