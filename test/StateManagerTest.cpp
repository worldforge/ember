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

#include "StateManagerTest.h"
#include "framework/Exception.h"

namespace dime
{


void StateManagerTestCase::testFindStateNonExistant()
{
  bool bFailedToFindNonExistant = false;

  //try to find a nonexistant state
  try
    {
      mySM->findState("testdontmakestatecalledthis");
    }
  catch(Exception &)
    {
      bFailedToFindNonExistant = true;
    }

  CPPUNIT_ASSERT(bFailedToFindNonExistant == true);
}

void StateManagerTestCase::testFindStateLegal()
{
}

void StateManagerTestCase::setUp ()
{
  mySM = new StateManager("");

  CPPUNIT_ASSERT (mySM != NULL);
}

void StateManagerTestCase::tearDown()
{
  delete mySM;
}

}
#endif
