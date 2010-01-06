/*-------------------------------------------------------------------------------------
Copyright (c) 2006 John Judnich
Modified 2008 by Erik Hjortsberg (erik.hjortsberg@gmail.com)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
	1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
	2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
	3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------------*/

#include "GrassLoader.h"
#include "PagedGeometry.h"
#include "PropertyMaps.h"

#include <OgreRoot.h>
#include <OgreTimer.h>
#include <OgreCamera.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreEntity.h>
#include <OgreString.h>
#include <OgreStringConverter.h>
#include <OgreMaterialManager.h>
#include <OgreMaterial.h>
#include <OgreHardwareBufferManager.h>
#include <OgreHardwareBuffer.h>
#include <OgreMeshManager.h>
#include <OgreMesh.h>
#include <OgreSubMesh.h>
#include <OgreLogManager.h>
#include <OgreTextureManager.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreRenderSystem.h>
#include <OgreRenderSystemCapabilities.h>
#include <OgreHighLevelGpuProgram.h>
#include <OgreHighLevelGpuProgramManager.h>
using namespace Ogre;


namespace Forests {

GrassLayer::GrassLayer(PagedGeometry *geom, GrassLoader<GrassLayer> *ldr)
{
	GrassLayer::geom = geom;
	GrassLayer::parent = ldr;

	density = 1.0f;
	minWidth = 1.0f; maxWidth = 1.0f;
	minHeight = 1.0f; maxHeight = 1.0f;
	minY = 0; maxY = 0;
	maxSlope = 1000;
	renderTechnique = GRASSTECH_QUAD;
	fadeTechnique = FADETECH_ALPHA;
	animMag = 1.0f;
	animSpeed = 1.0f;
	animFreq = 1.0f;
	waveCount = 0.0f;
	animate = false;
	blend = false;
	lighting = false;
	shaderNeedsUpdate = true;

	densityMap = NULL;
	densityMapFilter = MAPFILTER_BILINEAR;
	colorMap = NULL;
	colorMapFilter = MAPFILTER_BILINEAR;
}

GrassLayer::~GrassLayer()
{
	if (densityMap)
		densityMap->unload();
	if (colorMap)
		colorMap->unload();
}

unsigned int GrassLayer::prepareGrass(const PageInfo& page, float densityFactor, float volume)
{
	return density * densityFactor * volume;
}

unsigned int GrassLayer::_populateGrassList(PageInfo page, float *posBuff, unsigned int grassCount)
{
	if (densityMap){
		if (densityMap->getFilter() == MAPFILTER_NONE)
			return _populateGrassList_UnfilteredDM(page, posBuff, grassCount);
		else if (densityMap->getFilter() == MAPFILTER_BILINEAR)
			return _populateGrassList_BilinearDM(page, posBuff, grassCount);
	}
	return _populateGrassList_Uniform(page, posBuff, grassCount);

}

void GrassLayerBase::setMaterialName(const String &matName)
{
	if (material.isNull() || matName != material->getName()){
		material = MaterialManager::getSingleton().getByName(matName);
		if (material.isNull())
			OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "The specified grass material does not exist", "GrassLayer::setMaterialName()");
		shaderNeedsUpdate = true;
	}
}

void GrassLayerBase::setMinimumSize(float width, float height)
{
	minWidth = width;
	minHeight = height;
}

void GrassLayerBase::setMaximumSize(float width, float height)
{
	maxWidth = width;
	if (maxHeight != height){
		maxHeight = height;
		shaderNeedsUpdate = true;
	}
}

void GrassLayerBase::setRenderTechnique(GrassTechnique style, bool blendBase)
{
	if (blend != blendBase || renderTechnique != style){
		blend = blendBase;
		renderTechnique = style;
		shaderNeedsUpdate = true;
	}
}

void GrassLayerBase::setFadeTechnique(FadeTechnique style)
{
	if (fadeTechnique != style){
		fadeTechnique = style;
		shaderNeedsUpdate = true;
	}
}

void GrassLayerBase::setAnimationEnabled(bool enabled)
{
	if (animate != enabled){
		animate = enabled;
		shaderNeedsUpdate = true;
	}
}

void GrassLayerBase::setLightingEnabled(bool enabled)
{
	lighting = enabled;
}

