//
// C++ Interface: Carpenter
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
#ifndef EMBEROGRECARPENTER_H
#define EMBEROGRECARPENTER_H

//we must include xerces stuff before ogre stuff, because else we'll get errors when compiling in debug mode
//this seems to be because both uses their own internal memory handlers
#include <xercesc/util/XMemory.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/util/XMLString.hpp>
#include <xercesc/util/PlatformUtils.hpp>
// indicate using Xerces-C++ namespace in general
XERCES_CPP_NAMESPACE_USE


#include "../EmberOgrePrerequisites.h"
#include "../model/Model.h"

 namespace Carpenter {

/**
@author Erik Hjortsberg
*/

class AttachPair;
class BlockSpec;
class BuildingBlock;
class ModelBlock;
class ModelBlockDefinition;
class BuildingBlockBinding;
class BuildingBlockBindingDefinition;
// class ModelBlockDefinition
// class ModelBlock
// class BuildingBlock
class BuildingBlockDefinition;
class BluePrint;


class AttachPoint
{
friend class AttachPair;
friend class BlockSpec;
public:
	WFMath::Vector<3> mNormal;
	WFMath::Point<3> mPosition;
	std::string mName;
	AttachPair* mAttachPair;
	const AttachPoint* getSibling() const;
};

class AttachPair
{
friend class BlockSpec;
public:
	AttachPoint point1;	
	AttachPoint point2;	
//	const AttachPoint& [] getPoints() const;
	std::string mName;
	std::string mType;
	
	const AttachPoint* getAttachPoint(const std::string & name) const { 
		if (point1.mName == name) {
			return &point1;
		} else if (point2.mName == name) {
			return &point2;
		} else {
			//"No pair with that name."
			throw std::exception();
		}
		
	}
};

class BlockSpec
{
public:
	std::string mName;
	WFMath::AxisBox<3> mBoundingBox;
	std::map<const std::string, AttachPair> mAttachPairs;
	
	const AttachPair* getAttachPairForPoint(AttachPoint* point);
	const AttachPair* getAttachPair(const std::string & name)  { return &(mAttachPairs[name]); }
};

class BuildingBlockBindingDefinition
{
public:
	std::string mBlock1Name;
	std::string mBlock2Name;
	std::string mPoint1Name;
	std::string mPoint2Name;
};

class BuildingBlockBinding
{
public:
	BuildingBlockBindingDefinition mDefinition;
	const std::string& getType() const;
	BuildingBlock* mBlock1;
	const AttachPoint* mPoint1;
	BuildingBlock* mBlock2;
	const AttachPoint* mPoint2;
	//std::string mType;
};

class ModelBlock
{
public:
	ModelBlockDefinition* mDefinition;
	BlockSpec* mBlockSpec;
	EmberOgre::Model* mModel;
};

class ModelBlockDefinition
{
public:
	std::string mName;
	std::string mBlockSpecName;
	std::string mModelName;
	//ModelBlock getModelBlock();
};



class BuildingBlockDefinition
{
public:
	std::string mName;
	std::string mModelBlock;
};

class BuildingBlock
{
public:
	BuildingBlock();
	const std::string& getName() const { return mBlockDefinition.mName; }
	ModelBlock mModelBlock;
	EmberOgre::Model* getModel() const { return mModelBlock.mModel;}
	Ogre::SceneNode* mNode;
	const std::vector<BuildingBlockBinding*>& getBindingsForBlock() const;
	BuildingBlockDefinition mBlockDefinition;
	const AttachPair* getAttachPair(const std::string& name);
	
	WFMath::Point<3> mPosition;
	WFMath::Quaternion mRotation;
	
	WFMath::Point<3> getWorldPositionForPoint(const AttachPoint* point);
	bool mAttached;
	bool isAttached() const { return mAttached; }
	
	

	/**
		A vector of all points that are already bound
	*/
	std::vector<const AttachPoint*> mBoundPoints;
};



class BluePrint
{
public:
	//std::map<std::string, BuildingBlockDefinition> mBuildingBlockDefinitions;
	std::map<const std::string,  BuildingBlock> mBuildingBlocks;
	std::vector< BuildingBlockBinding> mBindings;
	
	void compile(Ogre::SceneNode* baseNode);
	
	/**
	 *    Places the unbound block in the supplied bindings correctly
	 * @param binding 
	 */
	void placeBindings(BuildingBlock* unboundBlock, std::vector<BuildingBlockBinding*>& bindings);
	
	void doBindingsForBlock(BuildingBlock *block);

	
	BuildingBlock* mStartingBlock;

};




class Carpenter{
public:
    Carpenter();

    ~Carpenter();
	bool loadBlockSpec(const std::string& filename);
	bool loadModelBlockDefinition(const std::string& filename);
	
	BluePrint* loadBlueprint(std::string filename);
protected:
	std::map<const std::string , BlockSpec > mBlockSpecs;
	std::map<const std::string , ModelBlockDefinition > mModelBlockDefinitions;
	
	std::map<const std::string, WFMath::Vector<3> > mNormalTypes;
	template <typename T>
	void fillFromElement(xercesc::DOMElement* , T& );


};

}

#endif
