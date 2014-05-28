/**
 * @file:est.hpp
 * A declaration of a set of classes reprodusing Execution State Tree
 * of Scheme language, used by Interpreter as representation of 
 * a program.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include "interpreter_dep.hpp"
#include "activation.hpp"
#include <iostream>

namespace interpreter
{
namespace est
{

using namespace parser::ast;

typedef boost::shared_ptr<std::istream> Istreamp;
typedef boost::shared_ptr<std::ostream> Ostreamp;

class Visitor :public parser::ast::Visitor
{
public:
    virtual SExprp onFunId( SExprp subj) = 0;
    virtual SExprp onMacId( SExprp subj) = 0;
    virtual SExprp onSpecId( SExprp subj) = 0;
    virtual SExprp onPort( SExprp subj) = 0;
    virtual SExprp onDelayedCall( SExprp subj) = 0;
    virtual SExprp onEofVal( SExprp subj) = 0;
    virtual SExprp onSyntax( SExprp subj) = 0;
};

#undef VISITING
#define VISITING( funName) \
    SExprp visit( Visitor* v, SExprp me) \
    { return v->funName( me);}

class SystemObj :public Atom
{
public:
    enum DynamicType
    {
        funid = SExpr::types_num,
        macid,
        specid,
        port,
        delayed_call,
        eof_val,
        syntax
    };

public:
    SystemObj( Type t) :Atom( t) {}

    SExprp visit( parser::ast::Visitor *v, SExprp me);
    virtual SExprp visit( Visitor *v, SExprp me) = 0;
};

class Handle :public SystemObj
{
public:
    Handle( int val_, Type t) :val( val_), SystemObj( t){}
    int id() const { return val; }
private:
    int val;
};

class FunId :public Handle
{
public:
    FunId( int val_) :Handle( val_, funid) {}
    VISITING( onFunId);

    static SExprp make( int id);
};

class MacId :public Handle
{
public:
    MacId( int val_) :Handle( val_, macid){}
    VISITING( onMacId);

    static SExprp make( int id);
};

class SpecId :public Handle
{
public:
    SpecId( int val_) :Handle( val_, specid) {}
    VISITING( onSpecId);

    static SExprp make( int id);
};

class Port :public SystemObj
{
public:
    Port( Istreamp in, Ostreamp out) :ost( out), ist( in), SystemObj( port) {}
    VISITING( onPort);
    Istreamp in() { return ist; }
    Ostreamp out() { return ost; }

    static SExprp make( Istreamp in, Ostreamp out);
private:
    Istreamp ist;
    Ostreamp ost;
};

class DelayedCall :public SystemObj
{
public:
    DelayedCall( int id, SExprp args, string name)
        :SystemObj( delayed_call), id_( id), args_( args), name_( name) {}
    VISITING( onDelayedCall);

    static SExprp make( int id, SExprp args, string name);

    int id() const { return id_; }
    SExprp args() const { return args_; }
    string name() const { return name_; }
private:
    int id_;
    SExprp args_;
    string name_;
};

class EofVal :public SystemObj
{
public:
    EofVal() :SystemObj( eof_val) {}
    VISITING( onEofVal);

    static SExprp make();
};

class Syntax :public SystemObj
{
public:
    Syntax( SExprp obj, Activationp ctx) :SystemObj( syntax), obj_(obj), ctx_(ctx) {}
    VISITING( onSyntax);

    static SExprp make( SExprp obj, Activationp ctx);
    const SExprp& obj() const { return obj_; }
    const Activationp& ctx() const { return ctx_; }
    
private:
    SExprp obj_;
    Activationp ctx_;
};


}//namespace est
}//namespace interpreter
