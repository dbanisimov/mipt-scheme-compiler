/**
 * @file: utils_iface.h 
 * Interface of Utils library in compiler prototype project
 * @defgroup Utils Utils
 * @ingroup Core
 * @brief Low level utils
 */
/*
 * Utils library in SCL
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef UTILS_IFACE_H
#define UTILS_IFACE_H

/**
 * Debug assert with diagnostic info for Graph library
 * @ingroup GraphBase
 */
#if !defined(UTILS_ASSERTXD)
#    define UTILS_ASSERTXD(cond, what) ASSERT_XD(cond, "Utils", what)
#endif

/**
 * Simple debug assert for Graph library
 * @ingroup GraphBase
 */
#if !defined(UTILS_ASSERTD)
#    define UTILS_ASSERTD(cond) ASSERT_XD(cond, "Utils", "")
#endif


#include "types.h"
#include "host.h"
#include "misc.h"
#include "asrt.h"
//#include "techniques.h"
#include "print.h"
#include "list.h"
#include "singleton.h"
#include "mem.h"


namespace Utils
{
    using namespace PrintUtils;
};

/**
 * Test list classes
 */
bool uTestList();

/**
 * Test memory manager
 */
bool uTestMem();


#endif
