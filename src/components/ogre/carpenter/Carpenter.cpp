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
#include "BluePrint.h"

#include <wfmath/error.h>

namespace Carpenter {


Carpenter::Carpenter()
{
}


Carpenter::~Carpenter()
{
}

BuildingBlockSpec::BuildingBlockSpec()
: mBlockSpec(0)
{
}

BuildingBlockSpecDefinition::BuildingBlockSpecDefinition()
{
}
	
AttachPoint::AttachPoint(const std::string& name, WFMath::Point<3> position, WFMath::Vector<3> normal)
: mNormal(normal), mPosition(position), mName(name), mAttachPair(0)
{
}

AttachPair::AttachPair(const std::string& name, const std::string& type, AttachPoint point1, AttachPoint point2)
: mPoint1(point1), mPoint2(point2), mName(name), mType(type)
{
	mPoint1.setAttachPair(this);
	mPoint2.setAttachPair(this);
}

const std::vector<const AttachPoint*> BlockSpec::getAllPoints() const
{
	std::vector<const AttachPoint*> points;
	
	std::map<const std::string, AttachPair>::const_iterator I = mAttachPairs.begin();
	std::map<const std::string, AttachPair>::const_iterator I_end = mAttachPairs.end();
	
	for (;I != I_end; ++I) {
		points.push_back(&I->second.getPoint1());
		points.push_back(&I->second.getPoint2());
	}
	return points;
	
}

bool BlockSpec::addAttachPair(AttachPair* pair)
{
	if (mAttachPairs.find(pair->getName()) != mAttachPairs.end()) 
		return false;
	mAttachPairs.insert(std::map<const std::string, AttachPair>::value_type(pair->getName(), *pair));
	return true;
}

const AttachPair* BlockSpec::getAttachPair(const std::string & name) const
{
	AttachPairStore::const_iterator I = mAttachPairs.find(name);
	if (I == mAttachPairs.end()) {
		return 0;
	}
	return &(I->second); 
}

void BlockSpec::setBoundingBox(WFMath::AxisBox<3> bbox)
{
	mBoundingBox = bbox;
}


const AttachPoint* AttachPoint::getSibling() const
{
	if (&mAttachPair->mPoint1 == this) {
		return &mAttachPair->mPoint2;
	}
	return &mAttachPair->mPoint1;
}



BlockSpec* Carpenter::createBlockSpec(std::string name)
{
	mBlockSpecs[name];
	mBlockSpecs[name].mName = name;
	return &mBlockSpecs[name];
}

BuildingBlockSpec* Carpenter::createBuildingBlockSpec(BuildingBlockSpecDefinition definition)
{
/*	BuildingBlockSpec* spec = new BuildingBlockSpec();
	spec->mDefinition = definition;
	spec->mBlockSpec = &mBlockSpecs[definition.mBlockSpecName];
	mBuildingBlockSpecs.insert(std::map<const std::string , BuildingBlockSpec >::value_type(definition.mName, *spec));*/
 	
	mBuildingBlockSpecs[definition.mName];
	
 	mBuildingBlockSpecs[definition.mName].mDefinition = definition;
 	mBuildingBlockSpecs[definition.mName].mBlockSpec = &mBlockSpecs[definition.mBlockSpecName];

	return &mBuildingBlockSpecs[definition.mName];
}

BluePrint* Carpenter::createBlueprint(std::string name)
{
	mBluePrints.insert(std::map<const std::string , BluePrint>::value_type(name, BluePrint(name, this)));
	return &(mBluePrints.find(name)->second);
}

BuildingBlockSpec* Carpenter::getBuildingBlockSpec(const std::string& name)
{
	return &mBuildingBlockSpecs[name];
}



};
