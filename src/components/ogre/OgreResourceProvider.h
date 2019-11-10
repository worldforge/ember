//
// C++ Interface: OgreResourceProvider
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGREOGRERESOURCEPROVIDER_H
#define EMBEROGREOGRERESOURCEPROVIDER_H

#include "framework/IResourceProvider.h"
#include "components/ogre/EmberOgrePrerequisites.h"
#include <OgreDataStream.h>

namespace Ember {
namespace OgreView {


class OgreResourceWrapper : public IResourceWrapper
{
public:
	explicit OgreResourceWrapper(const Ogre::DataStreamPtr& dataStream);
	~OgreResourceWrapper() override;

	const char* getDataPtr() override;
	bool hasData() override;
	size_t getSize() override;
private:
	char* mBuffer;
	size_t mSize;
};


/**
	@author Erik Ogenvik <erik@ogenvik.org>
*/
class OgreResourceProvider : public IResourceProvider
{
public:
    explicit OgreResourceProvider(std::string groupName);

    ~OgreResourceProvider() override;

	ResourceWrapper getResource(const std::string& name) override;
private:
	std::string mGroupName;

};

}

}

#endif
