/*
 Copyright (C) 2011 Erik Ogenvik

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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "RenderedColourStringTextComponent.h"

#include <CEGUIRenderedStringTextComponent.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

RenderedColourStringTextComponent::RenderedColourStringTextComponent(const CEGUI::String& text, const CEGUI::String& font_name) :
		CEGUI::RenderedStringTextComponent(text, font_name)
{
}

void RenderedColourStringTextComponent::draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Vector2& position, const CEGUI::ColourRect* mod_colours, const CEGUI::Rect* clip_rect, const float vertical_space, const float space_extra) const
{
	CEGUI::RenderedStringTextComponent::draw(buffer, position, 0, clip_rect, vertical_space, space_extra);
}

CEGUI::RenderedStringTextComponent* RenderedColourStringTextComponent::clone() const
{
	return new RenderedColourStringTextComponent(*this);
}
}
}
}
