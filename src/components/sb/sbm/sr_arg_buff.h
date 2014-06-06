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
 */

#ifndef SR_ARG_BUFFER_H
#define SR_ARG_BUFFER_H

#include <sb/SBTypes.h>
#include <stdlib.h>




//////////////////////////////////////////////////////////////////

class SBAPI srArgBuffer	{
    public:
		srArgBuffer( const char *arg );
		virtual ~srArgBuffer( void );
		
        /**
         *  Returns a count remaining of tokens.
         */
		int calc_num_tokens();

        /**
         *  Returns the next token.  Depending on the first non-whitespace
         *  character, tokens are defined to be either whitespace delimited
         *  or quoted with '"' (inch mark/double quote) characters. In the
         *  case of quoted tokens, this method does not check for whitespace
         *  after the end quote, or even if there is an end quote.
         *
         *  Returns EMPTY_STRING if no more tokens remain.
         *
         *  Tokens returned are owned by the srArgBuffer, and should not be
         *  freed or deleted.
         */
		char *read_token( void );		

        /**
         *  Read the remainder of the argument buffer as a single string.
         *  Does not do any token or quote processing. After calling this 
		 *  function, no more tokens will remain.
         *
         *  The string returned is owned by the srArgBuffer, and should not
         *  be freed or deleted.
         */
		char *read_remainder_raw( void );


		/**
         *  Read the remainder of the argument buffer as a single string.
         *  Does not do any token or quote processing. After calling this
		 *  function, the args can be processed as usual and will not be
		 *  removed (as is done by the read_remainder_raw function).
         *
         *  The string returned is owned by the srArgBuffer, and should not
         *  be freed or deleted.
         */
        char *peek_string( void );

        /////////////////////////////////////////////////////////////////////
        //  Inline convience functions...

        /**
         *  Reads n tokens into an n length C string array.
         */
        void read_token_vect( char** sp, int n )	{
	        for( int i=0; i<n; i++ )	{
		        sp[ i ] = read_token();
	        }
        }
		
        /**
         *  Returns a token as an integer, as interpretted by atoi().
         */
		int		read_int( void )	{ return( atoi( read_token() ) ); }

        /**
         *  Returns a series of token as an array of integer, 
         *  as interpretted by atoi().
         */
        void	read_int_vect( int *ip, int n );

        /**
         *  Returns a token as an double float, as interpretted by atof().
         */
        float	read_float( void )	{ return( (float)atof( read_token() ) ); }

        /**
         *  Returns a series of token as an array of floats, 
         *  as interpretted by atof().
         */
		void	read_float_vect( float *fp, int n )	{
			for( int i=0; i<n; i++ )	{
				fp[ i ] = read_float();
			}
		}

        /**
         *  Returns a token as an double float, as interpretted by atof().
         */
        double	read_double( void )	{ return( atof( read_token() ) ); }

        /**
         *  Returns a series of token as an array of doubles, 
         *  as interpretted by atof().
         */
		void	read_double_vect( double *fp, int n )	{
			for( int i=0; i<n; i++ )	{
				fp[ i ] = read_double();
			}
		}


		

    private:
		int arg_len;
		char *tok_buff;
		char *token;

};

//////////////////////////////////////////////////////////////////
#endif
