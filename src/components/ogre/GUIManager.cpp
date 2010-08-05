/*
 Copyright (C) 2004  Miguel Guzman (Aganor)
 Copyright (C) 2005  Erik Hjortsberg <erik.hjortsberg@gmail.com>

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
#include "GUIManager.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/scripting/ScriptingService.h"

#include <CEGUIWindowManager.h>
#include <CEGUISchemeManager.h>
#include <CEGUIExceptions.h>
#include <CEGUIFactoryModule.h>
#include <CEGUIBasicRenderedStringParser.h>
#include <CEGUIRenderedStringTextComponent.h>
#include <ScriptingModules/LuaScriptModule/CEGUILua.h>
#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#include <RendererModules/Ogre/CEGUIOgreImageCodec.h>
#include <elements/CEGUIPushButton.h>
#include <elements/CEGUIGUISheet.h>
#include <elements/CEGUIMultiLineEditbox.h>
#include <elements/CEGUIEditbox.h>

#include "widgets/Widget.h"
#include "MousePicker.h"

#include "components/ogre/camera/MainCamera.h"
#include "EmberOgre.h"
#include "services/input/Input.h"
#include "gui/ActiveWidgetHandler.h"
#include "gui/CEGUILogger.h"

#include "widgets/WidgetDefinitions.h"

#include "EmberEntity.h"

#include "framework/IScriptingProvider.h"

#include "components/lua/LuaScriptingProvider.h"

#include "GUICEGUIAdapter.h"

#include "widgets/icons/IconManager.h"
#include "widgets/EntityIconManager.h"

#include "services/input/Input.h"
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreTextureManager.h>
#include <OgreRoot.h>

#ifdef __WIN32__
#include <windows.h>
#include <direct.h>
#endif

template<> EmberOgre::GUIManager* Ember::Singleton<EmberOgre::GUIManager>::ms_Singleton = 0;

using namespace CEGUI;
using namespace EmberOgre::Gui;
using namespace Ember;

namespace EmberOgre
{

unsigned long GUIManager::msAutoGenId(0);

/**
 * @brief Overrides colour modulation of CEGUI::RenderedStringTextComponent.
 * The default implementation of CEGUI::RenderedStringTextComponent will always use the mod_colours to modulate the final colour.
 * Since we use black text colours this means that all our colours will end up black (i.e. it will multiply by zero, which results in zero).
 * This class overrides this behaviour to always disable the mod_colours.
 */
class RenderedColourStringTextComponent: public CEGUI::RenderedStringTextComponent
{
public:
	RenderedColourStringTextComponent(const String& text, const String& font_name) :
		CEGUI::RenderedStringTextComponent(text, font_name)
	{
	}

	void draw(CEGUI::GeometryBuffer& buffer, const CEGUI::Vector2& position, const CEGUI::ColourRect* mod_colours, const CEGUI::Rect* clip_rect, const float vertical_space, const float space_extra) const
	{
		CEGUI::RenderedStringTextComponent::draw(buffer, position, 0, clip_rect, vertical_space, space_extra);
	}

	RenderedStringTextComponent* clone() const
	{
		return new RenderedColourStringTextComponent(*this);
	}

};

/**
 * @brief Overrides the default behaviour of CEGUI::BasicRenderedStringParser to use colour modulation.
 * Since the CEGUI::RenderedStringTextComponent will use colour modulation and this will result in all colours being black if the modulation colour is black we need to use an instance of RenderedColourStringTextComponent whenever a colour different from the base colour is to be used.
 */
