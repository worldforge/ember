/*
 *  sr_Class_manager.h - part of Motion Engine and SmartBody-lib
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

# ifndef SR_CLASS_MANAGER_H
# define SR_CLASS_MANAGER_H

/** \file sr_class_manager.h 
 * Generic way to allocate, io and compare classes */
#include <sb/SBTypes.h>
# include <sr/sr_input.h>
# include <sr/sr_output.h>
# include <sr/sr_shared_class.h>

class SrClassManagerBase : public SrSharedClass
 { protected :
    virtual ~SrClassManagerBase() {};
   public : // callbacks
    SBAPI virtual void* alloc ()=0;
    SBAPI virtual void* alloc ( const void* obj )=0;
    SBAPI virtual void sr_free ( void* obj )=0;
    SBAPI virtual void output ( SrOutput& o, const void* obj ) { }
    SBAPI virtual void input ( SrInput& i, void* obj ) { }
    SBAPI virtual int compare ( const void* obj1, const void* obj2 ) { return 0; }
 };

/*! Example of an implementation of a class to be automatically managed
   with SrClassManager<MyData> :
class MyData
 { public :
    MyData ();
    MyData ( const MyData& d );
    virtual ~MyData ();
    friend SrOutput& operator<< ( SrOutput& out, const MyData& d );
    friend SrInput& operator>> ( SrInput& inp, MyData& d );
    friend int sr_compare ( const MyData* d1, const MyData* d2 );
 };*/
 
template <class X>
class SrClassManager : public SrClassManagerBase
 { 
 protected :
    virtual ~SrClassManager<X> () {}

 public :
    SBAPI virtual void* alloc () { return (void*) new X; }

    SBAPI virtual void* alloc ( const void* obj ) { return (void*) new X(*((X*)obj)); }

    SBAPI virtual void sr_free ( void* obj ) { delete (X*) obj; }

    SBAPI virtual void output ( SrOutput& o, const void* obj ) { o<<*((const X*)obj); }

    SBAPI virtual void input ( SrInput& i, void* obj ) { i>>*((X*)obj); }

    SBAPI virtual int compare ( const void* obj1, const void* obj2 )
     { return sr_compare ( (const X*)obj1, (const X*)obj2 ); }
 };

//============================== end of file ===============================

# endif  // SR_CLASS_MANAGER_H
