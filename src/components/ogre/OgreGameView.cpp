/*  Copyright (C) 2002 Miguel Guzmán Miranda (Aglanor)
	Based on the templategameview.cpp code by
	Alistair Davidson and the Worldforge Project

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

#include "templategameview.h"

#include "main/DimeServices.h"

namespace dime {

OgreGameView::OgreGameView()
{

    /* Find out where the Eris world instance resides... */
    new Eris::World *w = DimeService.getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( this, &EntityCreate ) );

    w->EntityDelete.connect( SigC::slot( this, &EntityDelete ) );

    w->Entered.connect( SigC::slot( this, &Entered ) );

    w->Appearance.connect( SigC::slot( this, &Appearance ) );

    w->Disappearance.connect( SigC::slot( this, &Disappearances) );

}

OgreGameView::~OgreGameView()
{}

void OgreGameView::repaint()
{}

/* Eris::World entity signals */

void OgreGameView::EntityCreate( Eris::Entity *e )
{

    /* Whenever a new entity is created, make sure to connect to those signals
       too */
    e->AddedMember.connect( SigC::slot( this, &AddedMember ) );

    e->RemovedMember.connect( SigC::slot( this, &RemovedMember ) );

    e->Recontainered.connect( SigC::slot( this, &Recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( this, &Changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( this, &Moved ), e ) );

    e->Say.connect( SigC::bind(SigC::slot( this, &Say ), e ) );
}


void OgreGameView::entityDelete( Eris::Entity *e )
{}

void OgreGameView::entered( Eris::Entity *e )
{}

void OgreGameView::appearance( Eris::Entity *e )
{}

void OgreGameView::disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

void OgreGameView::recontainered( Eris::Entity *e, Eris::Entity *c )
{}

void OgreGameView::changed( const Eris::StringSet &s, Eris::Entity *e  )
{}

void OgreGameView::moved( const Eris::Coord &c, Eris::Entity *e )
{}

void OgreGameView::say( const std::string &s, Eris::Entity *e )
{}


}
