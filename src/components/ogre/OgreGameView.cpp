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

#include "OgreGameView.h"

#include "services/DimeServices.h"

namespace dime {

OgreGameView::OgreGameView()
{
	fprintf(stderr,"WOOHOOO --- GAMEVIEW IS CREATED");
    /* Find out where the Eris world instance resides... */
    Eris::World *w = DimeServices::getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( *this, &OgreGameView::entityCreate ) );

    w->EntityDelete.connect( SigC::slot( *this, &OgreGameView::entityDelete ) );

    w->Entered.connect( SigC::slot( *this, &OgreGameView::entered ) );

    w->Appearance.connect( SigC::slot( *this, &OgreGameView::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &OgreGameView::disappearance ) );
}

OgreGameView::~OgreGameView()
{}

void OgreGameView::repaint(DrawDevice* ddevice)
{}

/* Eris::World entity signals */

void OgreGameView::entityCreate( Eris::Entity *e )
{
    /* Whenever a new entity is created, make sure to connect to those signals
       too */
	fprintf(stderr,"WOOHOOO --- AN ENTITY IS CREATED");
    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *this, &OgreGameView::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *this, &OgreGameView::removedMember ) );

    e->Recontainered.connect( SigC::slot( *this, &OgreGameView::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *this, &OgreGameView::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *this, &OgreGameView::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *this, &OgreGameView::say ), e ) );
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

void OgreGameView::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{}

void OgreGameView::say( const std::string &s, Eris::Entity *e )
{
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << e->getName() << " says: "<< s<< ENDM;
}

void OgreGameView::addedMember(Eris::Entity *e)
{}

void OgreGameView::removedMember(Eris::Entity *e)
{}

void OgreGameView::runCommand(const std::string &command, const std::string &args)
{
}

}
