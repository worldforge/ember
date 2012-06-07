#include "AutoGraphicsLevelManager.h"
#include "FoliageLevelManager.h"

#include "World.h"

#include "EmberOgre.h"

#include "components/ogre/terrain/TerrainLayerDefinition.h"
#include "components/ogre/terrain/TerrainLayerDefinitionManager.h"
#include "ogreopcode/include/Opcode/Ice/IceTypes.h"

#include "sstream"
#include "cmath"

namespace Ember {
namespace OgreView {

FoliageLevelManager::FoliageLevelManager(AutomaticGraphicsLevelManager &automaticGraphicsLevelManager) :
	mThresholdLevel(1), mDefaultDensityStep(0.2)
{
	automaticGraphicsLevelManager.getGraphicalAdapter()->changeRequired.connect(sigc::ptr_fun(&changeLevel));
	
	const Terrain::TerrainLayerDefinitionManager::DefinitionStore defaultDefinitionStore = Terrain::TerrainLayerDefinitionManager.getSingleton().getDefinitions();
	
	//constructs the defaultDensityStore from the terrain layer definitions
	for(Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = defaultDefinitionStore.begin();I != defaultDefinitionStore.end(); ++I) {
		Terrain::TerrainLayerDefinition* currentDefinition = *I;
		for (Terrain::TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator J = currentDefinition->getFoliages().begin(); J != currentDefinition->getFoliages().end(); ++J) {
			mDefaultDensityStore.insert(std::make_pair(J->getPlantType() , atof(J->getParameter("density").c_str() ) ) );
			
		}
	}
	
	mUpdatedtDensityStore = mDefaultDensityStore;
}

FoliageLevelManager::~FoliageLevelManager()
{
	
}


void FoliageLevelManager::updateFoliageDensity()
{
	const Terrain::TerrainLayerDefinitionManager::DefinitionStore defaultDefinitionStore = Terrain::TerrainLayerDefinitionManager.getSingleton().getDefinitions();
	
	std::map <std::string,float>::const_iterator UpdatedDensityStoreIterator = mUpdatedtDensityStore.begin();
	
	//Updating density parameters of the definitions store at TerrainLayerDefinitionManager
	for(Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = defaultDefinitionStore.begin();I != defaultDefinitionStore.end() && UpdatedDensityStoreIterator != mUpdatedtDensityStore.end(); ++I) {
		Terrain::TerrainLayerDefinition* currentDefinition = *I;
		for (Terrain::TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator J = currentDefinition->getFoliages().begin(); J != currentDefinition->getFoliages().end() && UpdatedDensityStoreIterator != mUpdatedtDensityStore.end(); ++J) {
			if(J->getPlantType() == UpdatedDensityStoreIterator->first) {
				Terrain::TerrainLayerDefinition::StringParamStore currentParameters = J->getParameters();
				std::stringstream ss;
				ss << UpdatedDensityStoreIterator->second;
				
				//safe to use [] since we know the parameter exists
				currentParameters["density"] = ss.str();
			}
		}
	}
	
	EmberOgre::getSingleton().getWorld()->reloadFoliage();
}

bool FoliageLevelManager::changeLevel(float level)
{
	if(std::abs(level) < mThresholdLevel){
		return false;
	}
	
	const Terrain::TerrainLayerDefinitionManager::DefinitionStore defaultDefinitionStore = Terrain::TerrainLayerDefinitionManager.getSingleton().getDefinitions();
	
	//holds whether a change in density was made after iterating through all the foliages
	bool changeMade = false;
	
	//decreasing foliage density since more fps is required
	if(level > 0) {
		for(Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = defaultDefinitionStore.begin();I != defaultDefinitionStore.end(); ++I) {
			Terrain::TerrainLayerDefinition* currentDefinition = *I;
			for (Terrain::TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator J = currentDefinition->getFoliages().begin(); J != currentDefinition->getFoliages().end(); ++J) {
				changeMade = changeMade || stepDownFoliageDensity(J->getPlantType(), mDefaultDensityStep);
			}
		}
	}
	
	//increasing foliage density since less fps is required
	if(level < 0){
		for(Terrain::TerrainLayerDefinitionManager::DefinitionStore::const_iterator I = defaultDefinitionStore.begin();I != defaultDefinitionStore.end(); ++I) {
			Terrain::TerrainLayerDefinition* currentDefinition = *I;
			for (Terrain::TerrainLayerDefinition::TerrainFoliageDefinitionStore::const_iterator J = currentDefinition->getFoliages().begin(); J != currentDefinition->getFoliages().end(); ++J) {
				changeMade = changeMade || stepUpFoliageDensity(J->getPlantType(), mDefaultDensityStep);
			}
		}
	}
	
	return changeMade;
	
}

bool FoliageLevelManager::stepDownFoliageDensity(std::string foliage, float step)
{
	std::map<std::string,float>::const_iterator I = mUpdatedtDensityStore.find(foliage);
	
	if(I != mUpdatedtDensityStore.end()) {
		//step down only if existing density is greater than step
		if(I->second > step) {
			I->second = I->second - step;
			return true;
		}
		
		//if there is still some positive density left which is smaller than step, set it to 0
		else if(I->second < step && I->second >0) {
			I->second = 0;
			return true;
		}
		
		//step down not possible for this foliage
		else {
			return false;
		}
	}
	
	//foliage not found
	else {
		return false;
	}
}

bool FoliageLevelManager::stepUpFoliageDensity(std::string foliage, float step)
{
	std::map<std::string,float>::const_iterator I = mUpdatedtDensityStore.find(foliage);
	
	//default density of the foliage object that acts as the upper limit for foliage density
	float defaultDensity = mDefaultDensityStore.find(foliage)->second;
	
	if(I != mUpdatedtDensityStore.end()) {
		
		//step up only if the step doesn't cause density to go over default density
		if(defaultDensity - I->second > step) {
			I->second = I->second + step;
			return true;
		}
		
		//if the density is still below default density but a default step causes it to go over default density
		else if(defaultDensity - I->second > 0 && I->second < defaultDensity) {
			I->second = defaultDensity;
			return true;
		}
		
		//step up not possible for this foliage
		else {
			return false;
		}
	}
	
	//foliage not found
	else {
		return false;
	}
}

bool FoliageLevelManager::changeFoliageDensity(std::string foliage, float density)
{
	std::map<std::string,float>::const_iterator I = mUpdatedtDensityStore.find(foliage);
	
	if(I != mUpdatedtDensityStore.end()) {
		I->second = density;
		return true;
	}
	
	//foliage not found
	else {
		return false;
	}
}

float FoliageLevelManager::findDefaultDensity(std::string plantType)
{
	mDefaultDensityStore.find(plantType)->second;
}

	
}
}