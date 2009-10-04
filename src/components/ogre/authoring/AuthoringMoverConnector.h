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

#ifndef AUTHORINGMOVERCONNECTOR_H_
#define AUTHORINGMOVERCONNECTOR_H_

#include <sigc++/trackable.h>

namespace EmberOgre
{
class EmberEntity;

namespace Authoring
{

class AuthoringManager;
class EntityMoveManager;
class EntityMover;

class AuthoringMoverConnector : public virtual sigc::trackable
{
public:
	AuthoringMoverConnector(AuthoringManager& authoringManager, EntityMoveManager& moveManager);
	virtual ~AuthoringMoverConnector();

private:

	AuthoringManager& mAuthoringManager;
	EntityMoveManager& mMoveManager;

	void moveManager_StartMoving(EmberEntity& entity, EntityMover& mover);
	void moveManager_FinishedMoving();
	void moveManager_CancelledMoving();
};

}
}
#endif /* AUTHORINGMOVERCONNECTOR_H_ */
