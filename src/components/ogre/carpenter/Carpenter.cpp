//
// C++ Implementation: Carpenter
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
#include "Carpenter.h"

#include <wfmath/error.h>

namespace Carpenter {










Carpenter::Carpenter()
{
	mNormalTypes["NORTH"] = WFMath::Vector<3>(0,0,1);
	mNormalTypes["SOUTH"] = WFMath::Vector<3>(0,0,-1);
	mNormalTypes["WEST"] = WFMath::Vector<3>(-1,0,0);
	mNormalTypes["EAST"] = WFMath::Vector<3>(1,0,0);
	mNormalTypes["UP"] = WFMath::Vector<3>(0,1,0);
	mNormalTypes["DOWN"] = WFMath::Vector<3>(0,-1,0);
}


Carpenter::~Carpenter()
{
}
	
BuildingBlock::BuildingBlock() 
: mAttached(false)
{
mRotation.identity();

}

void BluePrint::doBindingsForBlock(BuildingBlock *block)
{
	std::map<BuildingBlock* , std::vector<BuildingBlockBinding*> > relatedBindings;

	std::vector< BuildingBlockBinding>::iterator I = mBindings.begin();
	std::vector< BuildingBlockBinding>::iterator I_end = mBindings.end();
		
	for (;I != I_end; ++I) {
		BuildingBlock* unboundBlock = 0;
		AttachPair* pair = 0;
		BuildingBlock* block1 = (*I).mBlock1;
		BuildingBlock* block2 = (*I).mBlock2;
		if (block1 == block && !block2->isAttached()) {
			unboundBlock = block2;
			pair = (*I).mPoint2->mAttachPair;
		} else if (block2 == block && !block1->isAttached()) {
			unboundBlock = block1;
			pair = (*I).mPoint1->mAttachPair;
		}
		
		if (unboundBlock) {
			relatedBindings[unboundBlock].push_back(&(*I));
			if (relatedBindings[unboundBlock].size() > 1 && !unboundBlock->isAttached()) {
				placeBindings(unboundBlock, relatedBindings[unboundBlock]);
				doBindingsForBlock(unboundBlock);
			}
		}
	}
}

void BluePrint::compile(Ogre::SceneNode* baseNode)
{

	BuildingBlock* baseBlock = mStartingBlock;
	mStartingBlock->mAttached = true;
	doBindingsForBlock(mStartingBlock);

// 	std::vector< BuildingBlockBinding>::iterator I = mBindings.begin();
// 	std::vector< BuildingBlockBinding>::iterator I_end = mBindings.end();
// 	
// 	baseBlock = (*I).mBlock1;
// 	baseBlock->mPosition = WFMath::Point<3>(0,0,0);
// 	baseBlock->mRotation.identity();
// 
// 	int bound = 0;
// 		
// 	placeBinding(&(*I));
// 	++bound;
// 	
// 	int boundBeforeIteration = bound;
// 	bool doContinue;
// 	
// 	while (doContinue) {
// 		for (;I != I_end; ++I) {
// 			if ((*I).mBlock1->isAttached()) {
// 				placeBinding(&(*I));
// 				++bound;
// 			}
// 			
// 		}
// 		if (bound > boundBeforeIteration) {
// 			doContinue = true;
// 		} else {
// 			doContinue = false;
// 		}
// 	}
	
	//now iterate over them all and create the SceneNodes
	//TODO: move this to another class
	std::map<const std::string,  BuildingBlock>::iterator J = mBuildingBlocks.begin();
	std::map<const std::string,  BuildingBlock>::iterator J_end = mBuildingBlocks.end();
	
	for (;J != J_end; ++J)
	{
		BuildingBlock* block = &J->second;
		if (block->mAttached) {
			Ogre::SceneNode* node = baseNode->createChildSceneNode(EmberOgre::Atlas2Ogre(block->mPosition), EmberOgre::Atlas2Ogre(block->mRotation));
			
			
			if (block->getModel()) {
				EmberOgre::Model* model = block->getModel();
				node->attachObject(block->getModel());
				
				const Ogre::AxisAlignedBox ogreBoundingBox = model->getBoundingBox();
				const Ogre::Vector3 ogreMax = ogreBoundingBox.getMaximum();
				const Ogre::Vector3 ogreMin = ogreBoundingBox.getMinimum();
			
		
				const WFMath::AxisBox<3> wfBoundingBox = block->mModelBlock.mBlockSpec->mBoundingBox;	
				const WFMath::Point<3> wfMax = wfBoundingBox.highCorner();
				const WFMath::Point<3> wfMin = wfBoundingBox.lowCorner();
				
				Ogre::Real scaleX;		
				Ogre::Real scaleY;		
				Ogre::Real scaleZ;		
			
				scaleX = fabs((wfMax.x() - wfMin.x()) / (ogreMax.x - ogreMin.x));		
				scaleY = fabs((wfMax.z() - wfMin.z()) / (ogreMax.y - ogreMin.y));		
				scaleZ = fabs((wfMax.y() - wfMin.y()) / (ogreMax.z - ogreMin.z));		

				node->setScale(scaleX, scaleY, scaleZ);
				
			}
		}
	}
	
	

}

const AttachPoint* AttachPoint::getSibling() const
{
	if (&mAttachPair->point1 == this) {
		return &mAttachPair->point2;
	}
	return &mAttachPair->point1;
}

WFMath::Point<3> BuildingBlock::getWorldPositionForPoint(const AttachPoint* point)
{
	WFMath::Vector<3> worldPoint = point->mPosition - WFMath::Point<3>(0,0,0);
	worldPoint.rotate(mRotation);
	return mPosition + worldPoint;
}


void BluePrint::placeBindings(BuildingBlock* unboundBlock, std::vector<BuildingBlockBinding*>& bindings)
{
	BuildingBlockBinding* binding1 = bindings[0];
	BuildingBlockBinding* binding2 = bindings[1];
	
	BuildingBlock * boundBlock;
	
	const AttachPoint* boundPoint1;
	const AttachPoint* boundPoint2;
	const AttachPoint* unboundPoint1;
	const AttachPoint* unboundPoint2;

	if (binding1->mBlock1->isAttached()) {
		boundBlock = binding1->mBlock1;
//		unboundBlock = binding1->mBlock2;
		boundPoint1 = binding1->mPoint1;
		unboundPoint1 = binding1->mPoint2;
	} else {
		boundBlock = binding1->mBlock2;
//		unboundBlock = binding1->mBlock1;
		boundPoint1 = binding1->mPoint2;
		unboundPoint1 = binding1->mPoint1;
	}
	
	if (binding2->mBlock1->isAttached()) {
		boundPoint2 = binding2->mPoint1;
		unboundPoint2 = binding2->mPoint2;
	} else {
		boundPoint2 = binding2->mPoint2;
		unboundPoint2 = binding2->mPoint1;
	}
	
	WFMath::Vector<3> boundPointNormal = boundPoint1->mPosition - boundPoint2->mPosition;
	boundPointNormal.rotate(boundBlock->mRotation);
	WFMath::Vector<3> unboundPointNormal = unboundPoint1->mPosition - unboundPoint2->mPosition;
	
	//we need the quaternion needed to rotate unboundPointNormal (and thus the whole unboundBlock) to point in the same direction as boundPointNormal
	WFMath::Quaternion neededRotation;
	neededRotation.identity();
	try {
		neededRotation.rotation(unboundPointNormal, boundPointNormal);
	} catch (WFMath::ColinearVectors<3> & e) {
/*		neededRotation = boundBlock->mRotation;
		neededRotation.inverse();	*/
		WFMath::Vector<3> upVector(0,0,1);
		upVector.setValid(true);
		neededRotation = WFMath::Quaternion(upVector, 3.1415926535898 );
	}

	//do the rotation
	unboundBlock->mRotation *= neededRotation;

	unboundBlock->mAttached = true;
	
	//we now have to position the block
	WFMath::Vector<3> distance = boundBlock->getWorldPositionForPoint(boundPoint1) - unboundBlock->getWorldPositionForPoint(unboundPoint1);
	unboundBlock->mPosition = unboundBlock->mPosition + distance;
	
	
	unboundBlock->mBoundPoints.push_back(unboundPoint1);
	unboundBlock->mBoundPoints.push_back(unboundPoint2);
	boundBlock->mBoundPoints.push_back(boundPoint1);
	boundBlock->mBoundPoints.push_back(boundPoint2);
	
	
	
	
	
	
	
	
	
		
	
	
// 	//we need the world normal of point1
// 	WFMath::Vector<3> worldNormal1 = point1->mNormal;
// 	worldNormal1.rotate(block1->mRotation);
// 	
// 	//first check if the other AttachPoint of block2 is already bound, if so, we don't have to reposition the block, only align the AttachPoint
// 	
// 	const AttachPoint* sibling = point2->getSibling();
// 	std::vector<const AttachPoint*>::iterator I_end = block2->mBoundPoints.end();
// 	bool found = false;
// 	for (std::vector<const AttachPoint*>::iterator I = block2->mBoundPoints.begin();I != I_end;++I) {
// 		if (*I == sibling) {
// 			found = true;
// 			break;
// 		}
// 	}
// 
// 	if (!found) {
// 		
// 		//rotate the block so that block2's point's normal is the inverse of block1's point's normal
// 		
// 		
// 		WFMath::Vector<3> worldNormal2 = point2->mNormal;
// 		worldNormal2.rotate(block2->mRotation);
// 		
// 		//we need the quaternion needed to rotate worldNormal2 (and thus the whole block2) to point in the same direction as the inverse of worldNormal1
// 		//(remember, two attachpoints can only attach if their normals are inverse of each other)
// 		WFMath::Quaternion neededRotation;
// 		neededRotation.rotation(worldNormal2, -worldNormal1);
// 		
// 		//do the rotation
// 		block2->mRotation = neededRotation;
// 		
// 		//we now have to position the block
// 		WFMath::Vector<3> distance = block1->getWorldPositionForPoint(point1) - block2->getWorldPositionForPoint(point2);
// 		block2->mPosition = block2->mPosition + distance;
// 	} else {
// 		//just rotate the block along the sibling's normal until point2's position is over point1
// 		//TODO: implement this
// 	
// 	}
// 		
// 		
// 	block1->mBoundPoints.push_back(point1);
// 	block2->mBoundPoints.push_back(point2);
// 	
// 	block2->mAttached = true;


} 


BluePrint* Carpenter::loadBlueprint(std::string filename)
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
	
