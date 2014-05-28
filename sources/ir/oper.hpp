/**
 * @file: oper.hpp 
 * Operation class definition
 * Author: Robert Khasanov
 */
#pragma once 

namespace IR
{
/**
 * @class Object
 * @brief Representation of an object
 */
template <class MdesType> class Object
{
public:
    typedef typename MdesType::ObjName ObjName;
        
    /** Set virtual flag to object */
    inline void setVirtual( bool is_virtual); 
    /** Set object id */
    inline void setId( UInt32 id);
    /** Return whether object is virtual */ 
    inline bool isVirtual() const;
    /** Return object id */
    inline UInt32 id() const;

    /** Constructor */
    inline Object();

    /** Destructor */
    inline ~Object();
    /** Set type of object */
    inline void setType( ObjName type);
    /** Return type of object */
    inline ObjName type() const;

    /** Print object to stream */
    inline void toStream(ostream& os) const;
private:
    ObjName type_;
    bool is_virtual;
    UInt32 id_;
};

/** Tag type for list */
template <class MdesType> class OperandList{};

/**
 * @class Operand
 * @brief Representation of Operand
 */
template <class MdesType> class Operand: 
    public SListIface< Operand< MdesType> >
{
public:
    /** Set object to operand */
    inline void setObject( Object< MdesType>* obj);
    /** Set DF to operand */
    //inline void setDF( DF* df);
    /** Set whether operand is implicit */
    inline void setImplicit( bool is_implicit);
    /** Set constant value */
    inline void setConstValue( Const i_value);
    /** Set target */
    inline void setTarget( Operation< MdesType> *target_oper);
    /** Set operand type */
    inline void setType( OperandType type);
    /** Return operand object */
    inline Object< MdesType> * object() const;
     /** Return DF edge of operand */
    //inline DF& df() const;
    /** Return whether operand is implicit */
    inline bool isImplicit() const;
    /** Return constant value */
    inline Const constValue() const;
    /** Return target operation */
    inline Operation< MdesType> * target() const;
    /** Return type of operand */
    inline OperandType type() const;
    
    /** Check that operand is target */
    inline bool isTarget() const { return type() == OP_TYPE_TARGET;};

    /** Constructor */
    inline Operand();
    /** Destructor */
    inline ~Operand();

    /** Print operand to stream */
    inline void toStream(ostream& os) const;
private:
    Object< MdesType> * obj;
    //DF* df_;
    bool is_implicit : 1;
    Const i_value;
    OperandType type_;
    Operation< MdesType> *target_;
};

/** Tag type for list */
template <class MdesType> class OperSeq{};

/**
 * @class Operation
 * @brief Representation of Operation
 */
template <class MdesType> class Operation:
    public Node< DG< MdesType>, Operation< MdesType>, DepEdge< MdesType> >,
    public ListItem< Operation< MdesType>, OperSeq< MdesType> >
{
public:
    
    /** Typedef for operation sequence list */
    typedef ListItem< Operation< MdesType>, OperSeq< MdesType> > OperSeqList;

    /** Type of operation name */
    typedef typename MdesType::OperName OperName;
    
    /** Constructor */
    Operation( DG <MdesType> *g);

    /** Destructor */
    ~Operation();

    /** Get operation's function */
    inline Function< MdesType> *function() const;

    /** Return name of the operation */
    inline OperName name() const;

    /** Get number of arguments */
    inline UInt32 numArgs() const;

    /** Get number of results */
    inline UInt32 numRess() const;

    /** Get argument */
    inline const Operand<MdesType>& arg( unsigned i) const;
    /** Get result */
    inline const Operand<MdesType>& res( unsigned i) const;
    
    /** Set argument's object */
    inline void setArgObj( UInt32 arg_num, Object< MdesType> *obj);

    /** Set argument's immediate value */
    inline void setArgImm( UInt32 arg_num, Const imm_val);

    /** Set argument's target */
    inline void setArgTarget( UInt32 arg_num, Operation<MdesType> *target);

    /** Set result's object */
    inline void setResObj( UInt32 res_num, Object< MdesType> *obj);

    /** Set name of operation */
    inline void setName( OperName nm);

    /** Print operation to stream */
    inline void toStream(ostream& os) const;

    /** Get the mnemonic string of the operation */
    inline string & nameStr() const;

    /** Detach from operation sequence */
    inline void detachFromSeq();

    /** Next operation in operation sequence */
    inline Operation< MdesType>* nextOper() const;

    /** Previous operation in operation sequence */
    inline Operation< MdesType>* prevOper() const;

    /** Insert operation into sequence after given operation */
    inline void insertAfter( Operation< MdesType> *prev_oper);

    /** Insert operation into sequence before given operation */
    inline void insertBefore( Operation< MdesType> *next_oper);

    /* Add an arguments to set */
    inline const Operand< MdesType> * addArgToSet( Object< MdesType> *obj); //< Add object argument
    inline const Operand< MdesType> * addArgToSet( Const imm_val);       //< Add immediate argument

    /* Add an result to set, only objects can be in results set */
    inline const Operand< MdesType> * addResToSet( Object< MdesType> *obj); //< Add object result

    /** Get first argument operand in argument set */
    inline Operand< MdesType> * firstArgInSet() const;
    
    /** Get first result in result set */
    inline Operand< MdesType> * firstResInSet() const;

    inline CFNode< MdesType> * node() const;    //< Get the control flow node of the operation
    inline void setNode( CFNode< MdesType> *n); //< Set the control flow node of the operation

    inline bool isLastInNode() const;  //< Check if the operation is last in its CF node
    inline bool isFirstInNode() const; //< Check if the operation is first in its CF node
    
private:
    /* Arrays of operands */
    Operand<MdesType> args[ MdesType::max_args]; //< Array of argument operands
    Operand<MdesType> ress[ MdesType::max_ress]; //< Array of result operands
    
    /* 
     * Argument and result sets of arbitrary length (e.g. in operations like call)
     * are implemented as the lists of operands. Memory is still controlled by the operation.
     */
    Operand<MdesType> *first_arg; //< First argument in set
    Operand<MdesType> *first_res; //< First result in set

    OperName name_; //< Operation name

    CFNode< MdesType> * node_; //< Control flow node of the operation

    /** Print operand list to stream */
    inline void opListToStream(ostream& os, Operand< MdesType>* op) const;
};

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, const Object<MdesType> &object) 
{   
    object.toStream(os);
    return os;
} 

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, Object<MdesType> *obj) 
{   
    obj->toStream(os);
    return os;
} 

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, const Operand<MdesType> &op) 
{   
    op.toStream(os);
    return os;
} 

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, const Operation<MdesType> &oper) 
{   
    oper.toStream(os);
    return os;
} 

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, Operation<MdesType> *oper) 
{   
    oper->toStream(os);
    return os;
} 

 
}; /* namespace IR */
