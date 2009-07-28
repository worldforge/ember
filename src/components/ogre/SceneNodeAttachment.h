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
class Node;
}

namespace EmberOgre {

class INodeProvider;
class SceneNodeController;

class SceneNodeAttachment : public AttachmentBase, public virtual sigc::trackable
{
public:
	SceneNodeAttachment(EmberEntity& parentEntity, EmberEntity& childEntity, INodeProvider* nodeProvider);
	SceneNodeAttachment(SceneNodeAttachment& source, SceneNodeAttachment& newParentAttachment);
	virtual ~SceneNodeAttachment();

	virtual void setControlDelegate(IAttachmentControlDelegate* controllerDelegate);

	virtual IAttachmentControlDelegate* getControlDelegate() const;

	virtual IEntityAttachment* attachEntity(EmberEntity& entity);

	void setPosition(const WFMath::Point<3>& position, const WFMath::Quaternion& orientation);

	Ogre::Node* getSceneNode() const;

	virtual void updatePosition();

protected:

	Ogre::Node* mSceneNode;
	INodeProvider* mNodeProvider;

	SceneNodeController* mAttachmentController;

	void setupListeners();

	void entity_VisibilityChanged(bool visible);


};

}
#endif /* EMBEROGRESCENENODEATTACHMENT_H_ */
