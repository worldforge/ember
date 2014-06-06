#include "SBScene.h"
#ifdef WIN32
#include <direct.h>
#endif

#define BOOST_NO_CXX11_SCOPED_ENUMS

#include <sb/SBTypes.h>
#include <sb/SBObject.h>
#include <sb/SBCharacter.h>
#include <sb/SBMotion.h>
#include <sb/SBScript.h>
#include <sb/SBEvent.h>
#include <sb/SBPhoneme.h>
#include <sb/SBSimulationManager.h>
#include <sb/SBBmlProcessor.h>
#include <sb/SBAnimationState.h>
#include <sb/SBAnimationTransition.h>
#include <sb/SBAnimationStateManager.h>
#include <sb/SBReach.h>
#include <sb/SBReachManager.h>
#ifdef EMBER_SB_STEER
	#include <sb/SBSteerAgent.h>
	#include <sb/SBSteerManager.h>
#endif
#include <sb/SBServiceManager.h> 
#include <sb/SBService.h>
#include <sb/SBPhysicsManager.h>
#ifdef EMBER_SB_BONEBUS
	#include <sb/SBBoneBusManager.h>
#endif
#include <sb/SBGestureMap.h>
#include <sb/SBGestureMapManager.h> 
#include <sb/SBJointMapManager.h> 
#include <sb/SBCollisionManager.h>
#include <sb/SBPhonemeManager.h>
#include <sb/SBBehaviorSetManager.h> // ???
#include <sb/SBRetargetManager.h> 
#include <sb/SBAssetManager.h>
#include <sb/SBSpeechManager.h> // ?
#include <sb/SBCommandManager.h>
#ifdef EMBER_SB_WSP
	#include <sb/SBWSPManager.h> // ???
#endif
#ifdef EMBER_SB_STEER
	#include <sb/SBNavigationMeshManager.h>
#endif
#include <sb/SBSkeleton.h>
#include <sb/SBParser.h>
#include <sb/SBRetarget.h>
#ifdef EMBER_SB_DEBUGGER
	#include <sb/SBDebuggerServer.h> // ?
	#include <sb/SBDebuggerClient.h> // ?
	#include <sb/SBDebuggerUtility.h> // ?
#endif
#ifdef EMBER_SB_VHMSG
#include <sb/SBVHMsgManager.h>
#endif
#include <sb/SBMotionGraph.h> 
#ifdef EMBER_SB_AUDIO
	#include <sbm/sbm_audio.h>
#endif
#include <boost/version.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <sb/nvbg.h> // ?
#include <sb/SBJointMap.h>
#include <sb/SBSceneListener.h>
#ifdef EMBER_SB_STEER
	#include <sb/SBNavigationMesh.h>
#endif
#ifdef EMBER_SB_OTHERFMT
	#include <sbm/ParserBVH.h> // ?
#include <sbm/ParserCOLLADAFast.h> // ?
#endif
#include <sbm/ParserOpenCOLLADA.h> // ?
#include <sbm/ParserOgre.h> // ?
#ifdef EMBER_SB_HEIGHTFIELD
	#include <sbm/Heightfield.h> // ?
#endif
#include <sbm/action_unit.hpp>
#include <sbm/xercesc_utils.hpp> // ?
#ifdef EMBER_SB_CAMERA
	#include <sr/sr_camera.h> 
#endif
#include <controllers/me_ct_gaze.h>
#include <controllers/me_ct_eyelid.h>
#include <controllers/me_ct_breathing.h>
#include <controllers/me_ct_example_body_reach.hpp>
#include <controllers/me_ct_saccade.h>
#ifdef EMBER_SB_KINECT
	#include <sbm/KinectProcessor.h>
#endif
#include <controllers/me_controller_tree_root.hpp>
#include <sr/sr_sn_group.h>
#ifdef EMBER_SB_GNU
#if !defined(SB_IPHONE) && !defined(__ANDROID__)
#include <sbm/GPU/SbmShader.h> // ??
#endif
#if !defined(SB_IPHONE)
#include <sbm/GPU/SbmTexture.h> // ??
#endif
#endif
#ifdef EMBER_SB_KINECT
	#include <sbm/KinectProcessor.h>
#endif
#include <sr/sr_sn_group.h>
#include <fstream>
#include <sstream>
// for minizip compression

#ifdef EMBER_SB_ZLIB
#if !defined(__FLASHPLAYER__)
#include <external/zlib-1.2.5/zip.h>
#endif
#endif

#ifndef WIN32
#define _stricmp strcasecmp
#endif

#define SHOW_DEPRECATION_MESSAGES 0
namespace SmartBody {

SBScene* SBScene::_scene = NULL;
bool SBScene::_firstTime = true;

const std::string defautAssetPath = "Assets";

std::map<std::string, std::string> SBScene::_systemParameters;


class ForwardLogListener : public vhcl::Log::Listener
{
    public:
		ForwardLogListener() {}
		virtual ~ForwardLogListener() {}

        virtual void OnMessage( const std::string & message )
		{
			SBScene* scene = SmartBody::SBScene::getScene();
			if (!scene)
				return;
			std::vector<SBSceneListener*>& listeners = scene->getSceneListeners();
			for (size_t i = 0; i < listeners.size(); i++)
			{
				listeners[i]->OnLogMessage(message);
			}
		}
};


SBScene::SBScene(void) : SBObject()
{
	//initialize();
}

void SBScene::initialize()
{
#ifndef SB_NO_PYTHON
#ifndef __native_client__
//	_mainModule = NULL;
//	_mainDict = NULL;
#endif
#endif
	_processId = "";

	createDefaultControllers();

	_sceneListeners.clear();

	_sim = new SBSimulationManager();
	_profiler = new SBProfiler();
	_bml = new SBBmlProcessor();
	_blendManager = new SBAnimationBlendManager();
	_reachManager = new SBReachManager();
#ifdef EMBER_SB_STEER
	_steerManager = new SBSteerManager();
#endif
	_serviceManager = new SBServiceManager();
	_physicsManager = new SBPhysicsManager();
	_gestureMapManager = new SBGestureMapManager();
	_jointMapManager = new SBJointMapManager();
#ifdef EMBER_SB_BONEBUS
	_boneBusManager = new SBBoneBusManager();
#endif
	_collisionManager = new SBCollisionManager();
	_diphoneManager = new SBDiphoneManager();
	_behaviorSetManager = new SBBehaviorSetManager();
	_retargetManager = new SBRetargetManager();
	_eventManager = new SBEventManager();
	_assetManager = new SBAssetManager();
	_speechManager = new SBSpeechManager();
#ifdef EMBER_SB_VHMSG
	_vhmsgManager = new SBVHMsgManager();
#endif
	_commandManager = new SBCommandManager();
#ifdef EMBER_SB_WSP
	_wspManager = new SBWSPManager();
#endif
#ifdef EMBER_SB_STEER
	_naviMeshManager = new SBNavigationMeshManager();
#endif
	_motionGraphManager = new SBMotionGraphManager();

	//_scale = .01f; // default scale is centimeters
	_scale = 1.f;

	// add the services
#ifdef EMBER_SB_STEER
	_serviceManager->addService(_steerManager);
#endif
	_serviceManager->addService(_physicsManager);
#ifdef EMBER_SB_BONEBUS
	_serviceManager->addService(_boneBusManager);
#endif
	_serviceManager->addService(_collisionManager);
#ifdef EMBER_SB_VHMSG
	_serviceManager->addService(_vhmsgManager);
#endif
#ifdef EMBER_SB_WSP
	_serviceManager->addService(_wspManager);
#endif

	_parser = new SBParser();

#ifdef EMBER_SB_DEBUGGER
	_debuggerServer = new SBDebuggerServer();
	_debuggerClient = new SBDebuggerClient();
	_debuggerUtility = new SBDebuggerUtility();
#endif
	_isRemoteMode = false;
   _isCameraLocked = false;

	createBoolAttribute("internalAudio",false,true,"",10,false,false,false,"Use SmartBody's internal audio player.");
	createStringAttribute("speechRelaySoundCacheDir","../../../..",true,"",20,false,false,false,"Directory where sound files from speech relays will be placed. ");
	createDoubleAttribute("scale",1.f,true,"",30,false,false,false,"The scale of scene (1 = meters, .01 = centimeters, etc).");
	createIntAttribute("colladaTrimFrames",0,true,"",40,false,false,false,"Number of frames to be trimmed in the front when loading a collada motion.");
	createBoolAttribute("useFastCOLLADAParsing",true,true,"",50,false,false,false,"Use faster parsing when reading COLLADA from a file.");
	createBoolAttribute("useFastSpeechParsing",false,true,"",50,false,false,false,"Use faster parsing when reading speech from a file.");
	createBoolAttribute("delaySpeechIfNeeded",true,true,"",60,false,false,false,"Delays any speech until other behaviors specified in the same BML need to execute beforehand. This can occur when a gesture is synchronized to a word early in the utterance, and the gesture motion needs to be played for awhile before the synch point.");
	createBoolAttribute("useXMLCache",false,true,"",500,false,false,false,"Cache the XML used when processing audio files.");
	createBoolAttribute("useXMLCacheAuto",false,true,"",510,false,false,false,"Automatically add the XML to the cache when processing audio files after playing for the first time.");
	createStringAttribute("defaultCharacter","",true,"",550,false,false,false,"Default character when processing BML.");
	createStringAttribute("defaultRecipient","ALL",true,"",550,false,false,false,"Default recipient when processing BML.");
	createIntAttribute("queuedCommandsIndex",1,true,"",560,false,false,false,"Unique identifier when executing sequence commands.");
	createIntAttribute("bmlIndex",1,true,"",560,false,false,false,"Unique identifier when executing BML commands.");
	createBoolAttribute("enableConsoleLogging",true,true,"",70,false,false,false,"Use SmartBody's internal audio player.");
	createBoolAttribute("warnDeprecatedCommands",false,true,"",600,false,false,false,"Shows a warning when a deprecated command is being used.");
	createBoolAttribute("enableFaceShader",false,true,"",70,false,false,false,"Enable the face shader for higher quality rendering.");
	createBoolAttribute("enableExportProcessedBML",false,true,"",80,false,false,false,"Enable exporting of bml after realization.");
	createBoolAttribute("enableExportProcessedBMLLOG",false,true,"",81,false,false,false,"Enable log for exporting of bml after realization.");
	createStringAttribute("processedBMLPath","./processedXML.xml",true,"",90,false,false,false,"path of exported bml file after realization.");

	createIntAttribute("shadowMapSize", 1024, true, "RenderingParameters", 100, false, false, false, "Size of the shadow map");
	createIntAttribute("shadowMapCount", 1, true, "RenderingParameters", 110, false, false, false, "Num of the shadow maps");

	vhcl::Log::g_log.RemoveAllListeners();
	ForwardLogListener* forwardListener = new ForwardLogListener();
	vhcl::Log::g_log.AddListener(forwardListener);

	//consoleAttr->setValue(true); // set up the console logging
	
	_mediaPath = ".";
		// re-initialize
	// initialize everything

#ifdef EMBER_SB_VIEWER
	_viewer = NULL;
	_ogreViewer = NULL;
	_viewerFactory = NULL;
	_ogreViewerFactory = NULL;
#endif
	
	_rootGroup = new SrSnGroup();
	_rootGroup->ref();

#ifdef EMBER_SB_HEIGHTFIELD
	_heightField = NULL;
#endif
#ifdef EMBER_SB_STEER
	_navigationMesh = NULL;
#endif
#ifdef EMBER_SB_KINECT
	_kinectProcessor = new KinectProcessor();
#endif

	// Create default settings
	createDefaultControllers();
	
	/*
	SmartBody::SBSceneListener* listener = getCharacterListener();
	//_scene = SmartBody::SBScene::getScene();
	addSceneListener(listener);
	*/

#ifdef EMBER_SB_DEBUGGER
	_debuggerServer->Init();
	_debuggerServer->SetSBScene(_scene);
#endif
	getBlendManager()->createBlend0D(PseudoIdleState);

	// reset timer & viewer window
	getSimulationManager()->reset();
//	getSimulationManager()->start();

	/*
#ifndef __native_client__
	SrViewer* viewer = SmartBody::getViewer();
	if (viewer)
		viewer->show_viewer();
#endif

	command("vhmsgconnect");
#ifndef __native_client__
	//Py_Finalize();
	//initPython(initPythonLibPath);
#ifndef SB_NO_PYTHON
	PyRun_SimpleString("scene = getScene()");
	PyRun_SimpleString("bml = scene.getBmlProcessor()");
	PyRun_SimpleString("sim = scene.getSimulationManager()");
#endif
#endif
	*/	
/*	if (_viewer)	
	{
		if (_viewerFactory)
			_viewerFactory->reset(_viewer);
		_viewer = NULL;
#if !defined (__ANDROID__) && !defined(SB_IPHONE) && !defined(__native_client__)
		SbmShaderManager::singleton().setViewer(NULL);
#endif
	}

	if (_viewerFactory)
		_viewerFactory->remove(_ogreViewer);
	if (_ogreViewer)
	{
		delete _ogreViewer;
		_ogreViewer = NULL;
	}
*/
	_logListener = NULL;


}

void SBScene::cleanup()
{
	// stop the simulation
	getSimulationManager()->stop();
	
	// reset the simulation parameters
	getSimulationManager()->setSimFps(0);

	// remove the characters
	removeAllCharacters();
	
	// remove the pawns
	removeAllPawns();

	// clear the joint maps
	getJointMapManager()->removeAllJointMaps();

	// remove all blends and transitions
	getBlendManager()->removeAllBlends();
	getBlendManager()->removeAllTransitions();

	// always need a PseudoIdle state
	//SmartBody::SBAnimationBlend0D* idleState = getBlendManager()->createBlend0D(PseudoIdleState);
	//addPABlend(idleState);


	// clear out the default face definitions
	std::vector<std::string> faceDefinitions = getFaceDefinitionNames();

	for (std::vector<std::string>::iterator iter = faceDefinitions.begin();
		 iter != faceDefinitions.end();
		 iter++)
	{
		std::string faceName = (*iter);
		removeFaceDefinition(faceName);
	}

	// stop the services
	SBServiceManager* serviceManager = getServiceManager();
	std::vector<std::string> serviceNames =  serviceManager->getServiceNames();
	for (std::vector<std::string>::iterator iter = serviceNames.begin();
		 iter != serviceNames.end();
		 iter++)
	{
		SBService* service = serviceManager->getService(*iter);
		service->stop();
	}

	removePendingCommands();

	clearAttributes();

	removeDefaultControllers();

	removeAllAssetPaths("script");
	removeAllAssetPaths("motion");
	removeAllAssetPaths("mesh");
	removeAllAssetPaths("audio");	

	delete _sim;
	delete _profiler;
	delete _bml;
	delete _blendManager;
	delete _reachManager;
#ifdef EMBER_SB_STEER
	delete _steerManager;
#endif
	delete _serviceManager;
	delete _physicsManager;
	delete _gestureMapManager;
	delete _jointMapManager;
#ifdef EMBER_SB_BONEBUS
	delete _boneBusManager;
#endif
	delete _collisionManager;
	delete _diphoneManager;
	delete _behaviorSetManager;
	delete _retargetManager;
	delete _eventManager;
	delete _assetManager;
	delete _speechManager;
	delete _commandManager;
#ifdef EMBER_SB_WSP
	delete _wspManager;
#endif
#ifdef EMBER_SB_STEER
	delete _naviMeshManager;
#endif
#ifdef EMBER_SB_KINECT
	delete _kinectProcessor;
#endif

	_sim = NULL;
	_profiler = NULL;
	_bml = NULL;
	_blendManager = NULL;
	_reachManager = NULL;
#ifdef EMBER_SB_STEER
	_steerManager= NULL;
#endif
	_serviceManager = NULL;
	_physicsManager = NULL;
	_gestureMapManager= NULL;
	_jointMapManager = NULL;
#ifdef EMBER_SB_BONEBUS
	_boneBusManager = NULL;
#endif
	_collisionManager = NULL;
	_diphoneManager = NULL;
	_behaviorSetManager = NULL;
	_retargetManager = NULL;
	_eventManager = NULL;
	_assetManager = NULL;
	_commandManager = NULL;
	_speechManager = NULL;
#ifdef EMBER_SB_WSP
	_wspManager = NULL;
#endif
#ifdef EMBER_SB_KINECT
	_kinectProcessor = NULL;
#endif

#ifdef EMBER_SB_CAMERA
	_cameraTracking.clear();
#endif
	
#ifdef EMBER_SB_HEIGHTFIELD
	if (_heightField)
	{
		delete _heightField;
	}
	_heightField = NULL;
#endif

#if 0 // this should be done in asset manager
	if (_navigationMesh)
	{
		delete _navigationMesh;
	}
#endif
#ifdef EMBER_SB_STEER
	_navigationMesh = NULL;
#endif

	_rootGroup->unref();
	_rootGroup = NULL;

#ifdef EMBER_SB_VIEWER
	_viewer = NULL;
	_ogreViewer = NULL;
	_viewerFactory = NULL;
	_ogreViewerFactory = NULL;
#endif
	
#ifdef EMBER_SB_AUDIO
	AUDIO_Close();
	AUDIO_Init();
#endif

#ifdef EMBER_SB_VHMSG
	if (_vhmsgManager->isEnable())
		_vhmsgManager->send( "vrProcEnd sbm" );
	
	delete _vhmsgManager;	
#endif

#ifdef EMBER_SB_GPU
#if !defined(SB_IPHONE)
	SbmTextureManager::destroy_singleton();
#endif
#endif

#ifdef EMBER_SB_PYTHON
#ifndef SB_NO_PYTHON
//	Py_Finalize();

#if defined(WIN_BUILD)
	{
		// According to the python docs, .pyd files are not unloaded during Py_Finalize().
		// This causes issues when trying to re-load the smartbody dll over and over.
		// So, we force unload these .pyd files.  This list is all the standard .pyd files included in the Python26 DLLs folder.
		// For reference:  http://docs.python.org/2/c-api/init.html  "Dynamically loaded extension modules loaded by Python are not unloaded"

		// initPythonLibPath - eg:  "../../../../core/smartbody/Python26/Lib"
		std::string pythonLibPath = Py_GetPythonHome();
		HMODULE hmodule;
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/bz2.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/pyexpat.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/select.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/unicodedata.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/winsound.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_bsddb.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_ctypes.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_ctypes_test.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_elementtree.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_hashlib.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_msi.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_multiprocessing.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_socket.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_sqlite3.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_ssl.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_testcapi.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
		hmodule = GetModuleHandle(vhcl::Format("%s/../DLLs/_tkinter.pyd", pythonLibPath.c_str()).c_str());
		FreeLibrary(hmodule);
	}
#endif  // WIN_BUILD
#endif  // USE_PYTHON
#endif
}

SBScene::~SBScene(void)
{
	cleanup();
	for (std::map<std::string, SBScript*>::iterator iter = _scripts.begin();
		 iter != _scripts.end();
		 iter++)
	{
	//	delete (*iter).second;
	}

	delete _sim;
	delete _profiler;
	delete _bml;
	delete _blendManager;
	delete _reachManager;
#ifdef EMBER_SB_STEER
	delete _steerManager;
#endif
	delete _physicsManager;
#ifdef EMBER_SB_BONEBUS
	delete _boneBusManager;
#endif
	delete _collisionManager;
	delete _gestureMapManager;
	delete _jointMapManager;
	delete _diphoneManager;
	delete _behaviorSetManager;
	delete _serviceManager;
	delete _eventManager;

	delete _parser;

#ifdef EMBER_SB_DEBUGGER
	_debuggerClient->Disconnect();
	_debuggerServer->Close();
	delete _debuggerServer;  // TODO: should delete these in reverse order?
	delete _debuggerClient;
	delete _debuggerUtility;
#endif
//	
	//mcu.reset();
}

#ifdef EMBER_SB_DEBUGGER
SBDebuggerServer* SBScene::getDebuggerServer()
{
	return _debuggerServer; 
}

SBDebuggerClient* SBScene::getDebuggerClient()
{
	return _debuggerClient; 
}


SBDebuggerUtility* SBScene::getDebuggerUtility()
{
	return _debuggerUtility; 
}
#endif

SBScene* SBScene::getScene()
{
	if (_firstTime)
	{
		XMLPlatformUtils::Initialize(); 
		_firstTime = false;
		_scene = new SBScene();
		_scene->initialize();
	}

	return _scene;
}
 
void SBScene::destroyScene()
{
	if (_scene)
	{
		XMLPlatformUtils::Terminate(); 
		delete _scene;
		_scene = NULL;
		_firstTime = true;
	}
}

void SBScene::setProcessId(const std::string& id)
{
	_processId = id;
}

const std::string& SBScene::getProcessId()
{
	return _processId;
}

void SBScene::update()
{
	// scripts
	std::map<std::string, SmartBody::SBScript*>& scripts = getScripts();
	for (std::map<std::string, SmartBody::SBScript*>::iterator iter = scripts.begin();
		iter != scripts.end();
		iter++)
	{
		if ((*iter).second->isEnable())
			(*iter).second->beforeUpdate(getSimulationManager()->getTime());
	}

	// services
	std::map<std::string, SmartBody::SBService*>& services = getServiceManager()->getServices();
	for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
		iter != services.end();
		iter++)
	{
		if ((*iter).second->isEnable())
			(*iter).second->beforeUpdate(getSimulationManager()->getTime());
	}

// 	if (physicsEngine && physicsEngine->getBoolAttribute("enable"))
// 	{		
// 		float dt = (float)physicsEngine->getDoubleAttribute("dT");//timeStep*0.03f;
// 		//elapseTime += time_dt;
// 		while (physicsTime < this->time)		
// 		//if (physicsTime < this->time)
// 		{
// 			//printf("elapse time = %f\n",elapseTime);
// 			physicsEngine->updateSimulation(dt);
// 			physicsTime += dt;
// 			//curDt -= dt;
// 		}		
// 	}
// 	else
// 	{
// 		physicsTime = this->time;
// 	}

