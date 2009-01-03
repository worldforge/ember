//
// C++ Implementation: ModelPart
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2008
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

#include "ModelPart.h"

#include "SubModelPart.h"

namespace EmberOgre {

namespace Model {

ModelPart::ModelPart() : mVisible(false)
{
}


void ModelPart::show()
{
	mVisible = true;
	for (SubModelPartStore::iterator I = mSubModelParts.begin(); I != mSubModelParts.end(); ++I) {
		(*I)->show();
	}
}

void ModelPart::hide()
{
	for (SubModelPartStore::iterator I = mSubModelParts.begin(); I != mSubModelParts.end(); ++I) {
		(*I)->hide();
	}
}

bool ModelPart::getVisible() const
{
	return mVisible;
}

void ModelPart::setVisible(bool visible)
{
	mVisible = visible;
}


const std::string& ModelPart::getGroupName() const
{
	return mGroupName;
}

void ModelPart::setGroupName(const std::string& groupName)
{
	mGroupName = groupName;
}

void ModelPart::addSubModelPart(SubModelPart* part)
{
	mSubModelParts.push_back(part);
}


}

}
