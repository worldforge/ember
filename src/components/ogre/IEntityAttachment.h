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
class IAttachmentControlDelegate;
class EmberEntity;

/**
 * @author Erik Hjortsberg <erik.hjortsberg@gmail.com>
 * @brief Represents an attachment between two entities, and a graphical representation belonging to one entity.
 *
 * The entities themselves are ordered in a hierarchy. However, the graphical representation of them are separate and bound together by instances of this.
 * The normal case would be two entities being both represented through an instance of Ogre::SceneNode, and a Model. However, there are other instances where a SceneNode doesn't make any sense, and instead another kind of attachment is suitable.
 *
 */
class IEntityAttachment {
public:

	/**
	 * @brief Dtor.
	 */
	virtual ~IEntityAttachment(){}

	/**
	 * @brief Gets the graphical representation that this attachment is bound to.
	 * The graphical representation belongs to the attached entity.
	 * @return A pointer to a graphical representation, or null if there are none for this attachment.
	 */
	virtual IGraphicalRepresentation* getGraphicalRepresentation() const = 0;

	/**
	 * @brief Gets the attached entity, or "child" entity.
	 * The attached entity is also the owner of any graphical representation.
	 * @return The child entity.
	 */
	virtual EmberEntity& getAttachedEntity() const = 0;

	/**
	 * @brief Gets the parent entity, to which the child entity is attached.
	 * This entity is not the owner of the graphical representation.
	 * All entities in the hierarchy have a parent entity, except for the topmost "world" entity.
	 * @return The parent entity, or null if there is none.
	 */
	virtual EmberEntity* getParentEntity() const = 0;

	/**
	 * @brief Attach a new child entity to this.
	 * The new entity will be attached to the "child" entity of this attachment.
	 * The kind of attachment produced is determined by the actual subclass. Different kinds of attachments can produce different kind of new child attachments.
	 * An example would be an attachment which is designed to hide all child attachments so that they won't have any graphical representation. Such an attachment would therefore always return an instance of itself, thus perpetuating the tree of non visible attachments.
	 * @param entity The new entity to attach as a child to this attachment.
	 * @return A new attachment instance. This can never be null.
	 */
	virtual IEntityAttachment* attachEntity(EmberEntity& entity) = 0;

	/**
	 * @brief Asks the attachment to update its scaling.
	 * In most cases this is only of interest for those attachments which have graphical representations.
	 */
	virtual void updateScale() = 0;

	/**
	 * @brief Asks the attachment to update its position.
	 * It's up to the attachment to determine how to best do this. In some cases an instance of IAttachmentControlDelegate can be used to get a position different from the one supplied by the entity.
	 */
	virtual void updatePosition() = 0;

	/**
	 * @brief Gets the offset of a contained attachment.
	 * The main reason for us wanting to offset contained attachments is that sometimes we want contained attachments to be adjusted to better fit the current mode. An example would be a table, where we want attachments that are contained by the table entity to appear to lie flat on the table. The server of course has some ideas about this, but depending on the actual model used this might look strange. We therefore adjust the contained attachments to better fit the actual in game representation.
	 * @param attachment The attachment to get an adjusted position for.
	 * @param localPosition The position of the attachment local to this attachment.
	 * @param offset The offset which needs to be applied.
	 */
	virtual void getOffsetForContainedNode(const IEntityAttachment& attachment, const WFMath::Point<3>& localPosition, WFMath::Vector<3>& offset) = 0;

	/**
	 * @brief Sets the control delegate to use for this attachment.
	 * The control delegate is used whenever the attachment needs to update its position. Instead of directly using the position of the attached entity it can then use the position supplied by the delegate, thus allowing for some indirection.
	 */
	virtual void setControlDelegate(IAttachmentControlDelegate* controllerDelegate) = 0;

};

}

#endif /* IENTITYATTACHMENT_H_ */