	std::string seqName = "";
	std::vector<std::string> sequencesToDelete;
	SequenceManager* activeSequences = getCommandManager()->getActiveSequences();
	int numSequences = activeSequences->getNumSequences();
	for (int s = 0; s < numSequences; s++)
	{
		srCmdSeq* seq = activeSequences->getSequence(s, seqName);
		std::string cmd = "";
		if (seq && seq->isValid())
		{
			do {
				cmd = seq->pop( (float) getSimulationManager()->getTime() );
				if (cmd != "")			
				{
					//LOG("execute command = %s",cmd);
					int err = getCommandManager()->execute( (char*)  cmd.c_str() );
					if( err != CMD_SUCCESS )
					{
						LOG( "update ERR: execute FAILED: '%s'\n", cmd.c_str() );
					}
				} 
			} while( cmd != "" );
			if( seq->get_count() < 1 )
			{
				sequencesToDelete.push_back(seqName);
			}
		}
	}

	for (size_t d = 0; d < sequencesToDelete.size(); d++)
	{
		activeSequences->removeSequence(sequencesToDelete[d], true);
	}
	activeSequences->cleanupMarkedSequences();

	const std::vector<std::string>& pawns = SmartBody::SBScene::getScene()->getPawnNames();
	for (std::vector<std::string>::const_iterator pawnIter = pawns.begin();
		pawnIter != pawns.end();
		pawnIter++)
	{
		SBPawn* pawn = SmartBody::SBScene::getScene()->getPawn((*pawnIter));
		pawn->reset_all_channels();
		pawn->ct_tree_p->evaluate( getSimulationManager()->getTime() );
		pawn->ct_tree_p->applyBufferToAllSkeletons();

// 		if (pawn->hasPhysicsSim() && SBPhysicsSim::getPhysicsEngine()->getBoolAttribute("enable"))
// 		{
// 			//pawn->updateFromColObject();
// 		}
// 		else
		{			
			//pawn->updateToColObject();
			pawn->updateToSteeringSpaceObject();
		}

		SbmCharacter* char_p = getCharacter(pawn->getName().c_str() );
		if( char_p ) {

			// run the minibrain, if available
			SmartBody::MiniBrain* brain = char_p->getMiniBrain();
			if (brain)
			{
				SmartBody::SBCharacter* sbchar = dynamic_cast<SmartBody::SBCharacter*>(char_p);
				brain->update(sbchar, getSimulationManager()->getTime(), getSimulationManager()->getTimeDt());
			}

			// scene update moved to renderer
			//if (char_p->scene_p)
			//	char_p->scene_p->update();
			//char_p->dMesh_p->update();
			//char_p->updateJointPhyObjs();
			/*
			bool hasPhySim = physicsEngine->getBoolAttribute("enable");
			char_p->updateJointPhyObjs(hasPhySim);
			//char_p->updateJointPhyObjs(false);
			*/
			char_p->_skeleton->update_global_matrices();

			char_p->forward_visemes( getSimulationManager()->getTime() );	
			//char_p->forward_parameters( getSimulationManager()->getTime() );	
		}  // end of char_p processing
	} // end of loop

	const std::vector<std::string>& pawnNames = getPawnNames();
	for (std::vector<std::string>::const_iterator iter = pawnNames.begin();
		iter != pawnNames.end();
		iter++)
	{
		SBPawn* pawn = getPawn(*iter);
		pawn->afterUpdate(getSimulationManager()->getTime());
	}


	std::vector<SmartBody::SBSceneListener*>& listeners = this->getSceneListeners();
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->OnSimulationUpdate( );
	}


	for (std::map<std::string, SmartBody::SBScript*>::iterator iter = scripts.begin();
		iter != scripts.end();
		iter++)
	{
		if ((*iter).second->isEnable())
			(*iter).second->update(getSimulationManager()->getTime());
	}

	for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
		iter != services.end();
		iter++)
	{
		(*iter).second->update(getSimulationManager()->getTime());
	}

	// scripts
	for (std::map<std::string, SmartBody::SBScript*>::iterator iter = scripts.begin();
		iter != scripts.end();
		iter++)
	{
		if ((*iter).second->isEnable())
			(*iter).second->afterUpdate(getSimulationManager()->getTime());
	}

	// services
	for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
		iter != services.end();
		iter++)
	{
		if ((*iter).second->isEnable())
			(*iter).second->afterUpdate(getSimulationManager()->getTime());
	}

}



void SBScene::setScale(float val)
{
	_scale = val;

	DoubleAttribute* scaleAttribute = dynamic_cast<DoubleAttribute*>(getAttribute("scale"));
	scaleAttribute->setValueFast(_scale);
}

float SBScene::getScale()
{
	return _scale;
}

void SBScene::reset()
{
	cleanup();
	initialize();	
}

void SBScene::notify( SBSubject* subject )
{
	BoolAttribute* boolAttr = dynamic_cast<BoolAttribute*>(subject);
#ifdef EMBER_SB_AUDIO
	if (boolAttr && boolAttr->getName() == "internalAudio")
	{
		bool val = boolAttr->getValue();
		if (!val)
		{
			AUDIO_Close();
		}
		else
		{
			AUDIO_Init();
		}
		return;
	}
	else
#endif
	if (boolAttr && boolAttr->getName() == "enableConsoleLogging")
	{
		bool val = boolAttr->getValue();
		if (val)
		{
			if (vhcl::Log::g_log.IsEnabled())
				return;

			vhcl::Log::StdoutListener* listener = new vhcl::Log::StdoutListener();
			vhcl::Log::g_log.AddListener(listener);
		}
		else
		{		
			vhcl::Log::g_log.RemoveAllListeners();
		}
	}

	DoubleAttribute* doubleAttr = dynamic_cast<DoubleAttribute*>(subject);
	if (doubleAttr && doubleAttr->getName() == "scale")
	{
		setScale((float) doubleAttr->getValue());
		return;
	}
}


SBAPI SBCharacter* SBScene::copyCharacter( const std::string& origCharName, const std::string& copyCharName )
{
	SmartBody::SBCharacter* origChar = getCharacter(origCharName);
	if (!origChar)
	{
		LOG("Character '%s' does not exists !", origCharName.c_str());
		return NULL;
	}
	else
	{
		SmartBody::SBCharacter* copyChar = createCharacter(copyCharName,"");
		if (!copyChar)
		{
			LOG("Can not copy to existing character '%s'",copyCharName.c_str());
			return NULL;
		}
		// successfully create a new character
		SmartBody::SBSkeleton* sk = new SmartBody::SBSkeleton(origChar->getSkeleton());
		copyChar->setSkeleton(sk);
		copyChar->createStandardControllers();
		copyChar->copy(origChar);
	#ifdef EMBER_SB_STEER
		SmartBody::SBSteerManager* steerManager = getSteerManager();
		SmartBody::SBSteerAgent* origAgent = steerManager->getSteerAgent(origCharName);
		if (origAgent) // the original character has steering
		{
			SmartBody::SBSteerAgent* agent = steerManager->createSteerAgent(copyCharName);
			agent->setSteerType(origAgent->getSteerType());
			agent->setSteerStateNamePrefix(origAgent->getSteerStateNamePrefix());
			bool steerEnable = steerManager->isEnable();
			if (steerEnable)
			{
				steerManager->setEnable(false);
				steerManager->setEnable(true);
			}			
		}
	#endif
		return copyChar;
	}
}


SBAPI SBPawn* SBScene::copyPawn( const std::string& origPawnName, const std::string& copyPawnName )
{
	SmartBody::SBPawn* origPawn = getPawn(origPawnName);
	if (!origPawn)
	{
		LOG("Pawn '%s' does not exists !", origPawnName.c_str());
		return NULL;
	}
	else
	{
		SmartBody::SBPawn* copyPawn = createPawn(copyPawnName);
		if (!copyPawn)
		{
			LOG("Can not copy to existing pawn '%s'",copyPawnName.c_str());
			return NULL;
		}
		// successfully create a new character
		SmartBody::SBSkeleton* sk = new SmartBody::SBSkeleton(origPawn->getSkeleton());
		copyPawn->setSkeleton(sk);		
		copyPawn->copy(origPawn);		
		return copyPawn;
	}
}

SBCharacter* SBScene::createCharacter(const std::string& charName, const std::string& metaInfo)
{	
	SmartBody::SBCharacter* character = SmartBody::SBScene::getScene()->getCharacter(charName);
	if (character)
	{
		LOG("Character '%s' already exists!", charName.c_str());
		return NULL;
	}
	else
	{
		SBCharacter* character = new SBCharacter(charName, metaInfo);
		
		std::map<std::string, SbmPawn*>::iterator iter = _pawnMap.find(character->getName());
		if (iter != _pawnMap.end())
		{
			LOG( "Register character: pawn_map.insert(..) '%s' FAILED\n", character->getName().c_str() );
			delete character;
			return NULL;
		}

		_pawnMap.insert(std::pair<std::string, SbmPawn*>(character->getName(), character));
		_pawnNames.push_back(character->getName());
	
		std::map<std::string, SbmCharacter*>::iterator citer = _characterMap.find(character->getName());
		if (citer != _characterMap.end())
		{
			LOG( "Register character: character_map.insert(..) '%s' FAILED\n", character->getName().c_str() );
			_pawnMap.erase(iter);
			delete character;
			return NULL;
		}
		_characterMap.insert(std::pair<std::string, SbmCharacter*>(character->getName(), character));
		_characterNames.push_back(character->getName());

		//if (getCharacterListener() )
		//	getCharacterListener()->OnCharacterCreate( character->getName().c_str(), character->getClassType() );
		SBSkeleton* skeleton = new SBSkeleton();		
		character->setSkeleton(skeleton);
//		SkJoint* joint = skeleton->add_joint(SkJoint::TypeQuat);
//		joint->setName("world_offset");		
//		joint->update_gmat();

#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
		if (getBoneBusManager()->isEnable())
			getBoneBusManager()->getBoneBus().CreateCharacter( character->getName().c_str(), character->getClassType().c_str(), true );
#endif
#endif

		std::vector<SmartBody::SBSceneListener*>& listeners = this->getSceneListeners();
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->OnCharacterCreate( character->getName().c_str(), character->getClassType() );
		}

		// notify the services		
		std::map<std::string, SmartBody::SBService*>& services = getServiceManager()->getServices();
		for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
			iter != services.end();
			iter++)
		{
			SBService* service = (*iter).second;
			service->onCharacterCreate(character);
		}		
		return character;
	}
}

SBPawn* SBScene::createPawn(const std::string& pawnName)
{
	SBPawn* pawn = getPawn(pawnName);
	SBCharacter* character = dynamic_cast<SBCharacter*>(pawn);
	if (character)
	{
		LOG("Pawn '%s' is a character.", pawnName.c_str());
		return NULL;
	}
	if (pawn)
	{
		LOG("Pawn '%s' already exists!", pawnName.c_str());
		return NULL;
	}
	else
	{
		SBPawn* pawn = new SBPawn(pawnName.c_str());
		SBSkeleton* skeleton = new SBSkeleton();
		pawn->setSkeleton(skeleton);
		//SkJoint* joint = skeleton->add_joint(SkJoint::TypeQuat);
		//joint->setName("world_offset");

		std::map<std::string, SbmPawn*>::iterator iter = _pawnMap.find(pawn->getName());
		if (iter != _pawnMap.end())
		{
			LOG( "Register pawn: pawn_map.insert(..) '%s' FAILED\n", pawn->getName().c_str() );
			delete pawn;
			return NULL;
		}

		_pawnMap.insert(std::pair<std::string, SbmPawn*>(pawn->getName(), pawn));
		_pawnNames.push_back(pawn->getName());
	

		std::vector<SmartBody::SBSceneListener*>& listeners = this->getSceneListeners();
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->OnPawnCreate( pawn->getName().c_str() );
		}

		// notify the services
		std::map<std::string, SmartBody::SBService*>& services = getServiceManager()->getServices();
		for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
			iter != services.end();
			iter++)
		{
			SBService* service = (*iter).second;
			service->onPawnCreate(character);
		}
		return pawn;
	}
}

void SBScene::removeCharacter(const std::string& charName)
{
	SBCharacter* character = this->getCharacter(charName);
	const std::string& name = character->getName();
	if (character)
	{

		string vrProcEnd_msg = "vrProcEnd sbm ";
		vrProcEnd_msg += getName();
	#ifdef EMBER_SB_VHMSG
		SmartBody::SBScene::getScene()->getVHMsgManager()->send( vrProcEnd_msg.c_str() );
	#endif

		// notify the services
		std::map<std::string, SmartBody::SBService*>& services = getServiceManager()->getServices();
		for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
			iter != services.end();
			iter++)
		{
			SBService* service = (*iter).second;
			service->onCharacterDelete(character);
		}
	
		std::vector<SmartBody::SBSceneListener*>& listeners = this->getSceneListeners();
		for (size_t i = 0; i < listeners.size(); i++)
		{
			listeners[i]->OnCharacterDelete( name);
		}

#ifdef EMBER_SB_BONEBUS
#ifndef SB_NO_BONEBUS
		if ( character->bonebusCharacter )
		{
			this->getBoneBusManager()->getBoneBus().DeleteCharacter(  character->bonebusCharacter );
			character->bonebusCharacter = NULL;
		}
#endif
#endif

		std::map<std::string, SbmPawn*>::iterator iter = _pawnMap.find(name);
		if (iter != _pawnMap.end())
		{
			_pawnMap.erase(iter);
		}
		for (std::vector<std::string>::iterator iter = _pawnNames.begin();
			 iter != _pawnNames.end();
			 iter++)
		{
			if (name == (*iter))
			{
				_pawnNames.erase(iter);
				break;
			}
		}

		std::map<std::string, SbmCharacter*>::iterator citer = _characterMap.find(name);
		if (citer != _characterMap.end())
		{
			_characterMap.erase(citer);
		}
		for (std::vector<std::string>::iterator iter = _characterNames.begin();
			 iter != _characterNames.end();
			 iter++)
		{
			if (name == (*iter))
			{
				_characterNames.erase(iter);
				break;
			}
		}

		delete character;
	}	
}

void SBScene::removeAllCharacters()
{
	std::vector<std::string> characters = getCharacterNames();
	for (std::vector<std::string>::const_iterator iter = characters.begin();
		 iter != characters.end();
		 iter++)
	{
		removeCharacter((*iter));
	}
	
}

void SBScene::removePawn(const std::string& pawnName)
{
	SbmPawn* pawn = SmartBody::SBScene::getScene()->getPawn(pawnName);
	if (pawn)
	{
		const std::string& name = pawn->getName();

		SbmCharacter* character = dynamic_cast<SbmCharacter*>(pawn);
		if (!character)
		{
			// notify the services
			std::map<std::string, SmartBody::SBService*>& services = getServiceManager()->getServices();
			for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
				iter != services.end();
				iter++)
			{
				SBService* service = (*iter).second;
				SBPawn* sbpawn = dynamic_cast<SBPawn*>(pawn);
				service->onPawnDelete(sbpawn);
			}
			
			std::vector<SmartBody::SBSceneListener*>& listeners = this->getSceneListeners();
			for (size_t i = 0; i < listeners.size(); i++)
			{
				listeners[i]->OnPawnDelete( name );
			}

			std::map<std::string, SbmPawn*>::iterator iter = _pawnMap.find(name);
			if (iter != _pawnMap.end())
			{
				_pawnMap.erase(iter);
			}
			for (std::vector<std::string>::iterator iter = _pawnNames.begin();
			 iter != _pawnNames.end();
			 iter++)
			{
				if (name == (*iter))
				{
					_pawnNames.erase(iter);
					break;
				}
			}

			delete pawn;
		}
	}	
}

