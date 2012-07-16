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

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"

#include <RendererModules/OpenGL/CEGUIOpenGLRenderer.h>
#include <CEGUI.h>

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

OgreConfigurator::OgreConfigurator()
{
	// fake args for glutInit
	int argc = 1;
	const char* argv = "Ember";

	// Do GLUT init
	glutInit(&argc, (char**)&argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(300, 400);
	glutInitWindowPosition(400, 300);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutCreateWindow("Ember");
	glutSetCursor(GLUT_CURSOR_INHERIT);

	CEGUI::DefaultResourceProvider* rp = new SimpleResourceProvider();

	CEGUI::OpenGLRenderer& renderer = CEGUI::OpenGLRenderer::create();
	CEGUI::System::create(renderer, rp);

	glutDisplayFunc(&OgreConfigurator::drawFrame);
	glutMotionFunc(&OgreConfigurator::mouseMotion);
	glutPassiveMotionFunc(&OgreConfigurator::mouseMotion);
	glutMouseFunc(&OgreConfigurator::mouseButton);

	// Set the clear color
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	CEGUI::SchemeManager::getSingleton().create("cegui/datafiles/schemes/EmberLookSkin.scheme", "");

	CEGUI::Window* sheet = CEGUI::WindowManager::getSingleton().createWindow("DefaultGUISheet", "root_wnd");
	CEGUI::System::getSingleton().setGUISheet(sheet);

	CEGUI::Window* configWindow = CEGUI::WindowManager::getSingleton().loadWindowLayout("cegui/datafiles/layouts/OgreConfigurator.layout", "OgreConfigure");

	sheet->addChildWindow(configWindow);
	try {
		glutMainLoop();
	} catch (...) {

	}

}

OgreConfigurator::~OgreConfigurator()
{
	CEGUI::System::getSingleton().destroy();
}

bool OgreConfigurator::buttonClicked(const CEGUI::EventArgs& args)
{
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
