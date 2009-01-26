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
#include "components/ogre/terrain/TerrainArea.h"
#include <wfmath/polygon.h>


namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

EntityAreaPolygonPositionProvider::EntityAreaPolygonPositionProvider(EmberEntity& entity)
: mEntity(entity)
{
}

float EntityAreaPolygonPositionProvider::getHeightForPosition(const WFMath::Point<2>& localPosition)
{
	//TODO: implement
	return 1;
}


AreaAdapter::AreaAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity)
: AdapterBase(element), mShowButton(showButton), mPolygon(0), mEntity(entity), mPositionProvider(0)
{
	if (entity) {
		mPositionProvider = new EntityAreaPolygonPositionProvider(*entity);
	}
	
	if (showButton) {
		addGuiEventConnection(showButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&AreaAdapter::showButton_Clicked, this)));
	}
	
	updateGui(mOriginalElement);
}


AreaAdapter::~AreaAdapter()
{
	delete mPolygon;
	delete mPositionProvider;
}

void AreaAdapter::updateGui(const ::Atlas::Message::Element& element)
{
}

bool AreaAdapter::showButton_Clicked(const CEGUI::EventArgs& e) {
	showPolygon();
}

void AreaAdapter::showPolygon()
{
	if (!mPolygon) {
		mPolygon = new ::EmberOgre::Manipulation::Polygon(mEntity->getSceneNode(), mPositionProvider);
		
		::Atlas::Message::Element areaElem(getChangedElement());
	
		if (areaElem.isMap()) {
			const ::Atlas::Message::MapType& areaData(areaElem.asMap());
	
			WFMath::Polygon<2> poly;
			int layer = 0;
			Terrain::TerrainAreaParser parser;
			if (parser.parseArea(areaData, poly, layer)) {
// 				if (mEntity) {
// 					/// transform polygon into terrain coords
// 					WFMath::Vector<3> xVec = WFMath::Vector<3>(1.0, 0.0, 0.0).rotate(mEntity->getOrientation());
// 					double theta = atan2(xVec.y(), xVec.x()); // rotation about Z
// 					
// 					WFMath::RotMatrix<2> rm;
// 					poly.rotatePoint(rm.rotation(theta), WFMath::Point<2>(0,0));
// 					poly.shift(WFMath::Vector<2>(mEntity->getPosition().x(), mEntity->getPosition().y()));
// 				}
				mPolygon->loadFromShape(poly);
			}
		}
	}
}

void AreaAdapter::fillElementFromGui()
{
	
}

bool AreaAdapter::_hasChanges()
{
	return true;
}


}

}

}

}
