//
// C++ Implementation: ModelMapping
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik@katastrof.nu>, (C) 2007
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
#include "ModelMapping.h"
#include <Eris/View.h>
#include "Cases/CaseBase.h"

namespace EmberOgre {

namespace Model {

namespace Mapping {
	

ModelMapping::ModelMapping(Eris::Entity* entity): mEntity(entity) 
{
}

ModelMapping::~ModelMapping()
{
}

Matches::EntityTypeMatch& ModelMapping::getRootEntityMatch() 
{ 
	return mRootEntityMatch;
}

void ModelMapping::evaluateChanges() {

	mRootEntityMatch.evaluateChanges();
}
void ModelMapping::addCase(Cases::CaseBase* aCase)
{
	mCases.push_back(aCase);
}


void ModelMapping::initialize()
{
	///make sure that we evaluate the changes of the root match first
	
// 	std::vector<CaseBase*> activateQueue;
// 	std::vector<CaseBase*> deactivateQueue;
// 	
// 	std::vector<EntityTypeCase*>::iterator endI = mRootEntityMatch.getCases().end();
// 	for (std::vector<EntityTypeCase*>::iterator I = mRootEntityMatch.getCases().begin(); I != endI; ++I) {
// 		(*I)->evaluateChanges(activateQueue, deactivateQueue);
// 	}	
// 	
// 	for (std::vector<CaseBase*>::iterator I = activateQueue.begin(); I != activateQueue.end(); ++I) {
// 		(*I)->activateActions();
// 	} 
	
	evaluateChanges();
}


}

}

}
