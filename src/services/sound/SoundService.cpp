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

		ALfloat listenerPos[3]={0.0,0.0,0.0};	// listener position
		ALfloat listenerVel[3]={0.0,0.0,0.0};	// listener velocity
		ALfloat listenerOri[6]={0.0,0.0,1.0,0.0,1.0,0.0};	// listener orientation
		ALfloat sourcePos[3]={ 0.0, 10.0, 0.0};	// source position
		ALfloat sourceVel[3]={ 0.0, 0.0, 1.0};	// source velocity

		data=NULL;

		// Initialize OpenAL
		alutInit(NULL,0);
		if(alGetError() != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) 
				<< "Error initiatin AL" << ENDM;
			return Service::FAILURE;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
				<< "AL initiated"  << ENDM;
		}

		// Generate buffers
		/*
		if (!alGenBuffers(1,&systemBuffer)) return Service::FAILURE;
		if (!alIsBuffer(systemBuffer)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating system buffer" << ENDM;
			return Service::FAILURE;
		}
*//*
		//alGenBuffers(1,&musicBuffer);
		if (!alIsBuffer(musicBuffer)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating music buffer" << ENDM;
			return Service::FAILURE;
		}*/

		alGenBuffers(NUM_WORLD_BUFFERS,worldBuffers);
		for (int i=0;i<NUM_WORLD_BUFFERS;i++)
		{
			if (!alIsBuffer(worldBuffers[i]))
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE)
					<< "Error creating world buffers" << ENDM;
				return Service::FAILURE;
			}
		}

		/*
		// Generate sources
		alGenSources(1,systemSource);
		if (!alIsSource(systemSource)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating system source" << ENDM;
			return Service::FAILURE;
		}

		alGenSources(1,&musicSource);
		if (!alIsSource(musicSource)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating music source" << ENDM;
			return Service::FAILURE;
		}*/

		alGenSources(NUM_WORLD_SOURCES,worldSources);
		for (int i=0;i<NUM_WORLD_SOURCES;i++)
		{
			if (!alIsSource(worldSources[i]))
			{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error creating world sources" << ENDM;
				return Service::FAILURE;
			}
		}


		TestPlatform();  // a test to determine the current platform (win, unix, ...)
		
		soundsDirPath = Ember::EmberServices::getInstance()->getConfigService()->getEmberDataDirectory() 
			+ "media/sounds/";
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << 
			"Sound Media Path: [" << soundsDirPath << "]" << ENDM;

		std::stringstream gyphPath;
		gyphPath << soundsDirPath << "gyph.wav";
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << 
			"Loading sound: [" << gyphPath.str() << "]" << ENDM;
		alutLoadWAV(gyphPath.str().c_str(),&data,&format,&size,&bits,&freq);
		// Load WAV file  // Should be LoadWAV, platform independant

		if(alGetError() != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) 
				<< "Error loading wav" << ENDM;
			return Service::FAILURE;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
				<< "WAV file loaded"  << ENDM;
		}


		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
			<< "Loaded WAV file? let's check"  << ENDM;
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
			<< "format: " << format << ENDM;
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
			 << "size: " << size << ENDM;
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
			<< "bits: " << bits << ENDM;
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
			<< "freq: " << freq << ENDM;

		// Connect WAV to buffer
		alBufferData(worldBuffers[0],format,data,size,freq);
		

		if(alGetError() != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) 
				<< "Error connecting wav to buffer" << ENDM;
			return Service::FAILURE;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
				<< "WAV connected to buffer"  << ENDM;
		}


	free (data);  // should be UnloadWAV, platform independant

	//	 UnloadWAV();

		// set listener initial parameters
		alListenerfv(AL_POSITION,listenerPos);			// position
		alListenerfv(AL_VELOCITY,listenerVel);			// velocity
		alListenerfv(AL_ORIENTATION,listenerOri);		// orientation

		int error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) 
				<< "Error in listener parameters: " << error << ENDM;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
				<< "Listener Parameters OK" << ENDM;
		}

		alSourcef(worldSources[0],AL_PITCH,1.0f);	// source Frequency

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) 
				<< "Error in source parameter pitch: " << error << ENDM;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
				<< "Source parameter pitch OK" << ENDM;
		}

		alSourcef(worldSources[0],AL_GAIN,1.0f);	// source gain

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE)
				<< "Error in source parameter gain: " << error << ENDM;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
				<< "Source parameter gain OK" << ENDM;
		}



		alSourcefv(worldSources[0],AL_POSITION,sourcePos);	// source position

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE)
				<< "Error in source parameter position: " << error << ENDM;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
				<< "Source parameter position OK" << ENDM;
		}



		alSourcefv(worldSources[0],AL_VELOCITY,sourceVel);	// source velocity
		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE)
				<< "Error in source parameter velocity: " << error << ENDM;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
				<< "Source parameter velocity OK" << ENDM;
		}


		alSourcei(worldSources[0],AL_LOOPING,AL_FALSE);		// looping play

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE)
				<< "Error in source parameter looping: " << error << ENDM;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
				<< "Source parameter looping OK" << ENDM;
		}



		alSourcei(worldSources[0],AL_BUFFER,worldBuffers[0]);		// link the source to the buffer

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE)
				<< "Error attaching buffer to source: " << error << ENDM;
		} else {
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
				<< "Buffer attached OK" << ENDM;
		}


		/*
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) 
			<< "Press 1 to play sound, 2 to stop, 3 to go on with Ember." << ENDM;
		*/


