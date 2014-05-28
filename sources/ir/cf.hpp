/**
 * @file:ir/cf.hpp
 * Declaration of the control flow representation classes
 * @defgroup CF Control flow
 * @ingroup IR
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{
    /**
     * @class CFEdge
     * @brief Control flow edge class
     * @ingroup CF
     */
    template <class MdesType> class CFEdge: 
        public Edge< CFG<MdesType>, CFNode<MdesType>, CFEdge<MdesType> >
    {
    public:
        /** Control flow edge constructor */
        inline CFEdge( CFG<MdesType> *g, CFNode<MdesType> *pred, CFNode<MdesType> *succ);
        inline Operation<MdesType> *srcOper() const;       //< Get control source operation
        inline void setSrcOper( Operation<MdesType> *oper);//< Set control source operation

        /** Print edge to stream */
        inline void toStream(ostream& os);

    private:
        Operation<MdesType> *src_oper_p;
    };

    /**
     * @class CFNode
     * @brief Control flow node
     * @ingroup CF
     */
    template <class MdesType> class CFNode:
        public Node< CFG<MdesType>, CFNode<MdesType>, CFEdge<MdesType> >
    {
    public:
        inline CFNode( CFG<MdesType> *g); //< Constructor of CF node

        inline Operation<MdesType> *firstOper() const;       //< Get first operation of the node
        inline void setFirstOper( Operation<MdesType> *oper);//< Set first operation of the node

        inline Operation<MdesType> *lastOper() const;        //< Get last operation of the node
        inline void setLastOper( Operation<MdesType> *oper); //< Set last operation of the node

        inline void append( Operation<MdesType> *oper); //< Insert given operation to the end of the node
        inline void prepend( Operation<MdesType> *oper);//< Insert given operation in the begining of the node

        /** Print node to stream */
        inline void toStream(ostream& os);

        inline bool isStart() const; //< Check that the given node is the start node of the CFG
        inline bool isStop() const;  //< Check that the given node is the stop node of the CFG
    private:
        Operation<MdesType> *first;
        Operation<MdesType> *last;
    };

    /**
     * @class CFG
     * @brief Control flow graph representation class
     * @ingroup CF
     */
    template <class MdesType> class CFG:
        public Graph< CFG<MdesType>, CFNode<MdesType>, CFEdge<MdesType> >
    {
    public:
        /** Default constructor */
        CFG();

        /** Set graph as valid (corresponds to operations in the function) */
        inline void setValid( bool valid);

        /** Check if graph is valid */
        inline bool isValid() const;

        /** Print CFG to stream */
        inline void toStream(ostream& os);

        inline CFNode<MdesType> *startNode() const; //< Get the start node of the CFG
        inline CFNode<MdesType> *stopNode() const;  //< Get the stop node of the CFG

        /** Number nodes in topological order. Nodes that are not reachable from start will have arbitrary order */
        Numeration makeTopologicalNumeration();

    private:
        bool is_valid;

        /* Pseudo nodes on top and on bottom of graph */
        CFNode<MdesType> *start_node;
        CFNode<MdesType> *stop_node;
    };

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, CFEdge<MdesType> &e) 
{   
    e.toStream(os);
    return os;
} 

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, CFNode<MdesType> &n) 
{   
    n.toStream(os);
    return os;
} 

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, CFG<MdesType> &cfg) 
{   
    cfg.toStream(os);
    return os;
} 

};
