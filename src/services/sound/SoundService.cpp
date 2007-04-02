/*
  Copyright (C) 2002 Miguel Guzman Miranda (Aglanor)
  Code based on the Worldspace OpenAL tutorials by Lord Loki
  	at http://worldspace.berlios.de/openal/

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

#include <AL/al.h>
#include <AL/alc.h>
#include <AL/alut.h>

#include "framework/Service.h"
#include "framework/ConsoleObject.h"

#include "services/EmberServices.h"
#include "services/config/ConfigService.h"
#include "services/logging/LoggingService.h"
#include "framework/ConsoleBackend.h"
#include "framework/Tokeniser.h"

/*
#include <list>
#include <algorithm>
*/
#include <sstream>

#include "SoundService.h"



namespace Ember
{
    // List of SoundService's console commands
	const char * const SoundService::PLAYSOUND = "playsound";
	const char * const SoundService::PLAYMUSIC = "playmusic";
	const char * const SoundService::PLAYFILE = "playfile";
	const char * const SoundService::PLAYSPEECH = "playspeech";


	/* ctor */
	SoundService::SoundService()
	{
		setName("Sound Service");
		setDescription("Service for reproduction of sound effects and background music");
	}

	/* dtor */
	SoundService::~SoundService()
	{

	}

	/* Method for starting this service 	*/
	Service::Status SoundService::start()
	{

		S_LOG_INFO("Sound Service starting");
		
	#ifndef WIN32
		// Test that /dev/dsp is availible
		FILE *temp = fopen("/dev/dsp","w");
		if (temp) {
			fclose(temp);
		} else {
			return Service::FAILURE;
		}
	#endif
		
		ALfloat listenerPos[3]={0.0,0.0,0.0};	// listener position
		ALfloat listenerVel[3]={0.0,0.0,0.0};	// listener velocity
		ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};	// listener orientation
		ALfloat sourcePos[3]={ 0.0, 10.0, 0.0};	// source position
		ALfloat sourceVel[3]={ 0.0, 0.0, 1.0};	// source velocity

		data=NULL;

		// Initialize OpenAL
		S_LOG_VERBOSE("Initializing OpenAL");

		if(!alutInit(NULL,0)) {
			S_LOG_FAILURE( "Error initiatin AL: " << alutGetErrorString(alutGetError()) );
			return Service::FAILURE;
		} else {
			S_LOG_INFO( "AL initiated"  );
		}

		// set listener initial parameters
		alListenerfv(AL_POSITION,listenerPos);
		alListenerfv(AL_VELOCITY,listenerVel);
		alListenerfv(AL_ORIENTATION,listenerOri);

		// Generate buffers
		S_LOG_VERBOSE("Generating Buffers");

		alGenBuffers(1,&systemBuffer);
		if (!alIsBuffer(systemBuffer))
		{
			S_LOG_FAILURE("Error creating system buffer");
			return Service::FAILURE;
		}

		alGenBuffers(1,&musicBuffer);
		if (!alIsBuffer(musicBuffer))
		{
			S_LOG_FAILURE("Error creating music buffer");
			return Service::FAILURE;
		}

		alGenBuffers(NUM_WORLD_BUFFERS,worldBuffers);
		for (int i=0;i<NUM_WORLD_BUFFERS;i++)
		{
			if (!alIsBuffer(worldBuffers[i]))
			{
				S_LOG_FAILURE( "Error creating world buffers" );
				return Service::FAILURE;
			}
		}

		
		// Generate sources

		S_LOG_VERBOSE("Generating Sources");
		alGenSources(1,&systemSource);
		if (!alIsSource(systemSource))
		{
			S_LOG_FAILURE("Error creating system source");
			return Service::FAILURE;
		}

		alGenSources(1,&musicSource);
		if (!alIsSource(musicSource))
		{
			S_LOG_FAILURE("Error creating music source");
			return Service::FAILURE;
		}

