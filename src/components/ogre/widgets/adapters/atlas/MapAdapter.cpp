//
// C++ Implementation: MapAdapter
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

#include "MapAdapter.h"

using Atlas::Message::Element;
using Atlas::Message::ListType;
using Atlas::Message::MapType;


namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {


MapAdapter::MapAdapter(const ::Atlas::Message::Element& element, CEGUI::Window* childContainer)
:  AdapterBase(element), mChildContainer(childContainer),
mAttributes(element.asMap())
{
}


MapAdapter::~MapAdapter()
{
	removeAdapters();
}

const ::Atlas::Message::Element& MapAdapter::valueOfAttr(const std::string& attr) const
{
	static Element emptyElement;
	auto A = mAttributes.find(attr);
    if (A == mAttributes.end())
    {
        return emptyElement;
    } else {
        return A->second;
    }
}

bool MapAdapter::hasAttr(const std::string& attr) const
{
    return mAttributes.count(attr) > 0;
}

void MapAdapter::updateGui(const ::Atlas::Message::Element& element)
{
}


void MapAdapter::fillElementFromGui()
{
	
}

bool MapAdapter::_hasChanges()
{
	bool hasChanges = false;
	for (auto& item : mAdapters) {
		if (item.second.Adapter) {
			hasChanges = hasChanges || item.second.Adapter->hasChanges();
		}
	}
	return hasChanges;
}

std::vector<std::string> MapAdapter::getAttributeNames()
{
	std::vector<std::string> attributeNames;
	for (const auto& attribute : mAttributes) {
		attributeNames.push_back(attribute.first);
	}
	return attributeNames;
}

void MapAdapter::addAttributeAdapter(const std::string& attributeName, Adapters::Atlas::AdapterBase* adapter, CEGUI::Window* containerWindow)
{
	AdapterWrapper wrapper;
	wrapper.Adapter = adapter;
	wrapper.ContainerWindow = containerWindow;
	mAdapters.insert(AdapterStore::value_type(attributeName, wrapper));
}
    
void MapAdapter::removeAdapters()
{
	for (auto& item : mAdapters) {
		delete item.second.Adapter;
// 		I->second.ContainerWindow->getParent()->removeChild(I->second.ContainerWindow);
		CEGUI::WindowManager::getSingleton().destroyWindow(item.second.ContainerWindow);
	}
	mAdapters.clear();
}

::Atlas::Message::Element MapAdapter::_getChangedElement()
{
	::Atlas::Message::MapType attributes;
	for (auto& item : mAdapters) {
		Adapters::Atlas::AdapterBase* adapter = item.second.Adapter;
		if (!adapter->isRemoved()) {
			attributes.insert(std::map<std::string, ::Atlas::Message::Element>::value_type(item.first, adapter->getChangedElement()));
		}
	}
	return Element(attributes);
}

::Atlas::Message::Element MapAdapter::getSelectedChangedElements()
{
	::Atlas::Message::MapType attributes;
	for (auto& item : mAdapters) {
		Adapters::Atlas::AdapterBase* adapter = item.second.Adapter;
		if (adapter->hasChanges() && !adapter->isRemoved()) {
			attributes.insert(std::map<std::string, ::Atlas::Message::Element>::value_type(item.first, adapter->getChangedElement()));
		}
	}
	return Element(attributes);
}

bool MapAdapter::hasAdapter(const std::string& attr) const
{
	return mAdapters.find(attr) != mAdapters.end();
}


}

}

}

}
}
