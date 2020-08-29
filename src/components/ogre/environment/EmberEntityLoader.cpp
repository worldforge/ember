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
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
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

#include <OgreNode.h>

#include <sigc++/bind.h>

namespace Ember {
namespace OgreView {

namespace Environment {

EmberEntityLoader::EmberEntityLoader(::Forests::PagedGeometry& geom, unsigned int batchSize) :
		mGeom(geom), mBatchSize(batchSize) {
}

EmberEntityLoader::~EmberEntityLoader() {
	//When shutting down, make sure to delete all connections.
#if EMBERENTITYLOADER_USEBATCH
	for (auto& column : mEntities) {
		for (auto& row : column.second) {
			for (auto& entry : row.second) {
				entry.second.movedConnection.disconnect();
				entry.second.visibilityChangedConnection.disconnect();
			}
		}
	}
#else
	for (auto& entity : mEntities) {
		entity.second.movedConnection.disconnect();
		entity.second.visibilityChangedConnection.disconnect();
	}
#endif
}

void EmberEntityLoader::addEmberEntity(Model::ModelRepresentation* modelRepresentation) {
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
	const int batchX = static_cast<const int>(std::floor(position.x / mBatchSize));
	const int batchZ = static_cast<const int>(std::floor(position.z / mBatchSize));
	mEntityLookup.insert(std::make_pair(&entity, std::pair<int, int>(batchX, batchZ)));

	EntityMap& entities(mEntities[batchX][batchZ]);
#else
	EntityMap& entities(mEntities);
#endif
	entities[entity.getId()] = instance;

	if (isValidPos) {
		//Rebuild geometry if necessary
		mGeom.reloadGeometryPage(position);
	}

}

void EmberEntityLoader::removeEmberEntity(EmberEntity* entity) {
	if (!entity) {
		S_LOG_WARNING("Tried to remove a null ref entity from the paged geometry.");
		return;
	}
#if EMBERENTITYLOADER_USEBATCH
	auto I = mEntityLookup.find(entity);
	if (I != mEntityLookup.end()) {
		auto J = mEntities.find(I->second.first);
		if (J != mEntities.end()) {
			EntityColumn& column = J->second;
			auto K = column.find(I->second.second);
			if (K != column.end()) {
				EntityMap& entityMap(K->second);
				auto L = entityMap.find(entity->getId());
				if (L != entityMap.end()) {
					L->second.movedConnection.disconnect();
					L->second.visibilityChangedConnection.disconnect();
					entityMap.erase(L);
					mEntityLookup.erase(I);
				}
			}
		}
	}
#else
	auto I = mEntities.find(entity->getId());
	if (I != mEntities.end()) {
		ModelRepresentationInstance& instance(I->second);
		//Model::ModelRepresentation* modelRepresentation(instance.modelRepresentation);
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
		mGeom.reloadGeometryPage(Convert::toOgre(pos), true);
	}
}

EmberEntityLoader::EntityMap* EmberEntityLoader::getStoreForEntity(EmberEntity* entity) {
#if EMBERENTITYLOADER_USEBATCH
	auto I = mEntityLookup.find(entity);
	if (I != mEntityLookup.end()) {
		auto J = mEntities.find(I->second.first);
		if (J != mEntities.end()) {
			EntityColumn& column = J->second;
			auto K = column.find(I->second.second);
			if (K != column.end()) {
				EntityMap& entityMap(K->second);
				return &entityMap;
			}
		}
	}
	return nullptr;
#else
	return &mEntities;
#endif
}

void EmberEntityLoader::loadPage(::Forests::PageInfo& page) {
	static Ogre::ColourValue colour(1, 1, 1, 1);

#if EMBERENTITYLOADER_USEBATCH
	int batchX = static_cast<int>(std::floor(page.bounds.left/ mBatchSize));
	int batchZ = static_cast<int>(std::floor(page.bounds.top / mBatchSize));
	EntityMap& entities(mEntities[batchX][batchZ]);
#else
	EntityMap& entities(mEntities);
#endif

	for (auto& entity : entities) {
		ModelRepresentationInstance& instance = entity.second;
		Model::ModelRepresentation* modelRepresentation = instance.modelRepresentation;
		auto* nodeProvider = modelRepresentation->getModel().getNodeProvider();
		EmberEntity& emberEntity = modelRepresentation->getEntity();
		if (nodeProvider) {
			if (emberEntity.isVisible()) {
				Ogre::Node* node = nodeProvider->getNode();
				const Ogre::Vector3& pos = node->_getDerivedPosition();
				const Ogre::Quaternion& orient = node->_getDerivedOrientation();
				if (!pos.isNaN() && !orient.isNaN()) {
					Model::Model& model = modelRepresentation->getModel();
					if (pos.x > page.bounds.left && pos.x < page.bounds.right && pos.z > page.bounds.top && pos.z < page.bounds.bottom && model.getBoundingBox().isFinite()) {
						addModel(&model, pos, orient, modelRepresentation->getScale(), colour);
					}
				}
			}
		}
	}
}

void EmberEntityLoader::EmberEntity_Moved(EmberEntity* entity) {
	EntityMap* entityMap(getStoreForEntity(entity));
	if (entityMap) {
		auto I = entityMap->find(entity->getId());
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

void EmberEntityLoader::EmberEntity_VisibilityChanged(bool, EmberEntity* entity) {
	WFMath::Point<3> viewPos = entity->getViewPosition();
	if (viewPos.isValid()) {
		//When the visibility changes, we only need to reload the page the entity is on.
		mGeom.reloadGeometryPage(Convert::toOgre(viewPos));
	}
}

}

}
}

