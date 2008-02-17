//
// C++ Implementation: OpcodeCollisionDetector
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

#include "OpcodeCollisionDetector.h"
#include "ogreopcode/include/OgreCollisionManager.h"
#include "ogreopcode/include/OgreEntityCollisionShape.h"
#include "ogreopcode/include/OgreCollisionObject.h"

#include "EmberOgrePrerequisites.h"
#include "EmberEntityUserObject.h"

#include "OpcodeCollisionDetectorVisualizer.h"

#include "model/Model.h"
#include "model/SubModel.h"
namespace EmberOgre {

OpcodeCollisionDetector::OpcodeCollisionDetector(Model::Model* model) : mModel(model), mVisualizer(0)
{
	buildCollisionObjects();
}

OpcodeCollisionDetector::~OpcodeCollisionDetector()
{
	destroyCollisionObjects();
}

void OpcodeCollisionDetector::reload()
{
	destroyCollisionObjects();
	buildCollisionObjects();
}


void OpcodeCollisionDetector::destroyCollisionObjects()
{
	OgreOpcode::CollisionContext* collideContext = OgreOpcode::CollisionManager::getSingletonPtr()->getDefaultContext();
	for (OpcodeCollisionDetector::CollisionObjectStore::iterator I = mCollisionObjects.begin(); I != mCollisionObjects.end(); ++I)
	{
		OgreOpcode::ICollisionShape* shape = (*I)->getShape();
		collideContext->destroyObject(*I);
		OgreOpcode::CollisionManager::getSingleton().destroyShape(shape);
	}
	mCollisionObjects.clear();

}

void OpcodeCollisionDetector::buildCollisionObjects()
{
	OgreOpcode::CollisionContext* collideContext = OgreOpcode::CollisionManager::getSingletonPtr()->getDefaultContext();
	const Model::Model::SubModelSet& submodels = mModel->getSubmodels();
	for (Model::Model::SubModelSet::const_iterator I = submodels.begin(); I != submodels.end(); ++I)
	{
		Ogre::Entity* entity = (*I)->getEntity();
// 		if (entity->isVisible()) {
			std::string collideShapeName(std::string("entity_") + entity->getName());
			OgreOpcode::EntityCollisionShape *collideShape = OgreOpcode::CollisionManager::getSingletonPtr()->createEntityCollisionShape(collideShapeName.c_str());
	// 		if (!collideShape->isInitialized()) {
				collideShape->load(entity);
	// 		}
			OgreOpcode::CollisionObject* collideObject = collideContext->createObject(collideShapeName);
			
			collideObject->setShape(collideShape);
			
			collideContext->addObject(collideObject);
			
			mCollisionObjects.push_back(collideObject);
// 		}
// 			collideObject->setDebug(true, false, false, false);
	/*		collideShape->setDebug(true);
			collideShape->visualize();*/
	/*	
			EmberEntityUserObject* userObject = new EmberEntityUserObject(this, getModel(), (*I)->getEntity(), 0);
			(*I)->getEntity()->setUserObject(userObject);*/
	}
}

void OpcodeCollisionDetector::refit()
{
	for (OpcodeCollisionDetector::CollisionObjectStore::iterator I = mCollisionObjects.begin(); I != mCollisionObjects.end(); ++I)
	{
		(*I)->refit();
	}
}


void OpcodeCollisionDetector::testCollision(Ogre::Ray& ray, CollisionResult& result)
{
	
	for (OpcodeCollisionDetector::CollisionObjectStore::iterator I = mCollisionObjects.begin(); I != mCollisionObjects.end(); ++I) {
		OgreOpcode::ICollisionShape* collisionShape = (*I)->getShape();
		OgreOpcode::CollisionPair pick_result;
		
// 		(*I)->update(1);
		//collisionShape->setTransform( mModel->_getParentNodeFullTransform());
		if (collisionShape->rayCheck(OgreOpcode::COLLTYPE_CONTACT, (*I)->getTransform(), ray, 1000.0f, pick_result, false)) {
		
		
/*		if (collisionShape->rayCheck(OgreOpcode::COLLTYPE_CONTACT, mModel->_getParentNodeFullTransform(), ray, 1000.0f, pick_result, false)) {*/
			result.collided = true;
			result.distance = pick_result.distance;
			result.position = pick_result.contact;
			return;
		}
	}
}

void OpcodeCollisionDetector::setVisualize(bool visualize)
{
	if (visualize) {
		if (!mVisualizer) {
			mVisualizer = new OpcodeCollisionDetectorVisualizerInstance(*this);
		}
	} else {
		delete mVisualizer;
		mVisualizer = 0;
	}
}

bool OpcodeCollisionDetector::getVisualize() const
{
	return mVisualizer != 0;
}


}
