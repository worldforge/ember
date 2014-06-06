/*
 *  sr_cmd_map.h - part of SmartBody-lib
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
 *      Andrew n marshall, USC
 */

#ifndef SR_CMD_MAP_H
#define SR_CMD_MAP_H

#include "sbm_constants.h"

#include "sr_hash_map.h"
#include "sr_arg_buff.h"
#include <string.h>

//////////////////////////////////////////////////////////////////

template <class X> class srCmdMap	{
	
	public:
		typedef int (*sr_cmd_callback_fp)( srArgBuffer&, X* );

		srCmdMap( unsigned int size = 128 )
            : cmdToCallback( size )
        {}

		virtual ~srCmdMap( void )	{
			cmdToCallback.reset();
			while( cmdToCallback.pull() != NULL )	{}
		}
		
		int insert( const char *key, sr_cmd_callback_fp fp )	{
			return( cmdToCallback.insert( key, (void*)fp, FALSE ) );
		}
		
		int execute( const char *key, srArgBuffer& argb, X *x_p = NULL )	{
			if( key )	{
				int keySize = strlen( key );
				if( keySize > 0 )	{
//					sr_cmd_callback_fp fp = static_cast<sr_cmd_callback_fp>( cmdToCallback.lookup( key ) );
					sr_cmd_callback_fp fp = (sr_cmd_callback_fp)( cmdToCallback.lookup( key ) );
					if( fp )	{
						return( fp( argb, x_p ) );
					}
					return( CMD_NOT_FOUND );
				}
				return( CMD_SUCCESS );
			}
			return( CMD_FAILURE );
		}

		int execute( char *cmd, X *x_p = NULL )	{
			if( cmd )	{
				srArgBuffer argb( cmd );
				char *key = argb.read_token();
				return( execute( key, argb, x_p ) );
			}
			return( CMD_FAILURE );
		}

		int is_command( const char* cmd_key ) {
//			sr_cmd_callback_fp fp = static_cast<sr_cmd_callback_fp>( cmdToCallback.lookup( cmd_key ) );
			sr_cmd_callback_fp fp = (sr_cmd_callback_fp)( cmdToCallback.lookup( cmd_key ) );

			return( fp != NULL );
		}

		void reset( ) {
			cmdToCallback.reset();
		}

		srHashMapBase& getHashMap() {
			return cmdToCallback;
		}
		


    protected:
        srHashMapBase cmdToCallback;
};

//////////////////////////////////////////////////////////////////
#endif
