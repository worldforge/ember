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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "services/server/ServerService.h"
#include "services/DimeServices.h"

#include "EntityListener.h"


namespace dime {

EntityListener::EntityListener()
{

    /* Find out where the Eris world instance resides... */
    Eris::World *w = DimeServices::getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( *this, &EntityListener::entityCreate ) );

    w->EntityDelete.connect( SigC::slot( *this, &EntityListener::entityDelete ) );

    w->Entered.connect( SigC::slot( *this, &EntityListener::entered ) );

    w->Appearance.connect( SigC::slot( *this, &EntityListener::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &EntityListener::disappearance ) );
}

EntityListener::~EntityListener()
{
#if 0
    Eris::World *w = DimeServices::getInstance()->getServerService()->getWorld();
    w->EntityCreate.disconnect();
#endif
}


/* Eris::World entity signals */

void EntityListener::entityCreate( Eris::Entity *e )
{
    /* Whenever a new entity is created, make sure to connect to those signals
       too */

    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *this, &EntityListener::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *this, &EntityListener::removedMember ) );

    e->Recontainered.connect( SigC::slot( *this, &EntityListener::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *this, &EntityListener::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *this, &EntityListener::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *this, &EntityListener::say ), e ) );
}


}
