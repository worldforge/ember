/*
 Copyright (C) 2022 Erik Ogenvik

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
#include "RegisterLua.h"
#include "components/ogre/environment/IEnvironmentProvider.h"
#include "components/ogre/environment/Environment.h"

using namespace Ember::OgreView::Environment;
using namespace Ember::Lua;

template <>
void registerLua<Environment>(sol::table& space) {
	auto iSun = space.new_usertype<ISun>("ISun", sol::no_constructor);
	iSun["setAmbientLight"] = &ISun::setAmbientLight;


	auto iFog = space.new_usertype<IFog>("IFog", sol::no_constructor);
	iFog["setDensity"] = &IFog::setDensity;
	iFog["getDensity"] = &IFog::getDensity;

	auto environment = space.new_usertype<Ember::OgreView::Environment::Environment>("Environment", sol::no_constructor);
	environment["getSun"] = &Ember::OgreView::Environment::Environment::getSun;
	environment["getFog"] = &Ember::OgreView::Environment::Environment::getFog;
	environment["setTime"] = [](Ember::OgreView::Environment::Environment* self, int seconds) { self->setTime(seconds); };
	environment["setTimeMultiplier"] = &Ember::OgreView::Environment::Environment::setTimeMultiplier;
	environment["getTimeMultiplier"] = &Ember::OgreView::Environment::Environment::getTimeMultiplier;
	environment["setAmbientLight"] = &Ember::OgreView::Environment::Environment::setAmbientLight;

}