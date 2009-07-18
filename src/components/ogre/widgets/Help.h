//
// C++ Interface: Help
//
// Description:
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2005
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
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREHELP_H
#define EMBEROGREHELP_H

#include "Widget.h"
#include "framework/ConsoleObject.h"
#include <elements/CEGUIGUISheet.h>
#include <CEGUIEvent.h>

namespace EmberOgre {
class GUIManager;
class EmberEntity;
namespace Gui {
class Widget;

/**
@author Erik Hjortsberg
*/
class Help : public Widget
{
public:
    Help();

    virtual ~Help();
	virtual void buildWidget();
	virtual void runCommand(const std::string &command, const std::string &args);

	/**
	 *    Called each frame.
	 * @param evt
	 */
	virtual void frameStarted(const Ogre::FrameEvent& evt);

	const Ember::ConsoleCommandWrapper HelpCommand;

protected:

	void show();

	void hide();

	Ogre::Real mTimeUntilShowBlurb, mTimeBlurbShown, mTimeToShowBlurb;

	CEGUI::GUISheet* mBlurb;

	/**
	 *    hooked to EmberOgre::EventCreatedAvatarEntity, show a help blurb about the movement mode
	 * @param entity
	 */
	void EmberOgre_CreatedAvatarEntity(EmberEntity* entity);

};
};
};

#endif
