//
// C++ Interface: Sun
//
// Description:
//
//
// Author: Erik Ogenvik <erik@ogenvik.org>, (C) 2004
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.//
//
#ifndef EMBEROGRE_SUN_H
#define EMBEROGRE_SUN_H

#include "components/ogre/EmberOgrePrerequisites.h"


#include <sigc++/signal.h>

#include "framework/ConsoleCommandWrapper.h"

#include "LensFlare.h"
#include <OgreFrameListener.h>

namespace Ember {
namespace OgreView {

namespace Environment {

/**
@author Erik Ogenvik
*/
class Sun :
		public ConsoleObject, public Ogre::FrameListener {
public:
	Sun(Ogre::Camera* camera, Ogre::SceneManager* sceneMgr);

	~Sun() override;

	/**
	 *    Reimplements the ConsoleObject::runCommand method
	 * @param command
	 * @param args
	 */
	void runCommand(const std::string& command, const std::string& args) override;

	/**
	* emitted when the sun changes position
	*/
	sigc::signal<void(Sun*, Ogre::Vector3)> EventUpdatedSunPosition;

	/**
	* emitted when the sun changes colour
	*/
	sigc::signal<void(Sun*, Ogre::ColourValue)> EventUpdatedSunColour;

	/**
	* emitted when the world ambient light is changed
	*/
	sigc::signal<void(Sun*, Ogre::ColourValue)> EventUpdatedAmbientLight;

	const ConsoleCommandWrapper SetSunPosition;
	const ConsoleCommandWrapper SetSunColour;
	const ConsoleCommandWrapper SetAmbientLight;


	/**
	 *    changes the sun's position
	 * @param position a new world position
	 */
	void setSunPosition(const Ogre::Vector3& position);


	/**
	 *    changes the colour of the sun light
	 * @param colour
	 */
	void setSunColour(const Ogre::ColourValue& colour);


	/**
	 *    changes the ambient light
	 * @param colour
	 */
	void setAmbientLight(const Ogre::ColourValue& colour);


	bool frameEnded(const Ogre::FrameEvent& event) override;

protected:
	Ogre::Light* mSun;

	Ogre::SceneNode* mSunNode;
	LensFlare mLensFlare;

};

}

}

}

#endif
