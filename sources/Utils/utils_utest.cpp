/**
 * @file: utils_utest.cpp 
 * Implementation of testing of utils
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utils_iface.h"
#include <boost/test/unit_test.hpp>

/** Sample class */
class A
{
    int a;
    /** private constructors, assignment and destructor */
    A(){};
    A( const A&){};
    A& operator =( const A&){};
    ~A(){};
    /** Needed for singleton creation */
    friend class Single< A>;
};

/** Typedef for A encapsulated in singleton */
typedef Single< A> SingleA;

BOOST_AUTO_TEST_SUITE( simple_utils)
/**
 * Test simple singleton
 */
BOOST_AUTO_TEST_CASE( singleton)
{
    SingleA::init();

    A* a1 = SingleA::instance();
    A* a2 = SingleA::instance();
    BOOST_CHECK( areEqP( a1, a2));
    SingleA::deinit();
}

#if 0
namespace UTestFinal
{
    class Base: Final< Base>
    {

    };

    class Derived: public Base
    {};
}
#endif

/**
 * Various tests
 */
BOOST_AUTO_TEST_CASE( misc)
{
#if 0 
    // We can create objects of the class Base
    UTestFinal::Base b;
    
    // This should trigger compiler error saying that the 
    // FinalHook<Base>() constructor is not accessible from Derived 
    UTestFinal::Derived *d = new UTestFinal::Derived(); // <-- compilation error (C2248 in MSVC 10) 
#endif
}
BOOST_AUTO_TEST_SUITE_END()

#if 0
/**
 * Test Utils package
 */
bool uTestUtils()
{
    /** Various tests of auxiliary routines - REPLACED WITH BOOST UTF */
    if ( !uTestMisc())
        return false;
    /** test singleton  - REPLACED WITH BOOST UTF */
    if ( !uTestSingle())
        return false;
    /** Tets list classes */
    if ( !uTestList())
        return false;

    /** Test memory management */
    if ( !uTestMem())
        return false;

    return true;
}
#endif
