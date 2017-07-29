//
// C++ Implementation: Foliage
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
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

#include "Foliage.h"
#include "FoliageBase.h"
#include "GrassFoliage.h"
#include "ShrubberyFoliage.h"

#include "../terrain/TerrainLayerDefinition.h"
#include "../terrain/TerrainLayerDefinitionManager.h"

#include "framework/Tokeniser.h"

#include <wfmath/point.h>

template<> Ember::OgreView::Environment::Foliage* Ember::Singleton<Ember::OgreView::Environment::Foliage>::ms_Singleton = 0;
using namespace Ember::OgreView::Terrain;

namespace Ember
{
namespace OgreView
{

namespace Environment
{

Foliage::Foliage(Terrain::TerrainManager& terrainManager) :
	ReloadFoliage("reloadfoliage", this, ""), mTerrainManager(terrainManager)
{
	Ogre::Root::getSingleton().addFrameListener(this);
}

Foliage::~Foliage()
{
	S_LOG_INFO("Shutting down foliage system.");
	
	for (FoliageStore::iterator I = mFoliages.begin(); I != mFoliages.end(); ++I) {
		delete *I;
	}

	Ogre::Root::getSingleton().removeFrameListener(this);
}

void Foliage::initialize()
{
	S_LOG_INFO("Initializing foliage system.");
	for (TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = TerrainLayerDefinitionManager::getSingleton().getDefinitions().begin(); I != TerrainLayerDefinitionManager::getSingleton().getDefinitions().end(); ++I) {
		const TerrainLayerDefinition* layerDef = *I;
		for (TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator J = layerDef->getFoliages().begin(); J != layerDef->getFoliages().end(); ++J) {
			FoliageBase* foliageBase(0);
			try {
				if (J->getRenderTechnique() == "grass") {
					foliageBase = new GrassFoliage(mTerrainManager, *layerDef, *J);
				} else if (J->getRenderTechnique() == "shrubbery") {
					foliageBase = new ShrubberyFoliage(mTerrainManager, *layerDef, *J);
				}
				if (foliageBase) {
					foliageBase->initialize();
					mFoliages.push_back(foliageBase);
				}
			} catch (const std::exception& ex) {
				S_LOG_FAILURE("Error when creating foliage." << ex);
				try {
					delete foliageBase;
				} catch (const std::exception& innerEx) {
					S_LOG_FAILURE("Even got an error when deleting the foliage, things could get ugly from here on." << innerEx);
				}
			}
		}
	}
}

void Foliage::runCommand(const std::string &command, const std::string &args)
{
	if (ReloadFoliage == command) {
		Tokeniser tokeniser(args);
		std::string xString = tokeniser.nextToken();
		std::string yString = tokeniser.nextToken();

		reloadAtPosition(WFMath::Point<2>(atof(xString.c_str()), atof(yString.c_str())));
	}
}

void Foliage::reloadAtPosition(const WFMath::Point<2>& worldPosition)
{
	for (FoliageStore::iterator I = mFoliages.begin(); I != mFoliages.end(); ++I) {
		(*I)->reloadAtPosition(worldPosition);
	}
}

bool Foliage::frameStarted(const Ogre::FrameEvent&)
{
	for (FoliageStore::iterator I = mFoliages.begin(); I != mFoliages.end(); ++I) {
		(*I)->frameStarted();
	}

	return true;
}

void Foliage::setDensity(float newDensity)
{
	for (FoliageStore::iterator I = mFoliages.begin(); I != mFoliages.end(); ++I) {
		(*I)->setDensity(newDensity);
	}
}

void Foliage::setFarDistance(float newFarDistance)
{
	for (FoliageStore::iterator I = mFoliages.begin(); I != mFoliages.end(); ++I) {
		(*I)->setFarDistance(newFarDistance);
	}
}

}
}
}

