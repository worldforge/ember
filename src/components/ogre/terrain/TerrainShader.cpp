#include "TerrainShader.h"
namespace EmberOgre {

TerrainShader::TerrainShader(Mercator::Terrain* terrain, int terrainIndex,  const Ogre::String textureName, Mercator::Shader* shader)
: mTextureName(textureName)
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
    textureUnitState->setTextureCoordSet(1);
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
    textureUnitState->setTextureCoordSet(1);
    
    return pass; 
//	textureUnitState->setColourOperationEx(LBX_BLEND_CURRENT_ALPHA, LBS_TEXTURE, LBS_CURRENT);
	
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
