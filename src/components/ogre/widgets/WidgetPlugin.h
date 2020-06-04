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

#ifndef EMBER_WIDGETPLUGIN_H
#define EMBER_WIDGETPLUGIN_H

#include <boost/dll.hpp>

namespace Ember {
namespace OgreView {
class GUIManager;
}
}
/**
 * A typedef for the return value of a Plugin Function.
 */
typedef std::function<void()> WidgetPluginCallback;
/**
 * A typedef for a Plugin Function as delivered by the Boost.DLL system.
 */
typedef boost::dll::detail::library_function<WidgetPluginCallback(Ember::OgreView::GUIManager&)> WidgetPluginFunction;

#endif //EMBER_WIDGETPLUGIN_H
