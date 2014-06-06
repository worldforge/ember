/*
 *  sr_buffer.cpp - part of Motion Engine and SmartBody-lib
 *  Copyright (C) 2008  University of Southern California
 *
 *  SmartBody-lib is free software: you can redistribute it and/or
 *  modify it under the terms of the Lesser GNU General Public License
 *  as published by the Free Software Foundation, version 3 of the
 *  license.
 *
 *  SmartBody-lib is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  Lesser GNU General Public License for more details.
 *
 *  You should have received a copy of the Lesser GNU General Public
 *  License along with SmarBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcelo Kallmann, USC (currently UC Merced)
 *      Andrew n marshall, USC
 */

# include <stdlib.h>
# include <string.h>

# include <sr/sr_buffer.h>

//=== SrBuffer =======================================================================

# define BUF(i) ((char*)buffer)+(sizeofx*(i))

void* sr_buffer_size ( void* buffer, int sizeofx, int& size, int newsize )
 { 
   if ( size==newsize ) return buffer;
   size = newsize;
   if ( size==0 )
    { if (buffer) { free(buffer); buffer=0; } }
   else
    { buffer = realloc ( buffer, (size_t)(sizeofx*size) ); }
   return buffer;
 }

void* sr_buffer_insert ( void* buffer, int sizeofx, int& size, int i, int dp )
 { 
   size += dp;
   buffer = realloc ( buffer, (size_t)(sizeofx*size) );
   if ( i<size-dp )
     memmove ( BUF(i+dp), BUF(i), sizeofx*(size-dp-i) ); // ok with overlap
   return buffer; 
 }

void* sr_buffer_remove ( void* buffer, int sizeofx, int& size, int i, int dp )
 { 
   if ( i<size-dp ) memmove ( BUF(i), BUF(i+dp), sizeofx*(size-(i+dp)) );
   return sr_buffer_size ( buffer, sizeofx, size, size-dp );
 }

void* sr_buffer_copy ( void* buffer, int sizeofx, int& size, const void* buffertocp, int sizetocp )
 { 
   if ( buffer==buffertocp ) return buffer;
   buffer = sr_buffer_size ( buffer, sizeofx, size, sizetocp );
   if ( buffer ) memcpy ( buffer, buffertocp, sizeofx*size ); // no overlap
   return buffer;
 }

# undef BUF

//=== End of File =====================================================================

/* Note:
void *memmove( void *dest, const void *src, size_t count );
If some regions of the source area and the destination overlap,
memmove ensures that the original source bytes in the overlapping
region are copied before being overwritten

void *memcpy( void *dest, const void *src, size_t count );
If the source and destination overlap, memcpy function does not
ensure that the original source bytes in the overlapping region
are copied before being overwritten. Use memmove to handle
overlapping regions. */




