/*
 Copyright (C) 2002  Martin Pollard (Xmp)
	
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

#ifndef STATEMANAGER_H
#define STATEMANAGER_H

// Included headers from the current project

// Included custom library headers
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

// Included system headers
#include <string>
#include <list>

namespace dime {

/**
 * General State Manager for Dime.
 *
 * Provides a system whereby dime can have states.  Whenever a state change is
 * requested this class will automatically unload certain things and load
 * others.  This shall include but not be limited to: the gui; the layout;
 * and Dime's services.
 *
 * A short piece of example code demonstarting how this class it is used,
 * and in what context, is encouraged.
 *
 * @author Martin Pollard aka Xmp
 *
 * NOTE: You can also specify the author for individual methods
 * if different persons have created them.
 * It is also possible to have multiple @author tags for a method.
 * Only add yourself as an @author if you have done serious work
 * on a class/method, and can help fixing bugs in it, etc.
 * If you just fixed a bug or added a short code snipplet you
 * don't need to add yourself.
 *
 * NOTE: Add other related classes here, doxygen will create links to them.
 */

class StateManager
{
    //======================================================================
    // Inner Classes, Typedefs, and Enums
    //======================================================================
    public:


    //======================================================================
    // Public Constants
    //======================================================================
    public:


    //======================================================================
    // Private Constants
    //======================================================================
    private:


    //======================================================================
    // Private Variables
    //======================================================================/
    private:

    /**
     * Holds the name of the file we are loading the states from
     */
    std::string myStateFile;

    /**
     * Holds the name of the state we are currently in
     */
    std::string myCurrentState;

    /**
     * Holds the XML tree of our open states doc
     */
    xmlDocPtr myStateDoc;

    //======================================================================
    // Public Methods
    //======================================================================
    public:

    //----------------------------------------------------------------------
    // Constructors

    /**
     * Creates a new StateManager using default values.
     */
    StateManager(const std::string& stateFile, const std::string& initialState)
      : myStateFile(stateFile), myStateDoc(NULL)
    {
      // Load the XML Document containing the States
      xmlNodePtr cur;

      myStateDoc = xmlParseFile(myStateFile.c_str());
      if (!myStateDoc)
	{
	  xmlFreeDoc(myStateDoc);
	  throw "Missing State Doc";
	}

      cur = xmlDocGetRootElement(myStateDoc);

      if (!cur)
	{
	  xmlFreeDoc(myStateDoc);
	  throw "Empty State Doc";
	}

      if (xmlStrcmp(cur->name, (const xmlChar *) "states"))
	{
	  xmlFreeDoc(myStateDoc);
	  throw "document of the wrong type, root node != states";
	}
      // OK if we're here then we've loaded the doc

      setState(initialState);
    }

    /**
     * Copy constructor.
     */
    StateManager( const StateManager &source )
    {
        // Use assignment operator to do the copy
        // NOTE: If you need to do custom initialization in the constructor this may not be enough.
        *this = source;
    }


    /**
     * Assignment operator.
     */
    StateManager &operator= ( const StateManager &source )
    {
        // Copy fields from source class to this class here.

        // Return this object with new value
        return *this;
    }


    //----------------------------------------------------------------------
    // Destructor

    /**
     * Deletes a StateManager instance.
     */
    virtual ~StateManager ()
    {
      // TODO: Free any allocated resources here.
      xmlFreeDoc(myStateDoc);
    }


    //----------------------------------------------------------------------
    // Getters

    // Example of a getter method:

    /**
     * Gets the value of State of this StateManager
     */
    std::string getStateName() const
    {
        return myCurrentState;
    }


    //----------------------------------------------------------------------
    // Setters

    /**
     * Sets the value of State of this StateManager.
     * returns true if successful
     */
    bool setState( const std::string& newState );

    //----------------------------------------------------------------------
    // Other public methods
    // NOTE: Group related public methods together and create a separator comment like above for them.
    /**
     * Check if a state exists
     */
    bool existsState( const std::string& state )
      {
	xmlNodePtr cur = findState(state);
	return (bool)cur;
      }

    //======================================================================
    // Protected Methods
    //======================================================================
    protected:
    /**
     * Locates a state in the StateFile and returns a pointer to the node
     * it's on.
     */
    xmlNodePtr findState(const std::string& state);

    //======================================================================
    // Private Methods
    //======================================================================
    private:


    //======================================================================
    // Disabled constructors and operators
    //======================================================================
    private:


}; // End of StateManager

} // End of dime namespace

#endif
