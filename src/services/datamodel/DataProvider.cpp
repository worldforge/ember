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
#include "DataProvider.h"
#include "DataObject.h"
#include "DataModelService.h"

// Include library headers here

#include <string>
#include <list>
#include <stdio.h> 
#include <stdarg.h>
#include <ctime>

// Include system headers here

namespace dime {

	
DataConnection::DataConnection()
{
	myConnected = false;
}

DataConnection::DataConnection(std::string path,
			   DataConnectionList::iterator & i)
{
	myPath = path;
	myI = i;
	myConnected = true;
}

DataConnection & DataConnection::operator=(const DataConnection & source)
{
	myPath		= source.myPath;
	myI			= source.myI;
	myConnected = source.myConnected;
	return *this;
}

void DataConnection::disconnect()
{
	if (!myConnected) return;
	
	//If it isn't there the connection is probably removed already
	try 
	{
		std::string path = myPath;
		DataProvider * provider = NULL;
		PDataKey key;
		
		DataObject::findObject(path, provider, key);

		provider->removeConnection(path, myI);						
	}
	catch(...)
	{
				
	}

	myConnected = false;
}

DataProvider::DataProvider()
{
	myParent	= NULL;
	myKey		= NULL;
}


//----------------------------------------------------------------------
// Destructor

DataProvider::~DataProvider()
{
	// free the connections
	DataConnectionMap::iterator i;

	for (i = myConns.begin(); i != myConns.end(); i++)
	{
		DataConnectionList * list = i->second;
		DataConnectionList::iterator j;

		for (j = list->begin(); j != list->end(); j++)
		{
			delete *j;
		}

		delete list;
	}

	// remove me from my parent 
	if (myParent)
	{
		myParent->removeAdopted(myKey);
	}
}

//----------------------------------------------------------------------
// Other public methods
DataConnection DataProvider::addConnection(std::string subpath, DataSlot & slot, 
					DataType event)
{
	DataConnectionList * list = myConns[subpath];

	if (!list)
	{
		list = new DataConnectionList;
		myConns[subpath] = list;
	}

	InternDataConnection * conn = new InternDataConnection;
	
	conn->mySignal.connect(slot);
	conn->myTypes  = event;

	DataConnectionList::iterator i;

	if (event & FIRE_ON_CHILD_EVENT)
	{
		// Child-forward events always go in front
		list->push_front(conn);
		i = list->begin();
	}
	else
	{
		// Non-child-forward events always go back
		list->push_back(conn);
		i = list->end();
		i--;
	}

	return DataConnection(makePath(myPath, subpath), i);
}

void DataProvider::removeConnection(std::string subpath, 
										DataConnectionList::iterator & i)
{
	DataConnectionList * list = myConns[subpath];

	// Only if the list's not already removed.
	if (list)
	{
		// ATTENTION: Cannot test if it is really here, but hope so.
		delete *i;
		list->erase(i);
	}
}

void DataProvider::removeAllConnections(std::string subpath)
{
	DataConnectionList::iterator i;

	DataConnectionList * list = NULL;
	DataConnectionMap::iterator a = myConns.find(subpath);

	if (a != myConns.end())
	{			
		list = a->second;
		for (i = list->begin(); i != list->end(); i++)
		{
			delete *i;
		}

		delete list;
		myConns.erase(a);
	}
	
}

void DataProvider::fireSignal(std::string subpath, DataType event)
{
	PDataObject subject(new DataObject(this, PDataKey(NULL), subpath, 
		makePath(myPath, subpath)));

	// Event for this data object first.
	DataConnectionList::iterator i;

	DataConnectionList * list = NULL;
	DataConnectionMap::iterator m = myConns.find(subpath);

	if (m != myConns.end())
	{			
		list = m->second;
		for (i = list->begin(); i != list->end(); i++)
		{
			if (event & (*i)->myTypes)
			{
				(*i)->mySignal(subject, event);
			}
		}
	}
	
	// Events for all parent data objects.
	DataProvider * provider = this;

	int a;
	if (subpath.empty())
	{		
		provider = provider->myParent;

		if (provider)
		{
			subpath = provider->mySubpath;
			a = subpath.rfind('/');
			subpath = subpath.substr(0, a);
		}			
	}
	else
	{
		a = subpath.rfind('/');
		if (a != std::string::npos)
		{
			subpath = subpath.substr(0, a);
		}
		else
		{
			subpath = "";			
		}
	}

	while (provider)
	{
		m = myConns.find(subpath);
		
		if (m != myConns.end())
		{
			list = m->second;
			for (i = list->begin(); i != list->end(); i++)
			{
				if (! ((*i)->myTypes & FIRE_ON_CHILD_EVENT))
				{
					// Since all fire-on-child-handler are added in front, one
					// can stop finding the first non-child handler.
					break;
				}	
				
				if (event & (*i)->myTypes)
				{
					(*i)->mySignal(subject, event);
				}
			}	
		}

		if (subpath.empty())
		{		
			provider = provider->myParent;

			if (provider)
			{
				subpath = provider->mySubpath;
				a = subpath.rfind('/');
				subpath = subpath.substr(0, a);
			}			
		}
		else
		{
			a = subpath.rfind('/');
			if (a != std::string::npos)
			{
				subpath = subpath.substr(0, a);
			}
			else
			{
				subpath = "";			
			}
		}
	}
}

void DataProvider::fireGeneralSignal(DataType event)
{
	DataConnectionMap::iterator i;

	for (i = myConns.begin(); i != myConns.end(); i++)
	{
		DataConnectionList * list = i->second;
		DataConnectionList::iterator j;
		fireSignal(i->first, event);
	}
}

std::string DataProvider::makeSubpath(std::string parentSubpath, std::string ID)
{
	std::string subpath = parentSubpath;

	if (!parentSubpath.empty())
	{
		subpath += "/";
	}

	subpath += ID;

	return subpath;
}

std::string DataProvider::makePath(std::string parentPath, std::string ID)
{
	std::string path = parentPath;
	if (parentPath != "/")
	{
		path += '/';
	}
	path += ID;

	return path;
}

} // namespace dime