	xercesc::XMLString::transcode("strartingblock", tempStr, 99);
	std::string startingBlockName = xercesc::XMLString::transcode(root->getAttribute(tempStr));

	
	BluePrint* blueprint = new BluePrint();
	 

	xercesc::XMLString::transcode("buildingblock", tempStr, 99);
	xercesc::DOMNodeList* buildingblocksNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < buildingblocksNodes->getLength(); ++i) {
		
		xercesc::XMLString::transcode("name", tempStr, 99);
		std::string blockDefName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(buildingblocksNodes->item(i))->getAttribute(tempStr));
		
		blueprint->mBuildingBlocks[blockDefName];
		BuildingBlock *bBlock = &blueprint->mBuildingBlocks[blockDefName];
		//bBlock->mBlockDefinition = *bBlockDef;
		BuildingBlockDefinition *bBlockDef = &bBlock->mBlockDefinition;
		
		bBlockDef->mName = blockDefName;
		
		xercesc::XMLString::transcode("modelblock", tempStr, 99);
		bBlockDef->mModelBlock = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(buildingblocksNodes->item(i))->getAttribute(tempStr));
		
		if (bBlockDef->mName == startingBlockName) {
			blueprint->mStartingBlock = bBlock;
		}
		
		ModelBlockDefinition *modelBlockDefinition = &mModelBlockDefinitions[bBlockDef->mModelBlock];
		ModelBlock *modelBlock = &bBlock->mModelBlock;
		modelBlock->mBlockSpec = &(mBlockSpecs[modelBlockDefinition->mBlockSpecName]);
		modelBlock->mDefinition = modelBlockDefinition;
		modelBlock->mModel = EmberOgre::Model::Create(modelBlockDefinition->mModelName + ".modeldef.xml", bBlockDef->mName);
		
	}
	
	xercesc::XMLString::transcode("binding", tempStr, 99);
	xercesc::DOMNodeList* bindingNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < bindingNodes->getLength(); ++i) {
		xercesc::DOMElement* bindingNode = dynamic_cast<xercesc::DOMElement*>(bindingNodes->item(i));
		
		BuildingBlockBindingDefinition *bindingDef = new BuildingBlockBindingDefinition();
		
		xercesc::XMLString::transcode("block1", tempStr, 99);
		bindingDef->mBlock1Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
		xercesc::XMLString::transcode("point1", tempStr, 99);
		bindingDef->mPoint1Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
		xercesc::XMLString::transcode("block2", tempStr, 99);
		bindingDef->mBlock2Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
		xercesc::XMLString::transcode("point2", tempStr, 99);
		bindingDef->mPoint2Name = xercesc::XMLString::transcode(bindingNode->getAttribute(tempStr));
	
		
		BuildingBlockBinding *binding = new BuildingBlockBinding();
		binding->mDefinition = *bindingDef;
		binding->mBlock1 = &blueprint->mBuildingBlocks[bindingDef->mBlock1Name];
		binding->mBlock2 = &blueprint->mBuildingBlocks[bindingDef->mBlock2Name];
		
		
		//HACK
		size_t slashposition;
		slashposition = bindingDef->mPoint1Name.find('/');
		std::string pair1Name = bindingDef->mPoint1Name.substr(0, slashposition);
		std::string point1Name = bindingDef->mPoint1Name.substr(slashposition + 1, bindingDef->mPoint1Name.size() - slashposition);
		const AttachPair *pair1 = binding->mBlock1->getAttachPair(pair1Name);
		binding->mPoint1 = pair1->getAttachPoint(point1Name);

		slashposition = bindingDef->mPoint2Name.find('/');
		std::string pair2Name = bindingDef->mPoint2Name.substr(0, slashposition);
		std::string point2Name = bindingDef->mPoint2Name.substr(slashposition + 1, bindingDef->mPoint2Name.size() - slashposition);
		const AttachPair *pair2 = binding->mBlock2->getAttachPair(pair2Name);
		binding->mPoint2 = pair2->getAttachPoint(point2Name);
		
		blueprint->mBindings.push_back(*binding);
				
	}
	
	parser->release();
	
	return blueprint;


}