void GrassLayer::setDensityMap(const String &mapFile, MapChannel channel)
{
	if (densityMap){
		densityMap->unload();
		densityMap = NULL;
	}
	if (mapFile != ""){
		densityMap = DensityMap::load(mapFile, channel);
		densityMap->setFilter(densityMapFilter);
	}
}
void GrassLayer::setDensityMap(TexturePtr map, MapChannel channel)
{
	if (densityMap){
		densityMap->unload();
		densityMap = NULL;
	}
	if (map.isNull() == false){
		densityMap = DensityMap::load(map, channel);
		densityMap->setFilter(densityMapFilter);
	}
}

void GrassLayer::setDensityMapFilter(MapFilter filter)
{
	densityMapFilter = filter;
	if (densityMap)
		densityMap->setFilter(densityMapFilter);
}

unsigned int GrassLayer::_populateGrassList_Uniform(PageInfo page, float *posBuff, unsigned int grassCount)
{
	float *posPtr = posBuff;

	//No density map
	if (!minY && !maxY){
		//No height range
		for (unsigned int i = 0; i < grassCount; ++i){
			//Pick a random position
			float x = Math::RangeRandom(page.bounds.left, page.bounds.right);
			float z = Math::RangeRandom(page.bounds.top, page.bounds.bottom);

			//Add to list in within bounds
			if (!colorMap){
				*posPtr++ = x;
				*posPtr++ = z;
			} else if (x >= mapBounds.left && x <= mapBounds.right && z >= mapBounds.top && z <= mapBounds.bottom){
				*posPtr++ = x;
				*posPtr++ = z;
			}
		}
	} else {
		//Height range
		Real min, max;
		if (minY) min = minY; else min = Math::NEG_INFINITY;
		if (maxY) max = maxY; else max = Math::POS_INFINITY;

		for (unsigned int i = 0; i < grassCount; ++i){
			//Pick a random position
			float x = Math::RangeRandom(page.bounds.left, page.bounds.right);
			float z = Math::RangeRandom(page.bounds.top, page.bounds.bottom);

			//Calculate height
			float y = parent->heightFunction(x, z, parent->heightFunctionUserData);

			//Add to list if in range
			if (y >= min && y <= max){
				//Add to list in within bounds
				if (!colorMap){
					*posPtr++ = x;
					*posPtr++ = z;
				} else if (x >= mapBounds.left && x <= mapBounds.right && z >= mapBounds.top && z <= mapBounds.bottom){
					*posPtr++ = x;
					*posPtr++ = z;
				}
			}
		}
	}

	grassCount = (posPtr - posBuff) / 2;
	return grassCount;
}

unsigned int GrassLayer::_populateGrassList_UnfilteredDM(PageInfo page, float *posBuff, unsigned int grassCount)
{
	float *posPtr = posBuff;

	//Use density map
	if (!minY && !maxY){
		//No height range
		for (unsigned int i = 0; i < grassCount; ++i){
			//Pick a random position
			float x = Math::RangeRandom(page.bounds.left, page.bounds.right);
			float z = Math::RangeRandom(page.bounds.top, page.bounds.bottom);

			//Determine whether this grass will be added based on the local density.
			//For example, if localDensity is .32, grasses will be added 32% of the time.
			if (Math::UnitRandom() < densityMap->_getDensityAt_Unfiltered(x, z, mapBounds)){
				//Add to list
				*posPtr++ = x;
				*posPtr++ = z;
			}
		}
	} else {
		//Height range
		Real min, max;
		if (minY) min = minY; else min = Math::NEG_INFINITY;
		if (maxY) max = maxY; else max = Math::POS_INFINITY;

		for (unsigned int i = 0; i < grassCount; ++i){
			//Pick a random position
			float x = Math::RangeRandom(page.bounds.left, page.bounds.right);
			float z = Math::RangeRandom(page.bounds.top, page.bounds.bottom);

			//Determine whether this grass will be added based on the local density.
			//For example, if localDensity is .32, grasses will be added 32% of the time.
			if (Math::UnitRandom() < densityMap->_getDensityAt_Unfiltered(x, z, mapBounds)){
				//Calculate height
				float y = parent->heightFunction(x, z, parent->heightFunctionUserData);

				//Add to list if in range
				if (y >= min && y <= max){
					//Add to list
					*posPtr++ = x;
					*posPtr++ = z;
				}
			}
		}
	}

	grassCount = (posPtr - posBuff) / 2;
	return grassCount;
}

