/*
    Copyright (C) 2004  Erik Hjortsberg
    
    Taken from the file OgreSingleton.h from the Ogre project, release 0.14
    (www.ogre3d.org)

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

#ifndef _EMBER_SINGLETON_H__
#define _EMBER_SINGLETON_H__

#include <cassert>

namespace Ember {

    /** 
    @brief Template class for creating single-instance global classes.
    
    Whenever you need a singleton in the system you should inherit from this, such as
    @code
    class Foo : public Ember::Singleton<Foo>
    @endcode
    
    In your implementation you're then required to intialize ms_Singleton to 0, such as this
    @code
    template<> Ember::Foo* Ember::Singleton<Ember::Foo>::ms_Singleton = 0;
    @endcode
    
    Whenever you need the singleton instance in your project you can then get it by calling @see getSingleton() or @see getSingletonPtr()
    Note that you still have to provide lifetime service and ownership and need to both create and destroy the instance where appropriate.
    */
    template <typename T> class Singleton
    {
    protected:

		/**
		@brief The static variable holding the singleton instance.
		Remember to instanciate this to 0 in your implementation.
		*/
		static T* ms_Singleton;

    public:
		
		/**
		 * @brief Standard constructor.
		 */
		Singleton( void )
		{
			assert( !ms_Singleton );
			ms_Singleton = static_cast< T* >( this );
		}
		
		~Singleton( void )
		{  
			assert( ms_Singleton );  ms_Singleton = 0;  
		}
		
		/**
		 *       @brief Gets the singleton instance.
		 * @return The singleton instance.
		 */
		static T& getSingleton( void )
		{	
			assert( ms_Singleton );  
			return ( *ms_Singleton ); 
		}
		
		/**
		 *       @brief Gets a pointer to the singleton instance.
		 * @return A pointer to the singleton instance.
		 */
		static T* getSingletonPtr( void )
		{ 
			assert( ms_Singleton ); 
			return ms_Singleton; 
		}
		
		/**
		 *       @brief Gets the singleton instance.
		 * @return The singleton instance.
		 */
		static T& instance( void )
		{
			return getSingleton();
		}
		
		/**
		 *       @brief Returns true if there's a singleton registered with the system.
		 * @return True if there's a singleton available.
		 */
		static bool hasInstance()
		{
			return ms_Singleton != 0;
		}
	};
}
#endif