void SBScene::removeAllPawns()
{
	std::vector<std::string> pawns = getPawnNames();
	for (std::vector<std::string>::const_iterator iter = pawns.begin();
		 iter != pawns.end();
		 iter++)
	{
		removePawn((*iter));
	}
	

	// clear the cameras
#ifdef EMBER_SB_CAMERA
	_cameras.clear();
#endif
}

int SBScene::getNumCharacters() 
{  
	 
	return _characterMap.size(); 
}

int SBScene::getNumPawns() 
{  
	return _pawnMap.size() - _characterMap.size(); 
}

SBPawn* SBScene::getPawn(const std::string& name)
{
	std::map<std::string, SbmPawn*>::iterator iter = _pawnMap.find(name);
	if (iter == _pawnMap.end())
	{
		return NULL;
	}
	else
	{
		SBPawn* sbpawn = dynamic_cast<SBPawn*>((*iter).second);
		return sbpawn;
	}
}

SBCharacter* SBScene::getCharacter(const std::string& name)
{
	std::map<std::string, SbmCharacter*>::iterator iter = _characterMap.find(name);
	if (iter == _characterMap.end())
	{
		return NULL;
	}
	else
	{
		SBCharacter* sbcharacter = dynamic_cast<SBCharacter*>((*iter).second);
		return sbcharacter;
	}
}

const std::vector<std::string>& SBScene::getPawnNames()
{
	return _pawnNames;
}

const std::vector<std::string>& SBScene::getCharacterNames()
{
	return _characterNames;
}

std::vector<std::string> SBScene::getEventHandlerNames()
{
	SBEventManager* eventManager = getEventManager();
	
	std::vector<std::string> ret;

	for(SBEventHandlerMap::iterator iter = eventManager->getEventHandlers().begin();
		iter != eventManager->getEventHandlers().end();
		iter++)
	{

		ret.push_back(std::string(iter->first));
	}
	return ret;
}


void SBScene::setMediaPath(const std::string& path)
{
	_mediaPath = path;
}

const std::string& SBScene::getMediaPath()
{
	return _mediaPath;
}

void SBScene::setDefaultCharacter(const std::string& character)
{
	SmartBody::SBScene::getScene()->setStringAttribute("defaultCharacter", character);
}

void SBScene::setDefaultRecipient(const std::string& recipient)
{
	SmartBody::SBScene::getScene()->setStringAttribute("defaultRecipient", recipient);
}

SBEventManager* SBScene::getEventManager()
{
	return _eventManager;
}


bool SBScene::command(const std::string& command)
{
	if (this->getBoolAttribute("warnDeprecatedCommands"))
	{
		LOG("Warning: Deprecated command [%s]", command.c_str());
	}

	int ret = getCommandManager()->execute((char*) command.c_str());

	if (ret == CMD_SUCCESS)
		return true;
	else
		return false;
}

bool SBScene::commandAt(float seconds, const std::string& command)
{
	if (this->getBoolAttribute("warnDeprecatedCommands"))
	{
		LOG("Warning: Deprecated command [%f] [%s]", seconds, command.c_str());
	}

	int ret = getCommandManager()->execute_later((char*) command.c_str(), seconds);

	if (ret == CMD_SUCCESS)
		return true;
	else
		return false;
}

void SBScene::removePendingCommands()
{
	SmartBody::SBScene::getScene()->getCommandManager()->getActiveSequences()->clear();
	SmartBody::SBScene::getScene()->getCommandManager()->getPendingSequences()->clear();
}

#ifdef EMBER_SB_VHMSG
void SBScene::sendVHMsg(const std::string& message)
{	 
	SmartBody::SBScene::getScene()->getVHMsgManager()->send(message.c_str());
}

void SBScene::sendVHMsg2(const std::string& message, const std::string& message2)
{
	 
	SmartBody::SBScene::getScene()->getVHMsgManager()->send2(message.c_str(), message2.c_str());
}
#endif

#ifdef EMBER_SB_PYTHON
bool SBScene::run(const std::string& command)
{
#ifndef SB_NO_PYTHON
	try {
		//LOG("executePython = %s",command);

		PyRun_SimpleString(command.c_str());
		//LOG("cmd result = %d",result);

		return true;
	} catch (...) {
		PyErr_Print();
		return false;
	}
#endif
	return true;
}

bool SBScene::runScript(const std::string& script)
{
#ifndef SB_NO_PYTHON
	// add the .seq extension if necessary
	std::string candidateSeqName = script;
	if (candidateSeqName.find(".py") == std::string::npos)
	{
		candidateSeqName.append(".py");
	}

	std::string curFilename = SmartBody::SBScene::getScene()->getAssetManager()->findFileName("script", candidateSeqName);
	if (curFilename != "")
	{
		try {
			std::stringstream strstr;
			strstr << "execfile(\"" << curFilename << "\")";
			PyRun_SimpleString(strstr.str().c_str());
			PyErr_Print();
			PyErr_Clear();
			return true;
		} catch (...) {
			PyErr_Print();
			return false;
		}
	}

	LOG("Could not find Python script '%s'", script.c_str());
	return false;

#endif
	return true;
}
#endif

SBSimulationManager* SBScene::getSimulationManager()
{
	return _sim;
}

SBProfiler* SBScene::getProfiler()
{
	return _profiler;
}

SBBmlProcessor* SBScene::getBmlProcessor()
{
	return _bml;
}

SBAnimationBlendManager* SBScene::getBlendManager()
{
	return _blendManager;
}

SBReachManager* SBScene::getReachManager()
{
	return _reachManager;
}

#ifdef EMBER_SB_STEER
SBSteerManager* SBScene::getSteerManager()
{
	return _steerManager;
}
#endif

SBServiceManager* SBScene::getServiceManager()
{
	return _serviceManager;
}


SBCollisionManager* SBScene::getCollisionManager()
{
	return _collisionManager;
}

SBDiphoneManager* SBScene::getDiphoneManager()
{
	return _diphoneManager;
}

SBBehaviorSetManager* SBScene::getBehaviorSetManager()
{
	return _behaviorSetManager;
}

SBMotionGraphManager* SBScene::getMotionGraphManager()
{
	return _motionGraphManager;
}

SBRetargetManager* SBScene::getRetargetManager()
{
	return _retargetManager;
}

SBAssetManager* SBScene::getAssetManager()
{
	return _assetManager;
}

SBSpeechManager* SBScene::getSpeechManager()
{
	return _speechManager;
}

SBPhysicsManager* SBScene::getPhysicsManager()
{
	return _physicsManager;
}

#ifdef EMBER_SB_BONEBUS
SBBoneBusManager* SBScene::getBoneBusManager()
{
	return _boneBusManager;
}
#endif

SBGestureMapManager* SBScene::getGestureMapManager()
{
	return _gestureMapManager;
}

SBJointMapManager* SBScene::getJointMapManager()
{
	return _jointMapManager;
}

SBCommandManager* SBScene::getCommandManager()
{
	return _commandManager;
}

#ifdef EMBER_SB_WSP
SBWSPManager* SBScene::getWSPManager()
{
	return _wspManager;
}
#endif

#ifdef EMBER_SB_STEER
SBNavigationMeshManager* SBScene::getNavigationMeshManager()
{
	return _naviMeshManager;
}
#endif

#ifdef EMBER_SB_VHMSG
SBVHMsgManager* SBScene::getVHMsgManager()
{
	return _vhmsgManager;
}
#endif

SBParser* SBScene::getParser()
{
	return _parser;
}

bool SBScene::isRemoteMode()	
{ 
	return _isRemoteMode; 
}

void SBScene::setRemoteMode(bool val)	
{ 
	_isRemoteMode = val; 
}

SmartBody::SBFaceDefinition* SBScene::createFaceDefinition(const std::string& name)
{
	// make sure the name doesn't already exist
	if (_faceDefinitions.find(name) != _faceDefinitions.end())
	{
		LOG("Face definition named '%s' already exists. Returning existing one", name.c_str());
		return _faceDefinitions[name];
	}

	SBFaceDefinition* face = new SBFaceDefinition(name);
	_faceDefinitions.insert(std::pair<std::string, SBFaceDefinition*>(name, face));
	for (size_t l = 0; l < this->_sceneListeners.size(); l++)
	{
		this->_sceneListeners[l]->OnObjectCreate(face);
	}

	return face;
}

void SBScene::removeFaceDefinition(const std::string& name)
{
	

	// make sure the name doesn't already exist
	std::map<std::string, SBFaceDefinition*>::iterator iter = _faceDefinitions.find(name);
	if (iter ==_faceDefinitions.end())
	{
		LOG("Face definition named '%s' does not exist.", name.c_str());
		return;
	}
	for (size_t l = 0; l < this->_sceneListeners.size(); l++)
	{
		this->_sceneListeners[l]->OnObjectDelete(iter->second);
	}
	delete iter->second;
	iter->second = NULL;
	_faceDefinitions.erase(iter);

}

SmartBody::SBFaceDefinition* SBScene::getFaceDefinition(const std::string& name)
{
	// make sure the name doesn't already exist
	std::map<std::string, SBFaceDefinition*>::iterator iter = _faceDefinitions.find(name);
	if (iter == _faceDefinitions.end())
	{
		LOG("Face definition named '%s' does not exist.", name.c_str());
		return NULL;
	}

	return (*iter).second;
}

int SBScene::getNumFaceDefinitions()
{
	return 	_faceDefinitions.size();
}

std::vector<std::string> SBScene::getFaceDefinitionNames()
{
	std::vector<std::string> faces;
	for (std::map<std::string, SBFaceDefinition*>::iterator iter =  _faceDefinitions.begin();
		 iter !=  _faceDefinitions.end();
		 iter++)
	{
		faces.push_back((*iter).second->getName());
	}

	return faces;
}

void SBScene::addScript(const std::string& name, SBScript* script)
{
	std::map<std::string, SBScript*>::iterator iter = _scripts.find(name);
	if (iter != _scripts.end())
	{
		LOG("Script with name %s already exists. Remove current script.", name.c_str());
		//return;
		_scripts.erase(iter);
	}
	script->setName(name);

	_scripts.insert(std::pair<std::string, SBScript*>(name, script));
}

void SBScene::removeScript(const std::string& name)
{
	std::map<std::string, SBScript*>::iterator iter = _scripts.find(name);
	if (iter != _scripts.end())
	{
		_scripts.erase(iter);
		return;
	}
	LOG("Script with name %s does not exist.", name.c_str());

}

int SBScene::getNumScripts()
{
	return _scripts.size();
}

std::vector<std::string> SBScene::getScriptNames()
{
	std::vector<std::string> scriptNames;

	for (std::map<std::string, SBScript*>::iterator iter = _scripts.begin();
		 iter != _scripts.end();
		 iter++)
	{
		scriptNames.push_back((*iter).first);
	}

	return scriptNames;

}

SBScript* SBScene::getScript(const std::string& name)
{
	std::map<std::string, SBScript*>::iterator iter = _scripts.find(name);
	if (iter == _scripts.end())
	{
		LOG("Script with name %s already exists.", name.c_str());
		return NULL;
	}

	return (*iter).second;
}

std::map<std::string, SBScript*>& SBScene::getScripts()
{
	return _scripts;
}

void SBScene::addSceneListener(SBSceneListener* listener)
{
	std::vector<SBSceneListener*>::iterator iter = std::find(_sceneListeners.begin(), _sceneListeners.end(), listener);
	if (iter == _sceneListeners.end())
		_sceneListeners.push_back(listener);
}

void SBScene::removeSceneListener(SBSceneListener* listener)
{
	std::vector<SBSceneListener*>::iterator iter = std::find(_sceneListeners.begin(), _sceneListeners.end(), listener);
	if (iter != _sceneListeners.end())
		_sceneListeners.erase(iter);
}

void SBScene::removeAllSceneListeners()
{
	_sceneListeners.clear();
}

std::vector<SBSceneListener*>& SBScene::getSceneListeners()
{
	return _sceneListeners;
}

std::string SBScene::saveSceneSetting()
{
	std::stringstream strstr;
	strstr << "# Autogenerated by SmartBody\n";

#ifdef EMBER_SB_CAMERA
	// save all default cameras
	std::vector<std::string> cameras = getCameraNames();
	for (std::vector<std::string>::iterator cameraIter = cameras.begin();
		cameraIter != cameras.end();
		cameraIter++)
	{
		if (*cameraIter == "cameraDefault")
			continue; // don't save default camera
		SrCamera* camera = getCamera((*cameraIter));
		strstr << "obj = scene.getCamera(\"" << camera->getName() << "\")\n";
		strstr << "if obj == None:\n";
		strstr << "\tobj = scene.createCamera(\"" << camera->getName() << "\")\n";		
		strstr << "obj.setEye(" << camera->getEye().x << ", " << camera->getEye().y << ", " << camera->getEye().z << ")\n";
		strstr << "obj.setCenter(" << camera->getCenter().x << ", " << camera->getCenter().y << ", " << camera->getCenter().z << ")\n";
		strstr << "obj.setUpVector(SrVec(" << camera->getUpVector().x << ", " << camera->getUpVector().y << ", " << camera->getUpVector().z << "))\n";
		strstr << "obj.setScale(" << camera->getScale() << ")\n";
		strstr << "obj.setFov(" << camera->getFov() << ")\n";
		strstr << "obj.setFarPlane(" << camera->getFarPlane() << ")\n";
		strstr << "obj.setNearPlane(" << camera->getNearPlane() << ")\n";
		strstr << "obj.setAspectRatio(" << camera->getAspectRatio() << ")\n";

		std::vector<std::string> attributeNames = camera->getAttributeNames();
		for (std::vector<std::string>::iterator iter = attributeNames.begin();
			iter != attributeNames.end();
			iter++)
		{
			SmartBody::SBAttribute* attr = camera->getAttribute((*iter));
			std::string attrWrite = attr->write();
			strstr << attrWrite;
		}
	}	
#endif

	// save all pawns (including light pawns) position & orientation.
	const std::vector<std::string>& pawns = getPawnNames();
	for (std::vector<std::string>::const_iterator pawnIter = pawns.begin();
		pawnIter != pawns.end();
		pawnIter++)
	{
		SBPawn* pawn = getPawn((*pawnIter));
	#ifdef EMBER_SB_CAMERA
		SrCamera* camera = dynamic_cast<SrCamera*>(pawn);
		if (camera)
			continue; // already wrote out pawns
	#endif
		strstr << "\n# ---- pawn: " << pawn->getName() << "\n";
		strstr << "obj = scene.getPawn(\"" << pawn->getName() << "\")\n";
		strstr << "if obj == None:\n";
		strstr << "\tobj = scene.createPawn(\"" << pawn->getName() << "\")\n";
		SrVec position = pawn->getPosition();
		strstr << "obj.setPosition(SrVec(" << position[0] << ", " << position[1] << ", " << position[2] << "))\n";
		SrVec hpr = pawn->getHPR();
		strstr << "obj.setHPR(SrVec(" << hpr[0] << ", " << hpr[1] << ", " << hpr[2] << "))\n";
		// attributes
		std::vector<std::string> attributeNames = pawn->getAttributeNames();
		for (std::vector<std::string>::iterator iter = attributeNames.begin();
			iter != attributeNames.end();
			iter++)
		{
			SmartBody::SBAttribute* attr = pawn->getAttribute((*iter));
			std::string attrWrite = attr->write();
			strstr << attrWrite;
		}
	}

	// restore all character position/orientation
	const std::vector<std::string>& characters = getCharacterNames();
	for (std::vector<std::string>::const_iterator characterIter = characters.begin();
		characterIter != characters.end();
		characterIter++)
	{
		SBCharacter* character = getCharacter((*characterIter));
		strstr << "\n# ---- character: " << character->getName() << "\n";
		strstr << "obj = scene.getCharacter(\"" << character->getName() << "\")\n";
		strstr << "if obj != None:\n";				
		SrVec position = character->getPosition();
		strstr << "\tobj.setPosition(SrVec(" << position[0] << ", " << position[1] << ", " << position[2] << "))\n";
		SrVec hpr = character->getHPR();
		strstr << "\tobj.setHPR(SrVec(" << hpr[0] << ", " << hpr[1] << ", " << hpr[2] << "))\n";
	}

	return strstr.str();
}

std::string SBScene::save(bool remoteSetup, std::string mediaPath)
{
	std::stringstream strstr;
	strstr << "# Autogenerated by SmartBody\n";

	saveScene(strstr, remoteSetup);
	saveAssets(strstr, remoteSetup, mediaPath);
#ifdef EMBER_SB_CAMERA
	saveCameras(strstr, remoteSetup);
#endif
#ifdef EMBER_SB_LIGHT
	saveLights(strstr, remoteSetup);
#endif
	saveFaceDefinitions(strstr, remoteSetup);
	//saveJointMaps(strstr, remoteSetup); // need to apply joint map before apply any motion operations
	saveLipSyncing(strstr, remoteSetup);
	saveBlends(strstr, remoteSetup);
	saveRetargets(strstr, remoteSetup);
	saveGestureMaps(strstr, remoteSetup);
	savePawns(strstr, remoteSetup);
	saveCharacters(strstr, remoteSetup);
	saveServices(strstr, remoteSetup);

	return strstr.str();
}

std::string SBScene::exportScene(const std::vector<std::string>& aspects, std::string mediaPath , bool remoteSetup)
{
	std::stringstream strstr;
	strstr << "# Autogenerated by SmartBody\n";
	
	std::set<std::string> set;
	for (std::vector<std::string>::const_iterator iter = aspects.begin();
		iter != aspects.end(); iter++)
	{
		set.insert(*iter);
	}

	if (set.find("scene") != set.end())
		saveScene(strstr, remoteSetup);
	if (set.find("assets") != set.end())
		saveAssets(strstr, remoteSetup, "");
#ifdef EMBER_SB_CAMERA
	if (set.find("cameras") != set.end())
		saveCameras(strstr, remoteSetup);
#endif
#ifdef EMBER_SB_LIGHT
	if (set.find("lights") != set.end())
		saveLights(strstr, remoteSetup);
#endif
	if (set.find("face definitions") != set.end())
		saveFaceDefinitions(strstr, remoteSetup);
	//if (set.find("joint maps") != set.end())
	//	saveJointMaps(strstr, remoteSetup);
	if (set.find("lip syncing") != set.end())
		saveLipSyncing(strstr, remoteSetup);
	if (set.find("blends and transitions") != set.end())
		saveBlends(strstr, remoteSetup);
	if (set.find("retargets") != set.end())
		saveRetargets(strstr, remoteSetup);
	if (set.find("gesture maps") != set.end())
		saveGestureMaps(strstr, remoteSetup);
	if (set.find("pawns") != set.end())
		savePawns(strstr, remoteSetup);
	if (set.find("characters") != set.end())
		saveCharacters(strstr, remoteSetup);
	if (set.find("services") != set.end())
		saveServices(strstr, remoteSetup);	

	return strstr.str();
}


