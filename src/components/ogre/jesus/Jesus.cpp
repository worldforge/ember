//
// C++ Implementation: Jesus
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2005
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
#include "Jesus.h"
#include "../carpenter/Carpenter.h"
#include "../carpenter/BluePrint.h"
#include "../model/Model.h"
#include "JesusPickerObject.h"




#include "services/EmberServices.h"
#include "services/config/ConfigService.h"



namespace EmberOgre {




Jesus::Jesus(Carpenter::Carpenter* carpenter)
: mCarpenter(carpenter)
{
	
	mNormalTypes["NORTH"] = WFMath::Vector<3>(0,1,0);
	mNormalTypes["SOUTH"] = WFMath::Vector<3>(0,-1,0);
	mNormalTypes["WEST"] = WFMath::Vector<3>(-1,0,0);
	mNormalTypes["EAST"] = WFMath::Vector<3>(1,0,0);
	mNormalTypes["UP"] = WFMath::Vector<3>(0,0,1);
	mNormalTypes["DOWN"] = WFMath::Vector<3>(0,0,-1);
	
}


Jesus::~Jesus()
{
	delete mCarpenter;
}

AttachPointNode::AttachPointNode(ModelBlock* modelBlock, Ogre::SceneNode* modelNode, const Carpenter::AttachPoint* attachPoint, Ogre::ColourValue colour,Ogre::BillboardSet* billboardSet )
: mModelBlock(modelBlock), mModelNode(modelNode), mAttachPoint(attachPoint), mColour(colour), mController(0)
{
	Ogre::Vector3 position = Atlas2Ogre(attachPoint->getPosition());
	std::string name = modelNode->getName() + "_" + attachPoint->getAttachPair()->getName() + "_" + attachPoint->getName();

/*	mPointerNode = mModelNode->createChildSceneNode ();
	
	mPointerEntity = EmberOgre::getSingleton().getSceneManager()->createEntity(name + "entity", "arrow.mesh");
	
	mPointerNode->attachObject(mPointerEntity);
	mPointerNode->setPosition(position);
	WFMath::Quaternion quat;
	quat.rotation(WFMath::Vector<3>(1,-1,1), attachPoint->getNormal());
	
	//Ogre::Vector3 normal = Atlas2Ogre(attachPoint->getNormal());
	mPointerNode->setOrientation(Atlas2Ogre(quat));*/
	
// 	mFlare = billboardSet->createBillboard(modelBlock->getBuildingBlock()->getWorldPositionForPoint(attachPoint) , colour);
	mFlare = billboardSet->createBillboard(position , colour);
//	mFlare = billboardSet->createBillboard(0,0,0 , colour);

}

AttachPointNode::~AttachPointNode()
{
	Ogre::ControllerManager::getSingleton().destroyController(mController);
}

	
void Jesus::addAttachPointType(const std::string & type) 
{
	
	AttachPointColourValueMap::iterator J = mColourMap.find(type);
	if (J == mColourMap.end()) {
		Ogre::ColourValue colour = Ogre::ColourValue(0.5,0.5,0.1 * mColourMap.size());
		mColourMap.insert(AttachPointColourValueMap::value_type(type, colour));
	} 

}


bool Jesus::addBluePrint(Carpenter::BluePrint* blueprint)
{
	if (mBlueprints.find(blueprint->getName()) != mBlueprints.end()) {
		//there is already a blueprint with that name
		return false;
	}
	mBlueprints.insert(std::map<std::string, Carpenter::BluePrint*>::value_type(blueprint->getName(), blueprint));
	return true;
}

Carpenter::BluePrint* Jesus::getBluePrint(const std::string& name) const
{
	std::map<std::string, Carpenter::BluePrint*>::const_iterator I = mBlueprints.find(name);
	if (I != mBlueprints.end()) 
	{
		return I->second;
	}
	return 0;
	
}



Construction::Construction(Carpenter::BluePrint* bluePrint, Jesus* jesus, Ogre::SceneNode* node)
: mBaseNode(node), mJesus(jesus), mBlueprint(bluePrint)
{


}

Construction::~Construction()
{
	for (std::vector<ModelBlock*>::iterator I = mModelBlocks.begin(); I != mModelBlocks.end(); ++I) {
		delete *I;
	}
//	mBaseNode->removeAndDestroyAllChildren();
}

void Construction::buildFromBluePrint(bool createAttachPointNodes)
{
	std::vector<Carpenter::BuildingBlock*> buildingBlocks = mBlueprint->getAttachedBlocks();
	
	std::vector<Carpenter::BuildingBlock*>::const_iterator J = buildingBlocks.begin();
	std::vector<Carpenter::BuildingBlock*>::const_iterator J_end = buildingBlocks.end();
	
	for (;J != J_end; ++J)
	{
		Carpenter::BuildingBlock* block = *J;
		if (block->isAttached()) {
			ModelBlock* modelBlock = createModelBlock(block, createAttachPointNodes);
		}
	}

}

ModelBlock* Construction::createModelBlock(const Carpenter::BuildingBlock* buildingBlock, bool createAttachPointNodes)
{
	std::string blockSpecName = buildingBlock->getBuildingBlockSpec()->getName();
	
	Model::Model* model = mJesus->createModelForBlockType(blockSpecName, mBaseNode->getName() + buildingBlock->getName());
	ModelBlock* modelBlock = new ModelBlock(mBaseNode, buildingBlock, model, this);
	
	if (createAttachPointNodes) 
		modelBlock->createAttachPointNodes();
	mModelBlocks.push_back(modelBlock);
	return modelBlock;	
}

bool Construction::remove(ModelBlock* modelBlock)
{
	bool result = mBlueprint->remove(modelBlock->getBuildingBlock());
	if (result) {
		std::vector<ModelBlock*>::iterator pos = mModelBlocks.end();
		for (std::vector<ModelBlock*>::iterator I = mModelBlocks.begin(); I != mModelBlocks.end(); ++I) {
			if (modelBlock == *I) {
				pos = I;
				break;
			}
		}
		if (pos != mModelBlocks.end()) {
			mModelBlocks.erase(pos);
		}
		delete modelBlock;
			
	}
	return result;
}


std::vector<ModelBlock*> Construction::getModelBlocks() const
{
	return mModelBlocks;
}

std::vector<AttachPointNode*> ModelBlock::getAttachPointNodes() const
{
	return mAttachPointNodes;
}


Model::Model* Jesus::createModelForBlockType(const std::string& blockType, const std::string& modelName)
{
	std::map<std::string, std::string>::iterator I = mModelMappings.find(blockType);
	if (I == mModelMappings.end()) {
		return 0;
	}
	Model::Model* aModel = new Model::Model(modelName);
	aModel->create(I->second);
	return aModel;
//	return Model::Create(I->second + ".modeldef.xml", modelName);
}

ModelBlock::~ModelBlock()
{
	mNode->getCreator()->destroyBillboardSet(mPointBillBoardSet);
	if (mModelNode) 
		mNode->getCreator()->destroySceneNode (mModelNode->getName());
		
	delete mModel;
		
	std::vector<AttachPointNode* >::iterator I = mAttachPointNodes.begin();
	std::vector<AttachPointNode* >::iterator I_end = mAttachPointNodes.end();
	for(; I != I_end; ++I) {
 		delete *I;
	}
	mNode->getParent()->removeChild(mNode);
	mNode->getCreator()->destroySceneNode (mNode->getName());

	
}


ModelBlock::ModelBlock(Ogre::SceneNode* baseNode,const  Carpenter::BuildingBlock* buildingBlock, Model::Model* model, Construction* construction)
: mBuildingBlock(buildingBlock), mModel(model), mConstruction(construction), mModelNode(0)
{
	
	
	mNode = baseNode->createChildSceneNode(Atlas2Ogre(buildingBlock->getPosition()), Atlas2Ogre(buildingBlock->getOrientation()));
	
	
	mPointBillBoardSet = mNode->getCreator()->createBillboardSet( std::string("__construction_") + construction->getBluePrint()->getName() + "_" + buildingBlock->getName() + "_billboardset__" + mNode->getName());
	mPointBillBoardSet->setDefaultDimensions(1, 1);
	mPointBillBoardSet->setMaterialName("carpenter/flare");
	mPointBillBoardSet->setVisible(false);
//	Ogre::SceneNode* aNode = EmberOgre::getSingleton().getSceneManager()->getRootSceneNode()->createChildSceneNode();
	mNode->attachObject(mPointBillBoardSet);
	
		
	if (model) {
		JesusPickerObject* pickerObject = new JesusPickerObject(this, 0);
		model->setUserObject(pickerObject);
		
		
		mModelNode = mNode->createChildSceneNode();
		mModelNode->attachObject(model);
		model->setQueryFlags(Jesus::CM_MODELBLOCK);
		
		//only autoscale the model if we've not set the scale in the modeldefinition
		//TODO: it would of course be best if all models were correctly scaled and this code could be removed
		if (model->getDefinition()->getScale() == 0) {
			const Ogre::AxisAlignedBox ogreBoundingBox = model->getBoundingBox();
			const Ogre::Vector3 ogreMax = ogreBoundingBox.getMaximum();
			const Ogre::Vector3 ogreMin = ogreBoundingBox.getMinimum();
		
	
			const WFMath::AxisBox<3> wfBoundingBox = buildingBlock->getBuildingBlockSpec()->getBlockSpec()->getBoundingBox();	
			const WFMath::Point<3> wfMax = wfBoundingBox.highCorner();
			const WFMath::Point<3> wfMin = wfBoundingBox.lowCorner();
			
			Ogre::Real scaleX;		
			Ogre::Real scaleY;		
			Ogre::Real scaleZ;		
		
			scaleX = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
			scaleY = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
			scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		
	
			mModelNode->setScale(scaleX, scaleY, scaleZ);
		}
		
	}

}

void ModelBlock::createAttachPointNodes( )
{
	std::vector<const Carpenter::AttachPoint* > allPoints = mBuildingBlock->getAllPoints();
	
	std::vector<const Carpenter::AttachPoint* >::const_iterator I = allPoints.begin();
	std::vector<const Carpenter::AttachPoint* >::const_iterator I_end = allPoints.end();

	for(; I != I_end; ++I) {
		Ogre::ColourValue colour = mConstruction->getJesus()->getColourForAttachPoint(*I);
		AttachPointNode* pointNode = new AttachPointNode(this, mNode, *I, colour, mPointBillBoardSet);
		mAttachPointNodes.push_back(pointNode);
	}
}

void ModelBlock::select()
{
	if (mModelNode) 
		mModelNode->showBoundingBox(true);
	if (mPointBillBoardSet)
		mPointBillBoardSet->setVisible(true);
}

void ModelBlock::deselect()
{
	if (mModelNode) 
		mModelNode->showBoundingBox(false);
	if (mPointBillBoardSet)
		mPointBillBoardSet->setVisible(false);
}


Ogre::ColourValue Jesus::getColourForAttachPoint(const Carpenter::AttachPoint* point) const
{
	AttachPointColourValueMap::const_iterator I = mColourMap.find(point->getAttachPair()->getType());
	if (I == mColourMap.end()) {
		return Ogre::ColourValue(1,1,1);
	}
	return I->second;
}

void AttachPointNode::select()
{
	Ogre::ControllerFunctionRealPtr func = Ogre::ControllerFunctionRealPtr(
			new Ogre::WaveformControllerFunction(Ogre::WFT_SINE, 0.0, 2));
	Ogre::ControllerManager& contMgr = Ogre::ControllerManager::getSingleton();
		
	Ogre::ControllerValueRealPtr val = Ogre::ControllerValueRealPtr(
			new LightWibbler(mColour, mFlare));
	mController = contMgr.createController(contMgr.getFrameTimeSource(), val, func);
}

void AttachPointNode::deselect()
{
	mFlare->setDimensions(1,1);
	mFlare->setColour(mColour);
	Ogre::ControllerManager::getSingleton().destroyController(mController);
	mController = 0;
}

LightWibbler::LightWibbler(const Ogre::ColourValue& originalColour, Ogre::Billboard* billboard)
{
	mOriginalColour = originalColour;
	mBillboard = billboard;
}

Ogre::Real  LightWibbler::getValue (void) const
{
	return intensity;
}

void  LightWibbler::setValue (Ogre::Real value)
{
	intensity = value;

	Ogre::ColourValue newColour(intensity, intensity, intensity);

	mBillboard->setColour(mOriginalColour * newColour);
	mBillboard->setDimensions(1 + (intensity * 2),1 + (intensity * 2));

}


};




