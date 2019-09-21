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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "TerrainEditorOverlay.h"

#include "TerrainEditor.h"
#include "TerrainManager.h"
#include "TerrainHandler.h"
#include "TerrainDefPoint.h"
#include "ITerrainAdapter.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/Convert.h"
#include "components/ogre/Avatar.h"
#include "components/ogre/camera/MainCamera.h"
#include "components/ogre/GUIManager.h"
#include "domain/EmberEntity.h"
#include "components/ogre/MousePicker.h"
#include "components/ogre/World.h"
#include "components/ogre/Scene.h"

#include "services/EmberServices.h"
#include "services/server/ServerService.h"

#include <Eris/Connection.h>
#include <Eris/Avatar.h>
#include <Mercator/Terrain.h>

#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreEntity.h>

#include <sigc++/bind.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>

namespace Ember
{
namespace OgreView
{

namespace Terrain
{

BasePointUserObject::BasePointUserObject(TerrainPosition terrainPosition, const Mercator::BasePoint& basePoint,
										 Ogre::SceneNode* basePointMarkerNode, BulletWorld& bulletWorld) :
		mBasePoint(basePoint),
		mBasePointMarkerNode(basePointMarkerNode),
		mPosition(terrainPosition),
		mCanonicalHeight(mBasePoint.height()),
		mIsMoving(false),
		mRoughness(basePoint.roughness()),
		mFalloff(basePoint.falloff()),
		mCollisionDetector(new BulletCollisionDetector(bulletWorld))
{
	mCollisionDetector->collisionInfo = this;
	auto sphereShape = std::make_shared<btSphereShape>(basePointMarkerNode->getAttachedObject(0)->getBoundingRadius());
	mCollisionDetector->addCollisionShape(std::move(sphereShape));
	mCollisionDetector->updateTransforms(Convert::toWF<WFMath::Point<3>>(getBasePointMarkerNode()->_getDerivedPosition()), WFMath::Quaternion::IDENTITY());
}

const Mercator::BasePoint& BasePointUserObject::getBasePoint() const
{
	return mBasePoint;
}

void BasePointUserObject::setBasePoint(const Mercator::BasePoint& basePoint)
{
	mBasePoint = basePoint;
	setHeight(mBasePoint.height());
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
	mCollisionDetector->updateTransforms(Convert::toWF<WFMath::Point<3>>(getBasePointMarkerNode()->_getDerivedPosition()), WFMath::Quaternion::IDENTITY());
	updateMarking();
	EventUpdatedPosition();
}

void BasePointUserObject::setHeight(Ogre::Real height)
{
	const Ogre::Vector3& position = getBasePointMarkerNode()->getPosition();
	getBasePointMarkerNode()->setPosition(position.x, height, position.z);
	mCollisionDetector->updateTransforms(Convert::toWF<WFMath::Point<3>>(getBasePointMarkerNode()->_getDerivedPosition()), WFMath::Quaternion::IDENTITY());
	updateMarking();
	EventUpdatedPosition();
}

void BasePointUserObject::setRoughness(float roughness)
{
	mRoughness = roughness;
}

float BasePointUserObject::getRoughness() const
{
	return mRoughness;
}

void BasePointUserObject::setFalloff(float falloff)
{
	mFalloff = falloff;
}

float BasePointUserObject::getFalloff() const
{
	return mFalloff;
}

void BasePointUserObject::updateMarking()
{
	auto entity = dynamic_cast<Ogre::Entity*>(getBasePointMarkerNode()->getAttachedObject(0));
	try {
		if (mIsMoving) {
			entity->setMaterialName("/common/base/authoring/point/moving");
		} else {
			if (!WFMath::Equal(mCanonicalHeight, getBasePointMarkerNode()->_getDerivedPosition().y)) {
				entity->setMaterialName("/common/base/authoring/point/moved");
			} else {
				entity->setMaterialName("/common/base/authoring/point");
			}
		}
	} catch (const std::exception& ex) {
		S_LOG_WARNING("Could not set new marker material. This is not fatal."<<ex);
	}
}

void BasePointUserObject::resetMarking()
{
	mCanonicalHeight = mBasePoint.height();
	updateMarking();
}

void BasePointUserObject::markAsMoving(bool isMoving)
{
	mIsMoving = isMoving;
	updateMarking();
}

BasePointPickListener::BasePointPickListener(TerrainEditorOverlay& overlay) :
		mOverlay(overlay),
		mPickedUserObject(nullptr)
{

}

void BasePointPickListener::processPickResult(bool& continuePicking, PickResult& result, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{
	if (result.collisionInfo.type() == typeid(BasePointUserObject*)) {
		continuePicking = false;
		if (mousePickerArgs.pickType == MPT_PRESS) {
			//TODO: make sure that it's a point which belongs to our polygon
			mPickedUserObject = boost::any_cast<BasePointUserObject*>(result.collisionInfo);
		}
	}
}

void BasePointPickListener::processDelayedPick(const MousePickerArgs& mousePickerArgs)
{
	//Don't process any delayed picks.
}

void BasePointPickListener::initializePickingContext(bool& willParticipate, const MousePickerArgs& pickArgs)
{
	//We will only react on press events, but we want do silence click and pressed events if they happen with our markers too.
	if (pickArgs.pickType == MPT_PRESS || pickArgs.pickType == MPT_CLICK || pickArgs.pickType == MPT_PRESSED) {
		willParticipate = true;
		mPickedUserObject = nullptr;
	}
}

void BasePointPickListener::endPickingContext(const MousePickerArgs& mousePickerArgs)
{
	if (mPickedUserObject) {
		mOverlay.pickedBasePoint(mPickedUserObject);
	}
}

TerrainEditorOverlay::TerrainEditorOverlay(TerrainEditor& editor, Ogre::SceneManager& sceneManager,
										   Ogre::SceneNode& worldSceneNode, TerrainManager& manager,
										   Camera::MainCamera& camera,
										   std::map<int, std::map<int, Mercator::BasePoint>>& basePoints) :
		mEditor(editor),
		mSceneManager(sceneManager),
		mWorldSceneNode(worldSceneNode),
		mManager(manager),
		mCamera(camera),
		mOverlayNode(nullptr),
		mPickListener(*this),
		mCurrentUserObject(nullptr)
{
	createOverlay(basePoints, worldSceneNode);
}

TerrainEditorOverlay::~TerrainEditorOverlay()
{
	for (auto entity : mEntities) {
		entity->detachFromParent();
		mSceneManager.destroyEntity(entity);
	}
	//TODO: also delete user objects
	if (mOverlayNode) {
		auto* parent = dynamic_cast<Ogre::SceneNode*>(mOverlayNode->getParent());
		if (parent) {
			parent->removeAndDestroyChild(mOverlayNode);
		} else {
			mOverlayNode->getCreator()->destroySceneNode(mOverlayNode);
		}
	}
	//It's safe to do this even if the pick listener hasn't been added yet.
	mCamera.removeWorldPickListener(&mPickListener);
}

void TerrainEditorOverlay::createOverlay(std::map<int, std::map<int, Mercator::BasePoint>>& basePoints, Ogre::SceneNode& worldSceneNode)
{

	mOverlayNode = worldSceneNode.createChildSceneNode();

	int x, y;
	for (Mercator::Terrain::Pointstore::const_iterator I = basePoints.begin(); I != basePoints.end(); ++I) {
		x = I->first;
		for (auto entry : I->second) {
			y = entry.first;
			std::stringstream ss;
			ss << "basepointmarker" << x << "_" << y;
			Ogre::Entity* entity(nullptr);
			try {
				entity = mSceneManager.createEntity(ss.str(), "common/primitives/model/sphere.mesh");
				//start out with a normal material
				entity->setMaterialName("/common/base/authoring/point");
				//The material is made to ignore depth checks, so if we put it in a later queue we're
				//making sure that the marker is drawn on top of everything else, making it easier to interact with.
				entity->setRenderQueueGroup(Ogre::RENDER_QUEUE_9);
				entity->setRenderingDistance(300);
				entity->setQueryFlags(MousePicker::CM_UNDEFINED);
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when creating base point marker entity." << ex);
				continue;
			}

			mEntities.push_back(entity);

			const Mercator::BasePoint& basepoint = entry.second;
			Ogre::SceneNode* basepointNode = mOverlayNode->createChildSceneNode();
			TerrainPosition tPos(x * 64, y * 64);
			Ogre::Vector3 ogrePos = Convert::toOgre<Ogre::Vector3>(tPos);
			ogrePos.y = basepoint.height();
			basepointNode->setPosition(ogrePos);
			basepointNode->attachObject(entity);

			auto* userObject = new BasePointUserObject(TerrainPosition(x, y), basepoint, basepointNode, mManager.getScene().getBulletWorld());

			//store the base point user object
			std::stringstream ss_;
			ss_ << x << "_" << y;
			mBasePointUserObjects[ss_.str()] = userObject;
		}
	}

	//register the pick listener
	mCamera.pushWorldPickListener(&mPickListener);

}

BasePointUserObject* TerrainEditorOverlay::getUserObject(const TerrainPosition& terrainIndex)
{
	std::stringstream ss;
	ss << terrainIndex.x() << "_" << terrainIndex.y();
	auto I = mBasePointUserObjects.find(ss.str());
	if (I != mBasePointUserObjects.end()) {
		return I->second;
	}
	return nullptr;

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
	mCurrentUserObject->markAsMoving(true);
	EventPickedBasePoint.emit(userObject);
}

void TerrainEditorOverlay::setRoughness(float roughness)
{
	if (mCurrentUserObject) {
		mCurrentUserObject->setRoughness(roughness);
		createAction(true);
	}
}

void TerrainEditorOverlay::setFalloff(float falloff)
{
	if (mCurrentUserObject) {
		mCurrentUserObject->setFalloff(falloff);
		createAction(true);
	}
}

bool TerrainEditorOverlay::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	float multiplier(15.0f);
	//hard coded to allow the shift button to increase the speed
	if (Input::getSingleton().isKeyDown(SDL_SCANCODE_RSHIFT) || Input::getSingleton().isKeyDown(SDL_SCANCODE_LSHIFT)) {
		multiplier *= 5;
	}

	assert(mCurrentUserObject);
	float translation(motion.yRelativeMovement * multiplier);
	mCurrentUserObject->translate(translation);

	//should we also translate secondary objects?
	if (mEditor.getRadius() > 1.0f) {
		// 		float squaredMovementRadius = mMovementRadiusInMeters * mMovementRadiusInMeters;
		for (auto& basePointUserObject : mBasePointUserObjects) {
			if (basePointUserObject.second != mCurrentUserObject) {
				float distance = WFMath::SquaredDistance<2>((basePointUserObject.second)->getPosition(), mCurrentUserObject->getPosition()) * 64;
				if (distance <= mEditor.getRadius()) {
					float movement = 1.0f - (distance / mEditor.getRadius());
					basePointUserObject.second->translate(translation * movement);
					mSecondaryUserObjects.insert(basePointUserObject.second);
				}
			}
		}
	}

	EventSelectedBasePointUpdatedPosition.emit(mCurrentUserObject);

	//we don't want to move the cursor
	freezeMouse = true;
	return false;
}

bool TerrainEditorOverlay::injectMouseButtonUp(Input::MouseButton button)
{
	if (button == Input::MouseButtonLeft) {
		releaseInput();
	}
	return true;
}

bool TerrainEditorOverlay::injectMouseButtonDown(Input::MouseButton button)
{
	return true;
}

bool TerrainEditorOverlay::injectChar(int)
{
	return true;
}

bool TerrainEditorOverlay::injectKeyDown(const SDL_Scancode&)
{
	return true;
}

bool TerrainEditorOverlay::injectKeyUp(const SDL_Scancode&)
{
	return true;
}

void TerrainEditorOverlay::catchInput()
{
	Input::getSingleton().addAdapter(this);
}

void TerrainEditorOverlay::releaseInput()
{
	Input::getSingleton().removeAdapter(this);

	//react on the movement
	createAction(true);

}

void TerrainEditorOverlay::createAction(bool alsoCommit)
{
	if (mCurrentUserObject) {
		mCurrentUserObject->markAsMoving(false);
		//lets get how much it moved
		float distance = mCurrentUserObject->getBasePointMarkerNode()->getPosition().y - mCurrentUserObject->getBasePoint().height();
		//only register an action if it has been moved
		TerrainEditAction action{};
		bool hadChanges = false;
		if (!WFMath::Equal(distance, .0f)) {
			TerrainEditBasePointMovement movement = { distance, std::make_pair((int)mCurrentUserObject->getPosition().x(), (int)mCurrentUserObject->getPosition().y()) };
			action.mMovements.push_back(movement);

			for (auto secondaryUserObject : mSecondaryUserObjects) {
				distance = secondaryUserObject->getBasePointMarkerNode()->getPosition().y - secondaryUserObject->getBasePoint().height();
				if (!WFMath::Equal(distance, .0f)) {
					TerrainEditBasePointMovement newMovement = { distance, std::make_pair((int) secondaryUserObject->getPosition().x(), (int) secondaryUserObject->getPosition().y()) };
					action.mMovements.push_back(newMovement);
				}
			}
			hadChanges = true;
		}

		if (mCurrentUserObject->getRoughness() != mCurrentUserObject->getBasePoint().roughness()) {
			action.mRoughnesses.emplace_back(std::make_pair((int)mCurrentUserObject->getPosition().x(), (int)mCurrentUserObject->getPosition().y()), mCurrentUserObject->getRoughness());
			hadChanges = true;
		}

		if (mCurrentUserObject->getFalloff() != mCurrentUserObject->getBasePoint().falloff()) {
			action.mFalloffs.emplace_back(std::make_pair((int)mCurrentUserObject->getPosition().x(), (int)mCurrentUserObject->getPosition().y()), mCurrentUserObject->getFalloff());
			hadChanges = true;
		}

		if (hadChanges) {
			mActions.push_back(action);

			//when a new action is created the undo list must be emptied
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
	sigc::slot<void, BasePointStore&> slot = sigc::mem_fun(*this, &TerrainEditorOverlay::sendChangesToServerWithBasePoints);
	mManager.getBasePoints(slot);
}

void TerrainEditorOverlay::sendChangesToServerWithBasePoints(std::map<int, std::map<int, Mercator::BasePoint>>& basePoints)
{

	try {
		std::set<TerrainIndex> updatedPositions;
		std::set<TerrainIndex> updatedRoughnesses;
		std::set<TerrainIndex> updatedFalloffs;

		std::map<std::pair<int, int>, TerrainPosition> positions;
		std::map<std::pair<int, int>, float> roughnesses;
		std::map<std::pair<int, int>, float> falloffs;
		for (const auto& action : mActions) {
			for (const auto& movement : action.mMovements) {
				updatedPositions.insert(movement.mPosition);
			}
			for (const auto& roughness : action.mRoughnesses) {
				updatedRoughnesses.insert(roughness.first);
			}
			for (const auto& falloff : action.mFalloffs) {
				updatedFalloffs.insert(falloff.first);
			}
		}

		Atlas::Objects::Operation::Set s;

		Atlas::Message::MapType sarg;
		EmberEntity* terrainEntity = mManager.getHandler().getTerrainHoldingEntity();
		if (terrainEntity) {
			sarg["id"] = terrainEntity->getId();
		} else {
			sarg["id"] = "0";
		}

		Atlas::Message::MapType & terrain = (sarg["terrain"] = Atlas::Message::MapType()).asMap();

		Atlas::Message::MapType & pointMap = (terrain["points"] = Atlas::Message::MapType()).asMap();

		auto createPointElementFn = [&](const Mercator::BasePoint& bp, int x, int y, const std::string& key) {
			Atlas::Message::ListType & point = (pointMap[key] = Atlas::Message::ListType(5)).asList();
			point[0] = (Atlas::Message::FloatType)(x);
			point[1] = (Atlas::Message::FloatType)(y);
			point[2] = (Atlas::Message::FloatType)(bp.height());
			point[3] = (Atlas::Message::FloatType)(bp.roughness());
			point[4] = (Atlas::Message::FloatType)(bp.falloff());
		};

		for (const auto& entry : updatedPositions) {

			Mercator::BasePoint bp;
			getBasePoint(basePoints, entry.first, entry.second, bp);
			std::stringstream key;
			key << entry.first << "x" << entry.second;

			createPointElementFn(bp, entry.first, entry.second, key.str());
		}

		for (const auto& entry : updatedRoughnesses) {
			Mercator::BasePoint bp;
			getBasePoint(basePoints, entry.first, entry.second, bp);

			std::stringstream key;
			key << entry.first << "x" << entry.second;
			auto I = pointMap.find(key.str());
			if (I != pointMap.end()) {
				I->second.List()[3] = entry.second;
			} else {
				createPointElementFn(bp, entry.first, entry.second, key.str());
			}
		}

		for (const auto& entry : updatedFalloffs) {
			Mercator::BasePoint bp;
			getBasePoint(basePoints, entry.first, entry.second, bp);

			std::stringstream key;
			key << entry.first << "x" << entry.second;
			auto I = pointMap.find(key.str());
			if (I != pointMap.end()) {
				I->second.List()[4] = entry.second;
			} else {
				createPointElementFn(bp, entry.first, entry.second, key.str());
			}
		}

		auto connection = EmberServices::getSingleton().getServerService().getConnection();
		Atlas::Message::ListType sargsList(1, sarg);
		s->setArgsAsList(sargsList, &connection->getFactories());
		s->setFrom(EmberOgre::getSingleton().getWorld()->getAvatar()->getErisAvatar()->getId());

		connection->send(s);
		S_LOG_INFO("Sent updated terrain to server (" << positions.size() << " base points updated).");

		//also reset the marking for the base points
		for (const auto& entry : mBasePointUserObjects) {
			entry.second->resetMarking();
		}
		//clear all actions
		mActions.clear();
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Could not send terrain to server." << ex);
	}

}

void TerrainEditorOverlay::setVisible(bool visible)
{
	if (mOverlayNode) {
		if (visible) {
			if (!mOverlayNode->getParentSceneNode()) {
				mWorldSceneNode.addChild(mOverlayNode);
			}
		} else {
			if (mOverlayNode->getParentSceneNode()) {
				mOverlayNode->getParentSceneNode()->removeChild(mOverlayNode);
			}
		}
	}
}

bool TerrainEditorOverlay::getVisible() const
{
	return mOverlayNode != nullptr && mOverlayNode->isInSceneGraph();
}

bool TerrainEditorOverlay::undoLastAction()
{
	if (!mActions.empty()) {
		TerrainEditAction action = mActions.back();
		//remove the last action from the list of active actions
		mActions.pop_back();
		//add the action to the list of undone actions
		mUndoneActions.push_front(action);
		//actually undo the action
		commitAction(action, true);
		return true;
	}
	return false;

}

bool TerrainEditorOverlay::redoAction()
{
	if (!mUndoneActions.empty()) {
		TerrainEditAction action = mUndoneActions.front();
		mUndoneActions.pop_front();
		mActions.push_back(action);
		commitAction(action);
	}
	return false;
}

void TerrainEditorOverlay::commitAction(const TerrainEditAction& action, bool reverse)
{
	sigc::slot<void, BasePointStore&> slot = sigc::bind(sigc::mem_fun(*this, &TerrainEditorOverlay::commitActionWithBasePoints), action, reverse);
	mManager.getBasePoints(slot);
}

void TerrainEditorOverlay::commitActionWithBasePoints(BasePointStore& basePoints, const TerrainEditAction& action, bool reverse)
{

	TerrainDefPointStore pointStore;

	std::set<TerrainPage*> pagesToUpdate;
	std::map<std::pair<int, int>, TerrainDefPoint> newPoints;

	for (const auto& movement : action.mMovements) {
		Mercator::BasePoint bp;
		int basepointX = movement.mPosition.first;
		int basepointY = movement.mPosition.second;
		getBasePoint(basePoints, basepointX, basepointY, bp);
		//check if we should do a reverse action (which is done when an action is undone)
		bp.height() = bp.height() + (reverse ? -movement.mVerticalMovement : movement.mVerticalMovement);

		TerrainDefPoint defPoint;
		defPoint.position = WFMath::Point<2>(basepointX, basepointY);
		defPoint.height = bp.height();
		defPoint.roughness = bp.roughness();
		defPoint.falloff = bp.falloff();

		newPoints[std::make_pair(basepointX, basepointY)] = defPoint;
	}

	for (const auto& roughness : action.mRoughnesses) {
		Mercator::BasePoint bp;
		int basepointX = std::get<0>(roughness).first;
		int basepointY = std::get<0>(roughness).second;
		getBasePoint(basePoints, basepointX, basepointY, bp);

		auto I = newPoints.find(std::make_pair(basepointX, basepointY));
		if (I != newPoints.end()) {
			I->second.roughness = std::get<1>(roughness);
		} else {
			TerrainDefPoint defPoint;
			defPoint.position = WFMath::Point<2>(basepointX, basepointY);
			defPoint.height = bp.height();
			defPoint.roughness = std::get<1>(roughness);
			defPoint.falloff = bp.falloff();
			newPoints[std::make_pair(basepointX, basepointY)] = defPoint;
		}
	}

	for (const auto& falloff : action.mFalloffs) {
		Mercator::BasePoint bp;
		int basepointX = std::get<0>(falloff).first;
		int basepointY = std::get<0>(falloff).second;
		getBasePoint(basePoints, basepointX, basepointY, bp);

		auto I = newPoints.find(std::make_pair(basepointX, basepointY));
		if (I != newPoints.end()) {
			I->second.falloff = std::get<1>(falloff);
		} else {
			TerrainDefPoint defPoint;
			defPoint.position = WFMath::Point<2>(basepointX, basepointY);
			defPoint.height = bp.height();
			defPoint.roughness = bp.roughness();
			defPoint.falloff = std::get<1>(falloff);
			newPoints[std::make_pair(basepointX, basepointY)] = defPoint;
		}
	}

	for (const auto& entry : newPoints) {
		pointStore.push_back(entry.second);

		TerrainPosition worldPosition(entry.first.first * 64, entry.first.second * 64);
		TerrainPage* page;
		//make sure we sample pages from all four points around the base point, in case the base point is on a page border
		for (int i = -65; i < 66; i += 64) {
			for (int j = -65; j < 66; j += 64) {
				TerrainPosition position(worldPosition.x() + i, worldPosition.y() + j);
				page = mManager.getHandler().getTerrainPageAtPosition(position);
				if (page) {
					pagesToUpdate.insert(page);
				}
			}
		}

		//make sure the marker node is updated
		BasePointUserObject* userObject = getUserObject(WFMath::Point<2>(entry.first.first, entry.first.second));
		if (userObject) {
			Mercator::BasePoint bp;
			bp.height() = entry.second.height;
			bp.roughness() = entry.second.roughness;
			bp.falloff() = entry.second.falloff;

			userObject->setBasePoint(bp);
		}
	}

	mManager.getHandler().updateTerrain(pointStore);

	//reload all shader textures of the affected pages
	// 	for (std::set<TerrainPage*>::iterator I = pagesToUpdate.begin(); I != pagesToUpdate.end(); ++I) {
	// 		(*I)->update();
	// 		(*I)->updateAllShaderTextures();
	// 	}

	// 	std::set<Ogre::PagingLandScapeData2D*> dataStore;
	// 	//reload all affected tiles
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
	// 	//also update the data
	// 	for (std::set<Ogre::PagingLandScapeData2D*>::iterator I = dataStore.begin(); I != dataStore.end(); ++I) {
	// 		uint x, z;
	// 		(*I)->getCoordinates(x, z);
	// 		sceneMgr->getData2DManager()->reload(x,z);
	// 	}
	// 	Ogre::Vector2 targetPage (X, Z);
	// 	sceneMgr->setOption("PageUpdate", &targetPage);

	//TODO: this shouldn't be necessary
	//sceneMgr->getPageManager()->load();
	// 	TerrainManager->getAdapter()->reloadAllPages();

	// 	updateEntityPositions(pagesToUpdate);

}

bool TerrainEditorOverlay::getBasePoint(const std::map<int, std::map<int, Mercator::BasePoint>>& basePoints, int x, int y, Mercator::BasePoint& z) const
{
	auto I = basePoints.find(x);
	if (I == basePoints.end()) {
		return false;
	}
	auto J = I->second.find(y);
	if (J == I->second.end()) {
		return false;
	}
	z = J->second;
	return true;
}

}

}
}