class ColouredRenderedStringParser: public CEGUI::BasicRenderedStringParser
{
protected:
	void appendRenderedText(CEGUI::RenderedString& rs, const CEGUI::String& text) const
	{
		size_t cpos = 0;
		// split the given string into lines based upon the newline character
		while (text.length() > cpos) {
			// find next newline
			const size_t nlpos = text.find('\n', cpos);
			// calculate length of this substring
			const size_t len = ((nlpos != String::npos) ? nlpos : text.length()) - cpos;

			// construct new text component and append it.
			if (len > 0) {
				//If we're using colours different from those of the default colours we'll also use our own implementation which doesn't do modulation.
				if (d_initialColours.d_bottom_left != d_colours.d_bottom_left || d_initialColours.d_top_left != d_colours.d_top_left || d_initialColours.d_top_right != d_colours.d_top_right || d_initialColours.d_bottom_right != d_colours.d_bottom_right) {
					RenderedColourStringTextComponent rtc(text.substr(cpos, len), d_fontName);
					rtc.setPadding(d_padding);
					rtc.setColours(d_colours);
					rtc.setVerticalFormatting(d_vertAlignment);
					rtc.setAspectLock(d_aspectLock);
					rs.appendComponent(rtc);
				} else {
					CEGUI::RenderedStringTextComponent rtc(text.substr(cpos, len), d_fontName);
					rtc.setPadding(d_padding);
					rtc.setColours(d_colours);
					rtc.setVerticalFormatting(d_vertAlignment);
					rtc.setAspectLock(d_aspectLock);
					rs.appendComponent(rtc);
				}
			}

			// break line if needed
			if (nlpos != String::npos)
				rs.appendLineBreak();

			// advance current position.  +1 to skip the \n char
			cpos += len + 1;
		}
	}
};

GUIManager::GUIManager(Ogre::RenderWindow* window) :
	ToggleInputMode("toggle_inputmode", this, "Toggle the input mode."), ReloadGui("reloadgui", this, "Reloads the gui."), ToggleGui("toggle_gui", this, "Toggle the gui display"), mGuiCommandMapper("gui", "key_bindings_gui"), mPicker(0), mSheet(0), mWindowManager(0), mDebugText(0), mWindow(window), mGuiSystem(0), mGuiRenderer(0), mLuaScriptModule(0), mIconManager(0), mActiveWidgetHandler(0), mCEGUILogger(new Gui::CEGUILogger()), mRenderedStringParser(0) ///by creating an instance here we'll indirectly tell CEGUI to use this one instead of trying to create one itself
{
	mGuiCommandMapper.restrictToInputMode(Input::IM_GUI);

	///we need this here just to force the linker to also link in the WidgetDefinitions
	WidgetDefinitions w;

	try {

		S_LOG_INFO("Starting CEGUI");
		mDefaultScheme = "EmberLook";
		S_LOG_VERBOSE("Setting default scheme to "<< mDefaultScheme);

		Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
		if (chdir(configSrv->getEmberDataDirectory().c_str())) {
			S_LOG_WARNING("Failed to change to the data directory. Gui loading might fail.");
		}

		///The OgreCEGUIRenderer is the main interface between Ogre and CEGUI. Note that the third argument tells the renderer to render the gui after all of the regular render queues have been processed, thus making sure that the gui always is on top.
		mGuiRenderer = &CEGUI::OgreRenderer::create(*window);
		CEGUI::ResourceProvider& resourceProvider = mGuiRenderer->createOgreResourceProvider();
		resourceProvider.setDefaultResourceGroup("Gui");

		CEGUI::ImageCodec& imageCodec = CEGUI::OgreRenderer::createOgreImageCodec();

		Ember::IScriptingProvider* provider = Ember::EmberServices::getSingleton().getScriptingService()->getProviderFor("LuaScriptingProvider");
		if (provider != 0) {
			Lua::LuaScriptingProvider* luaScriptProvider = static_cast<Lua::LuaScriptingProvider*>(provider);
			mLuaScriptModule = &LuaScriptModule::create(luaScriptProvider->getLuaState());
			if (luaScriptProvider->getErrorHandlingFunctionName().size() != 0) {
				mLuaScriptModule->setDefaultPCallErrorHandler(luaScriptProvider->getErrorHandlingFunctionName());
				mLuaScriptModule->executeString(""); ///We must call this to make CEGUI set up the error function internally. If we don't, CEGUI will never correctly set it up. The reason for this is that we never use the execute* methods in the CEGUI lua module later on, instead loading our scripts ourselves. And CEGUI is currently set up to require the execute* methods to be called in order for the error function to be registered.
			}
			mGuiSystem = &CEGUI::System::create(*mGuiRenderer, &resourceProvider, 0, &imageCodec, mLuaScriptModule, "cegui/datafiles/configs/cegui.config");

			Ember::EmberServices::getSingleton().getScriptingService()->EventStopping.connect(sigc::mem_fun(*this, &GUIManager::scriptingServiceStopping));
		} else {
			mGuiSystem = &CEGUI::System::create(*mGuiRenderer, &resourceProvider, 0, &imageCodec, 0, "cegui/datafiles/configs/cegui.config");
		}
		CEGUI::SchemeManager::SchemeIterator schemeI(SchemeManager::getSingleton().getIterator());
		if (schemeI.isAtEnd()) {
			// 			S_LOG_FAILURE("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
			throw Ember::Exception("Could not load any CEGUI schemes. This means that there's something wrong with how CEGUI is setup. Check the CEGUI log for more detail. We'll now exit Ember.");
		}

		mRenderedStringParser = new ColouredRenderedStringParser();
		mGuiSystem->setDefaultCustomRenderedStringParser(mRenderedStringParser);
		mWindowManager = &CEGUI::WindowManager::getSingleton();

		try {
			mGuiSystem->setDefaultMouseCursor(getDefaultScheme(), "MouseArrow");
		} catch (const CEGUI::Exception& ex) {
			S_LOG_FAILURE("CEGUI - could not set mouse pointer. Make sure that the correct scheme " << getDefaultScheme() << " is available. Message: " << ex.getMessage().c_str());
			throw Ember::Exception(ex.getMessage().c_str());
		}

		mSheet = mWindowManager->createWindow((CEGUI::utf8*)"DefaultGUISheet", (CEGUI::utf8*)"root_wnd");
		mGuiSystem->setGUISheet(mSheet);
		mSheet->activate();
		mSheet->moveToBack();
		mSheet->setDistributesCapturedInputs(false);

		BIND_CEGUI_EVENT(mSheet, CEGUI::ButtonBase::EventMouseButtonDown, GUIManager::mSheet_MouseButtonDown);
		BIND_CEGUI_EVENT(mSheet, CEGUI::Window::EventInputCaptureLost, GUIManager::mSheet_CaptureLost);
		BIND_CEGUI_EVENT(mSheet, CEGUI::ButtonBase::EventMouseDoubleClick, GUIManager::mSheet_MouseDoubleClick);

		///set a default tool tip
		CEGUI::System::getSingleton().setDefaultTooltip(getDefaultScheme() + "/Tooltip");

		S_LOG_INFO("CEGUI system set up");

		mPicker = new MousePicker();

		getInput().EventKeyPressed.connect(sigc::mem_fun(*this, &GUIManager::pressedKey));
		getInput().setInputMode(Input::IM_GUI);

		///add adapter for CEGUI, this will route input event to the gui
		mCEGUIAdapter = new GUICEGUIAdapter(mGuiSystem, mGuiRenderer);
		getInput().addAdapter(mCEGUIAdapter);

		mGuiCommandMapper.bindToInput(getInput());

		///connect to the creation of the avatar, since we want to switch to movement mode when that happens
		EmberOgre::getSingleton().EventCreatedAvatarEntity.connect(sigc::mem_fun(*this, &GUIManager::EmberOgre_CreatedAvatarEntity));

		mActiveWidgetHandler = new Gui::ActiveWidgetHandler(*this);

		Ogre::Root::getSingleton().addFrameListener(this);

	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("GUIManager - error when creating gui." << ex);
		throw Ember::Exception(ex.getMessage().c_str());

	}

}

