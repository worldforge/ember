/*
 *  sr_linear_curve.cpp - part of SmartBody-lib
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

#include "sbm/lin_win.h"
#include "sr_spline_curve.h"

using namespace gwiz;

//////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////

const char *srSplineCurve::algorithm_label( const int index ) {

	switch( index )	{
		case ALG_SIMPLE: 			return( "simple" );
		case ALG_HALTING: 			return( "halting" );
		case ALG_CATMULL:			return( "catmull" );
		case ALG_CARDINAL_C:		return( "cardinal" );
		case ALG_CARDINAL_ALT_C:	return( "cardinal_alt" );
		case ALG_KBARTELS_TBC:		return( "kbartels" );
	}
	return( "UNKNOWN" ); // default err
}

const char *srSplineCurve::extend_label( const int index ) {

	switch( index )	{
		case EXTEND_NONE: 		return( "none" );
		case EXTEND_REPEAT: 	return( "repeat" );
		case EXTEND_MIRROR: 	return( "reflect" );
		case EXTEND_DECEL:		return( "decel" );
		case EXTEND_ACCEL:		return( "accel" );
	}
	return( "UNKNOWN" ); // default err
}

const char *srSplineCurve::insert_label( const int index ) {

	switch( index )	{
		case INSERT_KEYS: 	return( "keys" );
		case INSERT_NODES: 	return( "nodes" );
	}
	return( "UNKNOWN" ); // default err
}

int srSplineCurve::algorithm_index( const char *label ) {

	if( LinWin_strcmp( label, "simple" ) == 0 ) return( ALG_SIMPLE );
	if( LinWin_strcmp( label, "halting" ) == 0 ) return( ALG_HALTING );
	if( LinWin_strcmp( label, "catmull" ) == 0 ) return( ALG_CATMULL );
	if( LinWin_strcmp( label, "cardinal" ) == 0 ) return( ALG_CARDINAL_C );
	if( LinWin_strcmp( label, "cardinal_alt" ) == 0 ) return( ALG_CARDINAL_ALT_C );
	if( LinWin_strcmp( label, "kbartels" ) == 0 ) return( ALG_KBARTELS_TBC );
	return( -1 );
}

int srSplineCurve::extend_index( const char *label ) {

	if( LinWin_strcmp( label, "none" ) == 0 ) return( EXTEND_NONE );
	if( LinWin_strcmp( label, "repeat" ) == 0 ) return( EXTEND_REPEAT );
	if( LinWin_strcmp( label, "mirror" ) == 0 ) return( EXTEND_MIRROR );
	if( LinWin_strncmp( label, "decelerate", 5 ) == 0 ) return( EXTEND_DECEL );
	if( LinWin_strncmp( label, "accelerate", 5 ) == 0 ) return( EXTEND_ACCEL );
	return( -1 );
}

int srSplineCurve::insert_index( const char *label ) {

	if( LinWin_strcmp( label, "keys" ) == 0 ) return( INSERT_KEYS );
	if( LinWin_strcmp( label, "nodes" ) == 0 ) return( INSERT_NODES );
	return( -1 );
}

//////////////////////////////////////////////////////////////////

void srSplineCurve::Key::print( int i ) {

	printf( " key[ %d ]: ( %f, %f )\n", i, p(), v() );
}

void srSplineCurve::Node::print( int i ) {
	
	printf( " node[ %d ]: ( %f, %f ) m( %f, %f ) d( %f, %f )", 
		i, p(), v(),
		ml(), mr(),
		dl(), dr()
	);
	if( ref_key_p ) {
		printf( ": k( %f, %f )", 
			ref_key_p->p(), 
			ref_key_p->v()
		);
	}
	printf( "\n" );
}

void srSplineCurve::print( void )	{

	if( dirty ) update();
	printf( "srSplineCurve: KEYS: %d[%d]\n", key_count, node_count );
	
	int c = 0;
	Key *key_p = head_key_p;
	while( key_p )	{
		key_p->print( c++ );
		key_p = key_p->next();
	}
	
	c = 0;
	Node *node_p = head_node_p;
	while( node_p )	{
		node_p->print( c++ );
		node_p = node_p->next();
	}

	if( dirty ) printf( " DIRTY!\n" );
	printf( " ALG: %s\n", algorithm_label( algorithm ) );
	printf( " INS: %s\n", insert_label( insert_mode ) );
	printf( " INS: tail %s\n", insert_tail ? "TRUE" : "FALSE" );
	
	printf( " HEAD Key: " );
	if( head_key_p ) head_key_p->print();
	else printf( "<null>\n" );
	
	printf( " TAIL Key: " );
	if( tail_key_p ) tail_key_p->print();
	else printf( "<null>\n" );
	
	printf( " HEAD Node: " );
	if( head_node_p ) head_node_p->print();
	else printf( "<null>\n" );
	
	printf( " TAIL Node: " );
	if( tail_node_p ) tail_node_p->print();
	else printf( "<null>\n" );
}

//////////////////////////////////////////////////////////////////

void srSplineCurve::clear( void )	{

	Key *key_p = head_key_p;
	while( key_p )	{
		Key *tmp_p = key_p;
		key_p = key_p->next();
		delete tmp_p;
		decrement_key();
	}
	if( key_count != 0 )	{
		printf( "key_count ERR: %d\n", key_count );
	}
	clear_nodes();
	curr_edit_key_p = NULL;
	curr_query_key_p = NULL;
	init();
}

void srSplineCurve::clear_nodes( void )	{

	Node *node_p = head_node_p;
	while( node_p )	{
		Node *tmp_p = node_p;
		node_p = node_p->next();
		delete tmp_p;
		decrement_node();
	}
	if( node_count != 0 )	{
		printf( "node_count ERR: %d\n", node_count );
	}
	head_node_p = NULL;
	tail_node_p = NULL;
	curr_query_node_p = NULL;
}

//////////////////////////////////////////////////////////////////

#define DEBUG_INSERT_COND	0

bool srSplineCurve::insert( double p, double v )	{

//	if( dirty ) update();
	if( insert_mode == INSERT_NODES )	{

		if( dirty ) update();

		if( key_count == 0 )	{

			// first key:
#if DEBUG_INSERT_COND
			printf( "# prepend head: %f %f\n", p - 1.0, v );
#endif
			bool ret = insert_key( new Key( p - 1.0, v ) );
			if( ret == false ) return( ret );
			insert_tail = true;
		}
		else
		if( insert_tail == false )	{ 

			// Has been updated since last insert
			if( head_node_p && tail_node_p )	{

				if( p > tail_node_p->keyref()->p() )	{

					// beyond the tail node:
#if DEBUG_INSERT_COND
					printf( "# EDIT tail: %f %f\n", p, v );
					printf( "# REQ append: %f %f\n", p + 1.0, v );
#endif
					insert_tail = true;
					return( edit_tail( p, v ) );
				}
				else
				if( p < head_node_p->keyref()->p() )	{

					// before the head node:
#if DEBUG_INSERT_COND
					printf( "# EDIT head: %f %f\n", p, v );
#endif
					if( edit_head( p, v ) )	{
#if DEBUG_INSERT_COND
						printf( "# INSERT new head: %f %f\n", p - 1.0, v );
#endif
						return( insert_key( new Key( p - 1.0, v ) ) );
					}
					return( false );
				}
#if DEBUG_INSERT_COND
				printf( "# pass through\n" );
#endif
			}
			else	{
				printf( "srSplineCurve::insert WARN: head:%d tail:%d\n", 
					(int)(head_node_p!=NULL), (int)(tail_node_p!=NULL) );
			}
		}
		else	{
#if DEBUG_INSERT_COND
			printf( "# fall through...\n" );
#endif
		}
	}
#if DEBUG_INSERT_COND
	printf( "# INSERT: %f %f\n", p, v );
#endif
	return( insert_key( new Key( p, v ) ) );
}

//////////////////////////////////////////////////////////////////

#if 0
#define Q_SPAN_EPS	0.1
#define EVAL_EPS	0.2
#else
#define Q_SPAN_EPS	0.0
#define EVAL_EPS	epsilon4()
#endif

#define DEBUG_END_COND	0

#define DEBUG_EXTEND_COND	0

/////////////////////////////////

double srSplineCurve::evaluate( double t, bool *cropped_p ) {

	if( dirty ) update(); // always...
	if( cropped_p ) { *cropped_p = false; }

	Node *node_p = find_floor_node( t );
	
	if( node_p )	{
		Node *next_p = node_p->next();

		if( next_p )	{
			return( hermite( t, *node_p, *next_p ) );
		}
//		if( t <= ( node_p->p() + epsilon4() ) ) {
		if( t <= ( node_p->p() + EVAL_EPS ) ) {
			// capture end node
			return( node_p->v() );
		}
#if DEBUG_END_COND
printf( "eval: t:%f <drop>\n", t );
#endif
	}
#if DEBUG_END_COND
else { printf( "eval: t:%f [crop]\n", t ); }
#endif
	if( cropped_p ) { *cropped_p = true; }
	return( 0.0 );
}

//////////////////////////////////////////////////////////////////

bool srSplineCurve::insert_key( Key *key_p )	{

	if( key_p ) {

		Key *floor_p = find_floor_key( key_p->p() );
		if( floor_p )	{

			insert_after_key( floor_p, key_p );
			return( true );
		}
		insert_head_key( key_p );
		return( true );
	}
	return( false ); // error
}

void srSplineCurve::insert_head_key( Key *key_p ) {

	if( key_p ) {
		key_p->next( head_key_p );
		head_key_p = key_p;
		increment_key();
	}
}

void srSplineCurve::insert_after_key( Key *prev_p, Key *key_p ) {
	
	if( prev_p )	{
		Key *next_p = prev_p->next();
		prev_p->next( key_p );
		key_p->next( next_p );
		increment_key();
	}
}

//////////////////////////////////////////////////////////////////

srSplineCurve::Key *srSplineCurve::find_floor_key( double t )	{

//	if( dirty ) update(); /* only if it affects search */
	Key *key_p = head_key_p;
	if( key_p ) {
		if( t < key_p->p() )	{
			return( NULL );
		}
	}
	while( key_p )	{
		Key *next_p = key_p->next();
		if( next_p )	{
			if( t < next_p->p() ) {
				return( key_p );
			}
			else	{
				key_p = next_p;
			}
		}
		else	{
			return( key_p );
		}
	}
	return( NULL );
}

