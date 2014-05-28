/**
 * @file: node_inline.h
 * Implementation of NodeImpl and related classes' inline routines
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef NODE_INLINE_H
#define NODE_INLINE_H

/** We can't create nodes separately, do it through newNode method of graph */
inline NodeImpl::NodeImpl( GraphImpl *_graph_p): uid(_graph_p->node_next_id), graph_p( _graph_p)
{
    first_edge[ GRAPH_DIR_UP] = NULL;
    first_edge[ GRAPH_DIR_DOWN] = NULL;
}

/**
 * Detach myself from GraphImpl's node list
 */
inline void NodeImpl::detachFromGraph()
{
    detach();
}

/**
 * Get node's unique ID
 */
inline GraphUid NodeImpl::id() const
{
    return uid;
}

/**
 * Get node's corresponding GraphImpl
 */
inline GraphImpl * NodeImpl::graph() const
{
    return graph_p;
}
/**
 * Next node in GraphImpl's list
 */
inline NodeImpl* NodeImpl::nextNode() const
{
    return next();
}

/**
 * Prev node in GraphImpl's list
 */
inline NodeImpl* NodeImpl::prevNode() const
{
    return prev();
}

/**
 * Add predecessor edge
 */
inline void NodeImpl::AddPred( EdgeImpl *edge)
{
    AddEdgeInDir( edge, GRAPH_DIR_UP);
}

/**
 * Add successor edge
 */
inline void NodeImpl::AddSucc( EdgeImpl *edge) 
{
    AddEdgeInDir( edge, GRAPH_DIR_DOWN);
}
/**
 * First edge in given direction
 */
inline EdgeImpl* NodeImpl::firstEdgeInDir( GraphDir dir) const
{
    return first_edge[ dir];
}
/** 
 * First successor edge
 */
inline EdgeImpl* NodeImpl::firstSucc() const
{
    return firstEdgeInDir( GRAPH_DIR_DOWN);
}
/** 
 * First predecessor edge
 */
inline EdgeImpl* NodeImpl::firstPred() const
{
    return firstEdgeInDir( GRAPH_DIR_UP);
}

/**
 * delete predecessor edge
 */
inline void NodeImpl::deletePred( EdgeImpl* edge)
{
    deleteEdgeInDir( GRAPH_DIR_UP, edge);
}

/**
 * delete successor edge
 */
inline void NodeImpl::deleteSucc( EdgeImpl* edge)
{
    deleteEdgeInDir( GRAPH_DIR_DOWN, edge);
}

/**
 * Add an edge to this node in specified direction
 */
inline void
NodeImpl::AddEdgeInDir( EdgeImpl *edge, GraphDir dir)
{
    GRAPH_ASSERTD( isNotNullP( edge));
    GRAPH_ASSERTXD( (int) GRAPH_DIR_DOWN == (int) EDGE_LIST_SUCCS,
                   "Enums of direction and edge lists are not having right values");
    GRAPH_ASSERTXD( (int) GRAPH_DIR_UP == (int) EDGE_LIST_PREDS,
                   "Enums of direction and edge lists are not having right values");
    edge->attach( dir, first_edge[ dir]); 
    first_edge[ dir] = edge;
}

/**
 * delete edge pointed by iterator in specidied direction
 */
inline void
NodeImpl::deleteEdgeInDir( GraphDir dir, EdgeImpl* edge)
{
    GRAPH_ASSERTD( isNotNullP( edge));
    if( first_edge[ dir] == edge)
    {
        first_edge[ dir] = edge->nextEdgeInDir( dir);
    }
}

template < class G, class N, class E> 
Node< G, N, E>::~Node()
{
    E *edge;
    
    /** delete incidient edges */
    for ( edge = firstSucc(); isNotNullP( edge);)
    {
        E* next = edge->nextSucc();
        graph()->deleteEdge( edge);
        edge = next;
    }
    for ( edge = firstPred(); isNotNullP( edge);)
    {
        E* next = edge->nextPred();
        graph()->deleteEdge( edge);
        edge = next;
    }
}

/** Get node's corresponding GraphImpl */
template < class G, class N, class E> 
G* 
Node< G, N, E>::graph() const
{
    return static_cast< G*>(NodeImpl::graph());
}

/** Next node in GraphImpl's list      */
template < class G, class N, class E>
N *
Node< G, N, E>::nextNode() const
{
    return static_cast< N*>(NodeImpl::nextNode());
}

/**< Prev node in GraphImpl's list      */
template < class G, class N, class E> N * Node< G, N, E>::prevNode() const
{
    return static_cast< N*>(NodeImpl::prevNode());
}

/**< Get first successor edge    */
template < class G, class N, class E> E * Node< G, N, E>::firstSucc() const
{
    return static_cast< E*>(NodeImpl::firstSucc());
} 

/**< Get first predecessor edge  */
template < class G, class N, class E> E * Node< G, N, E>::firstPred() const
{
    return static_cast< E*>(NodeImpl::firstPred());
}


/**
 * Create iterator for first succ
 */
template < class G, class N, class E> 
typename Node< G, N, E>::Succ
Node< G, N, E>::succsBegin()
{
    return Succ( static_cast<N *>(this) );
}

/**
 * Create iterator pointing to succ end
 */
template < class G, class N, class E> 
typename Node< G, N, E>::Succ
Node< G, N, E>::succsEnd()
{
    return Succ();
}
/**
 * Create iterator for first succ
 */
