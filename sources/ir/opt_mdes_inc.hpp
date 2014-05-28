/**
 * @file:ir/opt_mdes_inc.hpp
 * Macros for auto-generation of objects and operations in OPT-level IR
 * @ingroup MDES
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */
/** No pragma once, file may be included several times */
#ifndef OBJ
#  define OBJ( name, prefix, max_size, num, storage_class, can_be_virtual)
#endif
#ifndef OPER
#  define OPER( name, mnem, ...)
#endif

/** Objects */
OBJ( Reg, "t", 32, 0, REGISTER, true)

/** Arithmetic Operations */
OPER( Add,  "add",  Args<reg_imm_32, reg_imm_32>, Ress<reg_32> )
OPER( Sub,  "sub",  Args<reg_imm_32, reg_imm_32>, Ress<reg_32> )
OPER( Mul,  "mul",  Args<reg_imm_32, reg_imm_32>, Ress<reg_32> )

/* Memory operations */
OPER( Ld,   "ld",   Args<reg_imm_32>            , Ress<reg_32> )
OPER( St,   "st",   Args<reg_imm_32, reg_imm_32>, Ress<>       )

/* Branches */
OPER( Br,   "br",   Args<target>                    , Ress<> )
OPER( Brl,  "brl",  Args<reg_32, reg_imm_32, target>, Ress<> )
OPER( Brg,  "brg",  Args<reg_32, reg_imm_32, target>, Ress<> )
OPER( Brle, "brle", Args<reg_32, reg_imm_32, target>, Ress<> )
OPER( Brge, "brge", Args<reg_32, reg_imm_32, target>, Ress<> )
OPER( Bre,  "bre",  Args<reg_32, reg_imm_32, target>, Ress<> )
OPER( Brne, "brne",  Args<reg_32, reg_imm_32, target>, Ress<> )

/* Frame control */
OPER( Call, "call", Args<reg_imm_32, arg_set>, Ress<res_set> )
OPER( Ret,  "ret" , Args<arg_set>           , Ress<>         )

#undef OPER
#undef OBJ

