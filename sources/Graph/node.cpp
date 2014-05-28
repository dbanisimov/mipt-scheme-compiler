/**
 * @file: node.cpp
 * NodeImpl class implementation
 */
/*
 * GraphImpl library, internal representation of GraphImpls in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
/** 
 * Destructor. Corrects list of nodes in corresponding GraphImpl and deletes adjacent edges
 */
#include "graph_iface.h"

NodeImpl::~NodeImpl()
{
    GRAPH_ASSERTXD( isNullP(firstSucc()), "Deleted node still has successor edge(s)");
    GRAPH_ASSERTXD( isNullP(firstPred()), "Deleted node still has successor edge(s)");

    /** delete myself from GraphImpl */
    graph()->detachNode( this);
}

/**
 * Print node in Dot format to stdout
 */
void
NodeImpl::debugPrint()
{
    out("%llu;", id());
}
