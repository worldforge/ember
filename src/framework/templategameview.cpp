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

#include "TemplateGameView.h"

#include "services/DimeServices.h"

namespace dime {

TemplateGameView::TemplateGameView()
{

    /* Find out where the Eris world instance resides... */
    Eris::World *w = DimeServices::getInstance()->getServerService()->getWorld();

    /* Connect to the relevant World signals */
    w->EntityCreate.connect( SigC::slot( *this, &TemplateGameView::entityCreate ) );

    w->EntityDelete.connect( SigC::slot( *this, &TemplateGameView::entityDelete ) );

    w->Entered.connect( SigC::slot( *this, &TemplateGameView::entered ) );

    w->Appearance.connect( SigC::slot( *this, &TemplateGameView::appearance ) );

    w->Disappearance.connect( SigC::slot( *this, &TemplateGameView::disappearance ) );
}

TemplateGameView::~TemplateGameView()
{}

void TemplateGameView::repaint(DrawDevice* ddevice)
{}

/* Eris::World entity signals */

void TemplateGameView::entityCreate( Eris::Entity *e )
{
    /* Whenever a new entity is created, make sure to connect to those signals
       too */

    // Xmp's Notes: hmm need to work out how to connect these
    e->AddedMember.connect( SigC::slot( *this, &TemplateGameView::addedMember ) );

    e->RemovedMember.connect( SigC::slot( *this, &TemplateGameView::removedMember ) );

    e->Recontainered.connect( SigC::slot( *this, &TemplateGameView::recontainered ) );

    e->Changed.connect( SigC::bind( SigC::slot( *this, &TemplateGameView::changed ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( *this, &TemplateGameView::moved ), e ) );

    e->Say.connect( SigC::bind( SigC::slot( *this, &TemplateGameView::say ), e ) );
}


void TemplateGameView::entityDelete( Eris::Entity *e )
{}

void TemplateGameView::entered( Eris::Entity *e )
{}

void TemplateGameView::appearance( Eris::Entity *e )
{}

void TemplateGameView::disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

void TemplateGameView::recontainered( Eris::Entity *e, Eris::Entity *c )
{}

void TemplateGameView::changed( const Eris::StringSet &s, Eris::Entity *e  )
{}

void TemplateGameView::moved( const WFMath::Point< 3 > &p, Eris::Entity *e )
{}

void TemplateGameView::say( const std::string &s, Eris::Entity *e )
{
    LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::VERBOSE) << e->getName() << " says: "<< s<< ENDM;
}

void TemplateGameView::addedMember(Eris::Entity *e)
{}

void TemplateGameView::removedMember(Eris::Entity *e)
{}

void TemplateGameView::runCommand(const std::string &command, const std::string &args)
{
}

}
