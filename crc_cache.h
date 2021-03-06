#ifndef CRC_CACHE_H
#define CRC_CACHE_H

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// This file is distributed as part of the Cache Replacement Championship     //
// workshop held in conjunction with ISCA'2010.                               //
//                                                                            //
//                                                                            //
// Everyone is granted permission to copy, modify, and/or re-distribute       //
// this software.                                                             //
//                                                                            //
// Please contact Aamer Jaleel <ajaleel@gmail.com> should you have any        //
// questions                                                                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// --- DO NOT EDIT THIS FILE --- DO NOT EDIT THIS FILE --- DO NOT EDIT THIS FILE ---
// --- DO NOT EDIT THIS FILE --- DO NOT EDIT THIS FILE --- DO NOT EDIT THIS FILE ---
// --- DO NOT EDIT THIS FILE --- DO NOT EDIT THIS FILE --- DO NOT EDIT THIS FILE ---

// IMPORTANT NOTE: DO NOT CHANGE ANYTHING IN THIS HEADER FILE. Changing anything
// in here will violate the competition rules.

#include <cassert>
#include "utils.h"
#include "replacement_state.h"
#include "crc_cache_defs.h"

class CRC_CACHE
{
  private:

    // parameters
    UINT32 numsets;
    UINT32 assoc;
    UINT32 threads;
    UINT32 linesize;
    UINT32 replPolicy;
    
    LINE_STATE               **cache;
    CACHE_REPLACEMENT_STATE  *cacheReplState;

    // statistics
    COUNTER *lookups[ ACCESS_MAX ];
    COUNTER *misses[ ACCESS_MAX ];
    COUNTER *hits[ ACCESS_MAX ];

    // Lookup Parameters
    UINT32 lineShift;
    UINT32 indexShift;
    UINT32 indexMask;

    COUNTER mytimer; 
    
  public:

    CRC_CACHE( UINT32 _cacheSize, UINT32 _assoc, UINT32 _tpc, UINT32 _linesize=64, UINT32 _pol=CRC_REPL_LRU );

    bool   CacheInspect( UINT32 tid, Addr_t PC, Addr_t paddr, UINT32 accessType );
    bool   LookupAndFillCache( UINT32 tid, Addr_t PC, Addr_t paddr, UINT32 accessType );
    ostream &   PrintStats(ostream &out);

  private:

    Addr_t GetTag( Addr_t addr ) { return ((addr >> lineShift) >> indexShift); }
    UINT32 GetSetIndex( Addr_t addr ) { return ((addr >> lineShift) & indexMask); }

    void   InitCache();
    void   InitCacheReplacementState();

    void   InitStats();

    INT32  LookupSet( UINT32 setIndex, Addr_t tag );
    INT32  GetVictimInSet( UINT32 tid, UINT32 setIndex, Addr_t PC, Addr_t paddr, UINT32 accessType );

  public:

    // Statistics related functions
    COUNTER ThreadDemandLookupStats( UINT32 tid )
    {
        COUNTER stat = 0;
        for(UINT32 a=0; a<=ACCESS_STORE; a++) stat  += lookups[a][tid];
        return stat;
    }

    COUNTER ThreadDemandMissStats( UINT32 tid )
    {
        COUNTER stat = 0;
        for(UINT32 a=0; a<=ACCESS_STORE; a++) stat  += misses[a][tid];
        return stat;
    }
    
    COUNTER ThreadDemandHitStats( UINT32 tid )
    {
        COUNTER stat = 0;
        for(UINT32 a=0; a<=ACCESS_STORE; a++) stat  += hits[a][tid];
        return stat;
    }

};

#endif
