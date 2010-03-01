//
// C++ Implementation: AreaAdapter
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009
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

#include "AreaAdapter.h"
#include "components/ogre/terrain/TerrainAreaParser.h"
#include "components/ogre/EmberOgre.h"
#include "../../ColouredListItem.h"
#include "components/ogre/authoring/PolygonPointPickListener.h"

#include "PolygonAdapter.h"


#include <wfmath/polygon.h>

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {



AreaAdapter::AreaAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, CEGUI::Combobox* layerWindow, EmberEntity* entity)
: AdapterBase(element), mLayer(0), mLayerWindow(layerWindow), mEntity(entity), mPolygonAdapter(0)
{
	mPolygonAdapter = std::auto_ptr<PolygonAdapter>(new PolygonAdapter(element, showButton, entity));
	if (element.isMap()) {
		const ::Atlas::Message::MapType& areaData(element.asMap());

		WFMath::Polygon<2> poly;
		Terrain::TerrainAreaParser parser;
		parser.parseArea(areaData, poly, mLayer);
	}

	if (mLayerWindow) {
		addGuiEventConnection(mLayerWindow->subscribeEvent(CEGUI::Combobox::EventListSelectionChanged, CEGUI::Event::Subscriber(&AreaAdapter::layerWindow_ListSelectionChanged, this))); 
	}
	
	updateGui(mOriginalElement);
}


AreaAdapter::~AreaAdapter()
{
}

void AreaAdapter::updateGui(const ::Atlas::Message::Element& element)
{
}

bool AreaAdapter::layerWindow_TextChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		mLayer = atoi(mLayerWindow->getText().c_str());
		EventValueChanged.emit();
	}
	return true;
}

bool AreaAdapter::layerWindow_ListSelectionChanged(const CEGUI::EventArgs& e)
{
	if (!mSelfUpdate) {
		if (mLayerWindow->getSelectedItem()) {
			mLayer = mLayerWindow->getSelectedItem()->getID();
		} else {
			mLayer = 0;
		}
		EventValueChanged.emit();
	}
	return true;
}

void AreaAdapter::toggleDisplayOfPolygon()
{
	mPolygonAdapter->toggleDisplayOfPolygon();
}

void AreaAdapter::createNewPolygon()
{
	mPolygonAdapter->createNewPolygon();
}


void AreaAdapter::fillElementFromGui()
{
	///Start by using the shape element from the polygon adapter
	mEditedElement = mPolygonAdapter->getChangedElement();
	CEGUI::ListboxItem* item = mLayerWindow->getSelectedItem();
	if (item) {
		mLayer = item->getID();
	}
	if (mPolygonAdapter->hasShape()) {
		Terrain::TerrainAreaParser parser;
		mEditedElement = parser.createElement(mPolygonAdapter->getShape(), mLayer);
	} else {
		if (mEditedElement.isMap() && mLayer) {
			mEditedElement.asMap()["layer"] = mLayer;
		}
	}
}

bool AreaAdapter::_hasChanges()
{
	return mOriginalElement != getChangedElement();
}

void AreaAdapter::addAreaSuggestion(int id, const std::string& name)
{
	ColouredListItem* item = new ColouredListItem(name, id);
	mLayerWindow->addItem(item);
	if (id == mLayer) {
		item->setSelected(true);
		mLayerWindow->setText(name);
	}
}

void AreaAdapter::clearAreaSuggestions()
{
	mLayerWindow->resetList();
	mLayerWindow->setText("");
}


}

}

}

}
