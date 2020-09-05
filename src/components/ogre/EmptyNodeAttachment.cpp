/*
 Copyright (C) 2020 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "EmptyNodeAttachment.h"
#include "INodeProvider.h"
#include "domain/IGraphicalRepresentation.h"

namespace Ember {
namespace OgreView {
EmptyNodeAttachment::EmptyNodeAttachment(Ember::EmberEntity& parentEntity,
										 Ember::EmberEntity& childEntity,
										 std::unique_ptr<INodeProvider> nodeProvider,
										 std::unique_ptr<IGraphicalRepresentation> graphicalRepresentation)
		: NodeAttachment(parentEntity, childEntity, *nodeProvider.get()),
		  mNodeProvider(std::move(nodeProvider)),
		  mGraphicalRepresentation(std::move(graphicalRepresentation)) {

}

void EmptyNodeAttachment::attachEntity(EmberEntity& entity) {
	//No op
}

void EmptyNodeAttachment::detachEntity(EmberEntity& entity) {
	//No op
}

IGraphicalRepresentation* EmptyNodeAttachment::getGraphicalRepresentation() const {
	if (mGraphicalRepresentation) {
		return mGraphicalRepresentation.get();
	}
	return nullptr;
}


}
}
