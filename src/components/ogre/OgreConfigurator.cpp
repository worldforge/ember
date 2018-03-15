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
 along with this program; if not, write to the Free Software Foundation,
 Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
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

#include "framework/LoggingInstance.h"
#include "framework/TimeHelper.h"
#include "framework/MainLoopController.h"

#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <CEGUI/RendererModules/Ogre/ResourceProvider.h>
#include <CEGUI/CEGUI.h>


#include <OgreRoot.h>
#include <OgreRenderWindow.h>
#include <OgreWindowEventUtilities.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
//Needed for preventing resize of window
#include <X11/Xutil.h>
#endif

#include <thread>
#include <components/cegui/CEGUISetup.h>

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
	unsigned int width = 250;
	unsigned int height = 300;
	auto& root = Ogre::Root::getSingleton();

	auto renderSystem = root.getRenderSystem();


	root.initialise(false);

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
//We should use "externalWindowHandle" on Windows, and "parentWindowHandle" on Linux.
#ifdef _WIN32
	misc["externalWindowHandle"] = windowId;
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
	mLoader->loadBootstrap();
	mLoader->loadGui();

	Cegui::CEGUILogger* logger = new Cegui::CEGUILogger();

	CEGUI::OgreRenderer& renderer = Ember::Cegui::CEGUISetup::createRenderer(renderWindow);
	renderer.setRenderingEnabled(false);
	renderer.setFrameControlExecutionEnabled(false);
	CEGUI::OgreResourceProvider& rp = CEGUI::OgreRenderer::createOgreResourceProvider();

	CEGUI::System::create(renderer, &rp, nullptr, nullptr, nullptr, "cegui/datafiles/configs/ceguiMinimal.config");
	try {
		CEGUI::ImageManager::getSingleton().loadImageset("cegui/datafiles/imagesets/splash.imageset", "");

		CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultWindow", "root_wnd");
		CEGUI::System::getSingleton().getDefaultGUIContext().setRootWindow(sheet);

		mConfigWindow = CEGUI::WindowManager::getSingleton().loadLayoutFromFile("cegui/datafiles/layouts/OgreConfigurator.layout");
		mConfigWindow->update(0);
		sheet->addChild(mConfigWindow);

		updateResolutionList(renderSystem);
		CEGUI::Window* okButton = mConfigWindow->getChildRecursive("Button_ok");
		okButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber([&](){
			mResult = OC_OK;
			mContinueInLoop = false;
		}));
		//okButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonOkClicked, this));
		CEGUI::Window* cancelButton = mConfigWindow->getChildRecursive("Button_cancel");
		cancelButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonCancelClicked, this));

		auto fullscreenCheckbox = dynamic_cast<CEGUI::ToggleButton*>(mConfigWindow->getChildRecursive("Fullscreen"));
		auto dontShowAgainCheckbox = dynamic_cast<CEGUI::ToggleButton*>(mConfigWindow->getChildRecursive("DontShowAgain"));

		Input& input = Input::getSingleton();
		auto adapterDeleter = [&](IInputAdapter* ptr){
			input.removeAdapter(ptr);
			delete ptr;
		};
		std::unique_ptr<IInputAdapter, decltype(adapterDeleter)> adapter(new GUICEGUIAdapter(CEGUI::System::getSingletonPtr(), &renderer), adapterDeleter);
		input.addAdapter(adapter.get());

		Ogre::ConfigOptionMap configOptions = renderSystem->getConfigOptions();
		Ogre::ConfigOptionMap::const_iterator optionsIter = configOptions.find("Full Screen");
		if (optionsIter != configOptions.end()) {
			fullscreenCheckbox->setSelected(optionsIter->second.currentValue == "Yes");
		}

		input.setInputMode(Input::IM_GUI);

		long long lastTime = TimeHelper::currentTimeMillis();
		while (mContinueInLoop) {
			input.processInput();
			if (input.getMainLoopController()->shouldQuit()) {
				break;
			}
			float timeElapsed = (TimeHelper::currentTimeMillis() - lastTime) / 1000.0f;
			CEGUI::System::getSingleton().injectTimePulse(timeElapsed);
			CEGUI::System::getSingleton().getDefaultGUIContext().injectTimePulse(timeElapsed);
			lastTime = TimeHelper::currentTimeMillis();

			CEGUI::System::getSingleton().renderAllGUIContexts();
			renderWindow->swapBuffers();


			// We'll use a smooth 60 fps to provide a good impression
			std::this_thread::sleep_for(std::chrono::milliseconds(16));
		}

		mConfigOptions = renderSystem->getConfigOptions();

		CEGUI::Combobox* resolutionsCombobox = dynamic_cast<CEGUI::Combobox*>(mConfigWindow->getChildRecursive("Resolution"));
		mConfigOptions["Video Mode"].currentValue = resolutionsCombobox->getText().c_str();
		mConfigOptions["Full Screen"].currentValue = fullscreenCheckbox->isSelected() ? "Yes" : "No";

		if (mResult != OC_CANCEL && dontShowAgainCheckbox->isSelected()) {
			EmberServices::getSingleton().getConfigService().setValue("ogre", "suppressconfigdialog", true);
		}



		CEGUI::System::getSingleton().destroy();
		CEGUI::OgreRenderer::destroy(renderer);
		CEGUI::OgreRenderer::destroyOgreResourceProvider(rp);
		delete logger;

		Ogre::Root::getSingleton().destroyRenderTarget(renderWindow);

	} catch (const std::exception& ex) {
		CEGUI::System::getSingleton().destroy();
		CEGUI::OgreRenderer::destroy(renderer);
		CEGUI::OgreRenderer::destroyOgreResourceProvider(rp);
		delete logger;
		throw ex;
	}
	return mResult;
}

Ogre::ConfigOptionMap OgreConfigurator::getConfigOptions() const
{
	return mConfigOptions;
}

void OgreConfigurator::updateResolutionList(Ogre::RenderSystem* renderSystem)
{
	assert(renderSystem);

	Ogre::ConfigOptionMap configOptions = renderSystem->getConfigOptions();
	bool resolutionFoundInOptions = false;
	CEGUI::Combobox* resolutionsCombobox = dynamic_cast<CEGUI::Combobox*>(mConfigWindow->getChildRecursive("Resolution"));
	resolutionsCombobox->resetList();
	auto optionsIter = configOptions.find("Video Mode");
	if (optionsIter != configOptions.end()) {
		const Ogre::StringVector& possibleResolutions = optionsIter->second.possibleValues;
		for (const auto& possibleResolution : possibleResolutions) {
			std::string resolution = possibleResolution;
			//Trim away extra spaces which Ogre seem to generate
			resolution = Ogre::StringUtil::replaceAll(resolution, "  ", " ");
			Ogre::StringUtil::trim(resolution, true, true);
			Gui::ColouredListItem* item = new Gui::ColouredListItem(resolution);
			resolutionsCombobox->addItem(item);
			if (possibleResolution == optionsIter->second.currentValue) {
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

}
}
