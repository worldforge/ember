//
// C++ Implementation: OgreResourceProvider
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "OgreResourceProvider.h"
#include <OgreResourceGroupManager.h>

namespace Ember {
namespace OgreView {

OgreResourceWrapper::OgreResourceWrapper(const Ogre::DataStreamPtr& dataStream) {
	mSize = dataStream->size();
	mBuffer = new char[mSize];
	dataStream->read(mBuffer, mSize);
}

OgreResourceWrapper::~OgreResourceWrapper() {
	delete[] mBuffer;
}

const char* OgreResourceWrapper::getDataPtr() {
	return mBuffer;
}

bool OgreResourceWrapper::hasData() {
	return mSize != 0;
}

size_t OgreResourceWrapper::getSize() {
	return mSize;
}


OgreResourceProvider::OgreResourceProvider(std::string groupName)
		: mGroupName(std::move(groupName)) {
}


OgreResourceProvider::~OgreResourceProvider() = default;

ResourceWrapper OgreResourceProvider::getResource(const std::string& name) {
	Ogre::DataStreamPtr input =
			Ogre::ResourceGroupManager::getSingleton().openResource(name, mGroupName);

	if (!input) {
		throw Exception("Unable to open resource file '" + name + "' in resource group '" + name + "'.");
	}
	auto wrapper = std::make_shared<OgreResourceWrapper>(input);
	input->close();
	return ResourceWrapper(std::move(wrapper), name);
}


}
}
