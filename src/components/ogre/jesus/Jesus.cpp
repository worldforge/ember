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
#include "../EmberSceneManager/include/EmberTerrainSceneManager.h"
#include "JesusPickerObject.h"


namespace EmberOgre {

Jesus::Jesus(Carpenter::Carpenter* carpenter)
: mCarpenter(carpenter)
{
	
	mNormalTypes["NORTH"] = WFMath::Vector<3>(0,0,1);
	mNormalTypes["SOUTH"] = WFMath::Vector<3>(0,0,-1);
	mNormalTypes["WEST"] = WFMath::Vector<3>(-1,0,0);
	mNormalTypes["EAST"] = WFMath::Vector<3>(1,0,0);
	mNormalTypes["UP"] = WFMath::Vector<3>(0,1,0);
	mNormalTypes["DOWN"] = WFMath::Vector<3>(0,-1,0);
	
}


Jesus::~Jesus()
{
	delete mCarpenter;
}

AttachPointNode::AttachPointNode(ModelBlock* modelBlock, Ogre::SceneNode* modelNode, const Carpenter::AttachPoint* attachPoint, Ogre::ColourValue colour,Ogre::BillboardSet* billboardSet )
: mModelBlock(modelBlock), mModelNode(modelNode), mAttachPoint(attachPoint), mColour(colour), mController(0)
{
	
	std::string name = modelBlock->getBuildingBlock()->getName() + "_" + attachPoint->getAttachPair()->getName() + "_" + attachPoint->getName();
	
	Ogre::Vector3 position = Atlas2Ogre(attachPoint->getPosition());
// 	mFlare = billboardSet->createBillboard(modelBlock->getBuildingBlock()->getWorldPositionForPoint(attachPoint) , colour);
	mFlare = billboardSet->createBillboard(position , colour);
//	mFlare = billboardSet->createBillboard(0,0,0 , colour);

}

AttachPointNode::~AttachPointNode()
{
	Ogre::ControllerManager::getSingleton().destroyController(mController);
}

	
bool Jesus::loadModelBlockMapping(const std::string& filename)
{
	if (filename == "") {
		return false;
	}
	
	try {
	        xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
	        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
	        std::cout << "Error during initialization! :\n"
	         << message << "\n";
	    xercesc::XMLString::release(&message);
	    return false;
	}
	
	
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	xercesc::DOMBuilder* parser = ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	xercesc::DOMDocument *doc = 0;

	
	try {
	    doc = parser->parseURI(filename.c_str());  
	}
	catch (const xercesc::XMLException& toCatch) {
		parser->release();
//		delete impl;
		//xercesc::XMLString::release(**tempStr);
		return false;
	}
	
	if (!doc) {
		parser->release();
		return false;
	}
	
	
	xercesc::DOMElement* root = doc->getDocumentElement();
	

	 

	xercesc::XMLString::transcode("modelmapping", tempStr, 99);
	xercesc::DOMNodeList* modelblockNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < modelblockNodes->getLength(); ++i) {
		
		xercesc::XMLString::transcode("model", tempStr, 99);
		std::string model = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));
		
		xercesc::XMLString::transcode("blocktype", tempStr, 99);
		std::string blocktype = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));
		
		mModelMappings[blocktype] = model;
/*		xercesc::XMLString::transcode("model", tempStr, 99);
		modelBlockDef->mModelName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));*/
				
	}
	
	parser->release();

	return true;

}


