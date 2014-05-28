/**
 * @file:ir/opt_mdes.hpp
 * Description of objects and operations in OPT-level IR
 * @ingroup MDES
 */

#pragma once

namespace Opt
{
#define OBJ( name, ...) name,

    /** Object types */
    enum OptObjectName
    {
#include "opt_mdes_inc.hpp"
        OPT_OBJECTS_NUM
    };

    /** Objects descriptions */

#define OPER( name, mnem, ...) name,

    /** Operation names */
    enum OptOperName
    {
#include "opt_mdes_inc.hpp"
        
        OPT_OPERS_NUM
    };


    typedef Mdes< OptObjectName, OptOperName, OPT_OBJECTS_NUM, OPT_OPERS_NUM, 3, 1> OptMdes;


    /* Feasible operands descriptions */
    typedef OptMdes::OpType<OPT_OBJECTS_NUM, 32, CANNOT_BE_IMM, CAN_BE_TARGET> target;
    typedef OptMdes::OpType<OPT_OBJECTS_NUM, 32, CANNOT_BE_IMM, CANNOT_BE_TARGET, CAN_BE_SET> arg_set;
    typedef OptMdes::OpType<OPT_OBJECTS_NUM, 32, CANNOT_BE_IMM, CANNOT_BE_TARGET, CAN_BE_SET> res_set;
    typedef OptMdes::OpType<Reg, 32, CAN_BE_IMM> reg_imm_32;
    typedef OptMdes::OpType<Reg, 32, CANNOT_BE_IMM> reg_32;
    typedef OptMdes::OpType<OPT_OBJECTS_NUM, 32, CAN_BE_IMM> imm_32;
};
