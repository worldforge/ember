/*
    Copyright (C) 2002  Tim Enderling

    This program is free software; you can redistrbute it and/or modify
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
#include "FloatProvider.h"

// Include library headers here

// Include system headers here

namespace dime {

float FloatProvider::getValue()
{
	return myValue;
}

void FloatProvider::setPermissions(DataType permissions)
{
	myPermissions = permissions;
}

void FloatProvider::setValue(float newValue)
{
	fireSignal("", PRE_VALUE_CHANGE);

	myValue = newValue;
	
	fireSignal("", POST_VALUE_CHANGE);
}

PDataKey FloatProvider::getChild(PDataKey parent, std::string ID, DataProvider *& provider)
{
	return PDataKey(); //No childs available
}

std::string FloatProvider::getSubpath(PDataKey key)
{
	return ""; //No childs so the path is always ""
}

DataType FloatProvider::getType(PDataKey key)
{	
	return static_cast<DataType>(FLOAT | 
		myPermissions | PRE_VALUE_CHANGE | POST_VALUE_CHANGE | PRE_DELETION);
}

std::string FloatProvider::getDescription(PDataKey key)
{
	return myDescription;
}

void FloatProvider::setDescription(PDataKey key, std::string description)
{
	myDescription = description;
}

bool FloatProvider::getBoolVal(PDataKey key)
{
	return false;
}

void FloatProvider::setBoolVal(PDataKey key, bool newValue)
{
	
}

int  FloatProvider::getIntVal(PDataKey key)
{
	return 0;
}

void FloatProvider::setIntVal(PDataKey key, int newValue)
{

}

float FloatProvider::getFloatVal(PDataKey key)
{
	return myValue;
}

void  FloatProvider::setFloatVal(PDataKey key, float newValue)
{
	//HINT: Doesn't have to test on if the user has the permission to set
	//this since this is done by the DataObject already.

	setValue(newValue);
}

std::string FloatProvider::getStringVal(PDataKey key)
{
	return "";
}

void FloatProvider::setStringVal(PDataKey key, const std::string & newValue)
{
	
}

void FloatProvider::addChild(PDataKey parent, std::string & suggestedID, 
				DataProvider * provider)
{

	//No childs to add
}

void FloatProvider::remove(PDataKey child)
{
	//only me to remove
	delete this;
}

void FloatProvider::removeAdopted(PDataKey adopted)
{
	//is not adopting
}

void FloatProvider::getChilds(PDataKey parent, std::vector<std::string> & listOfChilds)
{
	//again: No childs!
}

FloatProvider::FloatProvider(float initialValue, std::string description,
						DataType permissions)
{
	myValue = initialValue;
	myDescription = description;
	myPermissions = permissions;
}

FloatProvider::~FloatProvider()
{
	// Fire a removal event to all observers listening to removal events		
	fireSignal("", PRE_DELETION);
}

} //dime 
