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
#include "IntProvider.h"

// Include library headers here

// Include system headers here

namespace dime {

int IntProvider::getValue()
{
	return myValue;
}

void IntProvider::setPermissions(DataType permissions)
{
	myPermissions = permissions;
}

void IntProvider::setValue(int newValue)
{
	fireSignal("", PRE_VALUE_CHANGE);

	myValue = newValue;
	
	fireSignal("", POST_VALUE_CHANGE);
}

PDataKey IntProvider::getChild(PDataKey parent, std::string ID, DataProvider *& provider)
{
	return PDataKey(); //No childs available
}

std::string IntProvider::getSubpath(PDataKey key)
{
	return ""; //No childs so the path is always ""
}

DataType IntProvider::getType(PDataKey key)
{	
	return static_cast<DataType>(INTEGER | 
		myPermissions | PRE_VALUE_CHANGE | POST_VALUE_CHANGE | PRE_DELETION);
}

std::string IntProvider::getDescription(PDataKey key)
{
	return myDescription;
}

void IntProvider::setDescription(PDataKey key, std::string description)
{
	myDescription = description;
}

bool IntProvider::getBoolVal(PDataKey key)
{
	return false;
}

void IntProvider::setBoolVal(PDataKey key, bool newValue)
{
	
}

int  IntProvider::getIntVal(PDataKey key)
{
	return myValue;
}

void IntProvider::setIntVal(PDataKey key, int newValue)
{
	//HINT: Doesn't have to test on if the user has the permission to set
	//this since this is done by the DataObject already.

	setValue(newValue);
}

float IntProvider::getFloatVal(PDataKey key)
{
	return 0.0;
}

void  IntProvider::setFloatVal(PDataKey key, float newValue)
{
	
}

std::string IntProvider::getStringVal(PDataKey key)
{
	return "";
}

void IntProvider::setStringVal(PDataKey key, const std::string & newValue)
{
	
}

void IntProvider::addChild(PDataKey parent, std::string & suggestedID, 
				DataProvider * provider)
{

	//No childs to add
}

void IntProvider::remove(PDataKey child)
{
	//only me to remove
	delete this;
}

void IntProvider::removeAdopted(PDataKey adopted)
{
	//is not adopting
}

void IntProvider::getChilds(PDataKey parent, std::vector<std::string> & listOfChilds)
{
	//again: No childs!
}

IntProvider::IntProvider(int initialValue, std::string description,
						DataType permissions)
{
	myValue = initialValue;
	myDescription = description;
	myPermissions = permissions;
}

IntProvider::~IntProvider()
{
	// Fire a removal event to all observers listening to removal events		
	fireSignal("", PRE_DELETION);
}

} //dime 
