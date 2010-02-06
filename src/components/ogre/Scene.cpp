/*
 Copyright (C) 2010 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Scene.h"
#include "ISceneRenderingTechnique.h"
#include "framework/LoggingInstance.h"

namespace EmberOgre
{

Scene::Scene(Ogre::SceneManager& sceneManager) :
	mSceneManager(sceneManager)
{
}

Scene::~Scene()
{
	if (mTechniques.size() > 0) {
		S_LOG_WARNING("Scene was deleted while there still was registered techniques.");
	}
}

Ogre::SceneManager& Scene::getSceneManager()
{
	return mSceneManager;
}

void Scene::registerEntityWithTechnique(EmberEntity& entity, const std::string& technique)
{
	RenderingTechniqueStore::const_iterator I = mTechniques.find(technique);
	if (I != mTechniques.end()) {
		I->second->registerEntity(entity);
	}
}

void Scene::addRenderingTechnique(const std::string& name, ISceneRenderingTechnique* technique)
{
	if (mTechniques.count(name) == 0) {
		mTechniques.insert(RenderingTechniqueStore::value_type(name, technique));
	}
}

ISceneRenderingTechnique* Scene::removeRenderingTechnique(const std::string& name)
{
	RenderingTechniqueStore::iterator I = mTechniques.find(name);
	if (I != mTechniques.end()) {
		ISceneRenderingTechnique* technique = I->second;
		mTechniques.erase(I);
		return technique;
	}
	return 0;
}

}
