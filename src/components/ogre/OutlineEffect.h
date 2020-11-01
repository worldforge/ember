/*
 Copyright (C) 2020 Erik Ogenvik

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

#ifndef EMBER_OUTLINEEFFECT_H
#define EMBER_OUTLINEEFFECT_H

#include "domain/EmberEntityRef.h"
#include "components/ogre/model/Model.h"
#include <OgreVector.h>
#include <memory>

namespace Ember {
namespace OgreView {
class Scene;

struct StencilOpQueueListener;

/**
 * Shows an outline effect around an entity (mainly the model used to display).
 *
 * This is used for showing the currently picked entity.
 */
class OutlineEffect {
public:
	explicit OutlineEffect(Scene& scene, EmberEntity& entity);

	~OutlineEffect();

	const EmberEntityRef& getEntity() const {
		return mSelectedEntity;
	}

protected:

	Scene& mScene;
	EmberEntityRef mSelectedEntity;
	std::unique_ptr<StencilOpQueueListener> mStencilOpQueueListener;
	
	/**
	 * Keeps track of the entities involved in the "outline".
	 */
	struct Outline {
		/**
		 * The entity which was selected.
		 */

		/**
		 * Any generated Ogre Entities used for the outline.
		 *
		 * If the selected entity is represented by InstancedEntities we must generate Ogre::Entities and render these for the outline.
		 *
		 */
		std::vector<Ogre::Entity*> generatedEntities;

		/**
		 * Any materials generated for the outline.
		 */
		std::vector<Ogre::MaterialPtr> generatedMaterials;

		/**
		 * The original render queue groups used for the selected entities.
		 */
		std::vector<std::uint8_t> originalRenderQueueGroups;

	};

	/**
	 * An outline shown for selected entities.
	 */
	Outline mOutline;
};
}
}

#endif //EMBER_OUTLINEEFFECT_H