template < class G, class N, class E> 
typename Node< G, N, E>::Pred
Node< G, N, E>::predsBegin()
{
    return Pred( static_cast<N *>(this));
}
/**
 * Create iterator pointing to succ end
 */
template < class G, class N, class E> 
typename Node< G, N, E>::Pred
Node< G, N, E>::predsEnd()
{
    return Pred();
}

/**
 * Create iterator for first succ
 */
template < class G, class N, class E> 
typename Node< G, N, E>::EdgeIter
Node< G, N, E>::edgesBegin()
{
    return EdgeIter( static_cast<N *>(this));
}
/**
 * Create iterator pointing to succ end
 */
template < class G, class N, class E> 
typename Node< G, N, E>::EdgeIter
Node< G, N, E>::edgesEnd()
{
    return EdgeIter();
}

/** Default Constructor: creates 'end' iterator */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline EdgeIterIface< N, E, EdgeIterImpl>::EdgeIterIface()
{

}

/** Constructor from node: iterator points on first edge, i.e. 'begin' iterator */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline EdgeIterIface< N, E, EdgeIterImpl>::EdgeIterIface( N *n):
    impl( n)
{

}

/** Copy constructor */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline EdgeIterIface< N, E, EdgeIterImpl>::EdgeIterIface( const EdgeIterIface& proto)
{
    impl = proto.impl;
}

/** Destructor */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline EdgeIterIface< N, E, EdgeIterImpl>::~EdgeIterIface()
{

}

/** Postincrement operator */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline EdgeIterIface< N, E, EdgeIterImpl> 
EdgeIterIface< N, E, EdgeIterImpl>::operator++( int)
{
    EdgeIterIface tmp = *this;
    ++*this;
    return tmp;
}

/** Dereferenece operator*/
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline E * 
EdgeIterIface< N, E, EdgeIterImpl>::operator*()
{
    return impl.edge();
}
    
/** Comparison operator */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl> 
inline bool
EdgeIterIface< N, E, EdgeIterImpl>::operator==(const EdgeIterIface< N, E, EdgeIterImpl>& o) const
{
    return impl == o.impl;
} 

/** Not equals operator */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline bool
EdgeIterIface< N, E, EdgeIterImpl>::operator!=(const EdgeIterIface< N, E, EdgeIterImpl>& o) const
{
    return !(*this == o);
}

/** Get EdgeImpl */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline E *
EdgeIterIface< N, E, EdgeIterImpl>::edge() const
{
    return impl.edge();
}

/** Get EdgeImpl */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline N *
EdgeIterIface< N, E, EdgeIterImpl>::node() const
{
    return impl.node();
}

/** Preincrement operator */
template < class N, class E, template < class n_class, class e_class> class EdgeIterImpl>
inline EdgeIterIface< N, E, EdgeIterImpl> & 
EdgeIterIface< N, E, EdgeIterImpl>::operator++()
{
    GRAPH_ASSERTXD( isNotNullP( impl.edge()), "EdgeImpl iterator is at end ( NULL in edge pointer)");
    impl.nextEdge();
    return *this;
}

/** Next pred */
template < class N, class E> void PredIterImpl< N, E>::nextEdge()
{
    GRAPH_ASSERTXD( isNotNullP( this->edge_p), "EdgeImpl iterator is at end ( NULL in edge_p pointer)");
    this->edge_p = this->edge_p->nextPred();
}


/** Next succ */
template < class N, class E> void SuccIterImpl< N, E>::nextEdge()
{
    GRAPH_ASSERTXD( isNotNullP( this->edge_p), "EdgeImpl iterator is at end ( NULL in edge_p pointer)");
    this->edge_p = this->edge_p->nextSucc();
}

/** Next edge in undirected edges traversal */
template < class N, class E> void UnDirIterImpl< N, E>::nextEdge()
{
    GRAPH_ASSERTXD( isNotNullP( this->edge_p), "EdgeImpl iterator is at end ( NULL in edge_p pointer)");
    
    if ( this->is_pred && isNullP( this->edge_p->nextPred()))
    {
        this->is_pred = false;
        this->edge_p = this->edge_p->succ()->firstSucc();
    } else
    {
        if ( this->is_pred)
        {
            this->edge_p = this->edge_p->nextPred();
        } else
        {
            this->edge_p = this->edge_p->nextSucc();
        }
    }
}

/** Get node on the end of edge */
template < class N, class E> N * SuccIterImpl< N, E>::node() const
{
    return this->edge()->succ();
}

/** Get node on the end of edge */
template < class N, class E> N * PredIterImpl< N, E>::node() const
{
    return this->edge()->pred();
}

/** Get node in UnDir traversal of node's edges */
template < class N, class E> N * UnDirIterImpl< N, E>::node() const
{
    if ( this->is_pred)
    {
        return this->edge()->pred();
    } else
    {
        return this->edge()->succ();
    }    
}

/** Constructor gets first succ */
template < class N, class E> SuccIterImpl< N, E>::SuccIterImpl( N *n)
{
    this->edge_p = n->firstSucc();
}

/** Constructor gets first pred */
template < class N, class E> PredIterImpl< N, E>::PredIterImpl( N *n)
{
    this->edge_p = n->firstPred();
}


/** Constructor gets first edge for undirected edges iteration */
template < class N, class E> UnDirIterImpl< N, E>::UnDirIterImpl( N *n)
{
    this->edge_p = n->firstPred();
    this->is_pred = true;
    if ( isNullP( this->edge_p)) 
    {
        this->is_pred = false;
        this->edge_p = n->firstSucc();
    } 
}

#endif /* NODE_INLINE_H */
