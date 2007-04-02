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


#include "../EmberOgrePrerequisites.h"
#include <string>
#include <map>

#include "../EmberOgre.h"
#include <wfmath/vector.h>

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

TYPEDEF_STL_MAP(const std::string, Ogre::ColourValue, AttachPointColourValueMap);

namespace Model {
	class Model;
}

class ModelBlock;
class Construction;

/**
@author Erik Hjortsberg

Classes for handling Carpenter instances and building creation in Ember. These classes should also allow for real time editing of blueprints, and serializing them into different formats (xml, Atlas etc.).

*/
class Jesus{
	friend class XMLJesusSerializer;

	public:

	enum ClickMasks
	{
		CM_MODELBLOCK = 1<<13,
		CM_ATTACHPOINT = 1<<14
	};

    Jesus(Carpenter::Carpenter* carpenter);

    ~Jesus();
	

		
	/**
	 *    Creates a new Model instance for the supplied block type.
	 *    If there's no Model defined for the supplies block type, a null pointer will be returned.
	 * @param blockType 
	 * @param modelName 
	 * @return A Model instance or 0
	 */
	Model::Model* createModelForBlockType(const std::string& blockType,  const std::string& modelName);
	
	
	/**
	 *    Finds the colour associated to the supplied attachpoint. If no colour is registered, a ColourValue of 1,1,1 will be returned
	 * @param point 
	 * @return 
	 */
	Ogre::ColourValue getColourForAttachPoint(const Carpenter::AttachPoint* point) const;
	
	/**
	 *    Accessor for the Carpenter object.
	 * @return 
	 */
	inline Carpenter::Carpenter* getCarpenter() const { return mCarpenter; }
	
	/**
	 *    adds a blueprint
	 * @param blueprint 
	 * @return false if there's already a blueprint with the same name as the supplied blueprint
	 */
	bool addBluePrint(Carpenter::BluePrint* blueprint);
	
	/**
	 *    returns a blueprint with the supplied name
	 *	  note that the blueprint must already be loaded and added to Jesus with addBluePrint(...)
	 *	  @see addBluePrint(Carpenter::BluePrint*)
	 * @param name 
	 * @return 
	 */
	Carpenter::BluePrint* getBluePrint(const std::string& name) const;
	
	
	inline const std::map<std::string , Carpenter::BluePrint* > * getAllBluePrints() const {return &mBlueprints;}

protected:
	
	/**
	a map of what Model should represent a certain Carpenter::BuildingBlockSpec
	the first value is the name of the Carpenter::BuildingBlockSpec and the second value is the name of the Model
	*/
	std::map<std::string, std::string> mModelMappings;
	Carpenter::Carpenter* mCarpenter;
	
	/**
	* Convenience map of commonly used normals, such as NORTH, UP, WEST etc.
	*/
	std::map<const std::string, WFMath::Vector<3> > mNormalTypes;
	
	/**
	Map of colours for Attach point types.
	*/
	AttachPointColourValueMap mColourMap;
	
	void addAttachPointType(const std::string & type);
	
	/**
	all blueprints are stored here
	*/
	std::map<std::string , Carpenter::BluePrint* > mBlueprints;

};


/** This class 'wibbles' the billboard */
class LightWibbler : public Ogre::ControllerValue<Ogre::Real>
{
public:
	LightWibbler(const Ogre::ColourValue& originalColour, Ogre::Billboard* billboard);

	virtual Ogre::Real  getValue (void) const;
	virtual void  setValue (Ogre::Real value);
protected:
	Ogre::Billboard* mBillboard;
	Ogre::ColourValue mOriginalColour;
	Ogre::Real intensity;
};


class AttachPointNode
{
public:
	AttachPointNode(ModelBlock* modelBlock, Ogre::SceneNode* modelNode, const Carpenter::AttachPoint* attachPoint, Ogre::ColourValue colour, Ogre::BillboardSet* billboardSet);
	~AttachPointNode();
	void select();
	void deselect();
	inline const Carpenter::AttachPoint* getAttachPoint() const { return mAttachPoint;}
	
protected:
	const ModelBlock* mModelBlock;
	Ogre::SceneNode* mModelNode;
	const Carpenter::AttachPoint* mAttachPoint;
	Ogre::Billboard* mFlare;
	Ogre::ColourValue mColour;
	Ogre::Controller<Ogre::Real>* mController;
	Ogre::SceneNode* mPointerNode;
	Ogre::Entity* mPointerEntity;
};


/**

A mapping between a Carpenter::BuildingBlock and an Ember::Model.
*/
class ModelBlock
{
public:
	ModelBlock(Ogre::SceneNode* baseNode, const Carpenter::BuildingBlock* buildingBlock,  Model::Model* model, Construction* construction);
	~ModelBlock();
	//void selectAttachPointNode(AttachPointNode* selectedNode);
	
	inline const Carpenter::BuildingBlock* getBuildingBlock() const { return mBuildingBlock; }
	inline Construction* getConstruction() const { return mConstruction; }
	void createAttachPointNodes();
	void select();
	void deselect();
	
	std::vector<AttachPointNode*> getAttachPointNodes() const;
	
	const Model::Model* getModel() const { return mModel;}
	const Ogre::SceneNode* getNode() const { return mNode;}
protected:
	const Carpenter::BuildingBlock* mBuildingBlock;
	Model::Model* mModel;
	
	std::vector<AttachPointNode*> mAttachPointNodes;
	Ogre::SceneNode *mNode, *mModelNode;
	Ogre::BillboardSet* mPointBillBoardSet;
	Construction* mConstruction;
	//Jesus* mJesus;
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
	~Construction();
	//inline Ogre::BillboardSet* getBillBoardSet() const { return mPointBillBoardSet; }
	inline Jesus* getJesus() const { return mJesus; }
	inline Carpenter::BluePrint* getBluePrint() const { return mBlueprint; }


	void buildFromBluePrint(bool createAttachPointNodes);

	
	/**
	 *    Creates a new ModelBlock and adds it to the construction.
	 * @param buildingBlock 
	 * @return 
	 */
	ModelBlock* createModelBlock(const Carpenter::BuildingBlock* buildingBlock, bool createAttachPointNodes);
	
	std::vector<ModelBlock*> getModelBlocks() const;
	
	bool remove(ModelBlock* modelBlock);
	
protected:
 	Carpenter::BluePrint* mBlueprint;
	Ogre::SceneNode* mBaseNode;
	Jesus* mJesus;
	
	std::vector<ModelBlock*> mModelBlocks;
	
};



};

#endif
