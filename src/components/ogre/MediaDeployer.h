/*
	ConsoleObjectImpl.h by Miguel Guzman (Aglanor)
	Copyright (C) 2002 Miguel Guzman & The Worldforge Project

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

#ifndef __DimeOgre_MediaDeployer_H__
#define __DimeOgre_MediaDeployer_H__

namespace DimeOgre {

class MediaDeployer
{

	public:

	~MediaDeployer();

	static MediaDeployer & getSingleton(void);

	bool addMedia(std::string);
	bool addMedia(std::string, std::string, Ogre::Vector3);

	private:

	/**
	 * Private constructor (for singleton)
	 */
	MediaDeployer(void);

	/**
	 * Instance variable for singleton console object implementation.
	 */
    static MediaDeployer* _mediaDeployerInstance;

	/**
	 * Scene manager to which deploy the media
	 */
	Ogre::SceneManager* mSceneMgr;

}; // End of class declaration
}


#endif