bool Jesus::loadBlockSpec(const std::string& filename)
{
	if (filename == "") {
		return false;
	}
	
	try {
	        xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
	        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
	        std::cout << "Error during initialization! :\n"
	         << message << "\n";
	    xercesc::XMLString::release(&message);
	    return false;
	}
	
	
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	xercesc::DOMBuilder* parser = ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	xercesc::DOMDocument *doc = 0;

	
	try {
	    doc = parser->parseURI(filename.c_str());  
	}
	catch (const xercesc::XMLException& toCatch) {
		parser->release();
//		delete impl;
		//xercesc::XMLString::release(**tempStr);
		return false;
	}
	
	if (!doc) {
		parser->release();
		return false;
	}
	
	
	xercesc::DOMElement* root = doc->getDocumentElement();
	
 

	xercesc::XMLString::transcode("blockspec", tempStr, 99);
	xercesc::DOMNodeList* blockspecNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < blockspecNodes->getLength(); ++i) {
		xercesc::XMLString::transcode("name", tempStr, 99);
		std::string name = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(blockspecNodes->item(i))->getAttribute(tempStr));
		
		Carpenter::BlockSpec *blockSpec = mCarpenter->createBlockSpec(name);

		xercesc::XMLString::transcode("bbox", tempStr, 99);
//		xercesc::DOMNodeList* partsNodes = dynamic_cast<xercesc::DOMElement*>(submodelsNodes->item(i))->getElementsByTagName(tempStr);
		xercesc::DOMNodeList* bboxNodes = dynamic_cast<xercesc::DOMElement*>(blockspecNodes->item(i))->getElementsByTagName(tempStr);
		
		if (bboxNodes->getLength()) {
			xercesc::DOMElement* bboxNode = dynamic_cast<xercesc::DOMElement*>(bboxNodes->item(0));
			
			WFMath::Point<3> bboxPoint1;
			
			fillFromElement(bboxNode, bboxPoint1);
			WFMath::Point<3> bboxPoint2(-bboxPoint1.x(),-bboxPoint1.y(),-bboxPoint1.z());
			
			WFMath::AxisBox<3> bbox(bboxPoint1, bboxPoint2);
			blockSpec->setBoundingBox(bbox);
		}
		 
				
		xercesc::XMLString::transcode("attachpair", tempStr, 99);
		xercesc::DOMNodeList* attachpairNodes = dynamic_cast<xercesc::DOMElement*>(blockspecNodes->item(i))->getElementsByTagName(tempStr);
		for (unsigned int i = 0; i < attachpairNodes->getLength(); ++i) {
/*			pair->point1.mAttachPair = pair;
			pair->point2.mAttachPair = pair;*/
			
			
			
			xercesc::XMLString::transcode("name", tempStr, 99);
			std::string name = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(attachpairNodes->item(i))->getAttribute(tempStr));

			xercesc::XMLString::transcode("type", tempStr, 99);
			std::string type = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(attachpairNodes->item(i))->getAttribute(tempStr));
			
			addAttachPointType(type);
			
			xercesc::XMLString::transcode("attachpoint", tempStr, 99);
			xercesc::DOMNodeList* attachpointNodes = dynamic_cast<xercesc::DOMElement*>(attachpairNodes->item(i))->getElementsByTagName(tempStr);

			
				xercesc::DOMElement* point1Node = dynamic_cast<xercesc::DOMElement*>(attachpointNodes->item(0));
				
				xercesc::XMLString::transcode("name", tempStr, 99);
				std::string name1 = xercesc::XMLString::transcode(point1Node->getAttribute(tempStr));
				
				xercesc::XMLString::transcode("normal", tempStr, 99);
				xercesc::DOMElement* point1NormalNode = dynamic_cast<xercesc::DOMElement*>(point1Node->getElementsByTagName(tempStr)->item(0));
				
				xercesc::XMLString::transcode("type", tempStr, 99);
				std::string normalType1 = xercesc::XMLString::transcode(point1NormalNode->getAttribute(tempStr));
				
				WFMath::Vector<3> normal1 = mNormalTypes[normalType1];
				
	
				xercesc::XMLString::transcode("position", tempStr, 99);
				xercesc::DOMElement* point1PositionNode = dynamic_cast<xercesc::DOMElement*>(point1Node->getElementsByTagName(tempStr)->item(0));
				
				WFMath::Point<3> position1;
				fillFromElement(point1PositionNode, position1);
				
				Carpenter::AttachPoint point1(name1, position1, normal1);
				
				
				xercesc::DOMElement* point2Node = dynamic_cast<xercesc::DOMElement*>(attachpointNodes->item(1));
				
				xercesc::XMLString::transcode("name", tempStr, 99);
				std::string name2 = xercesc::XMLString::transcode(point2Node->getAttribute(tempStr));
				
				xercesc::XMLString::transcode("normal", tempStr, 99);
				xercesc::DOMElement* point2NormalNode = dynamic_cast<xercesc::DOMElement*>(point2Node->getElementsByTagName(tempStr)->item(0));
				
				xercesc::XMLString::transcode("type", tempStr, 99);
				std::string normalType2 = xercesc::XMLString::transcode(point2NormalNode->getAttribute(tempStr));
				
				WFMath::Vector<3> normal2 = mNormalTypes[normalType2];
				
	
				xercesc::XMLString::transcode("position", tempStr, 99);
				xercesc::DOMElement* point2PositionNode = dynamic_cast<xercesc::DOMElement*>(point2Node->getElementsByTagName(tempStr)->item(0));
				
				WFMath::Point<3> position2;
				fillFromElement(point2PositionNode, position2);
				
				Carpenter::AttachPoint point2(name2, position2, normal2);
				
			
			Carpenter::AttachPair* pair = new Carpenter::AttachPair(name, type, point1, point2);
			
