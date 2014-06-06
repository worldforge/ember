/*
 *  sk_joint_name.h - part of Motion Engine and SmartBody-lib
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
 */

# ifndef SK_JOINT_NAME_H
# define SK_JOINT_NAME_H

#if 0
# include <sr/sr_hash_table.h>

/*! SkJointName contains only one integer id, which is the id of
    a name stored in a globally defined hash table.
    The hash table management is transparent and joint names 
    comparison is thus performed simply by an integer comparison
    (name comparison is always case-insensitive). */
class SkJointName
 { private :
    int _id; // the id of this joint name
    static SrHashTableBase _htable;

   public :

    /*! Constructor initializing this name as undefined, ie, with index -1 */
    SkJointName () { _id=-1; }

    /*! Constructor initializing this name with the same id as in jn */
    SkJointName ( const SkJointName& jn ) { _id=jn._id; }

    /*! Constructor initializing this name with the given string, which is inserted
        in the global hash table (if not already there) */
    SkJointName ( const char* st ) { this->operator=(st); }

    /*! Returns true if the name is undefined, ie, its index is -1 */
    bool undefined () const { return _id==-1? true:false; }

    /*! Inserts the given string in the global hash table (if not already there)
        and stores its index as identifier of the SkJointName.
        If the given st is null, the string is set as undefined, ie, with index -1. */
    void operator= ( const char* st );
    
    /*! Assigns to this name the same id as in jn */
    void operator= ( const SkJointName& jn ) { _id=jn._id; }
    
    /*! Comparison operator simply compares ids */
    bool operator== ( const SkJointName& jn ) { return _id==jn._id? true:false; }

    /*! Comparison operator with a string without inserting the string
        in the hash table */
    bool operator== ( const char* st );

    /*! Type cast to a const char pointer ("" is returned in case the name is undefined).
        Note: this type cast must be used when sending a SkJointName to LOG() */
    operator const char* () const { return _id<0? "":_htable.key(_id); }
    
	/* thiebaux 6/19/2006 */
	const char * get_string( void ) { return _id<0? "":_htable.key(_id); }

    /*! Returns the unique id of this name; used for debug purposes only */
    int id () const { return _id; }
    
    /*! Returns true if the string exists among all joint names */
    static bool exist ( const char* name );

   private :
    void _check () { if ( _htable.hash_size()==0 ) _htable.init(256); }
 };

//==================================== End of File ===========================================

#endif
# endif  // SK_JOINT_NAME_H