srSplineCurve::Node *srSplineCurve::find_floor_node( double t )	{

	if( dirty ) update();
	Node *node_p = head_node_p;
	if( node_p ) {
		if( t < node_p->p() )	{
#if DEBUG_END_COND
printf( "find: t:%f -clip-\n", t );
#endif
			return( NULL );
		}
	}
	while( node_p )	{
		Node *next_p = node_p->next();
		if( next_p )	{
			if( t < next_p->p() ) {
				return( node_p );
			}
			else	{
				node_p = next_p;
			}
		}
		else	{
			return( node_p );
		}
	}
#if DEBUG_END_COND
printf( "find: t:%f =drop=\n", t );
#endif
	return( NULL );
}

//////////////////////////////////////////////////////////////////

bool srSplineCurve::edit_head( double t, double v )	{

	if( head_key_p )	{
		head_key_p->set( t, v );
		dirty = true;
		return( true );
	}
	return( false );
}

bool srSplineCurve::edit_tail( double t, double v )	{

	if( tail_key_p )	{
		tail_key_p->set( t, v );
		dirty = true;
		return( true );
	}
	return( false );
}

//////////////////////////////////////////////////////////////////

void srSplineCurve::build_tail( void ) {

	if( key_count >= 2 ) { // at least 2 nodes to register an interval, with default eps4()...

		Key *key0_p = head_key_p;
		Key *key1_p = head_key_p->next();
		Key *key2_p = key1_p->next();

		if( key2_p == NULL )	{

			double p = key1_p->p();
			double v = key1_p->v();
// printf( "inserting tail key: A: after k( %f %f )\n", p, v );

			bool ret = insert_key( new Key( p + 1.0, v ) );
			if( ret == false )	{
				printf( "srSplineCurve::build_tail ERR: new Key FAILED\n" );
				return;
			}
			insert_tail = false;
			key2_p = key1_p->next();
			if( key2_p == NULL )	{
				printf( "srSplineCurve::build_tail major ERROR\n" );
				return;
			}
		}

		while( insert_tail ) {

			// swap-cycle:
			key0_p = key1_p;
			key1_p = key2_p;
			key2_p = key2_p->next();

			if( key2_p == NULL )	{
			
				double p = key1_p->p();
				double v = key1_p->v();
// printf( "inserting tail key: B: after k( %f %f )\n", p, v );

				bool ret = insert_key( new Key( p + 1.0, v ) );
				if( ret == false )	{
					printf( "srSplineCurve::build_tail ERR: new Key FAILED\n" );
					return;
				}
				insert_tail = false;
				key2_p = key1_p->next();
				if( key2_p == NULL )	{
					printf( "srSplineCurve::build_tail minor ERROR\n" );
				}
			}
		}
		tail_key_p = key2_p;
	}
	return;
}

