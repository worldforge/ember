/*
 *  sk_joint_name.cpp - part of Motion Engine and SmartBody-lib
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
 */
#if 0
# include <sk/sk_joint_name.h>

//============================= SkJointName ============================

SrHashTableBase SkJointName::_htable;

void SkJointName::operator= ( const char* st )
 {
   // check st:
   if ( !st ) { _id=-1; return; }

   // make sure the hash table was initialized:
   _check();
   
   // insert index:
   _htable.insert ( st, this ); // 'this' pointer does not need to be stored
    
   // get new or duplicated entry id:
   _id = _htable.lastid();
   
   //sr_out<<"SkJointName Hash Table Longest Entry: "<<_htable.longest_entry()<<srnl;
   //sr_out<<"SkJointName Hash Table Size: "<<_htable.size()<<srnl;
 }

bool SkJointName::operator== ( const char* st )
 {
   int id = _htable.lookup_index(st);
   if ( id<0 ) return false;
   return id==_id? true:false;
 }

bool SkJointName::exist ( const char* name ) // static
 {
   return _htable.lookup_index(name)<0? false:true;
 }

//============================ End of File ============================
#endif