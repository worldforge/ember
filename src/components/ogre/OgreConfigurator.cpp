/*
 Copyright (C) 2012 Erik Ogenvik

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

#include "OgreConfigurator.h"

#include "components/ogre/widgets/ColouredListItem.h"
#include "components/ogre/GUICEGUIAdapter.h"
#include "components/ogre/OgreResourceLoader.h"
#include "components/cegui/CEGUILogger.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/input/Input.h"

#include "framework/LoggingInstance.h"
#include "framework/Time.h"
#include "framework/MainLoopController.h"

#include <RendererModules/Ogre/CEGUIOgreRenderer.h>
#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#include <CEGUI.h>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUIListboxTextItem.h>

#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>
#include <OgreStringConverter.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
//Needed for preventing resize of window
#include <X11/Xutil.h>
#endif

#ifdef _MSC_VER
#include <SDL.h>
#include <SDL_syswm.h>
#include <SDL_keyboard.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
#include <SDL/SDL_keyboard.h>
#endif

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

namespace Ember
{
namespace OgreView
{

OgreConfigurator::OgreConfigurator() :
		mLastFrameTime(0), mResult(OC_CANCEL), mContinueInLoop(true), mLoader(new OgreResourceLoader()), mConfigWindow(0)
{
}

OgreConfigurator::~OgreConfigurator()
{
	delete mLoader;
}

OgreConfigurator::Result OgreConfigurator::configure()
{
	int width = 250;
	int height = 300;
	const Ogre::RenderSystemList& renderers = Ogre::Root::getSingleton().getAvailableRenderers();
	if (renderers.size() == 0) {
		return OC_CANCEL;
	}

	//Restore last used render system.
	Ogre::Root::getSingleton().restoreConfig();
	Ogre::RenderSystem* renderSystem = Ogre::Root::getSingleton().getRenderSystem();
	Ogre::Root::getSingleton().initialise(false);

	bool handleOpenGL = false;
#ifdef __APPLE__
	handleOpenGL = true;
#endif
#ifdef _WIN32
	//Only apply if we're using the OpenGL render plugin.
	if (renderSystem->getName() == "OpenGL Rendering Subsystem") {
		handleOpenGL = true;
	}
#endif

	const std::string windowId = Input::getSingleton().createWindow(width, height, false, false, true, handleOpenGL);

	Ogre::NameValuePairList misc;
#ifdef __APPLE__
	misc["currentGLContext"] = Ogre::String("true");
	misc["macAPI"] = Ogre::String("cocoa");
#else
#ifdef _WIN32
	SDL_SysWMinfo wmInfo;
	SDL_VERSION(&wmInfo.version);
	SDL_GetWMInfo(&wmInfo);

	size_t winHandle = reinterpret_cast<size_t>(wmInfo.window);
	size_t winGlContext = reinterpret_cast<size_t>(wmInfo.hglrc);

	misc["externalWindowHandle"] = Ogre::StringConverter::toString(winHandle);
	misc["externalGLContext"] = Ogre::StringConverter::toString(winGlContext);
	misc["externalGLControl"] = Ogre::String("True");
#else
	misc["parentWindowHandle"] = windowId;
#endif
#endif

	Ogre::RenderWindow* renderWindow = Ogre::Root::getSingleton().createRenderWindow("MainWindow", width, height, false, &misc);

	renderWindow->setVisible(true);
//On Windows we can't tell the window to resize, since that will lead to an infinite loop of resize events (probably stemming from how Windows lacks a proper window manager).
#ifndef _WIN32
	renderWindow->resize(width, height);
#endif
	renderWindow->windowMovedOrResized();

	mLoader->initialize();
	mLoader->loadSection("Bootstrap", false);
	mLoader->loadSection("Gui", false);
	mLoader->loadSection("General", false);

	Cegui::CEGUILogger* logger = new Cegui::CEGUILogger();

	CEGUI::OgreRenderer& renderer = CEGUI::OgreRenderer::create(*renderWindow);
	CEGUI::ResourceProvider& rp = CEGUI::OgreRenderer::createOgreResourceProvider();

	CEGUI::System::create(renderer, &rp);
	try {
		CEGUI::SchemeManager::getSingleton().create("cegui/datafiles/schemes/EmberLookSkinMinimal.scheme", "");
		CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-8");
		CEGUI::ImagesetManager::getSingleton().create("cegui/datafiles/imagesets/splash.imageset", "");
		CEGUI::System::getSingleton().setDefaultTooltip("EmberLook/Tooltip");

		CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultGUISheet", "root_wnd");
		CEGUI::System::getSingleton().setGUISheet(sheet);

		CEGUI::System::getSingleton().setDefaultMouseCursor("EmberLook", "MouseArrow");

		mConfigWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("cegui/datafiles/layouts/OgreConfigurator.layout", "OgreConfigure/");
		sheet->addChildWindow(mConfigWindow);

		CEGUI::Window* renderSystemWrapper = mConfigWindow->getChildRecursive("OgreConfigure/RenderSystem_wrapper");
		CEGUI::Combobox* renderSystemsBox = static_cast<CEGUI::Combobox*>(mConfigWindow->getChildRecursive("OgreConfigure/RenderSystem"));
		//If we only have one render system available we should hide the render system combobox.
		if (renderers.size() == 1) {
			renderSystemWrapper->getParent()->removeChildWindow(renderSystemWrapper);
		} else {
			int i = 0;
			for (Ogre::RenderSystemList::const_iterator I = renderers.begin(); I != renderers.end(); ++I) {
				const Ogre::String& rendererName = (*I)->getName();
				Gui::ColouredListItem* item = new Gui::ColouredListItem(rendererName, i++);
				renderSystemsBox->addItem(item);
				if (rendererName == renderSystem->getName()) {
					renderSystemsBox->setItemSelectState(item, true);
				}
			}

			renderSystemsBox->subscribeEvent(CEGUI::Combobox::EventListSelectionAccepted, CEGUI::Event::Subscriber(&OgreConfigurator::renderSystemChanged, this));
		}
		updateResolutionList(renderSystem);
		CEGUI::Window* okButton = mConfigWindow->getChildRecursive("OgreConfigure/Button_ok");
		okButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonOkClicked, this));
		CEGUI::Window* cancelButton = mConfigWindow->getChildRecursive("OgreConfigure/Button_cancel");
		cancelButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonCancelClicked, this));
		CEGUI::Window* advancedButton = mConfigWindow->getChildRecursive("OgreConfigure/Advanced");
		advancedButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonAdvancedClicked, this));

		CEGUI::Checkbox* fullscreenCheckbox = static_cast<CEGUI::Checkbox*>(mConfigWindow->getChildRecursive("OgreConfigure/Fullscreen"));
		CEGUI::Checkbox* dontShowAgainCheckbox = static_cast<CEGUI::Checkbox*>(mConfigWindow->getChildRecursive("OgreConfigure/DontShowAgain"));

		IInputAdapter* adapter = new GUICEGUIAdapter(CEGUI::System::getSingletonPtr(), &renderer);
		Input::getSingleton().addAdapter(adapter);

		Ogre::ConfigOptionMap configOptions = renderSystem->getConfigOptions();
		Ogre::ConfigOptionMap::const_iterator optionsIter = configOptions.find("Full Screen");
		if (optionsIter != configOptions.end()) {
			fullscreenCheckbox->setSelected(optionsIter->second.currentValue == "Yes");
		}

		Input& input = Input::getSingleton();
		input.setInputMode(Input::IM_GUI);

		long long lastTime = Time::currentTimeMillis();
		while (mContinueInLoop) {
			input.processInput();
			if (input.getMainLoopController()->shouldQuit()) {
				break;
			}
			float timeElapsed = (Time::currentTimeMillis() - lastTime) / 1000.0f;
			CEGUI::System::getSingleton().injectTimePulse(timeElapsed);
			lastTime = Time::currentTimeMillis();
			Ogre::Root::getSingleton().renderOneFrame();
		}
		input.removeAdapter(adapter);

		//Check if the user has selected a render system (in the case of there being multiple)
		if (renderers.size() > 1) {
			renderSystem = renderers[renderSystemsBox->getSelectedItem()->getID()];
		}
		mChosenRenderSystemName = renderSystem->getName();

		mConfigOptions = renderSystem->getConfigOptions();

		CEGUI::Combobox* resolutionsCombobox = static_cast<CEGUI::Combobox*>(mConfigWindow->getChildRecursive("OgreConfigure/Resolution"));
		mConfigOptions["Video Mode"].currentValue = resolutionsCombobox->getText().c_str();
		mConfigOptions["Full Screen"].currentValue = fullscreenCheckbox->isSelected() ? "Yes" : "No";

		if (mResult != OC_CANCEL && dontShowAgainCheckbox->isSelected()) {
			EmberServices::getSingleton().getConfigService().setValue("ogre", "suppressconfigdialog", true);
		}

		CEGUI::System::getSingleton().destroy();
		delete logger;

		Ogre::Root::getSingleton().destroyRenderTarget(renderWindow);

	} catch (const std::exception& ex) {
		CEGUI::System::getSingleton().destroy();
		delete logger;
		throw ex;
	}
	return mResult;
}

std::string OgreConfigurator::getChosenRenderSystemName() const
{
	return mChosenRenderSystemName;
}

Ogre::ConfigOptionMap OgreConfigurator::getConfigOptions() const
{
	return mConfigOptions;
}
bool OgreConfigurator::renderSystemChanged(const CEGUI::EventArgs& args)
{
	CEGUI::Window* wnd = mConfigWindow->getChildRecursive("OgreConfigure/RenderSystem");
	assert(wnd);
	CEGUI::Combobox* cmbRenderers = static_cast<CEGUI::Combobox*>(wnd);
	CEGUI::ListboxItem* item = cmbRenderers->getSelectedItem();
	assert(item);
	Ogre::RenderSystem* renderSystem = Ogre::Root::getSingleton().getRenderSystemByName(item->getText().c_str());
	updateResolutionList(renderSystem);
	return true;
}
void OgreConfigurator::updateResolutionList(Ogre::RenderSystem* renderSystem)
{
	assert(renderSystem);

	Ogre::ConfigOptionMap configOptions = renderSystem->getConfigOptions();
	bool resolutionFoundInOptions = false;
	CEGUI::Combobox* resolutionsCombobox = static_cast<CEGUI::Combobox*>(mConfigWindow->getChildRecursive("OgreConfigure/Resolution"));
	resolutionsCombobox->resetList();
	Ogre::ConfigOptionMap::const_iterator optionsIter = configOptions.find("Video Mode");
	if (optionsIter != configOptions.end()) {
		const Ogre::StringVector& possibleResolutions = optionsIter->second.possibleValues;
		for (Ogre::StringVector::const_iterator I = possibleResolutions.begin(); I != possibleResolutions.end(); ++I) {
			std::string resolution = *I;
			//Trim away extra spaces which Ogre seem to generate
			resolution = Ogre::StringUtil::replaceAll(resolution, "  ", " ");
			Ogre::StringUtil::trim(resolution, true, true);
			Gui::ColouredListItem* item = new Gui::ColouredListItem(resolution);
			resolutionsCombobox->addItem(item);
			if (*I == optionsIter->second.currentValue) {
				resolutionsCombobox->setItemSelectState(item, true);
				resolutionFoundInOptions = true;
			}
		}
		if (!resolutionFoundInOptions) {
			resolutionsCombobox->setText(optionsIter->second.currentValue);
		}
	}
}
bool OgreConfigurator::buttonOkClicked(const CEGUI::EventArgs& args)
{
	mResult = OC_OK;
	mContinueInLoop = false;
	return true;
}

bool OgreConfigurator::buttonCancelClicked(const CEGUI::EventArgs& args)
{
	mResult = OC_CANCEL;
	mContinueInLoop = false;
	return true;
}

bool OgreConfigurator::buttonAdvancedClicked(const CEGUI::EventArgs& args)
{
	mResult = OC_ADVANCED_OPTIONS;
	mContinueInLoop = false;
	return true;
}

}
}
