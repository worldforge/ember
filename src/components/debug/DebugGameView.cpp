/*  Copyright (C) 2002  Alistair Davidson, the Worldforge Project, Martin Pollard (xmp)

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

namespace dime {

DebugGameView::DebugGameView()
{

    /* Find out where the Eris world instance resides... */
    new Eris::World *w = DimeService.getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( this, &EntityCreate() ) );

    w->EntityDelete.connect( SigC::slot( this, &EntityDelete() ) );

    w->Entered.connect( SigC::slot( this, &Entered() ) );

    w->Appearance.connect( SigC::slot( this, &Appearance() ) );

    w->Disappearance.connect( SigC::slot( this, &Disappearance() ) );

}

DebugGameView::~DebugGameView()
{}

void DebugGameView::repaint()
{}

/* Eris::World entity signals */

void DebugGameView::EntityCreate( Eris::Entity *e )
{

    /* Whenever a new entity is created, make sure to connect to those signals
       too */
    e->AddedMember.connect( SigC::slot( this, &AddedMember() ) );

    e->RemovedMember.connect( SigC::slot( this, &RemovedMember() ) );

    e->Recontainered.connect( SigC::slot( this, &Recontainered() ) );

    e->Changed.connect( SigC::bind( SigC::slot( this, &Changed() ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( this, &Moved() ), e ) );

    e->Say.connect( SigC::slot( SigC::slot( this, &Say() ), e ) );
}


void DebugGameView::EntityDelete( Eris::Entity *e )
{}

void DebugGameView::Entered( Eris::Entity *e )
{}

void DebugGameView::Appearance( Eris::Entity *e )
{}

void DebugGameView::Disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

void DebugGameView::Recontainered( Eris::Entity *e, Eris::Entity *c )
{}

void DebugGameView::Changed( const Eris::StringSet &s, Eris::Entity *e  )
{}

void DebugGameView::Moved( const Eris::Coord &c, Eris::Entity *e )
{}

void DebugGameView::Say( const std::string &s, Eris::Entity *e )
{}


}
