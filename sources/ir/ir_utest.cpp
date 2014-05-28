/**
 * @file ir/ir_utest.cpp
 * Unit testing of IR
 */
#include "ir_iface.hpp"
#include <boost/test/unit_test.hpp>

using namespace IR;
using namespace Opt;

BOOST_AUTO_TEST_SUITE( IR)

typedef Module< OptMdes> OptModule;
typedef Function< OptMdes> OptFunction;
typedef CFG< OptMdes> OptCFG;
typedef CFNode< OptMdes> OptCFNode;
typedef CFEdge< OptMdes> OptCFEdge;
typedef Operation< OptMdes> OptOper;
typedef Object< OptMdes> OptObj;


BOOST_AUTO_TEST_CASE( ir_basic)
{
    OptModule m;
    OptFunction *f = m.newFunction();
    OptCFG & cfg = f->cfg();
    OptCFNode *node1 = cfg.newNode();
    OptCFNode *node2 = cfg.newNode();
    OptCFNode *node3 = cfg.newNode();
    cfg.newEdge( cfg.startNode(), node1);
    cfg.newEdge( node3, cfg.stopNode());

    OptObj* v1 = f->newObject( Reg);
    OptObj* v2 = f->newObject( Reg);
    OptObj* v3 = f->newObject( Reg);
    OptObj* v4 = f->newObject( Reg);
    
    /* {0} add vt2 = vt0, vt1*/
    OptOper *add = f->newOper( Add);
    add->setArgObj( 0, v1);
    add->setArgObj( 1, v2);
    add->setResObj( 0, v3);
    f->setFirstOper( add);
    node1->append( add);
    
    /* {1} brg vt2, 512, <label_4> */
    OptOper *brg = f->newOper( Brg);
    brg->setArgObj( 0, v3);
    brg->setArgImm( 1, (Int64) 0x200);
    node1->append( brg);
    OptCFEdge *edge1 = cfg.newEdge( node1, node2);
    OptCFEdge *edge2 = cfg.newEdge( node1, node3);
    edge2->setSrcOper( brg);

    /* {2} sub vt3 = vt2, 512 */
    OptOper *sub = f->newOper( Sub);
    sub->setArgObj( 0, v3);
    sub->setArgImm( 1, (Int64) 0x200);
    sub->setResObj( 0, v4);
    node2->append( sub);
    brg->OperSeqList::attach( sub); // to preserve the function's operation chain
    
        
    /* {3} call unknown_routine, vt3 */
    OptOper *call = f->newOper( Call);
    call->setArgImm( 0, string("unknown_routine"));
    call->addArgToSet( v4);
    call->addArgToSet( v3);
    call->addResToSet( v4);
    node2->append( call);
    cfg.newEdge( node2, node3);

    /* {4} st 74565, vt3 */
    OptOper *st = f->newOper( St);
    brg->setArgTarget( 2, st);
    st->setArgImm( 0, (Int64) 0x12345);
    st->setArgObj( 1, v4);
    node3->append( st);
    call->OperSeqList::attach( st); // to preserve the function's operation chain

    OptOper *ret = f->newOper( Ret);
    node3->append( ret);

    /* Unit test of store operation representation */
    OptMdes::OperDes *st_des = OptMdes::operDescription( st->name());

    BOOST_CHECK( st_des->num_args == 2);
    BOOST_CHECK( st_des->num_ress == 0);

    OptCFNode* cf_node = cfg.newNode();
    
    /* Iterating over CFG nodes */
    for ( OptCFNode *node = cfg.firstNode();
          isNotNullP( node);
          node = node->nextNode())
    {
        BOOST_CHECK( isNotNullP( node->graph()) );
    }

    cout << "Operations as they go in linear order" << endl;
    cout << f; // Print the whole function

    cfg.setValid( true);

    cout << "Control flow graph print" << endl;
    cout << f; // After CFG is valid the 'operator <<' should print the whole CFG

    /* Unit test of operation representation */
    OptMdes::OperDes *des = OptMdes::operDescription( Add);

    BOOST_CHECK( des->num_args == 2);
    BOOST_CHECK( des->num_ress == 1);
}

BOOST_AUTO_TEST_SUITE_END()
