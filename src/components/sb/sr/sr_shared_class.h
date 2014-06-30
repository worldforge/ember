/*  sr_shared_class.h - part of Motion Engine and SmartBody-lib
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

# ifndef SR_SHARED_CLASS_H
# define SR_SHARED_CLASS_H

#include <sb/SBTypes.h>
#define USE_SHARE_PTR 1
/*! \file sr_shared_class.h 
    reference counter for smart-pointer like behavior.
    Note: attention is required to avoid circular references */
class SBAPI SrSharedClass
 { private :
    int _ref;

   protected :

    /*! Constructor initializes the reference counter as 0 */
    SrSharedClass () { _ref=0; };

    /*! Destructor in derived classes should always be declared as
        protected in order to oblige users to call always unref(),
        instead of delete */
    virtual ~SrSharedClass() {};

   public :
#if USE_SHARE_PTR
    /*! Returns the current reference counter value. */
    int getref () const { return _ref; }

    /*! Increments the reference counter. */
    void ref () { _ref++; }

    /*! Decrements the reference counter (if >0), and if the
        counter becomes 0, the class is automatically self deleted. */
    void unref() { if(_ref>0) _ref--; if(_ref==0) delete this; }
#else // dummy functions for all ref counting
	    /*! Returns the current reference counter value. */
    int getref () const { return _ref; }

    /*! Increments the reference counter. */
    void ref () {  }

    /*! Decrements the reference counter (if >0), and if the
        counter becomes 0, the class is automatically self deleted. */
    void unref() {  }

#endif
 };

//============================== end of file ===============================

# endif  // SR_SHARED_CLASS_H
