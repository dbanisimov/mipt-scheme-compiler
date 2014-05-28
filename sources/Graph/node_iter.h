/**
 * @file: node_iter.h
 * Declaration of iterators for node's adjacent edges traversal and related classes
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef NODE_ITER_H
#define NODE_ITER_H


/** Baseclass for implementing iterator template parameters */
template < class N, class E> class IterImplBase
{
public:
    /** Get edge */
    inline E* edge() const { return edge_p;}
    /** Set edge */
    inline void setEdge( E *e){ edge_p = e;}
    /** Default constructor */
    inline IterImplBase(): edge_p( NULL) {}
protected:
    E *edge_p;
};

/** Parameter for iterator template specialization (pred traversal) */
template < class N, class E> class PredIterImpl: public IterImplBase< N, E>
{
public:
    inline void nextEdge();       /**< Move on to the next pred */
    inline N *node() const;       /**< Get node */
    inline PredIterImpl(){};      /**< Default constructor */
    inline PredIterImpl( N *n);   /**< Configures iterator with node's first pred */
    inline bool operator==(const PredIterImpl& o) const /**< Comparison operator */
    { 
        return this->edge_p == o.edge_p;
    }
};
/** Parameter for iterator template specialization (succ traversal) */
template < class N, class E> class SuccIterImpl: public IterImplBase< N, E>
{
public:
    inline void nextEdge();       /**< Move on to the next succ */
    inline N *node() const;       /**< Get node */
    inline SuccIterImpl(){};      /**< Default constructor */
    inline SuccIterImpl( N *n);   /**< Configures iterator with node's first succ */
    inline bool operator==(const SuccIterImpl& o) const /**< Comparison operator */
    { 
        return this->edge_p == o.edge_p;
    }
};

/** Parameter for iterator template specialization (undirected traversal) */
template < class N, class E> class UnDirIterImpl: public IterImplBase< N, E>
{
public:
    inline void nextEdge();        /**< Move on to the next edge */
    inline N *node() const;     /**< Get node */
    inline UnDirIterImpl():is_pred( false){};      /**< Default consturctor */
    inline UnDirIterImpl( N *n);/**< Configures iterator with node's first edge */
    inline bool operator==(const UnDirIterImpl& o) const /**< Comparison operator */
    { 
        return this->edge_p == o.edge_p 
               && this->is_pred == o.is_pred;
    }
private:
    bool is_pred;
};

/**
 * Convinience template for iterating through node's adjacent edges
 */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl> class EdgeIterIface
{   
public:
    /** Constructor from node */
    inline EdgeIterIface( N *n);
    /** Default constructor */
    inline EdgeIterIface();
    /** Copy constructor */
    inline EdgeIterIface( const EdgeIterIface& proto);
    /** Destructor */
    inline ~EdgeIterIface();
    /** Preincrement operator */
    inline EdgeIterIface & operator++();
    /** Postincrement operator */
    inline EdgeIterIface operator++( int);
    /** Dereferenece operator*/
    inline E * operator*();
    /** Comparison operator */
    inline bool operator==(const EdgeIterIface& o) const; 
    /** Not equals operator */
    inline bool operator!=(const EdgeIterIface& o) const;
    /** Get EdgeImpl */
    inline E *edge() const;
    /** Get node on the end of edge */
    inline N *node() const;

    /** Parameter-dependent implementation */
    EdgeIterImpl< N, E> impl;
};

#endif
