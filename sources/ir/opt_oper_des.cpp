/**
 * @file:ir/opt_oper_des.c
 * Impelemtation of objects and operations in OPT-level IR
 * @ingroup MDES
 */
#include "ir_iface.hpp"

using namespace Opt;

template <> OptMdes::OperDes OptMdes::opers[ OptMdes::num_opers] = {};
template <> OptMdes::ObjDes OptMdes::objects[ OptMdes::num_objs] = {};

#define OPER( name, str, ...) initOperDes< __VA_ARGS__>( name, str);
#define OBJ( ...) initObjDes( __VA_ARGS__);

/** Operations descriptions */
template <> OptMdes::Mdes()
{
    #include "opt_mdes_inc.hpp"
}

OptMdes opt_mdes;