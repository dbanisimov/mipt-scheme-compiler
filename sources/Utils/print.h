/**
 * File: print.h - Wrapper for printing routines in compiler prototype project
 * Copyright (C) 2012  Boris Shurygin
 */
#ifndef PRINT_H
#define PRINT_H

namespace PrintUtils
{
    /** Print to file */
    
    /** Print to console's STDOUT */
    inline void out( const char* format, ...)
    {
#ifdef _DEBUG
        va_list args;
        va_start( args, format);
        vfprintf( stdout, format, args);
        va_end( args);
        fprintf( stdout, "\n");
#endif
    }
    /** Print to console's STDERR */
    inline void err( const char* format, ...)
    {
#ifdef _DEBUG
        va_list args;
        va_start( args, format);
        vfprintf( stderr, format, args);
        va_end( args);
        fprintf( stderr, "\n");
#endif
    }

    template<class T>
    std::string toString(T i)
    {
        std::stringstream ss;
        std::string s;
        ss << i;
        s = ss.str();

        return s;
    }
};


 
#endif
