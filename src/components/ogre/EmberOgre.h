/*
-----------------------------------------------------------------------------

Author: Erik Hjortsberg <erik.hjortsberg@gmail.com>, (C) 2009

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
*/

#ifndef __EmberOgre_H__
#define __EmberOgre_H__

#include "EmberOgrePrerequisites.h"
#include <OgreFrameListener.h>

// ------------------------------
// Include sigc header files
// ------------------------------
#include <sigc++/trackable.h>
#include <sigc++/signal.h>


#include "framework/Singleton.h"
#include <memory>

namespace Eris {
class View;
class Connection;
}

namespace Carpenter
{
class Carpenter;
class BluePrint;
}

namespace Ember
{
class StreamLogObserver;
class InputCommandMapper;
}

namespace OgreOpcode {
class CollisionManager;
}

namespace EmberOgre {

namespace Terrain
{
class TerrainGenerator;
class TerrainLayerDefinitionManager;
}

class SoundDefinitionManager;

namespace Mapping {
	class EmberEntityMappingManager;
}

namespace Model {
	class ModelDefinitionManager;
	class ModelRepresentationManager;
}

class CameraRotator;

class CameraFrameListener;

class Avatar;

class MainCamera;

class MovementController;

class AvatarEmberEntity;

class EmberEntityFactory;

class EmberPagingSceneManager;

class MotionManager;

class GUIManager;

class Jesus;

class EmberEntity;

class OgreResourceLoader;

class OgreLogObserver;

class EntityMoveManager;

class MaterialEditor;

class OgreSetup;

class OgreResourceProvider;
class OpcodeCollisionDetectorVisualizer;

class EntityRecipeManager;

class ShaderManager;

/**
	@brief The main class of ember. This functions as a hub for almost all subsystems.

	This is the central class for the whole Ogre related part of Ember. It's quite large and could perhaps be refactored into smaller parts though.
	It holds references to most subcomponents, and is instanciated at the start of Ember. A lot of different signals from different subsystems are routed through here. If you're not sure how to get access to a certain subsystem from another unrelated class, this is probably the first place you should look.

	It's a singleton so you can access it through
	@code
	EmberOgre::EmberOgre::getSingleton()
	@endcode

	@author Erik Hjortsberg <erik.hjortsberg@gmail.com>

*/
class EmberOgre : public Ember::Singleton<EmberOgre>,
public sigc::trackable,
public Ogre::FrameListener
{
public:


    /**
     * @brief Ctor.
     */
    EmberOgre();

    /**
     * @brief Dtor.
     */
    ~EmberOgre();

	virtual bool frameStarted(const Ogre::FrameEvent & evt);
	virtual bool frameEnded(const Ogre::FrameEvent & evt);

	/**
	 * @brief Initialize all Ember services needed for this application
	 * @param  the prefix for the application, not appliable if running under win32
	 * @param  the an alternative home directory. If the default should be used, send an empty string.
	 */
	void initializeEmberServices(const std::string& prefix, const std::string& homeDir);

	/**
	 * @brief Gets the main avatar instance.
	 * @return The main avatar instance. This is currently valid even before the user has logged in to a server, but this will probably change in the future.
	 */
	Avatar* getAvatar() const;

	/**
	 * @brief Gets the main scene manager.
	 * There will be many different scene managers created by Ember, but this one is the main one which represents the world.
	 * @return The main scene manager.
	 */
	Ogre::SceneManager* getSceneManager() const;

	/**
	 * @brief Gets the terrain generator.
	 * @return The terrain generator, responsible for creating and handling the terrain.
	 */
	Terrain::TerrainGenerator* getTerrainGenerator() const;

	/**
	 * @brief Gets the motion manager, which is responsible for handling all motion and animations of entities.
	 * @return The motion manager.
	 */
	MotionManager* getMotionManager() const;

	/**
	 * @brief Gets the Ogre root object.
	 * @return The Ogre root object, which acts as an entry point for most of Ogre.
	 */
	Ogre::Root* getOgreRoot() const;

	/**
	 * @brief Gets the entity factory which is responsible for creating all new entities in the world.
	 * @return The entity factory.
	 */
	EmberEntityFactory* getEntityFactory() const;

	/**
	 * @brief Gets the main camera, which is attached to the Avatar.
	 * @return The main avatar camera.
	 */
	MainCamera* getMainCamera() const;

	Ogre::Camera* getMainOgreCamera() const;

	/**
	 * @brief Gets the avatar controller, which recieves input and makes sure that the Avatar is moved.
	 * @return The avatar controller.
	 */
	MovementController* getMovementController() const;

	/**
	 * @brief Gets the entity move manager, which handles movement of entities in the world by the user.
	 * @return The entity move manager.
	 */
	EntityMoveManager* getMoveManager() const;

	/**
	 * @brief Gets the shader manager, which handles setup of the graphic level and the shaders.
	 * @return The shader manager.
	 */
	ShaderManager* getShaderManager() const;

	/**
	 * @brief Finds and returns the entity with the given id, if it's available.
	 * @param id The unique id for the entity.
	 * @return An instance of EmberEntity or null if no entity with the specified id could be found.
	 */
	EmberEntity* getEmberEntity(const std::string & eid);

	/**
	 * @brief Returns the main Jesus object, which should be used as the main entry into the Jesus subsystem.
	 * @return The main Jesus instance.
	 */
//	Jesus* getJesus() const;

	/**
	 * @brief Gets the main render window.
	 * The system can contains many different render windows, but there's only one that's the main window.
	 * @return The main render window.
	 */
	Ogre::RenderWindow* getRenderWindow() const;

	/**
	 * @brief Emitted when the Ember entity factory has been created.
	 */
	sigc::signal<void, EmberEntityFactory*> EventCreatedEmberEntityFactory;

	/**
	 * @brief Emitted when the avatar entity has been created.
	 */
	sigc::signal<void, EmberEntity*> EventCreatedAvatarEntity;

	/**
	 * @brief Emitted when the Jesus subsystem has been created.
	 */
//	sigc::signal<void, Jesus*> EventCreatedJesus;

	/**
	 * @brief Returns the scenenode of the world entity.
	 * Throws en exception if no such node has been created yet.
	 * @return
	 */
	Ogre::SceneNode* getWorldSceneNode() const;

	/**
	 * @brief Returns the root scene node, to which all other nodes are attached.
	 * @return The Ogre root scene node.
	 */
	Ogre::SceneNode* getRootSceneNode() const;

	/**
	 * @brief Emitted after the GUIManager has been created, but not yet initialized
	 */
	sigc::signal<void, GUIManager&> EventGUIManagerCreated;

	/**
	 * @brief Emitted after the GUIManager has been initilized
	 */
	sigc::signal<void, GUIManager&> EventGUIManagerInitialized;

	/**
	 * @brief Emitted after the Motion has been created
	 */
	sigc::signal<void, MotionManager&> EventMotionManagerCreated;


	/**
	 * @brief Emitted after the TerrainGenerator has been created
	 */
	sigc::signal<void, Terrain::TerrainGenerator&> EventTerrainGeneratorCreated;

	/**
	 * @brief Emitted after the MovementController has been created
	 */
	sigc::signal<void> EventMovementControllerCreated;

	/**
	 * @brief Emitted before the main Ogre render window is rendered.
	 */
	sigc::signal<void> EventBeforeRender;

	/**
	 * @brief Emitted after the main Ogre render window is rendered.
	 */
	sigc::signal<void> EventAfterRender;

	/**
	 * @brief Renders one frame.
	 * @return True if rendering should continue. False if the application should end.
	 */
	bool renderOneFrame();

    /**
     * @brief Sets up the application - returns false if the user chooses to abandon configuration.
     * @return
     */
    bool setup();

    /**
     * @brief Shuts down the gui.
     * Call this at closing time, right before you destroy the view and shut down the scripting service.
     */
    void shutdownGui();

protected:

	/**
	 * @brief Sent from the server service when we've recieved a Eris::View instance from the server.
	 * @param world
	 */
	void Server_GotView(Eris::View* world);

	/**
	 * @brief Sent from the server when we've successfully connected.
	 * @param connection
	 */
	void Server_GotConnection(Eris::Connection* connection);

	/**
	 * @brief Utility object for setting up and tearing down ogre
	 */
	std::auto_ptr<OgreSetup> mOgreSetup;

	/**
	 * @brief Gets the main Eris View instance, which is the main inteface to the world.
	 * The View can also be accessed through the Server service, but this can be used for convenience.
	 * @return The main Eris View instance which represents the server world.
	 */
	Eris::View* getMainView();

	/**
	 * @brief The main user avatar
	 */
	Avatar* mAvatar;

	/**
	 * @brief When connected to a world, handles the avatar and patches mouse and keyboard movement events on the avatar.
	 */
	MovementController* mMovementController;

	/**
	 * @brief The main Ogre root object. All of Ogre is accessed through this.
	 */
	Ogre::Root *mRoot;

	/**
	 * @brief The main scene manager of the world.
	 */
	EmberPagingSceneManager* mSceneMgr;

	/**
	 * @brief The main render window. There can be many more render targets in the system, but they will all reside within this render window (such as entity preview through CEGUI).
	 */
	Ogre::RenderWindow* mWindow;

	/**
	 * @brief Utility object to configure shaders
	 */
	ShaderManager* mShaderManager;

	/**
	 * @brief An InputCommandMapper that will handle all general input events.
	 */
	std::auto_ptr<Ember::InputCommandMapper> mGeneralCommandMapper;

	/**
	 * @brief Main factory for all entities created in the world.
	 */
	EmberEntityFactory* mEmberEntityFactory;

	/**
	 * @brief Sets up Jesus. This inialized the mJesus member and loads all building blocks, blueprint and modelblocks etc.
	 */
//	void setupJesus();

	/**
	 * @brief Preloads the media, thus avoiding frame rate drops ingame.
	 * Note that preloading media can take quite a while.
	 */
	void preloadMedia();

	/**
	 * @brief Makes sure that there are files in ~/.ember.
	 * Call this early on at application startup. If the required files cannot be found they are copied there from their default versions as installed in the shared location.
	 */
	void checkForConfigFiles();

	void CreatedAvatarEntity(EmberEntity* entity);

	/**
	 * @brief Responsible for handling of terrain.
	 */
	Terrain::TerrainGenerator* mTerrainGenerator;

	/**
	 * @brief Responsible for handling sound loading
	 */
	SoundDefinitionManager* mSoundManager;

	/**
	 * @brief Responsible for updating motions and animations of entities.
	 */
	MotionManager* mMotionManager;

	/**
	 * @brief Responsible for the GUI.
	 */
	GUIManager* mGUIManager;

	/**
	 * @brief Resonsible for managing all Model definitions;
	 */
	Model::ModelDefinitionManager* mModelDefinitionManager;

	/**
	 * @brief Handles all model mappings.
	 */
	Mapping::EmberEntityMappingManager* mEntityMappingManager;

	/**
	 * @brief Responsible for handling all terrain layers.
	 */
	Terrain::TerrainLayerDefinitionManager* mTerrainLayerManager;

	/**
	 * @brief Responsible for handling all entity recipes.
	 */
	EntityRecipeManager* mEntityRecipeManager;

	/**
	 * @brief Responsible for allowing movement of entities in the world by the user.
	 */
	EntityMoveManager* mMoveManager;

	/**
	 * @brief Main entry point for the Jesus system (which is an Ember wrapper for the Carpenter lib)
	 * @note The Jesus and Carpenter system hasn't been touched in quite a while, and it's not certain that it will remain in Ember.
	 */
//	Jesus* mJesus;

	/**
	 * @brief The main log observer used for all logging. This will send Ogre logging events on to the internal Ember logging framework.
	 */
	OgreLogObserver* mLogObserver;

	/**
	 * @brief Helper object that allows for easy Ogre material editing.
	 */
	MaterialEditor* mMaterialEditor;

	/**
	 * @brief Manages the model representation instances and allows for access of a specific model representation attached to any entity.
	 */
	Model::ModelRepresentationManager* mModelRepresentationManager;

	/**
	 * @brief Listen for when all of the application services has been initialized and then register needed objects with them.
	 * This should occur fairly early on in the application life cycle.
	 */
	void Application_ServicesInitialized();

	/**
	 * @brief The scripting resource provider, which loads scripts using the Ogre resource system.
	 */
	std::auto_ptr<OgreResourceProvider> mScriptingResourceProvider;

	/**
	 * @brief The sound resource provider, which loads sounds using the Ogre resource system.
	 */
	std::auto_ptr<OgreResourceProvider> mSoundResourceProvider;

	/**
	 * @brief The collision manager, responsible for handling collisions of the geometry in the world.
	 */
//	OgreOpcode::CollisionManager* mCollisionManager;

	/**
	 * @brief Responsible for visualizing collisions.
	 */
//	OpcodeCollisionDetectorVisualizer* mCollisionDetectorVisualizer;

	/**
	 *@brief Handles loading of resources. This will also take care of registering our own Ogre::ArchiveFactory instance, so it needs to be destroyed first after ogre is shutdown (since there's no way to remove an already added ArchiveFactory instance from Ogre).
	 */
	OgreResourceLoader* mResourceLoader;

	/**
	 * @brief We hold a reference to our own Ogre log manager, thus making sure that Ogre doesn't create one itself.
	 * Since we do this we can better steer how Ogre log messages are handled.
	 */
	Ogre::LogManager* mOgreLogManager;

	/**
	 * @brief Set this to true when we're not rendering. The reason is that later on, when we resume rendering, we need to reset the event times to prevent various particle effect strangeness.
	 */
	bool mIsInPausedMode;

	MainCamera* mMainCamera;
	Ogre::Camera* mOgreMainCamera;

};

inline EntityMoveManager* EmberOgre::getMoveManager() const
{
	return mMoveManager;
}

inline Ogre::RenderWindow* EmberOgre::getRenderWindow() const
{
	return mWindow;
}


}


#endif
