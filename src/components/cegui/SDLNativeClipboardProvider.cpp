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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "SDLNativeClipboardProvider.h"
#include <SDL.h>

namespace Ember {
namespace Cegui {

SDLNativeClipboardProvider::SDLNativeClipboardProvider()
        : m_buffer(nullptr) {

}

SDLNativeClipboardProvider::~SDLNativeClipboardProvider() {
    SDL_free(m_buffer);
}

void SDLNativeClipboardProvider::sendToClipboard(const CEGUI::String& mimeType, void* buffer, size_t) {

    if (mimeType == "text/plain") {
        SDL_SetClipboardText((char*) buffer);
    }
}

void SDLNativeClipboardProvider::retrieveFromClipboard(CEGUI::String& mimeType, void*& buffer, size_t& size) {

    if (SDL_HasClipboardText() == SDL_TRUE) {
        mimeType = "text/plain";
        SDL_free(m_buffer);

        m_buffer = SDL_GetClipboardText();
        buffer = m_buffer;
        size = strnlen(m_buffer, 1000000);
    }
}

}
}
