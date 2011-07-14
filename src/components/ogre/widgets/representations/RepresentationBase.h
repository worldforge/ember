//
// C++ Interface: RepresentationBase
//
// Description: 
//
// Author: Martin Preisler <preisler.m@gmail.com>
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
#ifndef EMBEROGRE_GUI_REPRESENTATIONS_REPRESENTATIONBASE_H
#define EMBEROGRE_GUI_REPRESENTATIONS_REPRESENTATIONBASE_H

#include "framework/LoggingInstance.h"
#include <CEGUIEvent.h>
#include <Atlas/Message/Element.h>
#include <sigc++/signal.h>

namespace Ember {
namespace OgreView {

namespace Gui {

namespace Representations {

/**
 * @brief a visual (CEGUI widget) representation of data (using adapters)
 * 
 * Adapters simply connect widgets to native data but don't create the layouts
 * themselves. This class creates the layout and respective adapters so you
 * can use it standalone.
 */
template<typename ValueType>
class RepresentationBase
{
public:
	/**
	 * @brief Ctor
	 * 
	 * @param value value we want to represent and edit (will be directly altered)
	 */
	RepresentationBase(ValueType& value);
	
	/**
	 * @brief Dtor
	 */
	virtual ~RepresentationBase();
	
	/**
	 * @brief applies changes to the original value (changes the value you have passed)
	 * 
	 * This should be called when user pressed Apply/OK in the form to confirm the changes
	 * @internal This is virtual so that you can override and react to the change (example: resolution change)
	 */
	virtual void applyChanges();
	
	/**
	 * @brief gets rid of all the changes and sets the edited value to original value
	 */
	virtual void resetToOriginal() = 0;
	
	/**
	 * @brief retrieves the GUI layout root
	 * 
	 * This is useful for adding the representation to your own GUI layouts
	 */
	virtual CEGUI::Window* getGuiRoot() = 0;
	
	/**
	 * @brief retrieves the value user is editing
	 */
	virtual const ValueType& getEditedValue() const = 0;
	
	/**
	 * @brief retrieves the original value (at the time of representation construction)
	 * 
	 * @copydoc AdapterBase::getOriginalValue
	 */
	virtual const ValueType& getOriginalValue() const = 0;
	
	/**
	 * @brief checks whether this representation has changes
	 * 
	 * @copydoc AdapterBase::hasChanges
	 */
	virtual bool hasChanges() const = 0;
	
	/**
	 * @brief checks whether this represents something that has been removed
	 * 
	 * @copydoc AdapterBase::isRemoved
	 */
	virtual bool isRemoved() const = 0;
	
	/**
	 * @brief adds a suggested value
	 * 
	 * @copydoc AdapterBase::addSuggestion
	 */
	virtual void addSuggestion(const std::string& suggestion) = 0;
	
protected:
	/// this is the source and target value (original value that we change when the representation is edited)
	ValueType& mValue;
};

template<typename ValueType>
RepresentationBase<ValueType>::RepresentationBase(ValueType& value):
	mValue(value)
{}

template<typename ValueType>
RepresentationBase<ValueType>::~RepresentationBase()
{}

template<typename ValueType>
void RepresentationBase<ValueType>::applyChanges()
{
	mValue = getEditedValue();
	// FIXME: AdapterBase should have it's mOriginalValue changed at this point!
}

}

}

}

}

#endif
