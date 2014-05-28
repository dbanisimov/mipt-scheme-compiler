/**
 * @file: aGraphImpl.h 
 * Abstract GraphImpl for testing of GraphImpl's properties and usage model.
 *
 * @defgroup AGr Test GraphImpl
 *
 * @ingroup GraphBase
 * AGraph, ANode and AEdge classes present mimnimal code
 * that you need to write to employ GraphImpl Library functionality.
 * AGraph classes differ from base in only one member of ( int) type
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef AGraph_H
#define AGraph_H

/* Predeclarations */
class ANode;
class AEdge;
class AGraph;

/**
 * Abstract node
 *
 * @ingroup AGr
 */
class ANode: public Node< AGraph, ANode, AEdge>
{
public:
    int dummy;
    
    ANode( AGraph* g): Node< AGraph, ANode, AEdge>( g){};

    /** Get next GraphImpl's node */
    inline ANode* nextNode()
    {
        return static_cast< ANode*>( NodeImpl::nextNode());
    }
    /** Get prev GraphImpl's node */
    inline ANode* prevNode()
    {
        return static_cast< ANode*>( NodeImpl::prevNode());
    }
    /** EdgeImpl connection reimplementation */
    inline void AddEdgeInDir( AEdge *edge, GraphDir dir);
    /** Add predecessor */
    inline void AddPred( AEdge *edge);
    /** Add successor */
    inline void AddSucc( AEdge *edge);
    /** Get first edge in given direction */
    inline AEdge* firstEdgeInDir( GraphDir dir);
    /** Get first successor */
    inline AEdge* firstSucc();
    /** Get first predecessor */
    inline AEdge* firstPred();

};

/**
 * Abstract edge
 *
 * @ingroup AGr
 */
class AEdge: public Edge< AGraph, ANode, AEdge>
{
    int dummy;

public:
    
    AEdge( AGraph *g, ANode *pred, ANode *succ): Edge< AGraph, ANode, AEdge>( g, pred, succ)
    {
        
    }

    /** Get node in given direction */
    inline ANode *node( GraphDir dir) const
    {
        return static_cast< ANode *>( EdgeImpl::node( dir));
    }
    /** Get predecessor */
    inline ANode *pred() const 
    {
        return node( GRAPH_DIR_UP);
    }
    /** Get successor */
    inline ANode *succ() const 
    {
        return node( GRAPH_DIR_DOWN);
    }  
    /** Next edge in GraphImpl's list */
    inline AEdge* nextEdge()
    {
        return static_cast< AEdge *>( EdgeImpl::nextEdge());
    }
    /** Next edge in give direction */
    inline AEdge* nextEdgeInDir( GraphDir dir)
    {
        return static_cast< AEdge *>( EdgeImpl::nextEdgeInDir( dir));
    }
    /** Next successor */
    inline AEdge* nextSucc()
    {
        return nextEdgeInDir( GRAPH_DIR_DOWN);
    }
    /** Next predecessor */
    inline AEdge* nextPred()
    {
        return nextEdgeInDir( GRAPH_DIR_UP);
    } 
};

/**
 * Testing-purpose GraphImpl
 *
 * @ingroup AGr
 */
class AGraph: public Graph< AGraph, ANode, AEdge>
{
    int dummy; //Dummy class member

public:
            
    /** Get GraphImpl's first edge */
    inline AEdge* firstEdge() 
    {
        return static_cast< AEdge *>( GraphImpl::firstEdge());
    }
    /** Get GraphImpl's first node */
    inline ANode* firstNode()
    {
        return static_cast< ANode *>( GraphImpl::firstNode());
    }
};

/** Get first edge in given direction */
inline AEdge*
ANode::firstEdgeInDir( GraphDir dir)
{
    return static_cast< AEdge*>( NodeImpl::firstEdgeInDir( dir));
}
/** Get first successor */
inline AEdge*
ANode::firstSucc()
{
    return firstEdgeInDir( GRAPH_DIR_DOWN);
}
/** Get first predecessor */
inline AEdge*
ANode::firstPred()
{
    return firstEdgeInDir( GRAPH_DIR_UP);
}

/** EdgeImpl connection reimplementation */
inline void
ANode::AddEdgeInDir( AEdge *edge, GraphDir dir)
{
    NodeImpl::AddEdgeInDir( edge, dir);
}
/** Add predecessor */
inline void
ANode::AddPred( AEdge *edge)
{
    AddEdgeInDir( edge, GRAPH_DIR_UP);
}
/** Add successor */
inline void
ANode::AddSucc( AEdge *edge) 
{
    AddEdgeInDir( edge, GRAPH_DIR_DOWN);
}
#endif
