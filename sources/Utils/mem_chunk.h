/**
 * @file: mem_chunk.h 
 * Implementation of memory chunk
 */
/*
 * Utils/Mem library in compiler prototype project tool
 * Copyright (C) 2012  Boris Shurygin
 */
#pragma once

#ifndef MEM_H
#    error
#endif

#ifndef MEM_CHUNK_H
#define MEM_CHUNK_H

namespace MemImpl
{
    /**
     * Chunk lists identificators
     * @ingroup MemImpl
     */
    enum ChunkListType
    {
        CHUNK_LIST_ALL,
        CHUNK_LIST_FREE,
        CHUNK_LISTS_NUM
    };

    /**
     * Memory chunk representation
     * @ingroup MemImpl
     */
    template< size_t size> class Chunk: 
        public MListIface< Chunk< size>, // List client data
                           CHUNK_LISTS_NUM > // Lists number
    {
        void *dummy_ptr; //for alignment
        /** position of first free entry */
        ChunkPos free_entry;
        /** busy entries num */
        ChunkPos busy;
        /** Get chunk for given number */
        inline FixedEntry< size> *entry( ChunkPos pos);

    public:
#ifdef CHECK_CHUNKS
        void *pool;       
        /** Get chunk's first busy entry */
        inline FixedEntry< size> *firstBusyEntry();
#endif
        /** Constructor */
        inline Chunk();
        /** Check if this chunk has free entries */
        inline bool isFree() const;
        /** Check if this chunk is empty */
        inline bool isEmpty() const;
        /** Initialization of chunk */
        inline void initialize();
        /** Allocate on entry */
        inline void *allocateEntry();
        /** Deallocate one entry */
        inline void deallocateEntry( FixedEntry< size> *e);
        /** For some reason GCC asks for it :( */
        inline void operator delete( void* mem);
        /** Placement new */
        inline void *operator new ( size_t alloc_size, void* mem);
        /**
         * Operator 'delete' corresponding to placement new
         * WARNING: Compiler won't call this for deletion. 
         *          It is needed for freeing memory in case of exceptions in constructor
         */
        inline void operator delete( void* ptr, void* mem);
    };
    
    /** Constructor */
    template < size_t size> 
    Chunk< size>::Chunk()
    {
        FixedEntry< size> *e = NULL;
        
        /*
         * This loop basicly does new ( mem_ptr)[MAX_CHUNK_ENTRIES_NUM] for entries 
         * should be rewritten in explicit manner, with operator new overriding for Entry class
         */
        for ( int i = 0; i < MAX_CHUNK_ENTRIES_NUM; i++)
        {
            e = ( FixedEntry< size> *)( (UInt8 *) this 
                                       + sizeof( Chunk< size>) 
                                       + sizeof( FixedEntry< size>) * i);
            /** Initialization of every entry */
            e->setPos( i);
            e->setNextFree( i + 1);
#ifdef CHECK_ENTRY
            e->setBusy( false);
#endif
#ifdef USE_MEM_EVENTS        
            e->debugInfo().setAllocEvent( 0);
            e->debugInfo().setDeallocEvent( 0);
#endif 
#ifdef USE_REF_COUNTERS
            e->debugInfo().resetCount();
#endif
        }
        MEM_ASSERTD( e->nextFree() == UNDEF_POS, "Chunk size constant and undefined value do not match");
        free_entry = 0;
        busy = 0;
    }
    /** Placement new */
    template < size_t size> 
    void*
    Chunk< size>::operator new ( size_t alloc_size, void* mem)
    {
        return mem;
    }
    
    /** For some reason GCC asks for it :( */
    template < size_t size>
    void
    Chunk< size>::operator delete( void* mem)
    {

    }
    /**
     * Operator 'delete' corresponding to placement new
     * WARNING: Compiler won't call this for deletion. 
     *          It is needed for freeing memory in case of exceptions in constructor
     */
    template < size_t size> 
    void
    Chunk< size>::operator delete( void* ptr, void* mem)
    {
    
    }
  
    /** Get entry by number */
    template< size_t size> 
    FixedEntry< size>*
    Chunk< size>::entry( ChunkPos pos)
    {
        MEM_ASSERTD( pos != UNDEF_POS, "Requested entry with undefined number");
        return ( FixedEntry< size> *)( (UInt8 *) this 
                                  + sizeof( Chunk< size>) 
                                  + sizeof( FixedEntry< size>) * pos);
    }
   
#ifdef CHECK_CHUNKS        
    /** Get chunk's first busy entry */
    template< size_t size> 
    FixedEntry< size>*
    Chunk< size>::firstBusyEntry()
    {
        FixedEntry< size> *e = NULL;
        
        for ( int i = 0; i < MAX_CHUNK_ENTRIES_NUM; i++)
        {
            e = ( FixedEntry< size> *)( (UInt8 *) this 
                                       + sizeof( Chunk< size>) 
                                       + sizeof( FixedEntry< size>) * i);
            if ( e->isBusy())
                return e;
        }
        return NULL;
    }
#endif

    /** Check if this chunk has free entries */
    template< size_t size> 
    bool 
    Chunk< size>::isFree() const
    {
        return free_entry != UNDEF_POS;
    }
    /** Check if this chunk is empty */
    template< size_t size> 
    bool 
    Chunk< size>::isEmpty() const
    {
        return busy == 0;
    }      
    /** Allocate one entry */
    template< size_t size> 
    void*
    Chunk< size>::allocateEntry()
    {
        MEM_ASSERTD( this->isFree(), "Trying to allocated entry in a full chunk");
        
        FixedEntry< size> *e = entry( free_entry);
#ifdef CHECK_ENTRY
        e->setBusy( true);
#endif
#ifdef USE_MEM_EVENTS        
        e->debugInfo().setAllocEvent( Mem::MemMgr::instance()->allocEvent());
#endif        
        void *res = e->dataMem();
        free_entry = e->nextFree();
        busy++;
        return res;
    }
    /** Deallocate one entry */
    template< size_t size> 
    void
    Chunk< size>::deallocateEntry( FixedEntry< size> *e)
    {
        MEM_ASSERTD( busy > 0, "Trying to deallocate entry of an empty chunk");
#ifdef CHECK_ENTRY
        MEM_ASSERTD( e->isBusy(), 
                     "Trying to deallocate entry that is free. Check deallocation event ID");
        e->setBusy( false);
#endif
#ifdef USE_MEM_EVENTS        
        e->debugInfo().setDeallocEvent( Mem::MemMgr::instance()->deallocEvent());
#endif 
        e->setNextFree( free_entry);
        free_entry = e->pos();
        busy--;
    }
}; /* namespace MemImpl */
#endif /* MEM_CHUNK_H */
