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

#include "AuthoringMoverConnector.h"
#include "AuthoringManager.h"
#include "EntityMoveManager.h"
namespace EmberOgre
{

namespace Authoring
{
AuthoringMoverConnector::AuthoringMoverConnector(AuthoringManager& authoringManager, EntityMoveManager& moveManager) :
	mAuthoringManager(authoringManager), mMoveManager(moveManager)
{
	mMoveManager.EventStartMoving.connect(sigc::mem_fun(*this, &AuthoringMoverConnector::moveManager_StartMoving));
	mMoveManager.EventFinishedMoving.connect(sigc::mem_fun(*this, &AuthoringMoverConnector::moveManager_FinishedMoving));
	mMoveManager.EventCancelledMoving.connect(sigc::mem_fun(*this, &AuthoringMoverConnector::moveManager_CancelledMoving));
}

AuthoringMoverConnector::~AuthoringMoverConnector()
{
}

void AuthoringMoverConnector::moveManager_StartMoving(EmberEntity& entity, EntityMover& mover)
{
	mAuthoringManager.startMovement(entity, mover);
}

void AuthoringMoverConnector::moveManager_FinishedMoving()
{
	mAuthoringManager.stopMovement();
}
void AuthoringMoverConnector::moveManager_CancelledMoving()
{
	mAuthoringManager.stopMovement();
}

}
}
