//
// C++ Implementation: XMLJesusSerializer
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "XMLJesusSerializer.h"
#include "Jesus.h"
#include "../carpenter/Carpenter.h"
#include "../carpenter/BluePrint.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include "framework/osdir.h"

#ifdef __WIN32__
#include <direct.h>
#endif

namespace EmberOgre {

XMLJesusSerializer::XMLJesusSerializer(Jesus* jesus)
: mJesus(jesus)
{
}


XMLJesusSerializer::~XMLJesusSerializer()
{
}


bool XMLJesusSerializer::loadModelBlockMapping(const std::string& filename)
{
	if (filename == "" || filename == "." || filename == ".." || std::ifstream(filename.c_str() , std::ios::in ).fail()) {
		return false;
	}

	TiXmlDocument _XMLDoc;
	bool success = _XMLDoc.LoadFile(filename.c_str()); //load from data stream
	
	if (!success) {
		std::string errorDesc = _XMLDoc.ErrorDesc();
		int errorLine =  _XMLDoc.ErrorRow();
		int errorColumn =  _XMLDoc.ErrorCol();
		std::stringstream ss;
		ss << "Failed to load load model block mapping file '" << filename << "'! Error at column: " << errorColumn << " line: " << errorLine << ". Error message: " << errorDesc;
		S_LOG_FAILURE(ss.str());
		return false;
	}
	

	TiXmlElement* rootElem = _XMLDoc.RootElement();

	for (TiXmlElement* smElem = rootElem->FirstChildElement();
			smElem != 0; smElem = smElem->NextSiblingElement())
	{
		const char* tmp =  smElem->Attribute("model");
		if (!tmp) {
			continue;
		} 

		std::string model(tmp);
		
		tmp =  smElem->Attribute("blocktype");
		if (!tmp) {
			continue;
		} 
		
		std::string blocktype(tmp);
		
		mJesus->mEntityMappings[blocktype] = model;
	}	
	
	return true;

}


template <typename T>
void XMLJesusSerializer::fillFromElement(TiXmlElement* elem, T& geometryObject)
{
	WFMath::CoordType x = atof(elem->Attribute("x"));
	WFMath::CoordType y = atof(elem->Attribute("y"));
	WFMath::CoordType z = atof(elem->Attribute("z"));
	
	geometryObject.x() = x;
	geometryObject.y() = y;
	geometryObject.z() = z;
	
	geometryObject.setValid(true);


}

bool XMLJesusSerializer::loadBlockSpec(const std::string& filename)
{
	
	if (filename == "" || filename == "." || filename == ".." || std::ifstream(filename.c_str() , std::ios::in ).fail()) {
		return false;
	}

	TiXmlDocument _XMLDoc;
	bool success = _XMLDoc.LoadFile(filename.c_str()); //load from data stream
	
	if (!success) {
		std::string errorDesc = _XMLDoc.ErrorDesc();
		int errorLine =  _XMLDoc.ErrorRow();
		int errorColumn =  _XMLDoc.ErrorCol();
		std::stringstream ss;
		ss << "Failed to load load block spec file '" << filename << "'! Error at column: " << errorColumn << " line: " << errorLine << ". Error message: " << errorDesc;
		S_LOG_FAILURE(ss.str());
		return false;
	}
	
	TiXmlElement* elem;


	TiXmlElement* rootElem = _XMLDoc.RootElement();

	for (TiXmlElement* smElem = rootElem->FirstChildElement();
			smElem != 0; smElem = smElem->NextSiblingElement())
	{
	
		const char* tmp =  smElem->Attribute("name");
		if (!tmp) {
			continue;
		} 

		std::string name(tmp);
		
		Carpenter::BlockSpec *blockSpec = mJesus->getCarpenter()->createBlockSpec(name);
		
		elem = smElem->FirstChildElement("bbox");
		
		if (elem)
		{
			WFMath::Point<3> bboxPoint_temp;
			
			fillFromElement(elem, bboxPoint_temp);
			WFMath::Point<3> bboxPoint1(bboxPoint_temp.x() * 0.5, bboxPoint_temp.y() * 0.5, bboxPoint_temp.z() * 0.5);
			WFMath::Point<3> bboxPoint2(-bboxPoint_temp.x() * 0.5, -bboxPoint_temp.y() * 0.5, -bboxPoint_temp.z() * 0.5);
			//(-bboxPoint1.x(),-bboxPoint1.y(),-bboxPoint1.z());
			
			WFMath::AxisBox<3> bbox(bboxPoint1, bboxPoint2);
			blockSpec->setBoundingBox(bbox);
		}
		
		
		elem = smElem->FirstChildElement("attachpairs");
		
		readAttachPairs(blockSpec, elem);

		
	}
	return true;
}
		
		
Carpenter::AttachPoint* XMLJesusSerializer::readAttachPoint(TiXmlElement* elem)
{
	const char* tmp = 0;
	
	tmp =  elem->Attribute("name");
	if (!tmp) {
		return 0;
	} 
	std::string name(tmp);
				
	TiXmlElement* normalElem = elem->FirstChildElement("normal");
		
	tmp =  normalElem->Attribute("type");
	if (!tmp) {
		return 0;
	} 
	std::string type(tmp);

	if (mJesus->mNormalTypes.find(type) == mJesus->mNormalTypes.end()) {
		return 0;
	}
	WFMath::Vector<3> normal = mJesus->mNormalTypes.find(type)->second;

	TiXmlElement* positionElem = elem->FirstChildElement("position");
	if (!positionElem) {
		return 0;
	} 
				
	WFMath::Point<3> position;
	fillFromElement(positionElem, position);
				
	return new Carpenter::AttachPoint(name, position, normal);		
	
		
}

	
void XMLJesusSerializer::readAttachPairs(Carpenter::BlockSpec *blockSpec, TiXmlElement* parentElem)
{

	const char* tmp = 0;

	for (TiXmlElement* elem = parentElem->FirstChildElement();
			elem != 0; elem = elem->NextSiblingElement())
	{
		tmp =  elem->Attribute("name");
		if (!tmp) {
			continue;
		} 
		std::string name(tmp);
		

		tmp =  elem->Attribute("type");
		if (!tmp) {
			continue;
		} 
		std::string type(tmp); 
	
		
		TiXmlElement* attachPointElem = elem->FirstChildElement("attachpoint");
		//make sure that there are two attach points
		if (!attachPointElem->NextSiblingElement()) {
			continue;
		}
		
		Carpenter::AttachPoint* point1 = readAttachPoint(attachPointElem);
		Carpenter::AttachPoint* point2 = readAttachPoint(attachPointElem->NextSiblingElement());
		
		if (!point1 || !point2) {
			delete point1;
			delete point2;
			continue;
		}
		Carpenter::AttachPair* pair = new Carpenter::AttachPair(name, type, *point1, *point2);
		delete point1;
		delete point2;
		
		blockSpec->addAttachPair(pair);
		
		
		mJesus->addAttachPointType(type);
	}

}	



bool XMLJesusSerializer::loadBuildingBlockSpecDefinition(const std::string& filename)
{
	
	if (filename == "" || filename == "." || filename == ".." || std::ifstream(filename.c_str() , std::ios::in ).fail()) {
		return false;
	}

	TiXmlDocument _XMLDoc;
	bool success = _XMLDoc.LoadFile(filename.c_str()); //load from data stream
	
	if (!success) {
		S_LOG_FAILURE("Failed to load modeldefinition file!");
		return false;
	}
	

	TiXmlElement* rootElem = _XMLDoc.RootElement();

	for (TiXmlElement* smElem = rootElem->FirstChildElement();
			smElem != 0; smElem = smElem->NextSiblingElement())
	{
	
		Carpenter::BuildingBlockSpecDefinition buildingBlockSpecDef;
		const char* tmp =  smElem->Attribute("name");
		if (!tmp) {
			continue;
		} 
		buildingBlockSpecDef.mName = tmp;
	
		tmp =  smElem->Attribute("blockspec");
		if (!tmp) {
			continue;
		} 
		buildingBlockSpecDef.mBlockSpecName = tmp;
		
		mJesus->getCarpenter()->createBuildingBlockSpec(buildingBlockSpecDef);
	}

	return true;

}




Carpenter::BluePrint* XMLJesusSerializer::loadBlueprint(const std::string& filename)
{
	if (filename == "" || filename == "." || filename == ".." || std::ifstream(filename.c_str() , std::ios::in ).fail()) {
		return false;
	}

	TiXmlDocument _XMLDoc;
	bool success = _XMLDoc.LoadFile(filename.c_str()); //load from data stream
	
	if (!success) {
		S_LOG_FAILURE("Failed to load modeldefinition file!");
		return 0;
	}
	

	TiXmlElement* rootElem = _XMLDoc.RootElement();
	
	const char* tmp;
	 
	tmp = rootElem->Attribute("startingblock");
	if (!tmp) {
		return 0;
	} 
	std::string startingBlockName(tmp);
	
	tmp = rootElem->Attribute("name");
	if (!tmp) {
		return 0;
	} 
	std::string name(tmp);
	
	
	Carpenter::BluePrint* blueprint = mJesus->getCarpenter()->createBlueprint(name);

	for (TiXmlElement* bbElem = rootElem->FirstChildElement("buildingblocks");
			bbElem != 0; bbElem = bbElem->NextSiblingElement())
	{
		for (TiXmlElement* smElem = bbElem->FirstChildElement("buildingblock");
				   smElem != 0; smElem = smElem->NextSiblingElement())
		{
			tmp = smElem->Attribute("name");
			if (!tmp) {
				continue;
			} 
			std::string blockDefName(tmp);
	
			tmp = smElem->Attribute("blocktype");
			if (!tmp) {
				continue;
			} 
			std::string blockDefType(tmp);
			
					
			Carpenter::BuildingBlockDefinition bBlockDef;
			bBlockDef.mName = blockDefName;
			bBlockDef.mBuildingBlockSpec = blockDefType;
			
			blueprint->createBuildingBlock(bBlockDef);
			
			if (bBlockDef.mName == startingBlockName) {
				blueprint->setStartingBlock(startingBlockName);
			}
		}
	}
	
	for (TiXmlElement* bindingsElem = rootElem->FirstChildElement("bindings");
			bindingsElem != 0; bindingsElem = bindingsElem->NextSiblingElement())
	{
		for (TiXmlElement* smElem = bindingsElem->FirstChildElement("binding");
				smElem != 0; smElem = smElem->NextSiblingElement())
		{
			Carpenter::BuildingBlockBindingDefinition bindingDef;
	
			bindingDef.mBlock1Name = smElem->Attribute("block1");
			bindingDef.mPoint1Name = smElem->Attribute("point1");
			bindingDef.mBlock2Name = smElem->Attribute("block2");
			bindingDef.mPoint2Name = smElem->Attribute("point2");
	
			
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
	}	

	return blueprint;


}

void XMLJesusSerializer::saveBlueprintToFile(Carpenter::BluePrint* blueprint, const std::string& filename)
{
	saveBlueprintToFile(blueprint, filename, blueprint->getName());
}

void XMLJesusSerializer::saveBlueprintToFile(Carpenter::BluePrint* blueprint, const std::string& filename, const std::string& name)
{
	if (filename == "" || filename == "." || filename == "..") {
		return;
	}

	TiXmlDocument _XMLDoc;
	
	
	
	   
	try
	{
		//make sure the directory exists
		std::string dir = Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "/carpenter/blueprints";
		
		oslink::directory osdir(dir);

		if (!osdir) {
#ifdef __WIN32__
			mkdir((Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "carpenter").c_str());
			mkdir(dir.c_str());
#else 
			mkdir((Ember::EmberServices::getSingletonPtr()->getConfigService()->getHomeDirectory() + "carpenter").c_str(), S_IRWXU);
			mkdir(dir.c_str(), S_IRWXU);
#endif
		}
	
	
	
	
		
		TiXmlElement elem("blueprint");
		elem.SetAttribute("startingblock", blueprint->getStartingBlock()->getName().c_str());
		elem.SetAttribute("name", name.c_str());
		
		TiXmlElement buildingBlocksElem("buildingblocks");
		//now iterate over all building blocks
		
		const std::vector< Carpenter::BuildingBlock*> bblocks = blueprint->getAttachedBlocks();
		
		std::vector< Carpenter::BuildingBlock*>::const_iterator I = bblocks.begin();
		std::vector< Carpenter::BuildingBlock*>::const_iterator I_end = bblocks.end();
		
		for (;I != I_end; ++I) {
			TiXmlElement buildingBlockElem("buildingblock");
			buildingBlockElem.SetAttribute("blocktype",  (*I)->getBuildingBlockSpec()->getName().c_str() );
			buildingBlockElem.SetAttribute("name", (*I)->getName().c_str());
			buildingBlocksElem.InsertEndChild(buildingBlockElem);
		}
		
		
		//iterate over the bindings
		TiXmlElement bindingsElem("bindings");
		
		const std::list< Carpenter::BuildingBlockBinding>* bindings = blueprint->getBindings();
		std::list< Carpenter::BuildingBlockBinding>::const_iterator J = bindings->begin();
		std::list< Carpenter::BuildingBlockBinding>::const_iterator J_end = bindings->end();
		
		for (;J != J_end; ++J) {
			TiXmlElement bindingElem("binding");
			bindingElem.SetAttribute("block1", (*J).getBlock1()->getName().c_str());
			bindingElem.SetAttribute("block2", (*J).getBlock2()->getName().c_str());
			const Carpenter::AttachPoint* point1 = (*J).getAttachPoint1();
			std::string point1Name = point1->getAttachPair()->getName() + "/" + point1->getName();
			const Carpenter::AttachPoint* point2 = (*J).getAttachPoint2();
			std::string point2Name = point2->getAttachPair()->getName() + "/" + point2->getName();
			bindingElem.SetAttribute("point1", point1Name.c_str());
			bindingElem.SetAttribute("point2", point2Name.c_str());
				
			bindingsElem.InsertEndChild(bindingElem);
				
		}
		
		elem.InsertEndChild(bindingsElem);
		elem.InsertEndChild(buildingBlocksElem);
		_XMLDoc.InsertEndChild(elem);

		_XMLDoc.SaveFile(filename.c_str());
	}
	catch (...)
	{
		S_LOG_FAILURE("An error occurred creating the document." );
	}

	

	
}



};