GUIManager::~GUIManager()
{
	S_LOG_INFO("Shutting down GUI manager.");

	WidgetStore widgetStoreCopy(mWidgets);
	for (WidgetStore::iterator I = widgetStoreCopy.begin(); I != widgetStoreCopy.end(); ++I) {
		S_LOG_INFO("Deleting widget " << (*I)->getPrefix() << ".");
		delete *I;
	}

	delete mActiveWidgetHandler;
	delete mEntityIconManager;
	delete mIconManager;

	CEGUI::System::destroy();

	///note that we normally would delete the mCEGUILogger here, but we don't have to since mGuiSystem will do that in it's desctructor, even though it doesn't own the logger
	Ogre::Root::getSingleton().removeFrameListener(this);
	delete mCEGUIAdapter;

	delete mPicker;
	if (mGuiRenderer) {
		CEGUI::OgreRenderer::destroy(*mGuiRenderer);
	}
	if (mLuaScriptModule) {
		LuaScriptModule::destroy(*mLuaScriptModule);
	}
	delete mRenderedStringParser;
	//delete mMousePicker;
	//mMousePicker = 0;

	WidgetLoader::removeAllWidgetFactories();

}

void GUIManager::initialize()
{
	Ember::ConfigService* configSrv = Ember::EmberServices::getSingletonPtr()->getConfigService();
	chdir(configSrv->getEmberDataDirectory().c_str());
	try {
		mDebugText = (CEGUI::GUISheet*)mWindowManager->createWindow("DefaultGUISheet", (CEGUI::utf8*)"DebugText");
		mSheet->addChildWindow(mDebugText);
		mDebugText->setMaxSize(CEGUI::UVector2(UDim(1.0f, 0), UDim(0, 25)));
		mDebugText->setPosition(CEGUI::UVector2(UDim(0.0f, 0), UDim(1.0f, -25)));
		mDebugText->setSize(CEGUI::UVector2(UDim(1.0f, 0), UDim(0, 25)));

		/*		mDebugText->setFrameEnabled(false);
		 mDebugText->setBackgroundEnabled(false);*/
		//stxt->setHorizontalFormatting(StaticText::WordWrapCentred);


		//the console and quit widgets are not lua scripts, and should be loaded explicit
		// 		mConsoleWidget = static_cast<ConsoleWidget*>(createWidget("ConsoleWidget"));
		// 		if (!mConsoleWidget) {
		// 			throw Ember::Exception("Could not create console widget.");
		// 		}
		createWidget("Quit");
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when initializing widgets." << e);
		throw e;
	}
	try {
		mIconManager = new Gui::Icons::IconManager();
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating icon manager." << e);
	}

	try {
		mEntityIconManager = new Gui::EntityIconManager(*this);
	} catch (const std::exception& e) {
		S_LOG_FAILURE("GUIManager - error when creating entity icon manager." << e);
	}

	std::vector<std::string> widgetsToLoad;
	widgetsToLoad.push_back("IngameChatWidget");
	widgetsToLoad.push_back("MakeEntityWidget");
	//	widgetsToLoad.push_back("JesusEdit");
	widgetsToLoad.push_back("Help");
	widgetsToLoad.push_back("MeshPreview");

	///this should be defined in some kind of text file, which should be different depending on what game you're playing (like mason)
	try {
		///load the bootstrap script which will load all other scripts
		Ember::EmberServices::getSingleton().getScriptingService()->loadScript("lua/Bootstrap.lua");
	} catch (const std::exception& e) {
		S_LOG_FAILURE("Error when loading bootstrap script." << e);
	}

	for (std::vector<std::string>::iterator I = widgetsToLoad.begin(); I != widgetsToLoad.end(); ++I) {
		try {
			S_LOG_VERBOSE("Loading widget " << *I);
			createWidget(*I);
		} catch (const std::exception& e) {
			S_LOG_FAILURE("Error when initializing widget " << *I << "." << e);
		}
	}

}