bool srSplineCurve::extend_head( void )	{

	if( head_ext_mode == EXTEND_NONE ) return( false );

	if( head_key_p )	{

		Key *k1_p = head_key_p->next();
		if( k1_p )	{

			Key *k2_p = k1_p->next();
			if( k2_p )	{

#if 1
				if( k2_p->next() == NULL )	{
					return( edit_head( k1_p->p() - 1.0, k1_p->v() ) );
				}
#endif
				if( head_ext_mode == EXTEND_REPEAT )	{
					double new_p = k1_p->p() - ( k2_p->p() - k1_p->p() );
					return( edit_head( new_p, k1_p->v() ) );
				}
				if( head_ext_mode == EXTEND_MIRROR )	{
					double new_p = k1_p->p() - ( k2_p->p() - k1_p->p() );
					double new_v = k1_p->v() - ( k2_p->v() - k1_p->v() );
					return( edit_head( new_p, new_v ) );
				}
				if( head_ext_mode == EXTEND_DECEL )	{
					double new_p = k1_p->p() - ( k2_p->p() - k1_p->p() );
					double new_v = k1_p->v() + ( k2_p->v() - k1_p->v() );
					return( edit_head( new_p, new_v ) );
				}

				Key *k3_p = k2_p->next();
				if( k3_p )	{
					if( head_ext_mode == EXTEND_ACCEL ) {
						ctrl_key new_head = gwiz::ssvvcc_extend( *k3_p, *k2_p, *k1_p );
						return( edit_head( new_head.p(), new_head.v() ) );
					}
				}
#if DEBUG_EXTEND_COND
 printf( "extend_head: <drop=:dead\n" );
#endif
				return( false );
			}
#if DEBUG_EXTEND_COND
 printf( "extend_head: <drop=:c: head-XX is NULL\n" );
#endif
			return( false );
		}
#if DEBUG_EXTEND_COND
 printf( "extend_head: <drop=:b: head-X is NULL\n" );
#endif
		return( false );
	}
#if DEBUG_EXTEND_COND
 printf( "extend_head: <drop=:A: head is NULL\n" );
#endif
	return( false );
}