void SBScene::saveScene(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- cameras\n";
	strstr << "scene.setScale(" << getScale() << ")\n";

	strstr << "# -------------------- scene\n";
	strstr << "obj = scene\n";
	// scene attributes
	std::vector<std::string> attributeNames = this->getAttributeNames();
	for (std::vector<std::string>::iterator iter = attributeNames.begin();
			iter != attributeNames.end();
			iter++)
	{
		SmartBody::SBAttribute* attr = this->getAttribute((*iter));
		std::string attrWrite = attr->write();
		strstr << attrWrite;
	}
	
}

void SBScene::saveRetargets( std::stringstream& strstr, bool remoteSetup )
{	
	strstr << "# -------------------- on-line retargeting setup\n";
	strstr << "print \"Save Retargets\"\n";
	if (remoteSetup) // don't save retarget instance for remote setup
		return;

	SmartBody::SBRetargetManager* retargetManager = getRetargetManager();
	strstr << "retargetManager = scene.getRetargetManager()\n";
	std::vector<StringPair> retargetNames = retargetManager->getRetargetNames();
	for (unsigned int i=0;i<retargetNames.size();i++)
	{
		StringPair& sp = retargetNames[i];
		SmartBody::SBRetarget* retarget = retargetManager->getRetarget(sp.first, sp.second);
		strstr << "retarget = retargetManager.getRetarget(\"" << sp.first << "\",\"" << sp.second << "\")\n";
		strstr << "if retarget == None:\n";
		strstr << "\tretarget = retargetManager.createRetarget(\"" << sp.first << "\",\"" << sp.second << "\")\n";
		std::vector<std::string> endJoints = retarget->getEndJointNames();
		std::vector<std::string> relativeJoints = retarget->getRelativeJointNames();
		strstr << "endJoints = StringVec()\n";
		for (unsigned int k=0;k<endJoints.size();k++)
			strstr << "endJoints.append(\"" << endJoints[k] << "\")\n";
		strstr << "relativeJoints = StringVec()\n";
		for (unsigned int k=0;k<endJoints.size();k++)
			strstr << "relativeJoints.append(\"" << relativeJoints[k] << "\")\n";
		
		strstr << "retarget.initRetarget(endJoints, relativeJoints)\n";
	}
}


