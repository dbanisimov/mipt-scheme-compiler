/**
 * @file:ir/cf_inline.hpp
 * Implementation of the inlined functions of control flow representation classes
 */

#pragma once

namespace IR
{
    
/** Get control source operation */
template <class MdesType>
Operation<MdesType> *
CFEdge<MdesType>::srcOper() const
{
    return src_oper_p;
}
/** Set control source operation */
template <class MdesType>
void 
CFEdge<MdesType>::setSrcOper( Operation<MdesType> *oper)
{
    src_oper_p = oper;
}
/** Control Flow Edge constructor */
template <class MdesType> 
CFEdge<MdesType>::CFEdge( CFG<MdesType> *g, CFNode<MdesType> *pred, CFNode<MdesType> *succ):
    Edge< CFG<MdesType>, CFNode<MdesType>, CFEdge<MdesType> >( g, pred, succ),
    src_oper_p( NULL)
{
    
}

/** Control Flow node constructor */
template <class MdesType> CFNode<MdesType>::CFNode( CFG<MdesType> *g):
    Node< CFG<MdesType>, CFNode<MdesType>, CFEdge<MdesType> >( g), first( NULL), last( NULL)
{
    
}

/** Default CFG constructor */
template <class MdesType> CFG<MdesType>::CFG():
    Graph< CFG<MdesType>, CFNode<MdesType>, CFEdge<MdesType> >(),
    is_valid( false)
{
    start_node = this->newNode();
    stop_node = this->newNode();
}

/** Set graph as valid (corresponds to operations in the function) */
template <class MdesType>
void
CFG<MdesType>::setValid( bool valid)
{
    is_valid = valid;
}

// Get the start node of the CFG
template <class MdesType>
CFNode<MdesType> *
CFG<MdesType>::startNode() const
{
    return start_node;
}
    
// Get the stop node of the CFG
template <class MdesType>
CFNode<MdesType> *
CFG<MdesType>::stopNode() const
{
    return stop_node;
}

/** Check if graph is valid */
template <class MdesType>
bool
CFG<MdesType>::isValid() const
{
    return is_valid;
}

// Get first operation of the node
template <class MdesType>
Operation<MdesType> *
CFNode<MdesType>::firstOper() const
{
    return first;
}
    
// Set first operation of the node
template <class MdesType>
void
CFNode<MdesType>::setFirstOper( Operation<MdesType> *oper)
{
    first = oper;
}

// Get last operation of the node
template <class MdesType>
Operation<MdesType> *
CFNode<MdesType>::lastOper() const
{
    return last;
}
    
// Set last operation of the node      
template <class MdesType>
void
CFNode<MdesType>::setLastOper( Operation<MdesType> *oper)
{
    last = oper;
}

// Insert given operation to the end of the node
template <class MdesType>
void
CFNode<MdesType>::append( Operation<MdesType> *oper)
{
    IR_ASSERTD( isNotNullP( oper));

    if ( isNullP( last))
    {
        IR_ASSERTD( isNullP( first)); // Node must be empty if there is no last operation
        oper->detachFromSeq();
        last = oper;
        first = oper;
    } else
    {
        oper->insertAfter( last);
        last = oper;
    }
    oper->setNode( this);
}
    
// Insert given operation in the begining of the node 
template <class MdesType>
void 
CFNode<MdesType>::prepend( Operation<MdesType> *oper)
{
    IR_ASSERTD( isNotNullP( oper));

    if ( isNullP( first))
    {
        IR_ASSERTD( isNullP( last)); // Node must be empty if there is no last operation
        oper->detachFromSeq();
        first = oper;
        last = oper;
    } else
    {
        oper->insertBefore( first);
        first = oper;
    }
    oper->setNode( this);
}

/** Number nodes in topological order. Nodes that are not reachable from start will have arbitrary order */
template <class MdesType>
Numeration
CFG<MdesType>::makeTopologicalNumeration()
{
    /**
     * The algorithm makes an RPO numeration of nodes by 
     * doing the depth-first search on the CFG and assigning numbers
     * to a node AFTER all successors of the node have been visited.
     */
    Marker m = MarkerManager::newMarker();
    Numeration num = NumManager::newNum();
    GraphNum i = this->numNodes() - 1;
    
    /* 
     * We use while loop instead of recursive implementation just to show off :)
     * So we will need a stack for saving temporary values. We save the edge to
     * be processed on the stack. If the edge is last unprocessed successor of some node
     * than node is considered done and a number is assigned to it.
     */
    std::list< CFEdge< MdesType> *> stack;
    
    if ( isNotNullP( this->startNode()->firstSucc()) )
    {
        stack.push_back( this->startNode()->firstSucc());
        this->startNode()->mark( m);
        this->startNode()->firstSucc()->succ()->mark( m);

        while ( stack.size() != 0)
        {
            CFEdge< MdesType> *edge = stack.back();
            
            /*
             * Add next succ to the stack, if it needs to be processed
             */
            if ( isNotNullP( edge->nextSucc()) )
            {
                CFEdge< MdesType> *succ = edge->nextSucc();
                stack.pop_back();
                while ( isNotNullP( succ))
                {
                    if ( !succ->succ()->isMarked( m))
                    {
                        break;
                    } else if ( isNullP( succ->nextSucc()) )
                    {
                        succ->mark( m);
                        break;
                    }
                    succ = succ->nextSucc();
                }
                if ( isNotNullP( succ))
                {
                    stack.push_back( edge->nextSucc());
                    edge->nextSucc()->succ()->mark( m);
                }
            }
            
            /*
             * Check if the predecessor node is done. I.e. the current edge 
             * is already processed and it is the last successor edge
             */
            if ( edge->isMarked( m) && isNullP( edge->nextSucc()) )
            {
                CFNode< MdesType> *pred = edge->pred();
                pred->setNumber( num, i);
                IR_ASSERTD( pred->isMarked( m));
                //cout << "RPO trav " << pred->id() << " is numbered " << i << endl;
                i--;
                stack.pop_back();
                continue;
            }

            /** Add all successor edges of successor node to the stack */
            bool succ_found = false;
            CFEdge< MdesType> *succ = edge->succ()->firstSucc();
            while ( isNotNullP( succ))
            {
                CFNode< MdesType> *succ_node = succ->succ();
                if ( !succ_node->isMarked( m) )
                {
                    succ_node->mark( m);
                    stack.push_back( succ);
                    succ_found = true;
                    break;
                }
                succ = succ->nextSucc();
            }
            if ( !succ_found)
            {
                CFNode< MdesType> * succ = edge->succ();
                succ->setNumber( num, i);
                IR_ASSERTD( succ->isMarked( m));
                //cout << "RPO trav " << succ->id() << " is numbered " << i << endl;
                i--;
            }
            edge->mark( m);
        }
    }

    /* Number the rest of CFG nodes */
    for ( CFNode< MdesType> *node = this->firstNode();
          isNotNullP( node);
          node = node->nextNode() )
    {
        //cout << "Checking " << node->id() << " " << ( node->isMarked( m)? "marked": "non-marked") << endl;
        if ( !node->isMarked( m) )
        {
            node->setNumber( num, i);
            //cout << "RPO trav " << node->id() << " is numbered " << i << endl;
            i--;    
        }
    }

    this->freeMarker( m);
    
    return num;
}

// Check that the given node is the start node of the CFG
template <class MdesType>
bool
CFNode<MdesType>::isStart() const
{
    return areEqP( this->graph()->startNode(), this);
}

// Check that the given node is the stop node of the CFG
template <class MdesType>
bool
CFNode<MdesType>::isStop() const
{
    return areEqP( this->graph()->stopNode(), this);
}


template <class MdesType>
void
CFEdge< MdesType>::toStream(ostream& os)
{
    
    os << "CF Edge " << this->id() << ": ";

    os << this->pred()->id() << "->" << this->succ()->id();

    Operation< MdesType> *src = this->srcOper();
    if ( isNotNullP( src) )
    {
        os << ", src: " << src->id() << " "<< src->nameStr();
    } else
    {
        os << ", fallthrough";
    }
    os << endl;
}

    
template <class MdesType>
void
CFNode< MdesType>::toStream(ostream& os)
{
    /* Begin with node header */
    os << "CF Node " << this->id();
    
    /* Print node start/stop property */
    if ( this->isStart() )
    {
        os << " Start";
    } else if ( this->isStop() )
    {
        os << " Stop";
    }
    /* Node header done */
    os << endl;

    /* Iterating over predecessors */
    for ( typename CFNode< MdesType>::Pred
            pred_iter = this->predsBegin(),
            pred_iter_end = this->predsEnd();
          pred_iter != pred_iter_end;
          pred_iter++ )
    {
         CFEdge< MdesType> *edge = *pred_iter;
         os << *edge;
    }

    /* Print all operations */
    for ( Operation< MdesType> * oper = firstOper();
          isNotNullP( oper);
          oper = oper->nextOper() )
    {
        os << oper;
        
        if ( areEqP( oper, lastOper()) )
            break;
    }

    /* Iterating over successors */
    for ( typename CFNode< MdesType>::Succ
              succ_iter = this->succsBegin(),
              succ_iter_end = this->succsEnd();
          succ_iter != succ_iter_end;
          ++succ_iter )
    {
         CFEdge< MdesType> *edge = *succ_iter;
         os << *edge;        
    }
}

template <class MdesType>
void
CFG< MdesType>::toStream(ostream& os)
{
    Numeration num = makeTopologicalNumeration();
    std::vector< CFNode< MdesType> *> nodes( this->numNodes());

    /* Fill array of node pointers */
    for ( CFNode< MdesType> *node = this->firstNode();
          isNotNullP( node);
          node = node->nextNode() )
    {
        IR_ASSERTD( node->isNumbered( num));
        nodes[ node->number( num)] = node;
    }

    /* Print reachable nodes */
    for (  UInt32 i = startNode()->number( num);
           i < this->numNodes();
           ++i)
    {
        CFNode< MdesType> *node = nodes[ i];
        os << *node << endl;
    }

    for (  UInt32 i = 0, end = startNode()->number( num);
           i < end;
           ++i)
    {
        CFNode< MdesType> *node = nodes[ i];
        os << "unreachable " << *node << endl;
    }
    this->freeNum( num);
}

}; // namespace IR