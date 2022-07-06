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

using namespace Ogre;

template<>
void registerLua<RenderTarget>(sol::table& space) {
	auto RenderTarget = space["RenderTarget"].get_or_create<sol::table>();
	auto frameStats = RenderTarget.new_usertype<RenderTarget::FrameStats>("RenderTarget::FrameStats", sol::no_constructor);
	frameStats["lastFPS"] = &RenderTarget::FrameStats::lastFPS;
	frameStats["avgFPS"] = &RenderTarget::FrameStats::avgFPS;
	frameStats["bestFPS"] = &RenderTarget::FrameStats::bestFPS;
	frameStats["worstFPS"] = &RenderTarget::FrameStats::worstFPS;
	frameStats["bestFrameTime"] = &RenderTarget::FrameStats::bestFrameTime;
	frameStats["worstFrameTime"] = &RenderTarget::FrameStats::worstFrameTime;
	frameStats["triangleCount"] = &RenderTarget::FrameStats::triangleCount;
	frameStats["batchCount"] = &RenderTarget::FrameStats::batchCount;

}