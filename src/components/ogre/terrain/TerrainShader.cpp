#include "TerrainShader.h"
#include <OgreIteratorWrappers.h>
namespace EmberOgre {

TerrainShader::TerrainShader(Mercator::Terrain* terrain, int terrainIndex,  const Ogre::String textureName, Mercator::Shader* shader)
: mTextureName(textureName)
, mShader(shader)
, mTerrain(terrain)
, mTerrainIndex(terrainIndex)
{

	mTerrain->addShader(shader, mTerrainIndex);

}


TerrainShader::TerrainShader(Mercator::Terrain* terrain, int terrainIndex,  Ogre::MaterialPtr material, Mercator::Shader* shader)
: mTextureName("")
, mMaterial(material)
, mShader(shader)
, mTerrain(terrain)
, mTerrainIndex(terrainIndex)
{

	mTerrain->addShader(shader, mTerrainIndex);

}

TerrainShader::~TerrainShader()
{}

Mercator::Shader* TerrainShader::getShader() const
{
	return mShader;
}

const Ogre::String TerrainShader::getTextureName() const
{
	return mTextureName;
}

void TerrainShader::addTextureUnitsToPass(Ogre::Pass* pass, Ogre::String splatTextureName) {
	Ogre::TextureUnitState * textureUnitStateSplat = pass->createTextureUnitState();
    textureUnitStateSplat->setTextureName(splatTextureName);
     
    textureUnitStateSplat->setTextureCoordSet(0);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
	textureUnitStateSplat->setAlphaOperation(Ogre::LBX_SOURCE1, Ogre::LBS_TEXTURE, Ogre::LBS_TEXTURE);
	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_SOURCE1, Ogre::LBS_CURRENT, Ogre::LBS_CURRENT);

	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
    textureUnitState->setTextureName(mTextureName);
    textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
	textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureScale(0.01, 0.01);
	textureUnitState->setColourOperationEx(Ogre::LBX_BLEND_CURRENT_ALPHA, Ogre::LBS_TEXTURE, Ogre::LBS_CURRENT);
	
}

Ogre::Pass* TerrainShader::addPassToTechnique(Ogre::Technique* technique, Ogre::String splatTextureName) {
	Ogre::Pass* pass = technique->createPass();
	pass->setSceneBlending(Ogre::SBT_TRANSPARENT_ALPHA);
	//pass->setLightingEnabled(false);
	
	
	Ogre::TextureUnitState * textureUnitStateSplat = pass->createTextureUnitState();
    textureUnitStateSplat->setTextureName(splatTextureName);
     
    textureUnitStateSplat->setTextureCoordSet(0);
//	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
    textureUnitStateSplat->setTextureAddressingMode(Ogre::TextureUnitState::TAM_CLAMP);
	textureUnitStateSplat->setTextureFiltering(Ogre::TFO_ANISOTROPIC);
//	textureUnitStateSplat->setAlphaOperation(LBX_SOURCE1, LBS_TEXTURE, LBS_TEXTURE);
	textureUnitStateSplat->setColourOperationEx(Ogre::LBX_BLEND_DIFFUSE_ALPHA, Ogre::LBS_CURRENT, Ogre::LBS_TEXTURE);

	Ogre::TextureUnitState * textureUnitState = pass->createTextureUnitState();
    textureUnitState->setTextureName(mTextureName);
    textureUnitState->setTextureAddressingMode(Ogre::TextureUnitState::TAM_WRAP);
    textureUnitState->setTextureCoordSet(0);
	textureUnitState->setTextureScale(0.01, 0.01);

    
    return pass; 
//	textureUnitState->setColourOperationEx(LBX_BLEND_CURRENT_ALPHA, LBS_TEXTURE, LBS_CURRENT);
	
}

void TerrainShader::addMaterialToTechnique(Ogre::Technique*  technique, Ogre::String splatTextureName) {

	if (!mMaterial->getNumSupportedTechniques()) {
		return;
	}
	Ogre::Technique* sourceTech = mMaterial->getSupportedTechnique(0);
	Ogre::Technique::PassIterator I = sourceTech->getPassIterator();
	while (I.hasMoreElements()) {
		Ogre::Pass* sourcePass = I.getNext();
		Ogre::Pass* destPass = technique->createPass();
		//just make a copy of the source pass
		*destPass = *sourcePass;
		
		//now iterate through all textures and look for the splat
		Ogre::Pass::TextureUnitStateIterator J = destPass->getTextureUnitStateIterator();
		while (J.hasMoreElements()) {
			Ogre::TextureUnitState* texUnitState = J.getNext();
			if (texUnitState->getTextureName() == "splat") {
				texUnitState->setTextureName(splatTextureName);
			}
		}
	}
}

void TerrainShader::addSplatToTechnique(Ogre::Technique*  technique, Ogre::String splatTextureName) {
	if (!mMaterial.isNull()) {
		addMaterialToTechnique(technique, splatTextureName);
	} else {
		Ogre::Pass* pass = addPassToTechnique(technique, splatTextureName);
		pass->setLightingEnabled(false);
		pass->setSelfIllumination(Ogre::ColourValue(1,1,1));
	}
}


Mercator::Surface* TerrainShader::getSurfaceForSegment(Mercator::Segment* segment) const
{

	Mercator::Surface* surface = 0;
	if (segment->getSurfaces().find(getTerrainIndex()) != segment->getSurfaces().end()) {
		surface = segment->getSurfaces().find(getTerrainIndex())->second;
	}
	return surface;
}


}
