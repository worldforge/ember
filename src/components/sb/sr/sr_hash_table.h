/*
 *  sr_hash_table.h - part of Motion Engine and SmartBody-lib
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
 */

/** Hash table functions */

# ifndef SR_HASH_TABLE_H
# define SR_HASH_TABLE_H

#include <sb/SBTypes.h>
# include <sr/sr_array.h>
# include <sr/sr_shared_class.h>

#define SR_HASH_TABLE_OVERRIDE	( 0 )

//================================ SrHashTableBase ===============================

/*! \class SrHashTableBase sr_hash_table.h
    Stores user data associated with string keys in a hash table. 
    Note1: this is a rather specific implementation: colliding elements 
    are appended to the same array of entries, this is very efficient for inserting
    elements but less practical for removing elements (removal was not implemented).
    Note2: the user is responsible for allocation/deallocation of the appended user data,
    which is merely stored as given void pointers, more info about that in the SrHashTable class. */
class SBAPI SrHashTableBase
 { protected:
    struct Entry { char* st;     // the string id of this entry, or null if empty entry
                   void* data;   // the user data associated or null if none
                   int next;     // the index of the next colliding item
                   char dynamic; // 1 if st was created with new operator, 0 if st is static
                 };

    SrArray<Entry> _table;
    SrArray<int> _free;
    int _hash_size;
    int _elements; 
    int _last_id;
    
   public:

    /*! Default constructor creates a table of given hash size. If the hash size
        is 0 (the default value), init() must be properly called aftwerwards. */
    SrHashTableBase ( int hsize=0 );

    /*! Destructor */
    virtual ~SrHashTableBase ();

    /*! Destroy actual table and builds a new empty one with the given hash size.
        A value of zero will make the hash table empty and unusable. */
    void init ( int hsize );
    
    /*! Rebuilds the table with a different hash size */
    void rehash ( int new_hsize );
    
    /*! Returns the table size, which can be greater than the initial
        given hash size in case there were collisions */
    int size () const { return _table.size(); }

    /*! Returns the initial (minimum) size of the table */
    int hash_size () const { return _hash_size; }

    /*! Returns the number of colliding elements in the table */
    int collisions () const { return _table.size()-_free.size()-_hash_size; }

    /*! Calculates and returns the maximum number of comparisons
        that can happen when searching for a string in the table, 
        ie, the longest list size associated with a same entry in 
        the hash table. If there are no collisions, 1 is returned. */
    int longest_entry () const;
    
    /*! Total number of elements inserted in the table */
    int elements () const { return _elements; }

    /*! Returns the next index of an entry colliding with index id.
        -1 is returned in case there is no further "colliding index" */
    int collidingid ( int id ) const { return _table[id].next; }

    /*! Returns the string key associated with the given id (can be null).
        No validity checkings in the index are done! */
    const char* key ( int id ) const { return _table[id].st; }

    /*! Returns the user data associated with the given id (can be null).
        No validity checkings in the index are done! */
    void* data ( int id ) const { return _table[id].data; }

    /*! Returns the valid index entry (>=0) relative to the given string key,
        or -1 if the string key does not exist in the table (or if st==0)*/
    int lookup_index ( const char *st ) const;

    /*! Returns the user data associated with the given string key,
        or null if the string was not found */
    void* lookup ( const char* st ) const;

    /*! Inserts a string key and user data to the table and returns true in case of success.
        False is returned in case the string key already exists (or if st==0) meaning
        that the entry was not added to the table.
        If the string already exists, its index can be retrieved with lastid().
        The given st pointer is copied in an internal allocated string.
        (note: if not set, the hash size is automatically initialized with 256) */
    bool insert ( const char *st, void* data ) { return _insert(st,data,1); }

    /*! Same as the insert() method, however the given st pointer is considered to
        be a pointer to a static string and is not internally allocated. */
    bool insertstat ( const char *st, void* data ) { return _insert(st,data,0); }

    /*! Returns the id involved during the last call to the insert methods:
        it will be 1) the index of the new added entry, or 2) the index of the found 
        duplicated entry, or 3) -1 is the string key was null. */
    int lastid () const { return _last_id; }

    /*! Removes and returns the data associated with key st. Returns 0 if 
        the key was not found. */
    void* remove ( const char *st );

   private :
    bool _insert ( const char *st, void* data, char dynamic );
    void _set_entry ( int id, const char *st, void* data, char dynamic );
    void _clear_entry ( int id );
 };

//================================ SrHasTable ===============================
 
/*! \class SrHasTable sr_hash_table.h
    Template version for usefull typecasts to a user type X.
    For generality, there is no destructor and the user is responsible to
    delete any attached pointers, using or not methods delete_data() and unref_data(). */
template <class X>
class SrHashTable : public SrHashTableBase
 { public:
    /*! This constructor simply calls the constructor of the base class */
    SBAPI SrHashTable ( int hsize ) : SrHashTableBase(hsize) {}
    
    /*! Simple type cast to the base class method */
    SBAPI X data ( int id ) const { return (X)SrHashTableBase::data(id); }

    /*! Simple type cast to the base class method */
    SBAPI X lookup ( const char* st ) const { return (X)SrHashTableBase::lookup(st); }

    /*! Will delete and set to zero all attached data in the hash table using
        the delete operator after a type cast to (X*). */
    SBAPI void delete_data ()
     { int i; 
       for (i=0; i<_table.size(); i++)
        { delete (X*)_table[i].data; _table[i].data=0; } // delete 0 is ok
     }

    /*! Will call unref() and set to zero all attached data in the hash table. It assumes
        that X derives SrSharedClass, and therefore X* is type-casted to SrSharedClass*. */
    SBAPI void unref_data ()
     { int i;
       for (i=0; i<_table.size(); i++)
        { if (_table[i].data) { ((SrSharedClass*)_table[i].data)->unref(); _table[i].data=0; } }
     }

    /*! Simple type cast to the base class method */
    SBAPI X remove ( const char *st ) { return (X)SrHashTableBase::remove(st); }
 };
 
//============================== end of file ===============================

# endif  // SR_HASH_TABLE_H