#if (BOOST_VERSION > 104400)
bool copyDir(boost::filesystem::path const & source,boost::filesystem::path const & destination)
{
	namespace fs = boost::filesystem;
#else
bool copyDir(boost::filesystem2::path const & source,boost::filesystem2::path const & destination)
{
	namespace fs = boost::filesystem2;
#endif
	try
	{
		// Check whether the function call is valid
		if(
			!fs::exists(source) ||
			!fs::is_directory(source)
			)
		{
			std::cerr << "Source directory " << source.string()
				<< " does not exist or is not a directory." << std::endl
				;
			return false;
		}
		if(fs::exists(destination))
		{
			// still try to overwrite ?
// 			std::cerr << "Destination directory " << destination.string()
// 				<< " already exists." << std::endl
// 				;
// 			return false;
		}
		else if(!fs::create_directories(destination)) // create destination dir
		{
			std::cerr << "Unable to create destination directory"
				<< destination.string() << std::endl
				;
			return false;
		}
	}
	catch(fs::filesystem_error const & e)
	{
		std::cerr << e.what() << std::endl;
		return false;
	}
	// Iterate through the source directory
	for(
		fs::directory_iterator file(source);
		file != fs::directory_iterator(); ++file
		)
	{
		try
		{
			fs::path current(file->path());
			if(fs::is_directory(current))
			{
				// Found directory: Recursion
				if(
					!copyDir(
					current,
					destination / current.filename()
					)
					)
				{
					return false;
				}
			}
			else if (!fs::exists(destination / current.filename()))
			{
				// Found file: Copy
				fs::copy_file(
					current,
					destination / current.filename()
					);
			}
		}
		catch(fs::filesystem_error const & e)
		{
			std:: cerr << e.what() << std::endl;
		}
	}
	return true;
}


#if (BOOST_VERSION > 104400)
bool find_file(const boost::filesystem::path&  dir_path, const std::string & file_name, boost::filesystem::path& path_found) {
		using boost::filesystem::path;
		using boost::filesystem::directory_iterator;
		//using boost::filesystem::dot;
		//using boost::filesystem::slash;
#else
bool find_file(const boost::filesystem2::path&  dir_path, const std::string & file_name, boost::filesystem2::path& path_found) { 
		using boost::filesystem2::path;
		using boost::filesystem2::directory_iterator;
		//using boost::filesystem2::dot;
		//using boost::filesystem2::slash;
#endif

	if ( !exists( dir_path ) ) return false;
	directory_iterator end_itr; // default construction yields past-the-end
	for ( directory_iterator itr( dir_path );
		itr != end_itr;
		++itr )
	{
		if ( is_directory(itr->status()) )
		{
			if ( find_file( itr->path(), file_name, path_found ) ) return true;
		}
		else if ( itr->path().filename() == file_name ) // see below
		{
			path_found = itr->path();
			return true;
		}
	}
	return false;
}

/**
 * https://svn.boost.org/trac/boost/ticket/1976#comment:2
 * 
 * "The idea: uncomplete(/foo/new, /foo/bar) => ../new
 *  The use case for this is any time you get a full path (from an open dialog, perhaps)
 *  and want to store a relative path so that the group of files can be moved to a different
 *  directory without breaking the paths. An IDE would be a simple example, so that the
 *  project file could be safely checked out of subversion."
 * 
 * ALGORITHM:
 *  iterate path and base
 * compare all elements so far of path and base
 * whilst they are the same, no write to output
 * when they change, or one runs out:
 *   write to output, ../ times the number of remaining elements in base
 *   write to output, the remaining elements in path
 */
#if (BOOST_VERSION > 104400)
boost::filesystem::path
naive_uncomplete(boost::filesystem::path const p, boost::filesystem::path const base) {
    using boost::filesystem::path;
    //using boost::filesystem::dot;
    //using boost::filesystem::slash;
#else
boost::filesystem2::path
naive_uncomplete(boost::filesystem2::path const p, boost::filesystem2::path const base) {
    using boost::filesystem2::path;
    //using boost::filesystem2::dot;
    //using boost::filesystem2::slash;
#endif


    if (p == base)
        return "./";
        /*!! this breaks stuff if path is a filename rather than a directory,
             which it most likely is... but then base shouldn't be a filename so... */

    path from_path, from_base, output;

    path::iterator path_it = p.begin(),    path_end = p.end();
    path::iterator base_it = base.begin(), base_end = base.end();

    // check for emptiness
    if ((path_it == path_end) || (base_it == base_end))
        throw std::runtime_error("path or base was empty; couldn't generate relative path");

#ifdef WIN32
    // drive letters are different; don't generate a relative path
    if (*path_it != *base_it)
        return p;

    // now advance past drive letters; relative paths should only go up
    // to the root of the drive and not past it
    ++path_it, ++base_it;
#endif

    // Cache system-dependent dot, double-dot and slash strings
    const std::string _dot  = ".";//std::string(1, dot<path>::value);
    const std::string _dots = "..";//std::string(2, dot<path>::value);
    const std::string _sep = "/";//std::string(1, slash<path>::value);

    // iterate over path and base
    while (true) {

        // compare all elements so far of path and base to find greatest common root;
        // when elements of path and base differ, or run out:
        if ((path_it == path_end) || (base_it == base_end) || (*path_it != *base_it)) {

            // write to output, ../ times the number of remaining elements in base;
            // this is how far we've had to come down the tree from base to get to the common root
            for (; base_it != base_end; ++base_it) {
                if (*base_it == _dot)
                    continue;
                else if (*base_it == _sep)
                    continue;

                output /= "../";
            }

            // write to output, the remaining elements in path;
            // this is the path relative from the common root
            path::iterator path_it_start = path_it;
            for (; path_it != path_end; ++path_it) {

                if (path_it != path_it_start)
                    output /= "/";

                if (*path_it == _dot)
                    continue;
                if (*path_it == _sep)
                    continue;

                output /= *path_it;
            }

            break;
        }

        // add directory level to both paths and continue iteration
        from_path /= path(*path_it);
        from_base /= path(*base_it);

        ++path_it, ++base_it;
    }

    return output;
}

#ifdef EMBER_SB_ZLIB
#if !defined(__FLASHPLAYER__)
void writeFileToZip(zipFile& zf, std::string readFileName, std::string fileNameInZip)
{
	//LOG("writeFileToZip, srcFile = %s, fileInZip = %s",readFileName.c_str(),fileNameInZip.c_str());
	const int bufferSize = 16384;
	int size_buf = bufferSize;
	void* buf = NULL; 
	buf = (void*)malloc(size_buf);	
	zip_fileinfo zi;
	zi.tmz_date.tm_sec = zi.tmz_date.tm_min = zi.tmz_date.tm_hour =	zi.tmz_date.tm_mday = zi.tmz_date.tm_mon = zi.tmz_date.tm_year = 0;
	zi.dosDate = 0;
	zi.internal_fa = 0;
	zi.external_fa = 0;
	FILE* fp = fopen(readFileName.c_str(),"rb");	
	int err = zipOpenNewFileInZip(zf,fileNameInZip.c_str(),&zi,NULL,0,NULL,0,NULL,0,Z_DEFAULT_COMPRESSION);
	int size_read = 0;
	do
	{
		err = ZIP_OK;
		size_read = (int)fread(buf,1,size_buf,fp);
		if (size_read>0)
		{
			err = zipWriteInFileInZip (zf,buf,size_read);
		}

	} while ((err == ZIP_OK) && (size_read>0));
	err = zipCloseFileInZip(zf);
	fclose(fp);
	free(buf);
}

void writeDirToZip(zipFile& zf, std::string sourceFolder, std::string folderNameInZip)
{
#if (BOOST_VERSION > 104400)	
		namespace fs = boost::filesystem;
#else	
		namespace fs = boost::filesystem2;
#endif
	fs::path source(sourceFolder);
	for(
		fs::directory_iterator file(source);
		file != fs::directory_iterator(); ++file
		)
	{
		try
		{
			fs::path current(file->path());
#if (BOOST_VERSION > 104400)
			std::string curFileName = current.filename().string();
#else
			std::string curFileName = current.filename();
#endif
			if(fs::is_directory(current))
			{
				// Found directory: Recursion
				writeDirToZip(zf,current.string(), folderNameInZip + "/" + curFileName);				
			}
			else
			{
				// Found file: Copy
				writeFileToZip(zf, current.string(), folderNameInZip + "/" + curFileName);					
			}
		}
		catch(fs::filesystem_error const & e)
		{
			std:: cerr << e.what() << std::endl;
		}
	}
	
}


void SBScene::exportCharacter( std::string charName, std::string outDir )
{
	SmartBody::SBCharacter* sbChar = getCharacter(charName);
	if (!sbChar) return;
#if (BOOST_VERSION > 104400)	
	namespace fs =  boost::filesystem;
	//using boost::filesystem::dot;
	//using boost::filesystem::slash;
#else	
	namespace fs = boost::filesystem2;
	//using boost::filesystem2::dot;
	//using boost::filesystem2::slash;
#endif
	fs::path newOutPath(outDir);
	if (!fs::exists(newOutPath))
	{
		fs::create_directories(newOutPath);
	}
	std::vector<std::string> motions;	
	ParserOpenCOLLADA::exportCollada(outDir, sbChar->getSkeleton()->getName(), sbChar->getStringAttribute("deformableMesh"), motions, true, true, false);

}

void SBScene::exportScenePackage( std::string outDir, std::string outZipArchiveName )
{
	std::string mediaPath = getMediaPath();
	std::vector<std::string> motionNames = getMotionNames();
	std::vector<std::string> skelNames = getSkeletonNames();	


#if (BOOST_VERSION > 104400)	
	namespace fs =  boost::filesystem;
	//using boost::filesystem::dot;
	//using boost::filesystem::slash;
#else	
	namespace fs = boost::filesystem2;
	//using boost::filesystem2::dot;
	//using boost::filesystem2::slash;
#endif

	bool writeToZip = false;
	if (outZipArchiveName != "")
		writeToZip = true;
		
	fs::path newOutPath(outDir);
	fs::path mePath(mediaPath);
	fs::path tempPath(mePath.parent_path().string()+"/tempExportFiles");
	if (!fs::exists(newOutPath))
	{
		fs::create_directories(newOutPath);
	}

	if (!fs::exists(tempPath))
	{
		fs::create_directories(tempPath);
	}

	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	std::string fileString = scene->save(false,"__FILE__");
	std::string initScriptFile = "initScene.py";
	std::string scriptFileLocation = outDir + "/" + initScriptFile;
	std::ofstream file(scriptFileLocation.c_str());
	if (file.good())
	{
		file << fileString;
		file.close();
	}
	else
	{
		LOG("Fail to save scene. Abort export operation");
		file.close();
		return;
	}
	
	zipFile zf;	
	if (writeToZip)
	{
		zf = zipOpen((outDir+"/"+outZipArchiveName).c_str(),0);		
		writeFileToZip(zf,scriptFileLocation,initScriptFile);
	}		

	SmartBody::SBAssetManager* assetManager = getAssetManager();
	for (unsigned int i=0;i<motionNames.size();i++)
	{
		SmartBody::SBMotion* motion = getMotion(motionNames[i]);
		if (!motion)
		{
			LOG("Motion %s cannot be found.", motionNames[i].c_str());
			continue;
		}
		if (motion->getTransformDepth() > 0) // not an original motion
			continue;

		if (motion->getFullFilePath() == "")
		{
			std::string motionFullPath = tempPath.string()+ "/" + motion->getName() + ".skm";
			LOG("Skeleton %s is not loaded from a file, save the skeleton to temp directory %s",motion->getName().c_str(), motionFullPath.c_str());
			motion->saveToSkm(motionFullPath);
			motion->setFullFilePath(motionFullPath);
		}

		fs::path motionFile = fs::path(motion->getFullFilePath());	
		fs::path motionPath = motionFile.parent_path();
		std::string motionFileExt = fs::extension(motionFile);
		std::string assetLocation = assetManager->findAsset("motion",motion->getName()+motionFileExt);
		
		fs::path diffPath;	
		fs::path outPath;
		diffPath = naive_uncomplete(motionPath,mePath);
		
		if (tempPath == motionPath)
		{
			diffPath = fs::path(defautAssetPath);			
		}
		else if (assetLocation == "")
		{
			fs::path foundPath;
			if (!find_file(mePath,motionFile.filename().string(),foundPath)) // only set to defaultAssetPath if can not find the file under media directory
				diffPath = fs::path(defautAssetPath);			
		}
// 		else if (assetLocation != "")
// 		{
// 			fs::path mePath(mediaPath);
// 			fs::path assetPath(assetLocation);			
// 			diffPath = naive_uncomplete(assetPath,mePath);			
// 		}
		outPath = fs::path(outDir + "/" + diffPath.string());

		if (!writeToZip)
		{
			if (!fs::exists(outPath))
			{
				fs::create_directories(outPath);
			}
#if (BOOST_VERSION > 104400)
			std::string newFileName = outPath.string()+"/"+motionFile.filename().string();
#else
			std::string newFileName = outPath.string()+"/"+motionFile.filename();
#endif
			if (!fs::exists(newFileName))
				fs::copy_file(motionFile,fs::path(newFileName));
		}
		else
		{
#if (BOOST_VERSION > 104400)
			std::string zipFileName = diffPath.string()+"/"+motionFile.filename().string();
#else
			std::string zipFileName = diffPath.string()+"/"+motionFile.filename();
#endif			
			writeFileToZip(zf, motionFile.string(), zipFileName);
		}	
	}

	for (unsigned int i=0;i<skelNames.size();i++)
	{
		SmartBody::SBSkeleton* skel = getSkeleton(skelNames[i]);
		if (!skel)
		{
			LOG("Skeleton %s cannot be found.", skelNames[i].c_str());
			continue;
		}	
		
		if (skel->getFullFilePath() == "")
		{
			std::string skelFullPath = tempPath.string()+ "/" + skel->getName();
			LOG("Skeleton %s is not loaded from a file, save the skeleton to temp directory %s",skel->getName().c_str(), skelFullPath.c_str());
			skel->save(skelFullPath);
			skel->setFullFilePath(skelFullPath);
		}
				
		fs::path skelFile = fs::path(skel->getFullFilePath());
		fs::path skelDir = skelFile.parent_path();		
		std::string assetLocation = assetManager->findAsset("motion",skel->getName());
		fs::path diffPath;	
		fs::path outPath;
		diffPath = naive_uncomplete(skelDir,mePath);
		//LOG("diffPath after uncomplete = '%s'",diffPath.string().c_str());
		if (tempPath == skelDir)
		{
			diffPath = fs::path(defautAssetPath);			
		}
		else if (assetLocation == "")
		{
			fs::path foundPath;
			if (!find_file(mePath,skelFile.filename().string(),foundPath)) // only set to defaultAssetPath if can not find the file under media directory
				diffPath = fs::path(defautAssetPath);			
		}
// 		else if (assetLocation != "")
// 		{
// 			fs::path mePath(mediaPath);
// 			fs::path assetPath(assetLocation);			
// 			diffPath = naive_uncomplete(assetPath,mePath);			
// 		}
		//LOG("final diffpath = '%s'",diffPath.string().c_str());
		outPath = fs::path(outDir + "/" + diffPath.string());
		//LOG("skeleton = %s, file = %s, outpath = %s", skel->getName().c_str(), skelFile.string().c_str(), outPath.string().c_str());
		if (!writeToZip)
		{
			if (!fs::exists(outPath))
			{
				fs::create_directories(outPath);
			}
#if (BOOST_VERSION > 104400)
			std::string newFileName = outPath.string()+"/"+skelFile.filename().string();
#else
			std::string newFileName = outPath.string()+"/"+skelFile.filename();
#endif
			if (!fs::exists(newFileName))
				fs::copy_file(skelFile,fs::path(newFileName));
		}
		else
		{
#if (BOOST_VERSION > 104400)
			std::string zipFileName = diffPath.string()+"/"+skelFile.filename().string();
#else
			std::string zipFileName = diffPath.string()+"/"+skelFile.filename();
#endif			
			writeFileToZip(zf, skelFile.string(), zipFileName);
		}	
	}
	//LOG("finish copying skeleton files");
	std::vector<std::string> deformableMeshNames = assetManager->getDeformableMeshNames();
	for (unsigned int i=0;i<deformableMeshNames.size();i++)
	{
		std::string meshName = deformableMeshNames[i];
		DeformableMesh* mesh = assetManager->getDeformableMesh(meshName);
		if (!mesh)
		{
			LOG("Mesh %s cannot be found.", meshName.c_str());
			continue;
		}
		
		std::string meshBaseName = boost::filesystem::basename(meshName);

		if (mesh->getFullFilePath() == "") // the mesh is not loaded from a file, so write it to a temp directory first
		//if (true)
		{
			std::vector<std::string> moNames;
			std::string meshSkName = mesh->skeletonName;
			std::string exportMeshFullPath = tempPath.string() + "/" + meshBaseName;
			fs::path meshPath(exportMeshFullPath);
			if (!fs::exists(meshPath))
			{
				fs::create_directories(meshPath);
			}
			LOG("mesh %s is not loaded from a file. Export the mesh to %s",meshName.c_str(),exportMeshFullPath.c_str());
			ParserOpenCOLLADA::exportCollada(exportMeshFullPath,meshSkName,meshName,moNames,true,true,false);
			//LOG("finish exporting mesh as Collada files");
			mesh->setFullFilePath(exportMeshFullPath +  "/" + meshName);
			//continue;
		}

		fs::path meshFile = fs::path(mesh->getFullFilePath());
		fs::path meshDir = meshFile.parent_path();		
		std::string assetLocation = assetManager->findAsset("mesh",meshName);
		fs::path diffPath, outPath;	

		diffPath = naive_uncomplete(meshDir,mePath);

		if (tempPath == meshDir.parent_path())
		{
			diffPath = fs::path(defautAssetPath + "/" + meshBaseName);			
		}
		else if (assetLocation == "")
		{
			fs::path foundPath;
			if (!find_file(mePath,meshFile.filename().string(),foundPath)) // only set to defaultAssetPath if can not find the file under media directory
				diffPath = fs::path(defautAssetPath + "/" + meshBaseName);			
		}	
		
		outPath = fs::path(outDir + "/" + diffPath.string());

		if (!writeToZip)
		{
			copyDir(meshDir,outPath);						
		}
		else
		{
			writeDirToZip(zf,meshDir.string(),diffPath.string());
		}
	}
	//LOG("finish copying deformable mesh files");
	/*
	const std::vector<std::string>& characters = getCharacterNames();
	for (unsigned int i=0;i<characters.size();i++)
	{
		std::string charName = characters[i];
		SmartBody::SBCharacter* sbChar = getCharacter(charName);
		if (!sbChar) continue; 

		std::string meshName = sbChar->getStringAttribute("deformableMesh");		
		if (meshName == "") // otherwise we will copy all mesh directories ...
			continue;
		boost::filesystem::path path(meshName);
		std::string meshDir = boost::filesystem::basename(path);

		std::vector<std::string> meshPaths = SmartBody::SBScene::getScene()->getAssetManager()->getAssetPaths("mesh");
		bool hasMeshPath = false;
		for (size_t m = 0; m < meshPaths.size(); m++)
		{
			std::string meshPath = meshPaths[m];
			fs::path curpath( meshPath + "/" + meshDir );
			if (!boost::filesystem::is_directory(curpath))
			{

				// should we write out the mesh instead ?
				//continue;
			}
			//curpath /= std::string(meshDir);	

			fs::path mePath(mediaPath);		
			fs::path diffPath = naive_uncomplete(curpath,mePath);
			fs::path newMeshPath(outDir+"/"+diffPath.string());

			//LOG("curPath = %s, newMeshPath = %s", curpath.string().c_str(), newMeshPath.string().c_str());
			//path targetPath()
			// copy dir
			hasMeshPath = true;
			if (!writeToZip)
			{
				copyDir(curpath,newMeshPath);						
			}
			else
			{
				writeDirToZip(zf,curpath.string(),diffPath.string());
			}
		}
	}
	*/

#if 0
	// save motions
	//LOG("Num of motionNames = %d",motionNames.size());
	for (unsigned int i=0;i<motionNames.size();i++)
	{
		SmartBody::SBMotion* motion = getMotion(motionNames[i]);
		//LOG("motionName = %s",motionNames[i].c_str());

		if (!motion)
		{
			LOG("Motion %s cannot be found.", motionNames[i].c_str());
			continue;
		}
	
		fs::path motionFile(motion->filename());			
		fs::path motionPath = motionFile.parent_path();
		//LOG("motion %d, motionName = %s, filename = %s",motionNames[i].c_str(), motion->filename().c_str());
		if (motionPath.empty()) // don't care about empty path
			continue;
		if (motion->getTransformDepth() > 0) // not an original motion
			continue;

		fs::path mePath(mediaPath);
		fs::path diffPath = naive_uncomplete(motionPath,mePath);

		if (diffPath.is_absolute()) // the target path is not under mePath
		{			
			diffPath = fs::path("Assets");
		}
		fs::path newPath(outDir+"/"+diffPath.string());	
		
		//LOG("motionpath = %s, mediapath = %s, diffpath = %s, filename = %s", motionFile.directory_string().c_str(), mePath.native_directory_string().c_str(), diffPath.string().c_str(), motionFile.filename().c_str());		
		//LOG("new Path = %s, newFileName = %s",newPath.string().c_str(), newFileName.c_str());
		//motion->saveToSkm(newFileName);
		if (!writeToZip)
		{
			if (!fs::exists(newPath))
			{
				fs::create_directories(newPath);
			}
#if (BOOST_VERSION > 104400)
			std::string newFileName = newPath.string()+"/"+motionFile.filename().string();
#else
			std::string newFileName = newPath.string()+"/"+motionFile.filename();
#endif
			fs::copy_file(motionFile,fs::path(newFileName));
		}
		else
		{
#if (BOOST_VERSION > 104400)
			std::string zipFileName = diffPath.string()+"/"+motionFile.filename().string();
#else
			std::string zipFileName = diffPath.string()+"/"+motionFile.filename();
#endif			
			writeFileToZip(zf, motionFile.string(), zipFileName);
		}		
	}

	// save skeletons
	for (unsigned int i=0;i<skelNames.size();i++)
	{
		SmartBody::SBSkeleton* skel = getSkeleton(skelNames[i]);
		fs::path skelFile(skel->getFileName());			
		fs::path skelPath = skelFile.parent_path();
		if (skelPath.empty()) // don't care about empty path
			continue;

		fs::path mePath(mediaPath);
		fs::path diffPath = naive_uncomplete(skelPath,mePath);
		if (diffPath.is_absolute()) // the target path is not under mePath
		{			
			diffPath = fs::path("Assets");
		}
		fs::path newPath(outDir+"/"+diffPath.string());		

		  //LOG("motionpath = %s, mediapath = %s, diffpath = %s", skelPath.directory_string().c_str(), mePath.directory_string().c_str(), diffPath.directory_string().c_str());
		//skel->save(newFileName);
		//copy_file(path(skelFile),path(newFileName));

		if (!writeToZip)
		{	
			if (!fs::exists(newPath))
			{
				fs::create_directories(newPath);
			}
#if (BOOST_VERSION > 104400)
			std::string newFileName = newPath.string()+"/"+skelFile.filename().string();
#else
			std::string newFileName = newPath.string()+"/"+skelFile.filename();
#endif

			fs::copy_file(skelFile,fs::path(newFileName));
		}
		else
		{
#if (BOOST_VERSION > 104400)
			std::string zipFileName = diffPath.string()+"/"+skelFile.filename().string();
#else
			std::string zipFileName = diffPath.string()+"/"+skelFile.filename();
#endif			
			writeFileToZip(zf, skelFile.string(), zipFileName);
		}		
	}
	
#if 1
	// save character meshes
	const std::vector<std::string>& characters = getCharacterNames();
	for (unsigned int i=0;i<characters.size();i++)
	{
		std::string charName = characters[i];
		SmartBody::SBCharacter* sbChar = getCharacter(charName);
		if (!sbChar) continue; 
		
		std::string meshName = sbChar->getStringAttribute("deformableMesh");		
		if (meshName == "") // otherwise we will copy all mesh directories ...
			continue;
		boost::filesystem::path path(meshName);
		std::string meshDir = boost::filesystem::basename(path);
		
		std::vector<std::string> meshPaths = SmartBody::SBScene::getScene()->getAssetManager()->getAssetPaths("mesh");
		bool hasMeshPath = false;
		for (size_t m = 0; m < meshPaths.size(); m++)
		{
			std::string meshPath = meshPaths[m];
			fs::path curpath( meshPath + "/" + meshDir );
			if (!boost::filesystem::is_directory(curpath))
			{

				// should we write out the mesh instead ?
				//continue;
			}
			//curpath /= std::string(meshDir);	

			fs::path mePath(mediaPath);		
			fs::path diffPath = naive_uncomplete(curpath,mePath);
			fs::path newMeshPath(outDir+"/"+diffPath.string());
			
			//LOG("curPath = %s, newMeshPath = %s", curpath.string().c_str(), newMeshPath.string().c_str());
			//path targetPath()
			// copy dir
			hasMeshPath = true;
			if (!writeToZip)
			{
				copyDir(curpath,newMeshPath);						
			}
			else
			{
				writeDirToZip(zf,curpath.string(),diffPath.string());
			}
		}
	}
#else
	std::vector<std::string> meshNames = assetManager->getDeformableMeshNames();
	for (unsigned int i=0;i<meshNames.size();i++)
	{
		std::string meshName = meshNames[i];
		DeformableMesh* mesh = assetManager->getDeformableMesh(meshName);
		std::vector<std::string> meshPaths = SmartBody::SBScene::getScene()->getAssetManager()->getAssetPaths("mesh");
		bool hasMeshPath = false;
		fs::path curpath, curMeshPath;
		for (size_t m = 0; m < meshPaths.size(); m++)
		{
			std::string meshPath = meshPaths[m];
			curpath = fs::path( meshPath + "/" + meshName );			
			if (boost::filesystem::is_directory(curpath))
			{	
				curMeshPath = curpath;
				hasMeshPath = true;
				break;
			}
		}	

		if (!hasMeshPath) // file does not exist
		{
			curMeshPath = fs::path(getMediaPath()+ "/temp/" + meshName); // create a temp directory to write out the mesh
			fs::create_directory(curMeshPath);
			ParserOgre::exportOgreXMLMesh(mesh, meshName, curMeshPath.string());
		}

		fs::path mePath(mediaPath);		
		fs::path diffPath = naive_uncomplete(curpath,mePath);
		fs::path newMeshPath(outDir+"/"+diffPath.string());

		if (!writeToZip)
		{
			copyDir(curMeshPath,newMeshPath);						
		}
		else
		{
			writeDirToZip(zf,curMeshPath.string(),diffPath.string());
		}
	}
#endif
#endif

	if (writeToZip)
	{
		int errClose = zipClose(zf,NULL);
		fs::path initScrptPath(scriptFileLocation);
		if (fs::exists(initScrptPath))
		{
			fs::remove(initScrptPath);			
		}
	}

	if (fs::exists(tempPath)) 
	{
		fs::remove_all(tempPath);
	}
}
#endif
#endif

void SBScene::saveAssets(std::stringstream& strstr, bool remoteSetup, std::string outMediaPath)
{
	strstr << "# -------------------- media and asset paths\n";
	// mediapath
	strstr << "print \"Save Assets\"\n";
	std::string systemMediaPath = getMediaPath();
	if (outMediaPath == "__FILE__")
	{
		strstr << "import inspect\n";
		strstr << "scriptFilename = inspect.getframeinfo(inspect.currentframe()).filename\n";
		strstr << "fileMediaPath = os.path.dirname(os.path.abspath(scriptFilename))\n";
		strstr << "scene.setMediaPath(fileMediaPath)\n";
	}
	else if (outMediaPath != "")
	{
		strstr << "scene.setMediaPath(\"" << outMediaPath << "\")\n";
	}
	else
	{
		strstr << "scene.setMediaPath(\"" << systemMediaPath << "\")\n";
	}
		
	// asset paths
	std::vector<std::string>::iterator iter;
	std::vector<std::string> motionPaths = getLocalAssetPaths("motion");
	std::vector<std::string> motionNames = getMotionNames();
	std::vector<std::string> skelNames = getSkeletonNames();
	std::set<std::string> extraAssetPathSet;

	//exportScenePackage("../../../../temp/","test1.zip");

#if (BOOST_VERSION > 104400)
    using boost::filesystem::path;
    //using boost::filesystem::dot;
    //using boost::filesystem::slash;
#else
    using boost::filesystem2::path;
    //using boost::filesystem2::dot;
    //using boost::filesystem2::slash;
#endif
	SmartBody::SBAssetManager* assetManager = getAssetManager();
	// feng : since we may have use "loadAssetsFromPath" to load the motions, we should infer all other motion paths from existing motions
	for (unsigned int i=0;i<motionNames.size();i++)
	{
		SmartBody::SBMotion* motion = getMotion(motionNames[i]);	
		path motionFile(motion->filename());			
		path motionPath = motionFile.parent_path();
		if (motionPath.empty()) // don't care about empty path
			continue;

		std::string ext = boost::filesystem::extension(motionFile);
		std::string base = boost::filesystem::basename(motionFile);
		std::string assetName = base+ext;
		std::string assetExist = assetManager->findAsset("motion",assetName);

		path mePath(systemMediaPath);
		path diffPath = naive_uncomplete(motionPath,mePath);
		if (diffPath.is_absolute()) // the path is not under media path, skip
		{
			continue;
		}
		//LOG("motionpath = %s, mediapath = %s, diffpath = %s", motionFile.directory_string().c_str(), mePath.string().c_str(), diffPath.string().c_str());
		std::vector<std::string>::iterator st = std::find(motionPaths.begin(),motionPaths.end(),diffPath.string());
		//if (st == motionPaths.end())
		if (assetExist == "") // can not find this asset in the motion path
		{
			extraAssetPathSet.insert(diffPath.string());
		}
	}

	for (unsigned int i=0;i<skelNames.size();i++)
	{
		SmartBody::SBSkeleton* skel = getSkeleton(skelNames[i]);
		path skelFile(skel->getFileName());			
		path skelPath = skelFile.parent_path();
		if (skelPath.empty()) // don't care about empty path
			continue;

		std::string ext = boost::filesystem::extension(skelFile);
		std::string base = boost::filesystem::basename(skelFile);
		std::string assetName = base+ext;
		std::string assetExist = assetManager->findAsset("motion",assetName);
		path mePath(systemMediaPath);
		path diffPath = naive_uncomplete(skelPath,mePath);
		//LOG("motionpath = %s, mediapath = %s, diffpath = %s", skelPath.directory_string().c_str(), mePath.directory_string().c_str(), diffPath.directory_string().c_str());
		if (diffPath.is_absolute()) // the path is not under media path, skip
		{
			continue;
		}
		std::vector<std::string>::iterator st = std::find(motionPaths.begin(),motionPaths.end(),diffPath.string());
		//if (st == motionPaths.end())
		if (assetExist == "") // can not find this asset in the skeleton path
		{
			extraAssetPathSet.insert(diffPath.string());
		}
	}

	for (iter = motionPaths.begin(); iter != motionPaths.end(); iter++)
	{ 
		const std::string& path = (*iter);
		strstr << "scene.addAssetPath(\"motion\", \"" << path << "\")\n";
	}
	
	std::vector<std::string> scriptPaths = getLocalAssetPaths("script");
	for (iter = scriptPaths.begin(); iter != scriptPaths.end(); iter++)
	{
		const std::string& path = (*iter);
		strstr << "scene.addAssetPath(\"script\", \"" << path << "\")\n";
	}

	std::vector<std::string> audioPaths = getLocalAssetPaths("audio");
	for (iter = audioPaths.begin(); iter != audioPaths.end(); iter++)
	{
		const std::string& path = (*iter);
		strstr << "scene.addAssetPath(\"audio\", \"" << path << "\")\n";
	}

	std::vector<std::string> meshPaths = getLocalAssetPaths("mesh");
	for (iter = meshPaths.begin(); iter != meshPaths.end(); iter++)
	{
		const std::string& path = (*iter);
		strstr << "scene.addAssetPath(\"mesh\", \"" << path << "\")\n";
	}
	if (remoteSetup) // to-do : different treatment when saving setup script for remote connection
	{
		// need to go through all skeleton, and explicitly create those skeletons from script
		std::vector<std::string> skeletonNames = getSkeletonNames();
		const std::vector<std::string>& charNames = getCharacterNames();
		std::map<std::string, std::string> charSkelMap;
		for (unsigned int i=0;i<skeletonNames.size();i++)
		{
			std::string skelName = skeletonNames[i];
			SBSkeleton* skel = getSkeleton(skelName);
			if (skel)
			{
				std::string skelStr = skel->saveToString();
				strstr << "tempSkel = scene.addSkeletonDefinition(\"" << skelName << "\")\n";	
				std::string skelSaveStr = skel->saveToString();
				//skelSaveStr.replace('\n',)
				boost::replace_all(skelSaveStr,"\n","\\n");
				boost::replace_all(skelSaveStr,"\"","");
				LOG("Skeleton %s :\n%s",skelName.c_str(),skelSaveStr.c_str());
				strstr << "tempSkel.loadFromString(\"" << skelSaveStr << "\")\n";
				charSkelMap[skelName] = skelName;
			}			
		}
		for (unsigned int i=0;i<charNames.size(); i++)
		{
			std::string charName = charNames[i];
			SBCharacter* sbChar = getCharacter(charName);
			if (!sbChar)
				continue;
			std::string skelName = sbChar->getSkeleton()->getName();
			SBSkeleton* skel = getSkeleton(skelName);
			if (skel && charSkelMap.find(skelName) == charSkelMap.end())
			{
				std::string skelStr = skel->saveToString();
				strstr << "tempSkel = scene.addSkeletonDefinition(\"" << skelName << "\")\n";	
				std::string skelSaveStr = skel->saveToString();
				//skelSaveStr.replace('\n',)
				boost::replace_all(skelSaveStr,"\n","\\n");
				boost::replace_all(skelSaveStr,"\"","");
				LOG("Skeleton %s :\n%s",skelName.c_str(),skelSaveStr.c_str());
				strstr << "tempSkel.loadFromString(\"" << skelSaveStr << "\")\n";
				charSkelMap[skelName] = charName;
			}
		}
		std::vector<std::string> motionNames = getMotionNames();
		for (unsigned int i=0;i<motionNames.size();i++)
		{
			std::string moName = motionNames[i];
			SBMotion* motion = getMotion(moName);
			if (motion)
			{
				// add motion definition
				strstr << "tempMotion = scene.createMotion(\"" << moName << "\")\n";	
				strstr << "tempMotion.setEmptyMotion(" << motion->getDuration() << ", " << motion->getNumFrames() << ")\n";
				// add sync points
				strstr << "tempMotion.setSyncPoint(\"start\"," << motion->getTimeStart() <<")\n";
				strstr << "tempMotion.setSyncPoint(\"ready\"," << motion->getTimeReady() <<")\n";
				strstr << "tempMotion.setSyncPoint(\"stroke_start\"," << motion->getTimeStrokeStart() <<")\n";
				strstr << "tempMotion.setSyncPoint(\"stroke\"," << motion->getTimeStroke() <<")\n";
				strstr << "tempMotion.setSyncPoint(\"stroke_stop\"," << motion->getTimeStrokeEnd() <<")\n";
				strstr << "tempMotion.setSyncPoint(\"relax\"," << motion->getTimeRelax() <<")\n";
				strstr << "tempMotion.setSyncPoint(\"stop\"," << motion->getTimeStop() <<")\n";				
				// add meta data tag
				BOOST_FOREACH(const std::string& tagName, motion->getMetaDataTags())
				{
					strstr << "tempMotion.addMetaData(\"" << tagName << "\",\"" << motion->getMetaDataString(tagName) << "\")\n";
				}
			}
		}
	}
	else // loading the actual assets
	{
		std::set<std::string>::iterator si;
		for ( si  = extraAssetPathSet.begin();
			si != extraAssetPathSet.end();
			si++)
		{
			const std::string& path = (*si);
			strstr << "scene.loadAssetsFromPath(\"" << path << "\")\n";
		}
		// load the extra assets in asset directory
		strstr << "scene.loadAssetsFromPath(\"" << defautAssetPath << "\")\n";
		strstr << "# -------------------- load assets\n";
		strstr << "scene.loadAssets()\n";
	}	

	// this is a hack to save joint map here, should probably move motion transformation into another function ?
	saveJointMaps(strstr, remoteSetup);
	// create any mirrored assets
	std::vector<std::string> motions = this->getMotionNames();
#if 0	
	for (std::vector<std::string>::iterator iter = motions.begin();
		 iter != motions.end();
		 iter++)
	{
		SBMotion* mirroredMotion = this->getMotion(*iter);
		StringAttribute* mirroredMotionAttr = dynamic_cast<StringAttribute*>(mirroredMotion->getAttribute("mirrorMotion"));	
		if (mirroredMotionAttr) // the motion is generated from mirroring
		{
			strstr << "motion = scene.getMotion(\"" << mirroredMotionAttr->getValue() << "\")\n";
			// make sure the skeleton exists
			StringAttribute* mirroredSkeletonAttr = dynamic_cast<StringAttribute*>(mirroredMotion->getAttribute("mirrorSkeleton"));			
			if (mirroredSkeletonAttr) 
			{
				const std::string& skeletonName = mirroredSkeletonAttr->getValue();
				strstr << "mirrorSkeleton = scene.getSkeleton(\"" << skeletonName << "\")\n";
				strstr << "if mirrorSkeleton is not None:\n";
				strstr << "\tmirroredMotion = motion.mirror(\"" << mirroredMotion->getName() << "\", \"" << skeletonName << "\")\n";
			}			
		}			
	}
#else
	std::vector<SmartBody::SBMotion*> motionList;
	for (unsigned int i=0;i<motions.size();i++)
		motionList.push_back(getMotion(motions[i]));
	// sort motion array according to its transformation depth level
	std::sort(motionList.begin(),motionList.end(),SmartBody::motionComp);
	// process motion from low depth to high depth to capture all dependency of motion operations
	for (unsigned int i=0;i<motionList.size();i++)
	{
		SBMotion* motion = motionList[i];
		StringAttribute* mirroredMotionAttr = dynamic_cast<StringAttribute*>(motion->getAttribute("mirrorMotion"));	
		StringAttribute* smoothedMotionAttr = dynamic_cast<StringAttribute*>(motion->getAttribute("smoothMotion"));	
		if (mirroredMotionAttr)
		{
			strstr << "motion = scene.getMotion(\"" << mirroredMotionAttr->getValue() << "\")\n";
			// make sure the skeleton exists
			StringAttribute* mirroredSkeletonAttr = dynamic_cast<StringAttribute*>(motion->getAttribute("mirrorSkeleton"));			
			if (mirroredSkeletonAttr) 
			{
				const std::string& skeletonName = mirroredSkeletonAttr->getValue();
				strstr << "mirrorSkeleton = scene.getSkeleton(\"" << skeletonName << "\")\n";
				strstr << "if mirrorSkeleton is not None:\n";
				strstr << "\tmirroredMotion = motion.mirror(\"" << motion->getName() << "\", \"" << skeletonName << "\")\n";
			}
		}
		else if (smoothedMotionAttr)
		{
			strstr << "motion = scene.getMotion(\"" << smoothedMotionAttr->getValue() << "\")\n";
			DoubleAttribute* smoothIntervalAttr = dynamic_cast<DoubleAttribute*>(motion->getAttribute("smoothInterval"));
			if (smoothIntervalAttr)
			{
				strstr << "smoothMotion = motion.smoothCycle(\"" << motion->getName() << "\", " << boost::lexical_cast<std::string>(smoothIntervalAttr->getValue()) << ")\n";
			}
		}
	}

	std::vector<SBSkeleton*> skeletonList;	
	for (unsigned int i=0;i<skelNames.size();i++)
	{
		std::string skName = skelNames[i];
		SBSkeleton* sbSk = getSkeleton(skName);
		// rescale the skeleton to right size if necessary
		strstr << "sbSk = scene.getSkeleton(\"" << skName << "\")\n";
		//strstr << "print 'numSkeleton = ' + str(scene.getNumSkeletons())\n";
		strstr << "if sbSk != None:\n";
		strstr << "\tsbSk.rescale(" << boost::lexical_cast<std::string>(sbSk->getScale()) << ")\n";
	}
	
#endif

	for (std::vector<std::string>::iterator iter = motions.begin();
		iter != motions.end();
		iter++)
	{
		SBMotion* motion = this->getMotion(*iter);
		strstr << "tempMotion = scene.getMotion(\"" << *iter << "\")\n";
		strstr << "tempMotion.setMotionSkeletonName(\"" << motion->getMotionSkeletonName() <<"\")\n";	
	}
	

}

#ifdef EMBER_SB_CAMERA
void SBScene::saveCameras(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- cameras\n";
	strstr << "print \"Save Cameras\"\n";
	// save all default cameras
	std::vector<std::string> cameras = getCameraNames();
	for (std::vector<std::string>::iterator cameraIter = cameras.begin();
		cameraIter != cameras.end();
		cameraIter++)
	{
		SrCamera* camera = getCamera((*cameraIter));
		strstr << "obj = scene.getCamera(\"" << camera->getName() << "\")\n";
		strstr << "if obj == None:\n";
		strstr << "\tobj = scene.createCamera(\"" << camera->getName() << "\")\n";		
		strstr << "obj.setEye(" << camera->getEye().x << ", " << camera->getEye().y << ", " << camera->getEye().z << ")\n";
		strstr << "obj.setCenter(" << camera->getCenter().x << ", " << camera->getCenter().y << ", " << camera->getCenter().z << ")\n";
		strstr << "obj.setUpVector(SrVec(" << camera->getUpVector().x << ", " << camera->getUpVector().y << ", " << camera->getUpVector().z << "))\n";
		strstr << "obj.setScale(" << camera->getScale() << ")\n";
		strstr << "obj.setFov(" << camera->getFov() << ")\n";
		strstr << "obj.setFarPlane(" << camera->getFarPlane() << ")\n";
		strstr << "obj.setNearPlane(" << camera->getNearPlane() << ")\n";
		strstr << "obj.setAspectRatio(" << camera->getAspectRatio() << ")\n";

		std::vector<std::string> attributeNames = camera->getAttributeNames();
		for (std::vector<std::string>::iterator iter = attributeNames.begin();
			iter != attributeNames.end();
			iter++)
		{
			SmartBody::SBAttribute* attr = camera->getAttribute((*iter));
			std::string attrWrite = attr->write();
			strstr << attrWrite;
		}
	}	
}
#endif

void SBScene::savePawns(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- pawns and characters\n";
	strstr << "print \"Save Pawns\"\n";
	// pawns
	const std::vector<std::string>& pawns = getPawnNames();
	for (std::vector<std::string>::const_iterator pawnIter = pawns.begin();
		 pawnIter != pawns.end();
		 pawnIter++)
	{
		SBPawn* pawn = getPawn((*pawnIter));
		SmartBody::SBCharacter* sbChar = dynamic_cast<SmartBody::SBCharacter*>(pawn);
		if (sbChar) // we will handle character in saveCharacters
			continue; 
	#ifdef EMBER_SB_CAMERA
		SrCamera* camera = dynamic_cast<SrCamera*>(pawn);
		if (camera)
			continue; // already wrote out pawns
	#endif
		if (pawn->getName().find("light") == 0)
		{
			// already wrote lights
			continue;
		}
		strstr << "\n# ---- pawn: " << pawn->getName() << "\n";
		strstr << "obj = scene.getPawn(\"" << pawn->getName() << "\")\n";
		strstr << "if obj == None:\n";
		strstr << "\tobj = scene.createPawn(\"" << pawn->getName() << "\")\n";
		SrVec position = pawn->getPosition();
		strstr << "obj.setPosition(SrVec(" << position[0] << ", " << position[1] << ", " << position[2] << "))\n";
		SrQuat orientation = pawn->getOrientation();
		strstr << "obj.setOrientation(SrQuat(" << orientation.w << ", " << orientation.x << ", " << orientation.y << ", " << orientation.z << "))\n";
		// attributes
		std::vector<std::string> attributeNames = pawn->getAttributeNames();
		for (std::vector<std::string>::iterator iter = attributeNames.begin();
			 iter != attributeNames.end();
			 iter++)
		{
			SmartBody::SBAttribute* attr = pawn->getAttribute((*iter));
			std::string attrWrite = attr->write();
			strstr << attrWrite;
		}	
	}
}

void SBScene::savePositions(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- pawn positions\n";
	strstr << "print \"Save Positions\"\n";
	const std::vector<std::string>& pawns = getPawnNames();
	for (std::vector<std::string>::const_iterator pawnIter = pawns.begin();
		 pawnIter != pawns.end();
		 pawnIter++)
	{
		SBPawn* pawn = getPawn((*pawnIter));
	#ifdef EMBER_SB_CAMERA
		SrCamera* camera = dynamic_cast<SrCamera*>(pawn);
		if (camera)
			continue; // already wrote out pawns
	#endif
		if (pawn->getName().find("light") == 0)
		{
			// already wrote lights
			continue;
		}
		strstr << "obj = scene.getPawn(\"" << pawn->getName() << "\")\n";
		SrVec position = pawn->getPosition();
		SrQuat orientation = pawn->getOrientation();
		strstr << "if obj is not None:\n";
		strstr << "\tobj.setPosition(SrVec(" << position[0] << ", " << position[1] << ", " << position[2] << "))\n";
		strstr << "\tobj.setOrientation(SrQuat(" << orientation.w << ", " << orientation.x << ", " << orientation.y << ". " << orientation.z << "))\n";
	}

	strstr << "# -------------------- character positions\n";
	const std::vector<std::string>& characters = getCharacterNames();
	for (std::vector<std::string>::const_iterator characterIter = characters.begin();
			characterIter != characters.end();
			characterIter++)
	{
		SBCharacter* character = getCharacter((*characterIter));
		SrVec position = character->getPosition();
		SrQuat orientation = character->getOrientation();
		strstr << "obj = scene.getCharacter(\"" << character->getName() << "\", \"" << character->getType() << "\")\n";
		strstr << "if obj is not None:\n";
		strstr << "\tobj.setPosition(SrVec(" << position[0] << ", " << position[1] << ", " << position[2] << "))\n";
		strstr << "\tobj.setOrientation(SrQuat(" << orientation.w << ", " << orientation.x << ", " << orientation.y << ". " << orientation.z << "))\n";
	}
}

void SBScene::saveCharacters(std::stringstream& strstr, bool remoteSetup)
{
	// characters
	strstr << "print \"Save Characters\"\n";
	const std::vector<std::string>& characters = getCharacterNames();
	for (std::vector<std::string>::const_iterator characterIter = characters.begin();
		 characterIter != characters.end();
		 characterIter++)
	{
		SBCharacter* character = getCharacter((*characterIter));
		strstr << "\n# ---- character: " << character->getName() << "\n";
		strstr << "obj = scene.getCharacter(\"" << character->getName() << "\")\n";
		strstr << "if obj == None:\n";
		strstr << "\tobj = scene.createCharacter(\"" << character->getName() << "\", \"" << character->getType() << "\")\n";
		strstr << "print 'character skeleton rescale'\n";
		strstr << "skeleton = scene.createSkeleton(\"" << character->getSkeleton()->getName() << "\")\n";
		strstr << "skeleton.rescale(" <<  character->getSkeleton()->getScale() << ")\n";
		strstr << "obj.setSkeleton(skeleton)\n";
		SrVec position = character->getPosition();
		strstr << "obj.setPosition(SrVec(" << position[0] << ", " << position[1] << ", " << position[2] << "))\n";
		SrQuat orientation = character->getOrientation();
		strstr << "obj.setOrientation(SrQuat(" << orientation.w << ", " << orientation.x << ", " << orientation.y << ", " << orientation.z << "))\n";

		// face definition
		SBFaceDefinition* faceDef = character->getFaceDefinition();
		if (faceDef)
			strstr << "obj.setFaceDefinition(scene.getFaceDefinition(\"" << faceDef->getName() << "\"))\n";

		// controllers
		strstr << "obj.createStandardControllers()\n";
		// attributes
		std::vector<std::string> attributeNames = character->getAttributeNames();
		for (std::vector<std::string>::iterator iter = attributeNames.begin();
			 iter != attributeNames.end();
			 iter++)
		{
			SmartBody::SBAttribute* attr = character->getAttribute((*iter));
			std::string attrWrite = attr->write();
			strstr << attrWrite;
		}

		// reach
		strstr << "# -------------------- reaching for character " << character->getName() << "\n";
		// reach
		SBReachManager* reachManager = this->getReachManager();
		SBReach* reach = reachManager->getReach(character->getName());
		if (reach)
		{
			strstr << "reach = scene.getReachManager().createReach(\"" << character->getName() << "\")\n";
			std::string interpType = reach->getInterpolatorType();
			strstr << "reach.setInterpolatorType(\"" << interpType << "\")\n";
			// motions
			const std::vector<std::string>& leftMotionNames = reach->getMotionNames("left");
			for (std::vector<std::string>::const_iterator iter = leftMotionNames.begin();
				 iter != leftMotionNames.end();
				 iter++)
			{
				strstr << "reach.addMotion(\"left\", scene.getMotion(\"" << (*iter) << "\"))\n";
			}
			const std::vector<std::string>& rightMotionNames = reach->getMotionNames("right");
			for (std::vector<std::string>::const_iterator iter = rightMotionNames.begin();
				 iter != rightMotionNames.end();
				 iter++)
			{
				strstr << "reach.addMotion(\"right\", scene.getMotion(\"" << (*iter) << "\"))\n";
			}
			// point hand
			SBMotion* m = NULL;
			m = reach->getPointHandMotion("left");
			if (m)
			{
				strstr << "reach.setPointHandMotion(\"left\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			m = reach->getPointHandMotion("right");
			if (m)
			{
				strstr << "reach.setPointHandMotion(\"right\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			// grab hand
			m = reach->getGrabHandMotion("left");
			if (m)
			{
				strstr << "reach.setGrabHandMotion(\"left\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			m = reach->getGrabHandMotion("right");
			if (m)
			{
				strstr << "reach.setGrabHandMotion(\"right\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			// release hand
			m = reach->getReleaseHandMotion("left");
			if (m)
			{
				strstr << "reach.setReleaseHandMotion(\"left\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			m = reach->getReleaseHandMotion("right");
			if (m)
			{
				strstr << "reach.setReleaseHandMotion(\"right\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			// reach hand
			m = reach->getReachHandMotion("left");
			if (m)
			{
				strstr << "reach.setReachHandMotion(\"left\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			m = reach->getReachHandMotion("right");
			if (m)
			{
				strstr << "reach.setReachHandMotion(\"right\", scene.getMotion(\"" << m->getName() << "\"))\n";
			}
			strstr << "reach.build(scene.getCharacter(\"" << character->getName() << "\"))\n";
		}
		else
		{
			strstr << "# -- no reach\n";
		}

		// steering
	#ifdef EMBER_SB_STEER
		strstr << "# --- steering for character " << character->getName() << "\n";
		SBSteerManager* steerManager = this->getSteerManager();
		SBSteerAgent* steerAgent = steerManager->getSteerAgent(character->getName());
		if (steerAgent)
		{
			strstr << "steeragent = scene.getSteerManager().createSteerAgent(\"" << character->getName() << "\")\n";
			strstr << "steeragent.setSteerStateNamePrefix(\"" << steerAgent->getSteerStateNamePrefix() << "\")\n";
			strstr << "steeragent.setSteerType(\"" << steerAgent->getSteerType() << "\")\n";
		}
		else
		{
			strstr << "# --- no steering for character " << character->getName() << "\n";
		}
	#else
		strstr << "# --- no steering for character " << character->getName() << "\n";
	#endif

		// set character posture
		strstr << "bml.execBML('" << character->getName() << "', '<body posture=\"" << character->getPostureName() <<"\"/>')\n";
	}

	// enable steering
#ifdef EMBER_SB_STEER
	if (this->getSteerManager()->getSteerAgents().size() > 0)
	{
		strstr << "scene.getServiceManager().getService(\"steering\").setBoolAttribute(\"enable\", True)\n";
	}
#endif
}

void SBScene::saveLights(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- lights\n";
	strstr << "print \"Save Lights\"\n";
	// lights
	const std::vector<std::string>& pawns = getPawnNames();
	for (std::vector<std::string>::const_iterator pawnIter = pawns.begin();
		 pawnIter != pawns.end();
		 pawnIter++)
	{
		SBPawn* pawn = getPawn((*pawnIter));
	#ifdef EMBER_SB_CAMERA
		SrCamera* camera = dynamic_cast<SrCamera*>(pawn);
		if (camera)
			continue; 
	#endif
		if (pawn->getName().find("light") != 0)
		{
			continue;
		}
		strstr << "\n# ---- light: " << pawn->getName() << "\n";
		strstr << "obj = scene.getPawn(\"" << pawn->getName() << "\")\n";
		strstr << "if obj == None:\n";
		strstr << "\tobj = scene.createPawn(\"" << pawn->getName() << "\")\n";
		SrVec position = pawn->getPosition();
		strstr << "obj.setPosition(SrVec(" << position[0] << ", " << position[1] << ", " << position[2] << "))\n";
		SrQuat orientation = pawn->getOrientation();
		strstr << "obj.setOrientation(SrQuat(" << orientation.w << ", " << orientation.x << ", " << orientation.y << ", " << orientation.z << "))\n";

		// attributes
		std::vector<std::string> attributeNames = pawn->getAttributeNames();
		for (std::vector<std::string>::iterator iter = attributeNames.begin();
			 iter != attributeNames.end();
			 iter++)
		{
			SmartBody::SBAttribute* attr = pawn->getAttribute((*iter));
			std::string attrWrite = attr->write();
			strstr << attrWrite;
		}
	}
}

void SBScene::saveBlends(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- blends and transitions\n";
	strstr << "print \"Save Blends\"\n";
	// blends & transitions
	SBAnimationBlendManager* blendManager = getBlendManager();
	std::vector<std::string> blends = blendManager->getBlendNames();
	for (std::vector<std::string>::iterator blendIter = blends.begin();
		 blendIter != blends.end();
		 blendIter++)
	{
		SBAnimationBlend* blend = blendManager->getBlend((*blendIter));
		std::string blendString = blend->saveToString();
		strstr << blendString;
		strstr << "\n";
	}

	int numTransitions = blendManager->getNumTransitions();
	std::vector<std::string> transitions = blendManager->getBlendNames();
	for (int t = 0; t < numTransitions; t++)
	{
		SBAnimationTransition* transition = blendManager->getTransitionByIndex(t);
		std::string transitionString = transition->saveToString();
		strstr << transitionString;
		strstr << "\n";
	}
}

void SBScene::saveJointMaps(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- joint maps\n";
	strstr << "print \"Save Joint Maps\"\n";
	// joint maps
	SBJointMapManager* jointMapManager = getJointMapManager();
	std::vector<std::string> jointMapNames = jointMapManager->getJointMapNames();
	for (std::vector<std::string>::iterator iter = jointMapNames.begin(); iter != jointMapNames.end(); iter++)
	{
		const std::string& jointMapName = (*iter);
		SBJointMap* jointMap = jointMapManager->getJointMap(jointMapName);

		strstr << "jointMap = scene.getJointMapManager().getJointMap(\"" << jointMapName << "\")\n";
		strstr << "if jointMap == None:\n";
		strstr << "\tjointMap = scene.getJointMapManager().createJointMap(\"" << jointMapName << "\")\n";

		int numMappings = jointMap->getNumMappings();
		for (int m = 0; m < numMappings; m++)
		{
			std::string target = jointMap->getTarget(m);
			std::string source = jointMap->getSource(m);
			strstr << "jointMap.setMapping(\"" << source << "\", \"" << target << "\")\n";
		}
	}

	strstr << "# -------------------- applying joint maps\n";
	SmartBody::SBScene* scene = SmartBody::SBScene::getScene();
	std::vector<std::string> mappedMotions = scene->getMotionNames();
	std::vector<std::string> mappedSkeletons = scene->getSkeletonNames();

	for (std::vector<std::string>::iterator iter = jointMapNames.begin(); iter != jointMapNames.end(); iter++)
	{
		const std::string& jointMapName = (*iter);
		SBJointMap* jointMap = jointMapManager->getJointMap(jointMapName);

		strstr << "jointMap = scene.getJointMapManager().getJointMap(\"" << jointMapName << "\")\n";

		


#if 0
		std::vector<std::string>& mappedMotions = jointMap->getMappedMotions();
		for (std::vector<std::string>::iterator iter = mappedMotions.begin();
			 iter != mappedMotions.end();
			 iter++)
		{
			strstr << "jointMap.applyMotion(scene.getMotion(\"" << (*iter) << "\"))\n";
		}

		std::vector<std::string>& mappedSkeletons = jointMap->getMappedSkeletons();
		for (std::vector<std::string>::iterator iter = mappedSkeletons.begin();
			 iter != mappedSkeletons.end();
			 iter++)
		{
			strstr << "jointMap.applySkeleton(scene.getSkeleton(\"" << (*iter) << "\"))\n";
		}
#else	
		for (std::vector<std::string>::iterator iter = mappedMotions.begin();
			iter != mappedMotions.end();
			iter++)
		{
			SmartBody::SBMotion* motion = scene->getMotion(*iter);
			if (motion && motion->channels().getJointMapName() == jointMapName)
				strstr << "jointMap.applyMotion(scene.getMotion(\"" << (*iter) << "\"))\n";
		}

		for (std::vector<std::string>::iterator iter = mappedSkeletons.begin();
			iter != mappedSkeletons.end();
			iter++)
		{
			SmartBody::SBSkeleton* skel = scene->getSkeleton(*iter);
			if (skel && skel->getJointMapName() == jointMapName)
				strstr << "jointMap.applySkeleton(scene.getSkeleton(\"" << (*iter) << "\"))\n";
		}
#endif
	}
}

void SBScene::saveFaceDefinitions(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- face definitions\n";
	strstr << "print \"Save Face Definitions\"\n";
	// face definitions
	std::vector<std::string> faceDefinitions = getFaceDefinitionNames();
	for (std::vector<std::string>::iterator iter = faceDefinitions.begin(); iter != faceDefinitions.end(); iter++)
	{
		const std::string& faceDefName = (*iter);		
		SmartBody::SBFaceDefinition* faceDef = getFaceDefinition(faceDefName);
		std::string faceDefinitionName = "face";
		faceDefinitionName.append(faceDef->getName());
		strstr << faceDefinitionName << " = scene.getFaceDefinition(\"" << faceDefName << "\")\n";
		strstr << "if " << faceDefinitionName << " == None:\n";
		strstr << "\t" << faceDefinitionName << " = scene.createFaceDefinition(\""<< faceDefName << "\")\n";

		SkMotion* neutral = faceDef->getFaceNeutral();
		if (neutral)
		{
			strstr << faceDefinitionName << ".setFaceNeutral(\"" << neutral->getName() << "\")\n";
		}

		std::vector<std::string> visemeNames = faceDef->getVisemeNames();
		for (std::vector<std::string>::iterator faceIter = visemeNames.begin(); 
			 faceIter != visemeNames.end(); 
			 faceIter++)
		{
			const std::string& viseme = (*faceIter);
			strstr << faceDefinitionName << ".setViseme(\"" << viseme << "\", ";
			SkMotion* visemeMotion = faceDef->getVisemeMotion(viseme);
			if (visemeMotion)
			{
				strstr << "\"" + visemeMotion->getName() + "\")\n";
			}
			else
			{
				strstr << "\"\")\n";
			}
		}	

		std::vector<int> auNum = faceDef->getAUNumbers();
		for (std::vector<int>::iterator auIter = auNum.begin(); 
			 auIter != auNum.end(); 
			 auIter++)
		{
			int num = (*auIter);
			ActionUnit* au = faceDef->getAU((*auIter));
			strstr << faceDefinitionName << ".setAU(" << num << ", ";
			SkMotion* motion = NULL;
			if (au->is_bilateral())
			{
				strstr << "\"both\", \"";
				motion = au->left;
			}
			else
			{
				
				if (au->is_left())
				{
					strstr << "\"left\", \"";
					motion = au->left;
				}
				else
				{
					strstr << "\"right\", \"";
					motion = au->right;
				}
			}
			if (motion)
			{
				strstr << motion->getName(); 
			}
			strstr << "\")\n";

		}
	}
}

void SBScene::saveGestureMaps(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- gesture maps\n";
	strstr << "print \"Save Gesture Maps\"\n";
	const std::vector<std::string>& gestureMapNames = this->getGestureMapManager()->getGestureMapNames();
	for (std::vector<std::string>::const_iterator iter = gestureMapNames.begin();
		 iter != gestureMapNames.end();
		 iter++)
	{
		SBGestureMap* gestureMap = this->getGestureMapManager()->getGestureMap(*iter);
		strstr << "gestureMap = scene.getGestureMapManager().createGestureMap(\"" << (*iter) << "\")\n";
		int numMappings = gestureMap->getNumMappings();
		for (int m = 0; m < numMappings; m++)
		{
			SBGestureMap::GestureInfo& info = gestureMap->getGestureByIndex(m);
			strstr << "gestureMap.addGestureMapping(\"" << info._animation << "\", \"" << info._lexeme << "\", \"" << info._type << "\", \"" 
				                                        << info._hand << "\", \"" << info._style << "\", \"" << info._posture << "\")\n";

		}
	}

}

void SBScene::saveLipSyncing(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- lip syncing\n";
	strstr << "print \"Save Lip Syncing\"\n";
	// diphones
	SBDiphoneManager* diphoneManager = getDiphoneManager();
	std::vector<std::string> diphoneMapNames = diphoneManager->getDiphoneMapNames();
	strstr << "diphoneMapManager = scene.getDiphoneManager()\n";
	for (std::vector<std::string>::iterator iter = diphoneMapNames.begin(); iter != diphoneMapNames.end(); iter++)
	{
		const std::string& diphoneMapName = (*iter);
		std::vector<SBDiphone*>& diphones = diphoneManager->getDiphones(diphoneMapName);
		for (std::vector<SBDiphone*>::iterator diphoneIter = diphones.begin();
			 diphoneIter != diphones.end();
			 diphoneIter++)
		{
			SBDiphone* diphone = (*diphoneIter);
			const std::string& fromPhoneme = diphone->getFromPhonemeName();
			const std::string& toPhoneme = diphone->getToPhonemeName();
			strstr << "diphone = diphoneMapManager.createDiphone(\"" << fromPhoneme << "\", \"" << toPhoneme << "\", \"" << diphoneMapName << "\")\n";
			std::vector<std::string> visemes = diphone->getVisemeNames();
			for (std::vector<std::string>::iterator visemeIter = visemes.begin();
				 visemeIter != visemes.end();
				 visemeIter++)
			{
				std::vector<float>& keys = diphone->getKeys((*visemeIter));
				for (size_t x = 0; x < keys.size(); x++)
				{
					if (x + 1 < keys.size())
					{
						strstr << "diphone.addKey(\"" << (*visemeIter) << "\", " << keys[x] << ", " << keys[x + 1] << ")\n";
					}
					x++;
				}
				strstr << "\n";
			}
		}
	}
}

void SBScene::saveServices(std::stringstream& strstr, bool remoteSetup)
{
	strstr << "# -------------------- lip syncing\n";
	// services
	SmartBody::SBServiceManager* serviceManager = this->getServiceManager();
	std::vector<std::string> serviceNames = serviceManager->getServiceNames();
	for (std::vector<std::string>::iterator iter = serviceNames.begin();
		 iter != serviceNames.end();
		 iter++)
	{
		SmartBody::SBService* service = serviceManager->getService((*iter));
		strstr << "obj = scene.getServiceManager().getService(\"" << service->getName() << "\")\n";
		std::vector<std::string> attributeNames = service->getAttributeNames();
		for (std::vector<std::string>::iterator iter = attributeNames.begin();
			 iter != attributeNames.end();
			 iter++)
		{
			SmartBody::SBAttribute* attr = service->getAttribute((*iter));
			std::string attrWrite = attr->write();
			strstr << attrWrite;
		}

	}

}



void SBScene::setSystemParameter(const std::string& name, const std::string& value)
{
	std::map<std::string, std::string>::iterator iter = _systemParameters.find(name);
	if (iter != _systemParameters.end())
	{
		(*iter).second = value;
	}
	else
	{
		_systemParameters.insert(std::pair<std::string, std::string>(name, value));
	}
		
}

std::string SBScene::getSystemParameter(const std::string& name)
{
	std::map<std::string, std::string>::iterator iter = _systemParameters.find(name);
	if (iter != _systemParameters.end())
	{
		return (*iter).second;
	}
	else
	{
		return "";
	}
}

void SBScene::removeSystemParameter(const std::string& name)
{
	std::map<std::string, std::string>::iterator iter = _systemParameters.find(name);
	if (iter != _systemParameters.end())
	{
		_systemParameters.erase(iter);
		return;
	}

	LOG("Cannot remove system parameter named '%s', does not exist.", name.c_str());

}

void SBScene::removeAllSystemParameters()
{
	_systemParameters.clear();
}

std::vector<std::string> SBScene::getSystemParameterNames()
{

	std::vector<std::string> names;
	for (std::map<std::string, std::string>::iterator iter = _systemParameters.begin();
		 iter != _systemParameters.end();
		 iter++)
	{
		names.push_back((*iter).first);
	}

	return names;
}

#ifdef EMBER_SB_CAMERA
SrCamera* SBScene::createCamera(const std::string& name)
{
	SBPawn* pawn = getPawn(name);
	SrCamera* camera = dynamic_cast<SrCamera*>(pawn);
// 	if (camera)
// 	{
// 		LOG("A camera with name '%s' already exists.", name.c_str());
// 		return camera;
// 	}
// 	else 
	if (pawn)
	{
		LOG("A pawn with name '%s' already exists. Camera will not be created.", name.c_str());
		return NULL;
	}
	camera = new SrCamera();
	camera->setName(name);
	//SBSkeleton* skeleton = new SBSkeleton();
	//camera->setSkeleton(skeleton);
	//SkJoint* joint = skeleton->add_joint(SkJoint::TypeQuat);
	//joint->setName("world_offset");

	_cameras.insert(std::pair<std::string, SrCamera*>(name, camera));

	std::map<std::string, SbmPawn*>:: iterator iter = _pawnMap.find(camera->getName());
	if (iter != _pawnMap.end())
	{
		LOG( "Register pawn: pawn_map.insert(..) '%s' FAILED\n", camera->getName().c_str() ); 
	}

	_pawnMap.insert(std::pair<std::string, SbmPawn*>(camera->getName(), camera));
	_pawnNames.push_back(camera->getName());
	
	std::vector<SmartBody::SBSceneListener*>& listeners = this->getSceneListeners();
	for (size_t i = 0; i < listeners.size(); i++)
	{
		listeners[i]->OnPawnCreate( camera->getName() );
	}

	// notify the services
	std::map<std::string, SmartBody::SBService*>& services = getServiceManager()->getServices();
	for (std::map<std::string, SmartBody::SBService*>::iterator iter = services.begin();
		iter != services.end();
		iter++)
	{
		SBService* service = (*iter).second;
		service->onPawnCreate(camera);
	}
	

	// if this is the first camera that is created, make it the active camera
	if (_cameras.size() == 1)
	{
		this->setActiveCamera(camera);
	}

	return camera;
}

void SBScene::removeCamera(SrCamera* camera)
{
	SBPawn* pawn = getPawn(camera->getName());
	if (!pawn)
	{
		LOG("No camera with name '%s' already exists. Camera will not be removed.", camera->getName().c_str());
		return;
	}

	std::map<std::string, SrCamera*>::iterator iter = _cameras.find(camera->getName());
	if (iter == _cameras.end())
	{
		LOG("Pawn with name '%s' already exists, but is not a camera. It will not be removed.", camera->getName().c_str());
		return;
	}

	// is this the active camera?
	if (this->getActiveCamera() == camera)
	{
		setActiveCamera(NULL);
	}
	_cameras.erase(iter);
	removePawn(camera->getName());
}

void SBScene::setActiveCamera(SrCamera* camera)
{
	if (!camera)
	{
		_activeCamera = "";
		return;
	}

	_activeCamera = camera->getName();
}

SrCamera* SBScene::getActiveCamera()
{
	if (_activeCamera == "")
		return NULL;
	std::map<std::string, SrCamera*>::iterator iter = _cameras.find(_activeCamera);
	if (iter == _cameras.end())
		return NULL;

	return (*iter).second;
}

SrCamera* SBScene::getCamera(const std::string& name)
{
	
	std::map<std::string, SrCamera*>::iterator iter = _cameras.find(name);
	if (iter == _cameras.end())
	{
		//LOG("No camera with name '%s' found.", name.c_str());
		return NULL;
	}
	return (*iter).second;
}

void SBScene::SetCameraLocked(bool locked)
{
   _isCameraLocked = locked;
}

bool SBScene::IsCameraLocked()
{
   return _isCameraLocked;
}

int SBScene::getNumCameras()
{
	return _cameras.size();
}

std::vector<std::string> SBScene::getCameraNames()
{
	std::vector<std::string> cameraNames;
	for (std::map<std::string, SrCamera*>::iterator iter = _cameras.begin();
		iter != _cameras.end();
		iter++)
	{
		cameraNames.push_back((*iter).first);
	}

	return cameraNames;
}
#endif


std::vector<SBController*>& SBScene::getDefaultControllers()
{
	return _defaultControllers;
}

void SBScene::createDefaultControllers()
{
	 _defaultControllers.push_back(new MeCtEyeLidRegulator());
	 _defaultControllers.push_back(new MeCtSaccade(NULL));
	 std::map<int, MeCtReachEngine*> reachMap;
	 _defaultControllers.push_back(new MeCtExampleBodyReach(reachMap));
	 _defaultControllers.push_back(new MeCtBreathing());
	 _defaultControllers.push_back(new MeCtGaze());

	 for (size_t x = 0; x < _defaultControllers.size(); x++)
		 _defaultControllers[x]->ref();
}

void SBScene::removeDefaultControllers()
{
	 for (size_t x = 0; x < _defaultControllers.size(); x++)
		 _defaultControllers[x]->unref();
	 _defaultControllers.clear();
}

std::vector<std::string> SBScene::getAssetPaths(const std::string& type)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getAssetPaths() instead.");
	return getAssetManager()->getAssetPaths(type);
}

std::vector<std::string> SBScene::getLocalAssetPaths(const std::string& type)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getLocalAssetPaths() instead.");
	return getAssetManager()->getLocalAssetPaths(type);
}

void SBScene::addAssetPath(const std::string& type, const std::string& path)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.addAssetPath() instead.");
	getAssetManager()->addAssetPath(type, path);
}

void SBScene::removeAssetPath(const std::string& type, const std::string& path)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.addAssetPath() instead.");
	getAssetManager()->removeAssetPath(type, path);
}

void SBScene::removeAllAssetPaths(const std::string& type)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.removeAllAssetPaths() instead.");
	getAssetManager()->removeAllAssetPaths(type);
}

void SBScene::loadAssets()
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.addAssetPath() instead.");
	getAssetManager()->loadAssets();
}

void SBScene::loadAsset(const std::string& assetPath)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.loadAsset() instead.");
	getAssetManager()->loadAsset(assetPath);
}

void SBScene::loadAssetsFromPath(const std::string& assetPath)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.loadAssetsFromPath() instead.");
	getAssetManager()->loadAssetsFromPath(assetPath);
}

SBSkeleton* SBScene::addSkeletonDefinition(const std::string& skelName )
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.addSkeletonDefinition() instead.");
	return getAssetManager()->addSkeletonDefinition(skelName);
}

void SBScene::removeSkeletonDefinition(const std::string& skelName )
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.removeSkeletonDefinition() instead.");
	return getAssetManager()->removeSkeletonDefinition(skelName);
}


