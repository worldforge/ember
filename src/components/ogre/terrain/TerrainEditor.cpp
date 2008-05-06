//
// C++ Implementation: TerrainEditor
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2006
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

#include "TerrainEditor.h"
#include "../EmberOgrePrerequisites.h"
#include "../EmberOgre.h"
#include "../Avatar.h"
#include "TerrainGenerator.h"
#include "../AvatarCamera.h"
#include "../GUIManager.h"

#include <Atlas/Objects/Entity.h>
#include <Atlas/Objects/Operation.h>

#include <Eris/Connection.h>

#include "../EmberEntity.h"
#include "../WorldEmberEntity.h"
#include "../EmberPhysicalEntity.h"
#include "../AvatarEmberEntity.h"

#include "../EmberEntityFactory.h"


#include "services/EmberServices.h"
#include "services/server/ServerService.h"
#include "ISceneManagerAdapter.h"

#include "../MousePicker.h"


namespace EmberOgre {
namespace Terrain {

const std::string BasePointUserObject::s_TypeName("BasePointMarker");
	
	
BasePointUserObject::BasePointUserObject(const TerrainPosition terrainPosition,const Mercator::BasePoint& basePoint, Ogre::SceneNode* basePointMarkerNode) : mBasePoint(basePoint), mBasePointMarkerNode(basePointMarkerNode), mPosition(terrainPosition)
{
}

const Ogre::String & BasePointUserObject::getTypeName (void) const
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
	getBasePointMarkerNode()->translate(Ogre::Vector3(0,verticalMovement,0));
	markAsMoved();
	EventUpdatedPosition();
}

void BasePointUserObject::setHeight(Ogre::Real height)
{
	Ogre::Vector3 position = getBasePointMarkerNode()->getPosition();
	getBasePointMarkerNode()->setPosition(position.x, height, position.z);
	markAsMoved();
	EventUpdatedPosition();
}

void BasePointUserObject::markAsMoved()
{
	Ogre::Entity* entity = static_cast<Ogre::Entity*>(getBasePointMarkerNode()->getAttachedObject(0));
	entity->setMaterialName("BasePointMarkerMovedMaterial");
}

void BasePointUserObject::resetMarking()
{
	Ogre::Entity* entity = static_cast<Ogre::Entity*>(getBasePointMarkerNode()->getAttachedObject(0));
	entity->setMaterialName("BasePointMarkerMaterial");
}

BasePointPickListener::BasePointPickListener(TerrainEditor* terrainEditor) 
: mTerrainEditor(terrainEditor), mPickedUserObject(0)
{

}

void BasePointPickListener::processPickResult(bool& continuePicking, Ogre::RaySceneQueryResultEntry& entry, Ogre::Ray& cameraRay, const MousePickerArgs& mousePickerArgs)
{
	if (entry.movable) {
		Ogre::MovableObject* pickedMovable = entry.movable;
		if (pickedMovable->isVisible() && pickedMovable->getUserObject() != 0 && pickedMovable->getUserObject()->getTypeName() == BasePointUserObject::s_TypeName) {
			mPickedUserObject = static_cast<BasePointUserObject*>(pickedMovable->getUserObject());
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
		mTerrainEditor->pickedBasePoint(mPickedUserObject);
	}
}


TerrainEditBasePointMovement::TerrainEditBasePointMovement(Ogre::Real verticalMovement, TerrainPosition position)
: mVerticalMovement(verticalMovement), mPosition(position)
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


TerrainEditor::TerrainEditor() : mPickListener(this), mCurrentUserObject(0),mOverlayNode(0), mVisible(false)
{
	///create a material which will be used for base points (this will be blue)
	if (!Ogre::MaterialManager::getSingleton().resourceExists("BasePointMarkerMaterial")) {
		Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create("BasePointMarkerMaterial", "General"));
		
		material->setDiffuse(0,0,1,1);
		material->setAmbient(0,0,1);
		material->setSelfIllumination(0,0,1);
		//material->setLightingEnabled(false);
		material->setFog(true);
	}
	///create a material which will be used for base points that have been moved (this will be red)
	if (!Ogre::MaterialManager::getSingleton().resourceExists("BasePointMarkerMovedMaterial")) {
		Ogre::MaterialPtr material = static_cast<Ogre::MaterialPtr>(Ogre::MaterialManager::getSingleton().create("BasePointMarkerMovedMaterial", "General"));
		
		material->setDiffuse(1,0,0,1);
		material->setAmbient(1,0,0);
		material->setSelfIllumination(1,0,0);
		//material->setLightingEnabled(false);
		material->setFog(true);
	}
//	createOverlay();
	//hideOverlay();
}


TerrainEditor::~TerrainEditor()
{
	//TODO: also delete user objects
	if (mOverlayNode) {
		Ogre::SceneNode* parent = static_cast<Ogre::SceneNode*>(mOverlayNode->getParent());
		if (parent) {
			parent->removeAndDestroyChild(mOverlayNode->getName());
		} else {
			mOverlayNode->getCreator()->destroySceneNode(mOverlayNode->getName());
		}
	}
}

void TerrainEditor::showOverlay()
{
	mVisible = true;
	if (mOverlayNode) {
		mOverlayNode->setVisible(true, true);
	}

}

void TerrainEditor::hideOverlay()
{
	mVisible = false;
	if (mOverlayNode) {
		mOverlayNode->setVisible(false, true);
	}
}

bool TerrainEditor::isOverlayShown() const
{

	return mOverlayNode != 0 && mVisible;
}

void TerrainEditor::createOverlay()
{
	if (!mOverlayNode) {
		
		mOverlayNode = EmberOgre::getSingleton().getWorldSceneNode()->createChildSceneNode();
		
		const Mercator::Terrain& terrain = EmberOgre::getSingleton().getTerrainGenerator()->getTerrain();
		const Mercator::Terrain::Pointstore &points = terrain.getPoints();
		int x, y;
		for (Mercator::Terrain::Pointstore::const_iterator I = points.begin(); I != points.end(); ++I) {
			x = I->first;
			for (Mercator::Terrain::Pointcolumn::const_iterator J = I->second.begin(); J != I->second.end(); ++J) {
				y = J->first;
				const Mercator::BasePoint& basepoint = J->second;
				Ogre::SceneNode* basepointNode = mOverlayNode->createChildSceneNode();
				TerrainPosition tPos(x*64,y*64);
				Ogre::Vector3 ogrePos = Atlas2Ogre(tPos);
				ogrePos.y = basepoint.height();
				basepointNode->setPosition(ogrePos);
				std::stringstream ss;
				ss << "basepointmarker" << x << "_" << y;
				Ogre::Entity* entity = EmberOgre::getSingleton().getSceneManager()->createEntity(ss.str(), "3d_objects/primitives/models/sphere.mesh");
				///start out with a normal material
				entity->setMaterialName("BasePointMarkerMaterial");
				entity->setRenderingDistance(300);
				entity->setQueryFlags(MousePicker::CM_UNDEFINED);
				basepointNode->attachObject(entity);
				BasePointUserObject* userObject = new BasePointUserObject(TerrainPosition(x,y), basepoint, basepointNode);
				entity->setUserObject(userObject);
				
				
				///store the base point user object
				std::stringstream ss_;
				ss_ << x << "_" << y;
				mBasePointUserObjects[ss_.str()] = userObject;
			}
		}
		///register the pick listener
		EmberOgre::getSingleton().getMainCamera()->pushWorldPickListener(&mPickListener);
	}
}

BasePointUserObject* TerrainEditor::getUserObject(const TerrainPosition& terrainIndex)
{
	std::stringstream ss;
	ss << terrainIndex.x() << "_" << terrainIndex.y();
	BasePointUserObjectStore::iterator I = mBasePointUserObjects.find(ss.str());
	if (I != mBasePointUserObjects.end()) {
		return I->second;
	}
	return 0;

}

BasePointUserObject* TerrainEditor::getCurrentBasePointUserObject() const
{
	return mCurrentUserObject;
}


void TerrainEditor::pickedBasePoint(BasePointUserObject* userObject)
{
	assert(userObject);
	mCurrentUserObject = userObject;
	catchInput();
	EventPickedBasePoint.emit(userObject);
}


bool TerrainEditor::injectMouseMove(const MouseMotion& motion, bool& freezeMouse)
{
	assert(mCurrentUserObject);
	mCurrentUserObject->translate(motion.yRelativeMovement * 15);

	EventSelectedBasePointUpdatedPosition.emit(mCurrentUserObject);
	
	///we don't want to move the cursor
	freezeMouse = true;
	return false;
}

bool TerrainEditor::injectMouseButtonUp(const Input::MouseButton& button)
{
	if (button == Input::MouseButtonLeft) {
		releaseInput();
	}
	return true;
}

bool TerrainEditor::injectMouseButtonDown(const Input::MouseButton& button)
{
	return true;
}

bool TerrainEditor::injectChar(char character)
{
	return true;
}

bool TerrainEditor::injectKeyDown(const SDLKey& key)
{
	return true;
}

bool TerrainEditor::injectKeyUp(const SDLKey& key)
{
	return true;
}

void TerrainEditor::catchInput()
{
	GUIManager::getSingleton().getInput().addAdapter(this);
}

void TerrainEditor::releaseInput()
{
	GUIManager::getSingleton().getInput().removeAdapter(this);
	
	///react on the movement
	createAction(true);

}

void TerrainEditor::createAction(bool alsoCommit)
{
	if (mCurrentUserObject) {
		///lets get how much it moved
		float distance = mCurrentUserObject->getBasePointMarkerNode()->getPosition().y - mCurrentUserObject->getBasePoint().height();
		///only register an action if it has been moved
		if (distance != 0) {
			TerrainEditBasePointMovement movement(distance, mCurrentUserObject->getPosition());
			TerrainEditAction action;
			action.getMovements().push_back(movement);
			
			mActions.push_back(action);
			
			///when a new action is created the undo list must be emptied
			mUndoneActions.clear();

			
			EventActionCreated(&action);
			
			if (alsoCommit)
			{
				commitAction(action);
			}
		}
	}
}

void TerrainEditor::sendChangesToServer()
{

	try {
		std::map<std::string, TerrainPosition> positions;
		for (ActionStore::iterator I = mActions.begin(); I != mActions.end(); ++I) {
			for(TerrainEditAction::MovementStore::const_iterator J = I->getMovements().begin(); J != I->getMovements().end(); ++J)
			{
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
			EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getBasePoint(static_cast<int>(basepointX),static_cast<int>(basepointY), bp);
			
			Atlas::Message::ListType & point =
					(pointMap[I->first] = Atlas::Message::ListType(3)).asList();
			point[0] = (Atlas::Message::FloatType)(I->second.x());
			point[1] = (Atlas::Message::FloatType)(I->second.y());
			point[2] = (Atlas::Message::FloatType)(bp.height());
			
			
		}
		
	
		Atlas::Message::ListType sargsList(1, sarg);
		s->setArgsAsList(sargsList);
		s->setFrom(EmberOgre::getSingleton().getAvatar()->getAvatarEmberEntity()->getId());
	
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
	}catch (const Atlas::Exception& ex)
	{
		S_LOG_FAILURE("Could not send terrain to server. Message: " << ex.what());
	}catch (const std::exception& ex)
	{
		S_LOG_FAILURE("Could not send terrain to server. Message: " << ex.what());
	}

}

bool TerrainEditor::undoLastAction()
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

bool TerrainEditor::redoAction()
{
	if (mUndoneActions.size()) {
		TerrainEditAction action = mUndoneActions.front();
		mUndoneActions.pop_front();
		mActions.push_back(action);
		commitAction(action);
	}
	return false;
}


void TerrainEditor::commitAction(const TerrainEditAction& action, bool reverse)
{
	TerrainGenerator::TerrainDefPointStore pointStore;
	
// 	std::set<Ogre::PagingLandScapeTile*> tilesToUpdate;
	std::set<TerrainPage*> pagesToUpdate;
// 	EmberPagingSceneManager* sceneMgr = EmberOgre::getSingleton().getTerrainGenerator()->getEmberSceneManager();
	TerrainGenerator* terrainGenerator = EmberOgre::getSingleton().getTerrainGenerator();
	for(TerrainEditAction::MovementStore::const_iterator I = action.getMovements().begin(); I != action.getMovements().end(); ++I)
	{
		Mercator::BasePoint bp;
		int basepointX = static_cast<int>(I->getPosition().x());
		int basepointY = static_cast<int>(I->getPosition().y());
		EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().getBasePoint(basepointX,basepointY, bp);
		///check if we should do a reverse action (which is done when an action is undone)
        bp.height() = bp.height() + (reverse ? -I->getVerticalMovement() : I->getVerticalMovement());
		//EmberOgre::getSingleton().getTerrainGenerator()->getTerrain().setBasePoint(basepointX, basepointY, bp);
		
		TerrainDefPoint defPoint(basepointX, basepointY,bp.height());
		pointStore.push_back(defPoint);
		
		
// 		Ogre::Vector3 markerPos = Atlas2Ogre(I->getPosition());
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
				page = terrainGenerator->getTerrainPage(position);
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
	
// 	for (TerrainGenerator::TerrainPagestore::const_iterator I = terrainGenerator->getTerrainPages().begin();I != terrainGenerator->getTerrainPages().end(); ++I) {
// 		for (TerrainGenerator::TerrainPagecolumn::const_iterator J = I->second.begin();J != I->second.end(); ++J) {
// 			pagesToUpdate.insert(J->second);
// 		}
// 	}
	EmberOgre::getSingleton().getTerrainGenerator()->updateTerrain(pointStore);
	
	
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
// 	terrainGenerator->getAdapter()->reloadAllPages();
	
	
// 	updateEntityPositions(pagesToUpdate);
	

}






}
}
