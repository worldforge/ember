/*
 Copyright (C) 2012 Erik Ogenvik

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

#ifndef OGRECONFIGURATOR_H_
#define OGRECONFIGURATOR_H_

#include <OgreConfigOptionMap.h>
#include <string>

namespace CEGUI
{
class EventArgs;
}

namespace Ember
{
namespace OgreView
{

class OgreConfigurator
{
public:
	OgreConfigurator();
	virtual ~OgreConfigurator();

	bool configure();

	std::string getChosenRenderSystemName() const;
	Ogre::ConfigOptionMap getConfigOptions() const;

protected:

	bool buttonOkClicked(const CEGUI::EventArgs& args);
	bool buttonCancelClicked(const CEGUI::EventArgs& args);
	bool buttonAdvancedClicked(const CEGUI::EventArgs& args);

	static int mLastFrameTime;
	bool mCancel;
	bool mContinueInLoop;
	std::string mChosenRenderSystemName;
	Ogre::ConfigOptionMap mConfigOptions;
};

}
}
#endif /* OGRECONFIGURATOR_H_ */
