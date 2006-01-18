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


#include "SoundService.h"


namespace Ember
{
    // List of SoundService's console commands
	const char * const SoundService::PLAYSOUND = "playsound";
	const char * const SoundService::PLAYMUSIC = "playmusic";

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
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- SOUND SERVICE STARTING ***" << std::endl;
	std::cout << "************************************" << std::endl;
		ALfloat listenerPos[3]={0.0,0.0,0.0};	// listener position
		ALfloat listenerVel[3]={0.0,0.0,0.0};	// listener velocity
		ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};	// listener orientation
		ALfloat sourcePos[3]={ 0.0, 10.0, 0.0};	// source position
		ALfloat sourceVel[3]={ 0.0, 0.0, 1.0};	// source velocity

		data=NULL;

		// Initialize OpenAL
		S_LOG_VERBOSE("Initializing OpenAL");
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- INITIALIZING OPENAL ***" << std::endl;
	std::cout << "************************************" << std::endl;
		alutInit(NULL,0);
		if(alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error initiatin AL" )
			return Service::FAILURE;
		} else {
			S_LOG_INFO( "AL initiated"  )
		}

		// set listener initial parameters
		alListenerfv(AL_POSITION,listenerPos);
		alListenerfv(AL_VELOCITY,listenerVel);
		alListenerfv(AL_ORIENTATION,listenerOri);

		// Generate buffers
		std::cout << "************************************" << std::endl;
		std::cout << "TRACE --- GENERATING BUFFERS ***" << std::endl;
		std::cout << "************************************" << std::endl;
		S_LOG_VERBOSE("Generating Buffers");

		alGenBuffers(1,&systemBuffer);
		if (!alIsBuffer(systemBuffer))
		{
			S_LOG_FAILURE("Error creating system buffer")
			return Service::FAILURE;
		}

		alGenBuffers(1,&musicBuffer);
		if (!alIsBuffer(musicBuffer))
		{
			S_LOG_FAILURE("Error creating music buffer")
			return Service::FAILURE;
		}


		alGenBuffers(NUM_WORLD_BUFFERS,worldBuffers);
		for (int i=0;i<NUM_WORLD_BUFFERS;i++)
		{
			if (!alIsBuffer(worldBuffers[i]))
			{
				S_LOG_FAILURE( "Error creating world buffers" )
				return Service::FAILURE;
			}
		}

		
		// Generate sources
		std::cout << "************************************" << std::endl;
		std::cout << "TRACE --- GENERATING SOURCES ***" << std::endl;
		std::cout << "************************************" << std::endl;

		S_LOG_VERBOSE("Generating Sources");
		alGenSources(1,&systemSource);
		if (!alIsSource(systemSource))
		{
			S_LOG_FAILURE("Error creating system source")
			return Service::FAILURE;
		}

		alGenSources(1,&musicSource);
		if (!alIsSource(musicSource))
		{
			S_LOG_FAILURE("Error creating music source")
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

		/*
		TestPlatform();  // a test to determine the current platform (win, unix, ...)
		
		soundsDirPath = Ember::EmberServices::getSingletonPtr()->getConfigService()->getEmberDataDirectory() 
			+ "media/sounds/";
		S_LOG_INFO( "Sound Media Path: [" << soundsDirPath << "]" )

		std::stringstream gyphPath;
		gyphPath << soundsDirPath << "gyph.wav";
		S_LOG_INFO(
			"Loading sound: [" << gyphPath.str() << "]")
		//alutLoadWAV(gyphPath.str().c_str(),&data,&format,&size,&bits,&freq);
		// Load WAV file  // Should be LoadWAV, platform independant

		if(alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error loading wav" )
			return Service::FAILURE;
		} else {
			S_LOG_INFO( "WAV file loaded"  )
		}


		S_LOG_INFO( "Loaded WAV file? let's check"  )
		S_LOG_INFO( "format: " << format )
		S_LOG_INFO( "size: " << size )
		S_LOG_INFO( "bits: " << bits )
		S_LOG_INFO( "freq: " << freq )

		// Connect WAV to buffer
		alBufferData(worldBuffers[0],format,data,size,freq);
		

		if(alGetError() != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error connecting wav to buffer" )
			return Service::FAILURE;
		} else {
			S_LOG_INFO( "WAV connected to buffer"  )
		}
		*/
/*

	free (data);  // should be UnloadWAV, platform independant

	//	 UnloadWAV();



		int error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error in listener parameters: " << error )
		} else {
			S_LOG_INFO( "Listener Parameters OK" )
		}

		alSourcef(worldSources[0],AL_PITCH,1.0f);	// source Frequency

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error in source parameter pitch: " << error )
		} else {
			S_LOG_INFO( "Source parameter pitch OK" )
		}

		alSourcef(worldSources[0],AL_GAIN,1.0f);	// source gain

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error in source parameter gain: " << error )
		} else {
			S_LOG_INFO( "Source parameter gain OK" )
		}



		alSourcefv(worldSources[0],AL_POSITION,sourcePos);	// source position

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error in source parameter position: " << error )
		} else {
			S_LOG_INFO( "Source parameter position OK" )
		}



		alSourcefv(worldSources[0],AL_VELOCITY,sourceVel);	// source velocity
		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error in source parameter velocity: " << error )
		} else {
			S_LOG_INFO( "Source parameter velocity OK" )
		}


		alSourcei(worldSources[0],AL_LOOPING,AL_FALSE);		// looping play

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error in source parameter looping: " << error )
		} else {
			S_LOG_INFO( "Source parameter looping OK" )
		}



		alSourcei(worldSources[0],AL_BUFFER,worldBuffers[0]);		// link the source to the buffer

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE( "Error attaching buffer to source: " << error )
		} else {
			S_LOG_INFO( "Buffer attached OK" )
		}
*/

		/*
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
			<< "Press 1 to play sound, 2 to stop, 3 to go on with Ember." )
		*/


// This code is not useful anymore. Kept in order to use it as base for other methods, but will be removed. Do not uncomment, it does not work anymore. Use /playsound console command instead.
/*
		char a; bool loop = true;
	do {
		cin >> a;
		S_LOG_INFO("a: " << a)
 		switch (a)
		{
			case '1':

				S_LOG_INFO("playing")
				break;
			case '2':
				alSourceStop(worldSources[0]);
				S_LOG_INFO("stopping")
				break;
			case '3':
				loop = false;
				break;
		}
			error = alGetError();
			if(error != AL_NO_ERROR)
			{
				S_LOG_FAILURE("Error: " << error)
			}

	} while (loop);

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Error playing wav: " << error)

			return Service::FAILURE;
		}

	alSourceStop(worldSources[0]);
*/

		// Register service commands with the console
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- REGISTERING CONSOLE COMMANDS ***" << std::endl;
	std::cout << "************************************" << std::endl;
		S_LOG_VERBOSE("Registering Sound Service commands");
		ConsoleBackend::getMainConsole()->registerCommand(PLAYSOUND,this);
		ConsoleBackend::getMainConsole()->registerCommand(PLAYMUSIC,this);

		// Service initialized successfully
		setRunning( true );
		setStatus(Service::OK);
		setStatusText("Sound Service status OK.");

		S_LOG_INFO("Sound Service initialized");
	std::cout << "************************************" << std::endl;
	std::cout << "TRACE --- SOUND SERVICE INITIALIZED ***" << std::endl;
	std::cout << "************************************" << std::endl;
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
			//alSourcePlay(worldSources[0]);
/*
			int error = alGetError();
			if(error != AL_NO_ERROR)
			{
				S_LOG_FAILURE("Error playing sound: " << error)
			}
*/
		systemBuffer = alutCreateBufferHelloWorld();
		
		alSourcei(systemSource, AL_BUFFER, systemBuffer);
		alSourcePlay(systemSource);
		alutSleep(1); //??? WHAT IS THIS FOR???

		}
		else if(command == PLAYMUSIC)
		{
			S_LOG_INFO(getName() << " I should be playing music")
		}
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

	void SoundService::playTestGYPH(void) {
		alSourcePlay(worldSources[0]);
		int error = alGetError();
		if(error != AL_NO_ERROR)
		{
			S_LOG_FAILURE("Error playing sound: " << error)
		}
	}

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

		ALfloat listenerPos[3]={position.x(),position.y(),position.z()};
		//ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};

		// set listener initial parameters
		alListenerfv(AL_POSITION,listenerPos);
		//alListenerfv(AL_ORIENTATION,listenerOri);
	}


} // namespace Ember
