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
#include "FolderProvider.h"
#include "DataObject.h"
#include <framework/Exception.h>

// Include library headers here

// Include system headers here

namespace dime {



PDataKey FolderProvider::getChild(PDataKey parent, std::string ID, DataProvider *& provider)
{
	Item * parentItem = parent.get() ? Key::spec(parent) : myRoot;
	
	ItemMap::iterator i = parentItem->myMap.find(ID);

	if (i == parentItem->myMap.end()) return PDataKey(NULL);

	if (i->second->myDest)
	{
		provider = i->second->myDest;
	}
	
	return Key::gen(i->second);
}

std::string FolderProvider::getSubpath(PDataKey key)
{
	return key.get() ? Key::spec(key)->mySubpath : "";
}

DataType FolderProvider::getType(PDataKey key)
{
	return static_cast<DataType>( POST_ADDITION | PRE_DELETION | 
		FOLDER |  HAS_CHILDS | ADOPTS_CHILDS | CAN_ADD_CHILDS | CAN_REMOVE);
}

std::string FolderProvider::getDescription(PDataKey key)
{
	Item * item = key.get() ? Key::spec(key) : myRoot;

	return item->myDescrip;
}

void FolderProvider::setDescription(PDataKey key, std::string description)
{
	Item * item = key.get() ? Key::spec(key) : myRoot;

	item->myDescrip = description;
}

bool FolderProvider::getBoolVal(PDataKey key)
{
	return false;
}

void FolderProvider::setBoolVal(PDataKey key, bool newValue)
{
	
}

int  FolderProvider::getIntVal(PDataKey key)
{
	return 0;
}

void FolderProvider::setIntVal(PDataKey key, int newValue)
{
	
}

float FolderProvider::getFloatVal(PDataKey key)
{
	return 0.0;
}

void  FolderProvider::setFloatVal(PDataKey key, float newValue)
{
	
}

std::string FolderProvider::getStringVal(PDataKey key)
{
	return "";
}

void FolderProvider::setStringVal(PDataKey key, const std::string & newValue)
{
	
}

void FolderProvider::addChild(PDataKey parent, std::string & suggestedID, 
				DataProvider * provider)
{
	Item * parentItem = parent.get() ? Key::spec(parent) : myRoot;

	//Not in general but useful for FolderProvider: 
	//If the suggestedID doesn't work, throw an exception

	ItemMap::iterator i = parentItem->myMap.find(suggestedID);

	if (i != parentItem->myMap.end()) 
	{
		std::string path = myPath;
		if (myPath != "/")
		{
			path += '/';
		}
		path += parentItem->mySubpath;

		THROW2("There's already a child %s of %s.", suggestedID.c_str(),
								makePath(myPath, parentItem->mySubpath).c_str());
	}

	std::string subpath = makeSubpath(parentItem->mySubpath, suggestedID);

	parentItem->myMap[suggestedID] = new Item(subpath, parentItem, provider);
	fireSignal(subpath, POST_ADDITION);
}

void FolderProvider::removeAdopted(PDataKey child)
{
	Item * item = Key::spec(child);	

	//HINT: No PRE_DELETION warning to be fired here, since
	//the adopted data provider is responsible.

	item->myParent->myMap.erase(item->mySubpath.substr(item->mySubpath.rfind('/')+1));	

	delete item;
}

void FolderProvider::remove(PDataKey child)
{
	if (child.get() == NULL)
	{
		//HINT: This can only be NULL for an outer call. So don't worry:
		//if the destructor calls this with the top element, he uses
		//the actual pointer value as key.
		delete this;
		return;
	}	
	
	Item * item = Key::spec(child);	


	//remove all below childs (depth first)
	ItemMap::iterator i;

	for (i = item->myMap.begin(); i != item->myMap.end();)
	{
		Item * subitem = i->second;
		PDataKey subchild = Key::gen(subitem);
		
		i++;

		if (subitem->myDest)
		{
			//don't remove the provider - it is removing itself!
			delete subitem->myDest;
		}
		else
		{		
			remove(subchild);
		}
	}

	//Before you start removing the actual node, the observers
	//should be warned that something is deleted.
	fireSignal(item->mySubpath, PRE_DELETION);

	if (item->myParent)
	{
		item->myParent->myMap.erase(item->mySubpath.substr(item->mySubpath.rfind('/')+1));
	}

	removeAllConnections(item->mySubpath);
	delete item;
}

void FolderProvider::getChilds(PDataKey parent, std::vector<std::string> & listOfChilds)
{
	Item * parentItem = parent.get() ? Key::spec(parent) : myRoot;

	listOfChilds.clear();

	ItemMap::iterator i;

	for (i = parentItem->myMap.begin(); i != parentItem->myMap.end(); i++)
	{
		listOfChilds.push_back(i->first);
	}
}

FolderProvider::FolderProvider()
{
	myRoot = new Item();
}

FolderProvider::~FolderProvider()
{
	remove(Key::gen(myRoot));
}

} //dime 
