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
#include "StringProvider.h"

// Include library headers here

// Include system headers here

namespace dime {

std::string StringProvider::getValue()
{
	return myValue;
}

void StringProvider::setPermissions(DataType permissions)
{
	myPermissions = permissions;
}

void StringProvider::setValue(std::string newValue)
{
	fireSignal("", PRE_VALUE_CHANGE);

	myValue = newValue;
	
	fireSignal("", POST_VALUE_CHANGE);
}

PDataKey StringProvider::getChild(PDataKey parent, std::string ID, DataProvider *& provider)
{
	return PDataKey(NULL); //No childs available
}

std::string StringProvider::getSubpath(PDataKey key)
{
	return ""; //No childs so the path is always ""
}

DataType StringProvider::getType(PDataKey key)
{	
	return static_cast<DataType>(STRING | 
		myPermissions | PRE_VALUE_CHANGE | POST_VALUE_CHANGE | PRE_DELETION);
}

std::string StringProvider::getDescription(PDataKey key)
{
	return myDescription;
}

void StringProvider::setDescription(PDataKey key, std::string description)
{
	myDescription = description;
}

bool StringProvider::getBoolVal(PDataKey key)
{
	return false;
}

void StringProvider::setBoolVal(PDataKey key, bool newValue)
{
	
}

int  StringProvider::getIntVal(PDataKey key)
{
	return 0;
}

void StringProvider::setIntVal(PDataKey key, int newValue)
{
}

float StringProvider::getFloatVal(PDataKey key)
{
	return 0.0;
}

void  StringProvider::setFloatVal(PDataKey key, float newValue)
{
	
}

std::string StringProvider::getStringVal(PDataKey key)
{
	return myValue;
}

void StringProvider::setStringVal(PDataKey key, const std::string & newValue)
{
	//HINT: Doesn't have to test on if the user has the permission to set
	//this since this is done by the DataObject already.

	setValue(newValue);
}

void StringProvider::addChild(PDataKey parent, std::string & suggestedID, 
				DataProvider * provider)
{

	//No childs to add
}

void StringProvider::remove(PDataKey child)
{
	//only me to remove
	delete this;
}

void StringProvider::removeAdopted(PDataKey adopted)
{
	//is not adopting
}

void StringProvider::getChilds(PDataKey parent, std::vector<std::string> & listOfChilds)
{
	//again: No childs!
}

StringProvider::StringProvider(std::string initialValue, std::string description,
						DataType permissions)
{
	myValue = initialValue;
	myDescription = description;
	myPermissions = permissions;
}

StringProvider::~StringProvider()
{
	// Fire a removal event to all observers listening to removal events		
	fireSignal("", PRE_DELETION);
}

} //dime 
