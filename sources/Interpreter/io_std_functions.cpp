/**
 * @file:io_std_functions.cpp
 * A set of IO functions to work with ports in scheme.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <fstream>
#include "interpreter_dep.hpp"
#include "io_std_functions.hpp"
#include "exp_checker.hpp"
#include "context.hpp"
#include "evaluator.hpp"
#include "est_support.hpp"

namespace interpreter
{

using std::ofstream;
using std::ifstream;

SExprp outputPort( SExprp args, Evaluator* eva)
{
    ensureList1<Port>( args, eva);
    SExprp arg = as<Cons>(args)->car;
    return Boolean::make( as<Port>(arg)->out().get() != 0);
}

SExprp inputPort( SExprp args, Evaluator* eva)
{
    ensureList1<Port>( args, eva);
    SExprp arg = as<Cons>(args)->car;
    return Boolean::make( as<Port>(arg)->in().get() != 0);
}

SExprp currentInputPort( SExprp args, Evaluator* eva)
{
    ensure<Nil>( args, eva);
    return eva->context()->current_input;
}

SExprp currentOutputPort( SExprp args, Evaluator* eva)
{
    ensure<Nil>( args, eva);
    return eva->context()->current_output;
}

SExprp readChar( SExprp args, Evaluator* eva)
{
    ensureOr( ensureList1<Port>, ensure<Nil>, args, eva);
    SExprp iport = eva->context()->current_input;
    if ( args->type != SExpr::nil)
        iport = as<Cons>(args)->car;
    Istreamp in = as<Port>(iport)->in();
    assert( in.get() != 0);

    char c = (*in).get();

    if ((*in).eof())
        return EofVal::make();
    return Character::make( c);
}

SExprp peekChar( SExprp args, Evaluator* eva)
{
    ensureOr( ensureList1<Port>, ensure<Nil>, args, eva);
    SExprp iport = eva->context()->current_input;
    if ( args->type != SExpr::nil)
        iport = as<Cons>(args)->car;
    Istreamp in = as<Port>(iport)->in();
    assert( in.get() != 0);

    char c = (*in).peek();

    if ((*in).eof())
        return EofVal::make();
    return Character::make( c);
}

SExprp isCharReady( SExprp args, Evaluator* eva)
{
    ensureOr( ensureList1<Port>, ensure<Nil>, args, eva);
    SExprp iport = eva->context()->current_input;
    if ( args->type != SExpr::nil)
        iport = as<Cons>(args)->car;
    Istreamp in = as<Port>(iport)->in();
    assert( in.get() != 0);

    bool ready = in->rdbuf()->in_avail() != 0;
    return Boolean::make( ready);
}

SExprp writeChar( SExprp args, Evaluator* eva)
{
    ensureOr( ensureList1<Character>, ensureList2<Character, Port>,
              args, eva);
    SExprp oport = eva->context()->current_output;
    SExprp character = as<Cons>(args)->car;
    if ( as<Cons>(args)->cdr->type != SExpr::nil)
        oport = nth<1>(args)->car;
    Ostreamp out = as<Port>(oport)->out();
    assert( out.get() != 0);
    out->put( as<Character>( character)->val());
    return character;
}

SExprp newline( SExprp args, Evaluator* eva)
{
    ensureOr( ensureList1<Port>, ensure<Nil>, args, eva);
    SExprp oport = eva->context()->current_output;
    if ( args->type != SExpr::nil)
        oport = as<Cons>(args)->car;
    Ostreamp out = as<Port>(oport)->out();
    assert( out.get() != 0);
    out->put( '\n');
    return oport;
}

SExprp openInputFile( SExprp args, Evaluator* eva)
{
    ensureList1<StrConst>( args, eva);
    string fname = as<StrConst>( as<Cons>( args)->car)->val();
    return Port::make( Istreamp( new ifstream( fname.c_str())), Ostreamp());
}

SExprp openOutputFile( SExprp args, Evaluator* eva)
{
    ensureList1<StrConst>( args, eva);
    string fname = as<StrConst>( as<Cons>( args)->car)->val();
    return Port::make( Istreamp(), Ostreamp( new ofstream ( fname.c_str())));
}

SExprp closeInputPort( SExprp args, Evaluator* eva)
{
    ensureList1<Port>( args, eva);
    Istreamp in = as<Port>( as<Cons>( args)->car)->in();
    assert( in.get() != 0);
    ifstream* file = dynamic_cast<ifstream*>( in.get());
    if ( file != 0) file->close();
    return as<Cons>( args)->car;
}

SExprp closeOutputPort( SExprp args, Evaluator* eva)
{
    ensureList1<Port>( args, eva);
    Ostreamp in = as<Port>( as<Cons>( args)->car)->out();
    assert( in.get() != 0);
    ofstream* file = dynamic_cast<ofstream*>( in.get());
    if ( file != 0) file->close();
    return as<Cons>( args)->car;
}

SExprp load( SExprp args, Evaluator* eva)
{
    ensureList1<StrConst>( args, eva);
    string fname = as<StrConst>( as<Cons>( args)->car)->val();
    parser::Parser p( eva->context()->getStrings(), fname);
    if ( !p.fileOpened())
        throw Exception( "can't open file", args);
//        return Boolean::make( false);
    while (!p.eof())
    {
        bool end = false;
        SExprp exp = p.read( &end);
        if ( end) break;
        eva->eval( exp, false);
    }
    return Boolean::make( true);
}

void registerIOFunctions( Context* ctx, StringTable* strs)
{
    ctx->regFunction( strs->id( "input-port?"), &inputPort);
    ctx->regFunction( strs->id( "output-port?"), &outputPort);
    ctx->regFunction( strs->id( "current-input-port"), &currentInputPort);
    ctx->regFunction( strs->id( "current-output-port"), &currentOutputPort);
    ctx->regFunction( strs->id( "read-char"), &readChar);
    ctx->regFunction( strs->id( "peek-char"), &peekChar);
    ctx->regFunction( strs->id( "char-ready?"), &isCharReady);
    ctx->regFunction( strs->id( "write-char"), &writeChar);
    ctx->regFunction( strs->id( "newline"), &newline);

    ctx->regFunction( strs->id( "open-input-file"), &openInputFile);
    ctx->regFunction( strs->id( "open-output-file"), &openOutputFile);
    ctx->regFunction( strs->id( "close-input-port"), &closeInputPort);
    ctx->regFunction( strs->id( "close-output-port"), &closeOutputPort);

    ctx->regFunction( strs->id( "load"), &load);
}

}//namespace interpreter
