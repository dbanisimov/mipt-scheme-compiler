/**
 * @file:string_table.cpp
 * Definition of the string table methods, look at string_table.hpp.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#include <assert.h>
#include <iostream>
#include <sstream>
#include "string_table.hpp"

namespace parser
{

int StringTable::id( const std::string& str)
{
    int id = find( str);
    if ( id == -1 )
        return add( str);
    return id;
}
int StringTable::find( const std::string& str) const
{
    for ( int i = 0; i < size(); ++i)
        if ( at(i) == str )
            return i;
    return -1;
}
int StringTable::add( const std::string& str)
{
    push_back( str);
    return size() - 1;
}
int StringTable::gensym( const std::string& akin)
{
    std::string str = akin;
    std::stringstream num;
    num << gensym_next++;
    str += num.str();//TODO: make a real uncautchable, but printable symbol here
    push_back( str);
    return size() - 1;
}
const std::string& StringTable::str( int id) const
{
    assert( 0 <= id && id < size());
    return at( id);
}

}//namespace parser
