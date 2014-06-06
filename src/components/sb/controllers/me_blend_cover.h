/*
 *  me_blend_cover.h - part of SmartBody-lib
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
 *  License along with SmartBody-lib.  If not, see:
 *      http://www.gnu.org/licenses/lgpl-3.0.txt
 *
 *  CONTRIBUTORS:
 *      Marcus Thiebaux, USC
 */

#ifndef ME_BLEND_COVER
#define ME_BLEND_COVER

#include "sbm/sr_hash_map.h"

class MeBlendCoverMap	{
	
	public:
	
		MeBlendCoverMap( void ) {}
		~MeBlendCoverMap( void ) {}

		void clear( void )	{
			blend_map.expunge();
		}

		float read( const char *name ) {
			float *fp = blend_map.lookup( name );
			if( fp )	{
				return( *fp );
			}
			return( 0.0 );
		}

		int update( const char *name, float f )	{
			float *fp = blend_map.lookup( name );
			if( fp )	{
				if( f > *fp )	{
					*fp = f;
				}
			}
			else	{
				float *fp = new float;
				*fp = f;
				return( blend_map.insert( name, fp, TRUE ) );
			}
			return( CMD_SUCCESS );
		}

	private:
		srHashMap <float> blend_map;
};

#endif
