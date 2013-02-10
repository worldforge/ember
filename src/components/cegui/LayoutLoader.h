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
 * This code is lifted and altered from the CEGUIWindowManager.h file from the CEGUI project.
 */

#ifndef LAYOUTLOADER_H_
#define LAYOUTLOADER_H_

#include <CEGUIWindowManager.h>

namespace Ember
{
namespace Cegui
{

/**
 * @brief Loads CEGUI layout from layout files.
 *
 * This behaves just as the method found in CEGUI::WindowManager::loadWindowLayout, with the main exception that we'll use our
 * patched version of the XML layout handler.
 */
class LayoutLoader
{
public:
	static CEGUI::Window* loadWindowLayout(const CEGUI::String& filename, const CEGUI::String& name_prefix = "", const CEGUI::String& resourceGroup = "", CEGUI::WindowManager::PropertyCallback* callback = 0, void* userdata = 0);

};

}
}
#endif /* LAYOUTLOADER_H_ */
