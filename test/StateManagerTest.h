/*
    Copyright (C) 2002 Martin Pollard aka Xmp

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef USE_CPP_UNIT

#include <cppunit/TestCase.h>
#include <cppunit/extensions/HelperMacros.h>
#include "main/StateManager.h"

#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#include <sigc++/signal_system.h>
#else
#include <sigc++/object.h>
#endif

namespace dime
{

/* 
 * A test case that is designed to produce
 * example errors and failures
 *
 */

class StateManagerTestCase : public CppUnit::TestCase, public SigC::Object 
{

  CPPUNIT_TEST_SUITE( StateManagerTestCase );
  CPPUNIT_TEST( testFindStateNonExistant );
  CPPUNIT_TEST( testFindStateLegal );
  CPPUNIT_TEST_SUITE_END();
protected:
	StateManager * mySM;

public:
	void			setUp ();
	void			tearDown();
	
protected:
	void			testFindStateNonExistant();
	void			testFindStateLegal();


	
};

}
#endif
