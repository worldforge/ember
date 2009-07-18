/*
 Copyright (C) 2009 erik

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

#ifndef EMBEROGREOGREATTACHMENT_H_
#define EMBEROGREOGREATTACHMENT_H_

#include "components/ogre/IEntityAttachment.h"
#include "components/ogre/IMovable.h"

#include <sigc++/trackable.h>

namespace Ogre {
class SceneNode;
}

namespace EmberOgre {

class IGraphicalRepresentation;
class EmberEntity;

class OgreAttachment : public IEntityAttachment, public virtual sigc::trackable, public IMovable
{
public:
	OgreAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, Ogre::SceneNode& parentNode);
	virtual ~OgreAttachment();

	virtual IGraphicalRepresentation* getGraphicalRepresentation() const;

	virtual EmberEntity& getAttachedEntity() const;

	virtual EmberEntity* getParentEntity() const;

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

	virtual void updateScale();

	void updatePosition();

	virtual OgreAttachment* transferToNewParent(OgreAttachment& newParentAttachment);

	virtual void updateMotion(float timeSlice);

protected:


	EmberEntity& mParentEntity;
	EmberEntity& mChildEntity;

	Ogre::SceneNode* mSceneNode;

	OgreAttachment(const OgreAttachment& source);

	void entity_Moved();
	virtual void setupConnections();

};

}
#endif /* EMBEROGREOGREATTACHMENT_H_ */
