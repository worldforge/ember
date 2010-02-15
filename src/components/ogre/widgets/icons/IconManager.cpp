//
// C++ Implementation: IconManager
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
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

#include "IconManager.h"

#include "components/ogre/EmberEntity.h"
#include "components/ogre/SimpleRenderContext.h"
#include "components/ogre/model/Model.h"
#include "components/ogre/model/ModelDefinitionManager.h"
#include "components/entitymapping/EntityMappingManager.h"
#include "components/entitymapping/EntityMapping.h"
#include "components/entitymapping/Definitions/EntityMappingDefinition.h"
#include "components/entitymapping/IActionCreator.h"
#include "components/ogre/mapping/EmberEntityMappingManager.h"
#include "main/Application.h"
#include "services/server/ServerService.h"
#include <Eris/Entity.h>
#include <Eris/TypeInfo.h>
#include <Eris/Connection.h>

#include <OgreTextureManager.h>


namespace EmberOgre {

namespace Gui {

namespace Icons {


class DummyEntity : public Eris::Entity
{
protected:
	Eris::TypeService* mTypeService;

public:
	DummyEntity(const std::string& id, Eris::TypeInfo* ty, Eris::TypeService* typeService) : Eris::Entity(id, ty), mTypeService(typeService)
	{}

	Eris::TypeService* getTypeService() const
	{
		return mTypeService;
	}

	void removeFromMovementPrediction()
	{}

	void addToMovementPredition()
	{}

	Eris::Entity* getEntity(const std::string& id)
	{
		return 0;
	}

    virtual Eris::View* getView() const {
    	return 0;
    }
};

/**
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class IconActionCreator : public Ember::EntityMapping::IActionCreator
{
public:

IconActionCreator(Eris::Entity& entity): mEntity(entity), mModelName("")
{
}

~IconActionCreator() {}

virtual void createActions(Ember::EntityMapping::EntityMapping& modelMapping, Ember::EntityMapping::Cases::CaseBase* aCase, Ember::EntityMapping::Definitions::CaseDefinition& caseDefinition)
{
	Ember::EntityMapping::Definitions::CaseDefinition::ActionStore::iterator endJ = caseDefinition.getActions().end();
	for (Ember::EntityMapping::Definitions::CaseDefinition::ActionStore::iterator J = caseDefinition.getActions().begin(); J != endJ; ++J) {
		if (J->getType() == "display-model") {
			mModelName = J->getValue();
		}
	}
}

const std::string& getModelName() const
{
	return mModelName;
}
protected:
Eris::Entity& mEntity;
std::string mModelName;

};



IconManager::IconManager()
: mIconRenderer("IconManager", 64)
{
//if the direct renderer is activated you must also update IconImageStore so that a RenderTarget texture is used
// 	mIconRenderer.setWorker(new DirectRendererWorker(mIconRenderer));

 	mIconRenderer.setWorker(new DelayedIconRendererWorker(mIconRenderer));
}


IconManager::~IconManager()
{
}

Icon* IconManager::getIcon(int pixelWidth, EmberEntity* entity)
{

	std::string key = "entity_" + entity->getId();
	if (mIconStore.hasIcon(key)) {
		return mIconStore.getIcon(key);
	} else {
		IconActionCreator actionCreator(*entity);
		std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(::EmberOgre::Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(*entity, &actionCreator, Ember::Application::getSingleton().getMainView()));
		std::string modelName;
		if (modelMapping.get()) {
			modelMapping->initialize();
			modelName = actionCreator.getModelName();
		}
		///if there's no model defined for this use the placeholder model
		if (modelName == "") {
			modelName = "placeholder";
		}
		Ogre::ResourcePtr modelDefPtr = Model::ModelDefinitionManager::getSingleton().getByName(modelName);
		if (!modelDefPtr.isNull()) {
			Model::ModelDefinition* modelDef = static_cast< Model::ModelDefinition*>(modelDefPtr.get());
			const std::string& iconPath(modelDef->getIconPath());
			if (iconPath != "") {

				Ogre::TexturePtr texPtr;
				try {
					if (Ogre::TextureManager::getSingleton().resourceExists(iconPath)) {
						texPtr = static_cast<Ogre::TexturePtr>(Ogre::TextureManager::getSingleton().getByName(iconPath));
						///try to load it to make sure that's it a working image
						texPtr->load();
					}
					if (texPtr.isNull()) {
						texPtr = Ogre::TextureManager::getSingleton().load(iconPath, "Gui");
					}
				} catch (...) {
					S_LOG_WARNING("Error when trying to load the icon " << iconPath <<". The icon will be rendered dynamically.");
					texPtr.setNull();
				}
				if (!texPtr.isNull()) {
					Icon* icon = mIconStore.createIcon(key, texPtr);
					return icon;
				}
			}
		}
		Icon* icon = mIconStore.createIcon(key);
		if (icon) {
			///update the model preview window
// 				Model::Model* model = Model::Model::createModel(mIconRenderer.getRenderContext()->getSceneManager(), modelName);
			mIconRenderer.render(modelName, icon);
// 				mIconRenderer.getRenderContext()->getSceneManager()->destroyMovableObject(model);
		}
		return icon;
	}

	return 0;
}

Icon* IconManager::getIcon(int pixelWidth, Eris::TypeInfo* erisType)
{

	std::string key = "entity_" + erisType->getName();
	if (mIconStore.hasIcon(key)) {
		return mIconStore.getIcon(key);
	} else {
		Icon* icon = mIconStore.createIcon(key);
		if (icon) {
			///we need to get the model mapping definition for this type
			///once we have that, we will check for the first action of the first case of the first match (since that's guaranteed to be a show-model action
			if (erisType) {
				Eris::Connection* conn = Ember::EmberServices::getSingleton().getServerService()->getConnection();
				if (conn) {
					Eris::TypeService* typeService = conn->getTypeService();
					if (typeService) {
						DummyEntity dummyEntity("-1", erisType, typeService);
						IconActionCreator actionCreator(dummyEntity);
						std::auto_ptr<Ember::EntityMapping::EntityMapping> modelMapping(::EmberOgre::Mapping::EmberEntityMappingManager::getSingleton().getManager().createMapping(dummyEntity, &actionCreator, Ember::Application::getSingleton().getMainView()));
						std::string modelName;
						if (modelMapping.get()) {
							modelMapping->initialize();
							modelName = actionCreator.getModelName();
						}
						///if there's no model defined for this use the placeholder model
						if (modelName == "") {
							modelName = "placeholder";
						}
						///update the model preview window
	// 					Model::Model* model = Model::Model::createModel(mIconRenderer.getRenderContext()->getSceneManager(), modelName);
						mIconRenderer.render(modelName, icon);
	// 					mIconRenderer.getRenderContext()->getSceneManager()->destroyMovableObject(model);

						dummyEntity.shutdown();
					}
				}
			}
		}
		return icon;
	}
}


}

}

}
