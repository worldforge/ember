/*
 Copyright (C) 2012 Arjun Kumar <arjun1991@gmail.com>
 Copyright (C) 2012 Erik Ogenvik

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef GRAPHICALCHANGEADAPTER_H_
#define GRAPHICALCHANGEADAPTER_H_
#include <sigc++/signal.h>

namespace Ember
{
namespace OgreView
{

/**
 * @brief This structure is used to accumulate the return values from the slots and then pass a result back to the signal
 * It receives all the return values from all slots registered to this signal and passes on a calculated OR of all the values back to the signal.
 * It is used to calculate whether a further change in graphics level is possible.
 */
template<class T>
struct FurtherChangePossibleAccumulater
{
	/**
	 * The data type of the value returned to the signal. Required by sigc++
	 */
	typedef T result_type;

	/**
	 * An templated iterator to traverse through all the returns from the slots
	 */
	template<class T_iterator>

	/**
	 * @brief The overloaded () function is called by sigc++ to calculate the value to be returned to the signal from the return values of the slots.
	 * @return The value to be passed back to the signal
	 */
	result_type operator()(T_iterator first, T_iterator last) const
	{
		result_type endResult = false;
		for (; first != last; ++first) {
			endResult = endResult || *first;
		}
		return endResult;
	}
};
/**
 * @brief Adaptor interface class between the central AutomaticGraphicsLevelManager class and the graphics subsystems
 * This class accepts a change in fps required and translates it into a floating change required value that the subsystems understand
 */
class GraphicalChangeAdapter
{
public:

	/**
	 * Signals that a change is required.
	 * @param changeSize The change required in fps. A positive value means that graphical details should be improved. A negative value means that the details should be decreased.
	 * @return True if further change can be performed.
	 */
	bool fpsChangeRequired(float);

	sigc::signal<bool(float)>::accumulated<FurtherChangePossibleAccumulater<bool> > EventChangeRequired;
};

}
}
#endif /* GRAPHICALCHANGEADAPTER_H_ */
