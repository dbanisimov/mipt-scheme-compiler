/**
 * @file: GRAPH_inline.h
 * Implementation of GraphImpl's inline routines
 */
/*
 * GraphImpl library, internal representation of GraphImpl in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once
#ifndef GRAPH_INLINE_H
#define GRAPH_INLINE_H

/**
 * Remove node from node list of GraphImpl
 */
inline void GraphImpl::detachNode( NodeImpl* node)
{
    GRAPH_ASSERTD( isNotNullP( node));
    GRAPH_ASSERTD( node->graph() == this);

    if( first_node == node)
    {
       first_node = node->nextNode();
    }
    node->detachFromGraph();
    node_num--;
}

/**
 * Remove edge from edge list of GraphImpl
 */
inline void GraphImpl::detachEdge( EdgeImpl * edge)
{
    GRAPH_ASSERTD( isNotNullP( edge));
    GRAPH_ASSERTD( edge->graph() == this);

    if( first_edge == edge)
    {
       first_edge = edge->nextEdge();
    }
    edge->detachFromGraph();
    edge_num--;
}

/**
 * Return node quantity
 */
inline GraphNum GraphImpl::numNodes() const
{
    return node_num;
}

/**
 * Return edge quantity
 */
inline GraphNum GraphImpl::numEdges() const
{
    return edge_num;
}
/** 
 * Get first edge
 */
inline EdgeImpl* GraphImpl::firstEdge() 
{
    return first_edge;
}

/**
 * Get first node
 */
inline NodeImpl* GraphImpl::firstNode()
{
    return first_node;
}

/**
 * Add node to the graph
 */
inline void 
GraphImpl::addNode( NodeImpl *node_p)
{
    /** Check that we have available node id */
    GRAPH_ASSERTXD( node_next_id < GRAPH_MAX_NODE_NUM, "We're out of node identificators");
    
    /** Add node to GraphImpl's list of nodes */
    node_p->attach( first_node);
    first_node = node_p;
    
    ++node_num;
    ++node_next_id;
}

/**
 * Add edge to the graph
 */
inline void 
GraphImpl::addEdge( EdgeImpl *edge_p)
{
    /**
     * Check that we have available edge id 
     */
    GRAPH_ASSERTXD( edge_next_id < GRAPH_MAX_EDGE_NUM, "We're out of edge identificators");
    edge_p->attach( EDGE_LIST_GRAPH, first_edge);
    first_edge = edge_p;
    ++edge_num;
    ++edge_next_id;
}

/* Constructor */
template < class G, class N, class E> Graph< G, N, E>::Graph()
{

};

/** NodeImpl creation overload */
template < class G, class N, class E> 
    N * Graph< G, N, E>::newNode()
{
    N* node = new ( &node_pool) N( static_cast< G*>(this));
    addNode( node);
    return node;
}

/** EdgeImpl creation overload */
template < class G, class N, class E> 
    E * Graph< G, N, E>::newEdge( N *pred, N* succ)
{
    E *edge = new ( &edge_pool) E( static_cast< G*>(this), pred, succ);
    addEdge( edge);
    return edge;
} 

/** Delete node from memory pool */
template < class G, class N, class E> 
    void Graph< G, N, E>::deleteNode( N *n)
{
    node_pool.destroy( n);
}

/** Delete edge from memory pool */
template < class G, class N, class E> 
    void Graph< G, N, E>::deleteEdge( E *e)
{
    edge_pool.destroy( e);
}

/** Get first edge */
template < class G, class N, class E>
E*
Graph< G, N, E>::firstEdge()
{
    return static_cast< E*>( GraphImpl::firstEdge());
}

/** Get first node */
template < class G, class N, class E>
N*
Graph< G, N, E>::firstNode()
{
    return static_cast< N*>( GraphImpl::firstNode());
}


template < class G, class N, class E> 
Graph< G, N, E>::~Graph()
{
    for ( N *node = firstNode();
            isNotNullP( node);)
    {
        N* next = node->nextNode();
        deleteNode( node);
        node = next;
    }
}

#endif /** GRAPH_INLINE_H */