void GUIManager::scriptingServiceStopping()
{
	mGuiSystem->setScriptingModule(0);
	if (mLuaScriptModule) {
		LuaScriptModule::destroy(*mLuaScriptModule);
	}
	mLuaScriptModule = 0;
}

void GUIManager::EmitEntityAction(const std::string& action, EmberEntity* entity)
{
	EventEntityAction.emit(action, entity);
}

CEGUI::Window* GUIManager::createWindow(const std::string& windowType)
{
	std::stringstream ss;
	ss << "_autoWindow_" << (msAutoGenId++);
	return createWindow(windowType, ss.str());
}

CEGUI::Window* GUIManager::createWindow(const std::string& windowType, const std::string& windowName)
{
	try {
		CEGUI::Window* window = mWindowManager->createWindow(windowType, windowName);
		return window;
	} catch (const CEGUI::Exception& ex) {
		S_LOG_FAILURE("Error when creating new window of type " << windowType << " with name " << windowName << ".\n" << ex.getMessage().c_str());
		return 0;
	} catch (const std::exception& ex) {
		S_LOG_FAILURE("Error when creating new window of type " << windowType << " with name " << windowName << "." << ex);
		return 0;
	}
}

Widget* GUIManager::createWidget()
{
	return createWidget("Widget");
}

Widget* GUIManager::createWidget(const std::string& name)
{
	Widget* widget(0);
	try {

		widget = WidgetLoader::createWidget(name);
		if (widget == 0) {
			S_LOG_FAILURE( "Could not find widget with name " << name );
			return 0;
		}
		widget->init(this);
		widget->buildWidget();
		addWidget(widget);
		S_LOG_INFO( "Successfully loaded widget " << name );
	} catch (const std::exception& e) {
		S_LOG_FAILURE( "Error when loading widget " << name << "." << e);
		return 0;
	}
	return widget;
}

