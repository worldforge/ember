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

#ifndef AUTHORINGVISUALIZATION_H_
#define AUTHORINGVISUALIZATION_H_
#include "components/ogre/OgreIncludes.h"
#include <sigc++/trackable.h>

namespace EmberOgre
{

class EmberEntity;

namespace Authoring
{
class AuthoringVisualization : public virtual sigc::trackable
{
public:
	AuthoringVisualization(EmberEntity& entity, Ogre::SceneNode* sceneNode);
	virtual ~AuthoringVisualization();

	Ogre::SceneNode* getSceneNode() const;

	void updatePositionAndOrientation();

protected:
	EmberEntity& mEntity;
	Ogre::SceneNode* mSceneNode;
	Ogre::Entity* mGraphicalRepresentation;

	void entity_Moved();
	void createGraphicalRepresentation();
	void removeGraphicalRepresentation();
};

}
}
#endif /* AUTHORINGVISUALIZATION_H_ */