//			blockSpec->addAttachPair(name, type, point1, point2);
			blockSpec->addAttachPair(pair);
			
		}
		

	}
	
	parser->release();

	return true;
}


bool Jesus::loadBuildingBlockSpecDefinition(const std::string& filename)
{
	if (filename == "") {
		return false;
	}
	
	try {
	        xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
	        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
	        std::cout << "Error during initialization! :\n"
	         << message << "\n";
	    xercesc::XMLString::release(&message);
	    return false;
	}
	
	
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	xercesc::DOMBuilder* parser = ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	xercesc::DOMDocument *doc = 0;

	
	try {
	    doc = parser->parseURI(filename.c_str());  
	}
	catch (const xercesc::XMLException& toCatch) {
		parser->release();
//		delete impl;
		//xercesc::XMLString::release(**tempStr);
		return false;
	}
	
	if (!doc) {
		parser->release();
		return false;
	}
	
	
	xercesc::DOMElement* root = doc->getDocumentElement();
	

	 

	xercesc::XMLString::transcode("buildingblockspec", tempStr, 99);
	xercesc::DOMNodeList* modelblockNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < modelblockNodes->getLength(); ++i) {
		Carpenter::BuildingBlockSpecDefinition buildingBlockSpecDef;
		xercesc::XMLString::transcode("name", tempStr, 99);
		buildingBlockSpecDef.mName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));
		
		
		
		
		xercesc::XMLString::transcode("blockspec", tempStr, 99);
		buildingBlockSpecDef.mBlockSpecName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));

		mCarpenter->createBuildingBlockSpec(buildingBlockSpecDef);
/*		xercesc::XMLString::transcode("model", tempStr, 99);
		modelBlockDef->mModelName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));*/
				
	}
	
	parser->release();

	return true;

}

void Jesus::addAttachPointType(const std::string & type) 
{
	
	AttachPointColourValueMap::iterator J = mColourMap.find(type);
	if (J == mColourMap.end()) {
		Ogre::ColourValue colour = Ogre::ColourValue(0.5,0.5,0.1 * mColourMap.size());
		mColourMap.insert(AttachPointColourValueMap::value_type(type, colour));
	} 

}

template <typename T>
void Jesus::fillFromElement(xercesc::DOMElement* element, T& geometryObject)
{
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("x", tempStr, 99);
	WFMath::CoordType x = atof(xercesc::XMLString::transcode(element->getAttribute(tempStr)));
	xercesc::XMLString::transcode("y", tempStr, 99);
	WFMath::CoordType y = atof(xercesc::XMLString::transcode(element->getAttribute(tempStr)));
	xercesc::XMLString::transcode("z", tempStr, 99);
	WFMath::CoordType z = atof(xercesc::XMLString::transcode(element->getAttribute(tempStr)));
	
	geometryObject.x() = x;
	geometryObject.y() = y;
	geometryObject.z() = z;
	
	geometryObject.setValid(true);


}


