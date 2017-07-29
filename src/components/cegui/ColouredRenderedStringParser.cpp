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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "ColouredRenderedStringParser.h"
#include "RenderedColourStringTextComponent.h"

namespace Ember
{
namespace Cegui
{

void ColouredRenderedStringParser::appendRenderedText(CEGUI::RenderedString& rs, const CEGUI::String& text) const
{
	size_t cpos = 0;
	// split the given string into lines based upon the newline character
	while (text.length() > cpos) {
		// find next newline
		const size_t nlpos = text.find('\n', cpos);
		// calculate length of this substring
		const size_t len = ((nlpos != CEGUI::String::npos) ? nlpos : text.length()) - cpos;

		// construct new text component and append it.
		if (len > 0) {
			//If we're using colours different from those of the default colours we'll also use our own implementation which doesn't do modulation.
			if (d_initialColours.d_bottom_left != d_colours.d_bottom_left || d_initialColours.d_top_left != d_colours.d_top_left || d_initialColours.d_top_right != d_colours.d_top_right || d_initialColours.d_bottom_right != d_colours.d_bottom_right) {
				RenderedColourStringTextComponent rtc(text.substr(cpos, len), d_fontName);
				rtc.setPadding(d_padding);
				rtc.setColours(d_colours);
				rtc.setVerticalFormatting(d_vertAlignment);
				rtc.setAspectLock(d_aspectLock);
				rs.appendComponent(rtc);
			} else {
				CEGUI::RenderedStringTextComponent rtc(text.substr(cpos, len), d_fontName);
				rtc.setPadding(d_padding);
				rtc.setColours(d_colours);
				rtc.setVerticalFormatting(d_vertAlignment);
				rtc.setAspectLock(d_aspectLock);
				rs.appendComponent(rtc);
			}
		}

		// break line if needed
		if (nlpos != CEGUI::String::npos)
			rs.appendLineBreak();

		// advance current position.  +1 to skip the \n char
		cpos += len + 1;
	}
}
}
}
