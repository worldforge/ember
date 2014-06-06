/*
 *  sr_hash_map.cpp - part of SmartBody-lib
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
 *      Marcelo Kallmann, USC (currently at UC Merced)
 *      Marcus Thiebaux, USC
 *      Andrew n marshall, USC
 *      Ashok Basawapatna, USC (no longer)
 */

#include <stdio.h>
#include <string.h>
#include <vhcl_log.h>

#include "sr_hash_map.h"

#define EOS				'\0'
#define NULL_BUCKET		-1

///////////////////////////////////////////////////////////////////

unsigned int srHashMapBase::unsigned_hash_string( const char *string, unsigned int limit )	{
	unsigned int i = 0;
	unsigned int accum = 0;
		
#if 1
		if( string && limit ) {
			while( string[ i ] != EOS )	{
				accum = string[ i ] 
					^ ( accum << ( sizeof( char ) * 8 + 1 ) ) 
					^ ( accum >> ( ( sizeof( accum ) - sizeof( char ) ) * 8 - 1 ) );
				i++;
			}
#if SR_HASH_MAP_OVERRIDE
			srand( accum + SR_HASH_MAP_OVERRIDE );
			int r = rand();
			return( r % limit );
#else
			return( accum % limit );
#endif
		}
#endif
		return( 0 );
}

///////////////////////////////////////////////////////////////////

srHashMapBase::sr_map_entry_t *srHashMapBase::new_table_entry( const char *key, void *data, int claim, sr_map_entry_t *next )	{
	sr_map_entry_t *entry_p;

		entry_p = new sr_map_entry_t;
		entry_p->key = new char[ strlen( key ) + 1 ];
		sprintf( entry_p->key, "%s", key );
		entry_p->data = data;
		entry_p->claim_data = claim;
		entry_p->prev = NULL;
		entry_p->next = next;
		if( entry_p->next )	{
			entry_p->next->prev = entry_p;
		}
		return( entry_p );
}

void *srHashMapBase::find_table_data( sr_map_entry_t *bucket_p, const char *key )	{
		
		if( !bucket_p ) { 
			LOG( "srHashMapBase::find_table_data ERR: bucket_p is NULL \n" ); 
			return( NULL ); 
		}
		if( !key ) { 
			LOG( "srHashMapBase::find_table_data ERR: key is NULL \n" ); 
			return( NULL ); 
		}
		if( strcmp( bucket_p->key, key ) == 0 )	{
			return( bucket_p->data );
		}
		if( bucket_p->next )	{
			return( find_table_data( bucket_p->next, key ) );
		}
		return( NULL );
}

///////////////////////////////////////////////////////////////////

srHashMapBase::srHashMapBase( unsigned int size )	{
	unsigned int i;
		
		if( size < 1 )	{
			LOG( "srHashMapBase::srHashMapBase ERR: needs non-zero size\n" );
			return;
		}
		
		bucket_count = size;
		entry_count = 0;
		curr_bucket = NULL_BUCKET; 
		iterator_p = NULL;
		bucket_pp = new sr_map_entry_t * [ bucket_count ];
		for( i=0;i<bucket_count;i++ )	{
			bucket_pp[ i ] = NULL;
		}
		shallow_copy = false;
}

srHashMapBase::srHashMapBase( srHashMapBase & map )	{
	
	*this = map;
	shallow_copy = true;
}

srHashMapBase::~srHashMapBase( void )	{
		
	if( shallow_copy == false )	{
		if( entry_count > 0 )	{
			LOG( "srHashMapBase::~srHashMapBase ERR: leaking %d entries\n", entry_count );
		}
		delete [] bucket_pp;
	}
	bucket_pp = NULL;
}

void srHashMapBase::print( int reverse )	{
	unsigned int i;
	char label[512];
	sr_map_entry_t *entry_p, *last;
	
		for( i=0;i<bucket_count;i++ )	{
			if( bucket_pp[i] )	{
				sprintf( label, "[%u]:", i );
				last = NULL;
				entry_p = bucket_pp[i];
				while( entry_p )	{
					if( entry_p == iterator_p )
						sprintf( label, "%s {%s}->", label, entry_p->key );
					else
						sprintf( label, "%s %s->", label, entry_p->key );
					last = entry_p;
					entry_p = entry_p->next;
				}
				sprintf( label, "%s|", label );
				if( reverse )	{
					entry_p = last;
					while( entry_p )	{
						if( entry_p == iterator_p )
							sprintf( label, "%s {%s}->", label, entry_p->key );
						else
							sprintf( label, "%s %s->", label, entry_p->key );
						entry_p = entry_p->prev;
					}
					sprintf( label, "%s|", label );
				}
				LOG( "%s \n", label );
			}
		}
}

////////////////////////////////////////////////////////////
 