Carpenter::BluePrint* Jesus::loadBlueprint(std::string filename)
{
	if (filename == "") {
		return 0;
	}
	
	try {
	        xercesc::XMLPlatformUtils::Initialize();
	}
	catch (const xercesc::XMLException& toCatch) {
	        char* message = xercesc::XMLString::transcode(toCatch.getMessage());
	        std::cout << "Error during initialization! :\n"
	         << message << "\n";
	    xercesc::XMLString::release(&message);
	    return 0;
	}
	
	
	XMLCh tempStr[100];
	xercesc::XMLString::transcode("LS", tempStr, 99);
	xercesc::DOMImplementation *impl = xercesc::DOMImplementationRegistry::getDOMImplementation(tempStr);
	xercesc::DOMBuilder* parser = ((xercesc::DOMImplementationLS*)impl)->createDOMBuilder(xercesc::DOMImplementationLS::MODE_SYNCHRONOUS, 0);
	
	xercesc::DOMDocument *doc = 0;

	
	try {
	    doc = parser->parseURI(filename.c_str());  
	}
	catch (const xercesc::XMLException& toCatch) {
		parser->release();
//		delete impl;
		//xercesc::XMLString::release(**tempStr);
		return 0;
	}
	
	if (!doc) {
		parser->release();
		return 0;
	}
	
	
	xercesc::DOMElement* root = doc->getDocumentElement();
	
	xercesc::XMLString::transcode("startingblock", tempStr, 99);
	std::string startingBlockName = xercesc::XMLString::transcode(root->getAttribute(tempStr));

	
	Carpenter::BluePrint* blueprint = mCarpenter->createBlueprint(filename);
	 

	xercesc::XMLString::transcode("buildingblock", tempStr, 99);
	xercesc::DOMNodeList* buildingblocksNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < buildingblocksNodes->getLength(); ++i) {
		
		xercesc::XMLString::transcode("name", tempStr, 99);
		std::string blockDefName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(buildingblocksNodes->item(i))->getAttribute(tempStr));
		
/*		blueprint->mBuildingBlocks[blockDefName];
		BuildingBlock *bBlock = &blueprint->mBuildingBlocks[blockDefName];*/
		//bBlock->mBlockDefinition = *bBlockDef;
		Carpenter::BuildingBlockDefinition bBlockDef;
		
		bBlockDef.mName = blockDefName;
		
		xercesc::XMLString::transcode("blocktype", tempStr, 99);
		bBlockDef.mBuildingBlockSpec = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(buildingblocksNodes->item(i))->getAttribute(tempStr));
		
		
