/*
 Copyright (C) 2018 Erik Ogenvik

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

#ifndef EMBER_PRESENTMODELACTION_H
#define EMBER_PRESENTMODELACTION_H

#include "ModelActionBase.h"

namespace Ember {

namespace OgreView {

/**
 * A entity mapping action which will look for the value of the "present" attribute and use the corresponding model.
 */
class PresentAction : public ModelActionBase {
public:
	explicit PresentAction(EmberEntity& entity,
						   Scene& scene,
						   EntityMapping::EntityMapping& mapping,
						   AttachmentFunction attachmentFunction);

	void activate(EntityMapping::ChangeContext& context) override;

};

}
}

#endif //EMBER_PRESENTMODELACTION_H
