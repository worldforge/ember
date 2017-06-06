//
// C++ Implementation: EmberEntityLoader
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2008
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

#include "EmberEntityLoader.h"

#include "../model/Model.h"
#include "../model/SubModel.h"
#include "components/ogre/Convert.h"
#include "domain/EmberEntity.h"
#include "components/ogre/model/ModelRepresentation.h"

#include <sigc++/bind.h>

namespace Ember
{
namespace OgreView
{

namespace Environment
{

EmberEntityLoader::EmberEntityLoader(::Forests::PagedGeometry &geom, unsigned int batchSize) :
		mGeom(geom), mBatchSize(batchSize)
{
}

EmberEntityLoader::~EmberEntityLoader()
{
	//When shutting down, make sure to delete all connections.
#if EMBERENTITYLOADER_USEBATCH
	for (EntityStore::iterator I = mEntities.begin(); I != mEntities.end(); ++I) {
		for (EntityColumn::iterator J = I->second.begin(); J != I->second.end(); ++J) {
			for (EntityMap::iterator K = J->second.begin(); K != J->second.end(); ++K)
			{
				K->second.movedConnection.disconnect();
				K->second.visibilityChangedConnection.disconnect();
			}
		}
	}
#else
	for (EntityMap::iterator I = mEntities.begin(); I != mEntities.end(); ++I) {
		I->second.movedConnection.disconnect();
		I->second.visibilityChangedConnection.disconnect();
	}
#endif
}

void EmberEntityLoader::addEmberEntity(Model::ModelRepresentation* modelRepresentation)
{
	if (!modelRepresentation) {
		S_LOG_WARNING("Tried to add a null ref entity to the paged geometry.");
		return;
	}
	EmberEntity& entity = modelRepresentation->getEntity();
	ModelRepresentationInstance instance;
	instance.movedConnection = entity.Moved.connect(sigc::bind(sigc::mem_fun(*this, &EmberEntityLoader::EmberEntity_Moved), &entity));
	instance.visibilityChangedConnection = entity.VisibilityChanged.connect(sigc::bind(sigc::mem_fun(*this, &EmberEntityLoader::EmberEntity_VisibilityChanged), &entity));
	instance.modelRepresentation = modelRepresentation;

	WFMath::Point<3> viewPosition = entity.getViewPosition();
	Ogre::Vector3 position(std::numeric_limits<Ogre::Real>::quiet_NaN(), std::numeric_limits<Ogre::Real>::quiet_NaN(), std::numeric_limits<Ogre::Real>::quiet_NaN());

	bool isValidPos = false;
	if (viewPosition.isValid()) {
		isValidPos = true;
		position = Convert::toOgre(viewPosition);
	}
	instance.lastPosition = position;
#if EMBERENTITYLOADER_USEBATCH
	const int batchX = Ogre::Math::Floor(position.x / mBatchSize);
	const int batchY = Ogre::Math::Floor(position.y / mBatchSize);
	mEntityLookup[entity] = std::pair<int, int>(batchX, batchY);

	EntityMap& entities(mEntities[batchX][batchY]);
#else
	EntityMap& entities(mEntities);
#endif
	entities[entity.getId()] = instance;

	if (isValidPos) {
		//Rebuild geometry if necessary
		mGeom.reloadGeometryPage(position);
	}

}

void EmberEntityLoader::removeEmberEntity(EmberEntity* entity)
{
	if (!entity) {
		S_LOG_WARNING("Tried to remove a null ref entity from the paged geometry.");
		return;
	}
#if EMBERENTITYLOADER_USEBATCH
	EntityLookup::iterator I = mEntityLookup.find(entity);
	if (I != mEntityLookup.end()) {
		EntityStore::iterator J = mEntities.find(I->second.first);
		if (J != mEntities.end()) {
			EntityColumn& column(J->second);
			EntityColumn::iterator K = column.find(I->second.second);
			if (K != column.end()) {
				EntityMap& entityMap(K->second);
				EntityMap::iterator L = entityMap.find(entity->getId());
				if (L != entityMap.end()) {
					L->second.movedConnection.disconnect();
					L->second.beingDeletedConnection.disconnect();
					entityMap.erase(L);
					mEntityLookup.erase(I);
				}
			}
		}
	}
#else
	EntityMap::iterator I = mEntities.find(entity->getId());
	if (I != mEntities.end()) {
		ModelRepresentationInstance& instance(I->second);
		Model::ModelRepresentation* modelRepresentation(instance.modelRepresentation);
		instance.movedConnection.disconnect();
		instance.visibilityChangedConnection.disconnect();
		//Reset the rendering distance to the one set by the model def.
		//FIXME: this should be handled differently
		//modelRepresentation->getModel().setRenderingDistance(modelRepresentation->getModel().getDefinition()->getRenderingDistance());
		mEntities.erase(I);
	}

#endif

	WFMath::Point<3> pos = entity->getViewPosition();
	if (pos.isValid()) {
		//Rebuild geometry if necessary.
		mGeom.reloadGeometryPage(Convert::toOgre(pos));
	}
}

EmberEntityLoader::EntityMap* EmberEntityLoader::getStoreForEntity(EmberEntity* entity)
{
#if EMBERENTITYLOADER_USEBATCH
	EntityLookup::iterator I = mEntityLookup.find(entity);
	if (I != mEntityLookup.end()) {
		EntityStore::iterator J = mEntities.find(I->second.first);
		if (J != mEntities.end()) {
			EntityColumn& column(J->second);
			EntityColumn::iterator K = column.find(I->second.second);
			if (K != column.end()) {
				EntityMap& entityMap(K->second);
				return &entityMap;
			}
		}
	}
	return 0;
#else
	return &mEntities;
#endif
}

void EmberEntityLoader::loadPage(::Forests::PageInfo & page)
{
	static Ogre::ColourValue colour(1, 1, 1, 1);

#if EMBERENTITYLOADER_USEBATCH
	const int batchX = static_cast<int>(Ogre::Math::Floor(page.bounds.left/ mBatchSize));
	const int batchY = static_cast<int>(Ogre::Math::Floor(page.bounds.top / mBatchSize));
	EntityMap& entities(mEntities[batchX][batchY]);
#else
	EntityMap& entities(mEntities);
#endif

	for (EntityMap::iterator I = entities.begin(); I != entities.end(); ++I) {
		ModelRepresentationInstance& instance(I->second);
		Model::ModelRepresentation* modelRepresentation(instance.modelRepresentation);
		EmberEntity& emberEntity = modelRepresentation->getEntity();
		if (emberEntity.isVisible()) {
			WFMath::Point<3> viewPos = emberEntity.getViewPosition();
			if (viewPos.isValid()) {
				Ogre::Vector3 pos(Convert::toOgre(viewPos));
				Model::Model& model(modelRepresentation->getModel());
				if (pos.x > page.bounds.left && pos.x < page.bounds.right && pos.z > page.bounds.top && pos.z < page.bounds.bottom) {
					for (auto& submodel : model.getSubmodels()) {
						// 				if (!(*J)->getEntity()->getParentSceneNode()) {
						// 					model->getParentSceneNode()->attachObject((*J)->getEntity());
						// 				}
						//  				if ((*J)->getEntity()->isVisible()) {
						addEntity(submodel->getEntity(), pos, Convert::toOgre(emberEntity.getViewOrientation()), modelRepresentation->getScale(), colour);
						// 					(*J)->getEntity()->setVisible(false);
						//  				}
					}
				}
			}
		}
	}
}

void EmberEntityLoader::EmberEntity_Moved(EmberEntity* entity)
{
	EntityMap* entityMap(getStoreForEntity(entity));
	if (entityMap) {
		EntityMap::iterator I = entityMap->find(entity->getId());
		if (I != entityMap->end()) {
			ModelRepresentationInstance& instance(I->second);
			if (!instance.lastPosition.isNaN()) {
				mGeom.reloadGeometryPage(instance.lastPosition);
			}
			WFMath::Point<3> viewPos = entity->getViewPosition();
			if (viewPos.isValid()) {
				mGeom.reloadGeometryPage(Convert::toOgre(viewPos));
				instance.lastPosition = Convert::toOgre(viewPos);
			}
		}
	}
}

void EmberEntityLoader::EmberEntity_VisibilityChanged(bool, EmberEntity* entity)
{
	WFMath::Point<3> viewPos = entity->getViewPosition();
	if (viewPos.isValid()) {
		//When the visibility changes, we only need to reload the page the entity is on.
		mGeom.reloadGeometryPage(Convert::toOgre(viewPos));
	}
}

}

}
}

