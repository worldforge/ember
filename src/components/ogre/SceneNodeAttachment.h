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

#ifndef EMBEROGRESCENENODEATTACHMENT_H_
#define EMBEROGRESCENENODEATTACHMENT_H_

#include "components/ogre/AttachmentBase.h"
#include <sigc++/trackable.h>


namespace Ogre {
class SceneNode;
}

namespace EmberOgre {


class SceneNodeController;

class SceneNodeAttachment : public AttachmentBase, public virtual sigc::trackable
{
public:
	SceneNodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, Ogre::SceneNode& parentNode);
	SceneNodeAttachment(SceneNodeAttachment& source, SceneNodeAttachment& newParentAttachment);
	virtual ~SceneNodeAttachment();

	virtual void setControlDelegate(IAttachmentControlDelegate* controllerDelegate);

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

	void setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation);

	Ogre::SceneNode* getSceneNode() const;

	virtual void updatePosition();

protected:

	Ogre::SceneNode* mSceneNode;
	Ogre::SceneNode& mParentNode;

	SceneNodeController* mAttachmentController;

	void setupListeners();

	void entity_VisibilityChanged(bool visible);


};

}
#endif /* EMBEROGRESCENENODEATTACHMENT_H_ */