SBMotion* SBScene::addMotionDefinition(const std::string& motionName, double duration, int numFrames )
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.addSkeletonDefinition() instead.");
	
	SBMotion* sbMotion = new SBMotion();
	return this->getAssetManager()->addMotionDefinition(motionName, duration, numFrames);
}

SBMotion* SBScene::createMotion(const std::string& motionName)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.createMotion() instead.");
	return getAssetManager()->createMotion(motionName);
}

void SBScene::addMotions(const std::string& path, bool recursive)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.addMotion() instead.");
	getAssetManager()->addMotions(path, recursive);
}

int SBScene::getNumSkeletons()
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getNumSkeletons() instead.");
	return getAssetManager()->getNumSkeletons();
}

std::vector<std::string> SBScene::getSkeletonNames()
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getSkeletonNames() instead.");
	return getAssetManager()->getSkeletonNames();
}

SBMotion* SBScene::getMotion(const std::string& name)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getMotion() instead.");
	return getAssetManager()->getMotion(name);
}

int SBScene::getNumMotions()
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getNumMotions() instead.");
	return getAssetManager()->getNumMotions();
}

std::vector<std::string> SBScene::getMotionNames()
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getNumMotions() instead.");
	return getAssetManager()->getMotionNames();
}

SBSkeleton* SBScene::createSkeleton(const std::string& skeletonDefinition)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.createSkeleton() instead.");
	return getAssetManager()->createSkeleton(skeletonDefinition);
}

