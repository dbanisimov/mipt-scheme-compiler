/**
 * File: main.cpp
 * Copyright (C) 2012 Denis Anisimov
 */

#include "lowering_iface.hpp"
#include <string>
#include <sstream>
#include <istream>
#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/hierarchy.h>

using namespace parser;
using namespace parser::ast;
using namespace lowering;
using namespace IR;
using namespace Opt;
using namespace log4cplus;

int main()
{
    /* Init simple console logger */
    BasicConfigurator log_conf;
    log_conf.configure();
    Logger log_lowir = Logger::getInstance(LOG4CPLUS_TEXT("lowir"));

    /* Init parser */
    StringTable st;
    Parser pr(&st, &std::cin);
    AstPrinter ast_pr( std::cout);
    SExprp exp;
    bool end;

    /* Init memory manager */
    MemMgr::init();

    /* Init codegen */
    CodeGen cg( &st);

    /* Greeting */
    LOG4CPLUS_INFO(log_lowir, LOG4CPLUS_TEXT("Lowering"));
     
    /* Parse */
    exp = pr.read( &end);
    cout << endl;
    ast_pr.print( exp);
    cout << endl;

    /* Gen */
    cg.gen( exp);

    /* Print */
    cg.print();

    return 0;
}
