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


/* MAIN PROG BEGINS */

namespace dime {

/* Class declaration
 * We're using SigC++, so we have to inherit from SigC::Object... you should
 * read up on SigC++ ( http://libsigc.sourceforge.net ) if you're not familiar
 * with it.
 *
 * Component is a standard dime class that also has to be inherited from.
 */

class TemplateGameView : public SigC::Object : public Component
{

    public:

        TemplateGameView();
        ~TemplateGameView();

        /* For the time being at least (the API isn't even vaguely stable yet),
         * every GameView needs a "repaint()" function. This is where you put
         * your drawing code.
         */
        void repaint();

        /* Eris::World entity signals (see eris\src\world.h for more info) */

        /* Called on entity creation. This should add a MediaObject pointer to
         * your world model. More on that in the .cpp */            
        void EntityCreate( Eris::Entity *e );

        /* Called on entity deletion. This should remove the pointer added
         * in EntityCreate. */
        void EntityDelete( Eris::Entity *e );

        /* Called only once, when the player enters the game world. It's
         * possible that you won't need this one. */
        void Entered( Eris::Entity *e );

        /* Called when an entity appears. Probably treated similarly to
         * EntityCreate. */
        void Appearance( Eris::Entity *e );

        /* Called when an entity disappears. Probably treated similarly to
         * EntityCreate, but without removing metadata and so on. */
        void Disappearance( Eris::Entity *e );


        /* Eris::Entity signals  (see eris\src\entity.h for more info)*/

        /* Called when an entity changes its container. This may require
         * changes to your world model, but some gameviews can safely ignore
         * this signal. */
        void Recontainered( Eris::Entity *e, Eris::Entity *c ); 

        /* I'm not sure what this does. Let's ignore it until I can track down
         * James and bop him on the head for writing unhelpful comments ;) */
        void Changed( const Eris::StringSet &s, Eris::Entity *e );

        /* Called when the entity moves. Here you should alter the position
         * of the media pointer in your world model... this may involve
         * removing it from where it was before the entity moved and
         * placing it in the new position, in which case you'll need
         * a reverse-lookup of some kinda- Eris::Coord &c is the new
         * entity coordinate, the old one is only known if stored by you. */
        void Moved( const Eris::Coord &c, Eris::Entity *e );

        /* Called when the entity speaks. You'll probably want to display the
         * speech on the screen somehow. */
        void Say( const std::string &s, Eris::Entity *e );

    private:

        /* You should have some form of world model here. For example, the
         * one-dimensional DebugGameView uses a vector.
         *
         * James has suggested a Quad Tree for 3D game views (that includes
         * layered 2D views)
         */

};



}