bool srSplineCurve::extend_tail( void )	{

	if( tail_ext_mode == EXTEND_NONE ) return( false );

	if( ! tail_key_p )	{
#if DEBUG_EXTEND_COND
		printf( "srSplineCurve::extend_tail ERR: tail_key_p is NULL\n" );
#endif
		return( false );
	}

	// since we have no prev* links, start from head:
	Key *k0_p = head_key_p;
	if( k0_p )	{

		Key *k1_p = k0_p->next();
		if( k1_p )	{

			Key *k2_p = k1_p->next();
			if( k2_p )	{

				// k0, k1, k2, next...
				Key *next_p = k2_p->next();
				if( next_p == NULL )	{

#if 1
					return( edit_tail( k1_p->p() + 1.0, k1_p->v() ) );
#else
					if( tail_ext_mode >= EXTEND_ACCEL )	{
						return( false );
					}
					if( tail_ext_mode == EXTEND_REPEAT )	{
						double new_p = k1_p->p() + ( k1_p->p() - k0_p->p() );
						return( edit_tail( new_p, k1_p->v() ) );
					}
					if( tail_ext_mode == EXTEND_MIRROR )	{
						double new_p = k1_p->p() + ( k1_p->p() - k0_p->p() );
						double new_v = k1_p->v() + ( k1_p->v() - k0_p->v() );
						return( edit_tail( new_p, new_v ) );
					}
					if( tail_ext_mode == EXTEND_DECEL )	{
						double new_p = k1_p->p() + ( k1_p->p() - k0_p->p() );
						double new_v = k1_p->v() - ( k1_p->v() - k0_p->v() );
						return( edit_tail( new_p, new_v ) );
					}

// printf( "srSplineCurve::extend_tail ERR: next_p == NULL\n" );
					// back up a step:
//						k2_p = k1_p;
//						k1_p = k0_p;
#endif

				}
				else
				while( next_p != tail_key_p ) {

					// swap-cycle:
					k0_p = k1_p;
					k1_p = k2_p;
					k2_p = next_p;
					next_p = next_p->next();
					if( next_p == NULL )	{
						printf( "srSplineCurve::extend_tail ERR: tail NOT FOUND!\n" );
						return( false );
					}
				}

				if( tail_ext_mode == EXTEND_REPEAT )	{
					double new_p = k2_p->p() + ( k2_p->p() - k1_p->p() );
					return( edit_tail( new_p, k2_p->v() ) );
				}
				if( tail_ext_mode == EXTEND_MIRROR )	{
					double new_p = k2_p->p() + ( k2_p->p() - k1_p->p() );
					double new_v = k2_p->v() + ( k2_p->v() - k1_p->v() );
					return( edit_tail( new_p, new_v ) );
				}
				if( tail_ext_mode == EXTEND_DECEL )	{
					double new_p = k2_p->p() + ( k2_p->p() - k1_p->p() );
					double new_v = k2_p->v() - ( k2_p->v() - k1_p->v() );
					return( edit_tail( new_p, new_v ) );
				}
				if( tail_ext_mode == EXTEND_ACCEL )	{
					ctrl_key new_tail = gwiz::ssvvcc_extend( *k0_p, *k1_p, *k2_p );
					return( edit_tail( new_tail.p(), new_tail.v() ) );
				}
#if DEBUG_EXTEND_COND
				printf( "extend_tail: =drop>:dead\n" );
#endif
				return( false );
			}
#if DEBUG_EXTEND_COND
			printf( "extend_tail: =drop>:c: head-XX is NULL\n" );
#endif
			return( false );
		}
#if DEBUG_EXTEND_COND
		printf( "extend_tail: =drop>:b: head-X is NULL\n" );
#endif
		return( false );
	}
#if DEBUG_EXTEND_COND
	printf( "extend_tail: =drop>:A: head is NULL\n" );
#endif
	return( false );
}

