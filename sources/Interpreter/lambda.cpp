
#include <vector>
#include <boost/format.hpp>
#include "interpreter_dep.hpp"
#include "lambda.hpp"
#include "context.hpp"
#include "interpreter_exception.hpp"
#include "exp_checker.hpp"

namespace interpreter
{
namespace lambda 
{

using std::vector;
using namespace parser::ast;
using boost::format;
using boost::str;

SExprp Lambda::operator()( SExprp args, Evaluator* eva)
{
    Context* ctx = eva->context();
    Activationp last = ctx->activation();
    ctx->setActivation( scope);
    ctx->enterScope();
    bindParams( args, ctx);
    SExprp rez = eval( eva);
    ctx->exitScope();
    ctx->setActivation( last);
    return rez;
}
SExprp Lambda::eval( Evaluator* eva)
{
    SExprp todo = body;
    SExprp rez = SExprp( new Nil);
    while ( todo->type == SExpr::cons)
    {
        bool tail = do_tail_rec
                 && as<Cons>(todo)->cdr->type == SExpr::nil;
        rez = eva->eval( as<Cons>(todo)->car, tail);
        todo = as<Cons>(todo)->cdr;
    }
    assert( todo->type == SExpr::nil);
    return rez;
}

void Lambda::bindParams( SExprp args, Context* ctx)
{
    for ( int i = 0; i < params.size(); ++i)
    {
        if ( args->type != SExpr::cons)
            throw Exception( str( format( " wrong number of mandatory"
                                          " arguments(%d) given -"
                                          " required %d ")
                                  % i % params.size()),
                             args);
                                     
        assert( args->type == SExpr::cons);
        SExprp param = as<Cons>(args)->car;
        args = as<Cons>(args)->cdr;
        ctx->defVariable( params[i], param);
    }
    if ( rest != -1 )
    {
        ctx->defVariable( rest, args);
    }
}
LambdaManager::~LambdaManager()
{
    for ( vector<Lambda*>::iterator i = lambdas.begin(); i != lambdas.end(); ++i)
    {
        delete *i;
    }
}
Lambda* LambdaManager::makeLambda( SExprp argsNbody, Activationp scope,
                                   Evaluator* eva, bool do_tail_rec)
{
    ensure<Cons>( argsNbody, eva);
    SExprp args = as<Cons>(argsNbody)->car;
    SExprp body = as<Cons>(argsNbody)->cdr;
    assert( body->isList());
    vector<int> params;
    int rest = -1;
    while ( args->type == SExpr::cons)
    {
        SExprp arg = as<Cons>(args)->car;
        args = as<Cons>(args)->cdr;
        ensure<Symbol>( arg, eva);
        params.push_back( as<Symbol>(arg)->getName());
    }
    if ( args->type == SExpr::symbol)
        rest = as<Symbol>(args)->getName();
    else
    {
        rest = -1;
        ensure<Nil>( args, eva);
    }
    Lambda* l = new Lambda( params, rest, body, scope, do_tail_rec);
    lambdas.push_back( l);
    return l;
}

}//namespace lambda
}//namespace interpreter
