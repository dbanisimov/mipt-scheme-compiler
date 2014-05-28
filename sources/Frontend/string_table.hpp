/**
 * @file:string_table.hpp
 * Declaration of the string table, the object holding all
 * strings met in parsed source, and associating a unieuq
 * id with each of them.
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

#include <map>
#include <vector>
#include <string>

namespace parser
{

class StringTable :private std::vector<std::string>
{
    int gensym_next;
public:
    int id( const std::string& str);
    int find( const std::string& str) const;
    int add( const std::string& str);
    int gensym( const std::string& akin);

    const std::string& str( int id) const;
};

}