SBSkeleton* SBScene::getSkeleton(const std::string& name)
{
	if (SHOW_DEPRECATION_MESSAGES)
		LOG("DEPRECATED: Use AssetManager.getSkeleton() instead.");
	return getAssetManager()->getSkeleton(name);
}

SrSnGroup* SBScene::getRootGroup()
{
	return _rootGroup;
}


std::string SBScene::getValidName(const std::string& name)
{
	bool nameFound = true;
	int nameCounter = 0;
	std::string currentName = name;
	while (nameFound)
	{
		SmartBody::SBPawn* pawn = getPawn(currentName);
		if (!pawn)
		{
			nameFound = false;
		}
		else
		{
			SmartBody::SBCharacter* character = getCharacter(currentName);
			if (!character)
			{
				nameFound = false;
			}
			else
			{
				std::stringstream strstr;
				strstr << name << nameCounter;
				nameCounter++;
				currentName = strstr.str();
			}
		}
	}
	return currentName;
}

void SBScene::updatePawnNames()
{
	std::vector<SbmPawn*> allPawns;
	for (std::map<std::string, SbmPawn*>::iterator iter = _pawnMap.begin();
		 iter != _pawnMap.end();
		 iter++)
	{
		allPawns.push_back((*iter).second);
	}
	_pawnMap.clear();
	_pawnNames.clear();

	for (std::vector<SbmPawn*>::iterator iter = allPawns.begin();
		 iter != allPawns.end();
		 iter++)
	{
		_pawnMap.insert(std::pair<std::string, SbmPawn*>((*iter)->getName(), (*iter))); 
		_pawnNames.push_back((*iter)->getName());
	}

}

