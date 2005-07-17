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
	BuildingBlockBinding(const BuildingBlock* block1, const AttachPoint* point1, const BuildingBlock* block2,	const AttachPoint* point2);
	const std::string& getType() const;
	const BuildingBlock* getBlock1() const { return mBlock1; }
	const BuildingBlock* getBlock2() const { return mBlock2; }
	const AttachPoint* getAttachPoint1() const { return mPoint1; }
	const AttachPoint* getAttachPoint2() const { return mPoint2; }


protected:
	const AttachPoint* mPoint1;
	const BuildingBlock* mBlock2;
	const AttachPoint* mPoint2;
	const BuildingBlock* mBlock1;
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
	
	

	inline void setPosition(WFMath::Point<3> position) { mPosition = position;}
	inline void setOrientation(WFMath::Quaternion orientation) { mOrientation = orientation;}
	inline const WFMath::Point<3>& getPosition() const { return mPosition;}
	inline const WFMath::Quaternion& getOrientation() const { return mOrientation;}
	
	inline const BuildingBlockSpec* getBuildingBlockSpec() const { return mBuildingBlockSpec; }
	
	inline const BlockSpec* getBlockSpec() const { return mBuildingBlockSpec->getBlockSpec(); }
	
	const std::vector<const AttachPoint*> getAllPoints() const;
	
	/**
	 *    the number of bindings that are dependant on this block
			as long as it's more than zero, the block cannot be deleted
	 * @return 
	 */
	inline int getNumberOfChildBindings() const { return mChildBindings; }
	
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
	
	/**
	 *    removes a point from the list of bound points
	 * @param point 
	 */
	void removeBoundPoint(const AttachPoint* point);
	
	/**
	the number of bindings that are dependant on this block
	as long as it's more than zero, the block cannot be deleted
	*/
	int mChildBindings;

};







class BluePrint
{
public:
	BluePrint(const std::string & name, Carpenter* carpenter);
	
	inline const std::string& getName() const {return mName;}
	
	/**
	 *    compiles the blueprint into a structure
	 */
	void compile();
	
	BuildingBlock* createBuildingBlock(BuildingBlockDefinition);
	
	/**
	 *    deletes a building block from the blueprint
	 * @param name 
	 */
//    	void deleteBuildingBlock(const std::string & name);

	BuildingBlockBinding* addBinding(BuildingBlockBindingDefinition definition);
	BuildingBlockBinding* addBinding(const BuildingBlock* block1, const AttachPoint* point1, const BuildingBlock* block2,	const AttachPoint* point2);
	const std::vector< BuildingBlock*> getAttachedBlocks() const;
	const std::list< BuildingBlockBinding>* getBindings() const;

	/**
	 *    accessor for the name of the starting block
	 * @param name 
	 */
	void setStartingBlock(const std::string& name);
	inline const BuildingBlock* getStartingBlock() const {return mStartingBlock;}
	
	/**
	 *    Places the unbound block in the supplied bindings correctly
	 * @param binding 
	 */
	void placeBindings(BuildingBlock* unboundBlock, std::vector<BuildingBlockBinding*> bindings);
	
	inline Carpenter* const getCarpenter() { return mCarpenter;}
	
	
	/**
	 *    true if the building block can be removed from the blueprint
	 * @param  
	 * @return 
	 */
	bool isRemovable(const BuildingBlock* bblock) const;
	
	/**
	 *    removes the building block from the blueprint, if it can be removed
	 * @param bblock 
	 * @return 
	 */
	bool remove(const BuildingBlock* bblock);

	typedef std::map<const std::string,  BuildingBlock> BuildingBlockStore;
protected:
	BuildingBlockStore mBuildingBlocks;
	std::list< BuildingBlockBinding> mBindings;
	
	std::vector< BuildingBlock*> mAttachedBlocks;
	BuildingBlock* mStartingBlock;
	std::string mName;
	
	Carpenter* mCarpenter;
	
	
	void doBindingsForBlock(BuildingBlock *block);

};

};

#endif
