/*
    Copyright (C) 2002  Alistair Davidson, the Viewforge Project, Martin Pollard (xmp)

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
#include "services/EmberServices.h"

#include "EntityListener.h"


namespace Ember {

EntityListener::EntityListener()
{

    /* Find out where the Eris world instance resides... */
    Eris::View *w = EmberServices::getInstance()->getServerService()->getView();

    /* Connect to the relevant View signals */
    w->EntityCreated.connect( SigC::slot( *this, &EntityListener::entityCreate ) );

    w->EntityDeleted.connect( SigC::slot( *this, &EntityListener::entityDelete ) );

    w->EntitySeen.connect( SigC::slot( *this, &EntityListener::entered ) );

    w->Appearance.connect( SigC::slot( *this, &EntityListener::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &EntityListener::disappearance ) );
}

EntityListener::~EntityListener()
{
#if 0
    Eris::View *w = EmberServices::getInstance()->getServerService()->getView();
    w->EntityCreate.disconnect();
#endif
}


/* Eris::View entity signals */

void EntityListener::entityCreate( Eris::Entity *e )
{
    /* Whenever a new entity is created, make sure to connect to those signals
       too */

    // Xmp's Notes: hmm need to work out how to connect these
    e->ChildAdded.connect( SigC::slot( *this, &EntityListener::addedMember ) );

    e->ChildRemoved.connect( SigC::slot( *this, &EntityListener::removedMember ) );

    e->LocationChanged.connect( SigC::slot( *this, &EntityListener::recontainered ) );

    e->Changed.connect( SigC::slot( *this, &EntityListener::changed ) );

    e->Moved.connect( SigC::slot( *this, &EntityListener::moved ) );

    e->Say.connect( SigC::slot( *this, &EntityListener::say ) );
}


}