#define COALESCE_THRESH 0.1

void srSplineCurve::update_key_order( void )	{

	if( head_key_p )	{
		double t_prev = head_key_p->p();
		Key *key_p = head_key_p->next();
		
		bool collision = false;
		bool disorder = false;

		while( key_p )	{

			double t = key_p->p();

			if( fabs( t - t_prev ) < COALESCE_THRESH )	{ // COLLISION!
			
				collision = true;
			}
			if( t_prev >= ( t - COALESCE_THRESH ) )	 { // REORDER!
			
				disorder = true;
			}

			t_prev = t;
			key_p = key_p->next();
		}
#if 0
		if( collision && reorder ) printf( "coll && reorder\n" );
		else
		if( collision ) printf( "coll\n" );
		else
		if( disorder ) printf( "disorder\n" );
#elif 1
		if( collision || disorder ) {
			// enforce order by shuffling up/dn based on curr_edit_key_p?
			// when is curr_edit_key_p set?
			// ever by the auto-extend?
		}
#elif 1
		if( disorder )	{
			
			Key *edit_p = curr_edit_key_p;
			Key *query_p = curr_query_key_p;
			key_p = head_key_p;
			tail_key_p = key_p;
			
			curr_edit_key_p = NULL;
			curr_query_key_p = NULL;
			head_key_p = NULL;
//			key_count = 0;
//			clear();
//			clear_nodes();

			while( key_p )	{

				Key *new_p = new Key( key_p->p(), key_p->v() );
				if( key_p == edit_p )	{
					curr_edit_key_p = new_p;
				}
				if( key_p == query_p )	{
					curr_query_key_p = new_p;
				}
				insert_key( new_p );
				tail_key_p = new_p;
				
				Key *tmp_p = key_p;
				key_p = key_p->next();
				delete tmp_p;
				decrement_key();
			}

/*
	curr_query_node_p = NULL;
			Node *n_query_p = curr_query_node_p;
			
			Node *node_p = head_node_p;
			
			while( node_p )	{
				Node *tmp_p = node_p;
				node_p = node_p->next();
				delete tmp_p;
				decrement_node();
			}
*/
//			clear_nodes();
//			dirty = true;
		}
#endif
	}
}

