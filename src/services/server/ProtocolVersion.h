/*
 Copyright (C) 2020 Erik Ogenvik

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

#ifndef EMBER_PROTOCOLVERSION_H
#define EMBER_PROTOCOLVERSION_H
namespace Ember {
/**
 * The protocol version we're supporting. This should match what's in Cyphesis.
 * If Cyphesis uses a more future protocol version a warning should be shown to the user, since it means that some features won't work.
 */
constexpr const long protocolVersion = 20200601;
}

#endif //EMBER_PROTOCOLVERSION_H