const std::string& BuildingBlockBinding::getType() const
{
	return mPoint1->mAttachPair->mType;
}


const AttachPair* BuildingBlock::getAttachPair(const std::string& name)
{
	BlockSpec* spec = mModelBlock.mBlockSpec;
	return spec->getAttachPair(name);
	
}

bool Carpenter::loadModelBlockDefinition(const std::string& filename)
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
	

	 

	xercesc::XMLString::transcode("modelblock", tempStr, 99);
	xercesc::DOMNodeList* modelblockNodes = doc->getElementsByTagName(tempStr);
	//std::cout << "Number of submodel nodes: " << submodelsNodes->getLength() << "\n";
	
	for (unsigned int i = 0; i < modelblockNodes->getLength(); ++i) {
		ModelBlockDefinition *modelBlockDef = new ModelBlockDefinition();
		
		xercesc::XMLString::transcode("name", tempStr, 99);
		modelBlockDef->mName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));
		
		xercesc::XMLString::transcode("blockspec", tempStr, 99);
		modelBlockDef->mBlockSpecName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));

		xercesc::XMLString::transcode("model", tempStr, 99);
		modelBlockDef->mModelName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(modelblockNodes->item(i))->getAttribute(tempStr));
				
		mModelBlockDefinitions[modelBlockDef->mName] = *modelBlockDef;
	}
	
	parser->release();

	return true;

}