void GUIManager::destroyWidget(Widget* widget)
{
	if (!widget) {
		S_LOG_WARNING("Trying to destroy null widget.");
		return;
	}
	removeWidget(widget);
	delete widget;
}

void GUIManager::setDebugText(const std::string& text)
{
	if (mDebugText) {
		mDebugText->setText(text);
	}
}

Input& GUIManager::getInput() const
{
	return Input::getSingleton();
}

CEGUI::Window* GUIManager::getMainSheet() const
{
	return mSheet;
}

void GUIManager::removeWidget(Widget* widget)
{
	WidgetStore::iterator I = std::find(mWidgets.begin(), mWidgets.end(), widget);
	if (I != mWidgets.end()) {
		mWidgets.erase(I);
	}
}

void GUIManager::addWidget(Widget* widget)
{
	mWidgets.push_back(widget);
}

bool GUIManager::frameStarted(const Ogre::FrameEvent& evt)
{
	try {
		CEGUI::System::getSingleton().injectTimePulse(evt.timeSinceLastFrame);
	} catch (const CEGUI::Exception& ex) {
		S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
	}
	// 	if (mPreviousInputMode == IM_GUI) {
	// 		if (!mInput->getInputMode()) {
	// 			EventInputModeChanged.emit(IM_MOVEMENT);
	// 			mPreviousInputMode = IM_MOVEMENT;
	// 		}
	// 	} else {
	// 		if (mInput->isInGUIMode()) {
	// 			EventInputModeChanged.emit(IM_GUI);
	// 			mPreviousInputMode = IM_GUI;
	// 		}
	// 	}


	//iterate over all widgets and send them a frameStarted event
	WidgetStore::iterator I = mWidgets.begin();
	WidgetStore::iterator I_end = mWidgets.end();

	for (; I != I_end; ++I) {
		Widget* aWidget = *I;
		try {
			aWidget->frameStarted(evt);
		} catch (const CEGUI::Exception& ex) {
			S_LOG_WARNING("Error in CEGUI: " << ex.getMessage().c_str());
		}
	}

	EventFrameStarted.emit(evt.timeSinceLastFrame);

	return true;

}

bool GUIManager::mSheet_MouseButtonDown(const CEGUI::EventArgs& args)
{
	if (isInGUIMode()) {
		const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&> (args);
		S_LOG_VERBOSE("Main sheet is capturing input");
		CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
		if (aWindow) {
			aWindow->releaseInput();
			aWindow->deactivate();
		}
		//mSheet->activate();
		//mSheet->captureInput();

		if (mPicker) {
			const CEGUI::Point& position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
			MousePickerArgs pickerArgs;
			pickerArgs.windowX = mouseArgs.position.d_x;
			pickerArgs.windowY = mouseArgs.position.d_y;
			pickerArgs.pickType = MPT_CLICK;
			mPicker->doMousePicking(position.d_x, position.d_y, pickerArgs);
		}
	}

	return true;
}

bool GUIManager::mSheet_MouseDoubleClick(const CEGUI::EventArgs& args)
{

	const CEGUI::MouseEventArgs& mouseArgs = static_cast<const CEGUI::MouseEventArgs&> (args);
	S_LOG_VERBOSE("Main sheet double click.");
	CEGUI::Window* aWindow = CEGUI::Window::getCaptureWindow();
	if (aWindow) {
		aWindow->releaseInput();
		aWindow->deactivate();
	}
	//mSheet->activate();
	//mSheet->captureInput();

	if (mPicker) {
		const CEGUI::Point& position = CEGUI::MouseCursor::getSingleton().getDisplayIndependantPosition();
		MousePickerArgs pickerArgs;
		pickerArgs.windowX = mouseArgs.position.d_x;
		pickerArgs.windowY = mouseArgs.position.d_y;
		pickerArgs.pickType = MPT_DOUBLECLICK;
		mPicker->doMousePicking(position.d_x, position.d_y, pickerArgs);
	}

	return true;
}

bool GUIManager::mSheet_CaptureLost(const CEGUI::EventArgs& args)
{
	S_LOG_VERBOSE("Main sheet lost input");
	return true;
}

