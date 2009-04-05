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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "EntityEditor.h"

#include "adapters/atlas/AdapterBase.h"
#include "adapters/atlas/MapAdapter.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "framework/LoggingInstance.h"

#include <Atlas/Message/Element.h>
#include <Atlas/Objects/Operation.h>
#include <Atlas/Message/QueuedDecoder.h>
#include <Atlas/Codecs/XML.h>
#include <Atlas/Formatter.h>

#include <Eris/Entity.h>



using namespace Atlas::Message;

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

void EntityEditor::submitChanges()
{
	if (mRootAdapter->hasChanges()) {
		Atlas::Message::Element rootElement = mRootAdapter->getSelectedChangedElements();
		if (rootElement.isMap()) {
			std::map<std::string, ::Atlas::Message::Element> attributes(rootElement.asMap());
			if (attributes.size()) {
			
				std::stringstream ss;
			
				Atlas::Message::QueuedDecoder decoder;
			
				Atlas::Codecs::XML codec(ss, decoder);
				Atlas::Formatter formatter(ss, codec);
				Atlas::Message::Encoder encoder(formatter);
				formatter.streamBegin();
				encoder.streamMessageElement(attributes);
				formatter.streamEnd();
				S_LOG_VERBOSE("Sending attribute update to server:\n" << ss.str());	
				
				Ember::EmberServices::getSingleton().getServerService()->setAttributes(mEntity, attributes);		
			}
		}
	}
}

Atlas::Message::Element EntityEditor::createMapElement()
{
	return Element(MapType());
}

Atlas::Message::Element EntityEditor::createListElement()
{
	return Element(ListType());
}
Atlas::Message::Element EntityEditor::createStringElement()
{
	return Element("");
}
Atlas::Message::Element EntityEditor::createIntElement()
{
	return Element(0);
}
Atlas::Message::Element EntityEditor::createFloatElement()
{
	return Element(0.0f);
}
Atlas::Message::Element EntityEditor::createPosition2dElement()
{
	ListType list;
	list.push_back(createFloatElement());
	list.push_back(createFloatElement());
	return Element(list);
}


}

}
