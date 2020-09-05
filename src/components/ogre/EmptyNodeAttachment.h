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

#ifndef EMBER_EMPTYNODEATTACHMENT_H
#define EMBER_EMPTYNODEATTACHMENT_H

#include "NodeAttachment.h"
#include <sigc++/trackable.h>

namespace Ogre {
class Node;
}

namespace Ember {
namespace OgreView {

struct INodeProvider;

class NodeController;

/**
 * Used for attaching things where there's no Model.
 */
class EmptyNodeAttachment : public NodeAttachment, public virtual sigc::trackable {
public:
	EmptyNodeAttachment(EmberEntity& parentEntity,
			EmberEntity& childEntity,
			std::unique_ptr<INodeProvider> nodeProvider,
			std::unique_ptr<IGraphicalRepresentation> graphicalRepresentation);

	void attachEntity(EmberEntity& entity) override;

	void detachEntity(EmberEntity& entity);

	IGraphicalRepresentation* getGraphicalRepresentation() const override;

protected:
	std::unique_ptr<INodeProvider> mNodeProvider;
	std::unique_ptr<IGraphicalRepresentation> mGraphicalRepresentation;
};

}
}

#endif //EMBER_EMPTYNODEATTACHMENT_H