const bool GUIManager::isInMovementKeysMode() const
{
	return mSheet->isCapturedByThis() || !isInGUIMode();
}

const bool GUIManager::isInGUIMode() const
{
	return getInput().getInputMode() == Input::IM_GUI;
}

void GUIManager::pressedKey(const SDL_keysym& key, Input::InputMode inputMode)
{
	if ((key.mod & KMOD_CTRL || key.mod & KMOD_LCTRL || key.mod & KMOD_RCTRL) && (key.sym == SDLK_c || key.sym == SDLK_x)) {

		bool cut = (key.sym == SDLK_x);
		CEGUI::Window* active = mSheet->getActiveChild();
		if (!active)
			return;

		CEGUI::String seltext;
		const CEGUI::String& type = active->getType();

		if (type.find("/MultiLineEditbox") != CEGUI::String::npos) {
			CEGUI::MultiLineEditbox* edit = static_cast<CEGUI::MultiLineEditbox*> (active);
			CEGUI::String::size_type beg = edit->getSelectionStartIndex();
			CEGUI::String::size_type len = edit->getSelectionLength();
			seltext = edit->getText().substr(beg, len).c_str();

			// are we cutting or just copying?
			if (cut) {
				if (edit->isReadOnly())
					return;
				CEGUI::String newtext = edit->getText();
				edit->setText(newtext.erase(beg, len));
			}

		} else if (type.find("/Editbox") != CEGUI::String::npos) {
			CEGUI::Editbox* edit = static_cast<CEGUI::Editbox*> (active);
			CEGUI::String::size_type beg = edit->getSelectionStartIndex();
			CEGUI::String::size_type len = edit->getSelectionLength();
			seltext = edit->getText().substr(beg, len).c_str();

			// are we cutting or just copying?
			if (cut) {
				if (edit->isReadOnly())
					return;
				CEGUI::String newtext = edit->getText();
				edit->setText(newtext.erase(beg, len));
			}
		}
		getInput().writeToClipboard(seltext.c_str());
	}
}

void GUIManager::runCommand(const std::string &command, const std::string &args)
{
	if (command == ToggleInputMode.getCommand()) {
		getInput().toggleInputMode();
	} else if (command == ToggleGui.getCommand()) {

		S_LOG_VERBOSE("Toggle Gui Initiated -- " << getInput().getInputMode() );

		if (mWindow->getViewport(0)->getOverlaysEnabled()) {
			// disable overlays so gui disappears
			S_LOG_INFO("Disabling GUI");
			mWindow->getViewport(0)->setOverlaysEnabled(false);

			getInput().removeAdapter(mCEGUIAdapter);

		} else {

			// enable overlays
			S_LOG_INFO("Enabling GUI");
			mWindow->getViewport(0)->setOverlaysEnabled(true);

			getInput().addAdapter(mCEGUIAdapter);
		}
	} else if (command == ReloadGui.getCommand()) {
		Ogre::TextureManager* texMgr = Ogre::TextureManager::getSingletonPtr();
		Ogre::ResourcePtr resource = texMgr->getByName("cegui/" + getDefaultScheme() + ".png");
		if (!resource.isNull()) {
			resource->reload();
		}
	}
}

// void GUIManager::pushMousePicker( MousePicker * mousePicker )
// {
// 	mMousePickers.push(mousePicker);
// }

// MousePicker * GUIManager::popMousePicker()
// {
// 	///only pop if there's more than one registered picker
// 	if (mMousePickers.size() > 1)
// 		mMousePickers.pop();
// 	return mMousePickers.top();
// }

void GUIManager::EmberOgre_CreatedAvatarEntity(EmberEntity& entity)
{
	///switch to movement mode, since it appears most people don't know how to change from gui mode
	getInput().setInputMode(Input::IM_MOVEMENT);
}

const std::string& GUIManager::getLayoutDir() const
{
	static std::string dir("cegui/datafiles/layouts/");
	return dir;
}

const std::string& GUIManager::getDefaultScheme() const
{
	return mDefaultScheme;
}

Gui::Icons::IconManager* GUIManager::getIconManager()
{
	return mIconManager;
}

Gui::EntityIconManager* GUIManager::getEntityIconManager()
{
	return mEntityIconManager;
}

}

