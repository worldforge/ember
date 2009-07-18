/*
    Copyright (C) 2009 Erik Hjortsberg <erik.hjortsberg@gmail.com>

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

#ifndef IENTITYATTACHMENT_H_
#define IENTITYATTACHMENT_H_

namespace WFMath
{
template<int> class Point;
template<int> class Vector;
}

namespace EmberOgre {

class IGraphicalRepresentation;
class EmberEntity;

class IEntityAttachment {
public:

	virtual IGraphicalRepresentation* getGraphicalRepresentation() const = 0;

	virtual EmberEntity& getAttachedEntity() const = 0;

	virtual EmberEntity* getParentEntity() const = 0;

	virtual IEntityAttachment* attachEntity(EmberEntity& entity) = 0;

	virtual void updateScale() = 0;

	/**
	 * @brief Gets the offset of a contained attachment.
	 * The main reason for us wanting to offset contained attachments is that sometimes we want contained attachments to be adjusted to better fit the current mode. An example would be a table, where we want attachments that are contained by the table entity to appear to lie flat on the table. The server of course has some ideas about this, but depending on the actual model used this might look strange. We therefore adjust the contained attachments to better fit the actual in game representation.
	 * @param attachment The attachment to get an adjusted position for.
	 * @param localPosition The position of the attachment local to this attachment.
	 * @param offset The offset which needs to be applied.
	 */
	virtual void getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset) = 0;

};

}

#endif /* IENTITYATTACHMENT_H_ */
