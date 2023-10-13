//
// C++ Implementation: IconManager
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2007
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
#include "IconManager.h"

#include "components/ogre/EmberOgre.h"
#include "components/ogre/World.h"
#include "domain/EmberEntity.h"
#include "components/ogre/SimpleRenderContext.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/entitymapping/EntityMappingManager.h"
#include "components/entitymapping/IActionCreator.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "components/ogre/mapping/ModelActionCreator.h"
#include "services/server/ServerService.h"
#include "services/EmberServices.h"
#include <Eris/Connection.h>
#include <Eris/View.h>

#include <OgreTextureManager.h>

#include <utility>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Icons {

class DummyEntity : public Eris::Entity {

public:
	DummyEntity(const std::string& id, Eris::TypeInfo* ty) :
			Eris::Entity(id, ty) {
	}

	Eris::Entity* getEntity(const std::string&) override {
		return nullptr;
	}

};


IconManager::IconManager() :
		mIconRenderer("IconManager", 64) {
	//if the direct renderer is activated you must also update IconImageStore so that a RenderTarget texture is used
	mIconRenderer.setWorker(std::make_unique<DirectRendererWorker>(mIconRenderer));

	//mIconRenderer.setWorker(new DelayedIconRendererWorker(mIconRenderer));
}

Icon* IconManager::getIcon(int, EmberEntity* entity) {

	std::string key = "entity_" + entity->getId();
	if (mIconStore.hasIcon(key)) {
		return mIconStore.getIcon(key);
	} else {
		std::string modelName;


		Mapping::ModelActionCreator actionCreator(*entity, [&](const std::string& newModelName) {
			modelName = newModelName;
		}, [&](const std::string& partName) {
			//Ignore parts
		});
		auto& manager = Mapping::EmberEntityMappingManager::getSingleton().getManager();
		auto& view = EmberOgre::getSingleton().getWorld()->getView();
		auto modelMapping = manager.createMapping(*entity, actionCreator, view.getTypeService(), &view);


		if (modelMapping) {
			modelMapping->initialize();
		}
		if (modelName.empty()) {
			return nullptr;
		}
		auto modelDefPtr = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
		if (modelDefPtr) {
			Model::ModelDefinition* modelDef = modelDefPtr.get();
			const std::string& iconPath(modelDef->getIconPath());
			if (!iconPath.empty()) {

				Ogre::TexturePtr texPtr;
				try {
					if (Ogre::TextureManager::getSingleton().resourceExists(iconPath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME)) {
						texPtr = Ogre::TextureManager::getSingleton().getByName(iconPath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
						//try to load it to make sure that's it a working image
						texPtr->load();
					}
					if (!texPtr) {
						texPtr = Ogre::TextureManager::getSingleton().load(iconPath, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
					}
				} catch (...) {
					S_LOG_WARNING("Error when trying to load the icon " << iconPath << ". The icon will be rendered dynamically.");
					texPtr.reset();
				}
				if (texPtr) {
					Icon* icon = mIconStore.createIcon(key, texPtr);
					return icon;
				}
			}
		}
		Icon* icon = mIconStore.createIcon(key);
		if (icon) {
			//update the model preview window
			// 				Model::Model* model = Model::Model::createModel(mIconRenderer.getRenderContext()->getSceneManager(), modelName);
			render(*icon, modelName);
			// 				mIconRenderer.getRenderContext()->getSceneManager()->destroyMovableObject(model);
		}
		return icon;
	}

}

Icon* IconManager::getIcon(int, Eris::TypeInfo* erisType) {

	std::string key = "entity_" + erisType->getName();
	if (mIconStore.hasIcon(key)) {
		return mIconStore.getIcon(key);
	} else {
		Icon* icon = mIconStore.createIcon(key);
		return icon;
	}
}

void IconManager::render(Icon& icon, EmberEntity& entity) {
	std::string modelName;
	Mapping::ModelActionCreator actionCreator(entity, [&](const std::string& newModelName) {
		modelName = newModelName;
	}, [&](const std::string& partName) {
		//Ignore parts
	});
	auto& view = EmberOgre::getSingleton().getWorld()->getView();
	std::unique_ptr<EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(entity, actionCreator, view.getTypeService(), &view));
	if (modelMapping) {
		modelMapping->initialize();
	}
	//if there's no model defined for this use the placeholder model
	if (modelName.empty()) {
		modelName = "common/primitives/placeholder.modeldef";
	}

	render(icon, modelName);
}

void IconManager::render(Icon& icon, Eris::TypeInfo& erisType) {
	//we need to get the model mapping definition for this type
	//once we have that, we will check for the first action of the first case of the first match (since that's guaranteed to be a show-model action
	Eris::Connection* conn = EmberServices::getSingleton().getServerService().getConnection();
	if (conn) {
		auto& typeService = conn->getTypeService();
		DummyEntity dummyEntity("-1", &erisType);
		std::string modelName;
		Mapping::ModelActionCreator actionCreator(dummyEntity, [&](const std::string& newModelName) {
			modelName = newModelName;
		}, [&](const std::string& partName) {
			//Ignore parts
		});
		std::unique_ptr<EntityMapping::EntityMapping> modelMapping(Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(dummyEntity, actionCreator, typeService, &EmberOgre::getSingleton().getWorld()->getView()));
		if (modelMapping) {
			modelMapping->initialize();
		}
		//if there's no model defined for this use the placeholder model
		if (modelName.empty()) {
			modelName = "common/primitives/placeholder.modeldef";
		}
		//update the model preview window
		// 					Model::Model* model = Model::Model::createModel(mIconRenderer.getRenderContext()->getSceneManager(), modelName);
		render(icon, modelName);
		// 					mIconRenderer.getRenderContext()->getSceneManager()->destroyMovableObject(model);

	}
}

void IconManager::render(Icon& icon, const std::string& modelName) {
	mIconRenderer.render(modelName, &icon);
}

}

}

}
}
