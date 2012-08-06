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
#include "components/ogre/EmberIcon.h"
#include "components/ogre/gui/CEGUILogger.h"

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

#ifdef _MSC_VER
#include <SDL.h>
#include <SDL_syswm.h>
#else
#include <SDL/SDL.h>
#include <SDL/SDL_syswm.h>
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
		mLastFrameTime(0), mResult(OC_CANCEL), mContinueInLoop(true), mLoader(new OgreResourceLoader())
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
	Ogre::RenderSystem* renderSystem = *renderers.begin();
	Ogre::Root::getSingleton().setRenderSystem(renderSystem);
	Ogre::Root::getSingleton().initialise(false);

	//set the icon of the window
	Uint32 rmask, gmask, bmask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	rmask = 0xff000000;
	gmask = 0x00ff0000;
	bmask = 0x0000ff00;
#else
	rmask = 0x000000ff;
	gmask = 0x0000ff00;
	bmask = 0x00ff0000;
#endif

	Ogre::NameValuePairList misc;

	Ogre::RenderWindow* renderWindow = Ogre::Root::getSingleton().createRenderWindow("MainWindow", width, height, false, &misc);
	renderWindow->setActive(true);
	renderWindow->setAutoUpdated(true);
	renderWindow->setVisible(true);

	mLoader->initialize();
	mLoader->loadSection("Bootstrap", false);
	mLoader->loadSection("Gui", false);
	mLoader->loadSection("General", false);

	Gui::CEGUILogger* logger = new Gui::CEGUILogger();

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

		CEGUI::Window* configWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("cegui/datafiles/layouts/OgreConfigurator.layout", "OgreConfigure/");
		sheet->addChildWindow(configWindow);

		CEGUI::Window* renderSystemWrapper = configWindow->getChildRecursive("OgreConfigure/RenderSystem_wrapper");
		CEGUI::Combobox* renderSystemsBox = static_cast<CEGUI::Combobox*>(configWindow->getChildRecursive("OgreConfigure/RenderSystem"));
		//If we only have one render system available we should hide the render system combobox.
		if (renderers.size() == 1) {
			renderSystemWrapper->getParent()->removeChildWindow(renderSystemWrapper);
		} else {
			int i = 0;
			for (Ogre::RenderSystemList::const_iterator I = renderers.begin(); I != renderers.end(); ++I) {
				Gui::ColouredListItem* item = new Gui::ColouredListItem((*I)->getName(), i++);
				renderSystemsBox->addItem(item);
			}
			//Select the first one in the list
			renderSystemsBox->setItemSelectState((size_t)0, true);
		}

		CEGUI::Window* okButton = configWindow->getChildRecursive("OgreConfigure/Button_ok");
		okButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonOkClicked, this));
		CEGUI::Window* cancelButton = configWindow->getChildRecursive("OgreConfigure/Button_cancel");
		cancelButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonCancelClicked, this));
		CEGUI::Window* advancedButton = configWindow->getChildRecursive("OgreConfigure/Advanced");
		advancedButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonAdvancedClicked, this));

		CEGUI::Checkbox* fullscreenCheckbox = static_cast<CEGUI::Checkbox*>(configWindow->getChildRecursive("OgreConfigure/Fullscreen"));
		CEGUI::Checkbox* dontShowAgainCheckbox = static_cast<CEGUI::Checkbox*>(configWindow->getChildRecursive("OgreConfigure/DontShowAgain"));

		CEGUI::Combobox* resolutionsCombobox = static_cast<CEGUI::Combobox*>(configWindow->getChildRecursive("OgreConfigure/Resolution"));

		IInputAdapter* adapter = new GUICEGUIAdapter(CEGUI::System::getSingletonPtr(), &renderer);
		Input::getSingleton().addAdapter(adapter);

		Ogre::ConfigOptionMap configOptions = renderSystem->getConfigOptions();

		bool resolutionFoundInOptions = false;
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

		optionsIter = configOptions.find("Full Screen");
		if (optionsIter != configOptions.end()) {
			fullscreenCheckbox->setSelected(optionsIter->second.currentValue == "Yes");
		}

		Input& input = Input::getSingleton();
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
		Input::getSingleton().removeAdapter(adapter);

		//Check if the user has selected a render system (in the case of there being multiple)
		if (renderers.size() > 1) {
			renderSystem = renderers[renderSystemsBox->getSelectedItem()->getID()];
		}
		mChosenRenderSystemName = renderSystem->getName();

		mConfigOptions = renderSystem->getConfigOptions();

		mConfigOptions["Video Mode"].currentValue = resolutionsCombobox->getText().c_str();
		mConfigOptions["Full Screen"].currentValue = fullscreenCheckbox->isSelected() ? "Yes" : "No";

		CEGUI::System::getSingleton().destroy();
		delete logger;

		Ogre::Root::getSingleton().destroyRenderTarget(renderWindow);

		if (dontShowAgainCheckbox->isSelected()) {
			EmberServices::getSingleton().getConfigService().setValue("ogre", "suppressconfigdialog", true);
		}

		return mResult;
	} catch (const std::exception& ex) {
		CEGUI::System::getSingleton().destroy();
		delete logger;
		throw ex;
	}
}

std::string OgreConfigurator::getChosenRenderSystemName() const
{
	return mChosenRenderSystemName;
}

Ogre::ConfigOptionMap OgreConfigurator::getConfigOptions() const
{
	return mConfigOptions;
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