void srSplineCurve::update( void ) {

// TODO:
	// Can we now re-order its keys, since it will scratch build the node arr?
	// Coalescent time stamps: policy not implemented, may return INF/undefined...
	//  - bump up and down? via edit()?
	//  - nodes only? Let the head/tail fly?
	// bumping back requires prev() pointers.
	// reordering breaks the APP selection hot-node.

	if( insert_tail )	{
		build_tail();
	}
#if 0
	update_key_order();
#endif
	if( auto_extensions ) {
		apply_extensions();
	}

	// build cardinal nodes from scratch...
	clear_nodes();
	// >>> possible to simply re-use?

	tail_key_p = head_key_p;
	if( key_count > 1 ) {
		tail_key_p = head_key_p->next();
	}

	if( key_count >= 3 ) { // at least 2 nodes to register an interval, with default eps4()...

		Key *key0_p = head_key_p;
		Key *key1_p = head_key_p->next();
		Key *key2_p = key1_p->next();

		// first Node:
		head_node_p = new Node;
		increment_node();
		tail_node_p = head_node_p;
		Node *node_p = head_node_p;
		node_p->keyref( key1_p );

		tail_key_p = key2_p;

		while( node_p ) {

			if( algorithm == ALG_SIMPLE )	{
				node_p->simple( (*key0_p), (*key1_p), (*key2_p) );
			}
			else
			if( algorithm == ALG_HALTING )	{
				node_p->halting( (*key0_p), (*key1_p), (*key2_p) );
			}
			else
			if( algorithm == ALG_CATMULL )	{
				node_p->catmullrom( (*key0_p), (*key1_p), (*key2_p) );
			}
			else
			if( algorithm == ALG_CARDINAL_C )	{
				node_p->cardinal( 0.5, (*key0_p), (*key1_p), (*key2_p) );
			}
			else
			if( algorithm == ALG_CARDINAL_ALT_C )	{
				node_p->cardinal_alt( 0.5, (*key0_p), (*key1_p), (*key2_p) );
			}
			else	{
				node_p->kochbartels( 0.0, 0.0, 0.0, (*key0_p), (*key1_p), (*key2_p) );
			}

			// swap-cycle:
			key0_p = key1_p;
			key1_p = key2_p;
			key2_p = key2_p->next();

			if( key2_p == NULL )	{
				node_p = NULL;
			}
			else	{
			
				Node *next_p = new Node;
				increment_node();
				next_p->keyref( key1_p );
				node_p->next( next_p );
				
				// handoff:
				node_p = next_p;
				tail_node_p = node_p;

				tail_key_p = key2_p;
			}
		}
		
		if( node_count != ( key_count - 2 ) )	{
			printf( "srSplineCurve::update ERR: %d[%d]\n", key_count, node_count );
		}
	}
	dirty = false;
}

