/***************************************************************************
 *   Copyright (C) 2004 - 2006 Paul D Turner & The CEGUI Development Team
 *   Copyright (C) 2013 Erik Ogenvik
 *
 *   Permission is hereby granted, free of charge, to any person obtaining
 *   a copy of this software and associated documentation files (the
 *   "Software"), to deal in the Software without restriction, including
 *   without limitation the rights to use, copy, modify, merge, publish,
 *   distribute, sublicense, and/or sell copies of the Software, and to
 *   permit persons to whom the Software is furnished to do so, subject to
 *   the following conditions:
 *
 *   The above copyright notice and this permission notice shall be
 *   included in all copies or substantial portions of the Software.
 *
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *   EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 *   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 *   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 *   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *   OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/
/**
 * This code is lifted and altered from the CEGUIWindowManager.cpp file from the CEGUI project.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "LayoutLoader.h"
#include "CEGUIGUILayout_xmlHandler.h"
#include <CEGUIExceptions.h>
#include <CEGUIXMLParser.h>

using namespace CEGUI;
namespace Ember
{
namespace Cegui
{

Window* LayoutLoader::loadWindowLayout(const String& filename, const String& name_prefix, const String& resourceGroup, WindowManager::PropertyCallback* callback, void* userdata)
{
	if (filename.empty()) {
		CEGUI_THROW(InvalidRequestException("WindowManager::loadWindowLayout - Filename supplied for gui-layout loading must be valid."));
	}

	// log the fact we are about to load a layout
	Logger::getSingleton().logEvent("---- Beginning loading of GUI layout from '" + filename + "' ----", Informative);

	// create handler object
	Ember::Cegui::GUILayout_xmlHandler handler(name_prefix, callback, userdata);

	// do parse (which uses handler to create actual data)
	CEGUI_TRY
	{
		System::getSingleton().getXMLParser()->parseXMLFile(handler,
				filename, "GUILayout.xsd", resourceGroup.empty() ? WindowManager::getDefaultResourceGroup() : resourceGroup);
	}
	CEGUI_CATCH(...)
	{
		Logger::getSingleton().logEvent("WindowManager::loadWindowLayout - loading of layout from file '" + filename +"' failed.", Errors);
		CEGUI_RETHROW;
	}

	// log the completion of loading
	Logger::getSingleton().logEvent("---- Successfully completed loading of GUI layout from '" + filename + "' ----", Standard);

	return handler.getLayoutRootWindow();
}

}
}