// 		BuildingBlockSpeckDefinition *modelBlockDefinition = &mBuildingBlockSpecDefinitions[bBlockDef->mBuildingBlockSpec];
		
		
// 		BuildingBlockSpec *modelBlock = &bBlock->mBuildingBlockSpec;
// 		modelBlock->mBlockSpec = &(mBlockSpecs[modelBlockDefinition->mBlockSpecName]);
// 		modelBlock->mDefinition = modelBlockDefinition;
// 		if (modelBlockDefinition->mModelName != "") {
// 			modelBlock->mModel = EmberOgre::Model::Create(modelBlockDefinition->mModelName + ".modeldef.xml", bBlockDef->mName);
// 		}
// 		
		blueprint->createBuildingBlock(bBlockDef);
		
		if (bBlockDef.mName == startingBlockName) {
			blueprint->setStartingBlock(startingBlockName);
		}
	}
	
	xercesc::XMLString::transcode("binding", tempStr, 99);
	xercesc::DOMNodeList* bindingNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < bindingNodes->getLength(); ++i) {
		xercesc::DOMElement* bindingNode = dynamic_cast<xercesc::DOMElement*>(bindingNodes->item(i));
		
		Carpenter::BuildingBlockBindingDefinition bindingDef;
		
		xercesc::XMLString::transcode("block1", tempStr, 99);
		bindingDef.mBlock1Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
		xercesc::XMLString::transcode("point1", tempStr, 99);
		bindingDef.mPoint1Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
		xercesc::XMLString::transcode("block2", tempStr, 99);
		bindingDef.mBlock2Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
		xercesc::XMLString::transcode("point2", tempStr, 99);
		bindingDef.mPoint2Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
	
		
		
		//HACK
		size_t slashposition;
		slashposition = bindingDef.mPoint1Name.find('/');
		bindingDef.mPair1Name = bindingDef.mPoint1Name.substr(0, slashposition);
		bindingDef.mPoint1Name = bindingDef.mPoint1Name.substr(slashposition + 1, bindingDef.mPoint1Name.size() - slashposition);

		slashposition = bindingDef.mPoint2Name.find('/');
		bindingDef.mPair2Name = bindingDef.mPoint2Name.substr(0, slashposition);
		bindingDef.mPoint2Name = bindingDef.mPoint2Name.substr(slashposition + 1, bindingDef.mPoint2Name.size() - slashposition);
		
		blueprint->addBinding(bindingDef);
				
	}
	
	parser->release();
	
	return blueprint;


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

ModelBlock* Construction::createModelBlock(Carpenter::BuildingBlock* buildingBlock, bool createAttachPointNodes)
{
	std::string blockSpecName = buildingBlock->getBuildingBlockSpec()->getName();
	
	Model* model = mJesus->createModelForBlockType(blockSpecName, buildingBlock->getName());
	ModelBlock* modelBlock = new ModelBlock(mBaseNode, buildingBlock, model, this);
	
	if (createAttachPointNodes) 
		modelBlock->createAttachPointNodes();
	mModelBlocks.push_back(modelBlock);
	return modelBlock;	
}

std::vector<ModelBlock*> Construction::getModelBlocks() const
{
	return mModelBlocks;
}

std::vector<AttachPointNode*> ModelBlock::getAttachPointNodes() const
{
	return mAttachPointNodes;
}


Model* Jesus::createModelForBlockType(const std::string& blockType, const std::string& modelName)
{
	std::map<std::string, std::string>::iterator I = mModelMappings.find(blockType);
	if (I == mModelMappings.end()) {
		return 0;
	}
	return Model::Create(I->second + ".modeldef.xml", modelName);
}

ModelBlock::~ModelBlock()
{
	EmberOgre::getSingleton().getSceneManager()->removeBillboardSet(mPointBillBoardSet);
	mNode->getParent()->removeChild(mNode);
	EmberOgre::getSingleton().getSceneManager()->destroySceneNode (mNode->getName());
	if (mModelNode) 
		EmberOgre::getSingleton().getSceneManager()->destroySceneNode (mModelNode->getName());
		
	if (mModel) 
		delete mModel;
		
	std::vector<AttachPointNode* >::iterator I = mAttachPointNodes.begin();
	std::vector<AttachPointNode* >::iterator I_end = mAttachPointNodes.end();
	for(; I != I_end; ++I) {
 		delete *I;
	}

	
}


ModelBlock::ModelBlock(Ogre::SceneNode* baseNode, Carpenter::BuildingBlock* buildingBlock, Model* model, Construction* construction)
: mBuildingBlock(buildingBlock), mModel(model), mConstruction(construction), mModelNode(0)
{
	
	
	mNode = baseNode->createChildSceneNode(Atlas2Ogre(buildingBlock->getPosition()), Atlas2Ogre(buildingBlock->getOrientation()));
	
	
	mPointBillBoardSet = EmberOgre::getSingleton().getSceneManager()->createBillboardSet( buildingBlock->getName() + "_billboardset");
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




