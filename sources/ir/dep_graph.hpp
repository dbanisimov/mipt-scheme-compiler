/**
 * @file:ir/cf.hpp
 * Declaration of the dependence graph representation classes
 */
 /*
  * Copyright 2012 MIPT-COMPILER team
  */

#pragma once

namespace IR
{
    /**
     * @class DepEdge
     * @brief Dependence edge class
     * @ingroup DG
     */
    template <class MdesType> class DepEdge: 
        public Edge< DG <MdesType>, Operation< MdesType>, DepEdge<MdesType> >
    {
    public:
        /** Control flow edge constructor */
        inline DepEdge( DG<MdesType> *g, Operation<MdesType> *pred, Operation<MdesType> *succ);
    private:
    };

    /**
     * @class DG
     * @brief Dependence graph representation
     * @defgroup DG
     * @ingroup IR
     */
    template <class MdesType> class DG: 
        public Graph< DG <MdesType>, Operation< MdesType>, DepEdge<MdesType> >
    {
        
    };

    /** Dependence edge constructor */
    template < class MdesType> DepEdge<MdesType>::DepEdge( DG <MdesType> *g, Operation<MdesType> *pred, Operation<MdesType> *succ):
        Edge< DG <MdesType>, Operation< MdesType>, DepEdge<MdesType> >( g, pred, succ)
    {
    
    }


};