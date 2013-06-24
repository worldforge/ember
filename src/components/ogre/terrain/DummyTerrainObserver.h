/*
 Copyright (C) 2013 Samuel Kogler

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

#ifndef DUMMYTERRAINOBSERVER_H_
#define DUMMYTERRAINOBSERVER_H_

#include <sigc++/signal.h>

#include "ITerrainObserver.h"



namespace Ember
{
namespace OgreView
{
namespace Terrain
{

/**
 * @brief 
 */
class DummyTerrainObserver: public Ember::OgreView::Terrain::ITerrainObserver
{
public:
	DummyTerrainObserver();
	virtual ~DummyTerrainObserver();

	virtual void observeArea(const WFMath::AxisBox<2>& area);

	virtual void observeArea(const Ogre::TRect<int>& area);

	sigc::signal<void> EventAreaShown;

};

} /* namespace Terrain */
} /* namespace OgreView */
} /* namespace Ember */
#endif /* DUMMYTERRAINOBSERVER_H_ */
