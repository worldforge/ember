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
#include "DataObject.h"
#include <framework/Exception.h>
#include "DataModelService.h"
#include "LinkProvider.h"

// Include library headers here

#include <boost/shared_ptr.hpp>

// Include system headers here
#include <string>
#include <list>
#include <stdio.h> 
#include <stdarg.h>

namespace dime {
	
/**
 * returns the real ID of the node. (In cases it is a link this is different from the
 * destination of the link.)
 */
std::string DataObject::getID()
{
	if (myPath == "/")
	{
		return "";
	}
	else
	{
		return myPath.substr(myPath.rfind('/')+1);	
	}
}

/**
 * returns the destination of the link.
 */
PDataObject DataObject::getLinkDest()
{
	if (getType() & IS_LINK)
	{
		return PDataObject(new DataObject(dynamic_cast<LinkProvider*>(myProvider)->getLinkDest()));
	}		

	THROW1("DataObject %s is not a link.", myPath.c_str());

	return PDataObject(NULL);
}

/**
 * returns the real path to the node from the root node.
 */
std::string DataObject::getPath()
{
	return myPath;
}

/**
 * returns the subpath within it's data model provider. Usually not needed by 
 * Data Model users.
 */
std::string DataObject::getSubpath()
{
	return mySubpath;
}

PDataKey DataObject::getKey()
{
	return myKey;
}

/**
 * returns the Data Object's data model provider. Usually not needed by 
 * Data Model users.
 */
DataProvider * DataObject::getProvider()
{
	return myProvider;
}

/**
 * returns the DataType of the Data Object consisting of different flags described above
 */
DataType DataObject::getType()
{
	if (myProvider)
	{
		return myProvider->getType(myKey);
	}
	else
	{
		// We've lost the connection to our provider (the object may be deleted).
		return VOID;
	}
}

/**
 * returns the description of a Data Object.
 */
std::string DataObject::getDescription()
{
	if (myProvider)
	{
		return myProvider->getDescription(myKey);
	}
	else
	{
		return "Lost connection.";
	}
}

/**
 * sets the description of a Data Object.
 */
void DataObject::setDescription(std::string description)
{
	if (myProvider)
	{
		myProvider->setDescription(myKey, description);
	}
}



/**
 * All of the following value getting and setting methods can also be applied
 * to Data Objects of other types. In this case the values are converted in some default way. Some
 * of the conversions don't work for the setting method.
 *
 * If the value of a Data Object is 0 or "" or it has no child nodes, it's boolean value is false,
 * otherwise true.
 *
 * Booleans are presented as -1 for TRUE and 0 for FALSE, if asked for integers. Floats
 * are rounded to get an integer and strings are parsed (some kind of atoi). Folders return the
 * number of child nodes as integer.
 *
 * Float convertions are performed in the same way as integer conversions.
 *
 * String conversion of boolean values are "TRUE" and "FALSE". Other string values are
 * some printf-like version of integers and floats and the ID of a folder's childs.
 */

bool DataObject::getBoolVal()
{
	DataType DataType = getType();

	if (DataType & BOOLEAN || DataType & CUSTOM_TYPE)
	{
		return myProvider->getBoolVal(myKey);
	}
	
	if (DataType & VOID)
	{
		return false;
	}
	
	if (DataType & FOLDER)
	{
		std::vector<std::string> listOfChilds;
		myProvider->getChilds(myKey, listOfChilds);
		return !listOfChilds.empty();
	}
	
	if (DataType & INTEGER)
	{
		return myProvider->getIntVal(myKey) != 0;
	}
	
	if (DataType & FLOAT)
	{
		return myProvider->getFloatVal(myKey) != 0.0;
	}
	
	//@todo: Should we test on != "false" rather?
	return !myProvider->getStringVal(myKey).empty();
}

int DataObject::getIntVal()
{
	DataType DataType = getType();

	if (DataType & INTEGER || DataType & CUSTOM_TYPE)
	{
		return myProvider->getIntVal(myKey);			
	}
	
	if (DataType & BOOLEAN )
	{
		return myProvider->getBoolVal(myKey) ? -1 : 0;
	}
	
	if (DataType & VOID)
	{
		return 0;
	}
	
	if (DataType & FOLDER)
	{
		std::vector<std::string> listOfChilds;
		myProvider->getChilds(myKey, listOfChilds);
		return listOfChilds.size();
	}

	if (DataType & FLOAT)
	{
		//@todo: Should we take the next integer here instead?
		return static_cast<int>(myProvider->getFloatVal(myKey));
	}

	return myProvider->getStringVal(myKey).size();		
}

float DataObject::getFloatVal()
{
	DataType DataType = getType();

	if (DataType & FLOAT || DataType & CUSTOM_TYPE)
	{
		return myProvider->getFloatVal(myKey);			
	}
	
	if (DataType & BOOLEAN )
	{
		return myProvider->getBoolVal(myKey) ? -1.0 : 0.0;
	}
	
	if (DataType & VOID)
	{
		return 0.0;
	}
	
	if (DataType & FOLDER)
	{
		std::vector<std::string> listOfChilds;
		myProvider->getChilds(myKey, listOfChilds);
		return (float)listOfChilds.size();
	}

	if (DataType & INTEGER)
	{
		return (float)myProvider->getIntVal(myKey); 
	}

	return (float)myProvider->getStringVal(myKey).size();	
}

std::string DataObject::getStringVal()
{
	DataType DataType = getType();
	
	if (DataType & STRING || DataType & CUSTOM_TYPE)
	{
		return myProvider->getStringVal(myKey);			
	}

	if (DataType & FLOAT)
	{
		char buffer[CONVERT_BUFFER_SIZE];
		sprintf(buffer, "%2.10f", myProvider->getFloatVal(myKey));
		return std::string(buffer);
	}
	
	if (DataType & BOOLEAN )
	{
		return std::string(myProvider->getBoolVal(myKey) ? "true" : "false");
	}
	
	if (DataType & VOID)
	{
		return "";
	}
	
	if (DataType & FOLDER)
	{
		//@todo: Should we rather return "" here?
		std::vector<std::string> listOfChilds;
		myProvider->getChilds(myKey, listOfChilds);
		
		std::string buffer = "{ ";

		for (unsigned int i = 0; i < listOfChilds.size(); i++)
		{
			
			buffer += listOfChilds[i];
			if (i < listOfChilds.size()-1)
			{
				buffer += ", ";
			}
		}

		buffer += " }";
		
		return buffer;
	}

	{
		char buffer[CONVERT_BUFFER_SIZE];
		sprintf(buffer, "%d", myProvider->getIntVal(myKey));
		return std::string(buffer);			
	}
}

void DataObject::setBoolVal(bool newValue)
{
	DataType DataType = getType();

	if (!(DataType & CAN_SET_VALUE))
	{
		THROW1("Cannot set value of Data Object %s.", myPath.c_str());
	}

	if (DataType & BOOLEAN || DataType & CUSTOM_TYPE)
	{
		myProvider->setBoolVal(myKey, newValue);
		return;
	}

	if (DataType & VOID || DataType & FOLDER)
	{
		return;
	}

	if (DataType & FLOAT)
	{
		myProvider->setFloatVal(myKey, newValue ? 1.0 : 0.0);
		return;
	}

	if (DataType & INTEGER)
	{
		myProvider->setIntVal(myKey, newValue ? 1 : 0);
		return;
	}

	myProvider->setStringVal(myKey, newValue ? "true" : "false");
}

void DataObject::setIntVal(int newValue)
{
	DataType DataType = getType();

	if (!(DataType & CAN_SET_VALUE))
	{
		THROW1("Cannot set value of Data Object %s.", myPath.c_str());
	}

	if (DataType & INTEGER || DataType & CUSTOM_TYPE)
	{
		myProvider->setIntVal(myKey, newValue);
		return;
	}

	if (DataType & VOID || DataType & FOLDER)
	{
		return;
	}

	if (DataType & FLOAT)
	{
		myProvider->setFloatVal(myKey, static_cast<float>(newValue));
		return;
	}

	if (DataType & BOOLEAN)
	{
		myProvider->setBoolVal(myKey, newValue != 0);
		return;
	}

	{
		char buffer[CONVERT_BUFFER_SIZE];
		sprintf(buffer, "%d", newValue);
		myProvider->setStringVal(myKey, std::string(buffer));
	}
}

void DataObject::setFloatVal(float newValue)
{
	DataType DataType = getType();

	if (!(DataType & CAN_SET_VALUE))
	{
		THROW1("Cannot set value of Data Object %s.", myPath.c_str());
	}

	if (DataType & FLOAT || DataType & CUSTOM_TYPE)
	{
		myProvider->setFloatVal(myKey, newValue);
		return;
	}

	if (DataType & VOID || DataType & FOLDER)
	{
		return;
	}

	if (DataType & INTEGER)
	{
		//@todo: Should we take the next integer here instead?
		myProvider->setIntVal(myKey, static_cast<int>(newValue));
		return;
	}

	if (DataType & BOOLEAN)
	{
		myProvider->setBoolVal(myKey, newValue != 0.0);
		return;
	}

	{
		char buffer[CONVERT_BUFFER_SIZE];
		sprintf(buffer, "%2.10f", newValue);
		myProvider->setStringVal(myKey, std::string(buffer));
	}
}

void DataObject::setStringVal(std::string newValue)
{
	DataType DataType = getType();

	if (!(DataType & CAN_SET_VALUE))
	{
		THROW1("Cannot set value of Data Object %s.", myPath.c_str());
	}

	if (DataType & STRING || DataType & CUSTOM_TYPE)
	{
		myProvider->setStringVal(myKey, newValue);
		return;
	}

	if (DataType & VOID || DataType & FOLDER)
	{
		return;
	}

	if (DataType & INTEGER)
	{
		int dest = 0;
		sscanf(newValue.c_str(),"%d", &dest);
		myProvider->setIntVal(myKey, dest);
		return;
	}

	if (DataType & BOOLEAN)
	{
		//@todo: Should we test on != "false" rather?
		myProvider->setBoolVal(myKey, !newValue.empty());
		return;
	}

	{
		float dest = 0.0;
		sscanf(newValue.c_str(), "%f", &dest);
		myProvider->setFloatVal(myKey, dest);
	}
}

/**
 * returns a direct/indirect child specified by the subpath.
 */
PDataObject DataObject::getChild(std::string subpath)
{
	return PDataObject(new DataObject(myProvider, myKey, subpath, 
		DataProvider::makePath(myPath, subpath)));
}

/**
 * Fills a list with the subpath of all direct childs.
 */
void DataObject::getChildList(ChildList & childList)
{
	childList.clear();

	if (!(getType() & HAS_CHILDS))
	{
		THROW1("DataObject %s cannot have childs.", myPath.c_str());
	}


	std::vector<std::string> IDList;
	myProvider->getChilds(myKey, IDList);			
	
	for (unsigned int i = 0; i < IDList.size(); i++)
	{
		childList.push_back(getChild(IDList[i]));
	}
}

/**
 * returns a new Data Object added as child node. You can specify the new data object
 * to be another provider's top node.
 *
 * If no suggestedID (= "") is given, an unused ID is generated automatically. 
 * ATTENTION: Don't rely on the child to have the suggested ID. (That's why it's called
 * suggested!)
 */
PDataObject DataObject::addChild(std::string suggestedID, DataProvider * provider)
{
	DataType DataType = getType();

	if (!(DataType & CAN_ADD_CHILDS))
	{
		THROW1("You cannot add childs to Data Object %s.", myPath.c_str());
	}

	if (provider && !(DataType & ADOPTS_CHILDS))
	{
		THROW1("Data Object %s doesn't adopt childs.", myPath.c_str());			
	}


	if (suggestedID.empty())
	{		
		static int uniqueCounter = 0; //2^32 should be enough of uniqueness ;-)
		uniqueCounter++;
		
		char buffer[CONVERT_BUFFER_SIZE];
		sprintf(buffer, "~@#x%X", uniqueCounter); //some prefixing to not collide with non-generated names
		
		suggestedID = buffer;			
	}		

	myProvider->addChild(myKey, suggestedID, provider);	

	if (provider)
	{
		return PDataObject(new DataObject(provider, PDataKey(NULL), "", 
			DataProvider::makePath(myPath, suggestedID)));
	}
	else
	{
		return PDataObject(new DataObject(myProvider, myKey, suggestedID, 
				DataProvider::makePath(myPath, suggestedID)));
	}
}


/**
 * returns a new Data Object added as child node. If linkTo is specified (non ""), 
 * a link is added as new child Data Object. 
 *
 * If no suggestedID (= "") is given, an unused ID is generated automatically. 
 * ATTENTION: Don't rely on the child to have the suggested ID. (That's why it's called
 * suggested!)
 */
PDataObject DataObject::addChild(std::string suggestedID, std::string linkTo)
{
	if (!linkTo.empty())
	{
        LinkProvider * provider = new LinkProvider(linkTo);

		try
		{
			return addChild(suggestedID, provider);
		}
		catch (Exception *)
		{
			delete provider;
			throw;
		}

	}
	else
	{
		return addChild(suggestedID, NULL);
	}
}

/**
 * removes a Data Object. If the object is a link, only the link will be removed.
 *
 * All links pointing to this child Data Object will be set to VOID-DataType. The same
 * happens on the DataObject itself and all other DataObjects using it.
 */
void DataObject::remove()
{
	DataType DataType = getType();
	if (!(DataType & CAN_REMOVE))
	{
		THROW1("Cannot remove Data Object %s.", myPath.c_str());
	}
	
	myProvider->remove(myKey);
}

/**
 * Adds a new connection to the signal. The returned DataConnection can be saved to
 * remove the connection later. To disconnect just hold the DataConnection and call
 * disconnect to it.
 *
 * types are all DataType of events the connection should be used for,
 * or'd together. This can also include the FIRE_ON_CHILD_EVENT. If so, all event of specified
 * DataType concerning direct OR indirect child nodes of the node are forwarded to this
 * connection, too.
 */
DataConnection DataObject::addConnection(const DataSlot & slot, DataType types)
{
	if (myProvider)
	{
		return myProvider->addConnection(mySubpath, slot, types);
	}
	else
	{
		// @todo: Should it throw an exception, that there's nothing to connect instead?
		return DataConnection();
	}	
}

/**
 * Returns a new data object for the specified path. 
 */
PDataObject DataObject::getByPath(std::string path)
{
	return PDataObject(new DataObject(path));	
}

/**
 * Returns a new root data object for the specified path.
 */
PDataObject DataObject::getRoot(std::string path)
{
	return PDataObject(new DataObject(path));
}

//----------------------------------------------------------------------
// Constructors

void DataObject::findObject(std::string & subpath,
						   DataProvider *& provider,
						   PDataKey & key)
{
	if (provider == NULL)
	{
		//an absolute path is given
		provider = 
static_cast<DataProvider*>(DataModelService::getInstance()->getRootProvider());		

		if (subpath[0] != '/')
		{
			THROW1("Invalid path - missing trailing '/': %s'.", subpath.c_str());
		}
		
		subpath = subpath.substr(1);
	}

	if (!subpath.empty() && subpath[0] == '/')
	{
		THROW1("Invalid subpath - needs no trailing '/': %s'.", subpath.c_str());
	}

	unsigned int i;
	std::string	ID;

	std::string path = DataProvider::makePath(provider->myPath, subpath);

	while (!subpath.empty())
	{
		DataProvider * newProvider = NULL;

		i	= subpath.find('/');			
		ID	= subpath.substr(0, i);
		key = provider->getChild(key, ID, newProvider);

		if (key.get() == NULL) 
		{
			THROW1("Cannot find Data Object %s in the Data Model.", 
				path.c_str());
		}

		if (newProvider != NULL)
		{
			// Have to make the connection to the upper provider
			if (!newProvider->myParent)
			{
				newProvider->myParent	 = provider;
				newProvider->myKey		 = key;
				newProvider->mySubpath   = provider->getSubpath(key);
				newProvider->myPath		 = DataProvider::makePath(
									provider->myPath, newProvider->mySubpath);
			}

			provider = newProvider;
			key.reset(NULL);
		}
		
		if (i == std::basic_string<char>::npos)
		{
			subpath = "";
		}
		else
		{
			subpath = subpath.substr(i+1);
		}
	}

	subpath = provider->getSubpath(key);
}

DataObject::DataObject(DataProvider * provider, 
					   PDataKey key,
					   std::string subpath,
					   std::string path)
{
	myPath = path;
	myProvider = provider;
	myKey	   = key;
	mySubpath  = subpath;

	findObject(mySubpath, myProvider, myKey);

	// The DataObject should listen to removal operations, so 
	// it can be switched to VOID-state when necessary
	
	//@todo: What happens to Data Objects not supporting PRE_DELETION?
	
	myConnection = myProvider->addConnection(mySubpath, 
		SigC::slot(*this, &DataObject::onObjectDeletion), PRE_DELETION);
}

DataObject::DataObject(std::string path)
{
	myPath	   = path;
	myProvider = NULL;
	mySubpath  = path;

	findObject(mySubpath, myProvider, myKey);

	// The DataObject should listen to removal operations, so 
	// it can be switched to VOID-state when necessary
	
	//@todo: What happens to Data Objects not supporting PRE_DELETION?
	
	myConnection = myProvider->addConnection(mySubpath, 
		SigC::slot(*this, &DataObject::onObjectDeletion), PRE_DELETION);
}

DataObject::~DataObject()
{
	myConnection.disconnect();
}

void DataObject::onObjectDeletion(PDataObject object, DataType event)
{
	myProvider = NULL;
	myKey.reset();
}

} //dime
