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

// Include system headers here

namespace dime {

/**
 * Most of the LinkProviders actions are:
 * - Replace the NULL-DataKey by the destination Key
 * - Pass through to the other provider
 *
 * Also it translates subpaths and forwards all signals.
 */

/**
 * Really short for 'real key'
 **/
#define RK(key) ((key.get() != NULL) ? (key) : (myDestKey))

PDataKey LinkProvider::getChild(PDataKey parent, std::string ID, DataProvider *& provider)
{
	if (myProvider)
	{
		return myProvider->getChild(RK(parent), ID, provider);	
	}
	else
	{
		return NULL;
	}
}

std::string LinkProvider::getSubpath(PDataKey key)
{
	if (myProvider)
	{
		if (key.get() == NULL)
		{
			return "";
		}
		else
		{
			//remove the hidden part of the subpath 
			return myProvider->getSubpath(RK(key)).substr(myDestSubpath.size()+1);
		}
	}
	else
	{
		// @todo: Should an exception be thrown instead?
		return "";
	}

}

DataType LinkProvider::getType(PDataKey key)
{
	if (myProvider)
	{
		return static_cast<DataType>(myProvider->getType(RK(key)) | IS_LINK);
	}
	else
	{
		return static_cast<DataType>(VOID | IS_LINK);
	}
}

std::string LinkProvider::getDescription(PDataKey key)
{
	if (myProvider)
	{
		return myProvider->getDescription(RK(key));
	}
	else
	{
		return "Link to nothing.";
	}
}

void LinkProvider::setDescription(PDataKey key, std::string description)
{
	if (myProvider)
	{
		myProvider->setDescription(RK(key), description);
	}
}

bool LinkProvider::getBoolVal(PDataKey key)
{
	return myProvider->getBoolVal(RK(key));
}

void LinkProvider::setBoolVal(PDataKey key, bool newValue)
{
	myProvider->setBoolVal(RK(key), newValue);
}

int  LinkProvider::getIntVal(PDataKey key)
{
	return myProvider->getIntVal(RK(key));
}

void LinkProvider::setIntVal(PDataKey key, int newValue)
{
	myProvider->setIntVal(RK(key), newValue);
}

float LinkProvider::getFloatVal(PDataKey key)
{
	return myProvider->getFloatVal(RK(key));
}

void  LinkProvider::setFloatVal(PDataKey key, float newValue)
{
	myProvider->setFloatVal(RK(key), newValue);
}

std::string LinkProvider::getStringVal(PDataKey key)
{
	return myProvider->getStringVal(RK(key));
}

void LinkProvider::setStringVal(PDataKey key, const std::string & newValue)
{
	myProvider->setStringVal(RK(key), newValue);
}

void LinkProvider::addChild(PDataKey parent, std::string & suggestedID, 
				DataProvider * provider)
{
	myProvider->addChild(RK(parent), suggestedID, provider);
}

void LinkProvider::remove(PDataKey child)
{
	if (child.get() == NULL)
	{
		delete this;
	}
	else
	{
		myProvider->remove(child);
	}
}

void LinkProvider::removeAdopted(PDataKey adopted)
{
	//is not adopting
}

void LinkProvider::getChilds(PDataKey parent, std::vector<std::string> & listOfChilds)
{
	myProvider->getChilds(RK(parent), listOfChilds);
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
			myProvider = NULL;
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
	myProvider = NULL;
	myDestPath = destPath;
	myDestSubpath = destPath;

	DataObject::findObject(myDestSubpath, myProvider, myDestKey);
	
	myProvider->addConnection(myDestSubpath, SigC::slot(*this, &LinkProvider::onSignal), 
	 static_cast<DataType>(PRE_VALUE_CHANGE | POST_VALUE_CHANGE |  POST_ADDITION  | PRE_DELETION |
	  PRE_LINK_CHANGE | POST_LINK_CHANGE | FIRE_ON_CHILD_EVENT));
	
}

LinkProvider::~LinkProvider()
{
	myConnection.disconnect();

	DataConnectionMap::iterator i; 

	// Fire a removal event to all observers listening to removal events		
	fireGeneralSignal(PRE_DELETION);
}

} //dime 