bool Carpenter::loadBlockSpec(const std::string& filename)
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
		BlockSpec *blockSpec = new BlockSpec();
		
		xercesc::XMLString::transcode("name", tempStr, 99);
		blockSpec->mName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(blockspecNodes->item(i))->getAttribute(tempStr));
		

		xercesc::XMLString::transcode("bbox", tempStr, 99);
//		xercesc::DOMNodeList* partsNodes = dynamic_cast<xercesc::DOMElement*>(submodelsNodes->item(i))->getElementsByTagName(tempStr);
		xercesc::DOMNodeList* bboxNodes = dynamic_cast<xercesc::DOMElement*>(blockspecNodes->item(i))->getElementsByTagName(tempStr);
		xercesc::DOMElement* bboxNode = dynamic_cast<xercesc::DOMElement*>(bboxNodes->item(0));
		
		WFMath::Point<3> bboxPoint1;

		
		fillFromElement(bboxNode, bboxPoint1);
		WFMath::Point<3> bboxPoint2(-bboxPoint1.x(),-bboxPoint1.y(),-bboxPoint1.z());
		
		WFMath::AxisBox<3> bbox(bboxPoint1, bboxPoint2);
		blockSpec->mBoundingBox = bbox;
		 
				
		xercesc::XMLString::transcode("attachpair", tempStr, 99);
		xercesc::DOMNodeList* attachpairNodes = dynamic_cast<xercesc::DOMElement*>(blockspecNodes->item(i))->getElementsByTagName(tempStr);
		for (unsigned int i = 0; i < attachpairNodes->getLength(); ++i) {
			AttachPair* pair = new AttachPair();
			pair->point1.mAttachPair = pair;
			pair->point2.mAttachPair = pair;
			
			
			xercesc::XMLString::transcode("name", tempStr, 99);
			pair->mName = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(attachpairNodes->item(i))->getAttribute(tempStr));

			xercesc::XMLString::transcode("type", tempStr, 99);
			pair->mType = xercesc::XMLString::transcode(dynamic_cast<xercesc::DOMElement*>(attachpairNodes->item(i))->getAttribute(tempStr));
			
			xercesc::XMLString::transcode("attachpoint", tempStr, 99);
			xercesc::DOMNodeList* attachpointNodes = dynamic_cast<xercesc::DOMElement*>(attachpairNodes->item(i))->getElementsByTagName(tempStr);

			{			
				xercesc::DOMElement* point1Node = dynamic_cast<xercesc::DOMElement*>(attachpointNodes->item(0));
				AttachPoint * point1 = &(pair->point1);
				
				xercesc::XMLString::transcode("name", tempStr, 99);
				point1->mName = xercesc::XMLString::transcode(point1Node->getAttribute(tempStr));
				
				xercesc::XMLString::transcode("normal", tempStr, 99);
				xercesc::DOMElement* point1NormalNode = dynamic_cast<xercesc::DOMElement*>(point1Node->getElementsByTagName(tempStr)->item(0));
				
				xercesc::XMLString::transcode("type", tempStr, 99);
				std::string normalType = xercesc::XMLString::transcode(point1NormalNode->getAttribute(tempStr));
				
				
				point1->mNormal = mNormalTypes[normalType];
				
	
				xercesc::XMLString::transcode("position", tempStr, 99);
				xercesc::DOMElement* point1PositionNode = dynamic_cast<xercesc::DOMElement*>(point1Node->getElementsByTagName(tempStr)->item(0));
				
				
/*				xercesc::XMLString::transcode("x", tempStr, 99);
				WFMath::CoordType x = atof(xercesc::XMLString::transcode(point1PositionNode->getAttribute(tempStr)));
				xercesc::XMLString::transcode("y", tempStr, 99);
				WFMath::CoordType y = atof(xercesc::XMLString::transcode(point1PositionNode->getAttribute(tempStr)));
				xercesc::XMLString::transcode("z", tempStr, 99);
				WFMath::CoordType z = atof(xercesc::XMLString::transcode(point1PositionNode->getAttribute(tempStr)));*/
				
/*				WFMath::Vector<3> position(x,y,z);*/
				fillFromElement(point1PositionNode, point1->mPosition);
				
			}
			

			{			
				xercesc::DOMElement* point2Node = dynamic_cast<xercesc::DOMElement*>(attachpointNodes->item(1));
				AttachPoint * point2 = &(pair->point2);
				
				xercesc::XMLString::transcode("name", tempStr, 99);
				point2->mName = xercesc::XMLString::transcode(point2Node->getAttribute(tempStr));
				
				xercesc::XMLString::transcode("normal", tempStr, 99);
				xercesc::DOMElement* point2NormalNode = dynamic_cast<xercesc::DOMElement*>(point2Node->getElementsByTagName(tempStr)->item(0));
				
				xercesc::XMLString::transcode("type", tempStr, 99);
				std::string normalType = xercesc::XMLString::transcode(point2NormalNode->getAttribute(tempStr));
				
				point2->mNormal = mNormalTypes[normalType];
				
	
				xercesc::XMLString::transcode("position", tempStr, 99);
				xercesc::DOMElement* point2PositionNode = dynamic_cast<xercesc::DOMElement*>(point2Node->getElementsByTagName(tempStr)->item(0));
				
/*				
				xercesc::XMLString::transcode("x", tempStr, 99);
				WFMath::CoordType x = atof(xercesc::XMLString::transcode(point2PositionNode->getAttribute(tempStr)));
				xercesc::XMLString::transcode("y", tempStr, 99);
				WFMath::CoordType y = atof(xercesc::XMLString::transcode(point2PositionNode->getAttribute(tempStr)));
				xercesc::XMLString::transcode("z", tempStr, 99);
				WFMath::CoordType z = atof(xercesc::XMLString::transcode(point2PositionNode->getAttribute(tempStr)));
				
				WFMath::Vector<3> position(x,y,z);
				point2->mPosition = position;*/
				
				fillFromElement(point2PositionNode, point2->mPosition);
			}
			
			blockSpec->mAttachPairs[pair->mName] = *pair;
			
		}
		mBlockSpecs[blockSpec->mName] = *blockSpec;

	}
	
	parser->release();

	return true;
}

template <typename T>
void Carpenter::fillFromElement(xercesc::DOMElement* element, T& geometryObject)
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

};
