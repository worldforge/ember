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

#ifndef AUTHORINGMANAGER_H_
#define AUTHORINGMANAGER_H_

#include "framework/ConsoleCommandWrapper.h"
#include "framework/ConsoleObject.h"
namespace Eris
{
class View;
class Entity;
}

namespace EmberOgre
{

namespace Authoring
{

class AuthoringHandler;

class AuthoringManager: public Ember::ConsoleObject
{
public:
	AuthoringManager(Eris::View& view);
	virtual ~AuthoringManager();

	void displayAuthoringVisualization();

	void hideAuthoringVisualization();

	/**
	 * @copydoc ConsoleObject::runCommand
	 */
	virtual void runCommand(const std::string &command, const std::string &args);

	const Ember::ConsoleCommandWrapper DisplayAuthoringVisualizations;

	const Ember::ConsoleCommandWrapper HideAuthoringVisualizations;

protected:

	Eris::View& mView;
	AuthoringHandler* mHandler;

};
}
}

#endif /* AUTHORINGMANAGER_H_ */
