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

#include "TemplateEntityListener.h"

#include "services/DimeServices.h"

namespace dime {

TemplateEntityListener::TemplateEntityListener()
{

    /* Find out where the Eris world instance resides... */
    Eris::World *w = DimeServices::getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( *this, &TemplateEntityListener::entityCreate ) );

    w->EntityDelete.connect( SigC::slot( *this, &TemplateEntityListener::entityDelete ) );

    w->Entered.connect( SigC::slot( *this, &TemplateEntityListener::entered ) );

    w->Appearance.connect( SigC::slot( *this, &TemplateEntityListener::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &TemplateEntityListener::disappearance ) );
}

TemplateEntityListener::~TemplateEntityListener()
{}

void TemplateEntityListener::repaint(DrawDevice* ddevice)
{}

/* Eris::World entity signals */

void TemplateEntityListener::entityCreate( Eris::Entity *e )
{
    /* Whenever a new entity is created, make sure to connect to those signals
       too */

    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *this, &TemplateEntityListener::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *this, &TemplateEntityListener::removedMember ) );

    e->Recontainered.connect( SigC::slot( *this, &TemplateEntityListener::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *this, &TemplateEntityListener::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *this, &TemplateEntityListener::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *this, &TemplateEntityListener::say ), e ) );
}


void TemplateEntityListener::entityDelete( Eris::Entity *e )
{}

void TemplateEntityListener::entered( Eris::Entity *e )
{}

void TemplateEntityListener::appearance( Eris::Entity *e )
{}

void TemplateEntityListener::disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

void TemplateEntityListener::recontainered( Eris::Entity *e, Eris::Entity *c )
{}

void TemplateEntityListener::changed( const Eris::StringSet &s, Eris::Entity *e  )
{}

void TemplateEntityListener::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{}

void TemplateEntityListener::say( const std::string &s, Eris::Entity *e )
{
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << e->getName() << " says: "<< s<< ENDM;
}

void TemplateEntityListener::addedMember(Eris::Entity *e)
{}

void TemplateEntityListener::removedMember(Eris::Entity *e)
{}

void TemplateEntityListener::runCommand(const std::string &command, const std::string &args)
{
}

}
