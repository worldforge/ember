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

#include "DataModelTest.h"

namespace dime
{

CPPUNIT_TEST_SUITE_REGISTRATION( DataModelTestCase );

void DataModelTestCase::setUp ()
{
	myDMS = DataModelService::getInstance();
	myDeleted = 0;
}

void DataModelTestCase::testRoot()
{
	PDataObject p = DataObject::getRoot();

	p->setDescription("The root of all evil");

	CPPUNIT_ASSERT(p->getDescription() == "The root of all evil");
	CPPUNIT_ASSERT(p->getType() & HAS_CHILDS);
	CPPUNIT_ASSERT(p->getType() & CAN_ADD_CHILDS);
	CPPUNIT_ASSERT(p->getPath() == "/");
	CPPUNIT_ASSERT(p->getID() == "");
}

void DataModelTestCase::testAdd()
{
	PDataObject p = DataObject::getRoot();

	//simply add a child
	PDataObject q = p->addChild("Child1");
	
	CPPUNIT_ASSERT(q.get() != NULL);
	CPPUNIT_ASSERT(q->getID() == "Child1");
	CPPUNIT_ASSERT(q->getPath() == "/Child1");
	CPPUNIT_ASSERT(p->getChild("Child1")->getPath() == q->getPath());
	CPPUNIT_ASSERT(q->getType() & HAS_CHILDS);
	CPPUNIT_ASSERT(q->getType() & CAN_ADD_CHILDS);
	
	//add a subchild also
	PDataObject r = q->addChild("Subchild1");
	
	CPPUNIT_ASSERT(r.get() != NULL);
	CPPUNIT_ASSERT(r->getID() == "Subchild1");
	CPPUNIT_ASSERT(r->getPath() == "/Child1/Subchild1");
	CPPUNIT_ASSERT(r->getPath() == 
				DataObject::getByPath("/Child1/Subchild1")->getPath());
	CPPUNIT_ASSERT(r->getPath() == q->getChild("Subchild1")->getPath());

	bool bFailedToAddSame = false;

	//try to add a child with the same name
	try
	{
		PDataObject c = p->addChild("Child1");
	}
	catch(Exception &)
	{
		bFailedToAddSame = true;
	}

	CPPUNIT_ASSERT(bFailedToAddSame == true);
}

void DataModelTestCase::testRemove()
{
	//Tests also event handling on the sample of PRE_DELETION
    
	PDataObject p = DataObject::getRoot();
	
	//add something, register as observer and remove it
    PDataObject q = p->addChild("Child2");
    q->addConnection(SigC::slot(*this, &DataModelTestCase::onDelete), 
                     PRE_DELETION);
    
	myDeleted = 0;

	q->remove();
	CPPUNIT_ASSERT(myDeleted == 1);
	CPPUNIT_ASSERT(q->getType() == VOID);
	
	//same again, but this time the connection is cut before 
	//we delete it
	q = p->addChild("Child2");
	DataConnection conn = q->addConnection(SigC::slot(*this, &DataModelTestCase::onDelete), 
			PRE_DELETION);
	conn.disconnect();
	
	myDeleted = 0;

	q->remove();
	CPPUNIT_ASSERT(myDeleted == 0);
	CPPUNIT_ASSERT(q->getType() == VOID);

	//Now add a two levels of childs and register as a child-observer
	//to the top one and remove in the middle
	q = p->addChild("Child2");
	PDataObject r = q->addChild("Subchild");
	p->addConnection(SigC::slot(*this, &DataModelTestCase::onDelete), 
		static_cast<DataType>(PRE_DELETION | FIRE_ON_CHILD_EVENT));
	
	myDeleted = 0;
	q->remove();
	CPPUNIT_ASSERT(myDeleted == 2);
	CPPUNIT_ASSERT(r->getType() == VOID);
}

void DataModelTestCase::testLink()
{
	//Link one child to another
	//and test on various things
	PDataObject p = DataObject::getRoot();
	PDataObject dest = p->addChild("LinkDest");
	PDataObject destChild  = dest->addChild("DestChild");
	PDataObject destChild2 = dest->addChild("DestChild2");

	dest->setDescription("Destination");

	PDataObject link = p->addChild("Link", "/LinkDest");
	
	CPPUNIT_ASSERT(dest.get() != NULL);
	CPPUNIT_ASSERT(link.get() != NULL);
	CPPUNIT_ASSERT(link->getLinkDest()->getPath() == dest->getPath());

	link->setDescription("Now a link");
	CPPUNIT_ASSERT(dest->getDescription() == "Now a link");
	CPPUNIT_ASSERT(link->getChild("DestChild")->getPath() 
						== "/Link/DestChild");	

	std::vector<PDataObject> childs;
	link->getChildList(childs);

	CPPUNIT_ASSERT(childs.size() == 2);
	
	CPPUNIT_ASSERT(childs[0]->getPath() == "/Link/DestChild"
				|| childs[1]->getPath() == "/Link/DestChild");
	
	CPPUNIT_ASSERT(childs[0]->getPath() == "/Link/DestChild2"
				|| childs[1]->getPath() == "/Link/DestChild2");

	myDeleted = 0;

	//Sign up to listen to the removal of the link's virtual child

	link->getChild("DestChild")->addConnection(
		SigC::slot(*this, &DataModelTestCase::onDelete), 
		static_cast<DataType>(PRE_DELETION));
	
	p->addConnection(SigC::slot(*this, &DataModelTestCase::onDelete), 
		static_cast<DataType>(PRE_DELETION | FIRE_ON_CHILD_EVENT));

	//Now remove the original and see what's happening to the link

	dest->remove();
	
	CPPUNIT_ASSERT(myDeleted == 1 /*virtual*/ + 3 /*real*/);
	CPPUNIT_ASSERT(link->getType() == static_cast<DataType>(VOID | IS_LINK));
	CPPUNIT_ASSERT(link->getDescription() != "Now a link");
}

void DataModelTestCase::tearDown()
{
	
}

void DataModelTestCase::onDelete(PDataObject p, DataType t)
{
	CPPUNIT_ASSERT(t == PRE_DELETION);
	myDeleted++;
}

}
#endif
