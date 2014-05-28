/**
 * @file:ir/mdes.hpp
 * Declaration of the machine description classes and templates
 * @defgroup MDES Machine description
 * @ingroup IR
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */
#pragma once

enum CanBeImm
{ 
    CANNOT_BE_IMM = 0,
    CAN_BE_IMM = 1
};


enum CanBeTarget
{ 
    CANNOT_BE_TARGET = 0,
    CAN_BE_TARGET = 1
};

enum CanBeSet
{ 
    CANNOT_BE_SET = 0,
    CAN_BE_SET = 1
};

enum StorageClass
{ 
    REGISTER,
    STORAGE_CLASSES_NUM
};
    
class OP_NONE
{
public:
    static bool isNone(){return true;}
    static void initOp( void *des){};
};

template < typename ObjectName, typename OperName_t, UInt32 num_objs_, UInt32 num_opers_, UInt32 max_args_, UInt32 max_ress_> class Mdes
{
public:
    typedef ObjectName ObjName;
    typedef OperName_t OperName;

    static const  UInt32 num_opers = num_opers_;
    static const  UInt32 num_objs = num_objs_;
    static const  UInt32 max_args = max_args_;
    static const  UInt32 max_ress = max_args_;

    /**
     * Object description structure
     */
    struct ObjDes
    {
        ObjName name;
        string prefix;
        UInt32 max_size;
        UInt32 num;
        bool can_be_virtual;

        ObjDes(): name( (ObjName)num_objs){}
    };

    /**
     * Operand description structure
     */
    struct OpDes
    {
        ObjName name;
        UInt32 size;
        CanBeImm can_be_imm;
        CanBeTarget can_be_target;
        CanBeSet can_be_set;

        OpDes(){};

        bool isProperName( ObjName obj_name)
        {
            return name == obj_name;
        }

        bool canBeImm()
        {
            return can_be_imm == CAN_BE_IMM;
        }

        bool canBeObj()
        {
            return name != num_objs;
        }

        bool canBeTarget()
        {
            return can_be_target == CAN_BE_TARGET;
        }
        bool canBeSet()
        {
            return can_be_set == CAN_BE_SET;
        }
    };

    /**
     * Operation description structure
     */
    struct OperDes
    {
        OperName_t name;
        string mnemonic_str;
        UInt32 num_args;
        UInt32 num_ress;
        OpDes args[ max_args];
        OpDes ress[ max_ress];
        bool can_have_arg_set;
        bool can_have_res_set;

        OperDes():
            name( (OperName)num_opers),
            can_have_arg_set( false), 
            can_have_res_set( false)
        {};


        static UInt32 maxArgs(){ return max_args;}
        static UInt32 maxRess(){ return max_ress;}

        OpDes *argDes( UInt32 num)  { return (num < num_args) ? &args[ num] : NULL;}
        OpDes *resDes( UInt32 num)  { return (num < num_ress) ? &ress[ num] : NULL;}
        inline bool canHaveArgSet() { return can_have_arg_set;                     }
        inline bool canHaveResSet() { return can_have_res_set;                     }
    };

    template <ObjName obj_name,
              UInt32 size,
              CanBeImm can_be_imm = CANNOT_BE_IMM,
              CanBeTarget can_be_target = CANNOT_BE_TARGET,
              CanBeSet can_be_set = CANNOT_BE_SET> class OpType
    {
    public:    
        static bool isNone(){ return false;}
        static UInt32 opSize(){ return size;}
        static CanBeImm canBeImm(){ return can_be_imm;}
        static CanBeTarget canBeTarget(){ return can_be_target;}
        static ObjName name(){ return obj_name;}
        static void initOp( OpDes *obj)
        {
            obj->can_be_imm = can_be_imm;
            obj->name = obj_name;
            obj->size = size;
            obj->can_be_target = can_be_target;
            obj->can_be_set = can_be_set;
        }
    };

    template < bool dir,           //fake arg, prevents full specialization of template
               class Op1 = OP_NONE,
               class Op2 = OP_NONE,
               class Op3 = OP_NONE,
               class Op4 = OP_NONE> class OpsType
    {
    public:
        static UInt32 numOps(){ return 0;}
        static void initOps( OpDes *des) {}
    };
              
    template <bool dir>
    class OpsType <dir, OP_NONE, OP_NONE, OP_NONE, OP_NONE>
    {
    public:
        static UInt32 numOps(){ return 0;}
        static void initOps( OpDes *des) 
        {
        }
    };

    template <bool dir, class Op1>
    class OpsType <dir, Op1, OP_NONE, OP_NONE, OP_NONE>
    {
    public:
        static UInt32 numOps(){ return 1;}
        static void initOps( OpDes *des) 
        {
            Op1::initOp(&des[ 0]);
        }
    };

    template <bool dir, class Op1, class Op2>
    class OpsType <dir, Op1, Op2, OP_NONE, OP_NONE>
    {
    public:
        static UInt32 numOps(){ return 2;}
        static void initOps( OpDes *des) 
        {
            Op1::initOp(&des[ 0]);
            Op2::initOp(&des[ 1]);
        }
    };

    template <bool dir, class Op1, class Op2, class Op3>
    class OpsType <dir, Op1, Op2, Op3, OP_NONE>
    {
    public:
        static UInt32 numOps(){ return 3;}
        static void initOps( OpDes *des) 
        {
            Op1::initOp(&des[ 0]);
            Op2::initOp(&des[ 1]);
            Op3::initOp(&des[ 2]);
        }
    };

    template < class Op1 = OP_NONE,
               class Op2 = OP_NONE,
               class Op3 = OP_NONE,
               class Op4 = OP_NONE> 
    class Args: public OpsType< false, Op1, Op2, Op3, Op4>    {};

    template < class Op1 = OP_NONE,
               class Op2 = OP_NONE,
               class Op3 = OP_NONE,
               class Op4 = OP_NONE> 
    class Ress: public OpsType< true, Op1, Op2, Op3, Op4>    {};

    /** Constructor */
    inline Mdes(){};

    /**
     * Check that name is valid for this Mdes and operation's
     * description structure is properly initialized
     */
    static bool checkName( OperName name)
    {
        IR_ASSERTD( name < num_opers);
        IR_ASSERTXD( opers[ name].name != num_opers, "Operation description is not initialized");
        return name < num_opers
               && opers[ name].name != num_opers;
    }


    /** Accessor of operation description */
    static OperDes *operDescription( OperName_t name)
    {
        IR_ASSERTD( name < num_opers);

        IR_ASSERTXD( opers[ name].name != num_opers, "Operation description is not initialized");
        return &opers[ name];
    }
    /** Accessor of operation description */
    static ObjDes *objectDescription( ObjName name)
    {
        IR_ASSERTD( name < num_objs);

        IR_ASSERTXD( objects[ name].name != num_objs, "Object description is not initialized");
        return &objects[ name];
    }

    /** Operation initializer routine */
    template <class ArgsType, class RessType>
    void
    initOperDes( OperName_t name, string str)
    {
        OperDes * des = &opers[ name];
        des->name = name;
        des->mnemonic_str = str;
        des->num_args = ArgsType::numOps();
        des->num_ress = RessType::numOps();

        IR_ASSERTD( des->num_args <= OperDes::maxArgs() );
        IR_ASSERTD( des->num_ress <= OperDes::maxRess() );

        ArgsType::initOps( des->args);
        RessType::initOps( des->ress);

        for ( UInt32 arg = 0; arg < des->num_args; arg++)
        {
            if ( des->argDes( arg)->canBeSet())
                des->can_have_arg_set = true;
        }
        for ( UInt32 res = 0; res < des->num_ress; res++)
        {
            if ( des->resDes( res)->canBeSet())
                des->can_have_res_set = true;
        }
    }

    void
    initObjDes( ObjectName name, string str, UInt32 max_size, UInt32 num, StorageClass stor_class, bool can_be_virtual)
    {
        ObjDes * des = &objects[ name];
        des->name = name;
        des->prefix = str;
        des->max_size = max_size;
        des->num = num;
        des->can_be_virtual = can_be_virtual;
    }

private:    
    static OperDes opers[ num_opers];
    static ObjDes objects[ num_objs];
}; /* Mdes definition */


