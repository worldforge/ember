/*
    Copyright (C) 2002 Tim Enderling

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
#include <services/datamodel/DataModelService.h>
#include <services/datamodel/DataObject.h>
#include <framework/Exception.h>

namespace dime
{

class DataModelTestCase : public CppUnit::TestFixture, public SigC::Object 
{
  CPPUNIT_TEST_SUITE( DataModelTestCase );
  CPPUNIT_TEST( testRoot );
  CPPUNIT_TEST( testAdd  );
  CPPUNIT_TEST( testRemove );
  CPPUNIT_TEST( testLink );
  CPPUNIT_TEST_SUITE_END();

protected:
	DataModelService * myDMS;
	int				   myDeleted;

public:
	void setUp ();
	void tearDown();
	void testRoot();
	void testAdd();
	void testRemove();
	void testLink();
	void onDelete(PDataObject p, DataType t);
	
protected:
};



}

#endif
