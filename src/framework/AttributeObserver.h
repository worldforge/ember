//
// C++ Interface: AttributeObserver
//
// Description: 
//
//
// Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2007
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.//
//
#ifndef EMBEROGREATTRIBUTEOBSERVER_H
#define EMBEROGREATTRIBUTEOBSERVER_H

#include <memory>
#include <vector>
#include <string>
#include <sigc++/signal.h>

namespace Atlas {
namespace Message {
class Element;
}
}

namespace Eris {
class Entity;
}

namespace Ember {

class DeepAttributeObserver;
class DirectAttributeObserver;
/**
	@brief Observes changes to a specific attribute and emits a signal.
	
	Eris::Entity already has functionality for easily listening to attribute changes through the Eris::Entity::observe method, but this method is hard to use from a scripting environment (such as lua), thus the need for this class.
	In addition this class also allows you to listen to changed to nested attributes instead of only top level ones.
	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>
*/
class AttributeObserver
{
public:

	/**
	 * @brief Ctor. The entity to watch and the name of the attribute are supplied as parameters.
	 * @param entity The entity to watch.
	 * @param attributeName The name of the attribute to watch.
	 */
	AttributeObserver(Eris::Entity& entity, const std::string& attributeName);

	/**
	 * @brief Ctor. The entity to watch and the name of the attribute are supplied as parameters.
	 * @param entity The entity to watch.
	 * @param attributePath The path to the attribute.
	 */
	AttributeObserver(Eris::Entity& entity, const std::vector<std::string>& attributePath);

	/**
	 * @brief Ctor. The entity to watch and the name of the attribute are supplied as parameters.
	 * @param entity The entity to watch.
	 * @param attributePath The path to the attribute expressed as a delimited string.
	 * @param delimitor The delimitor to use for separation in the path.
	 */
	AttributeObserver(Eris::Entity& entity, const std::string& attributePath, const std::string& delimitor);

	/**
	 *    Dtor.
	 */
	~AttributeObserver();
	
	/**
	@brief Emitted when the attribute which is watched changes.
	The value passed is the new element for the named attribute.
	*/
	sigc::signal<void, const Atlas::Message::Element&> EventChanged;
	
	/**
	 * @brief Forces an evaluation of the current value and a possible emittance of the EventChanged signal.
	 */
	void forceEvaluation();

protected:

	std::auto_ptr<DeepAttributeObserver> mDeepAttributeObserver;

	std::auto_ptr<DirectAttributeObserver> mDirectAttributeObserver;

};

}

#endif
