//
// C++ Interface: OgreResourceProvider
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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
#ifndef EMBEROGREOGRERESOURCEPROVIDER_H
#define EMBEROGREOGRERESOURCEPROVIDER_H

#include "framework/IScriptingProvider.h"
#include <Ogre.h>

namespace EmberOgre {


class OgreResourceWrapper : public Ember::IResourceWrapper
{
public:
	OgreResourceWrapper(Ogre::DataStreamPtr dataStream);
	virtual ~OgreResourceWrapper();
	
	virtual char* getDataPtr();
	virtual bool hasData();
	virtual size_t getSize();
private:
	char* mBuffer;
	size_t mSize;
};


/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class OgreResourceProvider : public Ember::IResourceProvider
{
public:
    OgreResourceProvider(const std::string& groupName);

    virtual ~OgreResourceProvider();
    
	virtual Ember::ResourceWrapper getResource(const std::string& name);
private: 
	std::string mGroupName;

};

}

#endif