void SBScene::updateCharacterNames()
{
	std::vector<SbmCharacter*> allCharacters;
	for (std::map<std::string, SbmCharacter*>::iterator iter = _characterMap.begin();
		 iter != _characterMap.end();
		 iter++)
	{
		allCharacters.push_back((*iter).second);
	}
	_characterMap.clear();
	_characterNames.clear();

	for (std::vector<SbmCharacter*>::iterator iter = allCharacters.begin();
		 iter != allCharacters.end();
		 iter++)
	{
		_characterMap.insert(std::pair<std::string, SbmCharacter*>((*iter)->getName(), (*iter))); 
		_characterNames.push_back((*iter)->getName());
	}
}

#ifdef EMBER_SB_HEIGHTFIELD
Heightfield* SBScene::getHeightfield()
{
	return _heightField;
}


Heightfield* SBScene::createHeightfield()
{
	if (_heightField)
		delete _heightField;
	_heightField = new Heightfield();
	return _heightField;
}

void SBScene::removeHeightfield()
{
	if (_heightField)
		delete _heightField;
	_heightField = NULL;
}

float SBScene::queryTerrain( float x, float z, float *normal_p )
{
	if (_heightField)
	{
		return( _heightField->get_elevation( x, z, normal_p ) );
	}
	if( normal_p )	{
		normal_p[ 0 ] = 0.0;
		normal_p[ 1 ] = 1.0;
		normal_p[ 2 ] = 0.0;
	}
	return( 0.0 );
}
#endif

#ifdef EMBER_SB_PYTHON
#ifndef SB_NO_PYTHON
void SBScene::setPythonMainModule(boost::python::object& pyobject)
{
	_mainModule = pyobject;
}

void SBScene::setPythonMainDict(boost::python::object& pyobject)
{
	_mainDict = pyobject;
}

boost::python::object& SBScene::getPythonMainModule()
{
	return _mainModule;
}

boost::python::object& SBScene::getPythonMainDict()
{
	return _mainDict;
}
#endif
#endif

#ifdef EMBER_SB_CAMERA
void SBScene::setCameraTrack(const std::string& characterName, const std::string& jointName)
{
	SrCamera* camera = getActiveCamera();
	if (!camera)
	{
		LOG("No active camera found. Cannot create camera track.");
		return;
	}
	SbmPawn* pawn = SmartBody::SBScene::getScene()->getPawn(characterName);
	if (!pawn)
	{
		LOG("Object %s was not found, cannot track.", characterName.c_str());
		return;
	}
	if (jointName == "")
	{
		LOG("Need to specify a joint to track.");
		return;
	}

	SkSkeleton* skeleton = NULL;
	skeleton = pawn->getSkeleton();

	SkJoint* joint = pawn->getSkeleton()->search_joint(jointName.c_str());
	if (!joint)
	{
		LOG("Could not find joint %s on object %s.", jointName.c_str(), characterName.c_str());
		return;
	}

	joint->skeleton()->update_global_matrices();
	joint->update_gmat();
	const SrMat& jointMat = joint->gmat();
	SrVec jointPos(jointMat[12], jointMat[13], jointMat[14]);
	CameraTrack* cameraTrack = new CameraTrack();
	cameraTrack->joint = joint;
	cameraTrack->jointToCamera = camera->getEye() - jointPos;
	LOG("Vector from joint to target is %f %f %f", cameraTrack->jointToCamera.x, cameraTrack->jointToCamera.y, cameraTrack->jointToCamera.z);
	cameraTrack->targetToCamera = camera->getEye() - camera->getCenter();
	LOG("Vector from target to eye is %f %f %f", cameraTrack->targetToCamera.x, cameraTrack->targetToCamera.y, cameraTrack->targetToCamera.z);				
	_cameraTracking.push_back(cameraTrack);
	LOG("Object %s will now be tracked at joint %s.", characterName.c_str(), jointName.c_str());
}

void SBScene::removeCameraTrack()
{
	if (_cameraTracking.size() > 0)
	{
		for (std::vector<CameraTrack*>::iterator iter = _cameraTracking.begin();
			 iter != _cameraTracking.end();
			 iter++)
		{
			CameraTrack* cameraTrack = (*iter);
			delete cameraTrack;
		}
		_cameraTracking.clear();
		LOG("Removing current tracked object.");
	}
}

bool SBScene::hasCameraTrack()
{
	return _cameraTracking.size() > 0;
}

void SBScene::updateTrackedCameras()
{
	for (size_t x = 0; x < _cameraTracking.size(); x++)
	{
		// move the camera relative to the joint
		SkJoint* joint = _cameraTracking[x]->joint;
		joint->skeleton()->update_global_matrices();
		joint->update_gmat();
		const SrMat& jointGmat = joint->gmat();
		SrVec jointLoc(jointGmat[12], jointGmat[13], jointGmat[14]);
		SrVec newJointLoc = jointLoc;
		if (fabs(jointGmat[13] - _cameraTracking[x]->yPos) < _cameraTracking[x]->threshold)
			newJointLoc.y = (float) _cameraTracking[x]->yPos;
		SrVec cameraLoc = newJointLoc + _cameraTracking[x]->jointToCamera;
		SrCamera* activeCamera = getActiveCamera();
		activeCamera->setEye(cameraLoc.x, cameraLoc.y, cameraLoc.z);
		SrVec targetLoc = cameraLoc - _cameraTracking[x]->targetToCamera;
		activeCamera->setCenter(targetLoc.x, targetLoc.y, targetLoc.z);
	}	
}
#endif

#ifdef EMBER_SB_VIEWER
SrViewer* SBScene::getViewer()
{
	return _viewer;
}

SrViewer* SBScene::getOgreViewer()
{
	return _ogreViewer;
}

void SBScene::setViewer(SrViewer* viewer)
{
	_viewer = viewer;
}

void SBScene::setOgreViewer(SrViewer* viewer)
{
	_ogreViewer = viewer;
}

SrViewerFactory* SBScene::getViewerFactory()
{
	return _viewerFactory;
}

SrViewerFactory* SBScene::getOgreViewerFactory()
{
	return _ogreViewerFactory;
}

void SBScene::setViewerFactory(SrViewerFactory* viewerFactory)
{
	if (_viewerFactory)
		delete _viewerFactory;
	_viewerFactory = viewerFactory;
}

void SBScene::setOgreViewerFactory(SrViewerFactory* viewerFactory)
{
	if (_ogreViewerFactory)
		delete _ogreViewerFactory;
	_ogreViewerFactory = viewerFactory;
}
#endif

#ifdef EMBER_SB_KINECT
KinectProcessor* SBScene::getKinectProcessor()
{
	return _kinectProcessor;
}
#endif

std::map<std::string, GeneralParam*>& SBScene::getGeneralParameters()
{
	return _generalParams;
}

#if 0
SBAPI bool SBScene::createNavigationMesh( const std::string& meshfilename )
{	
	std::vector<SrModel*> meshVec;
#if (BOOST_VERSION > 104400)
	std::string ext = boost::filesystem::extension(meshfilename);
#else
	std::string ext = boost::filesystem2::extension(meshfilename);
#endif
	std::string file = boost::filesystem::basename(meshfilename);	
	bool loadSuccess = false;
	if (ext == ".obj" || ext == ".OBJ")
	{
		SrModel *mesh = new SrModel();
		loadSuccess = mesh->import_obj(meshfilename.c_str());		
		meshVec.push_back(mesh);
	}
	else if ( ext == ".xml" || ext == ".XML" )
	{
		std::vector<SkinWeight*> tempWeights;
		std::string skeletonName;
		loadSuccess = ParserOgre::parseSkinMesh(meshVec,tempWeights,meshfilename,skeletonName, 1.0,true,false);	
	}
	else if ( ext == ".dae" || ext == ".DAE" )
	{
		if (SmartBody::SBScene::getScene()->getBoolAttribute("useFastCOLLADAParsing"))
			loadSuccess = ParserCOLLADAFast::parseStaticMesh(meshVec, meshfilename);
		else
			loadSuccess = ParserOpenCOLLADA::parseStaticMesh(meshVec, meshfilename);
	}
	
	if (!loadSuccess || meshVec.size() == 0)
	{
		LOG("Error loading navigation mesh, filename = %s",meshfilename.c_str());
		return false;
	}
	//mesh.scale(0.3f);
	SrModel* srMesh = meshVec[0];
	for (unsigned int i=1;i<meshVec.size();i++)
		srMesh->add_model(*meshVec[i]); // adding all mesh together	
	srMesh->remove_redundant_materials();
	srMesh->validate();
	srMesh->computeNormals();
	if (_navigationMesh) 
	{
		delete _navigationMesh;
		_navigationMesh = NULL;
	}
	_navigationMesh = new SBNavigationMesh();
	_navigationMesh->buildNavigationMesh(*srMesh);
	return true;
}
#endif

#ifdef EMBER_SB_STEER
SBAPI void SBScene::setNavigationMesh( const std::string& naviMeshName )
{
	SBNavigationMeshManager* naviManager = getNavigationMeshManager();
	SBNavigationMesh* naviMesh = naviManager->getNavigationMesh(naviMeshName);
	if (naviMesh)
		_navigationMesh = naviMesh;
}

SBAPI SBNavigationMesh* SBScene::getNavigationMesh()
{
	return _navigationMesh;
}
#endif

void SBScene::startFileLogging(const std::string& filename)
{
	_logListener = new vhcl::Log::FileListener(filename.c_str());
	vhcl::Log::g_log.AddListener(_logListener);
}

void SBScene::stopFileLogging()
{
	if (_logListener)
		vhcl::Log::g_log.RemoveListener(_logListener);
}

std::string SBScene::getStringFromObject(SmartBody::SBObject* object)
{
	std::stringstream strstr;
	SmartBody::SBCharacter* character = dynamic_cast<SmartBody::SBCharacter*>(object);
	if (character)
	{
		strstr << "character/" << character->getName();
		return strstr.str();
	}

	SmartBody::SBPawn* pawn = dynamic_cast<SmartBody::SBPawn*>(object);
	if (pawn)
	{
		strstr << "pawn/" << pawn->getName();
		return strstr.str();
	}

	SmartBody::SBScene* scene = dynamic_cast<SmartBody::SBScene*>(object);
	if (scene)
	{
		strstr << "scene/" << scene->getName();
		return strstr.str();
	}

	SmartBody::SBSkeleton* skeleton = dynamic_cast<SmartBody::SBSkeleton*>(object);
	if (skeleton)
	{
		strstr << "skeleton/" << skeleton->getName();
		return strstr.str();
	}

	SmartBody::SBMotion* motion = dynamic_cast<SmartBody::SBMotion*>(object);
	if (motion)
	{
		strstr << "motion/" << motion->getName();
		return strstr.str();
	}

	SmartBody::SBService* service = dynamic_cast<SmartBody::SBService*>(object);
	if (service)
	{
		strstr << "service/" << service->getName();
		return strstr.str();
	}

	DeformableMesh* mesh = dynamic_cast<DeformableMesh*>(object);
	if (mesh)
	{
		strstr << "model/" << mesh->getName();
		return strstr.str();
	}

	SmartBody::SBController* controller = dynamic_cast<SmartBody::SBController*>(object);
	if (controller)
	{
		strstr << "character/" << controller->getCharacterName() << "/controller/" << controller->getName();
		return strstr.str();
	}

	SmartBody::SBJointMap* jointmap = dynamic_cast<SmartBody::SBJointMap*>(object);
	if (jointmap)
	{
		strstr << "jointmap/" << jointmap->getName();
		return strstr.str();
	}

	SmartBody::SBGestureMap* gesturemap = dynamic_cast<SmartBody::SBGestureMap*>(object);
	if (gesturemap)
	{
		strstr << "gesturemap/" << gesturemap->getName();
		return strstr.str();
	}

	SmartBody::SBEventHandler* eventHandler = dynamic_cast<SmartBody::SBEventHandler*>(object);
	if (eventHandler)
	{
		strstr << "eventhandler/" << eventHandler->getName();
		return strstr.str();
	}

	SmartBody::SBAnimationBlend* blend = dynamic_cast<SmartBody::SBAnimationBlend*>(object);
	if (blend)
	{
		strstr << "blend/" << blend->getName();
		return strstr.str();
	}

	SmartBody::SBAnimationTransition* transition = dynamic_cast<SmartBody::SBAnimationTransition*>(object);
	if (transition)
	{
		strstr << "transition/" << transition->getName();
		return strstr.str();
	}

	SmartBody::SBScript* script = dynamic_cast<SmartBody::SBScript*>(object);
	if (script)
	{
		strstr << "script/" << script->getName();
		return strstr.str();
	}

	SmartBody::SBFaceDefinition* facedef = dynamic_cast<SmartBody::SBFaceDefinition*>(object);
	if (facedef)
	{
		strstr << "facedefinition/" << facedef->getName();
		return strstr.str();
	}

	return "";
}


SmartBody::SBObject* SBScene::getObjectFromString(const std::string& value)
{
	int prefixPos = value.find("/");
	if (prefixPos == std::string::npos)
		return NULL;

	std::string prefix = value.substr(0, prefixPos);
	std::string suffix = value.substr(prefixPos + 1);

	if (prefix == "character")
	{
		// check for a second level name
		int prefixPos2 = suffix.find("/");
		if (prefixPos2 == std::string::npos)
		{
			SmartBody::SBCharacter* character = this->getCharacter(suffix);
			return character;
		}

		std::string characterName = suffix.substr(0, prefixPos2);
		SmartBody::SBCharacter* character = this->getCharacter(characterName);
		if (!character)
			return NULL;

		std::string remainder = suffix.substr(prefixPos2 + 1);

		int prefixPos3 = remainder.find("/");
		if (prefixPos3 != std::string::npos)
		{
			std::string part = remainder.substr(0, prefixPos3);		
			std::string rest = remainder.substr(prefixPos3 + 1);
			if (part == "controller")
			{
				SBController* controller = character->getControllerByName(rest);
				return controller;
			}
			if (part == "skeleton")
			{
				SBSkeleton* skeleton = character->getSkeleton();
				if (skeleton->getName() == rest)
					return skeleton;
				else
					return NULL;
			}
			if (part == "minibrain")
			{
				SmartBody::Nvbg* nvbg = character->getNvbg();
				return nvbg;
			}
		}
		
		return NULL;
	}
	else if (prefix == "pawn")
	{
		SmartBody::SBPawn* pawn = this->getPawn(suffix);
		return pawn;
	}
	else if (prefix == "scene")
	{
		return this;
	}
	else if (prefix == "motion")
	{
		SmartBody::SBMotion* motion = this->getAssetManager()->getMotion(suffix);
		return motion;
	}
	else if (prefix == "skeleton")
	{
		SmartBody::SBSkeleton* skeleton = this->getAssetManager()->getSkeleton(suffix);
		return skeleton;
	}
	else if (prefix == "service")
	{
		SmartBody::SBService* service = this->getServiceManager()->getService(suffix);
		return service;
	}
	else if (prefix == "model")
	{
		DeformableMesh* mesh = this->getAssetManager()->getDeformableMesh(suffix);
		return mesh;
	}
	else if (prefix == "controller")
	{
		int prefixPos2 = suffix.find("/");
		if (prefixPos2 == std::string::npos)
			return NULL;
		std::string prefix2 = suffix.substr(0, prefixPos2 - 1);
		std::string suffix2 = suffix.substr(prefixPos2 + 1);
		SmartBody::SBCharacter* character = this->getCharacter(suffix);
		if (!character)
			return NULL;
		return character->getControllerByName(suffix2);
	}
	else if (prefix == "jointmap")
	{
		SBJointMap* jointMap = this->getJointMapManager()->getJointMap(suffix);
		return jointMap;
	}
	else if (prefix == "gesturemap")
	{
		SBGestureMap* gestureMap = this->getGestureMapManager()->getGestureMap(suffix);
		return gestureMap;
	}
	else if (prefix == "eventhandler")
	{
		SBEventHandler* eventHandler = this->getEventManager()->getEventHandler(suffix);
		return eventHandler;
	}
	else if (prefix == "blend")
	{
		SBAnimationBlend* blend = this->getBlendManager()->getBlend(suffix);
		return blend;
	}
	else if (prefix == "transition")
	{
		SBAnimationTransition* transition = this->getBlendManager()->getTransitionByName(suffix);
		return transition;
	}
	else if (prefix == "script")
	{
		SBScript* script = this->getScript(suffix);
		return script;
	}
	else if (prefix == "facedefinition")
	{
		SBFaceDefinition* facedef = this->getFaceDefinition(suffix);
		return facedef;
	}
	return NULL;
}



};


