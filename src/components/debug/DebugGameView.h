/*  Copyright (C) 2002  Alistair Davidson, the Worldforge Project, Martin Pollard (Xmp)

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

#ifndef DEBUG_GAME_VIEW_H
#define DEBUG_GAME_VIEW_H

#include <Eris/Entity.h>
#include <Eris/World.h>

#include <sigc++/signal_system.h>

//#include "framework/Component.h"

namespace dime {

/**
 * This is a debugging game view for use by DIME game view developers.
 *
 * It contains all the essential methods and code, with plenty of comments to
 * show you how to use it. Essentially, it should be possible to "fill in the
 * blanks" to create a working game view. That's not to say that doing so will
 * be easy though ;)
 *
 * @author: AlistairD
 * @author: Xmp
 *
 *
 * Class declaration
 * We're using SigC++, so we have to inherit from SigC::Object... you should
 * read up on SigC++ ( http://libsigc.sourceforge.net ) if you're not familiar
 * with it.
 *
 * Component is a standard dime class that also has to be inherited from.
 */

class DebugGameView : public SigC::Object //: public Component
{


    //======================================================================
    //Public Methods
    //======================================================================
    public:


        /**
         * Constructor
         * This grabs a pointer to the current Eris entity from DimeService and
         * connects ithe relevant signals to Game View methods. If you're not using
         * any of those methods (Entered is the most likely to be unused IMO) then you
         * should also remove the connection code.
         */
        DebugGameView();

        /** Destructor, stick cleanup stuff her as per usual */
        ~DebugGameView();

        /**
         * For the time being at least (the API isn't even vaguely stable yet),
         * every GameView needs a "repaint()" function. This is where you put
         * your drawing code.
         *
         * Iterate through your world model and blit to your heart's content :)
         */
        void repaint(DrawDevice *ddevice);

        /* Eris::World entity signals (see eris\src\world.h for more info) */

        /**
         * Called when an entity is created. This connects entity-specific
         * signals to methods in the game view. In the case of Changed and
         * Moved, a pointer to the entity is bound in because these signals
         * do not provide the pointer by themselves.
         *
         * You should add in code that inserts a pointer to the entity's media
         * into your world model.
         *
         * @param e A pointer to the Eris entity that has been created.
         */
        void entityCreate( Eris::Entity *e );

        /**
         * Called on entity deletion. You should remove all information you
         * hold about the entity.
         *
         * @param e A pointer to the Eris entity that has been deleted.
         *
         */
        void entityDelete( Eris::Entity *e );

        /**
         * Called only once, when the player enters the game world. It's
         * possible that you won't need this one.
         *
         * @param e A pointer to the Eris entity
         *
         */
        void entered( Eris::Entity *e );

        /** Called when an entity become visible. You'll probably want to add
         * a media pointer to your world model at this point.
         *
         * @param e A pointer to the Eris entity
         *
         */
        void appearance( Eris::Entity *e );

        /**
         * Called when an entity becomes invisible. You should remove the media
         * pointer corresponding to the entity from your world view, but retain
         * any additional data you're holding about the entity.
         *
         * @param e A pointer to the Eris entity
         *
         */
        void disappearance( Eris::Entity *e );


        /* Eris::Entity signals  (see eris\src\entity.h for more info)*/

        /**
         * Called when an entity changes its container. This may require
         * changes to your world model, but some gameviews can safely ignore
         * this signal.
         *
         * @param e A pointer to the Eris entity that has been recontainered
         * @param c A pointer to the Eris entity that is the new container for e
         */
        void recontainered( Eris::Entity *e, Eris::Entity *c );

        /**
         * I'm not sure what this does. Let's ignore it until I can track down
         * James and bop him on the head for writing unhelpful comments ;)
         * NOTES: I suspect this is when an attribute of the object is changed.
         */
        void changed( const Eris::StringSet &s, Eris::Entity *e );

        /**
         * Called when the entity moves. Here you should alter the position
         * of the media pointer in your world model... this may involve
         * removing it from where it was before the entity moved and
         * placing it in the new position, in which case you'll need
         * a reverse-lookup of some kinda- WFMath::Point<3> is the new
         * entity coordinate, the old one is only known if stored by you.
         *
         * @param c The new coordinates of the entity
         * @param e A pointer to the Eris entity that has moved
         */
        void moved( const WFMath::Point< 3 > &, Eris::Entity *e );

        /**
         * Called when the entity speaks. You'll probably want to display the
         * speech on the screen somehow.
         *
         * @param s A string containing the speech
         * @param e A pointer to the Eris entity
         */
        void say( const std::string &s, Eris::Entity *e );

        /**
         * Sadly undocumented
         */
        void addedMember(Eris::Entity *e);

        /**
         * Also sadly undocumented
         */
        void removedMember(Eris::Entity *e);

    private:

        /**
         * You should have some form of world model here. For example, the
         * one-dimensional DebugGameView uses a vector.
         *
         * James has suggested a Quad Tree for 3D game views (that includes
         * layered 2D views)
         */

}; //End of class decleration

} // End of application namespace

#endif // DEBUG_GAME_VIEW_H
