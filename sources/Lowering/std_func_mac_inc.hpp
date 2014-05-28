/**
 * File: std_func_mac.h
 * Copyright (C) 2012 Denis Anisimov
 */

#define STD_FUNC_TEST_MAC(NAME, SYM)\
    bool isFunc ## NAME ( string name) { return name == SYM; }; 

#define STD_FUNC_NUMERIC_DO_MAC(NAME, SYM)\
    void doFunc ## NAME() { doFuncNumericWithOper( NAME, #SYM); };

#define GEN_STD_FUNCS_NUMERIC(GEN_MAC)\
    GEN_MAC( Add, "+")\
    GEN_MAC( Sub, "-")

#define IS_OR_MAC(NAME, ...)\
    isFunc ## NAME ( name) ||

#define IF_IS_ELSE_MAC(NAME, ...)\
    if ( isFunc ## NAME( name) ) { doFunc ## NAME (); } else 

#define STD_FUNCS_NUMERIC\
    GEN_STD_FUNCS_NUMERIC(STD_FUNC_TEST_MAC)\
    GEN_STD_FUNCS_NUMERIC(STD_FUNC_NUMERIC_DO_MAC)\
    void doStdFuncNumeric( string name)\
    {\
        GEN_STD_FUNCS_NUMERIC(IF_IS_ELSE_MAC) \
        throw LoweringException( str( boost::format( "Std numeric function \"%s\" is not known") %  name));\
    };\
    bool isStdFuncNumeric( string name){ return GEN_STD_FUNCS_NUMERIC(IS_OR_MAC) 0; };

#define STD_FUNCS\
    STD_FUNCS_NUMERIC\
    void doStdFunc( string name)\
    {\
        if ( isStdFuncNumeric( name) ) { doStdFuncNumeric( name); }\
        else { throw LoweringException( str( boost::format( "Std function \"%s\" is not known") %  name)); };\
    };\
    bool isStdFunc( string name) { return isStdFuncNumeric( name); };

STD_FUNCS
