/**
 * @file oper_inline.hpp
 * Implmentation of AbstractOperation class inline routines
 */

#pragma once 

namespace IR
{

template <class MdesType> void 
Object<MdesType>::setVirtual( bool is_virtual)
{
    this->is_virtual = is_virtual;
}

template <class MdesType> void 
Object<MdesType>::setId( UInt32 id_)
{
    this->id_ = id_;
}

template <class MdesType> bool 
Object<MdesType>::isVirtual() const
{
    return is_virtual;
}

template <class MdesType> UInt32 
Object<MdesType>::id() const
{
    return id_;
}

template <class MdesType> 
Object<MdesType>::Object()
{
    setVirtual( true);
    setId( (UInt32) -1); 
}

template <class MdesType> 
Object<MdesType>::~Object()
{
}

template <class MdesType> void
Object<MdesType>::toStream(ostream& os) const
{
    typename MdesType::ObjDes *des = MdesType::objectDescription( type());
    
    if ( isVirtual() )
        os << 'v';
    os << des->prefix << id();    
}

template <class MdesType> void 
Operand<MdesType>::setObject( Object<MdesType>* obj)
{
    this->obj = obj;
}

template <class MdesType> void 
Operand<MdesType>::setImplicit( bool is_implicit)
{
    this->is_implicit = is_implicit;
}

template <class MdesType> void 
Operand<MdesType>::setConstValue( Const i_value)
{
    this->i_value = i_value;
}

template <class MdesType> void 
Operand<MdesType>::setType( OperandType type)
{
    this->type_ = type;
}

template <class MdesType> 
Object<MdesType> *
Operand<MdesType>::object() const
{
    return obj;
}

template <class MdesType> bool 
Operand<MdesType>::isImplicit() const
{
    return is_implicit;
}

template <class MdesType> Const 
Operand<MdesType>::constValue() const
{
    return i_value;
}

template <class MdesType>
Operation< MdesType> *
Operand<MdesType>::target() const
{
    return target_;
}

template <class MdesType>
void
Operand<MdesType>::setTarget( Operation< MdesType> *target_oper)
{
    target_ = target_oper;
}

template <class MdesType> OperandType
Operand<MdesType>::type() const
{
    return type_;
}

template <class MdesType> 
Operand<MdesType>::Operand(): target_( NULL), i_value( (Int64)0) 
{
    setObject( NULL);
    setImplicit( false);
    setType( OP_TYPES_NUM);
}

template <class MdesType> 
Operand<MdesType>::~Operand()
{

}

template <class MdesType> void
Operand<MdesType>::toStream(ostream& os) const
{
    switch ( type() )
    {
        case OP_TYPE_OBJ:
        {
            IR_ASSERTD( isNotNullP( object()));
            os << object();
            break;
        }
        case OP_TYPE_IMM:
            os << constValue(); 
            break;
        case OP_TYPE_TARGET:
        {
            IR_ASSERTD( isNotNullP( target()));
            os << "<label_" << target()->id() << '>';
            break;
        }
        case OP_TYPE_SET:
            break;
        case OP_TYPES_NUM:
            os << "{unset}";
            break;

        default:
            IR_ASSERTD( 0);
    }   
}
 
/** Get number of arguments */
template <class MdesName> UInt32 Operation<MdesName>::numArgs() const
{
    return MdesName::operDescription( name())->num_args; 
}

/** Get number of results */
template <class MdesName> UInt32 Operation<MdesName>::numRess() const
{
    return MdesName::operDescription( name())->num_ress; 
}

template <class MdesName> 
const Operand<MdesName>&
Operation<MdesName>::arg( unsigned i) const
{
    IR_ASSERTXD( i < numArgs(), "Invalid argument number");
    return args[ i];
}

template <class MdesName> 
const Operand<MdesName>&
Operation<MdesName>::res( unsigned i) const
{
    IR_ASSERTXD( i < numRess(), "Invalid result number");
    return ress[ i];
}

/** Set type of object */
template <class MdesType> 
void Object<MdesType>::setType( ObjName tp)
{
    type_ = tp;
}   

/** Set operation's argument object */
template <class MdesType> void 
Operation<MdesType>::setArgObj( UInt32 arg_num, Object< MdesType> *obj)
{
    IR_ASSERTD( arg_num < numArgs());
    
    /** Check that given operand can actually hold object according to Mdes */
    typename MdesType::OperDes *des = MdesType::operDescription( name());
    typename MdesType::OpDes *op_des = des->argDes( arg_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->isProperName( obj->type()) );
    IR_ASSERTD( args[ arg_num].type() != OP_TYPE_TARGET);

    /** Set proper operand type */
    args[ arg_num].setType( OP_TYPE_OBJ);

    /** Set operand object */
    args[ arg_num].setObject( obj);
}

/** Set operation's argument object */
template <class MdesType> void 
Operation<MdesType>::setArgTarget( UInt32 arg_num, Operation<MdesType> *target)
{
    IR_ASSERTD( arg_num < numArgs());
    
    /** Check that given operand can actually hold object according to Mdes */
    typename MdesType::OperDes *des = MdesType::operDescription( name());
    typename MdesType::OpDes *op_des = des->argDes( arg_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->canBeTarget() );

    /** Set proper operand type */
    args[ arg_num].setType( OP_TYPE_TARGET);

    /** Set operand object */
    args[ arg_num].setTarget( target);

}

/** Set operation's argument object */
template <class MdesType> void 
Operation<MdesType>::setArgImm( UInt32 arg_num, Const imm_val)
{
    IR_ASSERTD( arg_num < numArgs());
    
    /** Check that given operand can actually be immediate according to Mdes */
    typename MdesType::OperDes *des = MdesType::operDescription( name());
    typename MdesType::OpDes *op_des = des->argDes( arg_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->canBeImm() );
    IR_ASSERTD( args[ arg_num].type() != OP_TYPE_TARGET);

    /** Set proper operand type */
    args[ arg_num].setType( OP_TYPE_IMM);

    /** Set operand immediate value */
    args[ arg_num].setConstValue( imm_val);
}

/** Set operation's argument object */
template <class MdesType> void 
Operation<MdesType>::setResObj( UInt32 res_num, Object< MdesType> *obj)
{
    IR_ASSERTD( res_num < numRess());
    
    /** Check that given operand can actually hold object according to Mdes */
    typename MdesType::OperDes *des = MdesType::operDescription( name());
    typename MdesType::OpDes *op_des = des->resDes( res_num);

    IR_ASSERTD( isNotNullP( op_des));
    IR_ASSERTD( op_des->isProperName( obj->type()) );

    /** Set proper operand type */
    ress[ res_num].setType( OP_TYPE_OBJ);

    /** Set operand object */
    ress[ res_num].setObject( obj);
}

/** Return type of object */
template <class MdesType> 
typename Object<MdesType>::ObjName
Object<MdesType>::type() const
{
    return type_;    
}

/** Set name of operation */
template <class MdesType> 
void Operation<MdesType>::setName( OperName nm)
{
    IR_ASSERTD( nm < MdesType::num_opers);
    name_ = nm;
}   

/** Return name of operation */
template <class MdesName> 
typename Operation<MdesName>::OperName
Operation<MdesName>::name() const
{
    return name_;    
}

/** Operation constructor */
template < class MdesType> 
Operation<MdesType>::Operation( DG <MdesType> *g):
    Node< DG< MdesType>, Operation< MdesType>, DepEdge<MdesType> >( g), // Dependence graph node constructor
    first_arg( NULL), // We don't have any arguments in the set on operation construction
    first_res( NULL), // We don't have any results in the set on operation construction
    node_( NULL)      // Node is not define at the moment of operation construction
{
    
}

/** Operation destructor */
template < class MdesType> 
Operation<MdesType>::~Operation()
{
    deleteList< Operand<MdesType> >( first_arg);
    deleteList< Operand<MdesType> >( first_res);
}


/** Print operand list to stream */
template < class MdesType> 
void
Operation<MdesType>::opListToStream(ostream& os, Operand< MdesType>* op) const
{
    if ( isNotNullP( op))
    {        
        bool first = true;
        os << '(';
        while ( isNotNullP( op) )
        {
            if ( !first )
            {
                os << ", ";   
            }
            else
            {
                first = false;
            }
            os << *op;
            op = op->next();
        }
        os << ')';
    }
}


/** Get the mnemonic string of the operation */
template < class MdesType> 
string &
Operation<MdesType>::nameStr() const
{
    typename MdesType::OperDes *des = MdesType::operDescription( name());

    return des->mnemonic_str;
}

/** Dump operation to stream */
template < class MdesType> 
void
Operation<MdesType>::toStream(ostream& os) const
{
    typename MdesType::OperDes *des = MdesType::operDescription( name());

    /* Print id */
    os << '{' << this->id() << "} ";
   
    /* Mnemonic string */
    os << des->mnemonic_str;

    UInt32 ress_printed = 0;

    /* Results */
    for ( UInt32 i = 0, first = 1; i < numRess(); i++)
    {  
        /* Skip results if it have to be res set */
        if ( des->resDes( i)->can_be_set) 
        {
            if ( isNotNullP( first_res))
            {
                if ( !first)
                   os << ',';   
                else
                   first = 0;
                
                os << ' ';
                
                opListToStream( os, first_res);
                ress_printed++;
            }
       } else
       {
           if ( !first)
               os << ',';   
           else
               first = 0;
           os << ' ' << res( i);
           ress_printed++;
       }
   }
   if ( ress_printed > 0)
      os << " =";
  
    /* Arguments */
    for ( UInt32 i = 0, first = 1; i < numArgs(); i++)
    {  
        /* Skip arguments if it have to be res set */
        if ( des->argDes( i)->can_be_set) 
        {
            if ( isNotNullP( first_arg))
            {
                if ( !first)
                   os << ',';   
                else
                   first = 0;
                os << ' ';
                opListToStream( os, first_arg);
            }
       } else
       {
           if ( !first)
               os << ',';
           else
               first = 0;   
           os << ' ' << arg( i);
       }
    }
    /* Newline */
    os << endl;
}

/** Detach from operation sequence */
template < class MdesType> 
void
Operation<MdesType>::detachFromSeq()
{
    return OperSeqList::detach();
}

/** Next in operation sequence */
template < class MdesType> 
Operation< MdesType> *
Operation<MdesType>::nextOper() const
{
    return OperSeqList::next();
}

/** Previous in operation sequence */
template < class MdesType> 
Operation< MdesType> *
Operation<MdesType>::prevOper() const
{
    return OperSeqList::prev();
}

/** Insert operation into sequence after the given operation */
template < class MdesType> 
void
Operation<MdesType>::insertAfter( Operation< MdesType> *prev_oper)
{
    OperSeqList::detach();
    prev_oper->OperSeqList::attach( this);
}

/** Insert operation into sequence before the given operation */
template < class MdesType> 
void
Operation<MdesType>::insertBefore( Operation< MdesType> *next_oper)
{
    OperSeqList::detach();
    next_oper->OperSeqList::attachInDir( this, LIST_DIR_RDEFAULT);
}

/** Get operation's function */
template < class MdesType> 
Function< MdesType> *
Operation<MdesType>::function() const
{
    return static_cast<Function< MdesType> *>(this->graph());
}

/** Add object argument */
template < class MdesType> 
const Operand< MdesType> *
Operation<MdesType>::addArgToSet( Object< MdesType> *obj)
{
    /** Check that the operation can actually have argument set according to Mdes */
    typename MdesType::OperDes *des = MdesType::operDescription( name());
    IR_ASSERTD( des->canHaveArgSet() );

    /* Create new operand, set its type and assign object to it */
    Operand< MdesType> *op = new Operand< MdesType>();
    op->setType( OP_TYPE_OBJ);
    op->setObject( obj);
    
    /* Operand list bookeeping */
    op->attach( first_arg);
    first_arg = op;
    return op;
}

/** Add immediate argument */
template < class MdesType> 
const Operand< MdesType> *
Operation<MdesType>::addArgToSet( Const imm_val)
{
    /** Check that the operation can actually have argument set according to Mdes */
    typename MdesType::OperDes *des = MdesType::operDescription( name());
    IR_ASSERTD( des->canHaveArgSet() );
    
    /* Create new operand, set its type and assign object to it */
    Operand< MdesType> *op = new Operand< MdesType>();
    op->setType( OP_TYPE_IMM);
    op->setConstValue( imm_val);
    
    /* Operand list bookeeping */
    op->attach( first_arg);
    first_arg = op;
    return op;
}

/** Add object result */
template < class MdesType> 
const Operand< MdesType> *
Operation<MdesType>::addResToSet( Object< MdesType> *obj)
{
    /** Check that the operation can actually have result set according to Mdes */
    typename MdesType::OperDes *des = MdesType::operDescription( name());
    IR_ASSERTD( des->canHaveResSet() );

    /* Create new operand, set its type and assign object to it */
    Operand< MdesType> *op = new Operand< MdesType>();
    op->setType( OP_TYPE_OBJ);
    op->setObject( obj);
    
    /* Operand list bookeeping */
    op->attach( first_res);
    first_res = op;

    return op;
}

/** Get first argument operand in argument set */
template < class MdesType> 
Operand< MdesType> * 
Operation<MdesType>::firstArgInSet() const
{
    return first_arg;
}
    
/** Get first result in result set */
template < class MdesType> 
Operand< MdesType> * 
Operation<MdesType>::firstResInSet() const
{
    return first_res;
}

/** Get the control flow node of the operation */
template < class MdesType> 
CFNode< MdesType> *
Operation<MdesType>::node() const
{
    return node_;
}

// Set the control flow node of the operation
template < class MdesType> 
void 
Operation<MdesType>::setNode( CFNode< MdesType> *n)
{
    node_ = n;
}

// Check if the operation is last in its CF node
template < class MdesType> 
bool
Operation<MdesType>::isLastInNode() const
{
    if ( isNullP( node()) )
        return false;

    return areEqP( this->node()->lastOper(), this);
}

// Check if the operation is first in its CF node
template < class MdesType> 
bool
Operation<MdesType>::isFirstInNode() const
{
    if ( isNullP( node()) )
        return false;

    return areEqP( this->node()->firstOper(), this);
}

}; /* namespace IR */
