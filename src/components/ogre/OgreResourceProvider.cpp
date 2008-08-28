//
// C++ Implementation: OgreResourceProvider
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreResourceProvider.h"
#include "framework/Exception.h"

namespace EmberOgre {

OgreResourceWrapper::OgreResourceWrapper(Ogre::DataStreamPtr dataStream)
{
	mSize = dataStream->size();
	mBuffer = new char[mSize];
	dataStream->read(mBuffer, mSize);
}

OgreResourceWrapper::~OgreResourceWrapper()
{
	delete[] mBuffer;
}

char* OgreResourceWrapper::getDataPtr()
{
	return mBuffer;
}

bool OgreResourceWrapper::hasData()
{
	return mSize != 0;
}

size_t OgreResourceWrapper::getSize()
{
	return mSize;
}



OgreResourceProvider::OgreResourceProvider(const std::string& groupName)
: mGroupName(groupName)
{
}


OgreResourceProvider::~OgreResourceProvider()
{
}

Ember::ResourceWrapper OgreResourceProvider::getResource(const std::string& name)
{
	Ogre::DataStreamPtr input =
		Ogre::ResourceGroupManager::getSingleton().openResource(name, mGroupName);
	
	if (input.isNull())
	{
		throw Ember::Exception("Unable to open resource file '" + name + "' in resource group '" + name + "'.");
	}
	OgreResourceWrapper* wrapper = new OgreResourceWrapper(input);
	input->close();
	return Ember::ResourceWrapper(wrapper, name);
	
// 	Ogre::String buf = input->getAsString();
// 	const size_t memBuffSize = buf.length();
// 	
// 	unsigned char* mem = new unsigned char[memBuffSize];
// 	memcpy(mem, buf.c_str(), memBuffSize);
// 	
// 	output.setData(mem);
// 	output.setSize(memBuffSize);
}




}