		alGenSources(NUM_WORLD_SOURCES,worldSources);
		for (int i=0;i<NUM_WORLD_SOURCES;i++)
		{
			if (!alIsSource(worldSources[i]))
			{
			S_LOG_FAILURE("Error creating world sources")
				return Service::FAILURE;
			}
		}

		// Register service commands with the console
		S_LOG_VERBOSE("Registering Sound Service commands");
		ConsoleBackend::getMainConsole()->registerCommand(PLAYSOUND,this);
		ConsoleBackend::getMainConsole()->registerCommand(PLAYMUSIC,this);
		ConsoleBackend::getMainConsole()->registerCommand(PLAYFILE,this);
		ConsoleBackend::getMainConsole()->registerCommand(PLAYSPEECH,this);

		// Service initialized successfully
		setRunning( true );
		setStatus(Service::OK);
		setStatusText("Sound Service status OK.");

		S_LOG_INFO("Sound Service initialized");
		return Service::OK;

	}

	/* Interface method for stopping this service */
	void SoundService::stop(int code)
	{
		alSourceStop(worldSources[0]);
		alutExit();			// Finalize OpenAL
		setStatus(Service::OK);
		setRunning( false );
	}

	void SoundService::runCommand(const std::string &command, const std::string &args)
	{
		if(command == PLAYSOUND)
		{
			playTestSound();

		}
		else if(command == PLAYMUSIC)
		{
			// TODO: play test music here
			S_LOG_INFO(getName() << " I should be playing music")
		}
		else if(command == PLAYFILE)
		{
			playSystemSound("pig_grunt.wav");
		}
		else if(command == PLAYSPEECH)
		{
			// TODO: play test music here
			S_LOG_INFO(getName() << " I should be playing speech")
		}
	}

	void SoundService::registerSoundProvider(ISoundProvider* provider)
	{
		mProvider = provider;
		provider->_registerWithService(this);
		S_LOG_INFO("Registered scripting provider " << provider->getName());
	}

	bool SoundService::LoadWAV(const char *fname,int buffer)
	{

#ifdef _WIN32 // Windows

		alutLoadWAVFile(fname,&format,&data,&size,&freq); 	// Load WAV file
		alBufferData(buffer,format,data,size,freq); 			// Connect WAV to buffer

#endif

#ifdef _LINUX // Linux

		alutLoadWAV(fname,&data,&format,&size,&bits,&freq);		// Load WAV file
		alBufferData(buffer,format,data,size,freq);				// Connect WAV to buffer
		S_LOG_INFO("Loading WAV and stuff")

#endif

	return true;

	}

  bool SoundService::UnloadWAV(void)
  {

#ifdef _WIN32 //Windows

		alutUnloadWAV(format,data,size,freq); // Funtion that unloads the Wav file on windows

#endif

#ifdef _LINUX // Linux

		free (data); // Free the memory used when loading the WAV file

#endif

	return true;

	}

	void SoundService::TestPlatform(void)
	{

		S_LOG_INFO("Testing Platform")

#ifdef _WIN32 //Windows

		S_LOG_INFO("Windows Platform found")

#endif

#ifdef _LINUX // Linux

		S_LOG_INFO("Linux Platform found")

#endif


	}
/*
	void SoundService::playTestGYPH(void) {
		alSourcePlay(worldSources[0]);
		int error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Error playing sound: " << error)
		}
	}
*/
	void SoundService::playTestGrunt(void) {
	
		std::stringstream gruntPath;
		gruntPath << soundsDirPath << "pig_grunt.wav";
		S_LOG_INFO( "Loading sound: [" << gruntPath.str() << "]" )
		//alutLoadWAV(gruntPath.str().c_str(),&data,&format,&size,&bits,&freq);
		
		// Connect WAV to buffer
		//alBufferData(worldBuffers[1],format,data,size,freq);
		
		// Play
		//alSourcePlay(worldSources[1]);
		
		// Check errors
		int error = alGetError();
		if(error != AL_NO_ERROR)
		{
			char* errorStr = (char*)alGetString(error);
			//std::string errorStr = alGetString(error);
			S_LOG_FAILURE( "Error playing sound: " << errorStr )
		}
	}

	void SoundService::updateListenerPosition(
		const WFMath::Point<3>& position,
		const WFMath::Quaternion& orientation) {

		ALfloat listenerPosition[3]={position.x(),position.y(),position.z()};
		//ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};

		// set listener initial parameters
		alListenerfv(AL_POSITION,listenerPosition);
		//alListenerfv(AL_ORIENTATION,listenerOri);

		// update the System and Music Source, 
		// because they will always be with the listener
		// TODO: WRONG! do this with relative position (0,0,0) to the listener
		//alSourcefv(systemSource,AL_POSITION,listenerPosition);
	}

