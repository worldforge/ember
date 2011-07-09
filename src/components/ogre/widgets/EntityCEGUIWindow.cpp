/*
 Copyright (C) 2011 Erik Hjortsberg

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
#include "EntityCEGUIWindow.h"
#include "../SimpleRenderContext.h"

#include <CEGUIWindow.h>
#include <CEGUIRenderingSurface.h>
#include <CEGUISystem.h>
#include <CEGUIRenderer.h>
#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <RendererModules/Ogre/CEGUIOgreTextureTarget.h>
#include <RendererModules/Ogre/CEGUIOgreTexture.h>
#include <OgreTexture.h>
#include <OgreRenderTexture.h>

namespace Ember
{
namespace OgreView
{
namespace Gui
{

EntityCEGUIWindow::EntityCEGUIWindow(CEGUI::Window& window) :
		mWindow(window), mRenderContext(0)
{
	window.setUsingAutoRenderingSurface(true);
	CEGUI::RenderingSurface* surface = window.getRenderingSurface();

	if (surface) {
		CEGUI::OgreTextureTarget& ceguiOgreTextureTarget = dynamic_cast<CEGUI::OgreTextureTarget&>(surface->getRenderTarget());CEGUI::OgreTexture& ceguiOgreTexture = dynamic_cast<CEGUI::OgreTexture&>(ceguiOgreTextureTarget.getTexture());
		Ogre::TexturePtr ogreTexture = ceguiOgreTexture.getOgreTexture();

		mRenderContext = new SimpleRenderContext(window.getName().c_str(), ogreTexture);
	}
}

EntityCEGUIWindow::~EntityCEGUIWindow()
{
	delete mRenderContext;
}

SimpleRenderContext* EntityCEGUIWindow::getRenderContext()
{
	return mRenderContext;
}

CEGUI::Window& EntityCEGUIWindow::getWindow()
{
	return mWindow;
}

void EntityCEGUIWindow::update()
{
	if (mWindow.getParent()) {
		mWindow.getParent()->invalidate(false);
	}
	if (mRenderContext) {
		mRenderContext->getRenderTexture()->update();
	}
}

}
}
}
