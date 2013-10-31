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

#ifndef SCRAPNATIVECLIPBOARDPROVIDER_H_
#define SCRAPNATIVECLIPBOARDPROVIDER_H_

#include <CEGUI/Clipboard.h>

namespace Ember
{
namespace Cegui
{

/**
 * @brief A native clipboard provider which will interface with Input.
 *
 * This allows the CEGUI clipboard to interact with the native one, through the
 * Input service.
 */
class ScrapNativeClipboardProvider: public CEGUI::NativeClipboardProvider
{
public:
	virtual ~ScrapNativeClipboardProvider();

	void sendToClipboard(const CEGUI::String &mimeType, void *buffer, size_t size);
	void retrieveFromClipboard(CEGUI::String &mimeType, void *&buffer, size_t &size);
};

}
}
#endif /* SCRAPNATIVECLIPBOARDPROVIDER_H_ */
