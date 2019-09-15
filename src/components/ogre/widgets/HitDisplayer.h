/*
 Copyright (C) 2019 Erik Ogenvik

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

#ifndef EMBER_HITDISPLAYER_H
#define EMBER_HITDISPLAYER_H


#include <Eris/View.h>
#include <Ogre.h>

namespace Ember {
namespace OgreView {

namespace Gui {

/**
 * Responsible for showing hit text indicators whenever an Entity is hit.
 */
class HitDisplayer : public virtual sigc::trackable, public Ogre::FrameListener {

public:
	explicit HitDisplayer(Eris::View& view, Ogre::SceneManager& sceneManager);

	~HitDisplayer() override;

	bool frameStarted(const Ogre::FrameEvent& evt) override;

protected:
	void entityActed(Eris::Entity& entity, const Atlas::Objects::Operation::RootOperation& arg);

	Eris::View& mView;
	Ogre::SceneManager& mSceneManager;

	struct Hit {
		float time;
		Ogre::SceneNode* node;
	};

	std::list<std::shared_ptr<Hit>> mHits;


	void createHit(const Ogre::Vector3& pos, std::string basicString);
};

}
}
}
#endif //EMBER_HITDISPLAYER_H
