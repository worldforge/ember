//
// C++ Interface: Jesus
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
#ifndef EMBEROGREJESUS_H
#define EMBEROGREJESUS_H

//we must include xerces stuff before ogre stuff, because else we'll get errors when compiling in debug mode
//this seems to be because both uses their own internal memory handlers
#include <xercesc/util/XMemory.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// indicate using Xerces-C++ namespace in general
XERCES_CPP_NAMESPACE_USE

#include "../EmberOgrePrerequisites.h"
#include <string>
#include <map>


namespace Carpenter
{
class AttachPair;
class AttachPoint;
class BlockSpec;
class BuildingBlock;
class BuildingBlockSpec;
class BuildingBlockSpecDefinition;
class BuildingBlockBinding;
class BuildingBlockBindingDefinition;
class BuildingBlockDefinition;
class BluePrint;
class Carpenter;

};

namespace EmberOgre {

class Model;
class ModelBlock;

/**
@author Erik Hjortsberg

Classes for handling Carpenter instances and building creation in Ember. These classes should also allow for real time editing of blueprints, and serializing them into different formats (xml, Atlas etc.).

*/
class Jesus{
public:
    Jesus(Carpenter::Carpenter* carpenter);

    ~Jesus();
	
	/**
	 *    Loads BlockSpecs from a xml file
	 * @param filename 
	 * @return 
	 */
	bool loadBlockSpec(const std::string & filename);
	
	
	/**
	 *    Loads BuildingBlockSpecDefinitions from a xml file.
	 * @param filename 
	 * @return 
	 */
	bool loadBuildingBlockSpecDefinition(const std::string& filename);
	
	
	/**
	 *    Loads ModelBlockMappings from a xml file.
	 * @param filename 
	 * @return 
	 */
	bool loadModelBlockMapping(const std::string& filename);
	
	
	/**
	 *    Loads and creates an instance of a BluePrint from a xml file.
	 *    Note that you have to call compile() on the resulting BluePrint to get it to arrange the BuildingBlocks.
	 * @param filename 
	 * @return 
	 */
	Carpenter::BluePrint* loadBlueprint(std::string filename);
	
	/**
	 *    Creates a new Model instance for the supplied block type.
	 *    If there's no Model defined for the supplies block type, a null pointer will be returned.
	 * @param blockType 
	 * @param modelName 
	 * @return A Model instance or 0
	 */
	Model* createModelForBlockType(const std::string& blockType,  const std::string& modelName);

protected:
	/**
	 *    Utility method to fill the supplied WFMath object (for example a Vector or a Point) with xyz values from the Xerces Node
	 * @param  
	 * @param  
	 */
	template <typename T> void fillFromElement(xercesc::DOMElement* , T& );
	
	std::map<std::string, std::string> mModelMappings;
	Carpenter::Carpenter* mCarpenter;
	
	/**
	* Convenience map of commonly used normals, such as NORTH, UP, WEST etc.
	*/
	std::map<const std::string, WFMath::Vector<3> > mNormalTypes;
	

};

/**

A mapping between a Carpenter::BuildingBlock and an Ember::Model.
*/
class ModelBlock
{
	ModelBlock(Carpenter::BuildingBlock* buildingBlock,  Model* model);
protected:
	Carpenter::BuildingBlock* mBuildingBlock;
	Model* mModel;

};

/**
	Mapping between a Carpenter block type and a Ember Model.
*/
struct ModelMapping
{

public: 
	const std::string model;
	const std::string blocktype;
};

/**

An Ember representation of a BluePrint.

*/
class Construction
{
public:
	Construction(Carpenter::BluePrint* blueprint, Jesus* jesus, Ogre::SceneNode* node);
	
protected:
	Carpenter::BluePrint* mBlueprint;
	Ogre::SceneNode* mBaseNode;
	Jesus* mJesus;
	
	std::vector<ModelBlock> mModelBlocks;

};



};

#endif
