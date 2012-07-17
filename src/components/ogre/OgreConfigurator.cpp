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

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include "framework/LoggingInstance.h"

#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
#include <RendererModules/Ogre/CEGUIOgreResourceProvider.h>
#include <CEGUI.h>
#include <elements/CEGUICombobox.h>
#include <elements/CEGUIListboxTextItem.h>

#include <OgreRoot.h>

#ifdef __APPLE__
#include <GLUT/freeglut.h>
#else
#ifdef __MINGW32__
#define GLUT_DISABLE_ATEXIT_HACK
#endif
#include <GL/freeglut.h>
#endif

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>

#ifdef _MSC_VER
# if defined(DEBUG) || defined (_DEBUG)
#	if defined(CEGUI_STATIC)
#		pragma comment (lib, "CEGUIOpenGLRenderer_Static_d.lib")
#	else
#		pragma comment (lib, "CEGUIOpenGLRenderer_d.lib")
#	endif
# else
#	if defined(CEGUI_STATIC)
#		pragma comment (lib, "CEGUIOpenGLRenderer_Static.lib")
#	else
#		pragma comment (lib, "CEGUIOpenGLRenderer.lib")
#	endif
# endif
#endif

namespace Ember
{
namespace OgreView
{

class SimpleResourceProvider: public CEGUI::DefaultResourceProvider
{
protected:

public:
	SimpleResourceProvider()
	{

	}

	~SimpleResourceProvider()
	{
	}

