//
// C++ Implementation: PolygonAdapter
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

#include "PolygonAdapter.h"

#include "components/ogre/EmberOgre.h"
#include "components/ogre/MathConverter.h"
#include "components/ogre/terrain/TerrainGenerator.h"
#include "components/ogre/manipulation/PolygonPointPickListener.h"
#include "components/ogre/AvatarCamera.h"

#include <wfmath/polygon.h>
#include <wfmath/atlasconv.h>

#include <OgreSceneNode.h>

namespace EmberOgre {

namespace Gui {

namespace Adapters {

namespace Atlas {

EntityPolygonPositionProvider::EntityPolygonPositionProvider(EmberEntity& entity)
: mEntity(entity)
{
}

float EntityPolygonPositionProvider::getHeightForPosition(const WFMath::Point<2>& localPosition)
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


PolygonAdapter::PolygonAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity)
: AdapterBase(element), mShowButton(showButton), mPolygon(0), mPickListener(0), mPointMovement(0), mEntity(entity), mPositionProvider(0)
{

	if (entity) {
		mPositionProvider = new EntityPolygonPositionProvider(*entity);
	}

	if (showButton) {
		addGuiEventConnection(showButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PolygonAdapter::showButton_Clicked, this)));
	}

	updateGui(mOriginalElement);
}


PolygonAdapter::~PolygonAdapter()
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

void PolygonAdapter::updateGui(const ::Atlas::Message::Element& element)
{
}

bool PolygonAdapter::showButton_Clicked(const CEGUI::EventArgs& e) {
	toggleDisplayOfPolygon();
	return true;
}

void PolygonAdapter::toggleDisplayOfPolygon()
{
	if (!mPolygon) {
		if (!mEntity) {
			S_LOG_WARNING("There's no entity attached to the PolygonAdapter, and the polygon can't thus be shown.");
		} else {
			::Atlas::Message::Element areaElem(getChangedElement());

			mPolygon = new ::EmberOgre::Manipulation::Polygon(mEntity->getSceneNode(), mPositionProvider);


			if (areaElem.isMap()) {
				try {
					WFMath::Polygon<2> poly(areaElem);
					mPolygon->loadFromShape(poly);
					mPickListener = new ::EmberOgre::Manipulation::PolygonPointPickListener(*mPolygon);
					mPickListener->EventPickedPoint.connect(sigc::mem_fun(*this, &PolygonAdapter::pickListener_PickedPoint));
					EmberOgre::getSingleton().getMainCamera()->pushWorldPickListener(mPickListener);
				} catch (const WFMath::_AtlasBadParse& ex) {
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

void PolygonAdapter::createNewPolygon()
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
	mPickListener->EventPickedPoint.connect(sigc::mem_fun(*this, &PolygonAdapter::pickListener_PickedPoint));
	EmberOgre::getSingleton().getMainCamera()->pushWorldPickListener(mPickListener);
}

void PolygonAdapter::fillElementFromGui()
{
	if (mPolygon) {
		mEditedElement = mPolygon->getShape().toAtlas();
	}
}

bool PolygonAdapter::_hasChanges()
{
	return mOriginalElement != getChangedElement();
}

void PolygonAdapter::pickListener_PickedPoint(Manipulation::PolygonPoint& point)
{
	delete mPointMovement;
	mPointMovement = new Manipulation::PolygonPointMovement(point, this);
}

void PolygonAdapter::endMovement()
{
	delete mPointMovement;
	mPointMovement = 0;
}
void PolygonAdapter::cancelMovement()
{
	delete mPointMovement;
	mPointMovement = 0;
}

bool PolygonAdapter::hasShape() const
{
	return mPolygon != 0;
}

const WFMath::Polygon<2> PolygonAdapter::getShape() const
{
	if (mPolygon) {
		return mPolygon->getShape();
	}
	return WFMath::Polygon<2>();
}

}

}

}

}
