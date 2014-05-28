/**
 * @file: GRAPH_utest.cpp 
 * Implementation of testing of Graph library
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "graph_iface.h"

#include <boost/array.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/graph/adjacency_list.hpp>

static const int GRAPH_TEST_NUM_NODES = 100000;


BOOST_AUTO_TEST_SUITE( graph)

 /**
  * TODO: Check Graph's data structures being consistent with node and edge functionality
  */
bool uTestGraphImplOwn()
{
    return true;
}

BOOST_AUTO_TEST_CASE( data_structures)
{
    {
        AGraph graph;
        ANode *dummy = graph.newNode();
        graph.deleteNode( dummy);
        ANode *pred = graph.newNode();
        ANode *succ = graph.newNode();
        AEdge *edge = graph.newEdge( pred, succ);
        
        /** Check node insertion */
        ANode *new_node = edge->insertNode();
        AEdge *edge2 = new_node->firstSucc();
        BOOST_CHECK( areEqP( new_node->firstPred(), pred->firstSucc())); 
        BOOST_CHECK( areEqP( new_node->firstSucc(), succ->firstPred()));
        BOOST_CHECK( areEqP( edge->pred(), pred));
        BOOST_CHECK( areEqP( pred->firstSucc(), edge));
        BOOST_CHECK( areEqP( edge->succ(), new_node));
        BOOST_CHECK( areEqP( new_node->firstPred(), edge));
        BOOST_CHECK( areEqP( edge2->pred(), new_node));
        BOOST_CHECK( areEqP( edge2->succ(), succ));
        BOOST_CHECK( areEqP( succ->firstPred(), edge2));
    }

    /** Test iterators */
    {
        AGraph graph;
        ANode *node1 = graph.newNode();
        ANode *node2 = graph.newNode();
        ANode *node3 = graph.newNode();
        AEdge *edge1 = graph.newEdge( node1, node2);
        AEdge *edge2 = graph.newEdge( node2, node3);
        
        for ( ANode::Succ succ_iter = node2->succsBegin(),
                         succ_iter_end = node2->succsEnd();
              succ_iter != succ_iter_end;
              ++succ_iter )
        {
            BOOST_CHECK( areEqP( *succ_iter, edge2));
        }
        for ( ANode::Pred pred_iter = node2->predsBegin(),
                         pred_iter_end = node2->predsEnd();
              pred_iter != pred_iter_end;
              pred_iter++ )
        {
            BOOST_CHECK( areEqP( *pred_iter, edge1));
        }
        ANode::EdgeIter edge_iter = node2->edgesBegin();
        ANode::EdgeIter edge_iter_end = node2->edgesEnd();
        BOOST_CHECK( edge_iter != edge_iter_end);
        BOOST_CHECK( areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            BOOST_CHECK( areEqP( edge_iter.node(), edge1->pred()));
            BOOST_CHECK( areEqP( edge_iter.node(), node1));
        } else
        {
            BOOST_CHECK( areEqP( edge_iter.node(), edge2->succ()));
            BOOST_CHECK( areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        BOOST_CHECK( edge_iter != edge_iter_end);
        BOOST_CHECK( areEqP( *edge_iter, edge1) || areEqP( *edge_iter, edge2));
        if ( areEqP( *edge_iter, edge1))
        {
            BOOST_CHECK( areEqP( edge_iter.node(), edge1->pred()));
            BOOST_CHECK( areEqP( edge_iter.node(), node1));
        } else
        {
            BOOST_CHECK( areEqP( edge_iter.node(), edge2->succ()));
            BOOST_CHECK( areEqP( edge_iter.node(), node3));
        }
        edge_iter++;
        BOOST_CHECK( edge_iter == edge_iter_end);
    }
}



struct DummyNodeProps
{
    int i;
};

struct DummyEdgeProps
{
    int i;
};

/* Graph description */
typedef boost::adjacency_list<
    boost::listS, boost::listS, boost::bidirectionalS,
    DummyNodeProps, DummyEdgeProps>
  BGraph;

typedef BGraph::vertex_descriptor BNode;
typedef BGraph::edge_descriptor BEdge;

TIMED_AUTO_TEST_CASE( perf_boost_graph)
{
    BGraph graph;
    boost::array< BNode, GRAPH_TEST_NUM_NODES> nodes;
    
    /* Create some nodes and edges */
    for ( int i = 0; i < GRAPH_TEST_NUM_NODES; i++)
    {
        nodes[ i] = boost::add_vertex( graph);    
        if ( i > 10)
        {
            boost::add_edge( nodes[ i -1], nodes[ i], graph);
        }
        graph[ nodes[i] ].i = 10;
    }
    /* Remove nodes and edges */
    for ( int i = 0; i < GRAPH_TEST_NUM_NODES; i++)
    {
        boost::remove_vertex( nodes[ i], graph);
    }
}

TIMED_AUTO_TEST_CASE( perf_my_graph)
{
    AGraph graph;
    boost::array< ANode *, GRAPH_TEST_NUM_NODES> nodes;
    
    /* Create some nodes and edges */
    for ( int i = 0; i < GRAPH_TEST_NUM_NODES; i++)
    {
        nodes[ i] = graph.newNode();
        if ( i > 10)
        {
            graph.newEdge( nodes[ i -1], nodes[ i]);
        }
        nodes[ i]->dummy = 10;
    }
    /* Remove nodes and edges */
    for ( int i = 0; i < GRAPH_TEST_NUM_NODES; i++)
    {
        graph.deleteNode( nodes[ i]);
    }
}

/**
 * TODO: Check consistency of NodeImpl and EdgeImpl classes interoperation
 */
bool uTestNodeEdge() { return true;}
BOOST_AUTO_TEST_CASE( nodes_and_edges)
{
    /**
     * TODO: Check that node and edge remain correct after basic edge/node creation/deletion
     */
    /**
     * 
     */
}

/**
 * Check marker functionality
 */
bool uTestMarkers(){ return true;}
BOOST_AUTO_TEST_CASE( markers)
{
    AGraph graph;
    ANode *dummy = graph.newNode();
    graph.deleteNode( dummy);
    ANode *pred = graph.newNode();
    ANode *succ = graph.newNode();
    AEdge *edge = graph.newEdge( pred, succ);
    Marker m = graph.newMarker();
    Marker m2 = graph.newMarker();

    Marker m_array[ MAX_GRAPH_MARKERS];
    
    BOOST_CHECK( !pred->isMarked( m));
    BOOST_CHECK( !succ->isMarked( m));
    BOOST_CHECK( !edge->isMarked( m));
    BOOST_CHECK( !pred->isMarked( m2));
    
    pred->mark( m);
    succ->mark( m);
    edge->mark( m);
    edge->mark( m2);

    BOOST_CHECK( pred->isMarked( m));
    BOOST_CHECK( succ->isMarked( m));
    BOOST_CHECK( edge->isMarked( m));
    BOOST_CHECK( edge->isMarked( m2));
    edge->unmark( m);

    /** Check that different markers have different behaviour */
    BOOST_CHECK( edge->isMarked( m2));
    BOOST_CHECK( !edge->isMarked( m));
    
    graph.freeMarker( m);
    graph.freeMarker( m2);
    
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        m_array [ i] = graph.newMarker();
    }
    for ( MarkerIndex i = 0; i < MAX_GRAPH_MARKERS; i++)
    {
        graph.freeMarker( m_array[ i]);
    }
    m = graph.newMarker();
    graph.freeMarker( m);
    
    ANode *n;
    for (  n = graph.firstNode(); isNotNullP( n);)
    {
        ANode *tmp = n;
        n = n->nextNode();
        graph.deleteNode( tmp);
    }
}

/**
 * Check marker functionality
 */
static bool uTestNumerations(){ return true;}
BOOST_AUTO_TEST_CASE( numerations)
{
    /** 
     * Every class that can be a numerations manager should implement
     * the routine for clearing numerations in objects
     */
    class NumMgrInst: public NumManager
    {
        /** Implementation of clearing - empty TODO: implement and test it */
        void clearNumerationsInObjects()
        {
        
        }
    };
    /**
     * Check correct error reporting
     *  1. Too many numerations
     */
    NumMgrInst mgr1;
    try
    {
        for ( int i = 0; i < MAX_NUMERATIONS + 1; i++)
        {
            mgr1.newNum();
        }
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        BOOST_CHECK( error == NUM_ERROR_OUT_OF_INDEXES);
    }

    /** 2. Too big number */
    NumMgrInst mgr2;
    Numeration num2 = mgr2.newNum();
    Numbered obj2; 
    try
    {
        obj2.setNumber( num2, -1);
    } catch ( NumErrorType error)
    {
        // thrown error type MUST match the expected one
        BOOST_CHECK( error == NUM_ERROR_NUMBER_OUT_OF_RANGE);
    }
    mgr2.freeNum( num2);

    /** 3. Functional testing */
    NumMgrInst mgr;
    for ( int i = 0; i < MAX_NUMERATIONS + 2; i++)
    {
        Numeration n = mgr.newNum();
        mgr.freeNum( n);
    } 
    Numeration num = mgr.newNum();
    Numeration num_unused = mgr.newNum();
    Numbered obj; 
    BOOST_CHECK( obj.number( num) == NUMBER_NO_NUM);
    BOOST_CHECK( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.setNumber( num, 1);
    BOOST_CHECK( obj.isNumbered( num));
    BOOST_CHECK( obj.number( num) == 1);
    BOOST_CHECK( obj.number( num_unused) == NUMBER_NO_NUM);
    obj.unNumber( num);
    BOOST_CHECK( obj.number( num) == NUMBER_NO_NUM);
    BOOST_CHECK( obj.number( num_unused) == NUMBER_NO_NUM);    
}

#if 0
/**
 * Create Graph and save it to XML
 */
bool uTestSave(){ return true;}
BOOST_AUTO_TEST_CASE( xml)
{
    AGraph graph;

    /** 
     *  Check basic operation of graph library
     */
    boost::array<ANode *, 20> nodes;

    /** Create nodes and edges */
    for ( int i =0; i<20; i++)
    {
        nodes[i] = graph.newNode();
        if ( i > 0)
        {
            graph.newEdge( nodes[ i - 1], nodes[ i]);
        }
        if ( i > 1 && i % 2 == 0)
        {
            graph.newEdge( nodes[ i - 2], nodes[ i]);
        }
    }
    graph.newEdge( nodes[ 8], nodes[ 4]);
    graph.deleteNode( nodes[ 8]);
    graph.debugPrint();
}

#endif

BOOST_AUTO_TEST_SUITE_END()

#if 0
/**
 * Unit tests for Graph library
 */
bool uTestGraph()
{
    /**
     * Check Graph's data structures consistency
     */
     if ( !uTestGraphImplOwn())
         return false;
    /**
     * Check node-edge consistency
     */
    if ( !uTestNodeEdge())
         return false;

    /**
     * Check numerations 
     */
    if ( !uTestNumerations())
        return false;

    /**
     * Check markers
     */
    if ( !uTestMarkers())
        return false;

    //GRAPH_ASSERTXD( 0, "Generic failing assertion");
    return true;
}
#endif

