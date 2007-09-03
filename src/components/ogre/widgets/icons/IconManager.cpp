//
// C++ Implementation: IconManager
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@iteam.se>, (C) 2007
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
#include "IconManager.h"
#include "../../EmberPhysicalEntity.h"
#include "../../EmberEntity.h"
#include <Eris/Entity.h>
#include <Eris/TypeInfo.h>
#include "components/ogre/model/Model.h"
#include "components/ogre/model/mapping/ModelMappingManager.h"
#include "components/ogre/model/mapping/EmberModelMappingManager.h"
#include "../../SimpleRenderContext.h"


namespace EmberOgre {

namespace Gui {

namespace Icons {

IconManager::IconManager()
: mIconRenderer("IconManager", 64)
{
}


IconManager::~IconManager()
{
}

Icon* IconManager::getIcon(int pixelWidth, EmberOgre::EmberPhysicalEntity* entity)
{
	std::string key = "entity_" + entity->getId();
	if (mIconStore.hasIcon(key)) {
		return mIconStore.getIcon(key);
	} else {
		Icon* icon = mIconStore.createIcon(key);
		mIconRenderer.render(entity->getModel(), icon);
		return icon;
	}
}

Icon* IconManager::getIcon(int pixelWidth, Eris::TypeInfo* erisType)
{
	
	std::string key = "entity_" + erisType->getName();
	if (mIconStore.hasIcon(key)) {
		return mIconStore.getIcon(key);
	} else {
		Icon* icon = mIconStore.createIcon(key);
		///we need to get the model mapping definition for this type
		///once we have that, we will check for the first action of the first case of the first match (since that's guaranteed to be a show-model action
		if (erisType) {
			const Model::Mapping::Definitions::ModelMappingDefinition* definition = Model::Mapping::EmberModelMappingManager::getSingleton().getManager().getDefinitionForType(erisType);
			if (definition) {
				Model::Mapping::Definitions::MatchDefinition::CaseStore::const_iterator first = definition->getRoot().getCases().begin();
				if (first != definition->getRoot().getCases().end()) {
					const Model::Mapping::Definitions::CaseDefinition& firstCase = *first;
					if (firstCase.getActions().begin() != firstCase.getActions().end()) {
						const Model::Mapping::Definitions::ActionDefinition& firstAction = *firstCase.getActions().begin();
						if (firstAction.getType() == "display-model") {
							///update the model preview window
							std::string modelName(firstAction.getValue());
							if (modelName != "") {
								Model::Model* model = Model::Model::createModel(mIconRenderer.getRenderContext()->getSceneManager(), modelName);
								mIconRenderer.render(model, icon);
								mIconRenderer.getRenderContext()->getSceneManager()->destroyMovableObject(model);
								
							}
						}
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
