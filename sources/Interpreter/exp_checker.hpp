/**
 * @file:exp_checker.hpp
 * A set of checking functions generating exceptions, when dynamic types
 * don't conforming requirements.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */


#pragma once

#include <boost/format.hpp>
#include "interpreter_exception.hpp"
#include "est.hpp"
#include "evaluator.hpp"

namespace interpreter
{

using boost::format;
using boost::str;
using namespace est;

template<typename T>
inline void ensure( SExprp e, Evaluator* eva)//throws Exception
{
    if ( e->type != getClassRuntimeTag<T>())
        throw Exception( str( format( " the expression must be a %s")
                              % getClassRuntimeName<T>()),
                         e, eva->backtrace());
}

template<>
inline void ensure<SExpr>( SExprp e, Evaluator* eva) {}

template<typename Checker1, typename Checker2>
inline void ensureOr( Checker1 check1, Checker2 check2, SExprp e, Evaluator* eva)//throws Exception
{
    try
    {
        check1( e, eva);
    }
    catch( Exception exp1)
    {
        try
        {
            check2( e, eva);
        }
        catch( Exception exp2)
        {
            throw Exception( str( format( " a subexpression must conform: %s or %s ")
                                  % exp1.what()
                                  % exp2.what()),
                             e, eva->backtrace());
        }
    }
}

template<typename CAR, typename CDR>
inline void ensureCons( SExprp cons, Evaluator* eva)//throws Exception
{
    ensure<Cons>( cons, eva);
    ensure<CAR>( as<Cons>(cons)->car, eva);
    ensure<CDR>( as<Cons>(cons)->cdr, eva);
}

inline void ensureList( SExprp list, Evaluator* eva)//throws Exception
{
    ensureOr( ensure<Cons>, ensure<Nil>, list, eva);
}

template<class T1>
inline void ensureList1( SExprp list, Evaluator* eva)//throws Exception
{
    ensureCons<T1, Nil>( list, eva);
}

template<class T1, class T2>
inline void ensureList2( SExprp list, Evaluator* eva)//throws Exception
{
    ensureCons<T1, Cons>( list, eva);
    ensureList1<T2>( as<Cons>(list)->cdr, eva);
}

template <class T1, class T2, class T3>
inline void ensureList3( SExprp list, Evaluator* eva)//throws Exception
{
    ensureCons<T1, Cons>( list, eva);
    ensureList2<T2, T3>( as<Cons>(list)->cdr, eva);
}

template <class T1, class T2, class T3, class T4>
inline void ensureList4( SExprp list, Evaluator* eva)//throws Exception
{
    ensureCons<T1, Cons>( list, eva);
    ensureList3<T2, T3, T4>( as<Cons>(list)->cdr, eva);
}

}//namespace interpreter
