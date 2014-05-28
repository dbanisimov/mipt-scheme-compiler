/**
 * @file:ast.hpp
 * A declaration of a set of classes reprodusing Abstract Symbol Tree 
 * of Scheme language
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <list>

namespace parser 
{

class StringTable;

namespace ast
{

using std::string;

class SExpr;
class Visitor;

class SExprp :public boost::shared_ptr<SExpr>
{
public:
    SExprp( SExpr* e) :boost::shared_ptr<SExpr>(e) {}
    SExprp () :boost::shared_ptr<SExpr>() {}
    SExprp visit( Visitor* v);

};

class Visitor
{
public:
    virtual ~Visitor() {/* empty */}
    virtual SExprp onNil( SExprp subj) = 0;
    virtual SExprp onCons( SExprp subj) = 0;
    virtual SExprp onSymbol( SExprp subj) = 0;
    virtual SExprp onVector( SExprp subj) = 0;
    virtual SExprp onNumber( SExprp subj) = 0;
    virtual SExprp onStrConst( SExprp subj) = 0;
    virtual SExprp onCharacter( SExprp subj) = 0;
    virtual SExprp onBoolean( SExprp subj) = 0;
};

#define VISITING( funName) \
    SExprp visit( Visitor* v, SExprp me) \
    { return v->funName( me);}

class SExpr
{
public:
    virtual ~SExpr() {/* empty */}
    virtual bool isList() = 0;
    virtual SExprp visit( Visitor* v, SExprp me) = 0;

    enum TypeValues
    {
        nil,
        cons,
        symbol,
        vector,
        string_const,
        number_const,
        character_const,
        boolean_const,
        types_num
    };
    typedef int Type;
    const Type type;
    SExpr( Type t) :type(t) {}
};

class List :public SExpr
{
public:
    List( Type t):SExpr( t){}
    bool isList() {return true;}
};

typedef boost::shared_ptr<List> Listp;

class Nil :public List
{
public:
    Nil() :List( nil) {}
    VISITING( onNil);

    static SExprp make();
};

class Cons :public List
{
public:
    SExprp car;
    SExprp cdr;
public:
    Cons( SExprp head, SExprp tail) :List( cons), car( head), cdr( tail) {}
    VISITING( onCons);

    static SExprp make( SExprp head, SExprp tail);
};

class Atom :public SExpr
{
public:
    Atom( Type t) :SExpr( t) {}
    bool isList() { return false; }
};

class Vector :public Atom
{
public:
    Vector( const std::vector<SExprp>& cont) :content( cont), Atom( vector) {}
    VISITING( onVector);

    const std::vector<SExprp>& val() const { return content; }
    std::vector<SExprp>& val() { return content; }

    static SExprp make( const std::vector<SExprp>& cont);

private:
    std::vector<SExprp> content;
};

class Number :public Atom
{
public:
    Number( long double num) :Atom( number_const), n( num) {}
    VISITING( onNumber);

    long double val() const { return n; }

    static SExprp make( long double num);

private:
    long double n;
};

class Character :public Atom
{
public:
    Character( char ch) :Atom( character_const), c( ch) {}
    VISITING( onCharacter);
    char val() const { return c; }

    static SExprp make( char ch);
private:
    char c;
};

class StrConst :public Atom
{
public:
    StrConst( const string& s) :Atom( string_const), str( s) {}
    VISITING( onStrConst);

    const string& val() const { return str; }
    string& val() { return str; }

    static SExprp make( const string& s);
private:
    string str;
};

class Symbol :public Atom
{
public:
    Symbol( int name_) :name( name_), Atom( symbol) {}
    VISITING( onSymbol);

    int getName() const { return name;}

    static SExprp make( int name);
    static SExprp make( const string& name, StringTable* table);
private:
    int name;
};

class Boolean :public Atom
{
public:
    Boolean( bool b) :Atom( boolean_const), value( b) {}
    VISITING( onBoolean);
    bool val() const { return value; }

    static SExprp make( bool b);
private:
    bool value;
};



template <class T>
int getClassRuntimeTag() {return -1;}

template <class T>
string getClassRuntimeName() { return "undefined"; }

#define SET_CLASS_RUNTIME_TAG_N_NAME( cls, tag, name)                   \
    template <> int getClassRuntimeTag<cls>() { return tag; }           \
    template <> string getClassRuntimeName<cls>() { return name; }


template<class T> T* as(const SExprp& e)
{ assert( e.get()->type == getClassRuntimeTag<T>()); return (T*)e.get();}

} //namespace ast
} //namespace parser
