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
#include "components/ogre/terrain/TerrainAreaParser.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/terrain/TerrainGenerator.h"
#include "../../ColouredListItem.h"
#include "components/ogre/manipulation/PolygonPointPickListener.h"
#include "components/ogre/AvatarCamera.h"

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
	///TODO: refactor into a better structure, so that we don't have to know about the terrain
	::EmberOgre::Terrain::TerrainGenerator* terrain = EmberOgre::getSingleton().getTerrainGenerator();
	if (terrain) {
		Ogre::Vector3 parentPos = mEntity.getSceneNode()->_getDerivedPosition();
		Ogre::Vector3 localPos(localPosition.x(), 0, -localPosition.y());
		localPos = mEntity.getSceneNode()->_getDerivedOrientation() * localPos;
		WFMath::Point<3> worldPos = Ogre2Atlas(parentPos + localPos);
		return terrain->getHeight(WFMath::Point<2>(worldPos.x(), worldPos.y())) - worldPos.z();
	}
	return 0;
}


AreaAdapter::AreaAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, CEGUI::Combobox* layerWindow, EmberEntity* entity)
: AdapterBase(element), mShowButton(showButton), mPolygon(0), mPickListener(0), mPointMovement(0), mLayer(0), mLayerWindow(layerWindow), mEntity(entity), mPositionProvider(0)
{
	if (element.isMap()) {
		const ::Atlas::Message::MapType& areaData(element.asMap());

		WFMath::Polygon<2> poly;
		Terrain::TerrainAreaParser parser;
		parser.parseArea(areaData, poly, mLayer);
	}


	if (entity) {
		mPositionProvider = new EntityAreaPolygonPositionProvider(*entity);
	}
	
	if (showButton) {
		addGuiEventConnection(showButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&AreaAdapter::showButton_Clicked, this)));
	}
	
	if (mLayerWindow) {
		addGuiEventConnection(mLayerWindow->subscribeEvent(CEGUI::Combobox::EventListSelectionChanged, CEGUI::Event::Subscriber(&AreaAdapter::layerWindow_ListSelectionChanged, this))); 
	}
	


	
	updateGui(mOriginalElement);
}


AreaAdapter::~AreaAdapter()
{
	if (mPickListener) {
		EmberOgre::getSingleton().getMainCamera()->removeWorldPickListener(mPickListener);
		try {
			delete mPickListener;
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when deleting polygon point pick listener.");
		}
	}

	delete mPolygon;
	delete mPositionProvider;
}

void AreaAdapter::updateGui(const ::Atlas::Message::Element& element)
{
}

bool AreaAdapter::showButton_Clicked(const CEGUI::EventArgs& e) {
	toggleDisplayOfPolygon();
	return true;
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
	if (!mPolygon) {
		if (!mEntity) {
			S_LOG_WARNING("There's no entity attached to the AreaAdapter, and the polygon can't thus be shown.");
		} else {
			::Atlas::Message::Element areaElem(getChangedElement());
			
			mPolygon = new ::EmberOgre::Manipulation::Polygon(mEntity->getSceneNode(), mPositionProvider);
			
		
			if (areaElem.isMap()) {
				const ::Atlas::Message::MapType& areaData(areaElem.asMap());
		
				WFMath::Polygon<2> poly;
				Terrain::TerrainAreaParser parser;
				if (parser.parseArea(areaData, poly, mLayer)) {
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
					mPickListener = new ::EmberOgre::Manipulation::PolygonPointPickListener(*mPolygon);
					mPickListener->EventPickedPoint.connect(sigc::mem_fun(*this, &AreaAdapter::pickListener_PickedPoint));
					EmberOgre::getSingleton().getMainCamera()->pushWorldPickListener(mPickListener);
				} else {
					createNewPolygon();
				}
			} else {
				createNewPolygon();
			}
		}
	} else {
		if (mPickListener) {
			EmberOgre::getSingleton().getMainCamera()->removeWorldPickListener(mPickListener);
			try {
				delete mPickListener;
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when deleting polygon point pick listener.");
			}
			mPickListener = 0;
		}
		try {
			delete mPolygon;
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when deleting polygon.");
		}
		mPolygon = 0;
	}
}

void AreaAdapter::createNewPolygon()
{
	delete mPolygon;
	mPolygon = new ::EmberOgre::Manipulation::Polygon(mEntity->getSceneNode(), mPositionProvider);
	WFMath::Polygon<2> poly;
	poly.addCorner(0, WFMath::Point<2>(-1, -1));
	poly.addCorner(1, WFMath::Point<2>(-1, 1));
	poly.addCorner(2, WFMath::Point<2>(1, 1));
	poly.addCorner(3, WFMath::Point<2>(1, -1));
	
	mPolygon->loadFromShape(poly);	
	mPickListener = new ::EmberOgre::Manipulation::PolygonPointPickListener(*mPolygon);
	mPickListener->EventPickedPoint.connect(sigc::mem_fun(*this, &AreaAdapter::pickListener_PickedPoint));
	EmberOgre::getSingleton().getMainCamera()->pushWorldPickListener(mPickListener);
	
	mLayer = atoi(mLayerWindow->getText().c_str());
}

void AreaAdapter::fillElementFromGui()
{
	CEGUI::ListboxItem* item = mLayerWindow->getSelectedItem();
	if (item) {
		mLayer = item->getID();
	}
	if (mPolygon) {
		Terrain::TerrainAreaParser parser;
		mEditedElement = parser.createElement(mPolygon->getShape(), mLayer);
	} else {
		if (mEditedElement.isMap()) {
			mEditedElement.asMap()["layer"] = mLayer;
		}
	}
}

bool AreaAdapter::_hasChanges()
{
	return mOriginalElement != getChangedElement();
}

void AreaAdapter::pickListener_PickedPoint(Manipulation::PolygonPoint& point)
{
	delete mPointMovement;
	mPointMovement = new Manipulation::PolygonPointMovement(point, this);
}

void AreaAdapter::endMovement()
{
	delete mPointMovement;
	mPointMovement = 0;
}
void AreaAdapter::cancelMovement()
{
	delete mPointMovement;
	mPointMovement = 0;
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
