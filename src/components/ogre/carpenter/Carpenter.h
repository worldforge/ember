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


// ------------------------------
// Include WFmath header files
// ------------------------------
#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/axisbox.h>
#include <wfmath/quaternion.h>

#include "../EmberOgrePrerequisites.h"
//#include "../model/Model.h"

 namespace Carpenter {

/**
@author Erik Hjortsberg
*/

class AttachPair;
class BlockSpec;
class BuildingBlock;
class BuildingBlockSpec;
class BuildingBlockSpecDefinition;
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
friend class Carpenter;
public:
	AttachPoint(const std::string& mName, WFMath::Point<3> position, WFMath::Vector<3> normal);
	inline const std::string& getName() const { return mName; }
	inline const WFMath::Vector<3>& getNormal() const { return mNormal; }
	inline const WFMath::Point<3>& getPosition() const { return mPosition; }

	inline const AttachPair* getAttachPair() const { return mAttachPair; }
	const AttachPoint* getSibling() const;
protected:
	WFMath::Vector<3> mNormal;
	WFMath::Point<3> mPosition;
	std::string mName;
	AttachPair* mAttachPair;
	void setAttachPair(AttachPair* pair) { mAttachPair = pair; }
};

class AttachPair
{
friend class BlockSpec;
friend class AttachPoint;
friend class Carpenter;
public:
	const AttachPoint* getAttachPoint(const std::string & name) const { 
		if (mPoint1.mName == name) {
			return &mPoint1;
		} else if (mPoint2.mName == name) {
			return &mPoint2;
		} else {
			//"No pair with that name."
			throw std::exception();
		}
		
	}
	
	AttachPair(const std::string& name, const std::string& type, AttachPoint point1, AttachPoint point2);
	
	inline const AttachPoint& getPoint1() const { return mPoint1; }
	inline const AttachPoint& getPoint2() const { return mPoint2; }
	inline const std::string& getName() const { return mName; }
	inline const std::string& getType() const { return mType; }

protected:
	AttachPoint mPoint1;	
	AttachPoint mPoint2;	
//	const AttachPoint& [] getPoints() const;
	const std::string mName;
	const std::string mType;
	
};

class BlockSpec
{
friend class Carpenter;
public:
	inline const std::string& getName() const { return mName; }
	inline const WFMath::AxisBox<3>& getBoundingBox() const { return mBoundingBox; }
	const AttachPair* getAttachPairForPoint(AttachPoint* point) const;
	const AttachPair* getAttachPair(const std::string & name) const  { return &(mAttachPairs.find(name)->second); }
	
	bool addAttachPair(AttachPair* pair);
	void setBoundingBox(WFMath::AxisBox<3> bbox);
	
	const std::vector<const AttachPoint*> getAllPoints() const;
	
protected:
	std::string mName;
	WFMath::AxisBox<3> mBoundingBox;
	std::map<const std::string, AttachPair> mAttachPairs;
	
};

class BuildingBlockSpecDefinition
{
public:
	BuildingBlockSpecDefinition();
	std::string mName;
	std::string mBlockSpecName;
	//std::string mModelName;
	//ModelBlock getModelBlock();
};


class BuildingBlockSpec
{
friend class Carpenter;
public:
	BuildingBlockSpec();
	const BuildingBlockSpecDefinition& getDefinition() const { return mDefinition; }
	const BlockSpec*  getBlockSpec() const { return mBlockSpec; }
	inline const std::string& getName() const { return mDefinition.mName; }
protected:
	BuildingBlockSpecDefinition mDefinition;
	BlockSpec* mBlockSpec;
	//EmberOgre::Model* mModel;
};










class Carpenter{
public:
    Carpenter();

    ~Carpenter();
/*	bool loadBlockSpec(const std::string& filename);
	bool loadModelBlockDefinition(const std::string& filename);*/
	
	BluePrint* createBlueprint(std::string name);
	
	BuildingBlockSpec* getBuildingBlockSpec(const std::string& name);
	
	BlockSpec* createBlockSpec(std::string name);
	BuildingBlockSpec* createBuildingBlockSpec(BuildingBlockSpecDefinition definition);
	
	const std::map<const std::string , BlockSpec >* getBlockSpecs() const {return &mBlockSpecs;}
	const std::map<const std::string , BuildingBlockSpec >* getBuildingBlockSpecs() const {return &mBuildingBlockSpecs;}
	const std::map<const std::string , BluePrint>* getBluePrints() const {return &mBluePrints;}

protected:
	std::map<const std::string , BlockSpec > mBlockSpecs;
	std::map<const std::string , BuildingBlockSpec > mBuildingBlockSpecs;
	std::map<const std::string , BluePrint> mBluePrints;

	


};

}

#endif
