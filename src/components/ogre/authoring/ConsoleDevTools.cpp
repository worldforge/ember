/*
 * Copyright (C) 2014 Peter Szucs <peter.szucs.dev@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "ConsoleDevTools.h"
#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "framework/Tokeniser.h"
#include "framework/ConsoleBackend.h"
#include "framework/MainLoopController.h"
#include "components/ogre/GUIManager.h"
#include "components/ogre/EmberOgre.h"
#include "components/ogre/World.h"
#include "components/ogre/MovementController.h"
#include "components/ogre/camera/ICameraMount.h"
#include "components/ogre/camera/MainCamera.h"
#include <CEGUI/BasicImage.h>
#include <CEGUI/ImageManager.h>
#include <CEGUI/WindowManager.h>
#include <CEGUI/widgets/DefaultWindow.h>
#include <CEGUI/widgets/FrameWindow.h>
#include <CEGUI/RendererModules/Ogre/Renderer.h>
#include <OgreGpuProgramManager.h>
#include <OgreTextureManager.h>
#include <OgreMaterialManager.h>
#include <Ogre.h>
#include <components/ogre/OgreInfo.h>

namespace Ember {
namespace OgreView {

ConsoleDevTools::ConsoleDevTools() :
		mReloadMaterial("reload_material", this, "Reloads the material. Parameters: <material name>. For example \"/reload_material /base/normalmap/specular\""),
		mShowTexture("show_texture", this, "Show given texture in a separated window. Parameters: <texture name>. For example \"/show_texture DepthBuffer\""),
		mBenchmark("benchmark", this, "Performs a benchmark test") {

}

void ConsoleDevTools::runCommand(const std::string& command, const std::string& args) {
	if (mReloadMaterial == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string materialName(tokeniser.nextToken());
		reloadMaterial(materialName);

	} else if (mShowTexture == command) {
		Tokeniser tokeniser;
		tokeniser.initTokens(args);
		std::string textureName(tokeniser.nextToken());
		showTexture(textureName);
	} else if (mBenchmark == command) {
		performBenchmark();
	}
}

std::string ConsoleDevTools::genUniqueName() {
	static int counter = 0;
	counter++;
	std::stringstream ret;
	ret << "unique" << counter;
	return ret.str();
}

void ConsoleDevTools::showTexture(const std::string& textureName) {
	const float left = 0.0f;
	const float top = 0.0f;
	const float widthScale = 0.25f;
	const float heightScale = 0.25f;

	Ogre::TexturePtr texture = Ogre::TextureManager::getSingleton().getByName(textureName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	if (!texture) {
		ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture not found.", "error");
		return;
	}

	CEGUI::System* sys = CEGUI::System::getSingletonPtr();
	CEGUI::Window* root = sys->getDefaultGUIContext().getRootWindow();
	CEGUI::WindowManager* wmgr = CEGUI::WindowManager::getSingletonPtr();
	const std::string& scheme = Ember::OgreView::GUIManager::getSingleton().getDefaultScheme();
	CEGUI::OgreRenderer* renderer = static_cast<CEGUI::OgreRenderer*>(sys->getRenderer());

	//Create BasicImage wrapper for the Ogre texture.
	std::string imageName = genUniqueName();
	CEGUI::Texture& ceguitexture = renderer->createTexture(imageName, texture);
	CEGUI::BasicImage* image = (CEGUI::BasicImage*) (&CEGUI::ImageManager::getSingleton().create("BasicImage", imageName));
	image->setTexture(&ceguitexture);
	image->setArea(CEGUI::Rectf(0, 0, texture->getWidth(), texture->getHeight()));
	image->setAutoScaled(CEGUI::ASM_Both);

	// Create staticImage to show the BasicImage.
	CEGUI::DefaultWindow* staticImage = static_cast<CEGUI::DefaultWindow*>(wmgr->createWindow(scheme + "/StaticImage", genUniqueName()));
	staticImage->setPosition(CEGUI::UVector2(CEGUI::UDim(0, 0), CEGUI::UDim(0, 0)));
	staticImage->setSize(CEGUI::USize(CEGUI::UDim(1, 0), CEGUI::UDim(1, 0)));
	staticImage->setVisible(true);
	staticImage->setEnabled(true);
	staticImage->setUpdateMode(CEGUI::WUM_VISIBLE);
	staticImage->setUsingAutoRenderingSurface(false);

	// Create scalable Frame window
	CEGUI::FrameWindow* mWindow = (CEGUI::FrameWindow*) wmgr->createWindow(scheme + "/DialogWindow", genUniqueName());
	mWindow->setText(textureName + " texture");
	mWindow->setSize(CEGUI::USize(CEGUI::UDim(0, texture->getWidth() * widthScale), CEGUI::UDim(0, texture->getHeight() * heightScale)));

	mWindow->setPosition(CEGUI::UVector2(CEGUI::UDim(0, left), CEGUI::UDim(0, top)));
	mWindow->moveToFront();
	mWindow->setSizingBorderThickness(5);
	mWindow->setCloseButtonEnabled(true);
	mWindow->setUpdateMode(CEGUI::WUM_VISIBLE);
	mWindow->setUsingAutoRenderingSurface(false);
	mWindow->setEnabled(true);
	mWindow->setVisible(true);

	auto onCloseClicked = [&](const CEGUI::EventArgs& e) -> bool {
		const auto& me = static_cast<const CEGUI::MouseEventArgs&>(e);
		me.window->destroy();
		return true;
	};

	mWindow->subscribeEvent(CEGUI::FrameWindow::EventCloseClicked, onCloseClicked);

	// Link windows together
	staticImage->setProperty("Image", imageName);
	mWindow->addChild((CEGUI::Window*) staticImage);
	root->addChild((CEGUI::Window*) mWindow);
}

void ConsoleDevTools::reloadMaterial(const std::string& materialName) {
	// Normally multiple techniques are using the same texture,
	// so to prevent reloading the texture multiple times, I will put them in a set.
	std::set<std::string> mReloadableTextures;
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(materialName);
	if (!material) {
		ConsoleBackend::getSingleton().pushMessage("'" + materialName + "' material not found.", "error");
		return;
	}
	for (const auto* technique : material->getTechniques()) {
		for (const auto* pass : technique->getPasses()) {
			Ogre::String name;
			Ogre::GpuProgramPtr vertexProgram;
			Ogre::GpuProgramPtr fragmentProgram;
			if (pass->hasVertexProgram()) {
				vertexProgram = pass->getVertexProgram();
				name += "Vertex Program:";
				name += vertexProgram->getName();
			}
			if (pass->hasFragmentProgram()) {
				fragmentProgram = pass->getFragmentProgram();
				name += " Fragment Program:";
				name += fragmentProgram->getName();
			}
			// NOTE: Microcode name may depend on render system. This works for OpenGL!
			Ogre::GpuProgramManager::getSingleton().removeMicrocodeFromCache(name);

			if (vertexProgram) {
				vertexProgram->reload();
				vertexProgram->createParameters();
				ConsoleBackend::getSingleton().pushMessage("Reloaded vertex program '" + vertexProgram->getName() + "'.", "info");
			}
			if (fragmentProgram) {
				fragmentProgram->reload();
				fragmentProgram->createParameters();
				ConsoleBackend::getSingleton().pushMessage("Reloaded fragment program '" + fragmentProgram->getName() + "'.", "info");
			}
			for (const auto* tex : pass->getTextureUnitStates()) {
				const std::string& textureName(tex->getTextureName());
				if (!textureName.empty()) {
					mReloadableTextures.insert(textureName);
				}
			}
		}
	}
	for (const std::string& textureName : mReloadableTextures) {
		reloadTexture(textureName);
	}
	material->reload();
}

void ConsoleDevTools::reloadTexture(const std::string& textureName) {
	Ogre::ResourcePtr texture = Ogre::TextureManager::getSingleton().getByName(textureName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);
	if (!texture) {
		ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture not found.", "error");
		return;
	}
	if (!texture->isReloadable()) {
		ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture is not reloadable. Skipped!", "info");
		return;
	}
	texture->reload();
	ConsoleBackend::getSingleton().pushMessage("'" + textureName + "' texture reloaded.", "info");
}

void ConsoleDevTools::performBenchmark() {


	auto& emberOgre = EmberOgre::getSingleton();
	if (emberOgre.getWorld()) {

		Ogre::SceneNode* cameraNode = emberOgre.getWorld()->getSceneManager().createSceneNode(OgreInfo::createUniqueResourceName("StaticCameraNode"));

		class StaticCameraMount : public Camera::ICameraMount {
		public:
			Ogre::SceneNode* mCameraNode = nullptr;

			Ogre::Degree pitch(float relativeMovement) override {
				return Ogre::Degree(0);
			}

			Ogre::Degree yaw(float relativeMovement) override {
				return Ogre::Degree(0);
			}

			Ogre::Degree getPitch() const override {
				return Ogre::Degree(0);
			}

			Ogre::Degree getYaw() const override {
				return Ogre::Degree(0);
			}

			void move(const WFMath::Vector<3>& movement, const WFMath::Quaternion& orientation, float timeslice) override {}

			void attachToCamera(Camera::MainCamera& camera) override {
				mCameraNode->attachObject(&camera.getCamera());
			}

			void detachFromCamera() override {
				mCameraNode->detachAllObjects();
			}

			void setMotionHandler(ICameraMotionHandler* handler) override {}
		};

		std::shared_ptr<StaticCameraMount> cameraMount(new StaticCameraMount());
		cameraMount->mCameraNode = cameraNode;
		emberOgre.getWorld()->getMainCamera().attachToMount(cameraMount.get());


		//Listen for 60 frames at each location, keeping track of time.
		struct BenchmarkFrameListener : public Ogre::FrameListener {
			std::list<std::pair<Ogre::Vector3, Ogre::Vector3>> positionsAndDirections;
			std::vector<std::chrono::steady_clock::duration> results;
			int frames = -1;
			std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
			double originalFps;
			Ogre::SceneNode* cameraNode;
			std::shared_ptr<StaticCameraMount> cameraMount;

			BenchmarkFrameListener() {
				originalFps = (double) EmberServices::getSingleton().getConfigService().getValue("general", "desiredfps");
				EmberServices::getSingleton().getConfigService().setValue("general", "desiredfps", 0);

			}

			bool frameEnded(const Ogre::FrameEvent& evt) override {
				frames++;
				if (frames == 60 || frames == 0) {

					if (frames == 60) {
						results.emplace_back(std::chrono::steady_clock::now() - start);
					}

					if (positionsAndDirections.empty()) {
						Ogre::Root::getSingleton().removeFrameListener(this);
						EmberServices::getSingleton().getConfigService().setValue("general", "desiredfps", originalFps);
						EmberOgre::getSingleton().getWorld()->getMovementController()->setCameraFreeFlying(EmberOgre::getSingleton().getWorld()->getMovementController()->isCameraFreeFlying());

						//Report results
						std::chrono::steady_clock::duration totalDuration{};
						for (auto entry : results) {
							totalDuration += entry;
							auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(entry).count();
							std::string message = "FPS: " + std::to_string(60 / (microseconds / 1000000.0));
							S_LOG_INFO(message);
							ConsoleBackend::getSingleton().pushMessage(message);
						}

						auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(totalDuration).count();
						std::string message = "Total FPS: " + std::to_string((60 * results.size()) / (microseconds / 1000000.0));
						S_LOG_INFO(message);
						ConsoleBackend::getSingleton().pushMessage(message);

					} else {
						cameraNode->setPosition(positionsAndDirections.front().first);
						cameraNode->setDirection(positionsAndDirections.front().second);
						positionsAndDirections.pop_front();
						frames = 0;
						start = std::chrono::steady_clock::now();
					}
				}

				return true;
			}
		};

		auto benchmarkFrameListener = new BenchmarkFrameListener();
		benchmarkFrameListener->cameraNode = cameraNode;
		benchmarkFrameListener->cameraMount = cameraMount;

		benchmarkFrameListener->positionsAndDirections.emplace_back(Ogre::Vector3(-800, 20, 990), Ogre::Vector3(0, 0, -1));
		benchmarkFrameListener->positionsAndDirections.emplace_back(Ogre::Vector3(-800, 50, 590), Ogre::Vector3(0, 0, -1));
		benchmarkFrameListener->positionsAndDirections.emplace_back(Ogre::Vector3(-870, 5, 800), Ogre::Vector3(0, 0, -1));
		benchmarkFrameListener->positionsAndDirections.emplace_back(Ogre::Vector3(-707, 5, 750), Ogre::Vector3(0, 0, -1));
		benchmarkFrameListener->positionsAndDirections.emplace_back(Ogre::Vector3(-800, 20, 990), Ogre::Vector3(0, 0, -1));


		Ogre::Root::getSingleton().addFrameListener(benchmarkFrameListener);

	}
}

}
}
