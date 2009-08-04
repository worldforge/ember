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
#include "components/ogre/Convert.h"
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/terrain/TerrainGenerator.h"
#include "components/ogre/manipulation/PolygonPointPickListener.h"
#include "components/ogre/camera/MainCamera.h"

#include <wfmath/polygon.h>
#include <wfmath/atlasconv.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace EmberOgre
{

namespace Gui
{

namespace Adapters
{

namespace Atlas
{

EntityPolygonPositionProvider::EntityPolygonPositionProvider(EmberEntity& entity) :
	mEntity(entity)
{
}

float EntityPolygonPositionProvider::getHeightForPosition(const WFMath::Point<2>& localPosition)
{
	///TODO: refactor into a better structure, so that we don't have to know about the terrain
	const ::EmberOgre::Terrain::TerrainGenerator* terrain = EmberOgre::getSingleton().getTerrainGenerator();
	if (terrain) {
		Ogre::Vector3 parentPos = Convert::toOgre(mEntity.getViewPosition());
		Ogre::Vector3 localPos(localPosition.x(), 0, -localPosition.y());
		localPos = Convert::toOgre(mEntity.getViewOrientation()) * localPos;
		WFMath::Point<3> worldPos = Convert::toWF<WFMath::Point<3> >(parentPos + localPos);
		float height = 0;
		if (terrain->getHeight(WFMath::Point<2>(worldPos.x(), worldPos.y()), height)) {
			return height - worldPos.z();
		}
	}
	return 0;
}

PolygonAdapter::PolygonAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity) :
	AdapterBase(element), mShowButton(showButton), mPolygon(0), mPickListener(0), mPointMovement(0), mEntity(entity), mPositionProvider(0)
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

bool PolygonAdapter::showButton_Clicked(const CEGUI::EventArgs& e)
{
	toggleDisplayOfPolygon();
	return true;
}

Ogre::SceneNode* PolygonAdapter::getEntitySceneNode() const
{
	NodeAttachment* nodeAttachment = dynamic_cast<NodeAttachment*> (mEntity->getAttachment());
	if (nodeAttachment) {
		return dynamic_cast<Ogre::SceneNode*> (nodeAttachment->getNode());
	}
	return 0;
}

void PolygonAdapter::toggleDisplayOfPolygon()
{
	if (!mPolygon) {
		if (!mEntity) {
			S_LOG_WARNING("There's no entity attached to the PolygonAdapter, and the polygon can't thus be shown.");
		} else {
			//It's important that we do the call to getChangedElement before we create and set mPolygon, since if that's set, the values from there will be used instead of the original atlas values.
			::Atlas::Message::Element areaElem(getChangedElement());

			Ogre::SceneNode* entitySceneNode = getEntitySceneNode();
			if (entitySceneNode) {
				mPolygon = new ::EmberOgre::Manipulation::Polygon(entitySceneNode, mPositionProvider);

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
	mPolygon = 0;
	Ogre::SceneNode* entitySceneNode = getEntitySceneNode();
	if (entitySceneNode) {
		mPolygon = new ::EmberOgre::Manipulation::Polygon(entitySceneNode, mPositionProvider);
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
