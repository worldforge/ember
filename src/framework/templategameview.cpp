/* Copyright (C) Alistair Davidson and the Worldforge Project, 2001 
 * Licensed under the General Public License of the Free Software Foundation.
 * See file "COPYING" for details.
 */

/* Template Game View.
 *
 * Revision history:
 * ver  - yyyy/mm/dd - author    - change descriptor
 * 0.01 - 2002/02/16 - AlistairD - Class derived from very early version of DebugGameView
 */

#include "templategameview.h"

namespace dime {

/* Constructor
 * This grabs a pointer to the current Eris entity from DimeService and
 * connects ithe relevant signals to Game View methods. If you're not using
 * any of those methods (Entered is the most likely to be unused IMO) then you
 * should also remove the connection code.
 */
TemplateGameView::TemplateGameView()
{
    new Eris::World *w = DimeService.getInstance()->getEntityService()->getErisInstance();

    w->EntityCreate.connect( SigC::slot( this, &EntityCreate() ) );

    w->EntityDelete.connect( SigC::slot( this, &EntityDelete() ) );

    w->Entered.connect( SigC::slot( this, &Entered() ) );

    w->Appearance.connect( SigC::slot( this, &Appearance() ) );

    w->Disappearance.connect( SigC::slot( this, &Disappearance() ) );

}

/* Destructor */
TemplateGameView::~DebugGameView()
{}

/* Repaint method. Iterate through your world model and blit to your heart's
 * content :) */
void TemplateGameView::repaint()
{}

/* Eris::World entity signals */


/* Called when an entity is created. This connects entity-specific signals to
 * methods in the game view. In the case of Changed and Moved, a pointer to the
 * entity is bound in because these signals do not provide the pointer by
 * themselves.
 *
 * You should add in code that inserts a pointer to the entity's media into
 * your world model.
 */
void TemplateGameView::EntityCreate( Eris::Entity *e )
{
    e->AddedMember.connect( SigC::slot( this, &AddedMember() ) );

    e->RemovedMember.connect( SigC::slot( this, &RemovedMember() ) );

    e->Recontainered.connect( SigC::slot( this, &Recontainered() ) );

    e->Changed.connect( SigC::bind( SigC::slot( this, &Changed() ), e ) );

    e->Moved.connect( SigC::bind( SigC::slot( this, &Moved() ), e ) );

    e->Say.connect( SigC::slot( SigC::slot( this, &Say() ), e ) );
}

/* Called when an entity is deleted. You should remove all information about
 * it from your world model. */
void TemplateGameView::EntityDelete( Eris::Entity *e )
{}

/* Called once, when the player enters the game world. I'm not sure if you'll
 * have a use for this. */
void TemplateGameView::Entered( Eris::Entity *e )
{}

/* Called when an entity become visible. You'll probably want to add a media
 * pointer to your world model at this point. */
void TemplateGameView::Appearance( Eris::Entity *e )
{}

/* Called when an entity becomes invisible. You should remove the media pointer
 * corresponding to the entity from your world view, but retain any additional
 * data you're holding about the entity. */
void TemplateGameView::Disappearance( Eris::Entity *e )
{}

/* Eris::Entity signals */

/* Called when an entity changes its container. This may require
 * changes to your world model, but some gameviews can safely ignore
 * this signal. */
void TemplateGameView::Recontainered( Eris::Entity *e, Eris::Entity *c )
{}

/* I'm not sure what this does. Let's ignore it until I can track down
 * James and bop him on the head for writing unhelpful comments ;) */
void TemplateGameView::Changed( const Eris::StringSet &s, Eris::Entity *e  )
{}


/* Called when the entity moves. Here you should alter the position
 * of the media pointer in your world model... this may involve
 * removing it from where it was before the entity moved and
 * placing it in the new position, in which case you'll need
 * a reverse-lookup of some kinda- Eris::Coord &c is the new
 * entity coordinate, the old one is only known if stored by you. 
 */
void TemplateGameView::Moved( const Eris::Coord &c, Eris::Entity *e )
{}

/* Called when the entity speaks. You'll probably want to display the
 * speech on the screen somehow. */
void TemplateGameView::Say( const std::string &s, Eris::Entity *e )
{}


}