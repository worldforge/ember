/*
    Copyright (C) 2002  Alistair Davidson, the Worldforge Project, Martin Pollard (xmp)

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

#include "DebugGameView.h"

#include "services/DimeServices.h"

namespace dime {

DebugGameView::DebugGameView() : EntityListener()
{
}

DebugGameView::~DebugGameView()
{}

void DebugGameView::repaint(DrawDevice* ddevice)
{}

/* Eris::World entity signals */

void DebugGameView::entityCreate( Eris::Entity *e )
{
    // Call base class to connect signals
    EntityListener::entityCreate( Eris::Entity *e )
}


void DebugGameView::entityDelete( Eris::Entity *e )
{}

void DebugGameView::entered( Eris::Entity *e )
{}

void DebugGameView::appearance( Eris::Entity *e )
{}

void DebugGameView::disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

void DebugGameView::recontainered( Eris::Entity *e, Eris::Entity *c )
{}

void DebugGameView::changed( const Eris::StringSet &s, Eris::Entity *e  )
{}

void DebugGameView::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{}

void DebugGameView::say( const std::string &s, Eris::Entity *e )
{
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << e->getName() << " says: "<< s<< ENDM;
}

void DebugGameView::addedMember(Eris::Entity *e)
{}

void DebugGameView::removedMember(Eris::Entity *e)
{}

void DebugGameView::runCommand(const std::string &command, const std::string &args)
{
}

}