// This code is not useful anymore. Kept in order to use it as base for other methods, but will be removed. Do not uncomment, it does not work anymore. Use /playsound console command instead.
/*
		char a; bool loop = true;
	do {
		cin >> a;
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "a: " << a << ENDM;
 		switch (a)
		{
			case '1':

				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "playing"<< ENDM;
				break;
			case '2':
				alSourceStop(worldSources[0]);
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "stopping" << ENDM;
				break;
			case '3':
				loop = false;
				break;
		}
			error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error: " << error << ENDM;
			}

	} while (loop);

		error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error playing wav: " << error << ENDM;

			return Service::FAILURE;
		}

	alSourceStop(worldSources[0]);
*/

		// Register service commands with the console
		ConsoleBackend::getMainConsole()->registerCommand(PLAYSOUND,this);
		ConsoleBackend::getMainConsole()->registerCommand(PLAYMUSIC,this);

		// Service initialized successfully
		setRunning( true );
		setStatus(Service::OK);
		setStatusText("Sound Service status OK.");

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO)
			<< getName() << " initialized" << ENDM;
		return Service::OK;

  }

	/* Interface method for stopping this service 	*/
	void SoundService::stop(int code)
	{
		alSourceStop(worldSources[0]);
		alutExit();		// Finalize OpenAL
		setStatus(Service::OK);
		setRunning( false );
	}

	void SoundService::runCommand(const std::string &command, const std::string &args)
	{
		if(command == PLAYSOUND)
		{
			alSourcePlay(worldSources[0]);
			int error = alGetError();
			if(error != AL_NO_ERROR)
			{
				LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) << "Error playing sound: " << error << ENDM;
			}

		}
		else if(command == PLAYMUSIC)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << getName() << " I should be playing music" << ENDM;
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
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Loading WAV and stuff" << ENDM;

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

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Testing Platform" << ENDM;

#ifdef _WIN32 //Windows

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Windows Platform found" << ENDM;

#endif

#ifdef _LINUX // Linux

		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << "Linux Platform found" << ENDM;

#endif


	}

	void SoundService::playTestGYPH(void) {
		alSourcePlay(worldSources[0]);
		int error = alGetError();
		if(error != AL_NO_ERROR)
		{
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) 
				<< "Error playing sound: " << error << ENDM;
		}
	}

	void SoundService::playTestGrunt(void) {
	
		std::stringstream gruntPath;
		gruntPath << soundsDirPath << "pig_grunt.wav";
		LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::INFO) << 
			"Loading sound: [" << gruntPath.str() << "]" << ENDM;
		alutLoadWAV(gruntPath.str().c_str(),&data,&format,&size,&bits,&freq);
		
		// Connect WAV to buffer
		alBufferData(worldBuffers[1],format,data,size,freq);
		
		// Play
		alSourcePlay(worldSources[1]);
		
		// Check errors
		int error = alGetError();
		if(error != AL_NO_ERROR)
		{
			char* errorStr = (char*)alGetString(error);
			//std::string errorStr = alGetString(error);
			LoggingService::getInstance()->slog(__FILE__, __LINE__, LoggingService::FAILURE) 
				<< "Error playing sound: " << errorStr << ENDM;
		}
	}
	

} // namespace Ember
