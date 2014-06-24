/*
   This file is part of VHMsg written by Edward Fast at 
   University of Southern California's Institute for Creative Technologies.
   http://www.ict.usc.edu
   Copyright 2008 Edward Fast, University of Southern California

   VHMsg is free software: you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   VHMsg is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with VHMsg.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "vhcl.h"

#if defined(WIN_BUILD)

// "Memory Leak Detection Enabling" http://msdn.microsoft.com/en-us/library/e5ewb1h3(v=VS.90).aspx

#include <windows.h>
#include <malloc.h>
#include <stdlib.h>
#include <crtdbg.h>


using namespace vhcl;


void Memory::EnableDebugFlags( const uint32_t flags )
{
#if defined( DEBUG_BUILD )
   _CrtSetDbgFlag( flags );

   //_CrtMemState  state;
   //_CrtMemCheckpoint( &state );
   //_CrtMemDumpStatistics( &state );
#endif
}


void Memory::OutputStatistics()
{
#if defined( DEBUG_BUILD )
   OutputDebugString( "-- Memory Statistics -----------------------\n" );
   _CrtMemState  state;
   _CrtMemCheckpoint( &state );
   _CrtMemDumpStatistics( &state );
   OutputDebugString( "--------------------------------------------\n" );
#endif
}


size_t Memory::GetMemAllocated()
{
   // This gets how much memory is currently allocated
#if defined( DEBUG_BUILD )
   _CrtMemState  mState;
   _CrtMemCheckpoint( &mState );
   return mState.lSizes[ _NORMAL_BLOCK ];
#else
   return 0;
#endif
}


size_t Memory::GetNumAllocs()
{
   // This gets the total number of memory allocations
#if defined( DEBUG_BUILD )
   _CrtMemState  mState;
   _CrtMemCheckpoint( &mState );
   return mState.lCounts[ _NORMAL_BLOCK ];
#else
   return 0;
#endif
}


size_t Memory::GetTotalMemAllocated()
{
   // This gets the total memory allocated (includes all memory (CRT, freed, etc)
#if defined( DEBUG_BUILD )
   _CrtMemState  mState;
   _CrtMemCheckpoint( &mState );
   return mState.lTotalCount;
#else
   return 0;
#endif
}


size_t Memory::GetHighestMemAllocated()
{
   // This gets the highest memory allocated during the lifetime of the program
#if defined( DEBUG_BUILD )
   _CrtMemState  mState;
   _CrtMemCheckpoint( &mState );
   return mState.lHighWaterCount;
#else
   return 0;
#endif
}


#else  // WIN_BUILD

// TODO: linux

using namespace vhcl;

void Memory::EnableDebugFlags( const uint32_t flags ) {}
void Memory::OutputStatistics() {}
size_t Memory::GetMemAllocated() { return 0; }
size_t Memory::GetNumAllocs() { return 0; }
size_t Memory::GetTotalMemAllocated() { return 0; }
size_t Memory::GetHighestMemAllocated() { return 0; }

#endif
