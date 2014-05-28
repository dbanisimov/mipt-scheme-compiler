/**
 * @file:est.cpp
 * A definition of some support methods of a set of classes
 * reprodusing Execution State Tree of Scheme language.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include "est.hpp"

namespace parser
{
namespace ast
{
    SET_CLASS_RUNTIME_TAG_N_NAME( interpreter::est::FunId,
                                  interpreter::est::SystemObj::funid, "function");
    SET_CLASS_RUNTIME_TAG_N_NAME( interpreter::est::MacId,
                                  interpreter::est::SystemObj::macid, "macros");
    SET_CLASS_RUNTIME_TAG_N_NAME( interpreter::est::SpecId,
                                  interpreter::est::SystemObj::specid, "special form");
    SET_CLASS_RUNTIME_TAG_N_NAME( interpreter::est::Port,
                                  interpreter::est::SystemObj::port, "port");
    SET_CLASS_RUNTIME_TAG_N_NAME( interpreter::est::DelayedCall,
                                  interpreter::est::SystemObj::delayed_call, "delayed call");
    SET_CLASS_RUNTIME_TAG_N_NAME( interpreter::est::EofVal,
                                  interpreter::est::SystemObj::eof_val, "EOF");
    SET_CLASS_RUNTIME_TAG_N_NAME( interpreter::est::Syntax,
                                  interpreter::est::SystemObj::syntax, "syntax");
}
}

namespace interpreter
{
namespace est
{

SExprp FunId::make( int id)
{
    return SExprp( new FunId( id));
}
SExprp MacId::make( int id)
{
    return SExprp( new MacId( id));
}
SExprp SpecId::make( int id)
{
    return SExprp( new SpecId( id));
}
SExprp Port::make( Istreamp in, Ostreamp out)
{
    return SExprp( new Port( in, out));
}
SExprp DelayedCall::make( int id, SExprp args, string name)
{
    return SExprp( new DelayedCall( id, args, name));
}
SExprp EofVal::make()
{
    return SExprp( new EofVal);
}
SExprp Syntax::make( SExprp obj, Activationp ctx)
{
    return SExprp( new Syntax( obj, ctx));
}

SExprp SystemObj::visit( parser::ast::Visitor* v, SExprp me)
{
    Visitor* vi = dynamic_cast<Visitor*>(v);
    assert( vi != 0);
    return visit( vi, me);
}

}//namespace est
}//namespace interpreter