//////////////////////////////////////////////////////////////////

bool srSplineCurve::query_span( double *t_fr_p, double *t_to_p ) {

	if( dirty ) update();
	if( head_node_p )	{
		if( tail_node_p )	{
			if( t_fr_p ) *t_fr_p = head_node_p->p();
//			if( t_to_p ) *t_to_p = tail_node_p->p();
			if( t_to_p ) *t_to_p = tail_node_p->p() + Q_SPAN_EPS;
			return( true );
		}
		printf( "srSplineCurve::query_span ERR: head node without tail!\n" );
	}
	return( false );
}

int srSplineCurve::probe_bbox_key( 
	double t, double v, 
	double radius, 
	bool set_qu, 
	bool set_ed
) {

	int c = 0;
	Key *key_p = head_key_p;
	while( key_p )	{
		if( key_p->bound_box( t, v, radius ) )	{
			if( set_qu )	{
				curr_query_key_p = key_p;
			}
			if( set_ed )	{
				curr_edit_key_p = key_p;
			}
			return( c );
		}
		key_p = key_p->next();
		c++;
	}
	return( -1 );
}

int srSplineCurve::probe_bbox_node(
	double t, double v, 
	double radius, 
	bool set_qu, 
	bool set_key_qu, 
	bool set_key_ed
)	{

	int node_c = 0;
	Node *node_p = head_node_p;
	while( node_p )	{
		if( node_p->bound_box( t, v, radius ) )	{

			if( set_qu )	{
				curr_query_node_p = node_p;
			}
			if( set_key_qu )	{
				curr_query_key_p = node_p->keyref();
			}
			if( set_key_ed )	{
				curr_edit_key_p = node_p->keyref();
			}
			return( node_c );
		}
		node_p = node_p->next();
		node_c++;
	}
	return( -1 );
}

bool srSplineCurve::edit( double t, double v, bool increment )	{

	if( curr_edit_key_p )	{
		curr_edit_key_p->set( t, v );
		dirty = true;
		if( increment ) {
			curr_edit_key_p = curr_edit_key_p->next();
		}
		return( true );
	}
	return( false );
}

bool srSplineCurve::query_key( 
	double *t_p, double *v_p, 
	bool increment 
)	{

	if( curr_query_key_p )	{
		if( t_p ) { *t_p = curr_query_key_p->p(); }
		if( v_p ) { *v_p = curr_query_key_p->v(); }
		if( increment ) {
			curr_query_key_p = curr_query_key_p->next();
		}
		return( true );
	}
	return( false );
}

bool srSplineCurve::query_node( 
	double *t_p, double *v_p, 
	double *ml_p, double *mr_p, 
	double *dl_p, double *dr_p, 
	bool increment 
)	{

	if( curr_query_node_p )	{
		if( t_p ) { *t_p = curr_query_node_p->p(); }
		if( v_p ) { *v_p = curr_query_node_p->v(); }
		if( ml_p ) { *ml_p = curr_query_node_p->ml(); }
		if( mr_p ) { *mr_p = curr_query_node_p->mr(); }
		if( dl_p ) { *dl_p = curr_query_node_p->dl(); }
		if( dr_p ) { *dr_p = curr_query_node_p->dr(); }
		if( increment ) {
			curr_query_node_p = curr_query_node_p->next();
		}
		return( true );
	}
	return( false );
}

//////////////////////////////////////////////////////////////////
