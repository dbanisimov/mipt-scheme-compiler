/**
 * @file: GraphImpl.cpp
 * GraphImpl class implementation
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#include "graph_iface.h"

/**
 * Constructor.
 */
GraphImpl::GraphImpl():
    node_next_id( 0),
    edge_next_id( 0),
    node_num( 0),
    edge_num( 0),
    first_node( NULL),
    first_edge( NULL)
{

}

/**
 * Destructor - doesn't remove all nodes since GraphImpl is not really the owner of nodes and edge
 */
GraphImpl::~GraphImpl()
{

}

/**
 * Print GraphImpl to stdout in DOT format.
 * Note: Iterates through nodes and edges separately instead
 *       of iterating through nodes and at iterating through edges of each node
 */
void 
GraphImpl::debugPrint()
{
    NodeImpl *n;
    EdgeImpl *e;
    out( "diGraphImpl{");
    /** Print nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        n->debugPrint();
    }
    /** Print edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        e->debugPrint();
    }
    out( "}");
}

/**
 * Implementation for numerations cleanup
 */
void 
GraphImpl::clearNumerationsInObjects()
{
    NodeImpl *n;
    EdgeImpl *e;
    /** Clean markers in nodes */
    for ( n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedNumerations( n);
    }
    /** Clean markers in edges */
    for ( e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedNumerations( e);
    }
}

/**
 * Implementation for markers cleanup
 */
void 
GraphImpl::clearMarkersInObjects()
{
    NodeImpl *n;
    EdgeImpl *e;
    /** Clean markers in nodes */
    for (  n = firstNode(); isNotNullP( n); n = n->nextNode())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
    /** Clean markers in edges */
    for (  e = firstEdge(); isNotNullP( e); e = e->nextEdge())
    {
        clearUnusedMarkers( static_cast<Marked *>(n));
    }
}