int srHashMapBase::insert( const char *key, void *data, int claim )	{
		
		if( bucket_pp == NULL ) {
			LOG( "srHashMapBase::insert ERR: bucket_pp is NULL \n" ); 
			return( CMD_FAILURE );
		}
		if( key == NULL ) { 
			LOG( "srHashMapBase::insert ERR: NULL key \n" ); 
			return( CMD_FAILURE );
		}
		
		unsigned int i = unsigned_hash_string( key, bucket_count );
		sr_map_entry_t *head_entry_p = bucket_pp[ i ];
		if( head_entry_p )	{
			if( find_table_data( head_entry_p, key ) )	{
				// commented out for now - causing noisy log messages - AS 7/24/12
				//LOG( "srHashMapBase::insert ERR: duplicate key '%s'\n", key ); 
				return( CMD_FAILURE );
			}
		}
		bucket_pp[ i ] = new_table_entry( key, data, claim, head_entry_p );
		entry_count++;
		return( CMD_SUCCESS );
}

void *srHashMapBase::lookup( const char *key )	{
	
		if( bucket_pp == NULL ) {
			LOG("srHashMapBase::lookup ERR: bucket_pp is NULL \n" ); 
			return( NULL );
		}
		if( key == NULL ) {
			LOG("srHashMapBase::lookup ERR: key is NULL \n" ); 
			return( NULL );
		}

		sr_map_entry_t *head_entry_p = bucket_pp[ unsigned_hash_string( key, bucket_count ) ];
		if( head_entry_p )	{
			return( find_table_data( head_entry_p, key ) );
		}
		return( NULL );
}

void *srHashMapBase::remove( const char *key, int *claimed_p )	{
	sr_map_entry_t *entry_p;
	void *data;
	
		if( bucket_pp == NULL ) {
			LOG( "srHashMapBase::remove ERR: bucket_pp is NULL \n" ); 
			return( NULL );
		}
		if( key == NULL ) {
			LOG( "srHashMapBase::remove ERR: key is NULL \n" ); 
			return(NULL);
		}

		unsigned int i = unsigned_hash_string( key, bucket_count );
		entry_p = bucket_pp[ i ];
		while( entry_p )	{
			if( strcmp( entry_p->key, key ) == 0 ) {

				if( iterator_p == entry_p )	{
					decrement_iterator();
				}

				if( entry_p->prev )	{
					entry_p->prev->next = entry_p->next;
				}
				else	{
					bucket_pp[i] = entry_p->next;
				}
				
				if( entry_p->next )	{
					entry_p->next->prev = entry_p->prev;
				}
				
				data = entry_p->data;
				if( claimed_p )	{
					*claimed_p = entry_p->claim_data;
				}
				
				delete [] entry_p->key;
				delete entry_p;
				entry_count--;
				return( data );
			}
			entry_p = entry_p->next;
		}
		return( NULL );
}

#if 0
void srHashMapBase::removeAll()	{

	int claimed = FALSE;
    reset();
	while( get_num_entries() )	{
        pull( &claimed );
		if( claimed )	{
			LOG( "srHashMapBase::removeAll WARNING: leaking claimed entry!\n" );
		}
	}
}
#endif

////////////////////////////////////////////////////////////
 
void srHashMapBase::reset(void)	{
		curr_bucket = NULL_BUCKET; 
		iterator_p = NULL;
}

void *srHashMapBase::next( char** key_ref_p )	{

		if( bucket_pp == NULL ) {
			LOG( "srHashMapBase::next ERR: bucket_pp is NULL \n" ); 
			return( NULL );
		}
		increment_iterator();
		if( iterator_p != NULL )	{
			if( key_ref_p ) {
				*key_ref_p = iterator_p->key;
			}
			return( iterator_p->data );
		}
		return( NULL );
}
 
void *srHashMapBase::pull( int *claimed_p )	{

		if( bucket_pp == NULL ) {
			LOG( "srHashMapBase::pull ERR: bucket_pp is NULL \n" ); 
			return( NULL );
		}
		increment_iterator();
		if( iterator_p != NULL )	{
			return( remove( iterator_p->key, claimed_p ) );
		}
		return( NULL );
}
 
////////////////////////////////////////////////////////////

void srHashMapBase::increment_iterator(void)	{

		if( bucket_pp == NULL ) {
			LOG( "srHashMapBase::increment_iterator ERR: bucket_pp is NULL \n" ); 
			return;
		}
		if( iterator_p )	{
			iterator_p = iterator_p->next;
		}
		while( ( !iterator_p )&&( curr_bucket < ( (int)bucket_count - 1 ) ) )	{
			iterator_p = bucket_pp[ ++curr_bucket ];
		}
}

void srHashMapBase::decrement_iterator(void)	{

		if( bucket_pp == NULL ) {
			LOG( "srHashMapBase::decrement_iterator ERR: bucket_pp is NULL \n" ); 
			return;
		}
		if( iterator_p )	{
			iterator_p = iterator_p->prev;
		}
		while( ( !iterator_p )&&( curr_bucket > NULL_BUCKET ) )	{

			curr_bucket -= 1;
			if( curr_bucket < 0 )	{
				iterator_p = NULL;
			}
			else	{
				iterator_p = bucket_pp[ curr_bucket ];
				if( iterator_p )	{
					while( iterator_p->next )	{
						iterator_p = iterator_p->next;
					}
				}
			}
		}
}

////////////////////////////////////////////////////////////
