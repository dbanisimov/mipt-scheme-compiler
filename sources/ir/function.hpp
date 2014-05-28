/**
 * @file:ir/cf.hpp
 * Function description
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{

/**
    * @brief Representation of function
    * @ingroup IR
    */
template <class MdesType> class Function: 
    public DG< MdesType>,
    public SListIface<Function <MdesType> >
{
public:
    Function( Module<MdesType> *m);                   //< Create function in given module
    Function( Module<MdesType> *m, string func_name); //< Create named function in given module
    
    virtual ~Function();  //< Delete function

    inline Module<MdesType> *module() const; //< Get the module of the function
    inline CFG<MdesType> &cfg() ;            //< Get control flow graph of the function
        
    /** Create new operation */
    inline Operation< MdesType> * newOper( typename MdesType::OperName name);
        
    /** Create new operaiton and insert it into sequence */
    inline Operation< MdesType> * newOperAfter( typename MdesType::OperName name, Operation< MdesType> *prev_oper); 
        
    /** Create new object */
    inline Object< MdesType> * newObject( typename MdesType::ObjName name);
    
    /** Create new object */
    inline Object< MdesType> * newArg( UInt16 arg_num, typename MdesType::ObjName name);

    /** Get function's first operation in sequence */
    inline Operation< MdesType> * firstOper() const;

    /** Set function's first operation in sequence */
    inline void setFirstOper( Operation< MdesType> * oper);

    /** Print function to stream */
    inline void toStream(ostream& os);

    /** Get the function name */
    inline string name() const;

    /** Get the function name */
    inline void setName( string new_name);
private:
    Operation< MdesType> *newNode();   // Create new dep graph node (operation without a name)
    Module< MdesType> *module_p;       //< Pointer to the module
    list< Object< MdesType> *> objects;//< List of function's objects
    CFG< MdesType> cfg_;               //< Control flow graph
        
    /** First operation in linear sequence of operaitons in this function */
    Operation< MdesType> *first_oper_in_seq;
    /** Next object id */
    UInt32 obj_id[ MdesType::num_objs];
    /** Name of the function */
    string name_;
    /** Arguments */
    vector< Object< MdesType> *> args;
};

/** Get the function name */
template <class MdesType> string Function< MdesType>::name() const
{
    return name_;
}

/** Get the function name */
template <class MdesType> void Function< MdesType>::setName( string new_name)
{
    name_ = new_name;
}
    
/** Get function's first operation in sequence */
template <class MdesType> Operation< MdesType> *  Function< MdesType>::firstOper() const
{
    return first_oper_in_seq;
}

/** Set function's first operation in sequence */
template <class MdesType> void Function< MdesType>::setFirstOper( Operation< MdesType> * oper)
{
    first_oper_in_seq = oper;
}

/** 
 * Create function in given module
 */
template <class MdesType> Function< MdesType>::Function( Module<MdesType> *m): module_p( m) 
{
    for ( int i = 0; i < MdesType::num_objs; ++i)
    {
        obj_id[ i] = 0;
    }
}

/** 
 * Create named function in given module
 */
template <class MdesType> Function< MdesType>::Function( Module<MdesType> *m, string func_name):
    module_p( m), name_( func_name) 
{
    for ( int i = 0; i < MdesType::num_objs; ++i)
    {
        obj_id[ i] = 0;
    }
}

/** 
 * Function destructor
 */
template <class MdesType> 
Function< MdesType>::~Function()
{
    for ( typename list< Object< MdesType> *>::iterator it = objects.begin(), end =  objects.end();
            it != end;
            ++it)
    {
        delete *it;
    }
}

/** Get the module of the function */
template <class MdesType> Module< MdesType> *Function<MdesType>::module() const
{
    return module_p;
}
/** Get control flow graph of the function */
template <class MdesType> CFG<MdesType> &
Function< MdesType>::cfg()
{
    return cfg_;
}

template <class MdesType>
Operation< MdesType> * 
Function< MdesType>::newNode()
{
    return DG< MdesType>::newNode();
}

template <class MdesType>
Operation< MdesType> * 
Function< MdesType>::newOper( typename MdesType::OperName name)
{
    IR_ASSERTD( MdesType::checkName( name) );
    Operation< MdesType> * oper = newNode();
    oper->setName( name);
    return oper;
}
template <class MdesType>
Operation< MdesType> * 
Function< MdesType>::newOperAfter( typename MdesType::OperName name, Operation< MdesType> *prev_oper)
{
    IR_ASSERTD( MdesType::checkName( name) );
    Operation< MdesType> * oper = newNode();
    oper->setName( name);
    oper->insertAfter( prev_oper);
    return oper;
}

template <class MdesType>
Object< MdesType> * 
Function< MdesType>::newObject( typename MdesType::ObjName name)
{
    Object< MdesType> * obj = new Object< MdesType>();
    obj->setType( name);
    obj->setId( obj_id[ name]++);
    objects.push_back( obj);
    return obj;
}

template <class MdesType>
Object< MdesType> * 
Function< MdesType>::newArg( UInt16 arg_num, typename MdesType::ObjName name)
{
    Object< MdesType> * obj = newObject( name);
    if ( ( arg_num + 1 ) > args.size() )
    {
        args.resize( arg_num + 1);
    }
    args[ arg_num] = obj;
    return obj;
}

template <class MdesType>
void
Function< MdesType>::toStream(ostream& os)
{
    Marker m = MarkerManager::newMarker();

    /** Print function header */
    os << "Function " << name_;

    /** Print arguments */
    os << "( ";
    for ( typename vector< Object< MdesType> *>::iterator it = args.begin(),
                                                end = args.end();
         it != end; ++it)
    {
        os << *it;
    }

    os << ")" << endl;

    /* If CFG is valid print CFG instead of operation sequence */
    if ( this->cfg().isValid())
    {
        os << this->cfg();
        return;
    }

    /* Find the target operations */
    for ( Operation< MdesType> * oper = DG< MdesType>::firstNode();
            isNotNullP( oper);
            oper = oper->nextNode())
    {
        for ( UInt32 i = 0; i < oper->numArgs(); i ++)
        {
            if ( oper->arg( i).isTarget())
            {
                Operation< MdesType> * target = oper->arg(i).target();
                target->mark( m);
            }
        }
    }

    /* Iterating over operations in functions */
    for ( Operation< MdesType> *oper = firstOper();
            isNotNullP( oper);
            oper = oper->nextOper())
    {
        if ( oper->isMarked( m))
            os << "label_" << oper->id() << ':' << endl;
        os << oper;
        IR_ASSERTD( areEqP( oper->function(), this) );
    }

    this->freeMarker( m);
}

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, const Function<MdesType> &f) 
{   
    f.toStream(os);
    return os;
} 

template <class MdesType> 
std::ostream& operator<<(std::ostream& os, Function<MdesType> *f) 
{   
    f->toStream(os);
    return os;
} 

};
