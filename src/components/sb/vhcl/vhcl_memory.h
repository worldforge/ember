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

#ifndef VHCL_MEMORY_H
#define VHCL_MEMORY_H


#include <stddef.h>


namespace vhcl
{
   class Memory
   {
      // CRT Debugging flags - Search help:
      //   _CrtSetDbgFlag
      // Memory Leaks - To find out where a memory leak happens:
      //   Rerun the programs and set this variable in the debugger
      //   to the alloc # that leaks: {,,msvcr90d.dll}_crtBreakAlloc
      //   The program will stop right before the alloc happens

      public:
         static const uint32_t ALLOC_MEM_DF        = 0x00000001;  // enable debug heap
         static const uint32_t DELAY_FREE_MEM_DF   = 0x00000002;  // keep free'd memory on the heap
         static const uint32_t CHECK_ALWAYS_DF     = 0x00000004;  // call _CrtCheckMemory at every alloc, slow but effective
         static const uint32_t CHECK_CRT_DF        = 0x00000010;  // check blocks allocated by the CRT (internal checking)
         static const uint32_t LEAK_CHECK_DF       = 0x00000020;  // enable leak checking at the end of the program
         static const uint32_t CHECK_EVERY_16_DF   = 0x00100000;  // check heap every 16 heap ops
         static const uint32_t CHECK_EVERY_128_DF  = 0x00800000;  // check heap every 128 heap ops
         static const uint32_t CHECK_EVERY_1024_DF = 0x04000000;  // check heap every 1024 heap ops

         static const uint32_t MEM_DEFAULT_FLAGS = ALLOC_MEM_DF | LEAK_CHECK_DF;


      public:
         static void EnableDebugFlags( const uint32_t flags = MEM_DEFAULT_FLAGS );

         static void OutputStatistics();
         static size_t GetMemAllocated();
         static size_t GetNumAllocs();
         static size_t GetTotalMemAllocated();
         static size_t GetHighestMemAllocated();
   };

};


#endif  // VHCL_MEMORY_H