unsigned int GrassLayer::_populateGrassList_BilinearDM(PageInfo page, float *posBuff, unsigned int grassCount)
{
	float *posPtr = posBuff;

	if (!minY && !maxY){
		//No height range
		for (unsigned int i = 0; i < grassCount; ++i){
			//Pick a random position
			float x = Math::RangeRandom(page.bounds.left, page.bounds.right);
			float z = Math::RangeRandom(page.bounds.top, page.bounds.bottom);

			//Determine whether this grass will be added based on the local density.
			//For example, if localDensity is .32, grasses will be added 32% of the time.
			if (Math::UnitRandom() < densityMap->_getDensityAt_Bilinear(x, z, mapBounds)){
				//Add to list
				*posPtr++ = x;
				*posPtr++ = z;
			}
		}
	} else {
		//Height range
		Real min, max;
		if (minY) min = minY; else min = Math::NEG_INFINITY;
		if (maxY) max = maxY; else max = Math::POS_INFINITY;

		for (unsigned int i = 0; i < grassCount; ++i){
			//Pick a random position
			float x = Math::RangeRandom(page.bounds.left, page.bounds.right);
			float z = Math::RangeRandom(page.bounds.top, page.bounds.bottom);

			//Determine whether this grass will be added based on the local density.
			//For example, if localDensity is .32, grasses will be added 32% of the time.
			if (Math::UnitRandom() < densityMap->_getDensityAt_Bilinear(x, z, mapBounds)){
				//Calculate height
				float y = parent->heightFunction(x, z, parent->heightFunctionUserData);

				//Add to list if in range
				if (y >= min && y <= max){
					//Add to list
					*posPtr++ = x;
					*posPtr++ = z;
				}
			}
		}
	}

	grassCount = (posPtr - posBuff) / 2;
	return grassCount;
}

void GrassLayer::setColorMap(const String &mapFile, MapChannel channel)
{
	if (colorMap){
		colorMap->unload();
		colorMap = NULL;
	}
	if (mapFile != ""){
		colorMap = ColorMap::load(mapFile, channel);
		colorMap->setFilter(colorMapFilter);
	}
}

void GrassLayer::setColorMap(TexturePtr map, MapChannel channel)
{
	if (colorMap){
		colorMap->unload();
		colorMap = NULL;
	}
	if (map.isNull() == false){
		colorMap = ColorMap::load(map, channel);
		colorMap->setFilter(colorMapFilter);
	}
}

void GrassLayer::setColorMapFilter(MapFilter filter)
{
	colorMapFilter = filter;
	if (colorMap)
		colorMap->setFilter(colorMapFilter);
}

