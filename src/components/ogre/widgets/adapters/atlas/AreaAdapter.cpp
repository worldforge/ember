//
// C++ Implementation: AreaAdapter
//
// Description: 
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2009
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

#include "AreaAdapter.h"
#include "components/ogre/terrain/TerrainAreaParser.h"
#include "components/ogre/EmberOgre.h"
#include "../../ColouredListItem.h"

#include "PolygonAdapter.h"

#include <wfmath/atlasconv.h>

#include <memory>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

AreaAdapter::AreaAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, CEGUI::Combobox* layerWindow, EmberEntity* entity) :
		AdapterBase(element),
		mLayer(0),
		mLayerWindow(layerWindow),
		mEntity(entity),
		mPolygonAdapter(nullptr) {
	if (element.isMap()) {
		const ::Atlas::Message::MapType& areaData(element.asMap());
		auto shapeI = areaData.find("shape");
		if (shapeI != areaData.end()) {
			mPolygonAdapter = std::make_unique<PolygonAdapter>(shapeI->second, showButton, entity);
		} else {
			::Atlas::Message::MapType defaultShape;

			mPolygonAdapter = std::make_unique<PolygonAdapter>(getDefaultPolygon().toAtlas(), showButton, entity);
		}
		WFMath::Polygon<2> poly;
		Terrain::TerrainAreaParser parser;
		parser.parseArea(areaData, poly, mLayer);
	} else {
		mPolygonAdapter = std::make_unique<PolygonAdapter>(getDefaultPolygon().toAtlas(), showButton, entity);
	}

	if (mLayerWindow) {
		addGuiEventConnection(mLayerWindow->subscribeEvent(CEGUI::Combobox::EventListSelectionChanged, CEGUI::Event::Subscriber(&AreaAdapter::layerWindow_ListSelectionChanged, this)));
	}

	updateGui(mOriginalValue);
}


WFMath::Polygon<2> AreaAdapter::getDefaultPolygon() const {
	WFMath::Polygon<2> poly;
	poly.addCorner(0, WFMath::Point<2>(-1, -1));
	poly.addCorner(1, WFMath::Point<2>(-1, 1));
	poly.addCorner(2, WFMath::Point<2>(1, 1));
	poly.addCorner(3, WFMath::Point<2>(1, -1));
	return poly;
}

void AreaAdapter::updateGui(const ::Atlas::Message::Element& element) {
}

bool AreaAdapter::layerWindow_TextChanged(const CEGUI::EventArgs& e) {
	if (!mSelfUpdate) {
		mLayer = std::stoi(mLayerWindow->getText().c_str());
		EventValueChanged.emit();
	}
	return true;
}

bool AreaAdapter::layerWindow_ListSelectionChanged(const CEGUI::EventArgs& e) {
	if (!mSelfUpdate) {
		if (mLayerWindow->getSelectedItem()) {
			mLayer = (int) mLayerWindow->getSelectedItem()->getID();
		} else {
			mLayer = 0;
		}
		EventValueChanged.emit();
	}
	return true;
}

void AreaAdapter::toggleDisplayOfPolygon() {
	mPolygonAdapter->toggleDisplayOfPolygon();
}

void AreaAdapter::createNewPolygon() {
	mPolygonAdapter->createNewPolygon();
}

void AreaAdapter::fillElementFromGui() {
	//Start by using the shape element from the polygon adapter
	mEditedValue = ::Atlas::Message::MapType();
	CEGUI::ListboxItem* item = mLayerWindow->getSelectedItem();
	if (item) {
		mLayer = (int) item->getID();
	}
	Terrain::TerrainAreaParser parser;
	mEditedValue = parser.createElement(mPolygonAdapter->getShape(), mLayer);
}

bool AreaAdapter::_hasChanges() {
	return mOriginalValue != getChangedElement();
}

void AreaAdapter::addAreaSuggestion(int id, const std::string& name) {
	auto* item = new ColouredListItem(name, id);
	mLayerWindow->addItem(item);
	if (id == mLayer) {
		item->setSelected(true);
		mLayerWindow->setText(name);
	}
}

void AreaAdapter::clearAreaSuggestions() {
	mLayerWindow->resetList();
	mLayerWindow->setText("");
}

}

}

}

}
}
