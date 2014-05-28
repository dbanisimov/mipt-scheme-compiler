/**
 * File: Environment.cpp
 * Implementation of compile & run-time variables environment.
 * Copyright (C) 2012 Denis Anisimov
 */

#include "environment.hpp"

namespace lowering
{

/* Next free id */
int Environment::free_bind_id = 0;
int Environment::free_env_id = 0;

/* Allocate envs list */
vector<Environment*> Environment::env_list;

/* Default constructor */
Environment::Environment(): outer(),id( free_env_id++) 
{ 
    Environment::env_list.push_back( this);
};

/* Constructor with outer Environment */
Environment::Environment( Envp out): outer( out),id( free_env_id++) 
{
    Environment::env_list.push_back( this);
};

/* Set outer Environment */
void Environment::setOuter( Envp out)
{
    outer = out;
}

/* Get outer environment */
Envp Environment::getOuter()
{
    return outer;
}

/* Check whether this environment is outermost */
bool Environment::isOutermost()
{
    return ( outer == NULL);
}

/* Add binding with given name */
void Environment::addBinding( int sym)
{
    Binding bind( sym, free_bind_id++);
    push_back( bind);
}

/**
 * Recursively find binding in chain of environments
 * and return chain depth. 
 * Return -1 if variable is not bound
 */
int Environment::findBinding( int sym)
{
    int dep = 0;

    Environment* env = this;
    vector<Binding>::const_iterator it; 
    for ( it = begin(); 
          ( it < end() ) && ( it->sym != sym );
          it++ ){};
    bool res = ( it != end());
    while ( !res && !env->isOutermost())
    {
        dep++;
        env = env->getOuter().get();
        for ( it = env->begin(); 
              ( it < env->end() ) && ( it->sym != sym );
              it++ ){};
        res = ( it != env->end());
    }

    return (res) ? dep : -1;
}


/**
 * Recursively test whether variable is bound
 * in chain of environments.
 */
bool Environment::testBinding( int sym)
{
    return ( findBinding( sym) != -1 );
}

/**
 * Total size of the environment
 */
int Environment::getSize()
{
    int size = 0;
    vector<Binding>::const_iterator it; 

    for ( it = begin(); it < end(); it++ )
    {
        size += it->size;
    }

    return size;
}

/* Output to stream */
void Environment::toStream(ostream& os) const
{
    os << "| ## |  sym  |  id  |" << endl;
    int i = 0;
    for ( vector<Binding>::const_iterator it=begin(); it < end(); it++)
    {
        os << setw(6) << i++ << setw(8) << it->sym << setw(7) << it->id << endl;
    }
}

/* Stream operator output */
std::ostream& operator<< (std::ostream& os, const Environment& env) 
{            
    env.toStream(os);
    return os;
}

} //namespace lowering