	void loadRawDataContainer(const CEGUI::String& filename, CEGUI::RawDataContainer& output, const CEGUI::String& resourceGroup)
	{
		ConfigService& cfgService = EmberServices::getSingleton().getConfigService();
		std::string mediaDirectory = cfgService.getSharedMediaDirectory();
		std::string guiRoot = mediaDirectory + "/gui/";

		std::string finalFilename(guiRoot + filename.c_str());

		std::ifstream ifile(finalFilename.c_str(), std::ifstream::in);
		if (!ifile) {
			ifile.open((std::string("/home/erik/ember/home/ember-media-dev/common/") + filename.c_str()).c_str(), std::ifstream::in);
			if (!ifile) {
				CEGUI_THROW(CEGUI::InvalidRequestException("DefaultResourceProvider::load: " + filename + " does not exist"));
			}
		}

		ifile.seekg(0, std::ios::end);
		std::streamsize size = ifile.tellg();
		ifile.seekg(0, std::ios::beg);

		std::auto_ptr<char> buffer(new char[size]);
		ifile.read(buffer.get(), size);

		if (ifile.gcount() != size) {
			CEGUI_THROW(CEGUI::GenericException( "DefaultResourceProvider::loadRawDataContainer: "
			"A problem occurred while reading file: " + filename));
		}

		output.setData((unsigned char*)buffer.release());
		output.setSize(size);

	}

};

int OgreConfigurator::mLastFrameTime = 0;

OgreConfigurator::OgreConfigurator() :
		mCancel(true)
{
}

OgreConfigurator::~OgreConfigurator()
{
}

bool OgreConfigurator::configure()
{
	// fake args for glutInit
	int argc = 1;
	const char* argv = "Ember";

	// Do GLUT init
	glutInit(&argc, (char**)&argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(250, 300);
	glutInitWindowPosition(400, 300);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutCreateWindow("Ember");
	glutSetCursor(GLUT_CURSOR_INHERIT);

	CEGUI::ResourceProvider* rp = new SimpleResourceProvider();
//	CEGUI::ResourceProvider* rp = new CEGUI::OgreResourceProvider();

	CEGUI::OpenGLRenderer& renderer = CEGUI::OpenGLRenderer::create();
	CEGUI::System::create(renderer, rp);

	glutDisplayFunc(&OgreConfigurator::drawFrame);
	glutMotionFunc(&OgreConfigurator::mouseMotion);
	glutPassiveMotionFunc(&OgreConfigurator::mouseMotion);
	glutMouseFunc(&OgreConfigurator::mouseButton);

	// Set the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	CEGUI::SchemeManager::getSingleton().create("cegui/datafiles/schemes/EmberLookSkin.scheme", "");
	CEGUI::System::getSingleton().setDefaultFont("DejaVuSans-8");
	CEGUI::ImagesetManager::getSingleton().create("cegui/datafiles/imagesets/splash.imageset", "");

	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultGUISheet", "root_wnd");
	CEGUI::System::getSingleton().setGUISheet(sheet);

	CEGUI::Window* configWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("cegui/datafiles/layouts/OgreConfigurator.layout", "OgreConfigure/");
	sheet->addChildWindow(configWindow);

	CEGUI::Window* okButton = configWindow->getChildRecursive("OgreConfigure/Button_ok");
	okButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonOkClicked, this));
	CEGUI::Window* cancelButton = configWindow->getChildRecursive("OgreConfigure/Button_cancel");
	cancelButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonCancelClicked, this));
	CEGUI::Window* advancedButton = configWindow->getChildRecursive("OgreConfigure/Advanced");
	advancedButton->subscribeEvent(CEGUI::PushButton::EventClicked, CEGUI::Event::Subscriber(&OgreConfigurator::buttonAdvancedClicked, this));

	CEGUI::Checkbox* fullscreenCheckbox = static_cast<CEGUI::Checkbox*>(configWindow->getChildRecursive("OgreConfigure/Fullscreen"));

	CEGUI::Combobox* resolutionsCombobox = static_cast<CEGUI::Combobox*>(configWindow->getChildRecursive("OgreConfigure/Resolution"));

	const Ogre::RenderSystemList& renderers = Ogre::Root::getSingleton().getAvailableRenderers();
	if (renderers.size() == 0) {
		return false;
	}

	Ogre::RenderSystem* renderSystem = *renderers.begin();
	Ogre::ConfigOptionMap& configs = renderSystem->getConfigOptions();
	for (Ogre::ConfigOptionMap::const_iterator I = configs.begin(); I != configs.end(); ++I) {
		S_LOG_VERBOSE(I->first);
	}

	bool resolutionFoundInOptions = false;
	Ogre::ConfigOptionMap::const_iterator optionsIter = configs.find("Video Mode");
	if (optionsIter != configs.end()) {
		const Ogre::StringVector& possibleResolutions = optionsIter->second.possibleValues;
		for (Ogre::StringVector::const_iterator I = possibleResolutions.begin(); I != possibleResolutions.end(); ++I) {
			Gui::ColouredListItem* item = new Gui::ColouredListItem(*I);
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

	optionsIter = configs.find("Full Screen");
	if (optionsIter != configs.end()) {
		fullscreenCheckbox->setSelected(optionsIter->second.currentValue == "Yes");
	}

	try {
		glutMainLoop();
	} catch (...) {

	}

	renderSystem->setConfigOption("Video Mode", resolutionsCombobox->getText().c_str());
	renderSystem->setConfigOption("Full Screen", fullscreenCheckbox->isSelected() ? "Yes" : "No");

	Ogre::Root::getSingleton().setRenderSystem(renderSystem);
	CEGUI::System::getSingleton().destroy();
	return !mCancel;
}

bool OgreConfigurator::buttonOkClicked(const CEGUI::EventArgs& args)
{
	mCancel = false;
	glutLeaveMainLoop();
	return true;
}

bool OgreConfigurator::buttonCancelClicked(const CEGUI::EventArgs& args)
{
	glutLeaveMainLoop();
	return true;
}

bool OgreConfigurator::buttonAdvancedClicked(const CEGUI::EventArgs& args)
{
	mCancel = !Ogre::Root::getSingleton().showConfigDialog();
	glutLeaveMainLoop();
	return true;
}

void OgreConfigurator::drawFrame(void)
{
	CEGUI::System& guiSystem = CEGUI::System::getSingleton();
	int thisTime = glutGet(GLUT_ELAPSED_TIME);
	float elapsedTime = static_cast<float>(thisTime - mLastFrameTime);
	mLastFrameTime = thisTime;
	guiSystem.injectTimePulse(elapsedTime / 1000.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	guiSystem.renderGUI();
	glutPostRedisplay();
	glutSwapBuffers();
}

void OgreConfigurator::mouseMotion(int x, int y)
{
	CEGUI::System::getSingleton().injectMousePosition(x, y);
}

void OgreConfigurator::mouseButton(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_UP) {
			CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::LeftButton);
		} else {
			CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::LeftButton);
		}
		break;

	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_UP) {
			CEGUI::System::getSingleton().injectMouseButtonUp(CEGUI::RightButton);
		} else {
			CEGUI::System::getSingleton().injectMouseButtonDown(CEGUI::RightButton);
		}
		break;
	}

}

}
}
