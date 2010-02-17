/*
 Copyright (C) 2009 erik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "TerrainEditorOverlay.h"

#include "TerrainEditor.h"
#include "TerrainManager.h"
#include "TerrainDefPoint.h"
#include "ISceneManagerAdapter.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/Convert.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/EmberEntity.h"
#include "components/ogre/WorldEmberEntity.h"
#include "components/ogre/EmberEntityFactory.h"
#include "components/ogre/MousePicker.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "services/input/Input.h"

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>

#include <Eris/Connection.h>
#include <Mercator/Terrain.h>
#include <Mercator/BasePoint.h>

#include <sstream>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

namespace EmberOgre
{

namespace Terrain
{
const std::string BasePointUserObject::s_TypeName("BasePointMarker");

BasePointUserObject::BasePointUserObject(const TerrainPosition terrainPosition, const Mercator::BasePoint& basePoint, Ogre::SceneNode* basePointMarkerNode) :
	mBasePoint(basePoint), mBasePointMarkerNode(basePointMarkerNode), mPosition(terrainPosition)
{
}

const Ogre::String & BasePointUserObject::getTypeName() const
{
	return s_TypeName;
}

const Mercator::BasePoint& BasePointUserObject::getBasePoint() const
{
	return mBasePoint;
}

Ogre::SceneNode* BasePointUserObject::getBasePointMarkerNode() const
{
	return mBasePointMarkerNode;
}

const TerrainPosition& BasePointUserObject::getPosition() const
{
	return mPosition;
}

float BasePointUserObject::getHeight() const
{
	return getBasePointMarkerNode()->getPosition().y;
}

void BasePointUserObject::translate(Ogre::Real verticalMovement)
{
	getBasePointMarkerNode()->translate(Ogre::Vector3(0, verticalMovement, 0));
	markAsMoved();
	EventUpdatedPosition();
}

void BasePointUserObject::setHeight(Ogre::Real height)
{
	const Ogre::Vector3& position = getBasePointMarkerNode()->getPosition();
	getBasePointMarkerNode()->setPosition(position.x, height, position.z);
	markAsMoved();
	EventUpdatedPosition();
}

void BasePointUserObject::markAsMoved()
{
	Ogre::Entity* entity = static_cast<Ogre::Entity*> (getBasePointMarkerNode()->getAttachedObject(0));
	entity->setMaterialName("/global/authoring/point/moved");
}

void BasePointUserObject::resetMarking()
{
	Ogre::Entity* entity = static_cast<Ogre::Entity*> (getBasePointMarkerNode()->getAttachedObject(0));
	entity->setMaterialName("/global/authoring/point");
}

BasePointPickListener::BasePointPickListener(TerrainEditorOverlay& overlay) :
	mOverlay(overlay), mPickedUserObject(0)
{

}

void BasePointPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{
	if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && pickedMovable->getUserObject()->getTypeName() == BasePointUserObject::s_TypeName) {
			mPickedUserObject = static_cast<BasePointUserObject*> (pickedMovable->getUserObject());
			continuePicking = false;
		}
	}
}

void BasePointPickListener::initializePickingContext()
{
	mPickedUserObject = 0;
}

void BasePointPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (mPickedUserObject) {
		mOverlay.pickedBasePoint(mPickedUserObject);
	}
}

TerrainEditBasePointMovement::TerrainEditBasePointMovement(Ogre::Real verticalMovement, TerrainPosition position) :
	mVerticalMovement(verticalMovement), mPosition(position)
{
}

Ogre::Real TerrainEditBasePointMovement::getVerticalMovement() const
{
	return mVerticalMovement;
}

const TerrainPosition& TerrainEditBasePointMovement::getPosition() const
{
	return mPosition;

}
TerrainEditorOverlay::TerrainEditorOverlay(TerrainEditor& editor, Ogre::SceneManager& sceneManager, Ogre::SceneNode& worldSceneNode, TerrainManager& manager, Camera::MainCamera& camera, std::map<int, std::map<int, Mercator::BasePoint> >& basePoints) :
	mEditor(editor), mSceneManager(sceneManager), mManager(manager), mCamera(camera), mBasePoints(basePoints), mPickListener(*this), mCurrentUserObject(0), mOverlayNode(0)
{
	createOverlay(worldSceneNode);
}

TerrainEditorOverlay::~TerrainEditorOverlay()
{
	//TODO: also delete user objects
	if (mOverlayNode) {
		Ogre::SceneNode* parent = static_cast<Ogre::SceneNode*> (mOverlayNode->getParent());
		if (parent) {
			parent->removeAndDestroyChild(mOverlayNode->getName());
		} else {
			mOverlayNode->getCreator()->destroySceneNode(mOverlayNode->getName());
		}
	}
	///It's safe to do this even if the pick listener hasn't been added yet.
	mCamera.removeWorldPickListener(&mPickListener);
}

void TerrainEditorOverlay::createOverlay(Ogre::SceneNode& worldSceneNode)
{

	mOverlayNode = worldSceneNode.createChildSceneNode();

	int x, y;
	for (Mercator::Terrain::Pointstore::const_iterator I = mBasePoints.begin(); I != mBasePoints.end(); ++I) {
		x = I->first;
		for (Mercator::Terrain::Pointcolumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
			y = J->first;
			std::stringstream ss;
			ss << "basepointmarker" << x << "_" << y;
			Ogre::Entity* entity(0);
			try {
				entity = mSceneManager.createEntity(ss.str(), "3d_objects/primitives/models/sphere.mesh");
				///start out with a normal material
				entity->setMaterialName("/global/authoring/point");
				entity->setRenderingDistance(300);
				entity->setQueryFlags(MousePicker::CM_UNDEFINED);
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when creating base point marker entity." << ex);
				continue;
			}

			if (!entity) {
				S_LOG_FAILURE("Unexpected error when creating base point marker entity.");
				continue;
			}

			const Mercator::BasePoint& basepoint(J->second);
			Ogre::SceneNode* basepointNode = mOverlayNode->createChildSceneNode();
			TerrainPosition tPos(x * 64, y * 64);
			Ogre::Vector3 ogrePos = Convert::toOgre<Ogre::Vector3>(tPos);
			ogrePos.y = basepoint.height();
			basepointNode->setPosition(ogrePos);
			basepointNode->attachObject(entity);

			BasePointUserObject* userObject = new BasePointUserObject(TerrainPosition(x, y), basepoint, basepointNode);
			entity->setUserObject(userObject);

			///store the base point user object
			std::stringstream ss_;
			ss_ << x << "_" << y;
			mBasePointUserObjects[ss_.str()] = userObject;
		}
	}

	///register the pick listener
	mCamera.pushWorldPickListener(&mPickListener);

}

BasePointUserObject* TerrainEditorOverlay::getUserObject(const TerrainPosition& terrainIndex)
{
	std::stringstream ss;
	ss << terrainIndex.x() << "_" << terrainIndex.y();
	BasePointUserObjectStore::iterator I = mBasePointUserObjects.find(ss.str());
	if (I != mBasePointUserObjects.end()) {
		return I->second;
	}
	return 0;

}

BasePointUserObject* TerrainEditorOverlay::getCurrentBasePointUserObject() const
{
	return mCurrentUserObject;
}

void TerrainEditorOverlay::pickedBasePoint(BasePointUserObject* userObject)
{
	assert(userObject);
	mCurrentUserObject = userObject;
	catchInput();
	EventPickedBasePoint.emit(userObject);
}

bool TerrainEditorOverlay::injectMouseMove(const Ember::MouseMotion& motion, bool& freezeMouse)
{
	float multiplier(15.0f);
	///hard coded to allow the shift button to increase the speed
	if (Ember::Input::getSingleton().isKeyDown(SDLK_RSHIFT) || Ember::Input::getSingleton().isKeyDown(SDLK_LSHIFT)) {
		multiplier *= 5;
	}
	assert(mCurrentUserObject);
	float translation(motion.yRelativeMovement * multiplier);
	mCurrentUserObject->translate(translation);

	///should we also translate secondary objects?
	if (mEditor.getRadius() > 1.0f) {
		// 		float squaredMovementRadius = mMovementRadiusInMeters * mMovementRadiusInMeters;
		for (BasePointUserObjectStore::iterator I = mBasePointUserObjects.begin(); I != mBasePointUserObjects.end(); ++I) {
			if (I->second != mCurrentUserObject) {
				float distance = WFMath::SquaredDistance<2>((I->second)->getPosition(), mCurrentUserObject->getPosition()) * 64;
				if (distance <= mEditor.getRadius()) {
					float movement = 1.0f - (distance / mEditor.getRadius());
					I->second->translate(translation * movement);
					mSecondaryUserObjects.insert(I->second);
				}
			}
		}
	}

	EventSelectedBasePointUpdatedPosition.emit(mCurrentUserObject);

	///we don't want to move the cursor
	freezeMouse = true;
	return false;
}

bool TerrainEditorOverlay::injectMouseButtonUp(const Ember::Input::MouseButton& button)
{
	if (button == Ember::Input::MouseButtonLeft) {
		releaseInput();
	}
	return true;
}

bool TerrainEditorOverlay::injectMouseButtonDown(const Ember::Input::MouseButton& button)
{
	return true;
}

bool TerrainEditorOverlay::injectChar(char character)
{
	return true;
}

bool TerrainEditorOverlay::injectKeyDown(const SDLKey& key)
{
	return true;
}

bool TerrainEditorOverlay::injectKeyUp(const SDLKey& key)
{
	return true;
}

void TerrainEditorOverlay::catchInput()
{
	Ember::Input::getSingleton().addAdapter(this);
}

void TerrainEditorOverlay::releaseInput()
{
	Ember::Input::getSingleton().removeAdapter(this);

	///react on the movement
	createAction(true);

}

void TerrainEditorOverlay::createAction(bool alsoCommit)
{
	if (mCurrentUserObject) {
		///lets get how much it moved
		float distance = mCurrentUserObject->getBasePointMarkerNode()->getPosition().y - mCurrentUserObject->getBasePoint().height();
		///only register an action if it has been moved
		if (distance != 0) {
			TerrainEditBasePointMovement movement(distance, mCurrentUserObject->getPosition());
			TerrainEditAction action;
			action.getMovements().push_back(movement);

			for (BasePointUserObjectSet::iterator I = mSecondaryUserObjects.begin(); I != mSecondaryUserObjects.end(); ++I) {
				distance = (*I)->getBasePointMarkerNode()->getPosition().y - (*I)->getBasePoint().height();
				if (distance != 0) {
					TerrainEditBasePointMovement movement(distance, (*I)->getPosition());
					action.getMovements().push_back(movement);
				}
			}

			mActions.push_back(action);

			///when a new action is created the undo list must be emptied
			mUndoneActions.clear();

			EventActionCreated(&action);

			if (alsoCommit) {
				commitAction(action);
			}
		}
	}
	mSecondaryUserObjects.clear();
}

void TerrainEditorOverlay::sendChangesToServer()
{

	try {
		std::map<std::string, TerrainPosition> positions;
		for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
			for (TerrainEditAction::MovementStore::const_iterator J = I->getMovements().begin(); J != I->getMovements().end(); ++J) {
				std::stringstream key;
				key << J->getPosition().x() << "x" << J->getPosition().y();
				positions[key.str()] = J->getPosition();
			}
		}

		Atlas::Objects::Operation::Set s;

		Atlas::Message::MapType sarg;
		sarg["id"] = "0";

		Atlas::Message::MapType & terrain = (sarg["terrain"] = Atlas::Message::MapType()).asMap();

		Atlas::Message::MapType & pointMap = (terrain["points"] = Atlas::Message::MapType()).asMap();

		for (std::map<std::string, TerrainPosition>::iterator I = positions.begin(); I != positions.end(); ++I) {

			Mercator::BasePoint bp;
			WFMath::CoordType basepointX = I->second.x();
			WFMath::CoordType basepointY = I->second.y();
			getBasePoint(static_cast<int> (basepointX), static_cast<int> (basepointY), bp);

			Atlas::Message::ListType & point = (pointMap[I->first] = Atlas::Message::ListType(3)).asList();
			point[0] = (Atlas::Message::FloatType)(I->second.x());
			point[1] = (Atlas::Message::FloatType)(I->second.y());
			point[2] = (Atlas::Message::FloatType)(bp.height());

		}

		Atlas::Message::ListType sargsList(1, sarg);
		s->setArgsAsList(sargsList);
		s->setFrom(EmberOgre::getSingleton().getAvatar()->getEmberEntity().getId());

		Ember::EmberServices::getSingleton().getServerService()->getConnection()->send(s);
		S_LOG_INFO("Sent updated terrain to server (" << positions.size() << " base points updated).");

		///also reset the marking for the base points
		for (std::map<std::string, TerrainPosition>::iterator I = positions.begin(); I != positions.end(); ++I) {
			BasePointUserObject* userObject = getUserObject(I->second);
			if (userObject) {
				userObject->resetMarking();
			}
		}
		///clear all actions
		mActions.clear();
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not send terrain to server." << ex);
	}

}

bool TerrainEditorOverlay::undoLastAction()
{
	if (mActions.size() > 0) {
		TerrainEditAction action = mActions.back();
		///remove the last action from the list of active actions
		mActions.pop_back();
		///add the action to the list of undone actions
		mUndoneActions.push_front(action);
		///actually undo the action
		commitAction(action, true);
		return true;
	}
	return false;

}

bool TerrainEditorOverlay::redoAction()
{
	if (mUndoneActions.size()) {
		TerrainEditAction action = mUndoneActions.front();
		mUndoneActions.pop_front();
		mActions.push_back(action);
		commitAction(action);
	}
	return false;
}

void TerrainEditorOverlay::commitAction(const TerrainEditAction& action, bool reverse)
{
	TerrainDefPointStore pointStore;

	std::set<TerrainPage*> pagesToUpdate;
	for (TerrainEditAction::MovementStore::const_iterator I = action.getMovements().begin(); I != action.getMovements().end(); ++I) {
		Mercator::BasePoint bp;
		int basepointX = static_cast<int> (I->getPosition().x());
		int basepointY = static_cast<int> (I->getPosition().y());
		getBasePoint(basepointX, basepointY, bp);
		///check if we should do a reverse action (which is done when an action is undone)
		bp.height() = bp.height() + (reverse ? -I->getVerticalMovement() : I->getVerticalMovement());
		//EmberOgre::getSingleton().getTerrainManager()->getTerrain().setBasePoint(basepointX, basepointY, bp);

		TerrainDefPoint defPoint(basepointX, basepointY, bp.height());
		pointStore.push_back(defPoint);

		// 		Ogre::Vector3 markerPos = Convert::toOgre(I->getPosition());
		//
		// 		markerPos *= 64;
		// 		Ogre::PagingLandScapeTile* tile;
		// 		for (int i = -1; i < 2; i += 2) {
		// 			for (int j = -1; j < 2; j += 2) {
		// 				tile = sceneMgr->getPageManager()->getTile(markerPos.x + i, markerPos.z + j, false);
		// 				if (tile) {
		// 					tilesToUpdate.insert(tile);
		// 				}
		// 			}
		// 		}

		TerrainPosition worldPosition(I->getPosition().x() * 64, I->getPosition().y() * 64);
		TerrainPage* page;
		///make sure we sample pages from all four points around the base point, in case the base point is on a page border
		for (int i = -65; i < 66; i += 64) {
			for (int j = -65; j < 66; j += 64) {
				TerrainPosition position(worldPosition.x() + i, worldPosition.y() + j);
				page = mManager.getTerrainPageAtPosition(position);
				if (page) {
					pagesToUpdate.insert(page);
				}
			}
		}

		///make sure the marker node is updated
		BasePointUserObject* userObject = getUserObject(I->getPosition());
		if (userObject) {
			userObject->setHeight(bp.height());
		}

	}

	mManager.updateTerrain(pointStore);

	///reload all shader textures of the affected pages
	// 	for (std::set<TerrainPage*>::iterator I = pagesToUpdate.begin(); I != pagesToUpdate.end(); ++I) {
	// 		(*I)->update();
	// 		(*I)->updateAllShaderTextures();
	// 	}


	// 	std::set<Ogre::PagingLandScapeData2D*> dataStore;
	// 	///reload all affected tiles
	// 	for (std::set<Ogre::PagingLandScapeTile*>::iterator I = tilesToUpdate.begin(); I != tilesToUpdate.end(); ++I) {
	// //		(*I)->updateTerrain();
	//
	// 		Ogre::PagingLandScapeData2D *data = sceneMgr->getData2DManager()->getData2D((*I)->getInfo()->pageX, (*I)->getInfo()->pageZ);
	// //		dataStore.insert(data);
	// 		uint x, z;
	// 		data->getCoordinates(x, z);
	// 		sceneMgr->getData2DManager()->reload(x,z);
	//
	// 		(*I)->unload();
	// 		(*I)->load();
	//
	// 	}
	//
	// 	///also update the data
	// 	for (std::set<Ogre::PagingLandScapeData2D*>::iterator I = dataStore.begin(); I != dataStore.end(); ++I) {
	// 		uint x, z;
	// 		(*I)->getCoordinates(x, z);
	// 		sceneMgr->getData2DManager()->reload(x,z);
	// 	}
	// 	Ogre::Vector2 targetPage (X, Z);
	// 	sceneMgr->setOption("PageUpdate", &targetPage);

	///TODO: this shouldn't be necessary
	//sceneMgr->getPageManager()->load();
	// 	TerrainManager->getAdapter()->reloadAllPages();


	// 	updateEntityPositions(pagesToUpdate);


}

bool TerrainEditorOverlay::getBasePoint(int x, int y, Mercator::BasePoint& z) const
{
    Mercator::Terrain::Pointstore::const_iterator I = mBasePoints.find(x);
    if (I == mBasePoints.end()) {
        return false;
    }
    Mercator::Terrain::Pointcolumn::const_iterator J = I->second.find(y);
    if (J == I->second.end()) {
        return false;
    }
    z = J->second;
    return true;
}

}

}
