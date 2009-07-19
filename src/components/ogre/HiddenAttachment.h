/*
 Copyright (C) 2009 Erik Hjortsberg

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

#ifndef HIDDENATTACHMENT_H_
#define HIDDENATTACHMENT_H_

#include "components/ogre/AttachmentBase.h"

namespace EmberOgre
{

class IGraphicalRepresentation;
class EmberEntity;


/**
 * @brief Hides all graphical representation.
 *
 * Use this attachment when you want to hide all graphical representations.
 * All child attachments produced by this will be of HiddenAttachment type, thus guaranteeing that no child is shown.
 */
class HiddenAttachment : public AttachmentBase
{
public:
	HiddenAttachment(EmberEntity& parentEntity, EmberEntity& childEntity);

	virtual ~HiddenAttachment();

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

private:

};

}
#endif /* HIDDENATTACHMENT_H_ */
