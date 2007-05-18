//
// C++ Implementation: EntityEditor
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#include "EntityEditor.h"

#include "adapters/atlas/AdapterBase.h"
#include "adapters/atlas/MapAdapter.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include <Atlas/Message/Element.h>
#include <Eris/Entity.h>

namespace EmberOgre {

namespace Gui {

EntityEditor::EntityEditor(Eris::Entity* entity, Adapters::Atlas::MapAdapter* rootAdapter)
: mRootAdapter(rootAdapter), mEntity(entity)
{
}


EntityEditor::~EntityEditor()
{
	delete mRootAdapter;
}

// std::vector<std::string> EntityEditor::getAttributeNames()
// {
// 	std::vector<std::string> attributeNames;
// 	for (Eris::Entity::AttrMap::const_iterator I = mEntity->getAttributes().begin(); I != mEntity->getAttributes().end(); ++I) {
// 		attributeNames.push_back(I->first);
// 	}
// 	return attributeNames;
// }


void EntityEditor::submitChanges()
{
	if (mRootAdapter->hasChanges()) {
		Atlas::Message::Element rootElement = mRootAdapter->getSelectedChangedElements();
		if (rootElement.isMap()) {
			std::map<std::string, ::Atlas::Message::Element> attributes(rootElement.asMap());
			if (attributes.size()) {
				Ember::EmberServices::getSingleton().getServerService()->setAttributes(mEntity, attributes);		
			}
		}
	}
}

// void EntityEditor::addAttributeAdapter(const std::string& attributeName, Adapters::Atlas::AdapterBase* adapter, CEGUI::Window* containerWindow)
// {
// 	AdapterWrapper wrapper;
// 	wrapper.Adapter = adapter;
// 	wrapper.ContainerWindow = containerWindow;
// 	mAdapters.insert(AdapterStore::value_type(attributeName, wrapper));
// }
//     
// void EntityEditor::removeAdapters()
// {
// 	for (AdapterStore::iterator I = mAdapters.begin(); I != mAdapters.end(); ++I) {
// 		delete I->second.Adapter;
// // 		I->second.ContainerWindow->getParent()->removeChildWindow(I->second.ContainerWindow);
// 		CEGUI::WindowManager::getSingleton().destroyWindow(I->second.ContainerWindow);
// 	}
// 	mAdapters.clear();
// }


}

}
