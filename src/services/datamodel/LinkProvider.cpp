/*
    Copyright (C) 2002  Tim Enderling

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

// Include other headers of the current program here
#include "LinkProvider.h"
#include "DataObject.h"

// Include library headers here
#if SIGC_MAJOR_VERSION == 1 && SIGC_MINOR_VERSION == 0
#else
#include <sigc++/object_slot.h>
#endif

// Include system headers here

namespace dime {

/**
 * Most of the LinkProviders actions are:
 * - Replace the NULL-DataKey by the Root destination Key
 * - Pass through to the other provider
 *
 * Also it translates subpaths and forwards all signals.
 */

/**
 * Really short for 'real key'
 **/
#define RK(key) ((key.get() == NULL) ? &myRoot : (&Key::spec(key)))

/**
 * Really short for 'real key variable'
 */

#define RKV(key) Item * item = RK(key) /*;*/

PDataKey LinkProvider::getChild(PDataKey parent, std::string ID, DataProvider *& provider)
{
	RKV(parent);

	if (myRoot.myDest)
	{
		Item newItem;
		newItem.myDestKey = 
		item->myDest->getChild(item->myDestKey, ID, newItem.myDest);	
		newItem.mySubpath = DataProvider::makeSubpath(item->mySubpath, ID);
		return Key::gen(newItem);
	}
	else
	{
		return PDataKey();
	}
}

std::string LinkProvider::getSubpath(PDataKey key)
{
	if (myRoot.myDest)
	{
		return RK(key)->mySubpath;
	}
	else
	{
		// @todo: Should an exception be thrown instead?
		return "";
	}
}

DataType LinkProvider::getType(PDataKey key)
{
	if (myRoot.myDest)
	{
		RKV(key);
		return static_cast<DataType>(item->myDest->getType(item->myDestKey) | IS_LINK);
	}
	else
	{
		return static_cast<DataType>(VOID | IS_LINK);
	}
}

std::string LinkProvider::getDescription(PDataKey key)
{
	if (myRoot.myDest)
	{
		RKV(key);
		return item->myDest->getDescription(item->myDestKey);
	}
	else
	{
		return "Link to nothing.";
	}
}

void LinkProvider::setDescription(PDataKey key, std::string description)
{
	if (myRoot.myDest)
	{
		RKV(key);
		item->myDest->setDescription(item->myDestKey, description);
	}
}

bool LinkProvider::getBoolVal(PDataKey key)
{
	RKV(key);
	return item->myDest->getBoolVal(item->myDestKey);
}

void LinkProvider::setBoolVal(PDataKey key, bool newValue)
{
	RKV(key);
	item->myDest->setBoolVal(item->myDestKey, newValue);
}

int  LinkProvider::getIntVal(PDataKey key)
{
	RKV(key);
	return item->myDest->getIntVal(item->myDestKey);
}

void LinkProvider::setIntVal(PDataKey key, int newValue)
{
	RKV(key);
	item->myDest->setIntVal(item->myDestKey, newValue);
}

float LinkProvider::getFloatVal(PDataKey key)
{
	RKV(key);
	return item->myDest->getFloatVal(item->myDestKey);
}

void  LinkProvider::setFloatVal(PDataKey key, float newValue)
{
	RKV(key);
	item->myDest->setFloatVal(item->myDestKey, newValue);
}

std::string LinkProvider::getStringVal(PDataKey key)
{
	RKV(key);
	return item->myDest->getStringVal(item->myDestKey);
}

void LinkProvider::setStringVal(PDataKey key, const std::string & newValue)
{
	RKV(key);
	item->myDest->setStringVal(item->myDestKey, newValue);
}

void LinkProvider::addChild(PDataKey parent, std::string & suggestedID, 
				DataProvider * provider)
{
	RKV(parent);
	item->myDest->addChild(item->myDestKey, suggestedID, provider);
}

void LinkProvider::remove(PDataKey child)
{
	if (child.get() == NULL)
	{
		delete this;
	}
	else
	{
		RKV(child);
		item->myDest->remove(item->myDestKey);
	}
}

void LinkProvider::removeAdopted(PDataKey adopted)
{
	//is not adopting
}

void LinkProvider::getChilds(PDataKey parent, std::vector<std::string> & listOfChilds)
{
	RKV(parent);
	item->myDest->getChilds(item->myDestKey, listOfChilds);
}

void LinkProvider::onSignal(PDataObject object, DataType event)
{
	std::string subpath = object->getPath();

	if (subpath.size() <= myDestPath.size())
	{
		subpath = "";
	}
	else
	{
		subpath = subpath.substr(myDestPath.size()+1);
	}

	if (subpath.empty() && (event & PRE_DELETION))
	{
		fireSignal("", PRE_LINK_CHANGE);
	}
	else
	{
		fireSignal(subpath, event);
	}

	if (event & PRE_DELETION)
	{
		if (subpath.empty())
		{
			//lost connection to the provider
			myRoot.myDest = NULL;
			fireSignal("", POST_LINK_CHANGE);
		}
		else
		{
			removeAllConnections(subpath);
		}
	}
}

std::string LinkProvider::getLinkDest()
{
	return myDestPath;
}

LinkProvider::LinkProvider(std::string destPath)
{
	myRoot.myDest = NULL;
	myDestPath = destPath;
	std::string destSubpath = destPath;

	DataObject::findObject(destSubpath, myRoot.myDest, myRoot.myDestKey);
	
	myRoot.myDest->addConnection(destSubpath, SigC::slot(*this, &LinkProvider::onSignal), 
	 static_cast<DataType>(PRE_VALUE_CHANGE | POST_VALUE_CHANGE |  POST_ADDITION  | PRE_DELETION |
	  PRE_LINK_CHANGE | POST_LINK_CHANGE | FIRE_ON_CHILD_EVENT));
	
}

LinkProvider::~LinkProvider()
{
	myConnection.disconnect();

	// Fire a removal event to all observers listening to removal events		
	fireGeneralSignal(PRE_DELETION);
}
} //dime
