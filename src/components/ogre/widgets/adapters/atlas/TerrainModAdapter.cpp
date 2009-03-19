//
// C++ Implementation: TerrainModAdapter
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

#include "TerrainModAdapter.h"
#include "PolygonAdapter.h"


namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {





TerrainModBase::TerrainModBase(const std::string& type)
: mType(type)
{
}

const std::string& TerrainModBase::getType() const
{
	return mType;
}

LevelTerrainMod::LevelTerrainMod()
: TerrainModBase("levelmod")
{
}

AdjustTerrainMod::AdjustTerrainMod()
: TerrainModBase("adjustmod")
{
}


PositioningBase::PositioningBase(const std::string& type)
: mType(type)
{
}

const std::string& PositioningBase::getType() const
{
	return mType;
}

FixedPositioning::FixedPositioning()
: PositioningBase("height")
{
}

RelativePositioning::RelativePositioning()
: PositioningBase("heightoffset")
{
}


TerrainModAdapter::TerrainModAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity, CEGUI::Combobox* posTypeCombobox, CEGUI::Combobox* modTypeCombobox, CEGUI::Editbox* heightTextbox)
: AdapterBase(element), mEntity(entity), mPolygonAdapter(0), mHeightTextbox(heightTextbox), mTerrainModsBinder(modTypeCombobox), mPositioningsBinder(posTypeCombobox)
{
	
	if (element.isMap()) {
		const ::Atlas::Message::MapType& areaData(element.asMap());
		::Atlas::Message::MapType::const_iterator I = areaData.find("shape");
		if (I != areaData.end()) {
			mPolygonAdapter = std::auto_ptr<PolygonAdapter>(new PolygonAdapter(I->second, showButton, entity));
		} else {
			mPolygonAdapter = std::auto_ptr<PolygonAdapter>(new PolygonAdapter(::Atlas::Message::Element(), showButton, entity));
		}
	} else {
		mPolygonAdapter = std::auto_ptr<PolygonAdapter>(new PolygonAdapter(::Atlas::Message::Element(), showButton, entity));
	}
	
	if (heightTextbox) {
		addGuiEventConnection(heightTextbox->subscribeEvent(CEGUI::Window::EventTextChanged, CEGUI::Event::Subscriber(&TerrainModAdapter::heightTextbox_TextChanged, this))); 
	}
	mTerrainModsBinder.addType("levelmod", "Level", LevelTerrainMod());
	mTerrainModsBinder.addType("adjustmod", "Adjust", AdjustTerrainMod());
	
	mPositioningsBinder.addType("height", "Fixed", FixedPositioning());
	mPositioningsBinder.addType("heightoffset", "Relative", RelativePositioning());
	
	updateGui(element);
}


TerrainModAdapter::~TerrainModAdapter()
{
}

void TerrainModAdapter::updateGui(const ::Atlas::Message::Element& element)
{
	mPositioningsBinder.sync();
	mTerrainModsBinder.sync();
	if (element.isMap()) {
		const ::Atlas::Message::MapType& mapElement = element.asMap();
		::Atlas::Message::MapType::const_iterator I = mapElement.find("height");
		float height = 0;
		if (I != mapElement.end()) {
			mPositioningsBinder.select("height");
			if (I->second.isNum()) {
				height = I->second.asNum();
			}
		} else {
			I = mapElement.find("heightoffset");
			if (I != mapElement.end()) {
				mPositioningsBinder.select("heightoffset");
				if (I->second.isNum()) {
					height = I->second.asNum();
				}
			}
		}
		std::stringstream ss;
		ss << height;
		mHeightTextbox->setText(ss.str());
		
		I = mapElement.find("type");
		if (I != mapElement.end() && I->second.isString()) {
			mTerrainModsBinder.select(I->second.asString());
		}
	}
}

bool TerrainModAdapter::heightTextbox_TextChanged(const CEGUI::EventArgs& e)
{
	return true;
}

void TerrainModAdapter::fillElementFromGui()
{
	::Atlas::Message::MapType mapElement;
	::Atlas::Message::Element shapeElement = mPolygonAdapter->getChangedElement();
	if (shapeElement.isMap()) {
		shapeElement.asMap()["type"] = "polygon";
	}
	mapElement["shape"] = shapeElement;
	
	std::pair<const std::string&, PositioningBase*> positioning = mPositioningsBinder.getCurrentSelected();
	if (positioning.second) {
		mapElement[positioning.first] = ::Atlas::Message::Element(atof(mHeightTextbox->getText().c_str()));
	}
	std::pair<const std::string&, TerrainModBase*> modType = mTerrainModsBinder.getCurrentSelected();
	if (modType.second) {
		mapElement["type"] = modType.first;
	}
	
	mEditedElement = mapElement;
}

bool TerrainModAdapter::_hasChanges()
{
	return mOriginalElement != getChangedElement();
}
}

}

}

}
