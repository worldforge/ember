/*
 Copyright (C) 2017 Erik Ogenvik

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

#ifndef EMBER_CEGUISETUP_H
#define EMBER_CEGUISETUP_H

#include <memory>
#include <boost/noncopyable.hpp>

namespace Ogre {
class RenderWindow;
}

namespace CEGUI {
class XMLParser;

class OgreRenderer;

class System;

class OgreResourceProvider;

class OgreImageCodec;
}

namespace Ember::Cegui {
class CEGUILogger;

class CEGUISetup : public boost::noncopyable {
public:

	explicit CEGUISetup(Ogre::RenderWindow& window);

	~CEGUISetup();

	static CEGUI::OgreRenderer& createRenderer(Ogre::RenderWindow* renderWindow);

	Ogre::RenderWindow& getRenderWindow() const;

	CEGUI::System& getSystem() const;

	CEGUI::OgreRenderer& getRenderer() const;

	CEGUI::OgreResourceProvider& getResourceProvider() const;

	CEGUI::OgreImageCodec& getImageCodec() const;

protected:
	/**
	 * @brief We'll provide our own CEGUI logger instance, which will route all cegui log messages to the main ember log.
	 */
	std::unique_ptr<CEGUILogger> mCEGUILogger;

	Ogre::RenderWindow& mWindow;
	CEGUI::System* mGuiSystem;
	CEGUI::OgreRenderer* mGuiRenderer;
	CEGUI::OgreResourceProvider* mOgreResourceProvider;
	CEGUI::OgreImageCodec* mOgreImageCodec;
	std::unique_ptr<CEGUI::XMLParser> mXmlParser;

};

inline Ogre::RenderWindow& CEGUISetup::getRenderWindow() const {
	return mWindow;
}

inline CEGUI::System& CEGUISetup::getSystem() const {
	return *mGuiSystem;
}

inline CEGUI::OgreRenderer& CEGUISetup::getRenderer() const {
	return *mGuiRenderer;
}

inline CEGUI::OgreResourceProvider& CEGUISetup::getResourceProvider() const {
	return *mOgreResourceProvider;
}

inline CEGUI::OgreImageCodec& CEGUISetup::getImageCodec() const {
	return *mOgreImageCodec;
}

}

#endif //EMBER_CEGUISETUP_H
