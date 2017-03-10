//
// Copyright (C) 2005 Alistair Riddoch
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#include "AtlasMessageLoader.h"

#include "LoggingInstance.h"

#include <Atlas/Objects/Root.h>
#include <Atlas/Objects/SmartPtr.h>

using Atlas::Objects::Root;
namespace Ember
{

AtlasMessageLoader::AtlasMessageLoader(std::map<std::string, Root>& messages) :
		mCount(0), mMessages(messages)
{
}

AtlasMessageLoader::~AtlasMessageLoader()
{
}

void AtlasMessageLoader::objectArrived(const Root & obj)
{
	if (obj->isDefaultId()) {
		S_LOG_WARNING("Object without ID read from file.");
		return;
	}
	const std::string & id = obj->getId();
	if (mMessages.find(id) != mMessages.end()) {
		S_LOG_WARNING("Duplicate object ID '"<< id << "' loaded.");
	}
	mMessages[id] = obj;
	++mCount;
}
int AtlasMessageLoader::count()
{
	return mCount;
}

}

