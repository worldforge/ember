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
#include "BoolProvider.h"

// Include library headers here

// Include system headers here

namespace dime {

bool BoolProvider::getValue()
{
	return myValue;
}

void BoolProvider::setPermissions(DataType permissions)
{
	myPermissions = permissions;
}

void BoolProvider::setValue(bool newValue)
{
	fireSignal("", PRE_VALUE_CHANGE);

	myValue = newValue;
	
	fireSignal("", POST_VALUE_CHANGE);
}

PDataKey BoolProvider::getChild(PDataKey parent, std::string ID, DataProvider *& provider)
{
	return PDataKey(NULL); //No childs available
}

std::string BoolProvider::getSubpath(PDataKey key)
{
	return ""; //No childs so the path is always ""
}

DataType BoolProvider::getType(PDataKey key)
{	
	return static_cast<DataType>(BOOLEAN | 
		myPermissions | PRE_VALUE_CHANGE | POST_VALUE_CHANGE | PRE_DELETION);
}

std::string BoolProvider::getDescription(PDataKey key)
{
	return myDescription;
}

void BoolProvider::setDescription(PDataKey key, std::string description)
{
	myDescription = description;
}

bool BoolProvider::getBoolVal(PDataKey key)
{
	return myValue;
}

void BoolProvider::setBoolVal(PDataKey key, bool newValue)
{
	//HINT: Doesn't have to test on if the user has the permission to set
	//this since this is done by the DataObject already.

	setValue(newValue);	
}

int  BoolProvider::getIntVal(PDataKey key)
{
	return 0;
}

void BoolProvider::setIntVal(PDataKey key, int newValue)
{

}

float BoolProvider::getFloatVal(PDataKey key)
{
	return 0.0;
}

void  BoolProvider::setFloatVal(PDataKey key, float newValue)
{
	
}

std::string BoolProvider::getStringVal(PDataKey key)
{
	return "";
}

void BoolProvider::setStringVal(PDataKey key, const std::string & newValue)
{
	
}

void BoolProvider::addChild(PDataKey parent, std::string & suggestedID, 
				DataProvider * provider)
{

	//No childs to add
}

void BoolProvider::remove(PDataKey child)
{
	//only me to remove
	delete this;
}

void BoolProvider::removeAdopted(PDataKey adopted)
{
	//is not adopting
}

void BoolProvider::getChilds(PDataKey parent, std::vector<std::string> & listOfChilds)
{
	//again: No childs!
}

BoolProvider::BoolProvider(bool initialValue, std::string description,
						DataType permissions)
{
	myValue = initialValue;
	myDescription = description;
	myPermissions = permissions;
}

BoolProvider::~BoolProvider()
{
	// Fire a removal event to all observers listening to removal events		
	fireSignal("", PRE_DELETION);
}

} //dime 
