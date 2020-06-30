//
// C++ Implementation: PolygonAdapter
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

#include "PolygonAdapter.h"

#include "components/ogre/EmberOgre.h"
#include "components/ogre/World.h"
#include "components/ogre/Scene.h"
#include "components/ogre/Convert.h"
#include "components/ogre/NodeAttachment.h"
#include "components/ogre/authoring/PolygonPointPickListener.h"
#include "components/ogre/camera/MainCamera.h"

#include <wfmath/atlasconv.h>

#include <OgreSceneNode.h>
#include <OgreSceneManager.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Adapters {

namespace Atlas {

EntityPolygonPositionProvider::EntityPolygonPositionProvider(EmberEntity& entity) :
		mEntity(entity) {
}

float EntityPolygonPositionProvider::getHeightForPosition(const WFMath::Point<2>& localPosition) const {
	return mEntity.getHeight(localPosition);
}

PolygonAdapter::PolygonAdapter(const ::Atlas::Message::Element& element, CEGUI::PushButton* showButton, EmberEntity* entity) :
		AdapterBase(element),
		mShowButton(showButton),
		mEntity(entity),
		mEntityNode(nullptr) {

	if (entity) {
		mPositionProvider = std::make_unique<EntityPolygonPositionProvider>(*entity);
	}

	if (showButton) {
		addGuiEventConnection(showButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&PolygonAdapter::showButton_Clicked, this)));
	}

	updateGui(mOriginalValue);
}

PolygonAdapter::~PolygonAdapter() {
	if (mPickListener) {
		EmberOgre::getSingleton().getWorld()->getMainCamera().removeWorldPickListener(mPickListener.get());
	}
	mPolygon.reset();
	if (mEntityNode) {
		EmberOgre::getSingleton().getWorld()->getSceneManager().destroySceneNode(mEntityNode);
	}
}

void PolygonAdapter::updateGui(const ::Atlas::Message::Element& element) {
}

bool PolygonAdapter::showButton_Clicked(const CEGUI::EventArgs& e) {
	toggleDisplayOfPolygon();
	return true;
}

Ogre::SceneNode* PolygonAdapter::getEntitySceneNode() {
	if (!mEntityNode) {
		mEntityNode = EmberOgre::getSingleton().getWorld()->getSceneManager().getRootSceneNode()->createChildSceneNode();
		mEntity->Moved.connect(sigc::mem_fun(*this, &PolygonAdapter::entityMoved));
		entityMoved();
	}
	return mEntityNode;
}

void PolygonAdapter::toggleDisplayOfPolygon() {
	if (!mPolygon) {
		if (!mEntity) {
			S_LOG_WARNING("There's no entity attached to the PolygonAdapter, and the polygon can't thus be shown.");
		} else {
			//It's important that we do the call to getChangedElement before we create and set mPolygon, since if that's set, the values from there will be used instead of the original atlas values.
			::Atlas::Message::Element areaElem(getChangedElement());

			Ogre::SceneNode* entitySceneNode = getEntitySceneNode();
			if (areaElem.isMap()) {
				try {
					WFMath::Polygon<2> poly(areaElem);
					createNewPolygon(&poly);
				} catch (const WFMath::_AtlasBadParse& ex) {
					createNewPolygon(nullptr);
				}
			} else {
				createNewPolygon(nullptr);
			}


		}
	} else {
		if (mPickListener) {
			EmberOgre::getSingleton().getWorld()->getMainCamera().removeWorldPickListener(mPickListener.get());
			try {
				mPickListener.reset();
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when deleting polygon point pick listener.");
			}
		}
		try {
			mPolygon.reset();
		} catch (const std::exception& ex) {
			S_LOG_FAILURE("Error when deleting polygon.");
		}
	}
}

void PolygonAdapter::createNewPolygon(WFMath::Polygon<2>* existingPoly) {
	mPolygon.reset();
	Ogre::SceneNode* entitySceneNode = getEntitySceneNode();
	mPolygon = std::make_unique<Authoring::Polygon>(entitySceneNode, mPositionProvider.get(), true);
	//These polygons should all be interactive.
	mPolygon->makeInteractive(&EmberOgre::getSingleton().getWorld()->getScene().getBulletWorld());
	WFMath::Polygon<2> poly;
	if (existingPoly) {
		poly = *existingPoly;
	} else {
		poly.addCorner(0, WFMath::Point<2>(-1, -1));
		poly.addCorner(1, WFMath::Point<2>(-1, 1));
		poly.addCorner(2, WFMath::Point<2>(1, 1));
		poly.addCorner(3, WFMath::Point<2>(1, -1));
	}

	mPolygon->loadFromShape(poly);
	if (!mPickListener) {
		mPickListener = std::make_unique<Authoring::PolygonPointPickListener>(*mPolygon);
		mPickListener->EventPickedPoint.connect(sigc::mem_fun(*this, &PolygonAdapter::pickListener_PickedPoint));
		EmberOgre::getSingleton().getWorld()->getMainCamera().pushWorldPickListener(mPickListener.get());
	}


}

void PolygonAdapter::fillElementFromGui() {
	if (mPolygon) {
		mEditedValue = mPolygon->getShape().toAtlas();
	}
}

bool PolygonAdapter::_hasChanges() {
	return mOriginalValue != getChangedElement();
}

void PolygonAdapter::pickListener_PickedPoint(Authoring::PolygonPoint& point) {
	mPointMovement = std::make_unique<Authoring::PolygonPointMovement>(*mPolygon, point, this, EmberOgre::getSingleton().getWorld()->getMainCamera());
}

void PolygonAdapter::endMovement() {
	mPointMovement.reset();
}

void PolygonAdapter::cancelMovement() {
	mPointMovement.reset();
}

bool PolygonAdapter::hasShape() const {
	return mPolygon != nullptr;
}

WFMath::Polygon<2> PolygonAdapter::getShape() {
	if (mPolygon) {
		return mPolygon->getShape();
	}
	auto element = getChangedElement();
	if (element.isMap() || element.isList()) {
		return WFMath::Polygon<2>(element);
	}
	return WFMath::Polygon<2>();
}

void PolygonAdapter::entityMoved() {
	mEntityNode->setPosition(Convert::toOgre(mEntity->getPosition()));
	mEntityNode->setOrientation(Convert::toOgre(mEntity->getOrientation()));
}

}

}

}

}
}
