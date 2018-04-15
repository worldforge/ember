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

#ifndef EMBER_SKELETONDISPLAY_H
#define EMBER_SKELETONDISPLAY_H

#include <OgreFrameListener.h>

namespace Ogre {
class ManualObject;

class Entity;
}

namespace Ember {
namespace OgreView {

/**
 * Shows the skeleton of an entity, if such exists.
 */
class SkeletonDisplay : public Ogre::FrameListener {
public:
	explicit SkeletonDisplay(Ogre::Entity& entity);

	~SkeletonDisplay() override;

	bool frameStarted(const Ogre::FrameEvent& evt) override;

private:

	Ogre::Entity& mEntity;

	Ogre::ManualObject* mManualObject;


	void update();

	void drawChild(Ogre::Node* const& parent, Ogre::Node* const& child);
};

}
}
#endif //EMBER_SKELETONDISPLAY_H
