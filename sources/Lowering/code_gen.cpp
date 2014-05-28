/**
 * File: ir_code_gen.cpp
 * Copyright (C) 2012 Denis Anisimov
 */

#include "code_gen.hpp"

#define FREE_MEM_PTR_ADDR 0
#define REF_SIZE 4
#define NIL_VALUE 0

namespace lowering
{
using namespace parser::ast;
using namespace IR;
using namespace Opt;

/**
 * Constructor
 */
CodeGen::CodeGen( StringTable* strt): st( strt), cur_env() 
{
};

/**
 * Main routine to generate code for given ast.
 */
void CodeGen::gen( SExprp subj)
{  
    /* Create outermost environment */ 
    cur_env.reset( new Environment());
    outer_env = cur_env;

    /* Create main function */
    cur_func = module.newFunction( "main");

    /* Create malloc */
    doMalloc();

    /* Init free mem ptr */
    cur_oper = cur_func->newOper( St);
    cur_func->setFirstOper( cur_oper);
    cur_oper->setArgImm( 0, (Int64)0);
    cur_oper->setArgImm( 1, (Int64)FREE_MEM_PTR_ADDR);

    /* Create env reg */
    env_reg = cur_func->newObject( Reg);

#if 0
    /* FIXME: return to operation when continuations would be ready */
    /* Allocate memory for outermost continuation info */
    cont_reg = cur_func->newObject( Reg);
    cur_oper = cur_func->newOperAfter( Call, cur_oper);
    cur_oper->setArgImm( 0, string("malloc"));
    cur_oper->addArgToSet( (Int64)( REF_SIZE ));
    cur_oper->addResToSet( cont_reg);
    
    /* Set outer continuation to nil */
    cur_oper = cur_func->newOperAfter( St, cur_oper);
    cur_oper->setArgImm( 0, (Int64)NIL_VALUE);
    cur_oper->setArgObj( 1, cont_reg);
#endif

    /* Start traversal */
    subj.visit( this);

    /**
     * Create environment. 
     * It is done in the end, because now outermost env is fully defined
     */
    OptOper* oper = cur_func->newOper( Call);
    oper->setArgImm( 0, string("malloc"));
    oper->addArgToSet( (Int64)( cur_env->getSize()));
    oper->addResToSet( env_reg);

    /* Return result */
    /* FIXME: now - single gen = single program & main */
    cur_oper = cur_func->newOperAfter( Ret, cur_oper);
    cur_oper->addArgToSet( return_reg);
}

/**
 * Create simple memory allocation function
 * FIXME: replace with something more intellectual
 */
void CodeGen::doMalloc()
{
    /* Create malloc function */
    OptFunction* prev_func = cur_func;
    OptOper* prev_oper = cur_oper;
    cur_func = module.newFunction( "malloc");
    OptObj* arg = cur_func->newArg( 0, Reg);

    /* Store current free pointer */
    /**
     * FIXME: For the moment it is assumed that malloc
     * next free memory pointer is stored at 0x0 address.
     */
    OptObj* free_addr = cur_func->newObject( Reg);
    cur_oper = cur_func->newOper( Ld);
    cur_func->setFirstOper( cur_oper);
    cur_oper->setArgImm( 0, (Int64)FREE_MEM_PTR_ADDR);
    cur_oper->setResObj( 0, free_addr);

    /* Increment by arg */
    OptObj* tmp = cur_func->newObject( Reg);
    cur_oper = cur_func->newOperAfter( Add, cur_oper);
    cur_oper->setArgObj( 0, free_addr);
    cur_oper->setArgObj( 1, arg);
    cur_oper->setResObj( 0, tmp);

    /* Store */
    cur_oper = cur_func->newOperAfter( St, cur_oper);
    cur_oper->setArgObj( 0, tmp);
    cur_oper->setArgImm( 1, (Int64)FREE_MEM_PTR_ADDR);

    /* Return allocated addr */
    cur_oper = cur_func->newOperAfter( Ret, cur_oper);
    cur_oper->addArgToSet( free_addr);

    /* Restore values */
    cur_func = prev_func;
    cur_oper = prev_oper;
}

/**
 * Print IR in work
 */
void CodeGen::print()
{
    cout << &module;
}

SExprp CodeGen::onNil( SExprp subj)
{
    /* Realy nothing to do here */

    return subj;
}

SExprp CodeGen::onCons( SExprp subj)
{
    /*
     * Main idea: explicit cons processing means some function/special form
     */

    SExprp car = as<Cons>(subj)->car;
    SExprp cdr = as<Cons>(subj)->cdr;

    /* Function or special form in ast could be symbol only */
    if ( car->type != SExpr::symbol )
    {
        throw LoweringException( "Expected symbol");
    }

    /* Check type and do appropriate routine */
    if ( isLambda( car) )
    {
        doLambda( cdr);
    } else if ( isDefine( car) )
    {
        doDefine( cdr);
    } else if ( isLet( car) || isLetSeq( car) || isLetRec( car) )
    {
        /* FIXME: Add separate processing of different lets */
        doLet( cdr);
    } else
    {
        doFunctionCall( subj);
#if 0
        throw LoweringException( str( boost::format( "Special form %s is not supported") 
                                      % st->str(as<Symbol>(car)->getName())));
#endif 
    }

    return subj;
}

SExprp CodeGen::onSymbol( SExprp subj)
{
    /*
     * FIXME: Add symbol evaluation.
     */

    int sym_id = as<Symbol>( subj)->getName();

    if ( !cur_env->testBinding( sym_id) )
    {
        throw LoweringException( str( boost::format( "Variable \"%s\"(#%d) is not bound") %  st->str(sym_id) % sym_id));
    }

    /* Calculate address by binding offset */
    /* FIXME: add depth */
    int offset = cur_env->findBinding( sym_id);
    OptObj* bind_addr = cur_func->newObject( Reg);
    cur_oper = cur_func->newOperAfter( Add, cur_oper);
    cur_oper->setArgObj( 0, env_reg);
    cur_oper->setArgImm( 1, (Int64)( offset * REF_SIZE ));
    cur_oper->setResObj( 0, bind_addr);

    /* Load value from environment */
    cur_oper = cur_func->newOperAfter( Ld, cur_oper);
    cur_oper->setArgObj( 0, bind_addr);
    cur_oper->setResObj( 0, return_reg);

    return subj;
}

SExprp CodeGen::onVector( SExprp subj)
{
    throw LoweringException( str( boost::format("%s is not yet implemented") % __func__));
}


SExprp CodeGen::onNumber( SExprp subj)
{
    /**
     * Load number as imm to reg
     */

    OptObj* reg = cur_func->newObject( Reg);
    OptOper* ld = cur_func->newOperAfter( Ld, cur_oper);
    cur_oper = ld;

    /**
     * FIXME: only int values are supported
     */
    ld->setArgImm( 0, (Int64)as<Number>(subj)->val());
    ld->setResObj( 0, reg);

    /* Return reg obj */
    return_reg = reg;

    return subj;
}

SExprp CodeGen::onStrConst( SExprp subj)
{
    throw LoweringException( str( boost::format("%s is not yet implemented") % __func__));
}

SExprp CodeGen::onCharacter( SExprp subj)
{
    throw LoweringException( str( boost::format("%s is not yet implemented") % __func__));
}

SExprp CodeGen::onBoolean( SExprp subj)
{
    throw LoweringException( str( boost::format("%s is not yet implemented") % __func__));
}

/**
 * Process lambda definition
 */
void CodeGen::doLambda( SExprp lambda)
{
    throw LoweringException( str( boost::format("%s is not yet implemented") % __func__));
#if 0
    /* Create new environment */
    Envp env( new Environment( cur_env));
    cur_env = env;

    SExprp formals = as<Cons>(lambda)->car;
    SExprp body = as<Cons>(lambda)->cdr;
    SExprp sym;

    /* Add symbols to it */
    for ( ;
          formals->type != SExpr::nil;
          formals = as<Cons>(formals)->cdr)
    {
        sym = as<Cons>(formals)->car;
        env->addBinding( as<Symbol>(sym)->getName());
    }

    /* 
     * Continue with the body of lambda.
     * Note: in scheme lambda body may consist of list of expressions, 
     * so the result of lambda body is the result of the last expression.
     */
    while ( body->type != SExpr::nil )
    {
        as<Cons>(body)->car.visit( this);
        body = as<Cons>(body)->cdr;
    }

    /* Lambda is done - switch to outer environment */
    cur_env = cur_env->getOuter();
#endif
}

/**
 * Process definitions
 */
void CodeGen::doDefine( SExprp define)
{
    /*
     * Note: only simple define form is supported
     *   define <variabe> <expr>
     * Other forms should be expanded to simple form before
     */

    SExprp var = as<Cons>(define)->car;
    SExprp expr = as<Cons>(as<Cons>(define)->cdr)->car;
   
    int sym_id = as<Symbol>(var)->getName();

    /*
     * If variabe is already bound then behaviour of 
     * define is equivalent to set!
     */
    if ( cur_env->testBinding( sym_id))
    {
        /* FIXME: do set! */
    } else
    {
        /* Add new binding */
        cur_env->addBinding( sym_id);
    }

    /* Expr evaluation */
    expr.visit( this);

    /* Calculate address by binding offset */
    /* FIXME: add depth */
    int offset = cur_env->findBinding( sym_id);
    OptObj* bind_addr = cur_func->newObject( Reg);
    cur_oper = cur_func->newOperAfter( Add, cur_oper);
    cur_oper->setArgObj( 0, env_reg);
    cur_oper->setArgImm( 1, (Int64)( offset * REF_SIZE ));
    cur_oper->setResObj( 0, bind_addr);

    /* Store expr value to environment */
    cur_oper = cur_func->newOperAfter( St, cur_oper);
    cur_oper->setArgObj( 0, return_reg);
    cur_oper->setArgObj( 1, bind_addr);
}

/**
 * Process let bindings
 */
void CodeGen::doLet( SExprp let)
{
    throw LoweringException( "Let expression is not supported explicitly. Please expand to Lambda.");
#if 0
    /* 
     * Create new environment.
     * Note: init expressions are evaluated within old environment,
     * so do not switch environment here!
     */
    Envp env( new Environment( cur_env));

    SExprp bindings = as<Cons>(let)->car;
    SExprp body = as<Cons>(let)->cdr;

    /* Process bindings */
    for ( ;
          bindings->type != SExpr::nil;
          bindings = as<Cons>(bindings)->cdr)
    {
        SExprp binding = as<Cons>(bindings)->car;
        SExprp var = as<Cons>(binding)->car;
        SExprp init = as<Cons>(as<Cons>(binding)->cdr)->car;

        /* FIXME: should check multiple bindings with the same name - it's an error! */
        env->addBinding( as<Symbol>(var)->getName());

        /* Process var init */
        init.visit();

        /* Calculate value address */
        /* FIXME: why not to get this value after add? */
        int bind_num = env->findBinding( as<Symbol>(var)->getName());
        cur_oper = cur_func->newOperAfter( Add, cur_oper);


        cur_oper = cur_func->newOperAfter( St, cur_oper);
        cur_oper->setArgObj( 0, return_reg);
        cur_oper->setArgObj( 1, _addr);
    }

    /* Switch environment */
    cur_env = env;

    /* FIXME: unite with the same part of doLambda */
    /* 
     * Continue with the body of let.
     * Note: in scheme let body may consist of list of expressions, 
     * so the result of let body is the result of the last expression.
     */
    while ( body->type != SExpr::nil )
    {
        as<Cons>(body)->car.visit( this);
        body = as<Cons>(body)->cdr;
    }

    /* Let is done - switch to outer environment */
    cur_env = cur_env->getOuter();
#endif
}

/**
 * Process function call
 */
void CodeGen::doFunctionCall( SExprp func)
{
    SExprp name = as<Cons>(func)->car;
    SExprp args = as<Cons>(func)->cdr;

    /**
     * Firstly process list of function arguments.
     * The result of such processing should be a
     * pointer to list of arguments
     */
    doList( args);

    /**
     * Check whether function is defined
     */
    int sym_id = as<Symbol>(name)->getName();
    string func_name = getSymStr( name);
    if ( cur_env->testBinding( sym_id) )
    {
        /* FIXME: add function addr load */
    } else if ( isStdFunc( func_name) )
    {
        /* Add function to outer environment */
        outer_env->addBinding( sym_id);
        /* Do function */
        doStdFunc( func_name);
    } else
    {
        throw LoweringException( str( boost::format( "Function \"%s\"(#%d) is not defined") %  st->str(sym_id) % sym_id));
    }

    /**
     * Create function call
     */
    /**
     * FIXME: function is defined check should be here
     */

    cur_oper = cur_func->newOperAfter( Call, cur_oper);
    cur_oper->setArgImm( 0, string( getSymStr( name) ));
    cur_oper->addArgToSet( return_reg);
    return_reg = cur_func->newObject( Reg);
    cur_oper->addResToSet( return_reg);
}

/**
 * Process list
 */
void CodeGen::doList( SExprp list)
{
    assert( list->type != SExpr::nil);

    /* Allocate cons memory */
    cur_oper = cur_func->newOperAfter( Call, cur_oper);
    cur_oper->setArgImm( 0, string("malloc"));
    cur_oper->addArgToSet( (Int64)( REF_SIZE * 2 ));
    OptObj* car_addr = cur_func->newObject( Reg);
    cur_oper->addResToSet( car_addr);

    /* Process list item expression */
    as<Cons>(list)->car.visit( this);

    /* Write result to car */
    cur_oper = cur_func->newOperAfter( St, cur_oper);
    cur_oper->setArgObj( 0, return_reg);
    cur_oper->setArgObj( 1, car_addr);

    /* Compute cdr address */
    cur_oper = cur_func->newOperAfter( Add, cur_oper);
    cur_oper->setArgObj( 0, car_addr);
    cur_oper->setArgImm( 1, (Int64)REF_SIZE);
    OptObj* cdr_addr = cur_func->newObject( Reg);
    cur_oper->setResObj( 0, cdr_addr);

    /* Process rest of list if it is not null */
    if ( as<Cons>(list)->cdr->type != SExpr::nil )
    {
        doList( as<Cons>(list)->cdr);

        /* Store result of list creation to cdr */
        cur_oper = cur_func->newOperAfter( St, cur_oper);
        cur_oper->setArgObj( 0, return_reg);
        cur_oper->setArgObj( 1, cdr_addr);
    } else 
    {
        /* Store nil in cdr*/
        cur_oper = cur_func->newOperAfter( St, cur_oper);
        cur_oper->setArgImm( 0, (Int64) NIL_VALUE);
        cur_oper->setArgObj( 1, cdr_addr);
    }

    /* Return car */
    return_reg = car_addr;
}


/**
 * Create numeric function with given arithm oper 
 */
void CodeGen::doFuncNumericWithOper( OperName_t oper_name, string func_name)
{
    /* Create new function */
    OptFunction* prev_func = cur_func;
    OptOper* prev_oper = cur_oper;
    cur_func = module.newFunction( func_name);
    /**
     * Function argument is the address of the head of list to be processed 
     * Load it and start traversal with specified oper.
     */
    OptObj* car_addr = cur_func->newArg(0, Reg);
    cur_oper = cur_func->newOper( Ld);
    cur_func->setFirstOper( cur_oper);
    cur_oper->setArgObj( 0, car_addr);
    OptObj* car_val = cur_func->newObject( Reg);
    cur_oper->setResObj( 0, car_val);

    /* If next addr is null - return */
    cur_oper = cur_func->newOperAfter( Add, cur_oper);
    OptOper* next_addr_oper = cur_oper;
    cur_oper->setArgObj( 0, car_addr);
    cur_oper->setArgImm( 1, (Int64)REF_SIZE);
    OptObj* cdr_addr = cur_func->newObject( Reg);
    cur_oper->setResObj( 0, cdr_addr);
    OptObj* next_car_addr = cur_func->newObject( Reg);
    cur_oper = cur_func->newOperAfter( Ld, cur_oper);
    cur_oper->setArgObj( 0, cdr_addr);
    cur_oper->setResObj( 0, next_car_addr);

    cur_oper = cur_func->newOperAfter( Bre, cur_oper);
    OptOper* bre_oper = cur_oper;
    cur_oper->setArgObj( 0, next_car_addr);
    cur_oper->setArgImm( 1, (Int64)NIL_VALUE);
    // Target is set below

    /* Load next value */
    OptObj* next_car_val = cur_func->newObject( Reg);
    cur_oper = cur_func->newOperAfter( Ld, cur_oper);
    cur_oper->setArgObj( 0, next_car_addr);
    cur_oper->setResObj( 0, next_car_val);

    /* Compute result */
    cur_oper = cur_func->newOperAfter( oper_name, cur_oper);
    cur_oper->setArgObj( 0, car_val);
    cur_oper->setArgObj( 1, next_car_val);
    cur_oper->setResObj( 0, next_car_val);

    cur_oper = cur_func->newOperAfter( Ld, cur_oper);
    cur_oper->setArgObj( 0, next_car_val);
    cur_oper->setResObj( 0, car_val);

    cur_oper = cur_func->newOperAfter( Ld, cur_oper);
    cur_oper->setArgObj( 0, next_car_addr);
    cur_oper->setResObj( 0, car_addr);

    /* Branch on next addr computation and test */
    cur_oper = cur_func->newOperAfter( Br, cur_oper);
    cur_oper->setArgTarget( 0, next_addr_oper);

    /* Return result (and make target for bre) */
    cur_oper = cur_func->newOperAfter( Ret, cur_oper);
    cur_oper->addArgToSet( car_val);
    bre_oper->setArgTarget( 2, cur_oper);

    /* Back to previous function */
    cur_func = prev_func;    
    cur_oper = prev_oper;
}

/**
 * Get real name of symbol
 */
std::string CodeGen::getSymStr( SExprp subj)
{
    int sym_id = as<Symbol>(subj)->getName();
    return st->str( sym_id);
}

/**
 * Many different symbol type checkers
 */
/* FIXME: probably could be simplified by deriving new types from SExpr */
bool CodeGen::isLambda( SExprp subj)
{
    return ( getSymStr( subj) == "lambda");
}

bool CodeGen::isDefine( SExprp subj)
{
    return ( getSymStr( subj) == "define");
}

bool CodeGen::isLet( SExprp subj)
{
    return ( getSymStr( subj) == "let");
}

bool CodeGen::isLetSeq( SExprp subj)
{
    return ( getSymStr( subj) == "let*");
}

bool CodeGen::isLetRec( SExprp subj)
{
    return ( getSymStr( subj) == "letrec");
}

} //namespace lowering