void GrassLayerBase::_updateShaders()
{
	if (shaderNeedsUpdate){
		shaderNeedsUpdate = false;

		//Proceed only if there is no custom vertex shader and the user's computer supports vertex shaders
		const RenderSystemCapabilities *caps = Root::getSingleton().getRenderSystem()->getCapabilities();
		if (caps->hasCapability(RSC_VERTEX_PROGRAM) && geom->getShadersEnabled())
		{
			//Calculate fade range
			float farViewDist = geom->getDetailLevels().front()->getFarRange();
			float fadeRange = farViewDist / 1.2247449f;
			//Note: 1.2247449 ~= sqrt(1.5), which is necessary since the far view distance is measured from the centers
			//of pages, while the vertex shader needs to fade grass completely out (including the closest corner)
			//before the page center is out of range.

			//Generate a string ID that identifies the current set of vertex shader options
			StringUtil::StrStreamType tmpName;
			tmpName << "GrassVS_";
			if (animate)
				tmpName << "anim_";
			if (blend)
				tmpName << "blend_";
			if (lighting)
				tmpName << "lighting_";
			tmpName << renderTechnique << "_";
			tmpName << fadeTechnique << "_";
			if (fadeTechnique == FADETECH_GROW || fadeTechnique == FADETECH_ALPHAGROW)
				tmpName << maxHeight << "_";
			tmpName << farViewDist << "_";
			tmpName << "vp";
			const String vsName = tmpName.str();

			//Generate a string ID that identifies the material combined with the vertex shader
			const String matName = material->getName() + "_" + vsName;

			//Check if the desired material already exists (if not, create it)
			MaterialPtr tmpMat = MaterialManager::getSingleton().getByName(matName);
			if (tmpMat.isNull())
			{
				//Clone the original material
				tmpMat = material->clone(matName);

				//Disable lighting
				tmpMat->setLightingEnabled(false);
				//tmpMat->setReceiveShadows(false);

				//Check if the desired shader already exists (if not, compile it)
				String shaderLanguage;
				HighLevelGpuProgramPtr vertexShader = HighLevelGpuProgramManager::getSingleton().getByName(vsName);
				if (vertexShader.isNull())
				{
					if (Root::getSingleton().getRenderSystem()->getName() == "Direct3D9 Rendering Subsystem")
						shaderLanguage = "hlsl";
					else if(Root::getSingleton().getRenderSystem()->getName() == "OpenGL Rendering Subsystem")
						shaderLanguage = "glsl";
					else
						shaderLanguage = "cg";

					//Generate the grass shader
					String vertexProgSource;

					if(!shaderLanguage.compare("hlsl") || !shaderLanguage.compare("cg"))
					{

						vertexProgSource =
							"void main( \n"
							"	float4 iPosition : POSITION, \n"
							"	float4 iColor : COLOR, \n"
							"	float2 iUV       : TEXCOORD0,	\n"
							"	out float4 oPosition : POSITION, \n"
							"	out float4 oColor : COLOR, \n"
							"	out float2 oUV       : TEXCOORD0,	\n";

						if (lighting) vertexProgSource +=
							"   uniform float4   objSpaceLight,   \n"
							"   uniform float4   lightDiffuse,   \n"
							"   uniform float4   lightAmbient,   \n";

						if (animate) vertexProgSource +=
							"	uniform float time,	\n"
							"	uniform float frequency,	\n"
							"	uniform float4 direction,	\n";

						if (fadeTechnique == FADETECH_GROW || fadeTechnique == FADETECH_ALPHAGROW) vertexProgSource +=
							"	uniform float grassHeight,	\n";

						if (renderTechnique == GRASSTECH_SPRITE || lighting) vertexProgSource +=
							"   float4 iNormal : NORMAL, \n";

						vertexProgSource +=
							"	uniform float4x4 worldViewProj,	\n"
							"	uniform float3 camPos, \n"
							"	uniform float fadeRange ) \n"
							"{	\n"
							"	oColor.rgb = iColor.rgb;   \n"
							"	float4 position = iPosition;	\n"
							"	float dist = distance(camPos.xz, position.xz);	\n";

						if (lighting)
						{
							vertexProgSource +=
							"   float3 light = normalize(objSpaceLight.xyz - (iPosition.xyz * objSpaceLight.w)); \n"
							"   float diffuseFactor = max(dot(float4(0,1,0,0), light), 0); \n"
							"   oColor = (lightAmbient + diffuseFactor * lightDiffuse) * iColor; \n";
						}
						else
						{
							vertexProgSource +=
							"   oColor.rgb = iColor.rgb;               \n";
						}

						if (fadeTechnique == FADETECH_ALPHA || fadeTechnique == FADETECH_ALPHAGROW) vertexProgSource +=
							//Fade out in the distance
							"	oColor.a = 2.0f - (2.0f * dist / fadeRange);   \n";
						else vertexProgSource +=
							"	oColor.a = 1.0f;   \n";

						vertexProgSource +=
							"	float oldposx = position.x;	\n";

						if (renderTechnique == GRASSTECH_SPRITE) vertexProgSource +=
							//Face the camera
							"	float3 dirVec = (float3)position - (float3)camPos;		\n"
							"	float3 p = normalize(cross(float4(0,1,0,0), dirVec));	\n"
							"	position += float4(p.x * iNormal.x, iNormal.y, p.z * iNormal.x, 0);	\n";

						if (animate) vertexProgSource +=
							"	if (iUV.y == 0.0f){	\n"
							//Wave grass in breeze
							"		float offset = sin(time + oldposx * frequency);	\n"
							"		position += direction * offset;	\n"
							"	}	\n";

						if (blend && animate) vertexProgSource +=
							"	else {	\n";
						else if (blend) vertexProgSource +=
							"	if (iUV.y != 0.0f){	\n";

						if (blend) vertexProgSource +=
							//Blend the base of nearby grass into the terrain
							"		oColor.a = clamp(oColor.a, 0, 1) * 4.0f * ((dist / fadeRange) - 0.1f);	\n"
							"	}	\n";

						if (fadeTechnique == FADETECH_GROW || fadeTechnique == FADETECH_ALPHAGROW) vertexProgSource +=
							"	float offset = (2.0f * dist / fadeRange) - 1.0f; \n"
							"	position.y -= grassHeight * clamp(offset, 0, 1); ";

						vertexProgSource +=
							"	oPosition = mul(worldViewProj, position);  \n";

						vertexProgSource +=
							"	oUV = iUV;\n"
							"}";
					}
					else
					{
						//Must be glsl
						if (lighting)
						{
							vertexProgSource =
							"uniform vec4 objSpaceLight; \n"
							"uniform vec4 lightDiffuse; \n"
							"uniform vec4 lightAmbient; \n";
						}

						if (animate)
						{
							vertexProgSource +=
							"uniform float time; \n"
							"uniform float frequency; \n"
							"uniform vec4 direction; \n";
						}

						if (fadeTechnique == FADETECH_GROW || fadeTechnique == FADETECH_ALPHAGROW)
						{
							vertexProgSource +=
							"uniform float grassHeight;	\n";
						}

						vertexProgSource +=
							"uniform vec3 camPos; \n"
							"uniform float fadeRange; \n"
							"\n"
							"void main()"
							"{ \n"
							"    vec4 color = gl_Color; \n"
							"    vec4 position = gl_Vertex;	\n"
							"    float dist = distance(camPos.xz, position.xz);	\n";

						if (lighting)
						{
							vertexProgSource +=
							"    vec3 light = normalize(objSpaceLight.xyz - (gl_Vertex.xyz * objSpaceLight.w)); \n"
							"    float diffuseFactor = max( dot( vec3(0.0,1.0,0.0), light), 0.0); \n"
							"    color = (lightAmbient + diffuseFactor * lightDiffuse) * gl_Color; \n";
						}
						else
						{
							vertexProgSource +=
							"    color.xyz = gl_Color.xyz; \n";
						}

						if (fadeTechnique == FADETECH_ALPHA || fadeTechnique == FADETECH_ALPHAGROW)
						{
							vertexProgSource +=
							//Fade out in the distance
							"    color.w = 2.0 - (2.0 * dist / fadeRange); \n";
						}
						else
						{
							vertexProgSource +=
							"    color.w = 1.0; \n";
						}

						if (renderTechnique == GRASSTECH_SPRITE)
						{
							vertexProgSource +=
							//Face the camera
							"    vec3 dirVec = position.xyz - camPos.xyz; \n"
							"    vec3 p = normalize(cross(vec3(0.0,1.0,0.0), dirVec)); \n"
							"    position += vec4(p.x * gl_Normal.x, gl_Normal.y, p.z * gl_Normal.x, 0.0); \n";
						}

						if (animate)
						{
							vertexProgSource +=
							"    if (gl_MultiTexCoord0.y == 0.0) \n"
							"    { \n"
							//Wave grass in breeze
							"        position += direction * sin(time + gl_Vertex.x * frequency); \n"
							"    } \n";
						}

						if (blend && animate)
						{
							vertexProgSource +=
							"    else \n"
							"    { \n";
						}
						else if (blend)
						{
							vertexProgSource +=
							"    if (gl_MultiTexCoord0.y != 0.0) \n"
							"    { \n";
						}

						if (blend)
						{
							vertexProgSource +=
							//Blend the base of nearby grass into the terrain
							"        color.w = clamp(color.w, 0.0, 1.0) * 4.0 * ((dist / fadeRange) - 0.1); \n"
							"    } \n";
						}

						if (fadeTechnique == FADETECH_GROW || fadeTechnique == FADETECH_ALPHAGROW)
						{
							vertexProgSource +=
							"    position.y -= grassHeight * clamp((2.0 * dist / fadeRange) - 1.0, 0.0, 1.0); \n";
						}

						vertexProgSource +=
						"    gl_Position = gl_ModelViewProjectionMatrix * position; \n"
						"    gl_FrontColor = color; \n"
						"    gl_TexCoord[0] = gl_MultiTexCoord0; \n";
						if (geom->getSceneManager()->getFogMode() == Ogre::FOG_EXP2) {
							vertexProgSource +=
								"	gl_FogFragCoord = clamp(exp(- gl_Fog.density * gl_Fog.density * gl_Position.z * gl_Position.z), 0.0, 1.0); \n";
						} else {
							vertexProgSource +=
								"	gl_FogFragCoord = gl_Position.z; \n";
						}

						vertexProgSource +=	"}";
					}

					vertexShader = HighLevelGpuProgramManager::getSingleton().createProgram(
						vsName,
						ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
						shaderLanguage, GPT_VERTEX_PROGRAM);

					vertexShader->setSource(vertexProgSource);

					if (shaderLanguage == "hlsl")
					{
						vertexShader->setParameter("target", "vs_1_1");
						vertexShader->setParameter("entry_point", "main");
					}
					else if(shaderLanguage == "cg")
					{
						vertexShader->setParameter("profiles", "vs_1_1 arbvp1");
						vertexShader->setParameter("entry_point", "main");
					}
					// GLSL can only have one entry point "main".

					vertexShader->load();
				}
				//Now the vertex shader (vertexShader) has either been found or just generated
				//(depending on whether or not it was already generated).

				//Apply the shader to the material
				Pass *pass = tmpMat->getTechnique(0)->getPass(0);
				pass->setVertexProgram(vsName);
				GpuProgramParametersSharedPtr params = pass->getVertexProgramParameters();

				if(shaderLanguage.compare("glsl"))
					//glsl can use the built in gl_ModelViewProjectionMatrix
					params->setNamedAutoConstant("worldViewProj", GpuProgramParameters::ACT_WORLDVIEWPROJ_MATRIX);
				params->setNamedAutoConstant("camPos", GpuProgramParameters::ACT_CAMERA_POSITION_OBJECT_SPACE);
				params->setNamedAutoConstant("fadeRange", GpuProgramParameters::ACT_CUSTOM, 1);

				if (animate){
					params->setNamedAutoConstant("time", GpuProgramParameters::ACT_CUSTOM, 1);
					params->setNamedAutoConstant("frequency", GpuProgramParameters::ACT_CUSTOM, 1);
					params->setNamedAutoConstant("direction", GpuProgramParameters::ACT_CUSTOM, 4);
				}

				if (lighting){
					params->setNamedAutoConstant("objSpaceLight", GpuProgramParameters::ACT_LIGHT_POSITION_OBJECT_SPACE);
					params->setNamedAutoConstant("lightDiffuse", GpuProgramParameters::ACT_DERIVED_LIGHT_DIFFUSE_COLOUR);
					params->setNamedAutoConstant("lightAmbient", GpuProgramParameters::ACT_DERIVED_AMBIENT_LIGHT_COLOUR);
				}

				if (fadeTechnique == FADETECH_GROW || fadeTechnique == FADETECH_ALPHAGROW){
					params->setNamedAutoConstant("grassHeight", GpuProgramParameters::ACT_CUSTOM, 1);
					params->setNamedConstant("grassHeight", maxHeight * 1.05f);
				}

				pass->getVertexProgramParameters()->setNamedConstant("fadeRange", fadeRange);
			}
			//Now the material (tmpMat) has either been found or just created (depending on whether or not it was already
			//created). The appropriate vertex shader should be applied and the material is ready for use.

			//Apply the new material
			material = tmpMat;
		}
	}
}


