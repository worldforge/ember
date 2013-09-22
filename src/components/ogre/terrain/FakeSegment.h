/*
 Copyright (C) 2013 Erik Ogenvik

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

#ifndef EMBER_OGREVIEW_TERRAIN_FAKESEGMENT_H_
#define EMBER_OGREVIEW_TERRAIN_FAKESEGMENT_H_

#include "Segment.h"

#include <functional>

namespace Ember
{
namespace OgreView
{
namespace Terrain
{

/**
 * @brief Works as a standard Segment, but since it's "faked" it will be destroyed when invalidated.
 *
 * "Fake" means that it doesn't exist on the server, and instead is a client only structure.
 */
class FakeSegment : public Segment
{
public:
	/**
	 * @brief Ctor.
	 * @param segment The Mercator segment which this instance wraps around.
	 */
	FakeSegment(Mercator::Segment& segment);
	virtual ~FakeSegment();

	virtual void invalidate();

	/**
	 * @brief Sets an validator function which will be called when invalidate() is called.
	 *
	 * @param invalidator An invalidation function.
	 */
	void setInvalidator(std::function<void()>& invalidator);

private:

	/**
	 * @brief Handles invalidation of the data.
	 */
	std::function<void()> mInvalidator;
};

}
}
}
#endif /* EMBER_OGREVIEW_TERRAIN_FAKESEGMENT_H_ */
