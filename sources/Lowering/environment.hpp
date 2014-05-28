/**
 * File: Environment.hpp
 * Interface of compile & run-time variables environment.
 * Copyright (C) 2012 Denis Anisimov
 */

#pragma once

#include <vector>
#include <string>
#include <boost/shared_ptr.hpp>
#include <ostream>
#include <iomanip>

namespace lowering{

using std::vector;
using std::ostream;
using std::endl;
using std::setw;

class Binding
{
public:
    Binding( int s, int i): sym( s),id(i),size(4) {};
    int sym;
    int id;
    int size;
};

class Environment;

typedef boost::shared_ptr<Environment> Envp;

class Environment :private vector<Binding>
{
public:
    /* Default constructor */
    Environment();

    /* Constructor with outer Environment */
    Environment( Envp out);

    /* Next available unique id */
    static int free_bind_id;
    static int free_env_id;

    /* Set outer Environment */
    void setOuter( Envp out);

    /* Get outer environment */
    Envp getOuter();

    /* Check whether this environment is outermost */
    bool isOutermost();

    /* Add binding with given name and symbol id */
    void addBinding( int sym);

    /**
     * Recursively find binding in chain of environments
     * and return chain depth. 
     * Return -1 if variable is not bound
     */
    int findBinding( int sym);
 
    /**
     * Recursively test whether variable is bound
     * in chain of environments.
     */
    bool testBinding( int sym);

    /**
     * Total size of the environment
     */
    int getSize();

    /* Output to stream */
    void toStream(ostream& os) const;

    friend std::ostream& operator<<(ostream &os, const Environment &env);

    /* FIXME: reconsider */
    static vector<Environment*> env_list;
private:
    Envp outer;
    int id;
};

} //namespace lowering
