/*
 *  sr_arg_buff.h - part of SmartBody-lib
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
 *      Ed Fast, USC
 */

#include	<stdio.h>
#include	<string.h>

#include "sr_arg_buff.h"

#include "sbm_constants.h"


//////////////////////////////////////////////////////////////////

srArgBuffer::srArgBuffer( const char *arg )	{
	if( arg == NULL )	{
		arg_len = 0;
		tok_buff = new char;
		tok_buff[0] = '\0';  // Empty string
	}
	else	{
		arg_len = strlen( arg );
		tok_buff = new char[ arg_len + 1 ];
        strcpy( tok_buff, arg );
	}
	token = NULL;
}

srArgBuffer::~srArgBuffer( void )	{
	delete [] tok_buff;
}

int srArgBuffer::calc_num_tokens( /*int quote_sensitive = FALSE*/ )	{
	int tok_count = 0;
	char *peek_buff = peek_string();
	if( peek_buff == EMPTY_STRING )	{
		return( 0 );
	}
	srArgBuffer tmp_buff( peek_buff );
	//if( quote_sensitive )	{
	//	while( tmp_buff.read_quote() != EMPTY_STRING )	{
	//		tok_count++;
	//	}
	//}
	//else	{
		//while( tmp_buff.read_token() != EMPTY_STRING )	{
		while( tmp_buff.read_token()[0] != '\0' )	{
			tok_count++;
		}
	//}
	return( tok_count );
}

//// Old count quoted
//int srArgBuffer::calc_num_quotes( void )	{
//	return( calc_num_tokens( TRUE ) );
//}

//// Old read unquoted token
//char* srArgBuffer::read_token( void )	{
//	char *peek_buff = peek_string();
//	if( token == NULL )	{
//		token = EMPTY_STRING;
//	}
//	return( token );
//}


char* srArgBuffer::read_token( void )	{
	char *peek_buff = peek_string();
	size_t spaces = strspn( peek_buff, WHITE_SPACE );
	peek_buff += spaces;
	if( peek_buff[ 0 ] == '"' )	{
    	token = strtok( peek_buff, "\"" );  // read 'til next quote or end of string
    } else {
    	token = strtok( peek_buff, WHITE_SPACE );
    }
	if( token == NULL )	{
		token = (char*)EMPTY_STRING;
	}
	return( token );
}


char* srArgBuffer::read_remainder_raw( void )	{
	token = peek_string();
	return( token );
}


char* srArgBuffer::peek_string( void )	{
	if( token == NULL )	{
		return( tok_buff );
	}
	int tok_len = strlen( token );
	if( ( tok_len > 0 ) && ( arg_len > ( ( token - tok_buff ) + tok_len ) ) )	{
		return( token + tok_len + 1 );
	}
	return( (char*)EMPTY_STRING );
}