/*
	void SoundService::updateSourcePosition(
		const int sourceId,
		const WFMath::Point<3>& position,
		const WFMath::Quaternion& orientation) {

		ALfloat sourcePosition[3]={position.x(),position.y(),position.z()};
		//ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};

		// set listener initial parameters
		alSourcefv(AL_POSITION,sourcePosition);
		//alListenerfv(AL_ORIENTATION,listenerOri);
	}
*/

	void SoundService::updateAvatarSourcePosition(
		const WFMath::Point<3>& position,
		const WFMath::Quaternion& orientation) {

		ALfloat avatarSourcePosition[3]={position.x(),position.y(),position.z()};
		//ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};

		alSourcefv(avatarSource,AL_POSITION,avatarSourcePosition);
		//alListenerfv(AL_ORIENTATION,listenerOri);
	}

	void SoundService::playTestSound() {
		systemBuffer = alutCreateBufferHelloWorld();
		alSourcei(systemSource, AL_BUFFER, systemBuffer);
		alSourcePlay(systemSource);
	}

	void SoundService::playAvatarSound() {
		avatarBuffer = alutCreateBufferHelloWorld();
		alSourcei(avatarSource, AL_BUFFER, avatarBuffer);
		alSourcePlay(avatarSource);
	}

	void SoundService::playTalk(std::string message,
		const WFMath::Point<3>& position,
		const WFMath::Quaternion& orientation) {

		S_LOG_INFO( "Playing talk: " << message );

		// determine the source and buffer that will play the sound
		int i = getWorldSourceIndexForPlaying(0);
		ALuint worldSource = worldSources[i];
		ALuint worldBuffer = worldBuffers[i];

		// adjust position
		// TODO: adjust orientation
		ALfloat worldSourcePosition[3]={position.x(),position.y(),position.z()};	
		alSourcefv(worldSource,AL_POSITION,worldSourcePosition);

		worldBuffer = alutCreateBufferHelloWorld();
		alSourcei(worldSource, AL_BUFFER, worldBuffer);
		alSourcePlay(worldSource);
	}

	void SoundService::playSystemSound(std::string soundName) {

		// load the sound through the sound provider
		mProvider->loadSound(soundName);	

		// play sound

		/*
		Ogre::FileInfoListPtr files =
			Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo(
			"General", "pig_grunt.wav");

		Ogre::String filePath;
		for(Ogre::FileInfoList::iterator it=files->begin(); it!=files->end(); ++it )
		{
			filePath = (*it).path;
			//filePath.append((*it).filename);
			S_LOG_INFO( "Found a filepath: " << filePath);
		}
		// TODO: this plays the last one found. Should play the first one found
		// TODO: check error if it finds none

		systemBuffer = alutCreateBufferFromFile(filePath.c_str());
		if(systemBuffer == AL_NONE) 
		{
			S_LOG_FAILURE(alutGetErrorString(alutGetError()));
		} else 
		{
			S_LOG_INFO("TRACE - BUFFER LOADED");
		}
		alSourcei(systemSource, AL_BUFFER, systemBuffer);
		
		alSourcePlay(systemSource);
		*/
	}

	ALuint SoundService::getWorldSourceIndexForPlaying(int priority) {
		return 0;
	}

} // namespace Ember
