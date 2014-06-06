/*
 *  sr_hash_map.h - part of SmartBody-lib
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

#ifndef SR_HASH_MAP_H
#define SR_HASH_MAP_H
#include <string>
#include <map>

#include "sbm_constants.h"

#define SR_HASH_MAP_OVERRIDE	( 0 )

///////////////////////////////////////////////////////

class srHashMapBase {

	private:
		typedef struct sr_map_entry_s	{

			char			*key;
			void			*data;
			int				claim_data;

			sr_map_entry_s	*prev, *next;

		} sr_map_entry_t;

	public:
		srHashMapBase( unsigned int size = 128 );
		srHashMapBase( srHashMapBase & map );
		virtual ~srHashMapBase( void );		

		void	print( int reverse = 0 );
		int		get_num_entries( void )	{ return( entry_count ); }
		bool	key_in_use( const char *key ) { return( lookup( key ) != NULL ); }

		int		insert( const char *key, void *data, int claim = FALSE );
		void	*lookup( const char *key );
		void	*remove( const char *key, int *claimed_p = NULL );
		

		void	reset( void );					  // start iterator
		void	*next( char** key_ref_p = NULL ); // lookup iterator
		void	*pull( int *claimed_p = NULL );	  // remove iterator

		// checks for existence of KEY in hash table independent of the value it hashes to
//		bool	does_key_exist(const char *key ) { return( key_in_use( key ) ); } 

		// dump entries regardless of ownership: see template class: expunge()
//        void    removeAll();

	protected:
		bool shallow_copy;
		
	private:
		unsigned int	unsigned_hash_string( const char *string, unsigned int limit );
		sr_map_entry_t	*new_table_entry( const char *key, void *data, int claim, sr_map_entry_t *next );
		void			*find_table_data( sr_map_entry_t *bucket_p, const char *key );
		
		void			increment_iterator(void);
		void			decrement_iterator(void);

		int				curr_bucket;
		sr_map_entry_t	*iterator_p;

		unsigned int	bucket_count;
		sr_map_entry_t	**bucket_pp;

		int				entry_count;
};

template <class X> class srHashMap : public srHashMapBase	{
	
	public:
		srHashMap( unsigned int size = 128 ): srHashMapBase( size ) {}

		// shallow-copy constructor for multiple-iteration
		srHashMap( srHashMap<X> & map ): srHashMapBase( map ) {}

		virtual ~srHashMap( void ) {
			if( shallow_copy == false )	{
				expunge();
			}
		}

		// get stl map 
		std::map<char *, X *> * get_map(){
			stl_map.clear();
			reset();
			char * animation;
			X* data;
			data = next(&animation);
			while(data!=NULL)
			{
				stl_map.insert ( std::pair<char *, X *>(animation,data) );
				data = next(&animation);
			}
			return &stl_map;
		}
		
		int insert( const char *key, X *data, int claim = FALSE )	{
			return( srHashMapBase::insert( key, (void*)data, claim ) );
		}
		
		int insert( const std::string& key, X *data, int claim = FALSE )	{
			return( insert( key.c_str(), data, claim ) );
		}
		
		// claim: hashmap assumes ownership of entry, and is responsible for deletion
		int insert_claim( const char *key, X *data )	{
			return( srHashMapBase::insert( key, (void*)data, TRUE ) );
		}

		X* lookup( const char *key )	{
			return( (X*)srHashMapBase::lookup( key ) );
		}

		X* lookup( const std::string& key )	{
			return( lookup( key.c_str() ) );
		}

		X* remove( const char *key, int *claimed_p = NULL )	{
			return( (X*)srHashMapBase::remove( key, claimed_p ) );
		}

		X* next( char** key_ref_p = NULL )	{
			return( (X*)srHashMapBase::next( key_ref_p ) );
		}

		X* pull( int *claimed_p = NULL )	{
			return( (X*)srHashMapBase::pull( claimed_p ) );
		}
		
		void expunge( void )	{
			int claimed = FALSE;
			X* xp;
			reset();
			while( ( xp = (X*)pull( &claimed ) )!= NULL )	{
				if( claimed )	{
					delete xp;
				}
			}
		}

	private:
		std::map<char *, X *> stl_map;
};

///////////////////////////////////////////////////////
#endif
