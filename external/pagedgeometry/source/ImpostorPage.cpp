/*-------------------------------------------------------------------------------------
Copyright (c) 2006 John Judnich
Modified 2008 by Erik Ogenvik (erik@ogenvik.org)

This software is provided 'as-is', without any express or implied warranty. In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
    1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
    2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
    3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------------*/

//ImpostorPage.cpp
//ImposterPage is an extension to PagedGeometry which displays entities as imposters.
//-------------------------------------------------------------------------------------

#include "ImpostorPage.h"
#include "StaticBillboardSet.h"
#include "components/ogre/SceneNodeProvider.h"
#include "components/ogre/model/SubModel.h"

#include <OgreRoot.h>
#include <OgreTimer.h>
#include <OgreCamera.h>
#include <OgreVector.h>
#include <OgreQuaternion.h>
#include <OgreEntity.h>
#include <OgreSubEntity.h>
#include <OgreHardwarePixelBuffer.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>
#include <OgreInstancedEntity.h>
#include <OgreInstanceBatch.h>

using namespace Ogre;

namespace Forests {

//-------------------------------------------------------------------------------------

uint32 ImpostorPage::selfInstances = 0;

int ImpostorPage::impostorResolution = 128;
ColourValue ImpostorPage::impostorBackgroundColor = ColourValue(0.0f, 0.3f, 0.0f, 0.0f);
BillboardOrigin ImpostorPage::impostorPivot = BBO_CENTER;


void ImpostorPage::init(PagedGeometry *geom, const Ogre::Any &data)
{
	//Save pointers to PagedGeometry object
	sceneMgr = geom->getSceneManager();
	this->geom = geom;

	//Init. variables
	setBlendMode(ALPHA_REJECT_IMPOSTOR);
		
	if (++selfInstances == 1){
		//Set up a single instance of a scene node which will be used when rendering impostor textures
		mRenderNode = geom->getSceneNode()->createChildSceneNode("ImpostorPage::renderNode");
		mCameraNode = geom->getSceneNode()->createChildSceneNode("ImpostorPage::cameraNode");
        ResourceGroupManager::getSingleton().createResourceGroup("Impostors");
	}
}

ImpostorPage::ImpostorPage()
: mRenderNode(nullptr), mCameraNode(nullptr)
{

}

ImpostorPage::~ImpostorPage()
{
	//Delete all impostor batches
	std::map<String, ImpostorBatch *>::iterator iter;
	for (iter = impostorBatches.begin(); iter != impostorBatches.end(); ++iter){
		ImpostorBatch *ibatch = iter->second;
		delete ibatch;
	}

	if (--selfInstances == 0){
		if (mRenderNode) {
			sceneMgr->destroySceneNode(mRenderNode);
		}
		if (mCameraNode) {
			sceneMgr->destroySceneNode(mCameraNode);
		}
        ResourceGroupManager::getSingleton().destroyResourceGroup("Impostors");
	}
}


void ImpostorPage::setRegion(Ogre::Real left, Ogre::Real top, Ogre::Real right, Ogre::Real bottom)
{
	//Calculate center of region
	center.x = (left + right) * 0.5f;
	center.z = (top + bottom) * 0.5f;
	
	center.y = 0.0f;	//The center.y value is calculated when the entities are added
	aveCount = 0;
}

void ImpostorPage::addEntity(Entity *ent, const Vector3 &position, const Quaternion &rotation, const Vector3 &scale, const Ogre::ColourValue &color)
{
//	//Get the impostor batch that this impostor will be added to
//	ImpostorBatch *ibatch = ImpostorBatch::getBatch(this, ent);
//
//	//Then add the impostor to the batch
//	ibatch->addBillboard(position, rotation, scale, color);
//
//	//Add the Y position to the center.y value (to be averaged later)
//	center.y += position.y + ent->getBoundingBox().getCenter().y * scale.y;
//	++aveCount;
}

void ImpostorPage::addModel(Ember::OgreView::Model::Model *model, const Vector3 &position, const Quaternion &rotation, const Vector3 &scale, const Ogre::ColourValue &color)
{
	//Get the impostor batch that this impostor will be added to
	ImpostorBatch *ibatch = ImpostorBatch::getBatch(this, model);

	//Then add the impostor to the batch
	ibatch->addBillboard(position, rotation, scale, color);

	//Add the Y position to the center.y value (to be averaged later)
	auto bbox = model->getBoundingBox();
	if (bbox.isFinite()) {
		center.y += position.y + bbox.getCenter().y * scale.y;
	}
	++aveCount;
}

void ImpostorPage::build()
{
	//Calculate the average Y value of all the added entities
	if (aveCount != 0)
		center.y /= aveCount;
	else
		center.y = 0.0f;

	//Build all batches
	std::map<String, ImpostorBatch *>::iterator iter;
	for (iter = impostorBatches.begin(); iter != impostorBatches.end(); ++iter){
		ImpostorBatch *ibatch = iter->second;
		ibatch->build();
	}
}

void ImpostorPage::setVisible(bool visible)
{
	//Update visibility status of all batches
	std::map<String, ImpostorBatch *>::iterator iter;
	for (iter = impostorBatches.begin(); iter != impostorBatches.end(); ++iter){
		ImpostorBatch *ibatch = iter->second;
		ibatch->setVisible(visible);
	}
}

void ImpostorPage::setFade(bool enabled, Real visibleDist, Real invisibleDist)
{
	//Update fade status of all batches
	std::map<String, ImpostorBatch *>::iterator iter;
	for (iter = impostorBatches.begin(); iter != impostorBatches.end(); ++iter){
		ImpostorBatch *ibatch = iter->second;
		ibatch->setFade(enabled, visibleDist, invisibleDist);
	}
}

void ImpostorPage::removeEntities()
{
	//Clear all impostor batches
	std::map<String, ImpostorBatch *>::iterator iter;
	for (iter = impostorBatches.begin(); iter != impostorBatches.end(); ++iter){
		ImpostorBatch *ibatch = iter->second;
		ibatch->clear();
	}

	//Reset y center
	center.y = 0.0f;
	aveCount = 0;
}

void ImpostorPage::update()
{
	//Calculate the direction the impostor batches should be facing
	Vector3 camPos = geom->_convertToLocal(geom->getCamera()->getDerivedPosition());
	
	//Update all batches
	float distX = camPos.x - center.x;
	float distZ = camPos.z - center.z;
	float distY = camPos.y - center.y;
	float distRelZ = Math::Sqrt(distX * distX + distZ * distZ);
	Radian pitch = Math::ATan2(distY, distRelZ);

	Radian yaw;
	if (distRelZ > geom->getPageSize() * 3) {
		yaw = Math::ATan2(distX, distZ);
	} else {
		Vector3 dir = geom->_convertToLocal(geom->getCamera()->getDerivedDirection());
		yaw = Math::ATan2(-dir.x, -dir.z);
	}

	std::map<String, ImpostorBatch *>::iterator iter;
	for (iter = impostorBatches.begin(); iter != impostorBatches.end(); ++iter){
		ImpostorBatch *ibatch = iter->second;
		ibatch->setAngle(pitch.valueDegrees(), yaw.valueDegrees());
	}
}

//void ImpostorPage::regenerate(Entity *ent)
//{
//	ImpostorTexture *tex = ImpostorTexture::getTexture(nullptr, ent);
//	if (tex)
//		tex->regenerate();
//}

void ImpostorPage::regenerateAll()
{
	ImpostorTexture::regenerateAll();
}

void ImpostorPage::setImpostorPivot(BillboardOrigin origin)
{
	if (origin != BBO_CENTER && origin != BBO_BOTTOM_CENTER)
		OGRE_EXCEPT(Exception::ERR_INVALIDPARAMS, "Invalid origin - only BBO_CENTER and BBO_BOTTOM_CENTER is supported", "ImpostorPage::setImpostorPivot()");
	impostorPivot = origin;
}

//-------------------------------------------------------------------------------------

unsigned long ImpostorBatch::GUID = 0;

ImpostorBatch::ImpostorBatch(ImpostorPage *group, Ember::OgreView::Model::Model* model)
{
	//Render impostor texture for this entity
	tex = ImpostorTexture::getTexture(group, model);
	
	//Create billboard set
	bbset = new StaticBillboardSet(group->sceneMgr, group->geom->getSceneNode());
	bbset->setTextureStacksAndSlices(IMPOSTOR_PITCH_ANGLES, IMPOSTOR_YAW_ANGLES);

	setBillboardOrigin(ImpostorPage::impostorPivot);

	//Default the angle to 0 degrees
	pitchIndex = -1;
	yawIndex = -1;
	setAngle(0.0f, 0.0f);

	//Init. variables
	igroup = group;
}

ImpostorBatch::~ImpostorBatch()
{
	//Delete billboard set
	delete bbset;
	//Delete texture
	ImpostorTexture::removeTexture(tex);
}

//Returns a pointer to an ImpostorBatch for the specified entity in the specified
//ImpostorPage. If one does not already exist, one will automatically be created.
ImpostorBatch *ImpostorBatch::getBatch(ImpostorPage *group, Ember::OgreView::Model::Model *model)
{
	//Search for an existing impostor batch for this entity
	String entityKey = ImpostorBatch::generateEntityKey(model);
	std::map<String, ImpostorBatch *>::iterator iter;
	iter = group->impostorBatches.find(entityKey);

	//If found..
	if (iter != group->impostorBatches.end()){
		//Return it
		return iter->second;
	} else {
		//Otherwise, create a new batch
		ImpostorBatch *batch = new ImpostorBatch(group, model);

		//Add it to the impostorBatches list
		typedef std::pair<String, ImpostorBatch *> ListItem;
		group->impostorBatches.insert(ListItem(entityKey, batch));
		
		//Return it
		return batch;
	}
}

//Rotates all the impostors to the specified angle (virtually - it actually changes
//their materials to produce this same effect)
void ImpostorBatch::setAngle(float pitchDeg, float yawDeg)
{
	//Calculate pitch material index
	int newPitchIndex;
#ifdef IMPOSTOR_RENDER_ABOVE_ONLY
	if (pitchDeg > 0) {
		float maxPitchIndexDeg = (90.0f * (IMPOSTOR_PITCH_ANGLES-1)) / IMPOSTOR_PITCH_ANGLES;
		newPitchIndex = (int)(IMPOSTOR_PITCH_ANGLES * (pitchDeg / maxPitchIndexDeg));
		if (newPitchIndex > IMPOSTOR_PITCH_ANGLES-1) newPitchIndex = IMPOSTOR_PITCH_ANGLES-1;
	} else {
		newPitchIndex = 0;
	}
#else
	float minPitchIndexDeg = -90.0f;
	float maxPitchIndexDeg = ((180.0f * (IMPOSTOR_PITCH_ANGLES-1)) / IMPOSTOR_PITCH_ANGLES) - 90.0f;
	newPitchIndex = (int)(IMPOSTOR_PITCH_ANGLES * ((pitchDeg - minPitchIndexDeg) / (maxPitchIndexDeg - minPitchIndexDeg)));
	if (newPitchIndex > IMPOSTOR_PITCH_ANGLES-1) newPitchIndex = IMPOSTOR_PITCH_ANGLES-1;
	if (newPitchIndex < 0) newPitchIndex = 0;
#endif
	
	//Calculate yaw material index
	int newYawIndex;
	if (yawDeg > 0) {
		newYawIndex = (int)(IMPOSTOR_YAW_ANGLES * (yawDeg / 360.0f) + 0.5f) % IMPOSTOR_YAW_ANGLES;
	} else {
		newYawIndex = (int)(IMPOSTOR_YAW_ANGLES + IMPOSTOR_YAW_ANGLES * (yawDeg / 360.0f) + 0.5f) % IMPOSTOR_YAW_ANGLES;
	}
	
	//Change materials if necessary
	if (newPitchIndex != pitchIndex || newYawIndex != yawIndex){
		pitchIndex = newPitchIndex;
		yawIndex = newYawIndex;
		bbset->setMaterial(tex->material[pitchIndex][yawIndex]->getName());
	}
}

void ImpostorBatch::setBillboardOrigin(BillboardOrigin origin)
{
	bbset->setBillboardOrigin(origin);

	if (bbset->getBillboardOrigin() == BBO_CENTER)
		entityBBCenter = tex->entityCenter;
	else if (bbset->getBillboardOrigin() == BBO_BOTTOM_CENTER)
		entityBBCenter = Vector3(tex->entityCenter.x, tex->entityCenter.y - tex->entityRadius, tex->entityCenter.z);
}

String ImpostorBatch::generateEntityKey(Ember::OgreView::Model::Model* model)
{
	Ogre::StringStream entityKey;

	for (auto& submodel : model->getSubmodels()) {
		entityKey << submodel->getEntity()->getMesh()->getName();
	}

	entityKey << "-" << IMPOSTOR_YAW_ANGLES << "_" << IMPOSTOR_PITCH_ANGLES;
#ifdef IMPOSTOR_RENDER_ABOVE_ONLY
	entityKey << "_RAO";
#endif
	return entityKey.str();
}

//-------------------------------------------------------------------------------------


ImpostorTextureResourceLoader::ImpostorTextureResourceLoader(ImpostorTexture& impostorTexture)
: texture(impostorTexture)
{
}

void ImpostorTextureResourceLoader::loadResource (Ogre::Resource *resource)
{
	if (resource->getLoadingState() == Ogre::Resource::LOADSTATE_UNLOADED && texture.texture) {
		texture.regenerate();
	}
}

//-------------------------------------------------------------------------------------


std::map<String, ImpostorTexture *> ImpostorTexture::selfList;
unsigned long ImpostorTexture::GUID = 0;

//Do not use this constructor yourself - instead, call getTexture()
//to get/create an ImpostorTexture for an Entity.
ImpostorTexture::ImpostorTexture(ImpostorPage *group, Ember::OgreView::Model::Model* model)
: loader(nullptr)
{
	//Store scene manager and entity
	ImpostorTexture::sceneMgr = group->sceneMgr;
	ImpostorTexture::model = model;
	ImpostorTexture::group = group;

	//Add self to list of ImpostorTexture's
	entityKey = ImpostorBatch::generateEntityKey(model);
	typedef std::pair<String, ImpostorTexture *> ListItem;
	selfList.insert(ListItem(entityKey, this));
	
	//Calculate the entity's bounding box and it's diameter
	boundingBox = model->getBoundingBox();

	//Note - this radius calculation assumes the object is somewhat rounded (like trees/rocks/etc.)
	Real tmp;
	entityRadius = boundingBox.getMaximum().x - boundingBox.getCenter().x;
	tmp = boundingBox.getMaximum().y - boundingBox.getCenter().y;
	if (tmp > entityRadius) entityRadius = tmp;
	tmp = boundingBox.getMaximum().z - boundingBox.getCenter().z;
	if (tmp > entityRadius) entityRadius = tmp;

	entityDiameter = 2.0f * entityRadius;
	entityCenter = boundingBox.getCenter();
	
	//Render impostor textures
	renderTextures(false);
	
	//Set up materials
	for (int o = 0; o < IMPOSTOR_YAW_ANGLES; ++o){
	for (int i = 0; i < IMPOSTOR_PITCH_ANGLES; ++i){
		material[i][o] = MaterialManager::getSingleton().create(getUniqueID("ImpostorMaterial"), "Impostors");

		Material *m = material[i][o].get();
		Pass *p = m->getTechnique(0)->getPass(0);
		
		TextureUnitState *t = p->createTextureUnitState(texture->getName());
		
		t->setTextureUScroll((float)o / IMPOSTOR_YAW_ANGLES);
		t->setTextureVScroll((float)i / IMPOSTOR_PITCH_ANGLES);

		p->setLightingEnabled(false);
		m->setReceiveShadows(false);
		
		if (group->getBlendMode() == ALPHA_REJECT_IMPOSTOR){
			p->setAlphaRejectSettings(CMPF_GREATER_EQUAL, 128);
			//p->setAlphaRejectSettings(CMPF_GREATER_EQUAL, 64);
		} else if (group->getBlendMode() == ALPHA_BLEND_IMPOSTOR){
			p->setSceneBlending(SBF_SOURCE_ALPHA, SBF_ONE_MINUS_SOURCE_ALPHA);
			p->setDepthWriteEnabled(false);  
		}
	}
	}
}

void ImpostorTexture::updateMaterials()
{
	for (int o = 0; o < IMPOSTOR_YAW_ANGLES; ++o){
		for (int i = 0; i < IMPOSTOR_PITCH_ANGLES; ++i){
			Material *m = material[i][o].get();
			Pass *p = m->getTechnique(0)->getPass(0);

			TextureUnitState *t = p->getTextureUnitState(0);

			t->setTextureName(texture->getName());
		}
	}
}

ImpostorTexture::~ImpostorTexture()
{
	//Delete textures
	assert(texture);
	String texName(texture->getName());
		
	texture.reset();
	if (TextureManager::getSingletonPtr())
		TextureManager::getSingleton().remove(texName, "Impostors");
	
	//Delete materials
	for (int o = 0; o < IMPOSTOR_YAW_ANGLES; ++o){
	for (int i = 0; i < IMPOSTOR_PITCH_ANGLES; ++i){
		assert (material[i][o]);
		String matName(material[i][o]->getName());

		material[i][o].reset();
		if (MaterialManager::getSingletonPtr())
			MaterialManager::getSingleton().remove(matName, "Impostors");
	}
	}
	
	//Remove self from list of ImpostorTexture's
	selfList.erase(entityKey);
}

void ImpostorTexture::regenerate()
{
	assert(texture);
	String texName(texture->getName());
	texture.reset();
	if (TextureManager::getSingletonPtr())
		TextureManager::getSingleton().remove(texName, "Impostors");

	renderTextures(true);
	updateMaterials();
}

void ImpostorTexture::regenerateAll()
{
	std::map<String, ImpostorTexture *>::iterator iter;
	for (iter = selfList.begin(); iter != selfList.end(); ++iter){
		iter->second->regenerate();
	}
}

void ImpostorTexture::renderTextures(bool force)
{
#ifdef IMPOSTOR_FILE_SAVE
	TexturePtr renderTexture;
#else
	TexturePtr renderTexture(texture);
	//if we're not using a file image we need to set up a resource loader, so that the texture is regenerated if it's ever unloaded (such as switching between fullscreen and the desktop in win32)
	loader = std::unique_ptr<ImpostorTextureResourceLoader>(new ImpostorTextureResourceLoader(*this));
#endif
	RenderTexture *renderTarget;
	Camera *renderCamera;
	Viewport *renderViewport;
	SceneNode *camNode;

	//Set up RTT texture
	uint32 textureSize = ImpostorPage::impostorResolution;
	if (!renderTexture) {
	renderTexture = TextureManager::getSingleton().createManual(getUniqueID("ImpostorTexture"), "Impostors",
				TEX_TYPE_2D, textureSize * IMPOSTOR_YAW_ANGLES, textureSize * IMPOSTOR_PITCH_ANGLES, 1, MIP_UNLIMITED, PF_A8R8G8B8, TU_RENDERTARGET, loader.get());
	}

	//Set up render target
	renderTarget = renderTexture->getBuffer()->getRenderTarget(); 
	renderTarget->setAutoUpdated(false);
	renderTarget->setActive(false);

	//Set up camera
	camNode = sceneMgr->getSceneNode("ImpostorPage::cameraNode");
	renderCamera = sceneMgr->createCamera(getUniqueID("ImpostorCam"));
	camNode->attachObject(renderCamera);
	renderCamera->setLodBias(1000.0f);
	renderViewport = renderTarget->addViewport(renderCamera);
	renderViewport->setOverlaysEnabled(false);
	renderViewport->setClearEveryFrame(true);
	renderViewport->setShadowsEnabled(false);
	renderViewport->setBackgroundColour(ImpostorPage::impostorBackgroundColor);
	
	//Set up scene node
	SceneNode* node = sceneMgr->getSceneNode("ImpostorPage::renderNode");

	Ember::OgreView::SceneNodeProvider sceneNodeProvider(node, nullptr, false);
	auto* oldNodeProvider = model->getNodeProvider();

	model->attachToNode(&sceneNodeProvider);

	node->setPosition(-entityCenter);
	
	//Set up camera FOV
	const Real objDist = entityRadius * 100;
	const Real nearDist = objDist - (entityRadius + 1); 
	const Real farDist = objDist + (entityRadius + 1);
	
	renderCamera->setAspectRatio(1.0f);
	renderCamera->setFOVy(Math::ATan(entityDiameter / objDist));
	renderCamera->setNearClipDistance(nearDist);
	renderCamera->setFarClipDistance(farDist);
	
	//Disable mipmapping (without this, masked textures look bad)
	MaterialManager *mm = MaterialManager::getSingletonPtr();
	FilterOptions oldMinFilter = mm->getDefaultTextureFiltering(FT_MIN);
	FilterOptions oldMagFilter = mm->getDefaultTextureFiltering(FT_MAG);
	FilterOptions oldMipFilter = mm->getDefaultTextureFiltering(FT_MIP);
	mm->setDefaultTextureFiltering(FO_POINT, FO_LINEAR, FO_NONE);

	//Disable fog
	FogMode oldFogMode = sceneMgr->getFogMode();
	ColourValue oldFogColor = sceneMgr->getFogColour();
	Real oldFogDensity = sceneMgr->getFogDensity();
	Real oldFogStart = sceneMgr->getFogStart();
	Real oldFogEnd = sceneMgr->getFogEnd();
	sceneMgr->setFog(Ogre::FOG_EXP2, Ogre::ColourValue(0,0,0,0), 0.0f, 0.0f, 0.0f); //Ember change
	
	//We need to disable all lightning and render it full bright
	Ogre::ColourValue oldAmbientColour = sceneMgr->getAmbientLight();
	sceneMgr->setAmbientLight(ColourValue::White);

	std::vector<Ogre::MovableObject*> lightStore;
	auto& lights = sceneMgr->getMovableObjects(Ogre::LightFactory::FACTORY_TYPE_NAME);
	for (auto& entry : lights) {
		Ogre::MovableObject* light = entry.second;
		if (light) {
			if (light->getVisible()) {
				lightStore.push_back(light);
				light->setVisible(false);
			}
		}
	}

	// Get current status of the queue mode
	Ogre::SceneManager::SpecialCaseRenderQueueMode OldSpecialCaseRenderQueueMode = sceneMgr->getSpecialCaseRenderQueueMode();
	//Only render the entity
	sceneMgr->setSpecialCaseRenderQueueMode(Ogre::SceneManager::SCRQM_INCLUDE); 
	sceneMgr->addSpecialCaseRenderQueue(group->geom->getRenderQueue() + 1);

	std::vector<uint8> oldRenderQueues;
	std::vector<bool> oldVisibles;
	std::vector<float> oldRenderDistance;
	model->doWithMovables([&](Ogre::MovableObject* movable, int index) {
		if (movable->getMovableType() == "InstancedEntity") {
			auto instancedEntity = dynamic_cast<Ogre::InstancedEntity*>(movable);
			instancedEntity->_getOwner()->setRenderQueueGroup(group->geom->getRenderQueue() + 1);
			oldRenderQueues.push_back(instancedEntity->_getOwner()->getRenderQueueGroup());
			//Need to update the bounds here, as it's otherwise only done once each frame, and we've moved the entity
			instancedEntity->_getOwner()->_updateBounds();
		} else {
			movable->setRenderQueueGroup(group->geom->getRenderQueue() + 1);
			oldRenderQueues.push_back(movable->getRenderQueueGroup());
		}

		oldVisibles.push_back(movable->getVisible());
		oldRenderDistance.push_back(movable->getRenderingDistance());
		movable->setVisible(true);
		movable->setRenderingDistance(0);
	});

	bool needsRegen = true;
#ifdef IMPOSTOR_FILE_SAVE
	//Calculate the filename hash used to uniquely identity this render
	String strKey = entityKey;
	char key[32] = {0};
	uint32 i = 0;
	for (String::const_iterator it = entityKey.begin(); it != entityKey.end(); ++it)
	{
		key[i] ^= *it;
		i = (i+1) % sizeof(key);
	}
	for (i = 0; i < sizeof(key); ++i)
		key[i] = (key[i] % 26) + 'A';

	String tempdir = this->group->geom->getTempdir();
	ResourceGroupManager::getSingleton().addResourceLocation(tempdir, "FileSystem", "BinFolder");

	String fileNamePNG = "Impostor." + String(key, sizeof(key)) + '.' + StringConverter::toString(textureSize) + ".png";
	String fileNameDDS = "Impostor." + String(key, sizeof(key)) + '.' + StringConverter::toString(textureSize) + ".dds";

	//Attempt to load the pre-render file if allowed
	needsRegen = force;
	if (!needsRegen){
		try{
			texture = TextureManager::getSingleton().load(fileNameDDS, "BinFolder", TEX_TYPE_2D, MIP_UNLIMITED);
		}
		catch (...){
			try{
				texture = TextureManager::getSingleton().load(fileNamePNG, "BinFolder", TEX_TYPE_2D, MIP_UNLIMITED);
			}
			catch (...){
				needsRegen = true;
			}
		}
	}
#endif

	if (needsRegen){
		//If this has not been pre-rendered, do so now
		const float xDivFactor = 1.0f / IMPOSTOR_YAW_ANGLES;
		const float yDivFactor = 1.0f / IMPOSTOR_PITCH_ANGLES;
		for (int o = 0; o < IMPOSTOR_PITCH_ANGLES; ++o){ //4 pitch angle renders
#ifdef IMPOSTOR_RENDER_ABOVE_ONLY
			Radian pitch = Degree((90.0f * o) * yDivFactor); //0, 22.5, 45, 67.5
#else
			Radian pitch = Degree((180.0f * o) * yDivFactor - 90.0f);
#endif

			for (int i = 0; i < IMPOSTOR_YAW_ANGLES; ++i){ //8 yaw angle renders
				Radian yaw = Degree((360.0f * i) * xDivFactor); //0, 45, 90, 135, 180, 225, 270, 315
					
				//Position camera
				camNode->setPosition(0, 0, 0);
                camNode->setOrientation(Quaternion(yaw, Vector3::UNIT_Y) * Quaternion(-pitch, Vector3::UNIT_X));
                camNode->translate(Vector3(0, 0, objDist), Node::TS_LOCAL);
						
				//Render the impostor
				renderViewport->setDimensions((float)(i) * xDivFactor, (float)(o) * yDivFactor, xDivFactor, yDivFactor);
				renderTarget->update();

				//blit for each mipmap
				auto sourceBuffer = renderTexture->getBuffer(0,0);
				for (unsigned int mipmapIndex = 1; mipmapIndex < renderTexture->getNumMipmaps(); ++mipmapIndex) {
					Ogre::HardwarePixelBufferSharedPtr destBuffer = renderTexture->getBuffer(0, mipmapIndex);
					destBuffer->blit(sourceBuffer);
				}
			}
		}
	
#ifdef IMPOSTOR_FILE_SAVE
		//Save RTT to file with respecting the temp dir
		renderTarget->writeContentsToFile(tempdir + fileNamePNG);

		//Load the render into the appropriate texture view
		texture = TextureManager::getSingleton().load(fileNamePNG, "BinFolder", TEX_TYPE_2D, MIP_UNLIMITED);
#else
		texture = renderTexture;
#endif
	}


	model->doWithMovables([&](Ogre::MovableObject* movable, int index) {
		if (movable->getMovableType() == "InstancedEntity") {
			auto instancedEntity = dynamic_cast<Ogre::InstancedEntity*>(movable);
			instancedEntity->_getOwner()->setRenderQueueGroup(oldRenderQueues[index]);
			//Need to update the bounds here, as it's otherwise only done once each frame, and we've moved the entity
			instancedEntity->_getOwner()->_updateBounds();
		} else {
			movable->setRenderQueueGroup(oldRenderQueues[index]);
		}

		movable->setVisible(oldVisibles[index]);
		movable->setRenderingDistance(oldRenderDistance[index]);
	});

	sceneMgr->removeSpecialCaseRenderQueue(group->geom->getRenderQueue() + 1);
	// Restore original state
	sceneMgr->setSpecialCaseRenderQueueMode(OldSpecialCaseRenderQueueMode); 

	//Re-enable mipmapping
	mm->setDefaultTextureFiltering(oldMinFilter, oldMagFilter, oldMipFilter);

	//Re-enable fog
	sceneMgr->setFog(oldFogMode, oldFogColor, oldFogDensity, oldFogStart, oldFogEnd);

	//Re-enable both scene lightning and disabled individual lights
	sceneMgr->setAmbientLight(oldAmbientColour);
	for (std::vector<Ogre::MovableObject*>::const_iterator I = lightStore.begin(); I != lightStore.end(); ++I) {
		(*I)->setVisible(true);
	}

	//Delete camera
	renderTarget->removeViewport(0);
	renderCamera->getSceneManager()->destroyCamera(renderCamera);
	
	//Delete scene node
	model->attachToNode(oldNodeProvider);
#ifdef IMPOSTOR_FILE_SAVE
	//Delete RTT texture
	assert(renderTexture);
	String texName2(renderTexture->getName());

	renderTexture.reset();
	if (TextureManager::getSingletonPtr())
		TextureManager::getSingleton().remove(texName2);
#endif
}

String ImpostorTexture::removeInvalidCharacters(String s)
{
	Ogre::StringStream s2;

	for (uint32 i = 0; i < s.length(); ++i){
		char c = s[i];
		if (c == '/' || c == '\\' || c == ':' || c == '*' || c == '?' || c == '\"' || c == '<' || c == '>' || c == '|'){
			s2 << '-';
		} else {
			s2 << c;
		}
	}

	return s2.str();
}

void ImpostorTexture::removeTexture(ImpostorTexture* Texture)
{
	//Search for an existing impostor texture, in case it was already deleted
	for(std::map<String, ImpostorTexture *>::iterator iter=selfList.begin();
		iter!=selfList.end(); ++iter)
	{
		if(iter->second==Texture)
		{
			delete Texture;
			return;
		}
	}
	// no need to anything if it was not found, chances are that it was already deleted
}

ImpostorTexture *ImpostorTexture::getTexture(ImpostorPage *group, Ember::OgreView::Model::Model* model)
{
	//Search for an existing impostor texture for the given entity
	String entityKey = ImpostorBatch::generateEntityKey(model);
	std::map<String, ImpostorTexture *>::iterator iter;
	iter = selfList.find(entityKey);
	
	//If found..
	if (iter != selfList.end()){
		//Return it
		return iter->second;		
	} else {
		if (group){
			//Otherwise, return a new texture
			return (new ImpostorTexture(group, model));
		} else {
			//But if group is null, return null
			return NULL;
		}
	}
}
}
