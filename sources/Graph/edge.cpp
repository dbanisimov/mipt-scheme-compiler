/**
 * @file: edge.cpp 
 * EdgeImpl class implementation
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * EdgeImpl destructor.
 * delete edge from GraphImpl's list of edges
 */
EdgeImpl::~EdgeImpl()
{
    graph()->detachEdge( this);
    detachFromNode( GRAPH_DIR_UP);
    detachFromNode( GRAPH_DIR_DOWN);
}

/**
 * Print edge in DOT format to stdout
 */
void
EdgeImpl::debugPrint()
{
    out("%llu->%llu;", pred()->id(), succ()->id());
}

/** NodeImpl checking routine */
bool EdgeImpl::checkNodes( NodeImpl* _pred, NodeImpl* _succ)
{
    return isNotNullP( _pred)
           && isNotNullP( _succ)
           && areEqP( this->graph(), _pred->graph())
           && areEqP( _pred->graph(), _succ->graph());
}
