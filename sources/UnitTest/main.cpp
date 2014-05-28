/**
 * @file: UnitTest/main.cpp
 * Implementation and entry point for unit testing of compiler prototype project
 */
/*
 * Copyright (C) 2012  Boris Shurygin
 */
#include "utest_impl.h"

#define BOOST_TEST_MODULE compiler_prototype
#include <boost/test/unit_test.hpp>

/** Fixture for singletons' init/deinit */
struct GlobalFixture {
    GlobalFixture();
    ~GlobalFixture();
};

/* Initialization of singletons through fixture feature of boost unit testing framework (UTF) */
GlobalFixture::GlobalFixture()
{
    MemMgr::init();
}

/* Deinitialization of singletons through fixture feature of boost unit testing framework (UTF) */
GlobalFixture::~GlobalFixture()
{
    MemMgr::deinit();
    
    cout.flush();
    cerr.flush();
}

BOOST_GLOBAL_FIXTURE( GlobalFixture );

#ifdef USE_CUSTOM_MAIN
/**
 * The entry point for compiler prototype unit test project
 */
int main(int argc, char **argv)
{
    Log::init();
    MemMgr::init();

    /** Test utils */
    if ( !uTestUtils())
        return -1;

    /** Test Graph package */
    if ( !uTestGraph())
        return -1;
    
    MemMgr::deinit();
    Log::deinit();
    
    cout.flush();
    cerr.flush();
}
#endif