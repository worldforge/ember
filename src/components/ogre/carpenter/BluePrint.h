//
// C++ Interface: BluePrint
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
#ifndef CARPENTERBLUEPRINT_H
#define CARPENTERBLUEPRINT_H
// ------------------------------
// Include WFmath header files
// ------------------------------
#include <wfmath/point.h>
#include <wfmath/vector.h>
#include <wfmath/axisbox.h>
#include <wfmath/quaternion.h>

#include "Carpenter.h"

namespace Carpenter {

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

/**
@author Erik Hjortsberg
*/



class BuildingBlockBindingDefinition
{
public:
	BuildingBlockBindingDefinition() {}
	 std::string mBlock1Name;
	 std::string mBlock2Name;
	 std::string mPair1Name;
	 std::string mPair2Name;
	 std::string mPoint1Name;
	 std::string mPoint2Name;
};

class BuildingBlockBinding
{
friend class BluePrint;
public:
	const std::string& getType() const;
	const BuildingBlockBindingDefinition& getDefinition() { return mDefinition; }
	const BuildingBlock* getBlock1() const { return mBlock1; }
	const BuildingBlock* getBlock2() const { return mBlock2; }
	const AttachPoint* getAttachPoint1() const { return mPoint1; }
	const AttachPoint* getAttachPoint2() const { return mPoint2; }


protected:
	BuildingBlockBindingDefinition mDefinition;
	BuildingBlock* mBlock1;
	const AttachPoint* mPoint1;
	BuildingBlock* mBlock2;
	const AttachPoint* mPoint2;
	//std::string mType;
};









class BuildingBlockDefinition
{
public:
	BuildingBlockDefinition() {}
	 std::string mName;
	 std::string mBuildingBlockSpec;
};

class BuildingBlock
{
friend class BluePrint;
public:
	BuildingBlock();
	const std::vector<BuildingBlockBinding*> getBindingsForBlock() const;
	const AttachPair* getAttachPair(const std::string& name);
	const std::string& getName() const { return mBlockDefinition.mName; }
	
	
	WFMath::Point<3> getWorldPositionForPoint(const AttachPoint* point);
	inline bool isAttached() const { return mAttached; }
	void setStartingBlock(const std::string& name);

	inline void setPosition(WFMath::Point<3> position) { mPosition = position;}
	inline void setOrientation(WFMath::Quaternion orientation) { mOrientation = orientation;}
	inline const WFMath::Point<3>& getPosition() const { return mPosition;}
	inline const WFMath::Quaternion& getOrientation() const { return mOrientation;}
	
	inline const BuildingBlockSpec* getBuildingBlockSpec() const { return mBuildingBlockSpec; }
	
	inline const BlockSpec* getBlockSpec() const { return mBuildingBlockSpec->getBlockSpec(); }
	
	const std::vector<const AttachPoint*> getAllPoints() const;
	
protected: 	
	//ModelBlock mModelBlock;
	BuildingBlockSpec* mBuildingBlockSpec;
	BuildingBlockDefinition mBlockDefinition;
	WFMath::Point<3> mPosition;
	WFMath::Quaternion mOrientation;
	bool mAttached;
	/**
		A vector of all points that are already bound
	*/
	std::vector<const AttachPoint*> mBoundPoints;

};







class BluePrint
{
public:
	BluePrint(const std::string & name, Carpenter* carpenter);
	
	
	/**
	 *    compiles the blueprint into a structure
	 */
	void compile();
	
	BuildingBlock* createBuildingBlock(BuildingBlockDefinition);
	void addBinding(BuildingBlockBindingDefinition definition);
	
	const std::vector< BuildingBlock*> getAttachedBlocks() const;

	void setStartingBlock(const std::string& name);

protected:
	std::map<const std::string,  BuildingBlock> mBuildingBlocks;
	std::vector< BuildingBlockBinding> mBindings;
	
	std::vector< BuildingBlock*> mAttachedBlocks;
	BuildingBlock* mStartingBlock;
	std::string mName;
	
	Carpenter* mCarpenter;
	
	/**
	 *    Places the unbound block in the supplied bindings correctly
	 * @param binding 
	 */
	void placeBindings(BuildingBlock* unboundBlock, std::vector<BuildingBlockBinding*>& bindings);
	
	void doBindingsForBlock(BuildingBlock *block);

};

};

#endif