unsigned long GrassPage::GUID = 0;

void GrassPage::init(PagedGeometry *geom, const Ogre::Any &data)
{
	sceneMgr = geom->getSceneManager();
	rootNode = geom->getSceneNode();
}

GrassPage::~GrassPage()
{
	removeEntities();
}

void GrassPage::addEntity(Entity *entity, const Vector3 &position, const Quaternion &rotation, const Vector3 &scale, const Ogre::ColourValue &color)
{
	SceneNode *node = rootNode->createChildSceneNode();
	node->setPosition(position);
	nodeList.push_back(node);

	Entity *ent = entity->clone(getUniqueID());
	ent->setCastShadows(false);
	if(hasQueryFlag())
		ent->setQueryFlags(getQueryFlag());
	ent->setRenderQueueGroup(entity->getRenderQueueGroup());
	node->attachObject(ent);
}

void GrassPage::removeEntities()
{
	std::list<SceneNode*>::iterator i;
	for (i = nodeList.begin(); i != nodeList.end(); ++i){
		SceneNode *node = *i;
		sceneMgr->destroyEntity(static_cast<Entity*>(node->getAttachedObject(0)));
		sceneMgr->destroySceneNode(node->getName());
	}
	nodeList.clear();
}

void GrassPage::setVisible(bool visible)
{
	std::list<SceneNode*>::iterator i;
	for (i = nodeList.begin(); i != nodeList.end(); ++i){
		SceneNode *node = *i;
		node->setVisible(visible);
	}
}


}
