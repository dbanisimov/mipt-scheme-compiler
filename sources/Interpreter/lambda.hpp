/**
 * @file:lambda.hpp
 * A declaration for classes creating storing and managing of lambda functions,
 * used in simulator to support user defined functions and macroses.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once
#include <vector>
#include <iostream>
#include "evaluator.hpp"
#include "interpreter_dep.hpp"

namespace interpreter
{

    class Evaluator;

namespace lambda
{

using std::vector;
using parser::ast::SExprp;

class Lambda
{
public:
    Lambda( vector<int> par, int rest_, SExprp body_,
            Activationp scope_, bool do_tail_rec_)
        :params( par), rest( rest_), body( body_),
         scope(scope_), do_tail_rec( do_tail_rec_) {}
    SExprp operator()( SExprp args, Evaluator* eva);

private:
    vector<int> params;
    int rest;
    SExprp body;
    Activationp scope;
    bool do_tail_rec;

    void bindParams( SExprp args, Context* ctx);
    SExprp eval( Evaluator* eva);
};

class LambdaManager
{
public:
    ~LambdaManager();
    Lambda* makeLambda( SExprp args_and_body, Activationp scope,
                        Evaluator* eva, bool do_tail_rec);
private:
    vector<Lambda*> lambdas;
};

}//namespace lambda
}//namespace interpreter
